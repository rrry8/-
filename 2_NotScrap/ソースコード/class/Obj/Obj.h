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

constexpr float MOVE_RANGE = 2000.0f;	//�J�����𒆐S�ɓG�����삷��͈�

//�G�t�F�N�g�\����
struct Effect
{
	int resource = -1;     //�n���h��
	int playhandle = -1;   //�v���C�n���h��
	Vector3 pos; //�ʒu
	float angle = 0.0f; //�p�x
};

//�g�����X�t�H�[��
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
	//�J�����͈͓̔����ǂ����̔���(true:�͈͓�/false:�͈͊O)
	bool CheckWithinRange(Vector3 pos, Vector3 playerpos);
	bool CheckWithinRangeXonly(Vector3 pos, Vector3 playerpos);
	bool CheckWithinRangeXonly(Vector3 pos, Vector3 playerpos, float size);
private:
protected:

	bool isMove_;

	Vector3 pos_ = { 0,0,0 };
	Vector3 rot_ = { 0,0,0 };

	//�傫��
	Vector3 size_;

	//�F
	int color_;

	//�ړ����x
	float speed_;

	Vector3 movePow_;
	Vector3 moveVec_;
	Vector3 cameraPos_;
};

