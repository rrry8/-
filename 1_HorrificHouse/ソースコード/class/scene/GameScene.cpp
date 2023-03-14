#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "../common/manager/CBufferMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/SoundMng.h"
#include "../common/OriginalShader.h"
#include "../common/manager/ModelMng.h"
#include "SceneManager.h"

#include "../camera/Camera.h"
#include "../input/KeyInput.h"
#include "../input/PadInput.h"

#include "SceneExpansion/buffer/GameCBuffer.h"
#include "SceneExpansion/UI/GameUI.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "InventoryScene.h"
#include "GameOverScene.h"
#include "OptionScene.h"
#include "../object/player/Player.h"
#include "../object/enemy/Enemy.h"
#include "../object/Stage.h"

#include "../object/Goal.h"
#include"../object/Item/Item.h"

#include "../../_debug/_DebugConOut.h"
#include "EndingScene.h"
#include "../Transition/FadeinOut.h"
#include "../common/EffekseerEffectHandle.hpp"
#include "../scene/Effect/Effects.h"

constexpr int RANGE = 2000;
constexpr float DEF_SENS = 15.0f; //通常感度
constexpr float LOW_SENS = 30.0f; //低感度

constexpr float EFFECT_SCALE = 70.0f;//埃のエフェクトの大きさ

constexpr float CAMERA_ANGLE = 180.0f;

//被写界深度関係
constexpr float DOF_FOCUS_RANGE = 3000.0f;		//範囲
constexpr float DOF_FOCUS = 3200.0f;			//中心
constexpr float DOF_LERP_DISTANCE = 400.0f;	//補間距離

GameScene::GameScene()
{
	cntType_ = CntType::Key;
	Init();
}
GameScene::GameScene(CntType type)
{
	cntType_ = type;
	Init();
	gameUI_ = std::make_unique<GameUI>(type);
}

GameScene::~GameScene()
{
	Release();
}

