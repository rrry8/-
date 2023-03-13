#include<DxLib.h>
#include "TransitionScene.h"
#include"../SceneManager.h"
#include"../../_debug/_DebugConOut.h"


TransitionScene::TransitionScene(UniqueScene beforeScene, UniqueScene afterScene)
{
	beforeScene_ = std::move(beforeScene);
	afterScene_ = std::move(afterScene);
	count_ = 0.0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforeScene_->Draw();
}

TransitionScene::~TransitionScene()
{
}

void TransitionScene::Init(void)
{

}

UniqueScene TransitionScene::Update(UniqueScene ownScene)
{
	if (UpdateTransition(0.016))
	{
		afterScene_->DrawScreen();
		return std::move(afterScene_);
	}
	DrawScreen();
	return ownScene;
}

void TransitionScene::Release(void)
{
}

