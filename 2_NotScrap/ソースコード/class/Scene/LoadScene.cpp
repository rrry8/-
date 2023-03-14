#include "LoadScene.h"
#include"../Obj/Stage.h"
#include "../UI/SceneUIManager.h"
#include "../Transition/CrossOver.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Common/SoundMng.h"
#include "../Common/ImageMng.h"
#include "../Common/MovieMng.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "ResultScene.h"

constexpr float ENEMY_BULLET_MAX_NUM = 32;

LoadScene::LoadScene()
{
	stageNum = 8;
	tutorialNum_ = 8;
	Init(8);
	DrawOwnScreen(0.0);
}

LoadScene::LoadScene(int num)//(仮）numにステージ番号入れるといい。TITLE＝０、SELECT＝１、それ以降はGAMESCENE　時間あるときENUMに変える予定
{
	stageNum = num;
	tutorialNum_ = 8;
	Init(num);
	DrawOwnScreen(0.0);
}

LoadScene::~LoadScene()
{
}

bool LoadScene::Init(void)
{
	return true;
}

bool LoadScene::Init(int num)
{
	SetUseASyncLoadFlag(true);
	//ステージ
	lpStage.Init(stageNum);
	//タイトルの看板
	lpModelMng.GetID("./source/model/UI/TitleSignandarm.mv1");
	//プレイヤー
	lpModelMng.GetID("./source/model/Player/player_robot.mv1");
	if (lpSceneMng.GetMultiFlg())
	{
		lpModelMng.GetID("./source/model/Player/player_robot_Red.mv1");
		lpModelMng.GetID("./source/model/Player/player_robot_Blue.mv1");
	}
	//SE・BGM
	lpSoundMng.GetID("./source/Sound/walk_trim.mp3");
	lpSoundMng.GetID("./source/Sound/coinget.mp3");
	lpSoundMng.GetID("./source/Sound/飛来02.mp3");
	lpSoundMng.GetID("./source/Sound/ロボットを強く殴る3.mp3");
	lpSoundMng.GetID("./source/Sound/EnemyDeath.mp3");
	lpSoundMng.GetID("./source/Sound/machikouba.mp3");
	lpSoundMng.GetID("./source/Sound/streetofrage.mp3");
	lpSoundMng.GetID("./source/Sound/selectscene.mp3");
	lpSoundMng.GetID("./source/Sound/stageclear.mp3");
	lpSoundMng.GetID("./source/Sound/explosion.mp3");
	
	lpImageMng.GetID("./source/image/mouse8.png", "mouse", Vector2Int{ 500,500 }, Vector2Int{ 8,1 });
	lpImageMng.GetID("./source/image/mouse8B.png", "mouseB", Vector2Int{ 500,500 }, Vector2Int{ 8,1 });
	lpImageMng.GetID("./source/image/key.png", "key", Vector2Int{ 72,72 }, Vector2Int{ 3,1 });
	lpImageMng.GetID("./source/image/ray.png", "ray", Vector2Int{ 32,32 }, Vector2Int{ 30,1 });
	lpImageMng.GetID("./source/image/Shift.png", "Shift");

	lpImageMng.GetID("./source/image/key1.png", "key1");
	lpImageMng.GetID("./source/image/key2.png", "key2");
	lpImageMng.GetID("./source/image/key3.png", "key3");
	lpImageMng.GetID("./source/image/Nmouse.png", "Nmouse");
	lpImageMng.GetID("./source/image/Smouse.png", "Smouse");
	lpImageMng.GetID("./source/image/mouse.png", "NONmouse");
	lpImageMng.GetID("./source/image/spaceM.png", "shiftM");
	lpImageMng.GetID("./source/image/spaceMN.png", "shiftMN");
	lpImageMng.GetID("./source/image/R.png", "R");
	lpImageMng.GetID("./source/image/RR.png", "RR");

	lpImageMng.GetID("./source/image/S.png", "S");
	lpImageMng.GetID("./source/image/N.png", "N");
	lpImageMng.GetID("./source/image/NON.png", "NONtex");

	lpMovieMng.GetID("./source/movie/move.mp4","move");
	lpMovieMng.GetID("./source/movie/magWN.mp4", "magWN");
	lpMovieMng.GetID("./source/movie/magWS.mp4", "magWS");
	lpMovieMng.GetID("./source/movie/magSS.mp4", "magSS");
	lpMovieMng.GetID("./source/movie/magbox.mp4", "magbox");
	lpMovieMng.GetID("./source/movie/Move_left_and_right.mp4", "move");
	lpMovieMng.GetID("./source/movie/bullet.mp4", "bullet");
	lpMovieMng.GetID("./source/movie/death.mp4", "death");
	lpMovieMng.GetID("./source/movie/stick.mp4", "stick");

	//UI
	lpImageMng.GetID("./source/UIImage/string/数字.png", "NUM", { 42,54 }, { 10,1 });

	if (stageNum == 1)
	{
		lpModelMng.GetID("./source/model/Player/player_robot_new.mv1", 50);
		lpModelMng.GetID("./source/Model/Stage/MidPoint.mv1",2);
		lpModelMng.GetID("./source/model/Coin/Coin.mv1", 3);
		lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1", 3);
	}
	if (stageNum == 99)
	{
		SetUseASyncLoadFlag(false);
		lpStage.ReInit();
		SetUseASyncLoadFlag(true);
		//コイン

		if (0 != lpStage.GetCoinList().size())
		{
			lpModelMng.GetID("./source/model/Coin/Coin.mv1", static_cast<int>(lpStage.GetCoinList().size()));
			lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1", static_cast<int>(lpStage.GetCoinList().size()));
		}
		//プレイヤー
		lpModelMng.GetID("./source/model/Player/nonarm.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_ArmL.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_ArmR.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_Basket.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_Body.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_FootL.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_FootR.mv1", 2);
		lpModelMng.GetID("./source/model/Player/player_Head.mv1", 2);

		lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1",2);

		lpModelMng.GetID("./source/model/Player/Stick.mv1",2);

		lpModelMng.GetID("./source/model/Player/armonly_new.mv1",2);

		if (lpSceneMng.GetMultiFlg())
		{
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_ArmL.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false); 
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_ArmR.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_Basket.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_Body.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_FootL.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_FootR.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_Head.mv1")[0], 0, lpImageMng.GetID("player_Red")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_ArmL.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_ArmR.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_Basket.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_Body.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_FootL.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_FootR.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
			MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/player_Head.mv1")[1], 0, lpImageMng.GetID("player_Blue")[0], false);
		}

		if (0 != lpStage.GetEnemyList().size())
		{
			lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1", static_cast<int>(lpStage.GetEnemyList().size()));
		}
		if (0 != lpStage.GetShootEnemyList().size())
		{
			lpModelMng.GetID("./source/Model/Enemy/shoot_Enemy.mv1", static_cast<int>(lpStage.GetShootEnemyList().size()));
			lpModelMng.GetID("./source/Model/Enemy/bullet.mv1", ENEMY_BULLET_MAX_NUM);
		}
		if (0 != lpStage.GetDoorList().size())
		{
			lpModelMng.GetID("./source/Model/Gimmick/Door.mv1", static_cast<int>(lpStage.GetDoorList().size()) + tutorialNum_);
			lpModelMng.GetID("./source/Model/Gimmick/Button.mv1", static_cast<int>(lpStage.GetDoorList().size()));
			lpModelMng.GetID("./source/Model/Gimmick/OnButton.mv1", static_cast<int>(lpStage.GetDoorList().size()));
		}
		if (0 != lpStage.GetAccessoryList().size())
		{
			lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1", static_cast<int>(lpStage.GetAccessoryList().size()));
		}
		if (0 != lpStage.GetNMagBoxCollList().size())
		{
			lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1", static_cast<int>(lpStage.GetNMagBoxCollList().size()));
		}
		if (0 != lpStage.GetSMagBoxCollList().size())
		{
			lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1", static_cast<int>(lpStage.GetSMagBoxCollList().size()));
		}
		if (0 != lpStage.GetGearList().size())
		{
			lpModelMng.GetID("./source/Model/Gimmick/Gear.mv1", 10);
		}
		if (0 != lpStage.GetMidPointList().size())
		{
			lpModelMng.GetID("./source/Model/Stage/MidPoint.mv1", static_cast<int>(lpStage.GetMidPointList().size()));
		}
		if (0 != lpStage.GetWarningSignList().size())
		{
			lpModelMng.GetID("./source/Model/Gimmick/sign.mv1", static_cast<int>(lpStage.GetWarningSignList().size()));
		}
		if (0 != lpStage.GetFloorList().size())
		{
			lpModelMng.GetID("./source/Model/Stage/Floor_Tile.mv1" ,static_cast<int>(lpStage.GetFloorList().size()));
			lpModelMng.GetID("./source/Model/Gimmick/Button.mv1", static_cast<int>(lpStage.GetDoorList().size()));
			lpModelMng.GetID("./source/Model/Gimmick/OnButton.mv1", static_cast<int>(lpStage.GetDoorList().size()));
		}
		//チュートリアルの看板
		lpModelMng.GetID("./source/Model/Stage/signbot_new.mv1", tutorialNum_);
		lpModelMng.GetID("./source/Model/Stage/signbot2_new.mv1", tutorialNum_);
	}

	SetUseASyncLoadFlag(false);
	return true;
}

