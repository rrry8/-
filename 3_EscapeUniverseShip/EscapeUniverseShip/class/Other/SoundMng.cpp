#include<DxLib.h>
#include "SoundMng.h"

const VecInt& SoundMng::GetID(std::string key)
{
	//画像を読み込んでいるかのチェック
	if (!soundMap_.count(key))
	{
		//読み込んでいない場合リサイズして画像をロードする
		soundMap_[key].resize(1);
		soundMap_[key][0] = LoadSoundMem(key.c_str());
	}
	return soundMap_[key];
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
}
