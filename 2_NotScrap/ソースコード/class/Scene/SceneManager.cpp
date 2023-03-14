#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include <memory>
#include <chrono>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../Common/Fps.h"
#include "../Common/ImageMng.h"
#include "../UI/SceneUIManager.h"
#include "ResultScene.h"
#include "LoadScene.h"
#include "../Input/KeyInput.h"
#include "../Input/PadInput.h"
#include "../Input/ControllerMng.h"
#include"../../source/image/icon/icon.h"

// 画面サイズ
constexpr int ScreenSizeX = 1920;
constexpr int ScreenSizeY = 1080;

//歪ませたい範囲
constexpr float DISTORTION_SIZE_X = 500; //x
constexpr float DISTORTION_SIZE_Y = 500; //y

//スクリーンサイズ
constexpr float SCREEN_SIZE_X = 1920.0f; //x
constexpr float SCREEN_SIZE_Y = 1080.0f; //y


void SceneManager::Run(void)
{
	if (!initFlg_)
	{
		if (!SysInit())
		{
			return;
		}
	}
	gameEndFlg = false;		//ゲームループ終了フラグ

	if (GetJoypadNum())
	{
		lpCntMng.SetUseController(CntType::Pad);
	}
	else
	{
		lpCntMng.SetUseController(CntType::Key);
	}
	lpCntMng.GetController()->KeyConfig_SetDefault();;//ゲーム開始直前にキーコンフィグをリセット（click連打で数値がぶっ飛んでエラーになった時用）
	lpCntMng.GetController()->KeyConfig_Save();//リセット終了後.txtにセーブする。これにより、毎回ゲーム開始時にキーコンフィグはリセットされる。

	soundVol = 100;
	lpSoundMng.ChangeVolume(soundVol - 30, "./source/Sound/walk_trim.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/coinget.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/飛来02.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/ロボットを強く殴る3.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/EnemyDeath.mp3");
	lpSoundMng.ChangeVolume(soundVol + 30, "./source/Sound/UI移動音.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/UI選択音.mp3");
	lpSoundMng.ChangeVolume(soundVol - 30, "./source/Sound/machikouba.mp3");
	lpSoundMng.ChangeVolume(soundVol - 10, "./source/Sound/streetofrage.mp3");
	lpSoundMng.ChangeVolume(soundVol - 10, "./source/Sound/stageclear.mp3");
	lpSoundMng.ChangeVolume(soundVol - 20, "./source/Sound/selectscene.mp3");
	lpSoundMng.ChangeVolume(soundVol - 20, "./source/Sound/explosion.mp3");

	// 初期シーンのインスタンス
	//scene_ = lpSceneUIMng.Init((std::move(std::make_unique<TitleScene>())));
	scene_ = lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(0))));
	std::chrono::system_clock::time_point now, old;
	now = std::chrono::system_clock::now();

	Fps fps;

	//FontHandle_ = CreateFontToHandle("HG明朝E", 50, 5, DX_FONTTYPE_NORMAL);
	FontHandle_ = CreateFontToHandle("UDデジタル教科書体", 50, 5, DX_FONTTYPE_NORMAL);

	lpSceneUIMng.Data_SetDef();

	while (ProcessMessage() == 0 && !gameEndFlg)
	{
		//更新
		_dbgStartDraw();

		// 経過時間の計算
		old = now;
		now = std::chrono::system_clock::now();
		auto elTime = now - old;
		auto msec = std::chrono::duration_cast<std::chrono::microseconds>(elTime).count();
		double delta = msec / 1000000.0;
		deltaTime_ = delta;

		fps.Update();

		lpCntMng.GetController()->Update();
		scene_ = scene_->Update(delta, std::move(scene_));
		lpSceneUIMng.Update(delta);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();

		fps.Draw();

		Draw(delta);
		lpSceneUIMng.Draw(delta);
		cnt_++;
		ScreenFlip();
		fps.Wait();//待機
		//これがあるとモニターのHzが60以上でも60に固定してくれる！

		if (CheckHitKey(KEY_INPUT_ESCAPE)&& CheckHitKey(KEY_INPUT_F1))
		{
			gameEndFlg = true;
		}
	}
}

uniqueScene SceneManager::Update(void)
{
	return std::move(scene_);
}

void SceneManager::Draw(double delta)
{
	scene_->Draw(delta);

	//_dbgDrawFormatString(0, 1000, 0xffffff, "%d", GetDrawCallCount());
	_dbgDraw();
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

int SceneManager::GetFontHandle(void)
{
	return FontHandle_;
}

void SceneManager::SetMultiFlg(bool flg)
{
	multiFlg_ = flg;
}

bool SceneManager::GetMultiFlg(void)
{
	return multiFlg_;
}

Vector2Int SceneManager::GetScreenSize(void)
{
	Vector2Int ScreenSize = { ScreenSizeX,ScreenSizeY };
	return ScreenSize;
}

void SceneManager::SetEndFlg(bool flg)
{
	gameEndFlg = flg;
}

void SceneManager::SetSoundVol(int vol)
{
	soundVol = vol;
}

int SceneManager::GetSoundVol()
{
	return soundVol;
}

bool SceneManager::SysInit(void)
{
	SetWindowIconID(ICON_1);
	// システム処理
	SetWindowText("NotScrap");
	SetGraphMode(ScreenSizeX, ScreenSizeY, 16);

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	ChangeWindowMode(false);

	if (DxLib_Init() == -1)
	{
		return false;
	}
	if (Effekseer_Init(8000)==-1)
	{
		DxLib_End();
		return false;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//キー情報初期化
	_dbgSetup(ScreenSizeX, ScreenSizeY, 255);
	// マウスを表示状態にする
	SetMouseDispFlag(false);
	//マウスを画面外に行かないようにする
	SetValidMousePointerWindowOutClientAreaMoveFlag(true);
	return true;
}

Scene SceneManager::GetSceneID(void)
{
	return scene_->GetSceneID();
}

SceneManager::SceneManager()
{
	initFlg_ = SysInit();
}

SceneManager::~SceneManager()
{
}