uniqueScene LoadScene::Update(double delta, uniqueScene ownScene)
{
	DrawOwnScreen(delta);

	if (GetASyncLoadNum() == 0)
	{
		TRACE("非同期読み込み終了\n");
		switch (stageNum)
		{
		case 0:
			if (lpSceneUIMng.GetLoadingEndFlg())
			{
				return	lpSceneUIMng.Init((std::move(std::make_unique<TitleScene>()))); // 実体
			}
			break;
		case 1:
			if (lpSceneUIMng.GetLoadingEndFlg())
			{
				return	lpSceneUIMng.Init((std::move(std::make_unique<SelectScene>(false)))); // 実体
			}
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			if (lpSceneUIMng.GetLoadingEndFlg())
			{
				return lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(99))));			
			}
			break;
		case 99:
			if (lpSceneUIMng.GetLoadingEndFlg())
			{
				return lpSceneUIMng.Init(lpSceneUIMng.Init(std::move(std::make_unique<GameScene>())));
			}
			break;
		default:
			break;
		}
		
	}
	return ownScene;
}

void LoadScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawBox(0, 0, screenSize_.x, screenSize_.y, 0xaaaaaa, true);

	if (stageNum != 99)
	{
		DrawGraph(screenSize_.x / 2 - 378 / 2, screenSize_.y / 2 - 49 / 2, lpImageMng.GetID("./source/UIImage/string/LOAD/モデル情報読み込み中.png")[0], true);
	}
	else
	{
		DrawGraph(screenSize_.x / 2 - 258 / 2, screenSize_.y / 2 - 49 / 2, lpImageMng.GetID("./source/UIImage/string/LOAD/モデルの配置中.png")[0], true);
	}
}
