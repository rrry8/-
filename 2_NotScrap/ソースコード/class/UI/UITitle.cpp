#include <DxLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Scene/SelectScene.h"
#include "UITitle.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../Obj/CheckHit.h"
#include "../Common/ModelMng.h"
#include "../Common/AnimationController.h"
//#include "../Transition/TransitionScene.h"

UITitle::UITitle()
{
	Init();
	DrawOwnScreen(0.0);
}


UITitle::~UITitle()
{
	MV1DeleteModel(modelH);
}

bool UITitle::Init(void)
{
	tmxObj_.LoadTmx("././tmx/titleUI.tmx");
	camera_ = std::make_unique<Camera>();
	//std::make_unique<Camera>(worldsize.x, worldsize.y);
	SetCameraPositionAndTargetAndUpVec(VGet(0, 0, -400), VGet(0, 0, 0), VGet(0, 1, 0));

	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MouseCursormodule");
	IsNext_ = false;
	modelH = MV1LoadModel("./source/model/UI/TitleSignandarm.mv1");
	animController_ = std::make_shared<AnimationController>(modelH);
	animController_->Add((int)ANIM_TYPE::IDLE, "./source/model/UI/TitleSignandarm.mv1", 30.0f);
	animController_->Play((int)ANIM_TYPE::IDLE,false);
	MV1SetScale(modelH, VGet(3, 3, 3));
	for (int i = 0; i < 4; i++)
	{
		hitItem[i] = false;
		moveVol[i] = 200;
		fastOffset_[i] = 500 + (50 * i);
	}
	BrightVol=0;
	BrightFlg = true;
	exRate = 1.0;
	exFlg = true;
	exFlg_ = true;
	IsNextScene_ = false;
	return true;
}

void UITitle::Update(double delta)
{
	cntData_ = lpCntMng.GetController()->GetCntData();
	MV1SetPosition(modelH, VGet(950.0f/* + static_cast<float>(offset_.x)*/, 1000.0f, 0.0f));
	MV1SetRotationXYZ(modelH,VGet(0, 0, 0));
	auto node = stateNode_->first_node("module");
	if (!IsNextScene_)
	{
		GetMousePoint(&MousePos.x, &MousePos.y);
		if (lpCntMng.GetController()->GetCntType() == CntType::Key)
		{
			MousePos.x += 32;//画像ずれてる分補正
			MousePos.y += 32;
		}
	}
	int i = (int)UI_MAIN_ITEM::SINGLEPLAY;//一番上の項目の名前
	bool hitflg = false;

	for (auto& coll : tmxObj_.GetItemCollList())
	{
		
		if (CheckCollM()(raycast_, MousePos, node, coll))//カーソルが何かの項目に当たっていたら中に入る
		{
			//TRACE("当たった！\n");
			if (offset_.x <= 0)
			{
				
				hitflg = true;
				lpSceneUIMng.SetHitItem_Main((UI_MAIN_ITEM)i);//当たった項目をセット
				hitItem[i] = true;
				if (KeyTrgDown(InputID::Decide))
				{
					IsNextScene_ = true;
				}
					if (IsNextScene_)
					{
						for (int n = 0; n < 4; n++)
						{
							if (hitItem[i] != hitItem[n])
							{
								fastOffset_[n] += 120;
								if (fastOffset_[n] > 1000)
								{
									fastOffset_[n] = 1000;
								}
							}
						}
					}
					
				
				if (moveVol[i] > 0.0)
				{
					lpSoundMng.PlayingSound("./source/Sound/UI移動音.mp3");
					moveVol[i]-=60;
				}
				else
				{
					//緑の枠の拡縮
					if (exFlg)
					{
						if (moveVol[i] > -10)
						{
							moveVol[i]-=0.5;
						}
						else
						{
							exFlg = false;
						}
					}
					else
					{
						if (moveVol[i] <= -0.1)
						{
							moveVol[i] += 0.5;
						}
						else
						{
							exFlg = true;
						}
					}
				}
			}
		}
		else
		{
			hitItem[i] = false;
			if (moveVol[i] < 300)
			{
				moveVol[i]+=50;
			}
			else
			{
				moveVol[i] = 300;
			}
		}
		i++;
	}
	if (!hitflg)//何も選択されてないとき
	{
		lpSceneUIMng.SetHitItem_Main(UI_MAIN_ITEM::NONE);
	}
	
	//if (CheckHitKey(KEY_INPUT_LEFT))
	if (IsNext_)//ボタンが押されたらオフセットをずらす
	{
		//項目の後ろの半透明な白い背景
		if (offset_.x > 0)
		{
			offset_.x -= 50;
		}
		else
		{
			offset_.x = 0;
		}
		//---
		if (!IsNextScene_)
		{
			//項目が順番にスライドしてくるやつ
			for (int i = 0; i < 4; i++)
			{
				if (fastOffset_[i] > 0)
				{
					fastOffset_[i] -= 20;
				}
				else
				{
					fastOffset_[i] = 0;
				}
			}
		}
		//---
	}
	
	//	文字の点滅
	if (BrightFlg)
	{
		if (BrightVol < 255)
		{
			BrightVol++;
		}
		else
		{
			BrightFlg = false;
		}
	}
	else
	{
		if (BrightVol > 100)
		{
			BrightVol--;
		}
		else
		{
			BrightFlg = true;
		}
	}
	//--------
	////文字拡縮
	//if (exFlg_)
	//{
	//	if (exRate < 1.1)
	//	{
	//		exRate += 0.01;
	//	}
	//	else
	//	{
	//		exFlg_ = false;
	//	}
	//}
	//else
	//{
	//	if (exRate > 1.0)
	//	{
	//		exRate -= 0.01;
	//	}
	//	else
	//	{
	//		exFlg_ = true;
	//	}
	//}
	
	animController_->Update(delta);
	DrawOwnScreen(delta);
}


