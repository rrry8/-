#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Transition/CrossOver.h"
#include "ResultScene.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneManager.h"
#include "../UI/SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "SelectScene.h"
#include "../Common/ModelMng.h"
#include "../Common/AnimationController.h"
#include"../Obj/Unit/Item/Coin.h"
#include "TitleScene.h"
#include "LoadScene.h"

ResultScene::ResultScene()
{
	clearTime_ = 180;
	time_.s = (int)clearTime_ % 60;
	time_.s_ten = time_.s / 10;
	time_.s_one = time_.s - time_.s_ten * 10;

	time_.m = (int)clearTime_ / 60;
	time_.m_ten = time_.m / 10;
	time_.m_one = time_.m - time_.m_ten * 10;

	time_.h = time_.m / 60;
	time_.h_ten = time_.h / 10;
	time_.h_one = time_.h - time_.h_ten * 10;
	Init();
	DrawOwnScreen(0.0);
}

ResultScene::ResultScene(CoinList list,double clearTime)
{
	coinList_ = list;
	clearTime_ = clearTime;
	time_.s = (int)clearTime % 60;
	time_.s_ten = time_.s/10;
	time_.s_one = time_.s - time_.s_ten * 10;

	time_.m = (int)clearTime / 60;
	time_.m_ten = time_.m / 10;
	time_.m_one = time_.m - time_.m_ten * 10;

	time_.h = time_.m / 60;
	time_.h_ten = time_.h / 10;
	time_.h_one = time_.h - time_.h_ten * 10;
	Init();
	DrawOwnScreen(0.0);
}

ResultScene::~ResultScene()
{	
}

bool ResultScene::Init(void)
{
	//カメラの作成
	camera_ = std::make_unique<Camera>();

	lpImageMng.GetID("./source/UIImage/string/数字_大.png", "NUM_D", { 84,101 }, { 10,1 });
	
	if (lpSceneMng.GetMultiFlg())//マルチ
	{
		MV1SetPosition(lpModelMng.GetID("./source/model/Player/player_robot_Red.mv1")[0], { 400, 50, 0 });
		MV1SetScale(lpModelMng.GetID("./source/model/Player/player_robot_Red.mv1")[0], { 1,1,1 });
		auto radY = Deg2RadF(0.0f);
		MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/player_robot_Red.mv1")[0], { 0,radY,0 });
		anim_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Player/player_robot_Red.mv1")[0]);
		anim_->Add((int)ANIM_TYPE::JUMP, "./source/animation/jump_anim.mv1", 30.0f);
		anim_->Play((int)ANIM_TYPE::JUMP);

		MV1SetPosition(lpModelMng.GetID("./source/model/Player/player_robot_Blue.mv1")[0], { 600, 150, 450 });
		MV1SetScale(lpModelMng.GetID("./source/model/Player/player_robot_Blue.mv1")[0], { 1,1,1 });
		radY = Deg2RadF(-30.0f);
		MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/player_robot_Blue.mv1")[0], { 0,radY,0 });
		anim2_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Player/player_robot_Blue.mv1")[0]);
		anim2_->Add((int)ANIM_TYPE::IDLE, "./source/animation/idle_anim.mv1", 30.0f);
		anim2_->Add((int)ANIM_TYPE::JUMP, "./source/animation/jump_anim.mv1", 30.0f);
		anim2_->Play((int)ANIM_TYPE::JUMP);
	}
	else//シングル
	{
		MV1SetPosition(lpModelMng.GetID("./source/model/Player/player_robot.mv1")[0], { 400, 50, 0 });
		MV1SetScale(lpModelMng.GetID("./source/model/Player/player_robot.mv1")[0], { 1,1,1 });
		auto radY = Deg2RadF(0.0f);
		MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/player_robot.mv1")[0], { 0,radY,0 });
		anim_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Player/player_robot.mv1")[0]);
		anim_->Add((int)ANIM_TYPE::JUMP, "./source/animation/jump_anim.mv1", 30.0f);
		anim_->Play((int)ANIM_TYPE::JUMP);
	}

	float i = 0.0f;
	for (auto& coin : coinList_)
	{
		coin->ReSet({ 1200.0f + 200.0f * i, 250.0f, 0.0f });
		i += 1.0f;
		//lpSceneUIMng.Data_Save("./source/Data/Coin/CoinData.txt", coin->GetCoinFlg(),lpSceneUIMng.GetStageNum());
		
	}

	Cracker_.resource = LoadEffekseerEffect("./source/effect/cracker.efkefc", 100.0f);
	Cracker_.playhandle = -1;
	Cracker_.pos = /*{0,0,0}*/{ 1920 /3,1080/3,300};
	
	Star_.resource = LoadEffekseerEffect("./source/effect/Stars.efkefc", 100.0f);
	Star_.playhandle = -1;
	Star_.pos = /*{0,0,0}*/{ 1920 / 3,1080 / 3,300 };
	if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/stageclear.mp3")[0]) == 0)
	{
		PlaySoundMem(lpSoundMng.GetID("./source/Sound/stageclear.mp3")[0], DX_PLAYTYPE_BACK, true);
	}

	exRate = 1.0;
	exFlg_ = false;
	return true;
}