void GameScene::Init(void)
{

	if (cntType_ == CntType::Smh)
	{
		//スマホだったら、キーボード入力にする
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//パッドの数が１つ以上の場合はパッド操作にする
		if (GetJoypadNum() > 0)
		{
			controller_ = std::make_unique<PadInput>();
		}
		else
		{
			controller_ = std::make_unique<KeyInput>();
		}
	}

	//グラフィックサイズを確保
	inventoryBackGroundHandle_ = MakeGraph(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);

	//スクリーンを作成
	screenRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
	bloomRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);

	//インスタンス生成
	stage_ = std::make_unique<Stage>();																					//ステージ
	player_ = std::make_unique<Player>(controller_->GetControllerType(), stage_->GetGimmickList(), stage_->GetItemList());	//プレイヤー
	camera_ = std::make_unique<Camera>(Vector3(0.0f, Deg2RadF(CAMERA_ANGLE), 0.0f));												//カメラ
	cBuffer_ = std::make_unique<GameCBuffer>(defPos_ = player_->GetPosition(), defTargetPos_ = camera_->GetTargetPos());		//定数バッファ
	enemy_ = std::make_unique<Enemy>(stage_->GetGimmickList());															//敵

	//深度用シェーダ
	depthShader_ = std::make_unique<OriginalShader>(LoadVertexShader("resource/shader/vs/NormalModel_vs.vso"), LoadPixelShader("resource/shader/ps/3D/Depth_ps.pso"));

	//音
	lpSoundMng.AllStopSound();
	PlaySoundMem(lpSoundMng.GetID("./resource/sound/gameBGM.mp3")[0], DX_PLAYTYPE_LOOP);

	lpSoundMng.SetDefaultVolume();

	//ポストエフェクト
	psPostEffect_ = LoadPixelShader("./resource/shader/ps/HuntedPlayer_ps.pso");
	smartPhoneShader_ = std::make_unique<OriginalShader>(psPostEffect_);
	//埃エフェクトの初期化
	dust_.resource = LoadEffekseerEffect("resource/effect/dust.efkefc", EFFECT_SCALE);
	dust_.playhandle = -1;
	dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
	dust_.pos = player_->GetPosition();
	SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);

	//ブルーム関係初期化
	//横ブラー初期化
	int psBlur = LoadPixelShader("./resource/shader/ps/blur_ps.pso");
	blurX_.Init({ screenSizeX_ / 2, screenSizeY_ }, "./resource/shader/vs/blurX_vs.vso");
	blurX_.origShader = std::make_unique<OriginalShader>(blurX_.vsHandle, psBlur);
	//縦ブラー初期化
	blurY_.Init({ screenSizeX_, screenSizeY_ / 2 }, "./resource/shader/vs/blurY_vs.vso");
	blurY_.origShader = std::make_unique<OriginalShader>(blurY_.vsHandle, psBlur);
	//輝度抽出用
	luminanceShader_ = std::make_unique<OriginalShader>(LoadPixelShader("./resource/shader/ps/Luminance_ps.pso"));
	InitBloomVerts();

	//被写界深度初期化
	depthRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
	blurRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
	highBlurRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
	depthOfFieldShader_ = std::make_unique<OriginalShader>(LoadPixelShader("./resource/shader/ps/DepthOfField_ps.pso"));

	dofRange_ = DOF_FOCUS_RANGE;
	dofFocus = DOF_FOCUS;
	dofLerp_ = DOF_LERP_DISTANCE;
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	UpdateEffekseer3D();
	//コントローラ
	controller_->Update();
	enemy_->GetTargetLine();

	//担当箇所--------------------------------------------------------------
	//被写界深度定数
	//深度範囲
	Vector3 depthRange;
	depthRange.x = dofFocus - dofRange_ / 2.0f - dofLerp_;	//開始位置
	depthRange.y = dofFocus + dofRange_ / 2.0f + dofLerp_;	//終了位置
	depthRange.z = 1.0f / (depthRange.y - depthRange.x);	//範囲の逆数

	//補間範囲とフォーカスが合っている範囲の総距離
	float depthTotalDistance = dofLerp_ * 2.0f + dofRange_;
	//フォーカスが合っている場所
	Vector2 focusPos;
	focusPos.x = dofLerp_ / depthTotalDistance;
	focusPos.y = (dofLerp_ + dofRange_) / depthTotalDistance;
	//(担当箇所)------------------------------------------------------------

	cBuffer_->Update(camera_->GetPos(), camera_->GetTargetPos(), enemy_->GetPosition(), enemy_->GetDirectionVec(), enemy_->GetTargetLine(), depthRange, focusPos);
	//UIの更新
	gameUI_->Update();
	//ステージ
	stage_->Update();
	//カメラ
	camera_->Update(player_->GetDashFlag());
	//プレイヤー
	player_->GetItemID();
	player_->Update(camera_->GetAngle(), camera_->GetTargetPos());
	//敵
	enemy_->Update(player_->GetCapsule(), player_->GetPosition(), player_->CheckHitFlash(camera_->GetTargetPos(),camera_->GetAngle()), player_->GetItemID(), stage_->GetGimmickList());
	//低感度状態がtureだったら
	if (player_->GetLowSensFlg())
	{
		//低感度
		camera_->SetSensitivity(LOW_SENS);
	}
	else
	{
		//通常感度
		camera_->SetSensitivity(DEF_SENS);
	}

	if (abs(dust_.pos.x - camera_->GetPos().x) > RANGE ||
		abs(dust_.pos.z - camera_->GetPos().z) > RANGE)
	{
		//範囲外に入ったら埃エフェクトを更新
		StopEffekseer3DEffect(dust_.playhandle);
		dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
		dust_.pos = camera_->GetPos();
		SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);
	}

	//オプション
	if (controller_->CheckInputKey(KeyID::Pause))
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);

		return std::make_unique<OptionScene>(std::move(scene), GetSceneID());
	}
	//インベントリ
	if (controller_->CheckInputKey(KeyID::Inventory))
	{
		lpSoundMng.PlayingSound("resource/sound/InventoryOpenSE.mp3");
		//初回インベントリ展開時、Tutorialflgをtrue
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Inventry, true);
		//表示画面をインベントリで背景にする
		lpSceneMng.AddInventoryCnt();
		GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, inventoryBackGroundHandle_);
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);

		return std::make_unique<InventoryScene>(std::move(scene), inventoryBackGroundHandle_, controller_->GetControllerType(), player_->GetInventoryList(), player_->GetMinCameraPhotoVector());
	}
	//ゲームオーバー時
	if (enemy_->GetHitPlayerFlag())
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
		return std::make_unique<GameOverScene>(controller_->GetControllerType(), player_->GetPosition(), enemy_->GetPosition(), camera_->GetAngle(), cntType_);

	}
	//ゲームクリア時
	if (player_->GetIsGameClear())
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
		return std::make_unique<FadeinOut>(1.0f, std::move(scene), std::make_unique<EndingScene>(cntType_));
	}


	//デバッグ用
