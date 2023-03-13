#include <DxLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Scene/SelectScene.h"
#include "UISelect.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../Obj/CheckHit.h"
#include "../Input/ControllerMng.h"

UISelect::UISelect()
{
	Init();
	DrawOwnScreen(0.0);
}


UISelect::~UISelect()
{
}

bool UISelect::Init(void)
{
	lpImageMng.GetID("./source/UIImage/string/数字_大.png", "NUM_D", { 84,101 }, { 10,1 });
	tmxObj_.LoadTmx("././tmx/selectUI.tmx");
	//camera_ = std::make_unique<Camera>(1920.0f, 1152.0f);//std::make_unique<Camera>(worldsize.x, worldsize.y);
	
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MouseCursormodule");
	
	uiEffect_.offset_.x = 0;
	uiEffect_.offset_.y = 0;
	uiEffect_.move_L = true;
	uiEffect_.move_R = true;

	for (int i = 0; i < ITEMNUM; i++)
	{
		uiEffect_.extendVol_[i] = 0;
		uiEffect_.extend_[i] = true;
	}
	for (int i = 0; i < COIN_MAX; i++)
	{
		MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[i], { 930 + 50 * float(i) + 1000 * (lpSceneUIMng.GetStageNum() - 2), 464, 0 });
		MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[i], { 930 + 50 * float(i) + 1000 * (lpSceneUIMng.GetStageNum() - 2), 464, 0 });
		MV1SetScale(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[i], { 0.5,0.5,0.5 });
		MV1SetScale(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[i], { 0.5,0.5,0.5 });
	}
	return true;
}

void UISelect::Update(double delta)
{
	cntData_ = lpCntMng.GetController()->GetCntData();
	auto node = stateNode_->first_node("module");
	slideOffset_x = lpSceneUIMng.GetSelectMapOffset().x;
	GetMousePoint(&MousePos.x, &MousePos.y);
	if (lpCntMng.GetController()->GetCntType() == CntType::Key)
	{
		MousePos.x += 32;//画像ずれてる分補正
		MousePos.y += 32;
	}
	int i = (int)UI_SUB_ITEM::NEXT;
	for (auto& coll : tmxObj_.GetSubItemCollList())
	{
		if (CheckCollM()(raycast_, MousePos, node, coll))
		{
			
			//TRACE("当たった！\n");
			lpSceneUIMng.SetHitItem_Sub((UI_SUB_ITEM)i);

			
			break;
		}
		else
		{
			lpSceneUIMng.SetHitItem_Sub(UI_SUB_ITEM::NONE);
		}
		i++;
	}
	TimeUpdate(lpSceneUIMng.GetStageNum());
	CoinStateUpdate(lpSceneUIMng.GetStageNum());
	OffsetUpdate();
	DrawOwnScreen(delta);
}


