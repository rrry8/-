#pragma once
#include<memory>
#include"../Geometry.h"
class BaseScene;
class Camera;

using UniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// 初期化処理
	virtual void Init(void) = 0;

	// 更新
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// 描画処理
	virtual void Draw();
	//オフセット付き
	virtual void DrawplusOffset(Vector2 offset);
	virtual void DrawScreen(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	//サウンドの停止
	virtual void StopSound();

	//サウンドの音量変更
	virtual void ChangeSoundVolume(int volume);

private:

protected:

	//描画する画面
	int screenID_;
	//画面サイズｘ
	int screenSizeX_;
	//画面サイズｙ
	int screenSizeY_;
};