#ifdef _DEBUG
#endif 
	if (CheckHitKey(KEY_INPUT_1) && CheckHitKey(KEY_INPUT_2) && CheckHitKey(KEY_INPUT_3))
	{
		return std::make_unique<ResultScene>(controller_->GetControllerType());
	}

	//描画
	DrawScreen();

	return scene;
}

void GameScene::DrawScreen(void)
{
	//通常描画と深度描画をするためマルチレンダーターゲット
	SetRenderTargetToShader(0, screenRenderTarget_);
	SetRenderTargetToShader(1, depthRenderTarget_);
	ClsDrawScreen();	

	//カメラ処理(オブジェクト描画前に処理)
	camera_->SetBeforeDraw(player_->GetPosition());
	//ステージ
	stage_->Draw(player_->GetminCameraFlag());
	//敵
	enemy_->Draw();

	//描画先リセット
	SetRenderTargetToShader(1, -1);

	//プレイヤー
	player_->SetMinCameraRenderTarget(screenRenderTarget_);

	//輝度抽出とブラー
	DrawBloom(screenRenderTarget_);
	DrawGaussianBlur(screenRenderTarget_, blurRenderTarget_);
	DrawGaussianBlur(blurRenderTarget_, highBlurRenderTarget_);

	SetDrawScreen(screenID_);
	ClsDrawScreen();

	if (enemy_->GetHuntFlag())
	{
		smartPhoneShader_->DrawOnlyPixel(0.0f, 0.0f, screenRenderTarget_, -1, -1, -1);
	}
	else
	{
		//スクリーン描画(被写界深度入り)
		depthOfFieldShader_->DrawOnlyPixel(0, 0, screenRenderTarget_, blurRenderTarget_, highBlurRenderTarget_, depthRenderTarget_);
		//輝度を加算
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, bloomRenderTarget_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	DrawEffekseer3D();
	//プレイヤーの描画
	player_->Draw();
	//UIの描画
	gameUI_->Draw();

#ifdef _DEBUG

	DrawFormatStringF(0, 0, 0xffffff, "ぼかしなし中心(UJ):%f", dofFocus);
	DrawFormatStringF(0, 16, 0xffffff, "ぼかしなし範囲(IK):%f", dofRange_);
	DrawFormatStringF(0, 32, 0xffffff, "補間距離(OL):%f", dofLerp_);

#endif
}

void GameScene::InitBloomVerts(void)
{
	//ブルーム用
	for (auto& v : bloomVerts_)
	{
		v.rhw = 1.0f;
		v.dif = GetColorU8(255, 255, 255, 255);
		v.pos.z = 0.0f;
	}

	bloomVerts_[0].pos.x = 0.0f;
	bloomVerts_[0].pos.y = 0.0f;

	bloomVerts_[1].pos.x = screenSizeX_;
	bloomVerts_[1].pos.y = 0.0f;

	bloomVerts_[2].pos.x = 0.0f;
	bloomVerts_[2].pos.y = screenSizeY_;

	bloomVerts_[3].pos.x = screenSizeX_;
	bloomVerts_[3].pos.y = screenSizeY_;

	bloomVerts_[0].u = 0.0f;
	bloomVerts_[0].v = 0.0f;

	bloomVerts_[1].u = 1.0f;
	bloomVerts_[1].v = 0.0f;

	bloomVerts_[2].u = 0.0f;
	bloomVerts_[2].v = 1.0f;

	bloomVerts_[3].u = 1.0f;
	bloomVerts_[3].v = 1.0f;

	//ブラー
	for (auto& v : blurVerts_)
	{
		v.rhw = 1.0f;
		v.dif = GetColorU8(255, 255, 255, 255);
		v.pos.z = 0.0f;
	}

	blurVerts_[0].pos.x = 0.0f;
	blurVerts_[0].pos.y = 0.0f;

	blurVerts_[1].pos.x = screenSizeX_;
	blurVerts_[1].pos.y = 0.0f;

	blurVerts_[2].pos.x = 0.0f;
	blurVerts_[2].pos.y = screenSizeY_;

	blurVerts_[3].pos.x = screenSizeX_;
	blurVerts_[3].pos.y = screenSizeY_;

	blurVerts_[0].u = 0.0f;
	blurVerts_[0].v = 0.0f;

	blurVerts_[1].u = 1.0f;
	blurVerts_[1].v = 0.0f;

	blurVerts_[2].u = 0.0f;
	blurVerts_[2].v = 1.0f;

	blurVerts_[3].u = 1.0f;
	blurVerts_[3].v = 1.0f;
}

void GameScene::DrawBloom(int renderTarget)
{
	//輝度抽出
	SetDrawScreen(bloomRenderTarget_);
	ClsDrawScreen();
	luminanceShader_->DrawOnlyPixel(0.0f, 0.0f, renderTarget, -1, -1, -1);

	DrawGaussianBlur(bloomRenderTarget_);
}

void GameScene::DrawGaussianBlur(int renderTarget, int writeRenderTarget)
{
	//横ブラー
	SetDrawScreen(blurX_.renderTarget);
	ClsDrawScreen();
	blurX_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurX_.imageSize, renderTarget);

	//縦ブラー
	SetDrawScreen(blurY_.renderTarget);
	ClsDrawScreen();
	blurY_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurY_.imageSize, blurX_.renderTarget);

	//最終描画
	SetDrawScreen(writeRenderTarget);
	ClsDrawScreen();
	DrawPrimitive2D(blurVerts_.data(), blurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);
}

void GameScene::DrawGaussianBlur(int renderTarget)
{
	//横ブラー
	SetDrawScreen(blurX_.renderTarget);
	ClsDrawScreen();
	blurX_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurX_.imageSize, renderTarget);

	//縦ブラー
	SetDrawScreen(blurY_.renderTarget);
	ClsDrawScreen();
	blurY_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurY_.imageSize, blurX_.renderTarget);

	//最終描画
	SetDrawScreen(renderTarget);
	ClsDrawScreen();
	DrawPrimitive2D(blurVerts_.data(), blurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);
}

void GameScene::Release(void)
{
	DeleteGraph(inventoryBackGroundHandle_);
	DeleteGraph(bloomRenderTarget_);
	DeleteGraph(depthRenderTarget_);
	DeleteGraph(highBlurRenderTarget_);
	DeleteGraph(blurRenderTarget_);
	DeleteGraph(blurX_.renderTarget);
	DeleteGraph(blurY_.renderTarget);
}
