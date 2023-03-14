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
constexpr float DEF_SENS = 15.0f; //�ʏ튴�x
constexpr float LOW_SENS = 30.0f; //�ኴ�x

constexpr float EFFECT_SCALE = 70.0f;//���̃G�t�F�N�g�̑傫��

constexpr float CAMERA_ANGLE = 180.0f;

//��ʊE�[�x�֌W
constexpr float DOF_FOCUS_RANGE = 3000.0f;		//�͈�
constexpr float DOF_FOCUS = 3200.0f;			//���S
constexpr float DOF_LERP_DISTANCE = 400.0f;	//��ԋ���

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
		//�X�}�z��������A�L�[�{�[�h���͂ɂ���
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//�p�b�h�̐����P�ȏ�̏ꍇ�̓p�b�h����ɂ���
		if (GetJoypadNum() > 0)
		{
			controller_ = std::make_unique<PadInput>();
		}
		else
		{
			controller_ = std::make_unique<KeyInput>();
		}
	}

	//�O���t�B�b�N�T�C�Y���m��
	inventoryBackGroundHandle_ = MakeGraph(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);

	//�X�N���[�����쐬
	screenRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
	bloomRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);

	//�C���X�^���X����
	stage_ = std::make_unique<Stage>();																					//�X�e�[�W
	player_ = std::make_unique<Player>(controller_->GetControllerType(), stage_->GetGimmickList(), stage_->GetItemList());	//�v���C���[
	camera_ = std::make_unique<Camera>(Vector3(0.0f, Deg2RadF(CAMERA_ANGLE), 0.0f));												//�J����
	cBuffer_ = std::make_unique<GameCBuffer>(defPos_ = player_->GetPosition(), defTargetPos_ = camera_->GetTargetPos());		//�萔�o�b�t�@
	enemy_ = std::make_unique<Enemy>(stage_->GetGimmickList());															//�G

	//�[�x�p�V�F�[�_
	depthShader_ = std::make_unique<OriginalShader>(LoadVertexShader("resource/shader/vs/NormalModel_vs.vso"), LoadPixelShader("resource/shader/ps/3D/Depth_ps.pso"));

	//��
	lpSoundMng.AllStopSound();
	PlaySoundMem(lpSoundMng.GetID("./resource/sound/gameBGM.mp3")[0], DX_PLAYTYPE_LOOP);

	lpSoundMng.SetDefaultVolume();

	//�|�X�g�G�t�F�N�g
	psPostEffect_ = LoadPixelShader("./resource/shader/ps/HuntedPlayer_ps.pso");
	smartPhoneShader_ = std::make_unique<OriginalShader>(psPostEffect_);
	//���G�t�F�N�g�̏�����
	dust_.resource = LoadEffekseerEffect("resource/effect/dust.efkefc", EFFECT_SCALE);
	dust_.playhandle = -1;
	dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
	dust_.pos = player_->GetPosition();
	SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);

	//�u���[���֌W������
	//���u���[������
	int psBlur = LoadPixelShader("./resource/shader/ps/blur_ps.pso");
	blurX_.Init({ screenSizeX_ / 2, screenSizeY_ }, "./resource/shader/vs/blurX_vs.vso");
	blurX_.origShader = std::make_unique<OriginalShader>(blurX_.vsHandle, psBlur);
	//�c�u���[������
	blurY_.Init({ screenSizeX_, screenSizeY_ / 2 }, "./resource/shader/vs/blurY_vs.vso");
	blurY_.origShader = std::make_unique<OriginalShader>(blurY_.vsHandle, psBlur);
	//�P�x���o�p
	luminanceShader_ = std::make_unique<OriginalShader>(LoadPixelShader("./resource/shader/ps/Luminance_ps.pso"));
	InitBloomVerts();

	//��ʊE�[�x������
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
	//�R���g���[��
	controller_->Update();
	enemy_->GetTargetLine();

	//�S���ӏ�--------------------------------------------------------------
	//��ʊE�[�x�萔
	//�[�x�͈�
	Vector3 depthRange;
	depthRange.x = dofFocus - dofRange_ / 2.0f - dofLerp_;	//�J�n�ʒu
	depthRange.y = dofFocus + dofRange_ / 2.0f + dofLerp_;	//�I���ʒu
	depthRange.z = 1.0f / (depthRange.y - depthRange.x);	//�͈͂̋t��

	//��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂̑�����
	float depthTotalDistance = dofLerp_ * 2.0f + dofRange_;
	//�t�H�[�J�X�������Ă���ꏊ
	Vector2 focusPos;
	focusPos.x = dofLerp_ / depthTotalDistance;
	focusPos.y = (dofLerp_ + dofRange_) / depthTotalDistance;
	//(�S���ӏ�)------------------------------------------------------------

	cBuffer_->Update(camera_->GetPos(), camera_->GetTargetPos(), enemy_->GetPosition(), enemy_->GetDirectionVec(), enemy_->GetTargetLine(), depthRange, focusPos);
	//UI�̍X�V
	gameUI_->Update();
	//�X�e�[�W
	stage_->Update();
	//�J����
	camera_->Update(player_->GetDashFlag());
	//�v���C���[
	player_->GetItemID();
	player_->Update(camera_->GetAngle(), camera_->GetTargetPos());
	//�G
	enemy_->Update(player_->GetCapsule(), player_->GetPosition(), player_->CheckHitFlash(camera_->GetTargetPos(),camera_->GetAngle()), player_->GetItemID(), stage_->GetGimmickList());
	//�ኴ�x��Ԃ�ture��������
	if (player_->GetLowSensFlg())
	{
		//�ኴ�x
		camera_->SetSensitivity(LOW_SENS);
	}
	else
	{
		//�ʏ튴�x
		camera_->SetSensitivity(DEF_SENS);
	}

	if (abs(dust_.pos.x - camera_->GetPos().x) > RANGE ||
		abs(dust_.pos.z - camera_->GetPos().z) > RANGE)
	{
		//�͈͊O�ɓ������皺�G�t�F�N�g���X�V
		StopEffekseer3DEffect(dust_.playhandle);
		dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
		dust_.pos = camera_->GetPos();
		SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);
	}

	//�I�v�V����
	if (controller_->CheckInputKey(KeyID::Pause))
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);

		return std::make_unique<OptionScene>(std::move(scene), GetSceneID());
	}
	//�C���x���g��
	if (controller_->CheckInputKey(KeyID::Inventory))
	{
		lpSoundMng.PlayingSound("resource/sound/InventoryOpenSE.mp3");
		//����C���x���g���W�J���ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Inventry, true);
		//�\����ʂ��C���x���g���Ŕw�i�ɂ���
		lpSceneMng.AddInventoryCnt();
		GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, inventoryBackGroundHandle_);
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);

		return std::make_unique<InventoryScene>(std::move(scene), inventoryBackGroundHandle_, controller_->GetControllerType(), player_->GetInventoryList(), player_->GetMinCameraPhotoVector());
	}
	//�Q�[���I�[�o�[��
	if (enemy_->GetHitPlayerFlag())
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
		return std::make_unique<GameOverScene>(controller_->GetControllerType(), player_->GetPosition(), enemy_->GetPosition(), camera_->GetAngle(), cntType_);

	}
	//�Q�[���N���A��
	if (player_->GetIsGameClear())
	{
		StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
		return std::make_unique<FadeinOut>(1.0f, std::move(scene), std::make_unique<EndingScene>(cntType_));
	}


	//�f�o�b�O�p
