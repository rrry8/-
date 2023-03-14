#include <DxLib.h>
#include "../../class/Obj/Stage.h"
#include "../Transition/CrossOver.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "LoadScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "../Input/KeyInput.h"
#include "../UI/SceneUIManager.h"
#include "../Common/AnimationController.h"
#include"../Camera/Camera.h"
#include "TitleScene.h"

constexpr float CAMERA_SPEED = 10.0f;

SelectScene::SelectScene(bool multFlg)
{
	Init();
	lpStage.Init(1);
	lpStage.ReInit();
	DrawOwnScreen(0.0);
}

SelectScene::~SelectScene()
{
	MV1DeleteModel(playerHandle_);
}

bool SelectScene::Init(void)
{
	//カメラのクリップ距離の設定
	SetCameraNearFar(0.0f, 1500.0f);
	//カメラ
	camera_ = std::make_unique<Camera>();
	if (lpSceneUIMng.GetStageNum() == 0)
	{
		stageNum_ = (int)STAGE_NAME::STAGE_1;
		lpSceneUIMng.SetStageNum((int)STAGE_NAME::STAGE_1);
	}
	else
	{
		stageNum_ = lpSceneUIMng.GetStageNum();
	}
	switch (stageNum_)
	{
	case (int)STAGE_NAME::STAGE_1:
		GoalPos_ = { 1000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::ONE;
		break;
	case (int)STAGE_NAME::STAGE_2:
		GoalPos_ = { 2000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::TWO;
		break;
	case (int)STAGE_NAME::STAGE_3:
		GoalPos_ = { 3000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::THREE;
		break;
	case (int)STAGE_NAME::STAGE_4:
		GoalPos_ = { 4000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::FOUR;
		break;
	case (int)STAGE_NAME::STAGE_5:
		GoalPos_ = { 5000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::FIVE;
		break;
	case (int)STAGE_NAME::STAGE_6:
		GoalPos_ = { 6000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::SIX;
		break;
	case (int)STAGE_NAME::STAGE_7:
		GoalPos_ = { 7000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::SEVEN;
		break;
	default:
		stageNum_ = (int)STAGE_NAME::STAGE_1;
		GoalPos_ = { 1000,0,0 };
		cameraPos_.x = GoalPos_.x;
		point_ = Point::ONE;
		break;
	}

	playerHandle_ = MV1LoadModel("./source/model/Player/player_robot_past.mv1");

	MV1SetPosition(playerHandle_, VGet(pos_.x, pos_.y, pos_.z));
	MV1SetScale(playerHandle_, VGet(0.5, 0.5, 0.5));
	MV1SetRotationXYZ(playerHandle_, { 0, -90, 0 });
	animController_ = std::make_unique<AnimationController>(playerHandle_);
	animController_->Add((int)ANIM_TYPE::RUN, "./source/animation/walk_anim.mv1", 30.0f);
	animController_->Add((int)ANIM_TYPE::IDLE, "./source/animation/idle_anim.mv1", 30.0f);
	

	return true;
}


uniqueScene SelectScene::Update(double delta, uniqueScene ownScene)
{
	if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]) == 0)
	{
		PlaySoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0], DX_PLAYTYPE_BACK,true);
	}
	cntData_ = lpCntMng.GetController()->GetCntData();
	
	if (KeyTrgDown(InputID::Cancel))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
		lpStage.Remove();
		//音の停止
		StopSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]);
		return lpSceneUIMng.Init(
			std::move(std::make_unique<CrossOver>(
				0.5,
				std::move(ownScene),
				lpSceneUIMng.Init(std::move(std::make_unique<LoadScene>(0)))
				)));
	}
	
	
	switch (lpSceneUIMng.GetHitItem_Sub())//今カーソルが乗っている項目を取得
	{
	case UI_SUB_ITEM::NEXT:
	{
		if (KeyTrgDown(InputID::Decide))//決定が押された
		{
			lpSoundMng.PlayingSound("./source/Sound/UI移動音.mp3");
			auto radY = Deg2RadF(-90.0f);
			MV1SetRotationXYZ(playerHandle_, { 0, radY, 0 });
			switch (point_)
			{
			case Point::ONE:
				point_ = Point::TWO;
				break;
			case Point::TWO:
				point_ = Point::THREE;
				break;
			case Point::THREE:
				point_ = Point::FOUR;
				break;
			case Point::FOUR:
				point_ = Point::FIVE;
				break;
			case Point::FIVE:
				point_ = Point::SIX;
				break;
			case Point::SIX:
				point_ = Point::SEVEN;
				break;
			case Point::SEVEN:
				point_ = Point::SEVEN;
				break;
			case Point::MAX:
				break;
			default:
				break;
			}
			if (stageNum_ < (int)STAGE_NAME::STAGE_7)
			{
				stageNum_++;
			}
		}
		break;
	}
	case UI_SUB_ITEM::BACK:
	{
		if (KeyTrgDown(InputID::Decide))//決定が押された
		{
			lpSoundMng.PlayingSound("./source/Sound/UI移動音.mp3");
			if (pos_.x >= 1000)
			{
				auto radY = Deg2RadF(90.0f);
				MV1SetRotationXYZ(playerHandle_, { 0, radY, 0 });
			}
			switch (point_)
			{
			case Point::ONE:
				point_ = Point::ONE;
				break;
			case Point::TWO:
				point_ = Point::ONE;
				break;
			case Point::THREE:
				point_ = Point::TWO;
				break;
			case Point::FOUR:
				point_ = Point::THREE;
				break;
			case Point::FIVE:
				point_ = Point::FOUR;
				break;
			case Point::SIX:
				point_ = Point::FIVE;
				break;
			case Point::SEVEN:
				point_ = Point::SIX;
				break;
			case Point::MAX:
				point_ = Point::SIX;
				break;
			default:
				break;
			}
			if (stageNum_ > (int)STAGE_NAME::STAGE_1)
			{
				stageNum_--;
			}
		}
		break;
	}
	case UI_SUB_ITEM::YES://ステージの写真を選択
		if (KeyTrgUp(InputID::Decide))
		{
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]);
			}
			lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
			lpStage.Remove();
			//音の停止
			StopSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]);
			lpSceneUIMng.SetStageNum(stageNum_);
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					0.5,
					std::move(ownScene),
					lpSceneUIMng.Init(std::move(std::make_unique<LoadScene>(stageNum_)))
					)));
		}
		break;
	case UI_SUB_ITEM::NO://タイトルへ戻るを選択
		if (KeyTrgUp(InputID::Decide))
		{
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]);
			}
			lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
			lpStage.Remove();
			//音の停止
			StopSoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0]);
			lpSceneUIMng.SetStageNum(stageNum_);
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					0.5,
					std::move(ownScene),
					lpSceneUIMng.Init(std::move(std::make_unique<LoadScene>(0)))
					)));
		}
		break;
	default:
		break;
	}
	
	
	lpSceneUIMng.SetStageNum(stageNum_);

	//プレイヤーの移動
	MovePlayer();

	//カメラの更新
	UpdateCamera();
	
	//描画
	DrawOwnScreen(delta);
	
	return ownScene;
}

void SelectScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	//カメラ
	camera_->SetBeforeDraw(cameraPos_);

	//マップ
	lpStage.Draw();

	MV1DrawModel(playerHandle_);
}

void SelectScene::MovePlayer()
{
	
	if (KeyTrgDown(InputID::Right))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI移動音.mp3");
		auto radY = Deg2RadF(-90.0f);
		MV1SetRotationXYZ(playerHandle_, { 0, radY, 0 });
		switch (point_)
		{
		case Point::ONE:
			point_ = Point::TWO;
			break;
		case Point::TWO:
			point_ = Point::THREE;
			break;
		case Point::THREE:
			point_ = Point::FOUR;
			break;
		case Point::FOUR:
			point_ = Point::FIVE;
			break;
		case Point::FIVE:
			point_ = Point::SIX;
			break;
		case Point::SIX:
			point_ = Point::SEVEN;
			break;
		case Point::SEVEN:
			point_ = Point::SEVEN;
			break;
		case Point::MAX:
			break;
		default:
			break;
		}
		if (stageNum_ < (int)STAGE_NAME::STAGE_7)
		{
			stageNum_++;
		}

	}
	if (KeyTrgDown(InputID::Left))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI移動音.mp3");
		if (pos_.x >= 1000)
		{
			auto radY = Deg2RadF(90.0f);
			MV1SetRotationXYZ(playerHandle_, { 0, radY, 0 });
		}
		switch (point_)
		{
		case Point::ONE:
			point_ = Point::ONE;
			break;
		case Point::TWO:
			point_ = Point::ONE;
			break;
		case Point::THREE:
			point_ = Point::TWO;
			break;
		case Point::FOUR:
			point_ = Point::THREE;
			break;
		case Point::FIVE:
			point_ = Point::FOUR;
			break;
		case Point::SIX:
			point_ = Point::FIVE;
			break;
		case Point::SEVEN:
			point_ = Point::SIX;
			break;
		case Point::MAX:
			point_ = Point::SEVEN;
			break;
		default:
			break;
		}
		if (stageNum_ > (int)STAGE_NAME::STAGE_1)
		{
			stageNum_--;
		}
	}
	
	GoalPos_.x = 1000.0f + 1000.0f * (float)point_;
	if (pos_.x != GoalPos_.x)
	{
		animController_->Play((int)ANIM_TYPE::RUN);
		int speed = 10;
		if (GoalPos_.x > pos_.x+1000 || GoalPos_.x < pos_.x - 1000)//連打したときにプレイヤーがおいて行かれないように加速させる。
		{
			speed = 70;
		}
		if (pos_.x < GoalPos_.x)
		{
			pos_.x += speed;
		}
		else
		{
			pos_.x -= speed;
		}

	}
	else
	{
		MV1SetRotationXYZ(playerHandle_, { 0, 0, 0 });
		animController_->Play((int)ANIM_TYPE::IDLE);
	}

	if (selectMapOffset_.x != (int)GoalPos_.x-1000)
	{
		int speed = 100;
		if (GoalPos_.x > selectMapOffset_.x + 1000 || GoalPos_.x < selectMapOffset_.x - 1000)//連打したときにプレイヤーがおいて行かれないように加速させる。
		{
			speed = 100;
		}
		if (selectMapOffset_.x < (int)GoalPos_.x-1000)
		{
			selectMapOffset_.x += speed;
		}
		else
		{
			selectMapOffset_.x -= speed;
		}
		lpSceneUIMng.SetSelectMapOffset(-selectMapOffset_);
		lpSceneUIMng.SetSelectMapMoveFlg(true);
	}
	else
	{
		lpSceneUIMng.SetSelectMapMoveFlg(false);
	}

	MV1SetPosition(playerHandle_, VGet(pos_.x, pos_.y, pos_.z));
	//アニメーション
	animController_->Update(lpSceneMng.GetDeltaTime());
}

void SelectScene::UpdateCamera()
{
	if (cameraPos_.x != GoalPos_.x)
	{
		int speed = 100;
		if (GoalPos_.x > cameraPos_.x + 1000 || GoalPos_.x < cameraPos_.x - 1000)//連打したときにプレイヤーがおいて行かれないように加速させる。
		{
			speed = 100;
		}
		if (cameraPos_.x < GoalPos_.x)
		{
			cameraPos_.x += speed;
		}
		else
		{
			cameraPos_.x -= speed;
		}
	}
}