uniqueScene ResultScene::Update(double delta, uniqueScene ownScene)
{
	cntData_ = lpCntMng.GetController()->GetCntData();
	if (KeyTrgDown(InputID::Decide))
	{
		switch (lpSceneUIMng.GetHitItem_Main())
		{
		case UI_MAIN_ITEM::TO_SELECT:
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/stageclear.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/stageclear.mp3")[0]);
			}
			lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
			StopEffekseer3DEffect(Cracker_.playhandle);
			StopEffekseer3DEffect(Star_.playhandle);
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					0.5,
					std::move(ownScene),
					lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(1))))
					)));

			TRACE("toSelect\n");
			break;
		case UI_MAIN_ITEM::QUIT:
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/stageclear.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/stageclear.mp3")[0]);
			}
			lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
			TRACE("quit\n");
			StopEffekseer3DEffect(Cracker_.playhandle);
			StopEffekseer3DEffect(Star_.playhandle);
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					0.5,
					std::move(ownScene),
					lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(0))))
					)));
			break;

		default:
			break;
		}

	}
	anim_->Update(delta);
	if (lpSceneMng.GetMultiFlg())
	{
		anim2_->Update(delta);
	}

	for (auto& coin : coinList_)
	{
		coin->Update();
	}
	//if ((effectTime_ / 25) % 8 == 3)
	{
		if (IsEffekseer3DEffectPlaying(Cracker_.playhandle))
		{
			Cracker_.playhandle = PlayEffekseer3DEffect(Cracker_.resource);
			SetPosPlayingEffekseer3DEffect(Cracker_.playhandle, Cracker_.pos.x, Cracker_.pos.y, Cracker_.pos.z);
		}
		if (IsEffekseer3DEffectPlaying(Star_.playhandle))
		{
			Star_.playhandle = PlayEffekseer3DEffect(Star_.resource);
			SetPosPlayingEffekseer3DEffect(Star_.playhandle, Star_.pos.x, Star_.pos.y, Star_.pos.z);
		}
	}		

	//文字拡縮
	if (exFlg_)
	{
		if (exRate < 1.1)
		{
			exRate += 0.01;
		}
		else
		{
			exFlg_ = false;
		}
	}
	else
	{
		if (exRate > 1.0)
		{
			exRate -= 0.01;
		}
		else
		{
			exFlg_ = true;
		}
	}

	UpdateEffekseer3D();
	effectTime_++;
	DrawOwnScreen(delta);

	return ownScene;
}


void ResultScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	camera_->SetBeforeDraw({ 1000,300,500 });

	//背景
	DrawExtendGraph(0, 0, 1920, 1080,lpImageMng.GetID("./source/image/soto2.jpg")[0], true);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
	DrawGraph(-2500, -2500, lpImageMng.GetID("./source/image/light.jpg")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawEffekseer3D();

	//プレイヤー
	if (lpSceneMng.GetMultiFlg())
	{
		MV1DrawModel(lpModelMng.GetID("./source/model/Player/player_robot_Red.mv1")[0]);
		MV1DrawModel(lpModelMng.GetID("./source/model/Player/player_robot_Blue.mv1")[0]);
	}
	else
	{
		MV1DrawModel(lpModelMng.GetID("./source/model/Player/player_robot.mv1")[0]);
	}

	//コイン
	for (auto& coin : coinList_)
	{
		coin->Draw();
	}
	
	//文字の後ろにある背景
	DrawExtendGraph(1100, 460, 1600, 600, lpImageMng.GetID("./source/UIImage/string/mojihaikei.png")[0], true);
	//時間
	DrawRotaGraph(1520, 530, 1*exRate, 0, lpImageMng.GetID("NUM_D")[time_.s_one], true, false, false);
	DrawRotaGraph(1456, 530, 1 * exRate, 0, lpImageMng.GetID("NUM_D")[time_.s_ten], true, false, false);
	DrawRotaGraph(1423, 500, 1.5 * exRate, 0, lpImageMng.GetID("./source/UIImage/string/SELECT/timeの区切り.png")[0], true);
	DrawRotaGraph(1392, 530, 1 * exRate, 0, lpImageMng.GetID("NUM_D")[time_.m_one], true, false, false);
	DrawRotaGraph(1328, 530, 1 * exRate, 0, lpImageMng.GetID("NUM_D")[time_.m_ten], true, false, false);
	DrawRotaGraph(1295, 500, 1.5 * exRate, 0, lpImageMng.GetID("./source/UIImage/string/SELECT/timeの区切り.png")[0], true);
	DrawRotaGraph(1264, 530, 1 * exRate, 0, lpImageMng.GetID("NUM_D")[time_.h_one], true, false, false);
	DrawRotaGraph(1200, 530, 1 * exRate, 0, lpImageMng.GetID("NUM_D")[time_.h_ten], true, false, false);
}
