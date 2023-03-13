#include "UIMenu.h"
#include "SceneUIManager.h"
#include "../Obj/CheckHit.h"
#include "../Common/ImageMng.h"

UIMenu::UIMenu()
{
    Init();
    DrawOwnScreen(0.0);
}

UIMenu::~UIMenu()
{
}

bool UIMenu::Init(void)
{
	tmxObj_.LoadTmx("././tmx/menuUI.tmx");
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MouseCursormodule");
	return true;
}

void UIMenu::Update(double delta)
{
	auto node = stateNode_->first_node("module");
	GetMousePoint(&MousePos.x, &MousePos.y);
	if (lpCntMng.GetController()->GetCntType() == CntType::Key)
	{
		MousePos.x += 32;//画像ずれてる分補正
		MousePos.y += 32;
	}
	int i = (int)UI_MAIN_ITEM::M_GAME;//範囲for文の先頭を設定
	menuState_ = lpSceneUIMng.GetMenuState();//設定画面で選択している項目の状態を取得

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

	DrawOwnScreen(delta);
}

void UIMenu::DrawOwnScreen(double delta)
{
	auto tilesize = tmxObj_.GetTileSize();
	auto offset = Vector2{ 0.0f, 0.0f }/*camera_->GetPos()*/;

SetDrawBlendMode(DX_BLENDMODE_ALPHA, 233);
	DrawBox(745, 276, 1159, 868, 0x111111, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
DrawRotaGraph(screenSize_.x/2, 351, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/ゲームに戻る.png")[0], true);
	DrawRotaGraph(screenSize_.x/2, 495, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/リスタート.png")[0], true);
	DrawRotaGraph(screenSize_.x/2, 649, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/設定.png")[0], true);
	DrawRotaGraph(screenSize_.x/2, 799, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/セレクトへ.png")[0], true);
	//茶色の枠を描画

	DrawRotaGraph(screenSize_.x / 2, screenSize_.y / 2, 1, 0, lpImageMng.GetID("source/UIIMage/menuBox.png")[0], true);

	/*for (const auto& data : tmxObj_.GetMapData())	{
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
	}*/
}

bool UIMenu::GetLoadingEndFlg(void)
{
	return true;
}

void UIMenu::SetIsNext(bool flg)
{
}
