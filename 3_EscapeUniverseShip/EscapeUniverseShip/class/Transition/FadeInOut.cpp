#include<DxLib.h>
#include "FadeInOut.h"

FadeInOut::FadeInOut(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene):
	TransitionScene(std::move(beforeScene), std::move(afterScene))
{
	limitTime_ = LimitTime;
	DrawScreen();
}

FadeInOut::~FadeInOut()
{
}

bool FadeInOut::UpdateTransition(double delta)
{
	count_ += delta;
	if (count_ >= limitTime_)
	{
		return true;
	}
	return false;
}

void FadeInOut::DrawScreen()
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255.0 * count_ / limitTime_) * 2;
	if (count_ < limitTime_ / 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
		beforeScene_->Draw();
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha - 255);
		afterScene_->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

