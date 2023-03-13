#pragma once
#include "BaseScene.h"
#include "GameScene.h"
#include"../Common/Geometry.h"

// 置き換え
#define lpSceneMng SceneManager::GetInstance()

//シーン管理用
enum SCENE_ID
{
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_OPTION,
	MAX
};

class SceneManager
{
public:
	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}

	void Run(void);
	uniqueScene Update(void);
	void Draw(double delta);

	double GetDeltaTime(void);
	int GetFontHandle(void);
	void SetMultiFlg(bool flg);//マルチが選ばれたらtrue
	bool GetMultiFlg(void);//マルチ中ならtrue
	Vector2Int GetScreenSize(void);
	void SetEndFlg(bool flg);//trueをセットするとゲームループが終了する
	void SetSoundVol(int vol);
	int GetSoundVol();

	//現在のシーンを取得
	Scene GetSceneID(void);
private:
	SceneManager();
	~SceneManager();

	int mScreenSizeX;		//画面のXサイズ
	int mScreenSizeY;		//画面のYサイズ
	int iconH;
	SCENE_ID mSceneID;		// 親のインスタンス

	bool initFlg_;			//初期化フラグ
	uniqueScene scene_;		//シーン
	std::unique_ptr<Controller> controller_;
	double deltaTime_;
	int FontHandle_;
	bool SysInit(void);		//初期化
	bool gameEndFlg;//ゲームループを終了させるフラグ
	int soundVol;
	Vector2Int MousePos_;
	bool multiFlg_;//マルチ中ならtrue

	int cnt_ = 0;
};