void UISelect::DrawOwnScreen(double delta)
{
	for (int i = 0; i < (int)STAGE_NAME::MAX - 2; i++)
	{
		string No = std::to_string(i+1);
		DrawModiGraph(
			screenSize_.x / 2 - 480 + uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - 480 + uiEffect_.extendVol_[2] / 4,
			screenSize_.x / 2 + 480 - uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - 480 + uiEffect_.extendVol_[2] / 4,
			screenSize_.x / 2 + 480 - uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - uiEffect_.extendVol_[2] / 4,
			screenSize_.x / 2 - 480 + uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - uiEffect_.extendVol_[2] / 4,
			lpImageMng.GetID("./source/image/stage" + No + ".png")[0], true);
		

		//image画像の上の薄い四角
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(
			480 + uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i),380,
			1440 - uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i),540 - uiEffect_.extendVol_[2] / 4,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		
		DrawGraph(480 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - 490, lpImageMng.GetID("./source/UIImage/string/SELECT/ステージ.png")[0], true);

		DrawGraph(480 + slideOffset_x * 2 + (2000 * i), 380, lpImageMng.GetID("./source/UIImage/string/SELECT/成績.png")[0], true);
		//DrawFormatString(480 + slideOffset_x * 2 + (2000 * i), 380, 0xffffff, "%d", clearTime_);
		DrawGraph(screenSize_.x / 2 - 848 / 2, screenSize_.y - 80, lpImageMng.GetID("./source/UIImage/string/SELECT/ヒント.png")[0], true);

		DrawGraph(480 + 180 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - 488, lpImageMng.GetID("NUM")[i + 1], true);//ステージの番号
		


		//茶色の枠
		DrawModiGraph(
			screenSize_.x / 2 - 490 + uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - 490 + uiEffect_.extendVol_[2] / 4,
			screenSize_.x / 2 + 490 - uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 - 490 + uiEffect_.extendVol_[2] / 4,
			screenSize_.x / 2 + 490 - uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 +10 - uiEffect_.extendVol_[2] / 4,
			screenSize_.x / 2 - 490 + uiEffect_.extendVol_[2] / 4 + slideOffset_x * 2 + (2000 * i), screenSize_.y / 2 +10 - uiEffect_.extendVol_[2] / 4,
			lpImageMng.GetID("./source/UIImage/selectBox.png")[0], true);

	}
	
	if (!lpSceneUIMng.GetSelectMapMoveFlg())
	{
		if (lpSceneUIMng.GetStageNum() != (int)STAGE_NAME::MAX - 1)//選択されたステージが最後だったら表示しない
		{

			if (cntData_[InputID::Right][static_cast<int>(Trg::Now)] || (cntData_[InputID::Decide][static_cast<int>(Trg::Now)] && lpSceneUIMng.GetHitItem_Sub() == UI_SUB_ITEM::NEXT))
			{
				//赤くなる
				DrawModiGraph(
					1490 + uiEffect_.offset_.y + uiEffect_.extendVol_[1] / 4, 220 + uiEffect_.extendVol_[1] / 4,
					1650 + uiEffect_.offset_.y - uiEffect_.extendVol_[1] / 4, 220 + uiEffect_.extendVol_[1] / 4,
					1650 + uiEffect_.offset_.y - uiEffect_.extendVol_[1] / 4, 380 - uiEffect_.extendVol_[1] / 4,
					1490 + uiEffect_.offset_.y + uiEffect_.extendVol_[1] / 4, 380 - uiEffect_.extendVol_[1] / 4,
					lpImageMng.GetID("./source/UIImage/Square Buttons/Colored Square Buttons/Next col_Square Button.png")[0], true);
			}
			else
			{
				DrawModiGraph(
					1490 + uiEffect_.offset_.y + uiEffect_.extendVol_[1] / 4, 220 + uiEffect_.extendVol_[1] / 4,
					1650 + uiEffect_.offset_.y - uiEffect_.extendVol_[1] / 4, 220 + uiEffect_.extendVol_[1] / 4,
					1650 + uiEffect_.offset_.y - uiEffect_.extendVol_[1] / 4, 380 - uiEffect_.extendVol_[1] / 4,
					1490 + uiEffect_.offset_.y + uiEffect_.extendVol_[1] / 4, 380 - uiEffect_.extendVol_[1] / 4,
					lpImageMng.GetID("./source/UIImage/Square Buttons/Square Buttons/Next Square Button.png")[0], true);
			}
		}

		if (lpSceneUIMng.GetStageNum() != (int)STAGE_NAME::STAGE_1)//選択されたステージが最初だったら表示しない
		{

			if (cntData_[InputID::Left][static_cast<int>(Trg::Now)] || (cntData_[InputID::Decide][static_cast<int>(Trg::Now)] && lpSceneUIMng.GetHitItem_Sub() == UI_SUB_ITEM::BACK))
			{
				DrawModiGraph(
					270 + uiEffect_.offset_.x + uiEffect_.extendVol_[0] / 4, 220 + uiEffect_.extendVol_[0] / 4,
					430 + uiEffect_.offset_.x - uiEffect_.extendVol_[0] / 4, 220 + uiEffect_.extendVol_[0] / 4,
					430 + uiEffect_.offset_.x - uiEffect_.extendVol_[0] / 4, 380 - uiEffect_.extendVol_[0] / 4,
					270 + uiEffect_.offset_.x + uiEffect_.extendVol_[0] / 4, 380 - uiEffect_.extendVol_[0] / 4,
					lpImageMng.GetID("./source/UIImage/Square Buttons/Colored Square Buttons/Back col_Square Button.png")[0], true);
			}
			else
			{
				DrawModiGraph(
					270 + uiEffect_.offset_.x + uiEffect_.extendVol_[0] / 4, 220 + uiEffect_.extendVol_[0] / 4,
					430 + uiEffect_.offset_.x - uiEffect_.extendVol_[0] / 4, 220 + uiEffect_.extendVol_[0] / 4,
					430 + uiEffect_.offset_.x - uiEffect_.extendVol_[0] / 4, 380 - uiEffect_.extendVol_[0] / 4,
					270 + uiEffect_.offset_.x + uiEffect_.extendVol_[0] / 4, 380 - uiEffect_.extendVol_[0] / 4,
					lpImageMng.GetID("./source/UIImage/Square Buttons/Square Buttons/Back Square Button.png")[0], true);
			}
		}

		DrawModiGraph(
			uiEffect_.extendVol_[3] / 4, 0 + uiEffect_.extendVol_[3] / 4,
			165 - uiEffect_.extendVol_[3] / 4, 0 + uiEffect_.extendVol_[3] / 4,
			165 - uiEffect_.extendVol_[3] / 4, 80 - uiEffect_.extendVol_[3] / 4,
			uiEffect_.extendVol_[3] / 4, 80 - uiEffect_.extendVol_[3] / 4,
			lpImageMng.GetID("./source/UIImage/string/SELECT/タイトルへ.png")[0], true);
		//茶色の枠
		DrawModiGraph(
			0 - 10 + uiEffect_.extendVol_[3] / 4,0 - 10 + uiEffect_.extendVol_[3] / 4,
			165 + 10 - uiEffect_.extendVol_[3] / 4, 0 - 10 + uiEffect_.extendVol_[3] / 4,
			165 + 10 - uiEffect_.extendVol_[3] / 4, 80 - uiEffect_.extendVol_[3] / 4,
			0 - 10 + uiEffect_.extendVol_[3] / 4, 80 - uiEffect_.extendVol_[3] / 4,
			lpImageMng.GetID("./source/UIImage/selectBox.png")[0], true);

		//前回のクリアタイム
		DrawRotaGraph(1105, 410, 0.45, 0, lpImageMng.GetID("NUM_D")[time_.s_one], true, false, false);
		DrawRotaGraph(1070, 410, 0.45, 0, lpImageMng.GetID("NUM_D")[time_.s_ten], true, false, false);
		DrawRotaGraph(1052, 400, 1, 0, lpImageMng.GetID("./source/UIImage/string/SELECT/timeの区切り.png")[0], true);
		DrawRotaGraph(1035, 410, 0.45, 0, lpImageMng.GetID("NUM_D")[time_.m_one], true, false, false);
		DrawRotaGraph(1000, 410, 0.45, 0, lpImageMng.GetID("NUM_D")[time_.m_ten], true, false, false);
		DrawRotaGraph(982, 400, 1, 0, lpImageMng.GetID("./source/UIImage/string/SELECT/timeの区切り.png")[0], true);
		DrawRotaGraph(965, 410, 0.45, 0, lpImageMng.GetID("NUM_D")[time_.h_one], true, false, false);
		DrawRotaGraph(930, 410, 0.45, 0, lpImageMng.GetID("NUM_D")[time_.h_ten], true, false, false);

		//最速クリアタイム
		DrawRotaGraph(1105, 464, 0.45, 0, lpImageMng.GetID("NUM_D")[bestTime_.s_one], true, false, false);
		DrawRotaGraph(1070, 464, 0.45, 0, lpImageMng.GetID("NUM_D")[bestTime_.s_ten], true, false, false);
		DrawRotaGraph(1052, 454, 1, 0, lpImageMng.GetID("./source/UIImage/string/SELECT/timeの区切り.png")[0], true);
		DrawRotaGraph(1035, 464, 0.45, 0, lpImageMng.GetID("NUM_D")[bestTime_.m_one], true, false, false);
		DrawRotaGraph(1000, 464, 0.45, 0, lpImageMng.GetID("NUM_D")[bestTime_.m_ten], true, false, false);
		DrawRotaGraph(982, 454, 1, 0, lpImageMng.GetID("./source/UIImage/string/SELECT/timeの区切り.png")[0], true);
		DrawRotaGraph(965, 464, 0.45, 0, lpImageMng.GetID("NUM_D")[bestTime_.h_one], true, false, false);
		DrawRotaGraph(930, 464, 0.45, 0, lpImageMng.GetID("NUM_D")[bestTime_.h_ten], true, false, false);
		for (int i = 0; i < COIN_MAX; i++)
		{
			if (getCoin_[i])
			{
				MV1DrawModel(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[i]);
			}
			else
			{
				MV1DrawModel(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[i]);
			}
		}
	}
	
	//DrawStringToHandle(0, 0, "Press Cancel to go to the TitleScene", 0xffffff, lpSceneMng.GetFontHandle());
}

