#pragma once
#include<vector>
#include<string>
#include<map>
#include"../../Geometry.h"

#define lpSoundMng SoundMng::GetInstance()
using VecInt = std::vector<int>;

class SoundMng
{
public:
	static SoundMng& GetInstance()
	{
		static SoundMng s_Instance;
		return s_Instance;
	}
	const VecInt& GetID(std::string key);
private:
	SoundMng();
	~SoundMng();

	//サウンドのテーブル
	std::map<std::string, VecInt> soundMap_;
};
