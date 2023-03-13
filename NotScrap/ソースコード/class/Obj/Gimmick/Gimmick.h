#pragma once
#include "../Obj.h"
#include "../Unit/Player.h"
//上限下限
struct Limit {
	float upper, lower;
};


enum class objType
{
	NMagBox,
	SMagBox,
	PressurePlate,
	PresureDoor,
	ElectricPole,
	Accessory,
	Tutorial,
	Smoke,
	Gear,
	MidPoint,
	Crane,
	BeltConveyor,
	WarningSign,
};

enum class TutorialType
{
	TutorialN,
	TutorialS,
	TutorialKey,
	TutorialShift,
	TutorialMagBox,
	TutorialBullet,
	TutorialDeath,
	TutorialStick,
};
enum class DoorType
{
	Door,
SidewaysDoor,
};

class Gimmick :
    public Obj
{
public:
	Gimmick();
	~Gimmick();
	//更新処理
	virtual void Update(std::vector<std::shared_ptr<Player>>& Plyaer, GimmickList& gimmickList) = 0;
	//モデルハンドルを返す
	virtual int GetModelH(void) = 0;
	virtual  btRigidBody* GetRigidBody(void) = 0;
	//座標を設定
	virtual void SetPos(Vector3 pos) = 0;
	//フラグを設定
	void SetFlg(bool flg) { flg_ = flg; }
	virtual void SetMoveVec(Vector3 vec) = 0;
	void SetVec(Vector3 ve) { moveVec_ = ve; };
	//座標を返す
	virtual Vector3 GetPos() = 0;
	//オブジェクトタイプを返す
	virtual objType GetObjType(void) = 0;
	//生存フラグを設定
	virtual void SetIsAlive(bool flag) {};
	//ドアとボタン用のフラグ
	bool flg_;
protected:
	//複製数
	int num_;
	//ドア座標
	Vector3 doorPos_;
	//生存フラグ
	bool isAlive_;

private:

};

