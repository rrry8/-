#pragma once
#include "../Obj.h"
#include "../Unit/Player.h"
//�������
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
	//�X�V����
	virtual void Update(std::vector<std::shared_ptr<Player>>& Plyaer, GimmickList& gimmickList) = 0;
	//���f���n���h����Ԃ�
	virtual int GetModelH(void) = 0;
	virtual  btRigidBody* GetRigidBody(void) = 0;
	//���W��ݒ�
	virtual void SetPos(Vector3 pos) = 0;
	//�t���O��ݒ�
	void SetFlg(bool flg) { flg_ = flg; }
	virtual void SetMoveVec(Vector3 vec) = 0;
	void SetVec(Vector3 ve) { moveVec_ = ve; };
	//���W��Ԃ�
	virtual Vector3 GetPos() = 0;
	//�I�u�W�F�N�g�^�C�v��Ԃ�
	virtual objType GetObjType(void) = 0;
	//�����t���O��ݒ�
	virtual void SetIsAlive(bool flag) {};
	//�h�A�ƃ{�^���p�̃t���O
	bool flg_;
protected:
	//������
	int num_;
	//�h�A���W
	Vector3 doorPos_;
	//�����t���O
	bool isAlive_;

private:

};