void UITitle::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenUIID_);
	ClsDrawScreen();
	//3Dの背景色
	/*SetBackgroundColor(0, 0, 0);*/
	//DrawBox(0, 0, screenSize_.x, screenSize_.y, 0xffffff,true);
	auto tilesize = tmxObj_.GetTileSize();
	auto offset = Vector2{ 0.0f, 0.0f };
	camera_->GetPos();
	//右下のエリアの塗りつぶし
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawTriangle(screenSize_.x + offset_.x, 0, screenSize_.x - screenSize_.x / 3 + offset_.x, screenSize_.y, screenSize_.x + offset_.x, screenSize_.y, 0xffffff, true);
	DrawLine(screenSize_.x + offset_.x, 0 - 13, screenSize_.x - screenSize_.x / 3 - 15 + offset_.x, screenSize_.y + 13, 0xffffff, 4);
	DrawLine(screenSize_.x + offset_.x, 0 - 30, screenSize_.x - screenSize_.x / 3 - 35 + offset_.x, screenSize_.y + 30, 0xffffff, 8);
	//DrawLine(screenSize_.x + offset_.x, 0 - 40, screenSize_.x - screenSize_.x / 3 - 45 + offset_.x, screenSize_.y + 40, 0x000000, 2);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//黒の四角
	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x+ fastOffset_[0], screenSize_.y / 2 + 90,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[0], screenSize_.y / 2 + 90,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[0], screenSize_.y / 2 + 180,
		ITEM_LEFT_SINGLE - 172 + offset_.x + fastOffset_[0], screenSize_.y / 2 + 180,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_Bar.png")[0], true);

	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 205,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 205,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 295,
		ITEM_LEFT_SINGLE - 172 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 295,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_Bar.png")[0], true);

	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 315,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 315,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 405,
		ITEM_LEFT_SINGLE - 172 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 405,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_Bar.png")[0], true);

	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 430,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 430,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 520,
		ITEM_LEFT_SINGLE - 172 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 520,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_Bar.png")[0], true);

	//緑の四角
	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + static_cast<int>(moveVol[0]) + fastOffset_[0], screenSize_.y / 2 + 90,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[0], screenSize_.y / 2 + 90,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[0], screenSize_.y / 2 + 180,
		ITEM_LEFT_SINGLE - 172 + offset_.x + static_cast<int>(moveVol[0]) + fastOffset_[0], screenSize_.y / 2 + 180,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_ProgressBar.png")[0], true);

	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + static_cast<int>(moveVol[1]) + fastOffset_[1], screenSize_.y / 2 + 205,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 205,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 295,
		ITEM_LEFT_SINGLE - 172 + offset_.x + static_cast<int>(moveVol[1]) + fastOffset_[1], screenSize_.y / 2 + 295,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_ProgressBar.png")[0], true);

	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + static_cast<int>(moveVol[2]) + fastOffset_[2], screenSize_.y / 2 + 315,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 315,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 405,
		ITEM_LEFT_SINGLE - 172 + offset_.x + static_cast<int>(moveVol[2]) + fastOffset_[2], screenSize_.y / 2 + 405,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_ProgressBar.png")[0], true);

	DrawModiGraph(
		ITEM_LEFT_SINGLE - 124 + offset_.x + static_cast<int>(moveVol[3]) + fastOffset_[3], screenSize_.y / 2 + 430,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 430,
		ITEM_LEFT_SINGLE + 292 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 520,
		ITEM_LEFT_SINGLE - 172 + offset_.x + static_cast<int>(moveVol[3]) + fastOffset_[3], screenSize_.y / 2 + 520,
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_ProgressBar.png")[0], true);

	MV1DrawModel(modelH);
	if (!IsNext_)
	{
		int StrWidth, StrLen;
		StrLen = strlen("Press any button");
		StrWidth = GetDrawStringWidthToHandle("Press any button", StrLen, lpSceneMng.GetFontHandle());
		SetDrawBright(BrightVol, BrightVol, BrightVol);
		DrawStringToHandle((screenSize_.x/2)-StrWidth/2,(screenSize_.y/2)+100, "Press any button", 0xffffff, lpSceneMng.GetFontHandle());
		SetDrawBright(255, 255, 255);
	}
	else
	{
		SetDrawBright(255, 255, 255);
		//選択項目の枠とかを描画
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
								DrawGraph(x * tilesize.x + offset_.x, y * tilesize.y,
									lpImageMng.GetID("FantasyUIPack.png")[gid], true);
							}
						}
					}
				}
			}
		}

		//titleの文字の表示
		//DrawGraph(80 + offset_.x, 100,lpImageMng.GetID("./source/UIImage/GAMETITLE.png")[0], true);//TITLE
		
		if (!hitItem[0]&&!hitItem[1] && !hitItem[2] && !hitItem[3])//何も選んでない時
		{
			//int StrWidth, StrLen;
			//StrLen = strlen("カーソルを操作して、項目を選択してください。");
			//StrWidth = GetDrawStringWidth("カーソルを操作して、項目を選択してください。", StrLen);
			//DrawString(screenSize_.x / 2 - StrWidth / 2, screenSize_.y - 32, "カーソルを操作して、項目を選択してください。", 0xffffff);
			DrawGraph(screenSize_.x / 2 - 573 / 2, screenSize_.y - 64, lpImageMng.GetID("./source/UIImage/string/title_hint.png")[0], true);
		}
		
		DrawGraph(screenSize_.x / 2 - 343 / 2, screenSize_.y - 128, lpImageMng.GetID("./source/UIImage/string/決定ボタン.png")[0], true);
		
		if (!hitItem[0])//シングル
		{
			DrawGraph(ITEM_LEFT_SINGLE - 147 + offset_.x + fastOffset_[0], screenSize_.y / 2 + 90, lpImageMng.GetID("./source/UIImage/string/titleitem_singleplay.png")[0], true);//SINGLE
			//DrawStringToHandle(ITEM_LEFT_SINGLE + offset_.x, screenSize_.y / 2 + 90, "シングル", color[0], lpSceneMng.GetTitleFontHandle());
		}
		else
		{
			DrawGraph(screenSize_.x / 2 - 404 / 2 , screenSize_.y - 64, lpImageMng.GetID("./source/UIImage/string/title_hint1.png")[0], true);
			DrawRotaGraph(ITEM_LEFT_SINGLE + offset_.x + fastOffset_[0], screenSize_.y / 2 + 134, exRate, 0, lpImageMng.GetID("./source/UIImage/string/titleitem_singleplay.png")[0], true);//SINGLE
			//if (IsNextScene_)
			//{
			//	DrawRotaGraph(ITEM_LEFT_SINGLE + offset_.x + fastOffset_[0], screenSize_.y / 2 + 134, exRate, 0, lpImageMng.GetID("./source/UIImage/string/titleitem_singleplay_B.png")[0], true);//SINGLE
			//}
		}
		if (!hitItem[1])//マルチ
		{
			DrawGraph(ITEM_LEFT_MULTI - 109 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 205, lpImageMng.GetID("./source/UIImage/string/titleitem_multiplay.png")[0], true);//MULTI
			//DrawStringToHandle(ITEM_LEFT_MULTI + offset_.x, screenSize_.y / 2 + 205, "マルチ", color[1], lpSceneMng.GetTitleFontHandle());
		}
		else
		{
			DrawGraph(screenSize_.x / 2 - 418 / 2 , screenSize_.y - 64, lpImageMng.GetID("./source/UIImage/string/title_hint2.png")[0], true);
			DrawRotaGraph(ITEM_LEFT_MULTI + offset_.x + fastOffset_[1], screenSize_.y / 2 + 247, exRate, 0, lpImageMng.GetID("./source/UIImage/string/titleitem_multiplay.png")[0], true);//MULTI
			//if (IsNextScene_)
			//{
			//	DrawGraph(ITEM_LEFT_MULTI - 109 + offset_.x + fastOffset_[1], screenSize_.y / 2 + 205, lpImageMng.GetID("./source/UIImage/string/titleitem_multiplay_B.png")[0], true);//MULTI
			//}
		}
		if (!hitItem[2])//設定
		{
			DrawGraph(ITEM_LEFT_OPTION - 83 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 315, lpImageMng.GetID("./source/UIImage/string/titleitem_option.png")[0], true);//OPTION
			//DrawStringToHandle(ITEM_LEFT_OPTION + offset_.x, screenSize_.y / 2 + 315, "設定", color[2], lpSceneMng.GetTitleFontHandle());
		}
		else
		{
			DrawGraph(screenSize_.x / 2 - 262 / 2 , screenSize_.y - 64, lpImageMng.GetID("./source/UIImage/string/title_hint3.png")[0], true);
			DrawRotaGraph(ITEM_LEFT_OPTION + offset_.x + fastOffset_[2], screenSize_.y / 2 + 360, exRate, 0, lpImageMng.GetID("./source/UIImage/string/titleitem_option.png")[0], true);//OPTION
			//if (IsNextScene_)
			//{
			//	DrawGraph(ITEM_LEFT_OPTION - 83 + offset_.x + fastOffset_[2], screenSize_.y / 2 + 315, lpImageMng.GetID("./source/UIImage/string/titleitem_option_B.png")[0], true);//OPTION
			//}
		}
		if (!hitItem[3])//やめる
		{
			DrawGraph(ITEM_LEFT_QUIT - 106 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 430, lpImageMng.GetID("./source/UIImage/string/titleitem_quit.png")[0], true);//QUIT
			//DrawStringToHandle(ITEM_LEFT_QUIT + offset_.x, screenSize_.y / 2 + 430, "やめる", color[3], lpSceneMng.GetTitleFontHandle());
		}
		else
		{
			DrawGraph(screenSize_.x / 2 - 261 / 2 , screenSize_.y - 64, lpImageMng.GetID("./source/UIImage/string/title_hint4.png")[0], true);
			DrawRotaGraph(ITEM_LEFT_QUIT + offset_.x + fastOffset_[3], screenSize_.y / 2 + 472, exRate, 0, lpImageMng.GetID("./source/UIImage/string/titleitem_quit.png")[0], true);//QUIT
			//if (IsNextScene_)
			//{
			//	DrawGraph(ITEM_LEFT_QUIT - 106 + offset_.x + fastOffset_[3], screenSize_.y / 2 + 430, lpImageMng.GetID("./source/UIImage/string/titleitem_quit_B.png")[0], true);//QUIT
			//}
		}
		
		//DrawFormatString(0,0,0xffffff,"DrawCall:%d",GetDrawCallCount());
	}
}

bool UITitle::GetLoadingEndFlg(void)
{
	return true;
}

void UITitle::SetIsNext(bool flg)
{
	IsNext_ = flg;
}