#ifdef _DEBUG
#endif 
	if (CheckHitKey(KEY_INPUT_1) && CheckHitKey(KEY_INPUT_2) && CheckHitKey(KEY_INPUT_3))
	{
		return std::make_unique<ResultScene>(controller_->GetControllerType());
	}

	//�`��
	DrawScreen();

	return scene;
}

void GameScene::DrawScreen(void)
{
	//�ʏ�`��Ɛ[�x�`������邽�߃}���`�����_�[�^�[�Q�b�g
	SetRenderTargetToShader(0, screenRenderTarget_);
	SetRenderTargetToShader(1, depthRenderTarget_);
	ClsDrawScreen();	

	//�J��������(�I�u�W�F�N�g�`��O�ɏ���)
	camera_->SetBeforeDraw(player_->GetPosition());
	//�X�e�[�W
	stage_->Draw(player_->GetminCameraFlag());
	//�G
	enemy_->Draw();

	//�`��惊�Z�b�g
	SetRenderTargetToShader(1, -1);

	//�v���C���[
	player_->SetMinCameraRenderTarget(screenRenderTarget_);

	//�P�x���o�ƃu���[
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
		//�X�N���[���`��(��ʊE�[�x����)
		depthOfFieldShader_->DrawOnlyPixel(0, 0, screenRenderTarget_, blurRenderTarget_, highBlurRenderTarget_, depthRenderTarget_);
		//�P�x�����Z
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, bloomRenderTarget_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	DrawEffekseer3D();
	//�v���C���[�̕`��
	player_->Draw();
	//UI�̕`��
	gameUI_->Draw();

#ifdef _DEBUG

	DrawFormatStringF(0, 0, 0xffffff, "�ڂ����Ȃ����S(UJ):%f", dofFocus);
	DrawFormatStringF(0, 16, 0xffffff, "�ڂ����Ȃ��͈�(IK):%f", dofRange_);
	DrawFormatStringF(0, 32, 0xffffff, "��ԋ���(OL):%f", dofLerp_);

#endif
}

void GameScene::InitBloomVerts(void)
{
	//�u���[���p
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

	//�u���[
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
	//�P�x���o
	SetDrawScreen(bloomRenderTarget_);
	ClsDrawScreen();
	luminanceShader_->DrawOnlyPixel(0.0f, 0.0f, renderTarget, -1, -1, -1);

	DrawGaussianBlur(bloomRenderTarget_);
}

void GameScene::DrawGaussianBlur(int renderTarget, int writeRenderTarget)
{
	//���u���[
	SetDrawScreen(blurX_.renderTarget);
	ClsDrawScreen();
	blurX_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurX_.imageSize, renderTarget);

	//�c�u���[
	SetDrawScreen(blurY_.renderTarget);
	ClsDrawScreen();
	blurY_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurY_.imageSize, blurX_.renderTarget);

	//�ŏI�`��
	SetDrawScreen(writeRenderTarget);
	ClsDrawScreen();
	DrawPrimitive2D(blurVerts_.data(), blurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);
}

void GameScene::DrawGaussianBlur(int renderTarget)
{
	//���u���[
	SetDrawScreen(blurX_.renderTarget);
	ClsDrawScreen();
	blurX_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurX_.imageSize, renderTarget);

	//�c�u���[
	SetDrawScreen(blurY_.renderTarget);
	ClsDrawScreen();
	blurY_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurY_.imageSize, blurX_.renderTarget);

	//�ŏI�`��
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
