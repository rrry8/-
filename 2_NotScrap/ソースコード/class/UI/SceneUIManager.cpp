#include <DxLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Common/ImageMng.h"
#include "SceneUIManager.h"
#include "UITitle.h"
#include "UISelect.h"
#include "UIGame.h"
#include "UIResult.h"
#include "UIOption.h"
#include "UILoad.h"
#include"mouse/MouseIcon.h"
#include "../Scene/MenuScene.h"
#include "UIMenu.h"

uniqueScene SceneUIManager::Init(uniqueScene&& scene)
{
	switch (scene->GetSceneID())//各シーンのIDを取得
	{
	case Scene::Title:
		sceneUI_ = std::make_unique<UITitle>();//UIをタイトル画面用に遷移
		break;
	case Scene::Select:
		sceneUI_ = std::make_unique<UISelect>();//UIをセレクト画面用に遷移
		break;
	case Scene::Game:
		sceneUI_ = std::make_unique<UIGame>();//UIをゲームシーン用に遷移
		break;
	case Scene::Result:
		sceneUI_ = std::make_unique<UIResult>();//UIをResult画面用に遷移
		break;
	case Scene::Option:
		sceneUI_ = std::make_unique<UIOption>();//UIを設定画面用に遷移
		break;
	case Scene::Load:
		sceneUI_ = std::make_unique<UILoad>();//UIをロード画面用に遷移
		break;
	case Scene::Menu:
		sceneUI_ = std::make_unique<UIMenu>();//UIをロード画面用に遷移
		break;
	default:
		TRACE("不明なUIシーン");
		break;
	}
	
	return std::move(scene);
}

uniqueScene SceneUIManager::Init(uniqueScene&& scene, bool resetflg)
{
	switch (scene->GetSceneID())//各シーンのIDを取得
	{
	case Scene::Title:
		sceneUI_ = std::make_unique<UITitle>();//UIをタイトル画面用に遷移
		break;
	case Scene::Select:
		sceneUI_ = std::make_unique<UISelect>();//UIをセレクト画面用に遷移
		break;
	case Scene::Game:
		sceneUI_ = std::make_unique<UIGame>();//UIをゲームシーン用に遷移
		break;
	case Scene::Result:
		sceneUI_ = std::make_unique<UIResult>();//UIをResult画面用に遷移
		break;
	case Scene::Option:
		sceneUI_ = std::make_unique<UIOption>();//UIを設定画面用に遷移
		break;
	case Scene::Load:
		sceneUI_ = std::make_unique<UILoad>();//UIをロード画面用に遷移
		break;
	case Scene::Menu:
		sceneUI_ = std::make_unique<UIMenu>();//UIをロード画面用に遷移
		break;
	default:
		TRACE("不明なUIシーン");
		break;
	}

	return std::move(scene);
}

bool SceneUIManager::Init(UI_MAIN_ITEM item)
{
	switch (item)
	{
	case UI_MAIN_ITEM::SINGLEPLAY:
		break;
	case UI_MAIN_ITEM::MULTIPLAY:
		break;
	case UI_MAIN_ITEM::OPTION:
		break;
	case UI_MAIN_ITEM::QUIT:
		break;
	case UI_MAIN_ITEM::TO_SELECT:
		break;
	case UI_MAIN_ITEM::KEYCONFIG:
		break;
	case UI_MAIN_ITEM::VOLUME:
		break;
	case UI_MAIN_ITEM::TIPS:
		break;
	case UI_MAIN_ITEM::RETURN:
		break;
	case UI_MAIN_ITEM::NONE:
		sceneUI_ = std::make_unique<UIGame>();//UIをロード画面用に遷移
		break;
	case UI_MAIN_ITEM::MAX:
		break;
	default:
		break;
	}
	return false;
}


void SceneUIManager::Update(double delta)
{
	sceneUI_->Update(delta);
	mouseIcon_->Update();
}

void SceneUIManager::Draw(double delta)
{
	sceneUI_->Draw(delta);
	mouseIcon_->Draw();

	_dbgDraw();
}

void SceneUIManager::SetHitItem_Main(UI_MAIN_ITEM num)
{
	MainItemNo = num;
}

void SceneUIManager::SetHitItem_Sub(UI_SUB_ITEM num)
{
	SubItemNo = num;
}

UI_MAIN_ITEM SceneUIManager::GetHitItem_Main()
{
	return MainItemNo;
}

UI_SUB_ITEM SceneUIManager::GetHitItem_Sub()
{
	return SubItemNo;
}

void SceneUIManager::SetOptionState(OptionState state)
{
	state_ = state;
}

void SceneUIManager::SetMenuState(MenuState state)
{
	state_m = state;
}

OptionState SceneUIManager::GetOptionState()
{
	return state_;
}

MenuState SceneUIManager::GetMenuState()
{
	return state_m;
}

bool SceneUIManager::GetLoadingEndFlg(void)
{
	return sceneUI_->GetLoadingEndFlg();
}

