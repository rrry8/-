#include <DxLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Scene/SelectScene.h"
#include "UIOption.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../Obj/CheckHit.h"
#include "../Scene/OptionScene.h"
//#include "../Transition/TransitionScene.h"

UIOption::UIOption()
{
	Init();
	DrawOwnScreen(0.0);
}

UIOption::~UIOption()
{
}

bool UIOption::Init(void)
{
	tmxObj_.LoadTmx("././tmx/optionUI.tmx");
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MouseCursormodule");
	return true;
}

void UIOption::Update(double delta)
{
	auto node = stateNode_->first_node("module");
	GetMousePoint(&MousePos.x, &MousePos.y);
	if (lpCntMng.GetController()->GetCntType() == CntType::Key)
	{
		MousePos.x += 32;//画像ずれてる分補正
		MousePos.y += 32;
	}
	int i = (int)UI_MAIN_ITEM::KEYCONFIG;//範囲for文の先頭を設定
	optionState_ = lpSceneUIMng.GetOptionState();//設定画面で選択している項目の状態を取得
	
	//カーソルとメイン項目'の当たり判定				＊メイン項目：設定画面左側の項目
	for (auto& coll : tmxObj_.GetItemCollList())
	{
		if (CheckCollM()(raycast_, MousePos, node, coll))
		{
			//TRACE("当たった！\n");
			lpSceneUIMng.SetHitItem_Main((UI_MAIN_ITEM)i);
			break;
		}
		else
		{
			lpSceneUIMng.SetHitItem_Main(UI_MAIN_ITEM::NONE);
		}
		i++;
	}

	int cnt;
	switch (optionState_)
	{
	case OptionState::KeyConfig:
		cnt = (int)UI_SUB_ITEM::KEY_CHANGE;//範囲for文の先頭を設定
		break;
	case OptionState::Volume:
		cnt = (int)UI_SUB_ITEM::KEY_CHANGE;//範囲for文の先頭を設定
		break;
	case OptionState::Tips:
		cnt = (int)UI_SUB_ITEM::NEXT;//範囲for文の先頭を設定
		break;
	case OptionState::Quit:
		cnt = 8;//範囲for文の先頭を設定
		break;
	case OptionState::Max:
		cnt = (int)UI_SUB_ITEM::MAX;//範囲for文の先頭を設定
		break;
	default:
		break;
	}
	
	//カーソルとサブ項目'の当たり判定				＊サブ項目：設定画面右側の項目
	for (auto& coll : tmxObj_.GetSubItemCollList())
	{
		if (CheckCollM()(raycast_, MousePos, node, coll))
		{
			//TRACE("当たった！\n");
			lpSceneUIMng.SetHitItem_Sub((UI_SUB_ITEM)cnt);
			break;
		}
		else
		{
			lpSceneUIMng.SetHitItem_Sub(UI_SUB_ITEM::NONE);
		}
		cnt++;
	}
	DrawOwnScreen(delta);
}


void UIOption::DrawOwnScreen(double delta)
{

	auto tilesize = tmxObj_.GetTileSize();
	auto offset = Vector2{ 0.0f, 0.0f }/*camera_->GetPos()*/;

	
	//マップを描画
	for (const auto& data : tmxObj_.GetMapData())
	{
		for (int y = static_cast<int>(offset.y / tilesize.y) - 3; y < offset.y / tilesize.y + lpSceneMng.GetScreenSize().y / tilesize.y + 3; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetScreenSize().x / tilesize.x + 3; x++)
			{
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int y_ = y;
					if (y < 0)
					{
						y_ = 0;
					}
					if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
					{
						int gid = data.second[y_ * tmxObj_.GetWorldArea().x + x] - tmxObj_.GetFirstGID();
						if (gid >= 0)
						{
							DrawGraph(x * tilesize.x, y * tilesize.y,
								lpImageMng.GetID("FantasyUIPack.png")[gid], true);
						}
					}
				}
			}
		}
	}

	//各項目の文字を画像で描画
	//DrawExtendGraph(324, 150,624,296, lpImageMng.GetID("source/UIIMage/Optionitem_OPTION.png")[0], true);
	DrawGraph(345, 350, lpImageMng.GetID("source/UIIMage/Optionitem_KeySettings.png")[0], true);
	DrawGraph(400, 505, lpImageMng.GetID("source/UIIMage/Optionitem_Volume.png")[0], true);
	DrawGraph(345, 665, lpImageMng.GetID("source/UIIMage/Optionitem_Tips.png")[0], true);
	DrawGraph(375, 835,lpImageMng.GetID("source/UIIMage/Optionitem_Quit.png")[0], true);

	switch (optionState_)
	{
	case OptionState::KeyConfig:
		
		DrawGraph(SUB_ITEM_LEFT, 322, lpImageMng.GetID("./source/UIImage/sub_item_nagaosi.png")[0], true);
		//DrawGraph(SUB_ITEM_LEFT, 430, lpImageMng.GetID("./source/UIImage/sub_item_DOWN.png")[0], true);
		//DrawGraph(SUB_ITEM_LEFT, 544, lpImageMng.GetID("./source/UIImage/sub_item_kirikae.png")[0], true);
		//DrawGraph(SUB_ITEM_LEFT, 656, lpImageMng.GetID("./source/UIImage/sub_item_RIGHT.png")[0], true);
		//DrawGraph(SUB_ITEM_LEFT, 769, lpImageMng.GetID("./source/UIImage/sub_item_OPTION.png")[0], true);
		//DrawGraph(SUB_ITEM_LEFT, 872, lpImageMng.GetID("./source/UIImage/sub_item_RESET.png")[0], true);
		break;
	case OptionState::Volume:
		//DrawString(1000, 500, "volume", 0xffffff);
		break;
	case OptionState::Tips:
		DrawString(700, 500, "未実装:", 0xffffff);
		break;
	case OptionState::Quit:
		//DrawString(1000, 500, "quit", 0xffffff);
		break;
	case OptionState::Max:
		DrawString(1000, 500, "max", 0xffffff);
		break;
	default:
		break;
	}
}

bool UIOption::GetLoadingEndFlg(void)
{
	return true;
}

void UIOption::SetIsNext(bool flg)
{
}
