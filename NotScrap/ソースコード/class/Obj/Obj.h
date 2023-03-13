#pragma once
#include <list>
#include <vector>
#include <memory>
#include <string>
#include "../Common/Geometry.h"
#include "../Common/ModelMng.h"
#include "../../tmx/TmxObj.h"
#include "Stage.h"
#include "../Common/Quaternion.h"
#include "../Input/ControllerMng.h"

constexpr float MOVE_RANGE = 2000.0f;	//カメラを中心に敵が動作する範囲

//エフェクト構造体
struct Effect
{
	int resource = -1;     //ハンドル
	int playhandle = -1;   //プレイハンドル
	Vector3 pos; //位置
	float angle = 0.0f; //角度
};

//トランスフォーム
struct Transform
{
	VECTOR scale;
	VECTOR pos;
	Quaternion quaRot;
	Quaternion quaRotLocal;
};

enum class EnemyType
{
	WALK,
	SHOOT,
	MAX
};

enum class State
{
	Idle,
	Run,
	Fall,
	Magnet,
	MagnetWeak,
	MagnetFall,
	MagnetFallSmooth,
	MagInertia,
	Respawn,
	DismemberDeath,
	Death,
	Stick,
	Goal,
	Max
};

class Enemy;
class Gimmick;
class MagBox;
class Unit;
class Coin;

using EneymyList = std::list<std::shared_ptr<Unit>>;
using GimmickList = std::vector<std::shared_ptr<Gimmick>>;
using MagBoxList = std::list<std::shared_ptr<MagBox>>;
using CoinList = std::list<std::shared_ptr<Coin>>;
using UnitList = std::list<std::unique_ptr<Unit>>;
using HitHandle = std::pair<CoinList, bool>;

static  Vector3 Vector3Zero = {0,0,0};
static  Vector3 Vector3Up = { 0,1,0 };
static  Vector3 Vector3Down = { 0,-1,0 };
static  Vector3 Vector3Right = { 1,0,0 };
static  Vector3 Vector3Left = { -1,0,0 };

class Obj
{
public:
	Obj();
	~Obj();

	virtual bool Init(void) = 0;
	virtual bool Init(Vector3 pos) { return false; };
	virtual bool Init(int handle, Vector3 pos) { return false; };
	virtual void Update(void) {};
	virtual void Draw(void);
	//カメラの範囲内かどうかの判定(true:範囲内/false:範囲外)
	bool CheckWithinRange(Vector3 pos, Vector3 playerpos);
	bool CheckWithinRangeXonly(Vector3 pos, Vector3 playerpos);
	bool CheckWithinRangeXonly(Vector3 pos, Vector3 playerpos, float size);
private:
protected:

	bool isMove_;

	Vector3 pos_ = { 0,0,0 };
	Vector3 rot_ = { 0,0,0 };

	//大きさ
	Vector3 size_;

	//色
	int color_;

	//移動速度
	float speed_;

	Vector3 movePow_;
	Vector3 moveVec_;
	Vector3 cameraPos_;
};

