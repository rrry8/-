#include "SmokeAcs.h"
#include<EffekseerForDXLib.h>
SmokeAcs::SmokeAcs(Vector3 pos)
{
	Init(pos);
}

SmokeAcs::~SmokeAcs()
{
	DeleteEffekseerEffect(Smoke_.resource);
}

bool SmokeAcs::Init(Vector3 pos)
{
	pos_ = pos;
	//Smoke_.pos = pos_ + Vector3{ 0,0,-500 };

	time_ = 0;
	
	Smoke_.resource = LoadEffekseerEffect("./source/effect/exhaust.efkefc", 50.0f);
	Smoke_.playhandle = -1;
	Smoke_.pos = pos_;
	flag_ = false;
	return false;
}

void SmokeAcs::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
	
		if (!CheckWithinRange(pos_, PlyaerList[0]->GetPos()))
		{
			return;
		}
		if ((time_ / 25) % 8 == 3)
		{

			//TRACE("hhhhhh%d\n", (time_ / 10) % 8);
			if (!flag_)
			{
				flag_ = true;
				//TRACE("hhhhhh%d\n", (time_ / 10) % 8);
				Smoke_.playhandle = PlayEffekseer3DEffect(Smoke_.resource);
				Smoke_.pos = pos_;
				SetPosPlayingEffekseer3DEffect(Smoke_.playhandle, Smoke_.pos.x, Smoke_.pos.y, Smoke_.pos.z);
			}
		}
		else
		{
			flag_ = false;
		}
	
	
	
	time_++;
}

void SmokeAcs::Draw(void)
{
}
