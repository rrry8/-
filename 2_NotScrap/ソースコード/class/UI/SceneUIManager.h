#pragma once
#include "../Scene/GameScene.h"
#include"../Common/Geometry.h"
#include "../Scene/SceneManager.h"
#include "UIBase.h"
#include "../Scene/OptionScene.h"
#include "../Scene/MenuScene.h"
#include "../Transition/CrossOver.h"

class MouseIcon;

// 置き換え
#define lpSceneUIMng SceneUIManager::GetInstance()

enum class UI_MAIN_ITEM//項目名
{
	//-----------TITLE-----------
	SINGLEPLAY, //１人PLAY
	MULTIPLAY,  //２人プレイ
	OPTION, //設定
	QUIT,   //ゲームをやめる
	//----------SELECT-----------
	//-----------GAME------------
	//-----------RESULT----------
	TO_SELECT,//セレクトシーンへ
	//----------OPTION-----------
	KEYCONFIG,//キーコンフィグ
	VOLUME,//音量設定
	TIPS,//ゲームをプレイする上での秘訣や裏技をみれる
	RETURN,//前のシーンに戻る
	//----------Menu-------------
	M_GAME,//ゲームにもどる
	M_RESTART,//リスタート
	M_OPTION,//設定を開く
	M_SELECT,//セレクトに戻る
	//----------NONE-------------
	NONE,
	MAX
};

enum class UI_SUB_ITEM//各項目の直下の項目名
{
	//キーコンフィグ
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_MENU,
	KEY_RESET,
	KEY_CHANGE,
	
	//音量設定
	VOL_BGM,
	VOL_SE,
	//ページ切り替えなどで使う
	NEXT,
	BACK,
	//前のシーンに戻る　とか　yes or no を聞くときに使う
	YES,
	NO,
	//----------NONE-------------
	NONE,
	MAX
};


class SceneUIManager
{
public:
	static SceneUIManager& GetInstance()
	{
		static SceneUIManager s_Instance;
		return s_Instance;
	}
	
	uniqueScene Init(uniqueScene &&scene);
	uniqueScene Init(uniqueScene &&scene,bool resetflg);
	bool Init(UI_MAIN_ITEM item);
	void Update(double delta);
	void Draw(double delta);

	void SetHitItem_Main(UI_MAIN_ITEM num);//カーソルが重なった項目の番号をセット
	void SetHitItem_Sub(UI_SUB_ITEM num);//カーソルが重なった項目の番号をセット
	UI_MAIN_ITEM GetHitItem_Main(void);//カーソルが重なった項目の番号をゲット
	UI_SUB_ITEM GetHitItem_Sub(void);//カーソルが重なった項目の番号をゲット
	void SetOptionState(OptionState state);//項目別の状態の設定
	void SetMenuState(MenuState state);//項目別の状態の設定
	OptionState GetOptionState();//項目別の状態の取得
	MenuState GetMenuState();//項目別の状態の取得
	Vector2Int GetScreenSize(void);
	bool GetLoadingEndFlg(void);
	void SetIsNext(bool flg);
	void SetStageNum(int stageNum);
	int GetStageNum(void);

	void SetSelectMapOffset(Vector2Int offset);
	Vector2Int GetSelectMapOffset();
	void SetSelectMapMoveFlg(bool flg);
	bool GetSelectMapMoveFlg();

	//int CoinData_Save(const char* FilePath,int data,int coinNum, int stageNum); // コインのデータをファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
	//int CoinData_Load(const char* FilePath,int coinNum, int stageNum); // コインのデータをファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
	int CoinData_Save(const char* FilePath, bool coin1,bool coin2,bool coin3, int stageNum); // コインのデータをファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
	int CoinData_Load(const char* FilePath, int coinNum, int stageNum); // コインのデータをファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
	int Data_Save(const char* FilePath, int data, int stageNum); // クリア時間とかをファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
	int Data_Load(const char* FilePath, int stageNum); // クリア時間とかをファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
	void Data_SetDef(void);
private:
	SceneUIManager();
	~SceneUIManager();

	SCENE_ID mSceneID;		// 親のインスタンス

	UI_MAIN_ITEM MainItemNo;	//	マウスカーソルが重なっている項目の識別番号
	UI_SUB_ITEM SubItemNo;	//	マウスカーソルが重なっている項目の識別番号
	OptionState state_;	//設定画面の状態
	MenuState state_m;	//メニュー画面の状態
	uniqueSceneUI sceneUI_;		//シーン
	int stageNum_;
	std::unique_ptr<MouseIcon> mouseIcon_;
	Vector2Int selectMapOffset_;
	bool selectMapMoveFlg_;
	int Data_;
	int data_[(int)STAGE_NAME::MAX];
	int coinData_[(int)STAGE_NAME::MAX][COIN_MAX];

};