bool UISelect::GetLoadingEndFlg(void)
{
	return true;
}

void UISelect::SetIsNext(bool flg)
{
}

void UISelect::OffsetUpdate(void)
{
	if (uiEffect_.move_L)//左側の矢印
	{
		if (uiEffect_.offset_.x > -50)
		{
			uiEffect_.offset_.x-=5;
		}
		else
		{
			uiEffect_.offset_.x = -50;
			uiEffect_.move_L = false;
		}
	}
	else
	{
		if (uiEffect_.offset_.x < 0)
		{
			uiEffect_.offset_.x++;
		}
		else
		{
			uiEffect_.offset_.x = 0;
			uiEffect_.move_L = true;
		}
	}
	if (uiEffect_.move_R)//右側の矢印
	{
		if (uiEffect_.offset_.y < 50)
		{
			uiEffect_.offset_.y+=5;
		}
		else
		{
			uiEffect_.offset_.y = 50;
			uiEffect_.move_R = false;
		}
	}
	else
	{
		if (uiEffect_.offset_.y > 0)
		{
			uiEffect_.offset_.y--;
		}
		else
		{
			uiEffect_.offset_.y = 0;
			uiEffect_.move_R = true;
		}
	}

	if (lpSceneUIMng.GetHitItem_Sub() == UI_SUB_ITEM::YES)//真ん中のエリアが選択されている時
	{
		uiEffect_.extend_[2] = true;
		if (cntData_[InputID::Decide][static_cast<int>(Trg::Now)])
		{
			uiEffect_.extend_[2] = false;
		}
	}
	else if (lpSceneUIMng.GetHitItem_Sub() == UI_SUB_ITEM::BACK)//左の矢印が選択されている時
	{
		uiEffect_.extend_[0] = true;
	}
	else if (lpSceneUIMng.GetHitItem_Sub() == UI_SUB_ITEM::NEXT)//右の矢印が選択されている時
	{
		uiEffect_.extend_[1] = true;
	}
	else if (lpSceneUIMng.GetHitItem_Sub() == UI_SUB_ITEM::NO)//【タイトルへ戻る】が選択されている時
	{
		uiEffect_.extend_[3] = true;
	}
	
	
	for (int i = 0; i < ITEMNUM; i++)
	{
		if (!uiEffect_.extend_[i])
		{
			if (uiEffect_.extendVol_[i] < 0)
			{
				uiEffect_.extendVol_[i] += 5;
			}
			else
			{
				uiEffect_.extendVol_[i] = 0;
			}
		}
		else
		{
			if (uiEffect_.extendVol_[i] > -30)
			{
				uiEffect_.extendVol_[i] -= 5;
			}
			else
			{
				uiEffect_.extendVol_[i] = -30;
				uiEffect_.extend_[i] = false;
			}
		}
	}
	
}

