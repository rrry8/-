#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Transition/FadeinOut.h"
#include "SelectScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneManager.h"
#include "../UI/SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "OptionScene.h"
#include "../Common/ModelMng.h"
#include "../Common/AnimationController.h"
#include "../Obj/Gimmick/Scrap.h"
#include "../Obj/Gimmick/Crane.h"
#include "../Common/AnimationController.h"

static constexpr int START_LIMIT_TIME = 210;		//ボタンを押してからの時間

TitleScene::TitleScene()
{
	lpModelMng.GetID("./source/model/Player/player_robot_new.mv1", PLAYER_MAX_NUM);
	lpStage.Init(0);
	lpStage.ReInit();
	Init();


	crane_ = std::make_unique<Crane>();
	
	DrawOwnScreen(0.0);
}


TitleScene::~TitleScene()
{
	DeleteShadowMap(ShadowMapHandle_);
}

bool TitleScene::Init(void)
{
	//Zバッファを有効にする
	SetUseZBuffer3D(true);

	//Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	//バック化リングを有効にする
	SetUseBackCulling(true);

	//カメラのクリップ距離の設定
	SetCameraNearFar(0.0f, 1500.0f);

	//ライトの設定
	ChangeLightTypeDir({ 0.3f,-0.6f,0.8f });

	//3Dの背景色
	SetBackgroundColor(0,0, 0);
	// シャドウマップハンドルの作成
	ShadowMapHandle_ = MakeShadowMap(2048, 2048);

	SetShadowMapLightDirection(ShadowMapHandle_, VGet(0.1f, -0.3f, 0.8f));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(ShadowMapHandle_, VGet(-1000, -1000, -1000), VGet(1000, 1000, 1000));
	rapidxml::file<> moduleFileName = "tmx/TitleObj.tsx";
	TitleVec_ = moduleFileName.vecData();
	TitleDoc.parse<0>(TitleVec_.data());
	AnimationPlayerNode_ = TitleDoc.first_node("AnimationPlayer");

	LoadModel();
	rapidxml::xml_node<>* Createnode = AnimationPlayerNode_->first_node("Create");
	int mass = atoi(Createnode->first_attribute("mass")->value());
	Vector3 pos = Vector3{  static_cast<float>(atof(Createnode->first_attribute("posX")->value())),
							static_cast<float>(atof(Createnode->first_attribute("posY")->value())),
							static_cast<float>(atof(Createnode->first_attribute("posZ")->value())) };
	btVector3 size = btVector3{ atof(Createnode->first_attribute("sizeX")->value()),
								atof(Createnode->first_attribute("sizeY")->value()),
								atof(Createnode->first_attribute("sizeZ")->value()) };
	float Threshold = static_cast<float>(atof(Createnode->first_attribute("Threshold")->value()));
	float collsize = static_cast<float>(atof(Createnode->first_attribute("collsize")->value()));

	rapidxml::xml_node<>* Restraintnode = AnimationPlayerNode_->first_node("Restraint");
	btScalar Friction = btScalar(static_cast<float>(atof(Restraintnode->first_attribute("Friction")->value())));
	btVector3 LinearVec = btVector3(static_cast<float>(atof(Restraintnode->first_attribute("LinearVecX")->value())),
									static_cast<float>(atof(Restraintnode->first_attribute("LinearVecY")->value())),
									static_cast<float>(atof(Restraintnode->first_attribute("LinearVecZ")->value())));
	btVector3 AngularVec = btVector3(	static_cast<float>(atof(Restraintnode->first_attribute("AngularVecX")->value())),
										static_cast<float>(atof(Restraintnode->first_attribute("AngularVecY")->value())),
										static_cast<float>(atof(Restraintnode->first_attribute("AngularVecZ")->value())));
	
	player_.body = lpStage.BulletCreateBox(
		mass,
		pos,
		size,
		Threshold,
		collsize,
		Friction,
		LinearVec,
		AngularVec,
		player_.body);
	MV1SetPosition(player_.handle, { 50.0f,1300.0f,650.0f });

	AnimationPlayerNode2_ = TitleDoc.first_node("AnimationPlayer2");
	Createnode = AnimationPlayerNode2_->first_node("Create");
	mass = atoi(Createnode->first_attribute("mass")->value());
	pos = Vector3{ static_cast<float>(atof(Createnode->first_attribute("posX")->value())),
							static_cast<float>(atof(Createnode->first_attribute("posY")->value())),
							static_cast<float>(atof(Createnode->first_attribute("posZ")->value())) };
	size = btVector3{ atof(Createnode->first_attribute("sizeX")->value()),
								atof(Createnode->first_attribute("sizeY")->value()),
								atof(Createnode->first_attribute("sizeZ")->value()) };
	Threshold = static_cast<float>(atof(Createnode->first_attribute("Threshold")->value()));
	collsize = static_cast<float>(atof(Createnode->first_attribute("collsize")->value()));

	Restraintnode = AnimationPlayerNode2_->first_node("Restraint");
	Friction = btScalar(static_cast<float>(atof(Restraintnode->first_attribute("Friction")->value())));
	LinearVec = btVector3(static_cast<float>(atof(Restraintnode->first_attribute("LinearVecX")->value())),
		static_cast<float>(atof(Restraintnode->first_attribute("LinearVecY")->value())),
		static_cast<float>(atof(Restraintnode->first_attribute("LinearVecZ")->value())));
	AngularVec = btVector3(static_cast<float>(atof(Restraintnode->first_attribute("AngularVecX")->value())),
		static_cast<float>(atof(Restraintnode->first_attribute("AngularVecY")->value())),
		static_cast<float>(atof(Restraintnode->first_attribute("AngularVecZ")->value())));

	player2_.body = lpStage.BulletCreateBox(
		mass,
		pos,
		size,
		Threshold,
		collsize,
		Friction,
		LinearVec,
		AngularVec,
		player2_.body);
	MV1SetPosition(player2_.handle, { 100.0f,1300.0f,650.0f });
	rapidxml::xml_node<>* Animnode = AnimationPlayerNode_->first_node("Anim");
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[0]);
	animController_->Add((int)ANIM_TYPE::RUN, "./source/animation/walk_anim.mv1", static_cast<float>(atof(Animnode->first_attribute("RUN")->value())));
	animController_->Add((int)ANIM_TYPE::IDLE, "./source/animation/idle_anim.mv1", static_cast<float>(atof(Animnode->first_attribute("IDLE")->value())));
	animController_->Add((int)ANIM_TYPE::JUMP, "./source/animation/jump_anim.mv1", static_cast<float>(atof(Animnode->first_attribute("JUMP")->value())));
	animController_->Play((int)ANIM_TYPE::IDLE);
	
	Animnode = AnimationPlayerNode2_->first_node("Anim");
	animController2_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[1]);
	animController2_->Add((int)ANIM_TYPE::RUN, "./source/animation/walk_anim.mv1", static_cast<float>(atof(Animnode->first_attribute("RUN")->value())));
	animController2_->Add((int)ANIM_TYPE::IDLE, "./source/animation/idle_anim.mv1", static_cast<float>(atof(Animnode->first_attribute("IDLE")->value())));
	animController2_->Add((int)ANIM_TYPE::JUMP, "./source/animation/jump_anim.mv1", static_cast<float>(atof(Animnode->first_attribute("JUMP")->value())));
	animController2_->Play((int)ANIM_TYPE::IDLE);
	PlaySoundMem(lpSoundMng.GetID("./source/Sound/selectscene.mp3")[0], DX_PLAYTYPE_LOOP);
	CreateFlag_ = false;

	lpImageMng.GetID("./source/Textures/Player/player_Red_diffuse.png", "player_Red");
	lpImageMng.GetID("./source/Textures/Player/player_Blue_diffuse.png", "player_Blue");


	int pso = LoadPixelShader("./source/shader/ps_Sph.pso");
	int vso = LoadVertexShader("./source/shader/vs.vso");

	texmap.diffuse = LoadGraph("./source/Textures/Player/MergedBake_diffuse.png");
	texmap.norm = LoadGraph("./source/Textures/Player/MergedBake_normal.png");
	texmap.rough = LoadGraph("./source/Textures/Player/MergedBake_roughness.png");
	texmap.metallic = LoadGraph("./source/Textures/Player/MergedBake_metalness.png");
	texmap.shp = LoadGraph("./source/Textures/Player/sph.png");

	return true;
}

