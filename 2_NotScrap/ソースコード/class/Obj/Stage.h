#pragma once
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <btBulletDynamicsCommon.h>
#include "../Common/Geometry.h"

#define lpStage Stage::GetInstance()

using loadgim = std::pair<int, Vector3>;
class SceneManager;

using namespace std;

//3D�I�u�W�F�N�g���
struct Object {
	int path_;	//�t�@�C���p�X
	string objectName;	//�I�u�W�F�N�g��
	Vector3 pos_;	//���W
	Vector3 rotate_;	//��]
	Object(int path, string name, const Vector3& pos, const Vector3& rotate) :
		path_(path),
		objectName(name),
		pos_(pos),
		rotate_(rotate) {};
};

//3D�I�u�W�F�N�g���(1��1)
struct PairObject {
	int path_;
	int path2_;
	string objectName_;
	string objectName2_;
	Vector3 pos_;
	Vector3 pos2_;
	Vector3 rotate_;
	Vector3 rotate2_;
	PairObject(int path,
	int path2,
	string name,
	string name2,
	Vector3& pos,
	Vector3& pos2,
	Vector3& rotate,
	Vector3& rotate2):
		path_(path),
		path2_(path2),
		objectName_(name),
		objectName2_(name2),
		pos_(pos),
		pos2_(pos2),
		rotate_(rotate),
		rotate2_(rotate2) {};
};

struct CollObj {
	int handle;
	Vector3 pos;
	Vector3 vec;
};

enum class STAGE_NAME	//�X�e�[�W�̖��O
{
	TITLE,
	SELECT,
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	MAX
};

class Stage
{

public:
	static Stage& GetInstance(void)
	{
		static Stage s_Instance;
		return s_Instance;
	}
	//�R���X�g���N�^
	Stage(void);

	//�f�X�g���N�^
	~Stage(void);
	void Remove(void);
	//������
	bool Init(int num);
	void InitBullet(int Gravity);
	//�X�V
	void Update(void);

	//�`��
	void Draw(void);

	//�񓯊��ǂݍ���
	bool ReInit(void);
	//bullet�̃}�b�v�̍쐬
	void SetRigidBodies(void);

