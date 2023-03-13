#include <DxLib.h>
#include "CrossOver.h"

CrossOver::CrossOver(double limitTime, uniqueScene beforScene, uniqueScene afterScene):
	TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	//DrawOwnScreen(0.0);
}

CrossOver::~CrossOver()
{
}

bool CrossOver::UpdateTransition(double delta)
{
	count += delta;
	if (count > limitTime_)
	{
		//I‚í‚Á‚Ä‚¢‚é
		return true;
	}
	//I‚í‚Á‚Ä‚¢‚È‚¢
	return false;
}

void CrossOver::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255.0 * count / limitTime_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw(delta);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}


