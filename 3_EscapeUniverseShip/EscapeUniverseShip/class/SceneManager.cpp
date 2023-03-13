#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "SceneManager.h"
#include"Input/KeyInput.h"
#include"TitleScene.h"
#include"ClearScene.h"
#include"SelectScene.h"
#include"../Image/icon/icon.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

constexpr float VIEW_SIZE_X = 1280.0f;
constexpr float VIEW_SIZE_Y = 720.0f;

bool SceneManager::SysInit(void)
{
	SetGraphMode(static_cast<int>(VIEW_SIZE_X), static_cast<int>(VIEW_SIZE_Y), 32);
	ChangeWindowMode(true);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	SetWindowText("EscapeUniverseShip");
	SetWindowIconID(ICON_1);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseerに2D描画の設定をする。
	Effekseer_Set2DSetting(static_cast<int>(VIEW_SIZE_X), static_cast<int>(VIEW_SIZE_Y));

	//_dbgSetup(static_cast<int>(ViewSizeX), static_cast<int>(ViewSizeY), 255);
	return true;
}

SceneManager::SceneManager()
{
	deltaTime_ = 0.0;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(void)
{
	tickCount_ = std::chrono::system_clock::now();
}

void SceneManager::Run(void)
{
	if (!SysInit())
	{
		return;
	}
	Init();
	scene_ = std::make_unique<TitleScene>();
	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		auto tick = std::chrono::system_clock::now();
		deltaTime_ = std::chrono::duration_cast<std::chrono::microseconds>(tick - tickCount_).count() / 1000000.0f;
		tickCount_ = tick;

		scene_ = scene_->Update(std::move(scene_));

		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();

		scene_->Draw();
		ScreenFlip();
	}
	Effkseer_End();
	DxLib_End();
}

Vector2 SceneManager::GetViewSize(void)
{
	return { VIEW_SIZE_X,VIEW_SIZE_Y };
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}
