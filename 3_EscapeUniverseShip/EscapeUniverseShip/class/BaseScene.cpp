#include<DxLib.h>
#include "BaseScene.h"
#include"Other/SoundMng.h"

BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSizeX_, &screenSizeY_);
	screenID_ = MakeScreen(screenSizeX_, screenSizeY_, true);
}

BaseScene::~BaseScene()
{
}

void BaseScene::Draw()
{
	DrawGraph(0, 0, screenID_, true);
}

void BaseScene::DrawplusOffset(Vector2 offset)
{
	DrawGraph(offset.x, offset.y, screenID_, true);
}

void BaseScene::StopSound()
{
	if (CheckSoundMem(lpSoundMng.GetID("./sound/early.mp3")[0]))
	{
		StopSoundMem(lpSoundMng.GetID("./sound/early.mp3")[0]);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0]))
	{
		StopSoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0]);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./sound/last.mp3")[0]))
	{
		StopSoundMem(lpSoundMng.GetID("./sound/last.mp3")[0]);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./sound/ex.mp3")[0]))
	{
		StopSoundMem(lpSoundMng.GetID("./sound/ex.mp3")[0]);
	}
}

void BaseScene::ChangeSoundVolume(int volume)
{
	if (CheckSoundMem(lpSoundMng.GetID("./sound/early.mp3")[0]))
	{
		ChangeVolumeSoundMem(volume,lpSoundMng.GetID("./sound/early.mp3")[0]);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0]))
	{
		ChangeVolumeSoundMem(volume,lpSoundMng.GetID("./sound/mid.mp3")[0]);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./sound/last.mp3")[0]))
	{
		ChangeVolumeSoundMem(volume,lpSoundMng.GetID("./sound/last.mp3")[0]);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./sound/ex.mp3")[0]))
	{
		ChangeVolumeSoundMem(volume,lpSoundMng.GetID("./sound/ex.mp3")[0]);
	}
}