void UISelect::TimeUpdate(int stageNum)
{
	clearTime_ = lpSceneUIMng.Data_Load("./source/Data/Time/clearTime.txt", stageNum);
	bestClearTime_ = lpSceneUIMng.Data_Load("./source/Data/Time/bestClearTime.txt", stageNum);

	time_.s = clearTime_ % 60;
	bestTime_.s = bestClearTime_ % 60;
	time_.s_ten = time_.s / 10;
	bestTime_.s_ten = bestTime_.s / 10;
	time_.s_one = time_.s - time_.s_ten * 10;
	bestTime_.s_one = bestTime_.s - bestTime_.s_ten * 10;

	time_.m = clearTime_ / 60;
	bestTime_.m = bestClearTime_ / 60;
	time_.m_ten = time_.m / 10;
	bestTime_.m_ten = bestTime_.m / 10;
	time_.m_one = time_.m - time_.m_ten * 10;
	bestTime_.m_one = bestTime_.m - bestTime_.m_ten * 10;

	time_.h = time_.m / 60;
	bestTime_.h = bestTime_.m / 60;
	time_.h_ten = time_.h / 10;
	bestTime_.h_ten = bestTime_.h / 10;
	time_.h_one = time_.h - time_.h_ten * 10;
	bestTime_.h_one = bestTime_.h - bestTime_.h_ten * 10;
}

void UISelect::CoinStateUpdate(int stageNum)
{
	for (int i = 0; i < COIN_MAX; i++)
	{
		getCoin_[i] = lpSceneUIMng.CoinData_Load("./source/Data/Coin/CoinState.txt", i, stageNum);//コインの取得情報をロード
		//ステージごとに座標ずらす
		MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin.mv1")[i], { 960 + 100 * float(i) + 1000 * (stageNum - 2), 325, 0 });
		MV1SetPosition(lpModelMng.GetID("./source/model/Coin/Coin_Trans.mv1")[i], { 960 + 100 * float(i) + 1000 * (stageNum - 2), 325, 0 });
	}
}

