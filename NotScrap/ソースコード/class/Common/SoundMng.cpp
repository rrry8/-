#include <DxLib.h>
#include "SoundMng.h"
#include "../../_debug/_DebugConOut.h"


const VecInt& SoundMng::GetID(std::string key)
{
	if (soundMap_.count(key) == 0)//‰¹‚ğ“Ç‚İ‚ñ‚Å‚¢‚é‚©‚Ç‚¤‚©
	{
		//“Ç‚İ‚ñ‚Å‚È‚¢ê‡‚Í“Ç‚İ‚Ş
		soundMap_[key].resize(1);
		soundMap_[key][0] = LoadSoundMem(key.c_str());
	}
	return soundMap_[key];//‰¹‚Ìƒnƒ“ƒhƒ‹‚ğ“n‚·
}

void SoundMng::PlayingSound(std::string name)
{
	if (CheckSoundMem(lpSoundMng.GetID(name)[0]) != 0)
	{
		StopSoundMem(lpSoundMng.GetID(name)[0]);
	}
	PlaySoundMem(lpSoundMng.GetID(name)[0], DX_PLAYTYPE_BACK);
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
}

void SoundMng::ChangeVolume(int Vol, std::string name)
{
	ChangeVolumeSoundMem(Vol, GetID(name)[0]);
}