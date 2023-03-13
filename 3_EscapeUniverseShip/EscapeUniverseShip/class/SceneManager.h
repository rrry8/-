#pragma once
#include<chrono>
#include "BaseScene.h"
#include"../Geometry.h"

#define lpSceneMng SceneManager::GetInstance()

class SceneManager
{
public:

	//ƒVƒ“ƒOƒ‹ƒgƒ“
	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}

	void Init(void);
	void Run(void);
	Vector2 GetViewSize(void);
	double GetDeltaTime(void);

private:
	bool SysInit(void);
	SceneManager();
	~SceneManager();

	UniqueScene scene_;
	
	std::chrono::system_clock::time_point tickCount_;
	double deltaTime_;
};