uniqueScene TitleScene::Update(double delta, uniqueScene ownScene)
{
	lpStage.g_dynamicsworld->stepSimulation(1 / 60.f, 10);
	cntData_ = lpCntMng.GetController()->GetCntData();
	if (CheckHitKeyAll())
	{
		lpSceneUIMng.SetIsNext(true);
		lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
	}
	if (KeyTrgUp(InputID::Decide))
	{
		CreateFlag_ = true;
		lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");

		switch (lpSceneUIMng.GetHitItem_Main())
		{
		case UI_MAIN_ITEM::SINGLEPLAY:
			player_.isAlive = true;
			/*player2_.isAlive = true;*/
			lpSceneMng.SetMultiFlg(false);
			//lpSceneUIMng.Init(UI_MAIN_ITEM::NONE);
			break;

		case UI_MAIN_ITEM::MULTIPLAY:
			player_.isAlive = true;
			player2_.isAlive = true;
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);

			lpSceneMng.SetMultiFlg(true);
			//lpSceneUIMng.Init(UI_MAIN_ITEM::NONE);
			break;

		case UI_MAIN_ITEM::OPTION:
			return lpSceneUIMng.Init(
				(std::move(std::make_unique<OptionScene>(ownScene))));
			break;

		case UI_MAIN_ITEM::QUIT:
			lpSceneMng.SetEndFlg(true);
			break;

		case UI_MAIN_ITEM::NONE:
			break;
		default:
			break;

		}
	}

	//時間経過でシーン遷移
	if (player_.time > START_LIMIT_TIME)
	{
		lpStage.Remove();
		//音の停止
		return lpSceneUIMng.Init(
			(std::move(std::make_unique<FadeinOut>(
				1.0,
				std::move(ownScene),
				lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(1))))
				))));
	}
	btTransform trans;
	player_.body->getMotionState()->getWorldTransform(trans);
	VECTOR pos = VGet(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));
	MV1SetPosition(player_.handle, { pos.x,pos.y- PLAYER_OFFSET,pos.z });

	player2_.body->getMotionState()->getWorldTransform(trans);
	pos = VGet(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));
	MV1SetPosition(player2_.handle, { pos.x,pos.y- PLAYER_OFFSET,pos.z });
	if (CreateFlag_)
	{
		if (!player_.isAlive)
		{
			if (frame_ % (1000) == 999 && !MoveStartCraneFlag_) {
				MoveStartCraneFlag_ = true;
			}
			crane_->Update(MoveStartCraneFlag_);
		}
		
		CreatePlyaers();
		UpdatePlyaers();
		DestroyPlayers();

		AnimationPlayer(player_, animController_, AnimationPlayerNode_);
		
		AnimationPlayer(player2_, animController2_, AnimationPlayerNode2_);
		
	}

	DrawOwnScreen(delta);

	animController_->Update(lpSceneMng.GetDeltaTime());
	animController2_->Update(lpSceneMng.GetDeltaTime());

	return ownScene;
}

