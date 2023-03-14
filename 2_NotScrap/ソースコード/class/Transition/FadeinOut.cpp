#include <DxLib.h>
#include "FadeinOut.h"
FadeinOut::FadeinOut(double limitTime, uniqueScene beforScene, uniqueScene afterScene) : TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	/*limitTime_ = 3;*/
}

FadeinOut::FadeinOut(double limitTime, Transition tras, uniqueScene afterScene) : TransitionScene(std::move(afterScene))
{
	limitTime_ = limitTime;
	tras_ = tras;
	/*limitTime_ = 3;*/
}

FadeinOut::~FadeinOut()
{
}

bool FadeinOut::UpdateTransition(double delta)
{
	count += delta;
	
	if (count > limitTime_)
	{
		return true;
	}
	return false;
}

void FadeinOut::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	auto alpha = static_cast <int>(255.0 * count / (limitTime_ / 2));

	if (beforScene_ != nullptr)
	{
		if (alpha < 255)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
			beforScene_->Draw(delta);
		}
		else
		{

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha - 255);

			afterScene_->Draw(delta);

		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
	}
	else
	{
		if (tras_ == Transition::Out)
		{
			if (alpha < 255)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
				afterScene_->Draw(delta);
				
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
			}
		}
		if (tras_ == Transition::In)
		{
			if (alpha < 255)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);

			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha - 255);

				afterScene_->Draw(delta);
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
		}
		if (tras_ == Transition::FadeInOut)
		{
			
			if (alpha < 255)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
				afterScene_->Draw(delta);
				ff = true;
			}
			else
			{
				if (ff)
				{
					afterScene_ = afterScene_->Update(delta, std::move(afterScene_));
					ff = false;
				}
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha - 255);

				afterScene_->Draw(delta);
			}
			
		}
		
	}
}

//void FadeinOut::Draw(double delta)
//{
//	ClsDrawScreen();
//	auto alpha = static_cast < int>(255.0 * count / (limitTime_/2));
//	
//	if (alpha < 255)
//	{
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
//		beforScene_->Draw(delta);
//	}
//	else
//	{
//		
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha-255 );
//		afterScene_->Draw(delta);
//	}
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
//}
