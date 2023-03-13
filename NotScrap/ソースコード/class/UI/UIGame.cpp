#include <DxLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Scene/SelectScene.h"
#include "UIGame.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../Obj/CheckHit.h"
//#include "../Transition/TransitionScene.h"

UIGame::UIGame()
{
	Init();
	DrawOwnScreen(0.0);
}


UIGame::~UIGame()
{
}

bool UIGame::Init(void)
{
	

	return true;
}

void UIGame::Update(double delta)
{
	
	DrawOwnScreen(delta);
}


void UIGame::DrawOwnScreen(double delta)
{
	

}

bool UIGame::GetLoadingEndFlg(void)
{
	return true;
}

void UIGame::SetIsNext(bool flg)
{
}