void TitleScene::DrawOwnScreen(double delta)
{
	//シャドウマップへの描画の準備
	ShadowMap_DrawSetup(ShadowMapHandle_);

	for (int e = 1; e < PLAYER_MAX_NUM; e++)
	{
		if (players_[e].isAlive)
		{
			MV1DrawModel(players_[e].handle);
		}
	}

	MV1DrawModel(player_.handle);
	MV1DrawModel(player2_.handle);

	crane_->Draw();

	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, ShadowMapHandle_);

	lpStage.Draw();

	for (int e = 1; e < PLAYER_MAX_NUM; e++)
	{
		if (players_[e].isAlive)
		{
			MV1DrawModel(players_[e].handle);
		}
	}
	MV1DrawModel(player_.handle);
	MV1DrawModel(player2_.handle);
	MV1DrawModel(lpModelMng.GetID("./source/model/Stage/Scrap2.mv1")[0]);

	crane_->Draw();

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
}

bool TitleScene::LoadModel(void)
{
	for (int e = 2; e < PLAYER_MAX_NUM; e++)
	{
		players_[e].handle = lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[e];
		MV1SetPosition(players_[e].handle, { 50,1300,650 });
		MV1SetScale(players_[e].handle, { 0.25,0.25,0.25 });
	}
	player_.handle = lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[0];
	MV1SetScale(player_.handle, { 0.25,0.25,0.25 });
	player_.time = 0;
	player_.isAlive = false;
	
	player2_.handle = lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[1];
	MV1SetScale(player2_.handle, { 0.25,0.25,0.25 });
	player2_.time = 0;
	player2_.isAlive = false;


	MV1SetPosition(lpModelMng.GetID("./source/model/Stage/Scrap2.mv1")[0], {-1200,0,0});
	return true;
}

void TitleScene::CreatePlyaers(void)
{
	frame_++;
	if (!player_.isAlive)
	{
		if (frame_ % (10 + ((rand() % 6) * 10)) == 0) {

			for (int e = 2; e < PLAYER_MAX_NUM; e++)
			{
				if (!players_[e].isAlive)
				{

					players_[e].body = lpStage.BulletCreateBox(
						1,
						{ 45,1300,700 },
						btVector3{ 20,40,20 },
						0.1,
						20,
						btScalar(2),
						btVector3(1, 1, 1),
						btVector3(1, 1, 1),
						players_[e].body);
					players_[e].isAlive = true;
					players_[e].time = 0;
					players_[e].body->setAngularVelocity(btVector3{ btScalar(rand() % 6) ,btScalar(rand() % 6) ,btScalar(rand() % 6) });

					break;
				}
			}

		}
	}
}

