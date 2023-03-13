#pragma once
#include "../../Obj.h"
#include "../../../Scene/GameScene.h"
#include<vector>
#include "../../../Common/TextureMap.h"
class AnimationController;

using VecInt = std::vector<int>;

enum class CoinState
{
	Idle,
	Get,
	None,
	Max
};

class Coin :
	public Obj
{
public:
	Coin();
	Coin(Scene sceneID, Vector3 pos, int num);
	~Coin();

	bool ReSet(Vector3 pos);
	bool Init(void);
	bool Init(Vector3 pos)override;
	void Update(void) override;
	void Draw(void) override;

	void SetCoinPos(int handle, Vector3 Coinpos);
	Vector3 GetCoinPos();
	void SetCoinState(CoinState state);
	CoinState GetCoinState(void);
	void SetCoinfFlg(bool flg);
	bool GetCoinFlg();
	std::string GetCoinPath(void);
	bool GetClearFlg();
	int GetCoinNumber(void);

	int GetCoinHandle(void);

private:
	bool GetFlg;//ƒRƒCƒ“‚ªŽæ“¾‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©
	bool IsAlive;
	CoinState mCoinState;

	std::unique_ptr<AnimationController> animController_;

	int num_;
	Scene nowSceneID_;

	bool clearFlg;
	TextureMap texmap;
};