	btRigidBody* BulletCreateMeshShape(string name, int num, Vector3 pos, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bullet�̃J�v�Z���̍쐬
	btRigidBody* BulletCreateCapsule(Vector3 pos, btScalar radius, btScalar height, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bullet�̃{�b�N�X�̍쐬
	btRigidBody* BulletCreateBox(int mass, Vector3 pos, btVector3 size, float Threshold, float collsize, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	btRigidBody* BulletCreateBox(int mass, Vector3 pos, btQuaternion qrot, btVector3 size, float Threshold, float collsize, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bullet�̋��̍쐬
	btRigidBody* BulletCreateSphere(Vector3 pos, btScalar radius, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bullet��rigidbody�̍쐬
	btRigidBody* CreateRigidBody(double mass, const btTransform& init_trans, btCollisionShape* shape, int index);
	btRigidBody* CreateRigidBody(double mass, btVector3 LinearVec, btVector3 AngularVec, const btTransform& init_trans, btCollisionShape* shape, int index);
	//bullet��body����ɂ���
	void CleanBullet(void);
	//bullet�̈��������Ȃ���
	void CleanBulletObj(btRigidBody* body);

	//�����蔻���Ԃ�
	//S��
	std::vector<int>& GetSouthStageCollList(void);

	//N��
	std::vector<int>& GetNorthStageCollList(void);

	//�X�e�[�W
	std::vector<int>& GetStageCollList(void);

	//Box
	std::vector<CollObj>& GetNMagBoxCollList(void);
	std::vector<CollObj>& GetSMagBoxCollList(void);

	//�S�[������p
	std::vector<loadgim>& GetGoalCollList(void);

	//���ׂĂ̓����蔻��
	std::vector<std::vector<int>>& GetALLStageCollList(void);

	//���X�g��Ԃ�
	std::vector<Object>& GetMagnetList(void);

	//�R�C��
	std::vector<Object>& GetCoinList(void);

	//�G
	std::vector<Object>& GetEnemyList(void);

	//�G�F�ˌ�
	std::vector<Object>& GetShootEnemyList(void);

	//����
	std::vector<Object>& GetAccessoryList(void);

	//���G�t�F�N�g
	std::vector<Object>& GetSmokeList(void);

	//����
	std::vector<Object>& GetGearList(void);

	//�{�^���Ɣ�
	std::vector<PairObject>& GetDoorList(void);

	//�{�^���Ə�
	std::vector<PairObject>& GetFloorList(void);

	//����
	std::vector<Object>& GetMidPointList(void);

	//�x���Ŕ�
	std::vector<Object>& GetWarningSignList(void);


	//�X�N���b�v
	std::vector<Object>& GetScrapList(void);

	//�I�����ꂽ�X�e�[�W��Ԃ�Ԃ�
	STAGE_NAME GetStage(void);

	// Bullet
	btDynamicsWorld* g_dynamicsworld;	//!< Bullet���[���h

	//Wstring�ɕϊ�
	wstring ConvertToWstring(string& str);

private:
	//SceneManager* mSceneManager;
	//��񃊃X�g
	//����
	std::vector<Object> magnetList_;

	//�R�C��
	std::vector<Object> coinList_;

	//�G
	std::vector<Object> enemyList_;

	//�G�F�ˌ�
	std::vector<Object> shootEnemyList_;

	//����
	std::vector<Object> accessoryList_;

	//���G�t�F�N�g
	std::vector<Object> smokeList_;

	//����
	std::vector<Object> gearList_;

	//����
	std::vector<Object> midPointList_;

	//�x���Ŕ�
	std::vector<Object> warningSignList_;

	//�X�N���b�v
	std::vector<Object> scrapList_;

	//�{�^���Ɣ�
	std::vector<PairObject> doorList_;

	//�{�^���Ə�
	std::vector<PairObject> floorList_;

	//�����蔻�胂�f��
	int collisionHandle_;
	//�w�i���f��
	int backGroundHandle_;

	//�����蔻��p���X�g
	//����
	std::vector<int> SouthStageCollList_;	//S��

	//N
	std::vector<int> NorthStageCollList_;	//N��

	//�X�e�[�W
	std::vector<int> StageCollList_;

	//���΂̔�
	std::vector<CollObj> NMagBoxCollList_;
	std::vector<CollObj> SMagBoxCollList_;

	//���ׂĂ̓����蔻�胊�X�g
	std::vector<std::vector<int>> AllStageCollList_;

	//�S�[���n�_
	std::vector<loadgim> GoalCollList_;

	//�X�e�[�W�̎��
	STAGE_NAME stageName_;

	// Bullet
	btRigidBody* btstageBody_;
	btAlignedObjectArray<btCollisionShape*> g_collisionshapes;
	int GravityPow_;

	void SetStagePos(Vector3 pos);

	//Object�폜
	void ClearList(void);
	//���΃��f���ǂݍ���
	void LoadMagnet(void);
	//�f�[�^�ǂݍ���
	void LoadObjectData(std::vector<Object>& objList, const TCHAR* fName);
	//2�̃f�[�^�ǂݍ���
	void LoadPairObjectData(std::vector<PairObject>& objList, const TCHAR* fName, const TCHAR* fName2);

	//S�ɓǂݍ���
	void LoadSMagnet(wstring magnetName, const TCHAR* fName);
	//������S�ɕ�
	void LoadSWall_TMagnet(wstring magnetName, const TCHAR* fName);

	//S�ɓǂݍ���
	void LoadNMagnet(wstring magnetName, const TCHAR* fName);
	//������N�ɕ�
	void LoadNWall_TMagnet(wstring magnetName, const TCHAR* fName);

	//���^���Γǂݍ���
	void LoadNBoxMagnet(wstring magnetName, const TCHAR* fName);
	void LoadSBoxMagnet(wstring magnetName, const TCHAR* fName);

	//�f�o�b�O�\��
	void DebugDraw(void);
};