void TitleScene::UpdatePlyaers(void)
{
	btTransform trans;
	for (int i = 2; i < PLAYER_MAX_NUM; i++)
	{
		if (players_[i].isAlive)
		{
			auto iadd = i;
			players_[iadd].body->getMotionState()->getWorldTransform(trans);
			// 位置取得
			VECTOR pos = VGet(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));

			players_[iadd].pos_ = Vector3{ pos.x,pos.y,pos.z };
			MV1SetPosition(players_[iadd].handle, players_[iadd].pos_.toVECTOR());

			// 角度取得
			btMatrix3x3 rot = trans.getBasis();
			btVector3 euler;
			rot.getEulerZYX(euler[2], euler[1], euler[0]);

			MV1SetRotationXYZ(players_[iadd].handle, VGet(static_cast<float>(euler.x()), static_cast<float>(euler.y()), static_cast<float>(euler.z())));
			// モデルを描画
			if (player_.isAlive)
			{
				return;
			}
			if (crane_->GetPos().y < 1000 && crane_->GetPos().z >-400)
			{
				if (players_[iadd].time > 500)
				{

					posw_ = btVector3{ trans.getOrigin().x() - crane_->GetPos().x,
						trans.getOrigin().y() - crane_->GetPos().y ,
						trans.getOrigin().z() - crane_->GetPos().z };
					if (-posw_.y() > 200)
					{
						if (-posw_.y() < 300)
						{
							posw_ *= 5;
						}
						if (-posw_.y() < 800)
						{
							posw_ *= 2;
						}
					}
					else
					{
						posw_ *= 11;
					}
					if (-posw_.x() < 900)
					{
						posw_ *= {2, 1, 1};
						players_[iadd].body->setLinearVelocity(-posw_);
					}
				}

			}
			players_[iadd].time += 1;
		}

	}
}

void TitleScene::DestroyPlayers(void)
{
	for (int e = 2; e < PLAYER_MAX_NUM; e++)
	{
		if (players_[e].isAlive)
		{
			if (players_[e].pos_.y < -1000)
			{
				players_[e].isAlive = false;
				if (players_[e].body != nullptr)
				{
					lpStage.CleanBulletObj(players_[e].body);
				}
			}
		}
	}
}

void TitleScene::AnimationPlayer(StackPlayer& player_, std::unique_ptr<AnimationController>& animController_, rapidxml::xml_node<>*node)
{
	rapidxml::xml_node<>* UpPownode = node->first_node("UpPow");
	Vector3 Uppos = Vector3{ static_cast<float>(atof(UpPownode->first_attribute("PowX")->value())),
							static_cast<float>(atof(UpPownode->first_attribute("PowY")->value())),
							static_cast<float>(atof(UpPownode->first_attribute("PowZ")->value())) };
	rapidxml::xml_node<>* DownPownode = node->first_node("DownPow");
	Vector3 Downpos = Vector3{ static_cast<float>(atof(DownPownode->first_attribute("PowX")->value())),
							static_cast<float>(atof(DownPownode->first_attribute("PowY")->value())),
							static_cast<float>(atof(DownPownode->first_attribute("PowZ")->value())) };
	rapidxml::xml_node<>* LeftPownode = node->first_node("LeftPow");
	Vector3 Leftpos = Vector3{ static_cast<float>(atof(LeftPownode->first_attribute("PowX")->value())),
							static_cast<float>(atof(LeftPownode->first_attribute("PowY")->value())),
							static_cast<float>(atof(LeftPownode->first_attribute("PowZ")->value())) };
	rapidxml::xml_node<>*Timenode = node->first_node("Time");
	btTransform trans;
	if (player_.isAlive)
	{
		player_.body->getMotionState()->getWorldTransform(trans);
		VECTOR pos = VGet(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));
		MV1SetPosition(player_.handle, { pos.x,pos.y -40,pos.z });
		if ((player_.time > atoi(Timenode->first_attribute("Time1")->value())) && (player_.time < atoi(Timenode->first_attribute("Time2")->value())))
		{
			animController_->Play((int)ANIM_TYPE::JUMP);
			auto radY = Deg2RadF(-30.0f);
			MV1SetRotationXYZ(player_.handle, { 0, radY, 0 });
			player_.body->applyCentralForce(Uppos.tobtVec());

		}
		if (player_.time < atoi(Timenode->first_attribute("Time3")->value()) && player_.time > atoi(Timenode->first_attribute("Time2")->value()))
		{
			if (player_.body->getInterpolationLinearVelocity().y() > 0)
			{
				player_.body->applyForce(Downpos.tobtVec(), trans.getOrigin());
			}
		}
		if (player_.time > atoi(Timenode->first_attribute("Time4")->value()))
		{
			if (player_.time < atoi(Timenode->first_attribute("Time5")->value()))
			{
				animController_->Play((int)ANIM_TYPE::RUN);
			}
			auto radY = Deg2RadF(-90.0f);
			MV1SetRotationXYZ(player_.handle, { 0, radY, 0 });
			player_.body->applyCentralForce(Leftpos.tobtVec());
		}
		player_.time++;
	}
}