void SceneUIManager::SetIsNext(bool flg)
{
	sceneUI_->SetIsNext(flg);
}

void SceneUIManager::SetStageNum(int stageNum)
{
	stageNum_ = stageNum;
}

int SceneUIManager::GetStageNum(void)
{
	return stageNum_;
}

void SceneUIManager::SetSelectMapOffset(Vector2Int offset)
{
	selectMapOffset_ = offset;
}

Vector2Int SceneUIManager::GetSelectMapOffset()
{
	return selectMapOffset_;
}

void SceneUIManager::SetSelectMapMoveFlg(bool flg)
{
	selectMapMoveFlg_ = flg;
}

bool SceneUIManager::GetSelectMapMoveFlg()
{
	return selectMapMoveFlg_;
}

int SceneUIManager::CoinData_Save(const char* FilePath, bool coin1, bool coin2, bool coin3,int stageNum)
{
	coinData_[stageNum][0] = coin1;
	coinData_[stageNum][1] = coin2;
	coinData_[stageNum][2] = coin3;
	FILE* fp;
	int err;
	// 対応情報を保存するファイルを開く
	err = fopen_s(&fp, FilePath, "wt");
	if (err != 0)
	{
		// 開けなかったら何もせずに終了
		TRACE("ファイルの書き込みに失敗しました\n");
		return FALSE;
	}

	// コインの情報をファイルに書き出す
	for (int s = 0; s < (int)STAGE_NAME::MAX; s++)
	{
		for (int c = 0; c < COIN_MAX; c++)
		{
			fprintf(fp, "--stage%d--\ncoin%d = %d\n",s, c, coinData_[s][c]);
		}
	}
	// ファイルを閉じる
	fclose(fp);


	// 成功終了
	//TRACE("ファイルの書き込み完了\n");
	return TRUE;
}

int SceneUIManager::CoinData_Load(const char* FilePath, int coinNum ,int stageNum)
{
	FILE* fp;
	int err;

	// 対応情報を保存したファイルを開く
	err = fopen_s(&fp, FilePath, "rt");
	if (err != 0)
	{
		// 開けなかった場合は何もせずに終了
		TRACE("ファイルの読み込みに失敗しました\n");
		return FALSE;
	}

	// 情報をファイルから読み込む
	for (int s = 0; s < (int)STAGE_NAME::MAX; s++)
	{
		for (int c = 0; c < COIN_MAX; c++)
		{
			fscanf_s(fp, "--stage%d--\ncoin%d = %d\n", &s,&c, &coinData_[s][c]);
		}
	}
	// ファイルを閉じる
	//TRACE("ファイルの読み込み完了\n");
	fclose(fp);


	// 成功終了
	return coinData_[stageNum][coinNum];
}


int SceneUIManager::Data_Save(const char* FilePath, int data, int stageNum)
{
	data_[stageNum] = data;
	FILE* fp;
	int err;
	// 対応情報を保存するファイルを開く
	err = fopen_s(&fp, FilePath, "wt");
	if (err != 0)
	{
		// 開けなかったら何もせずに終了
		TRACE("ファイルの書き込みに失敗しました\n");
		return FALSE;
	}

	// ゲームの各入力とキーやパッドなどの入力との対応情報をファイルに書き出す
	for (int i = 0; i < (int)STAGE_NAME::MAX; i++)
	{
		fprintf(fp, "stage%d:time:%d\n", i, data_[i]);
	}
	// ファイルを閉じる
	fclose(fp);


	// 成功終了
	//TRACE("ファイルの書き込み完了\n");
	return TRUE;
}

int SceneUIManager::Data_Load(const char* FilePath, int stageNum)
{
	FILE* fp;
	int err;

	// 対応情報を保存したファイルを開く
	err = fopen_s(&fp, FilePath, "rt");
	if (err != 0)
	{
		// 開けなかった場合は何もせずに終了
		TRACE("ファイルの読み込みに失敗しました\n");
		return FALSE;
	}

	// ゲームの情報をファイルから読み込む
	for (int i = 0; i < (int)STAGE_NAME::MAX; i++)
	{
		fscanf_s(fp, "stage%d:time:%d\n", &i, &data_[i]);
	}
	// ファイルを閉じる
	//TRACE("ファイルの読み込み完了\n");
	fclose(fp);


	// 成功終了
	return data_[stageNum];
}

void SceneUIManager::Data_SetDef(void)
{
	//クリアタイム時間初期化
	for (int i = 0; i < (int)STAGE_NAME::MAX; i++)
	{
		data_[i] = 0;
	}
}

SceneUIManager::SceneUIManager()
{
	MainItemNo = UI_MAIN_ITEM::MAX;
	SubItemNo = UI_SUB_ITEM::MAX;


	mouseIcon_ = std::make_unique<MouseIcon>();
}

SceneUIManager::~SceneUIManager()
{
}