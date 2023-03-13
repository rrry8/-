#include <string>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>
#include <memory>
#include "Stage.h"
#include "../../_debug/debugDraw.h"
#include "../Common/Geometry.h"
#include "../../_debug/_DebugConOut.h"
#include "../Common/ModelMng.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

Stage::Stage()
{
	collisionHandle_ = -1;
	backGroundHandle_ = -1;
	GravityPow_ = 0;
	g_dynamicsworld = nullptr;
	stageName_ = STAGE_NAME::MAX;
	//Init();
}

Stage::~Stage()
{	
}

void Stage::Remove()
{
	//Object�폜
	ClearList();
	CleanBullet();
}

bool Stage::Init(int num)
{
	switch (num)
	{
	case 0://TITLE
		//�����蔻�胂�f��
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColTitleStage.mv1");
		//�w�i���f��
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/TitleStage.mv1");
		//�����蔻��p���X�g�ɒǉ�
		StageCollList_.emplace_back(collisionHandle_);
		//�X�e�[�W�̎��
		stageName_ = STAGE_NAME::TITLE;
		break;

	case 1://SLECT
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColSelectStage.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/SelectStage.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::SELECT;
		break;

	case 2://Stage1
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage1.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage1.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_1;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{24000,1000,0});
		break;
	case 3://Stage2
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage2.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage2.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_2;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{10791,1947,0});
		break;
	case 4://Stage3
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage3.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage3.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_3;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{13370,0,0});
		break;
	case 5://Stage4
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage4.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage4.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_4;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{ 13370,0,0});
		break;
	case 6://Stage5
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage5.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage5.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_5;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{ 8264,3272,0 });
		break;
	case 7://Stage6
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage6.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage6.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_6;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{ 3784,2270,0 });
		break;
	case 8://Stage7
		collisionHandle_ = MV1LoadModel("./source/Model/Stage/ColStage7.mv1");
		backGroundHandle_ = MV1LoadModel("./source/Model/Stage/Stage7.mv1");
		StageCollList_.emplace_back(collisionHandle_);
		stageName_ = STAGE_NAME::STAGE_7;
		GoalCollList_.emplace_back(MV1LoadModel("./source/Model/Magnet/Goal.mv1"), Vector3{-1863,63,0});
		break;

	default:
		break;
	}
	
	GravityPow_ = 1960;

	InitBullet(GravityPow_);
	
	return true;
}

void Stage::InitBullet(int Gravity)
{
	// �Փˌ��o���@�̑I��(�f�t�H���g��I��)
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);

	//// �u���[�h�t�F�[�Y�@�̐ݒ�(Dynamic AABB tree method)
	// ���[���h�̍L��
	btVector3 worldAabbMin(-100000, -100000, -1000);
	btVector3 worldAabbMax(100000, 100000, 1000);

	// �v���L�V�̍ő吔�i�Փ˕��̂̂悤�Ȃ��́j
	int maxProxies = 1024;

	// broadphase�̍쐬�iSAP�@�j
	btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	//// �S��(���̊ԃ����N)�̃\���o�ݒ�
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	g_dynamicsworld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	// �d�͉����x�̐ݒ�(OpenGL�ɍ��킹��y���������㉺�����ɂ���)
	g_dynamicsworld->setGravity(btVector3(0, -Gravity, 0));
}

bool Stage::ReInit(void)
{
	switch (stageName_)
	{
	case STAGE_NAME::TITLE:
		//�X�e�[�W�̔z�u���W�ݒ�
		SetStagePos({ -1000.0f, 0.0f, 0.0f });
		//�R���W���������\�z
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		break;
	case STAGE_NAME::SELECT:
		SetStagePos({ -1500.0f, 0.0f, 0.0f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		break;
	case STAGE_NAME::STAGE_1:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		//�f�[�^�ǂݍ���
		//����
		LoadObjectData(magnetList_,"./source/Data/Magnet/MagnetData1.dat");
		//�e��łG
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData1.dat");
		//���G�t�F�N�g
		LoadObjectData(smokeList_, "./source/Data/Gimmick/SmokeData1.dat");
		//�R�C��
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData1.dat");
		//�G
		LoadObjectData(enemyList_, "./source/Data/Enemy/EnemyData1.dat");
		//����
		LoadObjectData(accessoryList_, "./source/Data/Accessory/AccessoryData1.dat");
		//���Ԓn�_
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData1.dat");
		//�{�^���Ɣ�
		LoadPairObjectData(doorList_, "./source/Data/Gimmick/ButtonData1.dat", "source/Data/Gimmick/DoorData1.dat");
		
		//���΂̃��f���ǂݍ���
		LoadMagnet();

		//���Δz�u
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}

		break;
	case STAGE_NAME::STAGE_2:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		LoadObjectData(magnetList_, "./source/Data/Magnet/MagnetData2.dat");
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData2.dat");
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData2.dat");
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData2.dat");
		LoadObjectData(warningSignList_, "./source/Data/Gimmick/SignData2.dat");
		LoadMagnet();
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}
		break;
	case STAGE_NAME::STAGE_3:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		LoadObjectData(magnetList_, "./source/Data/Magnet/MagnetData3.dat");
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData3.dat");
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData3.dat");
		LoadObjectData(enemyList_, "./source/Data/Enemy/EnemyData3.dat");
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData3.dat");
		LoadMagnet();
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}
		break;
	case STAGE_NAME::STAGE_4:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		LoadObjectData(magnetList_, "./source/Data/Magnet/MagnetData4.dat");
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData4.dat");
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData4.dat");
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData4.dat");
		LoadPairObjectData(doorList_, "./source/Data/Gimmick/ButtonData4.dat", "source/Data/Gimmick/DoorData4.dat");
		LoadMagnet();
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}
		break;
	case STAGE_NAME::STAGE_5:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		LoadObjectData(magnetList_, "./source/Data/Magnet/MagnetData5.dat");
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData5.dat");
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData5.dat");
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData5.dat");
		LoadObjectData(warningSignList_, "./source/Data/Gimmick/SignData5.dat");
		LoadObjectData(gearList_, "./source/Data/Gimmick/GearData5.dat");
		LoadMagnet();
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}
		break;
	case STAGE_NAME::STAGE_6:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		LoadObjectData(magnetList_, "./source/Data/Magnet/MagnetData6.dat");
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData6.dat");
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData6.dat");
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData6.dat");
		LoadObjectData(gearList_, "./source/Data/Gimmick/GearData6.dat");
		LoadPairObjectData(doorList_, "./source/Data/Gimmick/ButtonData6.dat", "source/Data/Gimmick/DoorData6.dat");
		LoadMagnet();
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}
		break;
	case STAGE_NAME::STAGE_7:
		SetStagePos({ 0.0f, 0.0f, 1000.f });
		MV1SetupCollInfo(collisionHandle_, -1, 10, 10, 10);
		LoadObjectData(magnetList_, "./source/Data/Magnet/MagnetData7.dat");
		LoadObjectData(coinList_, "./source/Data/Coin/CoinData7.dat");
		LoadObjectData(shootEnemyList_, "./source/Data/Enemy/ShootEnemyData7.dat");
		LoadObjectData(midPointList_, "./source/Data/MidPoint/MidPointData7.dat");
		LoadObjectData(gearList_, "./source/Data/Gimmick/GearData7.dat");
		LoadPairObjectData(doorList_, "./source/Data/Gimmick/ButtonData7.dat", "source/Data/Gimmick/DoorData7.dat");
		LoadMagnet();
		for (auto& magnet : magnetList_)
		{
			MV1SetPosition(magnet.path_, magnet.pos_.toVECTOR());
		}
		break;
	case STAGE_NAME::MAX:
		break;
	default:
		break;
	}
	SetRigidBodies();
	btIDebugDraw* debug;
	debug = new debugDraw();
	debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe
		| btIDebugDraw::DBG_DrawContactPoints
		| btIDebugDraw::DBG_DrawConstraints);
	g_dynamicsworld->setDebugDrawer(debug);
	

	return true;
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	//���f����`��
	MV1DrawModel(backGroundHandle_);
}

//���Γǂݍ���
void Stage::LoadMagnet(void)
{
	//S��-------------
	//�l�p�`
	LoadSMagnet(L"S_Magnet", "./source/Model/Magnet/S_Magnet.mv1");
	//�l�p�`(������)
	LoadSMagnet(L"S_Magnet_T", "./source/Model/Magnet/S_Magnet_T.mv1");
	//��
	LoadSMagnet(L"S_Wall", "./source/Model/Magnet/S_WallC.mv1");
	//��(���� 1/2)
	LoadSMagnet(L"S_Wall_h", "./source/Model/Magnet/S_Wall_hC.mv1");
	//��(������)
	LoadSMagnet(L"S_Wall_T", "./source/Model/Magnet/S_Wall_T.mv1");
	//����
	LoadSMagnet(L"S_Sphere", "./source/Model/Magnet/S_Sphere.mv1");
	//����(������)
	LoadSMagnet(L"S_Sphere_T", "./source/Model/Magnet/S_Sphere_T.mv1");
	//��
	LoadSBoxMagnet(L"S_Box", "./source/Model/Magnet/S_Box.mv1");
	//N��--------------
	//�l�p�`
	LoadNMagnet(L"N_Magnet", "./source/Model/Magnet/N_Magnet.mv1");
	//�l�p�`(������)
	LoadNMagnet(L"N_Magnet_T", "./source/Model/Magnet/N_Magnet_T.mv1");
	//��
	LoadNMagnet(L"N_Wall", "./source/Model/Magnet/N_WallC.mv1");
	//��(���� 1/2)
	LoadNMagnet(L"N_Wall_h", "./source/Model/Magnet/N_Wall_hC.mv1");
	//��(������)
	LoadNMagnet(L"N_Wall_T", "./source/Model/Magnet/N_Wall_T.mv1");
	//����
	LoadNMagnet(L"N_Sphere", "./source/Model/Magnet/N_Sphere.mv1");
	//����(������)
	LoadNMagnet(L"N_Sphere_T", "./source/Model/Magnet/N_Sphere_T.mv1");
	//��
	LoadNBoxMagnet(L"N_Box", "./source/Model/Magnet/N_Box.mv1");
	//AllStageCollList_.emplace_back(SouthStageCollList_);
	//AllStageCollList_.emplace_back(NorthStageCollList_);
	AllStageCollList_.emplace_back(StageCollList_);
}

void Stage::LoadObjectData(std::vector<Object>& objList, const TCHAR* fName)
{
	int file = FileRead_open(fName);
	int version;
	FileRead_read(&version, sizeof(version), file);
	if (version == 6)
	{
		while (!FileRead_eof(file))
		{
			std::string name;	//���O
			Vector3 pos;	//���W
			Vector3 rot;	//��]
			char nameSize;
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			objList.emplace_back(0, name, pos, rot);
		}
	}
	FileRead_close(file);
}

void Stage::LoadPairObjectData(std::vector<PairObject>& objList, const TCHAR* fName, const TCHAR* fName2)
{
	int file = FileRead_open(fName);
	int file2 = FileRead_open(fName2);
	int version;
	int path = 0;
	std::string name;	//���O
	Vector3 pos;	//���W
	Vector3 rot;	//��]
	char nameSize;

	int path2 = 0;
	std::string name2;	//���O
	Vector3 pos2;	//���W
	Vector3 rot2;	//��]
	char nameSize2;
	FileRead_read(&version, sizeof(version), file);
	FileRead_read(&version, sizeof(version), file2);
	if (version == 6)
	{
		while (!FileRead_eof(file) && !FileRead_eof(file2))
		{

			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			FileRead_read(&nameSize2, sizeof(nameSize2), file2);
			name2.resize(static_cast<int>(nameSize2));
			FileRead_read(&name2[0], nameSize2, file2);
			FileRead_read(&pos2, sizeof(pos2), file2);
			FileRead_read(&rot2, sizeof(rot2), file2);
			objList.emplace_back(path, path2, name, name2, pos, pos2, rot, rot2);
		}
	}
	FileRead_close(file);
	FileRead_close(file2);
}

//S�Ɏ��Γǂݍ���
void Stage::LoadSMagnet(wstring magnetName, const TCHAR* fName)
{
	std::wstring objName;
	for (auto& magnet : magnetList_) {
		objName = ConvertToWstring(magnet.objectName);
		if (objName == magnetName)
		{
			SouthStageCollList_.emplace_back(magnet.path_ = MV1LoadModel(fName));
		}
	}
}

void Stage::LoadSWall_TMagnet(wstring magnetName, const TCHAR* fName)
{
	std::wstring objName;
	for (auto& magnet : magnetList_) {
		objName = ConvertToWstring(magnet.objectName);
		if (objName == magnetName)
		{
			magnet.path_ = MV1LoadModel(fName);
		}
	}
}

void Stage::LoadNMagnet(wstring magnetName, const TCHAR* fName)
{
	std::wstring objName;
	for (auto& magnet : magnetList_) {
		objName = ConvertToWstring(magnet.objectName);
		if (objName == magnetName)
		{
			NorthStageCollList_.emplace_back(magnet.path_ = MV1LoadModel(fName));
		}
	}
}

void Stage::LoadNWall_TMagnet(wstring magnetName, const TCHAR* fName)
{
	std::wstring objName;
	for (auto& magnet : magnetList_) {
		objName = ConvertToWstring(magnet.objectName);
		if (objName == magnetName)
		{
			magnet.path_ = MV1LoadModel(fName);
		}
	}
}

void Stage::LoadNBoxMagnet(wstring magnetName, const TCHAR* fName)
{
	std::wstring objName;
	for (auto& magnet : magnetList_) {
		objName = ConvertToWstring(magnet.objectName);

		if (objName == magnetName)
		{
			NMagBoxCollList_.emplace_back(CollObj{ magnet.path_,magnet.pos_ ,Vector3{0,0,0} });
		}
	}
}

void Stage::LoadSBoxMagnet(wstring magnetName, const TCHAR* fName)
{
	lpModelMng.GetID(fName, 10);
	std::wstring objName;
	for (auto& magnet : magnetList_) {
		objName = ConvertToWstring(magnet.objectName);
		if (objName == magnetName)
		{
			SMagBoxCollList_.emplace_back(CollObj{magnet.path_,magnet.pos_ ,Vector3{0,0,0} });
		}
	}
}

void Stage::DebugDraw(void)
{
	//XYZ�̃O���b�h���`��
	int y = 10;
	int x = 500;

	float LEN_LINE = 500.0f;
	for (int i = 0; i < 11; i++)
	{
		//X
		DrawLine3D(
			{ -LEN_LINE,0.0f,-LEN_LINE + i * 100 },
			{ LEN_LINE ,0.0f,-LEN_LINE + i * 100 },
			0xff0000);
		//Z
		DrawLine3D(
			{ -LEN_LINE + i * 100,0.0f,-LEN_LINE },
			{ -LEN_LINE + i * 100,0.0f,LEN_LINE },
			0x0000ff);
	}
	//Y
	DrawLine3D(
		{ 0.0f, LEN_LINE, 0.0f },
		{ 0.0f,-LEN_LINE,0.0f },
		0x00ff00);
}


void Stage::SetRigidBodies()
{
	btTransform trans;	// ���̃I�u�W�F�N�g�̈ʒu�p�����i�[����ϐ�(�s��)
	trans.setIdentity();// �ʒu�p���s��̏�����
	// �Q�Ɨp���b�V���̍\�z
	MV1SetupReferenceMesh(collisionHandle_, -1, TRUE);
	// �Q�Ɨp���b�V���̎擾
	MV1_REF_POLYGONLIST Mesh = MV1GetReferenceMesh(collisionHandle_, -1, TRUE);
	//�n�ʂ̃��b�V������g���C�A���O�����X�g�쐬
	btTriangleMesh* VartexGround = new btTriangleMesh();
	Vector3 pos[3];
	// �|���S���̐������J��Ԃ�
	for (int i = 0; i < Mesh.PolygonNum; i++)
	{
		pos[0].x = Mesh.Vertexs[Mesh.Polygons[i].VIndex[0]].Position.x;
		pos[0].y = Mesh.Vertexs[Mesh.Polygons[i].VIndex[0]].Position.y;
		pos[0].z = Mesh.Vertexs[Mesh.Polygons[i].VIndex[0]].Position.z;
		pos[1].x = Mesh.Vertexs[Mesh.Polygons[i].VIndex[1]].Position.x;
		pos[1].y = Mesh.Vertexs[Mesh.Polygons[i].VIndex[1]].Position.y;
		pos[1].z = Mesh.Vertexs[Mesh.Polygons[i].VIndex[1]].Position.z;
		pos[2].x = Mesh.Vertexs[Mesh.Polygons[i].VIndex[2]].Position.x;
		pos[2].y = Mesh.Vertexs[Mesh.Polygons[i].VIndex[2]].Position.y;
		pos[2].z = Mesh.Vertexs[Mesh.Polygons[i].VIndex[2]].Position.z;
		btVector3 pos0(pos[0].x, pos[0].y, pos[0].z);
		btVector3 pos1(pos[1].x, pos[1].y, pos[1].z);
		btVector3 pos2(pos[2].x, pos[2].y, pos[2].z);
		VartexGround->addTriangle(pos0, pos1, pos2, false);
	}
	// �n�ʂ̏Փˌ`��̍쐬
	btBvhTriangleMeshShape* ground_shape = new btBvhTriangleMeshShape(VartexGround, true, true);
	btstageBody_ = CreateRigidBody(0.0, trans, ground_shape, 0);

	g_collisionshapes.push_back(ground_shape); // �Ō�ɔj��(delete)���邽�߂Ɍ`��f�[�^���i�[���Ă���
}

btRigidBody* Stage::BulletCreateMeshShape(string name,int num,Vector3 pos, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body)
{
	btTransform trans;	// ���̃I�u�W�F�N�g�̈ʒu�p�����i�[����ϐ�(�s��)
	trans.setIdentity();// �ʒu�p���s��̏�����
	// �Q�Ɨp���b�V���̍\�z
	MV1SetupReferenceMesh(lpModelMng.GetID(name)[num], -1, TRUE);
	// �Q�Ɨp���b�V���̎擾
	MV1_REF_POLYGONLIST RefMesh = MV1GetReferenceMesh(lpModelMng.GetID(name)[num], -1, TRUE);
	//�n�ʂ̃��b�V������g���C�A���O�����X�g�쐬
	btTriangleMesh* VartexGround = new btTriangleMesh();
	// �|���S���̐������J��Ԃ�
	for (int i = 0; i < RefMesh.PolygonNum; i++)
	{
		float a0 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position.x;
		float a1 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position.y;
		float a2 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[0]].Position.z;
		float b0 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position.x;
		float b1 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position.y;
		float b2 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[1]].Position.z;
		float c0 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position.x;
		float c1 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position.y;
		float c2 = RefMesh.Vertexs[RefMesh.Polygons[i].VIndex[2]].Position.z;
		btVector3 A(a0, a1, a2);
		btVector3 B(b0, b1, b2);
		btVector3 C(c0, c1, c2);
		VartexGround->addTriangle(A, B, C, false);
	}
	// �n�ʂ̏Փˌ`��̍쐬
	btGImpactMeshShape* ground_shape = new btGImpactMeshShape(VartexGround);
	ground_shape->updateBound();
	//trans.setIdentity();// �ʒu�p���s��̏�����
	//btQuaternion qrot(0, 0, 0, 1);
	//trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	//trans.setRotation(qrot);	// �l�������s��ɕϊ����Ďp���s��Ɋ|�����킹��
	//ground_shape->updateBound();
	body = CreateRigidBody(0, LinearVec, AngularVec, trans, ground_shape, 0);
	body->setFriction(Friction);
	g_collisionshapes.push_back(ground_shape); // �Ō�ɔj��(delete)���邽�߂Ɍ`��f�[�^���i�[���Ă���
	return body;
}
btRigidBody* Stage::BulletCreateCapsule(Vector3 pos, btScalar radius, btScalar height, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body)
{
	btTransform trans;	// ���̃I�u�W�F�N�g�̈ʒu�p�����i�[����ϐ�(�s��)
	trans.setIdentity();// �ʒu�p���s��̏�����

	// ----- �����̃I�u�W�F�N�g�ǉ� -----
	// �`��ݒ�
	//btCollisionShape* box_shape = new btBoxShape(btVector3(20, 10, 20));
	btCollisionShape* Capsule_shape = new btCapsuleShape(radius, height);
	// �����ʒu�E�p��
	btQuaternion qrot(0, 0, 0, 1);
	trans.setIdentity();// �ʒu�p���s��̏�����
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(qrot);	// �l�������s��ɕϊ����Ďp���s��Ɋ|�����킹��

	// ���̃I�u�W�F�N�g����
	body = CreateRigidBody(1, LinearVec, AngularVec, trans, Capsule_shape, 0);
	body->setFriction(Friction);
	//body->setRestitution(1);

	/*body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);*/
	g_collisionshapes.push_back(Capsule_shape); // �Ō�ɔj��(delete)���邽�߂Ɍ`��f�[�^���i�[���Ă���
	// ----- �����܂� (�����̃I�u�W�F�N�g�ǉ�) -----
	// 
	// ���蔲���h�~�pSwept sphere�̐ݒ�(CCD:Continuous Collision Detection)
	body->setCcdMotionThreshold(0.1);	//���̑��x�𒴂����炷�蔲���h�~������//�[������off
	body->setCcdSweptSphereRadius(5);	//���a���w�肷��
	return body;
}

btRigidBody* Stage::BulletCreateBox(int mass, Vector3 pos, btVector3 size, float Threshold, float collsize, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body)
{
	btTransform trans;	// ���̃I�u�W�F�N�g�̈ʒu�p�����i�[����ϐ�(�s��)
	trans.setIdentity();// �ʒu�p���s��̏�����

	// ----- �����̃I�u�W�F�N�g�ǉ� -----
	// �`��ݒ�
	//btCollisionShape* box_shape = new btBoxShape(btVector3(20, 10, 20));
	btCollisionShape* box_shape = new btBoxShape(btVector3(size.x(), size.y(), size.z()));

	//btCollisionShape* box_shape = new btCapsuleShape(radius, height);
	// �����ʒu�E�p��
	btQuaternion qrot(0, 0, 0, 1);
	trans.setIdentity();// �ʒu�p���s��̏�����
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(qrot);	// �l�������s��ɕϊ����Ďp���s��Ɋ|�����킹��

	// ���̃I�u�W�F�N�g����
	body = CreateRigidBody(mass, LinearVec, AngularVec, trans, box_shape, 0);
	body->setFriction(Friction);
	//body->setRestitution(1);

	/*body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);*/
	g_collisionshapes.push_back(box_shape); // �Ō�ɔj��(delete)���邽�߂Ɍ`��f�[�^���i�[���Ă���
	// ----- �����܂� (�����̃I�u�W�F�N�g�ǉ�) -----
	// 
	// ���蔲���h�~�pSwept sphere�̐ݒ�(CCD:Continuous Collision Detection)
	body->setCcdMotionThreshold(Threshold);	//���̑��x�𒴂����炷�蔲���h�~������//�[������off
	body->setCcdSweptSphereRadius(collsize +0.1);	//���a���w�肷��
	return body;
}

btRigidBody* Stage::BulletCreateBox(int mass, Vector3 pos, btQuaternion qrot, btVector3 size, float Threshold, float collsize, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body)
{
	btTransform trans;	// ���̃I�u�W�F�N�g�̈ʒu�p�����i�[����ϐ�(�s��)
	trans.setIdentity();// �ʒu�p���s��̏�����

	// ----- �����̃I�u�W�F�N�g�ǉ� -----
	// �`��ݒ�
	//btCollisionShape* box_shape = new btBoxShape(btVector3(20, 10, 20));
	btCollisionShape* box_shape = new btBoxShape(btVector3(size.x(), size.y(), size.z()));

	//btCollisionShape* box_shape = new btCapsuleShape(radius, height);
	// �����ʒu�E�p��
	//btQuaternion qrot(0, 0, 0, 1);
	trans.setIdentity();// �ʒu�p���s��̏�����
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(qrot);	// �l�������s��ɕϊ����Ďp���s��Ɋ|�����킹��

	// ���̃I�u�W�F�N�g����
	body = CreateRigidBody(mass, LinearVec, AngularVec, trans, box_shape, 0);
	body->setFriction(Friction);
	//body->setRestitution(1);

	/*body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);*/
	g_collisionshapes.push_back(box_shape); // �Ō�ɔj��(delete)���邽�߂Ɍ`��f�[�^���i�[���Ă���
	// ----- �����܂� (�����̃I�u�W�F�N�g�ǉ�) -----
	// 
	// ���蔲���h�~�pSwept sphere�̐ݒ�(CCD:Continuous Collision Detection)
	body->setCcdMotionThreshold(Threshold);	//���̑��x�𒴂����炷�蔲���h�~������//�[������off
	body->setCcdSweptSphereRadius(collsize + 0.1);	//���a���w�肷��
	return body;
}

btRigidBody* Stage::BulletCreateSphere(Vector3 pos, btScalar radius, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body)
{
	btTransform trans;	// ���̃I�u�W�F�N�g�̈ʒu�p�����i�[����ϐ�(�s��)
	trans.setIdentity();// �ʒu�p���s��̏�����

	// ----- �����̃I�u�W�F�N�g�ǉ� -----
	// �`��ݒ�
	//btCollisionShape* box_shape = new btBoxShape(btVector3(20, 10, 20));
	btCollisionShape* Sphere_shape = new btSphereShape(radius);

	//btCollisionShape* box_shape = new btCapsuleShape(radius, height);
	// �����ʒu�E�p��
	btQuaternion qrot(0, 0, 0, 1);
	trans.setIdentity();// �ʒu�p���s��̏�����
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(qrot);	// �l�������s��ɕϊ����Ďp���s��Ɋ|�����킹��

	// ���̃I�u�W�F�N�g����
	body = CreateRigidBody(1, LinearVec, AngularVec, trans, Sphere_shape, 0);
	body->setFriction(Friction);
	/*body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);*/
	g_collisionshapes.push_back(Sphere_shape); // �Ō�ɔj��(delete)���邽�߂Ɍ`��f�[�^���i�[���Ă���
	// ----- �����܂� (�����̃I�u�W�F�N�g�ǉ�) -----
	// 
	// ���蔲���h�~�pSwept sphere�̐ݒ�(CCD:Continuous Collision Detection)
	body->setCcdMotionThreshold(0.1);	//���̑��x�𒴂����炷�蔲���h�~������//�[������off
	body->setCcdSweptSphereRadius(radius);	//���a���w�肷��
	return body;
}

btRigidBody* Stage::CreateRigidBody(double mass, const btTransform& init_trans, btCollisionShape* shape, int index)
{
	// ���ʂ�0�Ȃ�ΐÓI��(static)�I�u�W�F�N�g�Ƃ��Đݒ�C
	bool isDynamic = (mass != 0.0);

	btVector3 inertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* motion_state = new btDefaultMotionState(init_trans);

	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, shape, inertia);

	btRigidBody* body = new btRigidBody(rb_info);

	body->setUserIndex(index);
	if (mass <= 1e-10) {
		// Kinematic�I�u�W�F�N�g�Ƃ��Đݒ�(stepSimulation���Ă��^���̌v�Z���s��Ȃ��悤�ɂ���)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		// ��ɃX���[�v��Ԃɂ���
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	if (g_dynamicsworld) {
		g_dynamicsworld->addRigidBody(body);
	}

	return body;
}

btRigidBody* Stage::CreateRigidBody(double mass, btVector3 LinearVec, btVector3 AngularVec, const btTransform& init_trans, btCollisionShape* shape, int index)
{
	// ���ʂ�0�Ȃ�ΐÓI��(static)�I�u�W�F�N�g�Ƃ��Đݒ�C
	bool isDynamic = (mass != 0.0);

	btVector3 inertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* motion_state = new btDefaultMotionState(init_trans);

	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, shape, inertia);

	btRigidBody* body = new btRigidBody(rb_info);

	body->setUserIndex(index);
	//�ړ��ł�������̌Œ�0�̍��W�ɂ͈ړ��ł��Ȃ��Ȃ�		(1,1,0)�������炚�����Ɉړ����邱�Ƃ͂ł��Ȃ�
	 
	//�ړ��\�ȕ����̐ݒ�
	body->setLinearFactor(LinearVec);
	//��]�\�ȕ����̐ݒ�
	body->setAngularFactor(AngularVec);

	if (mass <= 1e-10) {
		// Kinematic�I�u�W�F�N�g�Ƃ��Đݒ�(stepSimulation���Ă��^���̌v�Z���s��Ȃ��悤�ɂ���)
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		// ��ɃX���[�v��Ԃɂ���
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	if (g_dynamicsworld) {
		g_dynamicsworld->addRigidBody(body);
	}

	return body;
}

void Stage::CleanBullet(void)
{
	// ���̃I�u�W�F�N�g�̔j��
	for (int i = g_dynamicsworld->getNumCollisionObjects() - 1; i >= 0; --i) {
		btCollisionObject* obj = g_dynamicsworld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		g_dynamicsworld->removeCollisionObject(obj);
		delete obj;
	}

	// �`��̔j��
	for (int j = 0; j < (int)g_collisionshapes.size(); ++j) {
		btCollisionShape* shape = g_collisionshapes[j];
		g_collisionshapes[j] = 0;
		delete shape;
	}
	g_collisionshapes.clear();
}

void Stage::CleanBulletObj(btRigidBody* body)
{
	// ���̃I�u�W�F�N�g�̔j��
	for (int i =g_dynamicsworld->getNumCollisionObjects() - 1; i >= 0; --i) {
		btCollisionObject* obj = g_dynamicsworld->getCollisionObjectArray()[i];
		if (body == btRigidBody::upcast(obj))
		{
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState()) {
				delete body->getMotionState();
			}
			g_dynamicsworld->removeCollisionObject(obj);
			delete obj;
		}
	}
}


std::vector<std::vector<int>>& Stage::GetALLStageCollList()
{
	return AllStageCollList_;
}

std::vector<Object>& Stage::GetMagnetList(void)
{
	return magnetList_;
}

std::vector<Object>& Stage::GetCoinList()
{
	return coinList_;
}

std::vector<Object>& Stage::GetEnemyList()
{
	return enemyList_;
}

std::vector<Object>& Stage::GetShootEnemyList()
{
	return shootEnemyList_;
}

std::vector<Object>& Stage::GetAccessoryList()
{
	return accessoryList_;
}

std::vector<Object>& Stage::GetSmokeList(void)
{
	return smokeList_;
}

std::vector<Object>& Stage::GetGearList(void)
{
	return gearList_;
}

std::vector<PairObject>& Stage::GetDoorList(void)
{
	return doorList_;
}

std::vector<PairObject>& Stage::GetFloorList(void)
{
	return floorList_;
}

std::vector<Object>& Stage::GetMidPointList(void)
{
	return midPointList_;
}

std::vector<Object>& Stage::GetWarningSignList(void)
{
	return warningSignList_;
}

std::vector<Object>& Stage::GetScrapList(void)
{
	return scrapList_;
}

STAGE_NAME Stage::GetStage(void)
{
	return stageName_;
}

std::vector<int>& Stage::GetNorthStageCollList()
{
	return NorthStageCollList_;
}

std::vector<int>& Stage::GetSouthStageCollList()
{
	return SouthStageCollList_;
}

std::vector<int>& Stage::GetStageCollList()
{
	return StageCollList_;
}

std::vector<CollObj>& Stage::GetNMagBoxCollList()
{
	return NMagBoxCollList_;
}

std::vector<CollObj>& Stage::GetSMagBoxCollList()
{
	return SMagBoxCollList_;
}

std::vector<loadgim>& Stage::GetGoalCollList()
{
	return GoalCollList_;
}

wstring Stage::ConvertToWstring(string& str)
{
	auto cnt = MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		str.length(),
		nullptr,
		0
	);

	wstring wstr;
	wstr.resize(cnt);

	MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		str.length(),
		&wstr[0],
		wstr.length()
	);

	return wstr;
}

void Stage::SetStagePos(Vector3 pos)
{
	MV1SetPosition(backGroundHandle_, pos.toVECTOR());
	MV1SetPosition(collisionHandle_, pos.toVECTOR());
}

void Stage::ClearList(void)
{
	//���f���폜
	for (auto& goal : GoalCollList_)
	{
		MV1DeleteModel(goal.first);
	}
	for (auto& magnet : magnetList_)
	{
		MV1DeleteModel(magnet.path_);
	}
	MV1DeleteModel(backGroundHandle_);
	MV1DeleteModel(collisionHandle_);

	//�x�N�^�[���N���A
	magnetList_.clear();
	enemyList_.clear();
	coinList_.clear();
	shootEnemyList_.clear();
	accessoryList_.clear();
	smokeList_.clear();
	gearList_.clear();
	midPointList_.clear();
	StageCollList_.clear();
	SouthStageCollList_.clear();
	NorthStageCollList_.clear();
	GoalCollList_.clear();
	AllStageCollList_.clear();
	SMagBoxCollList_.clear();
	NMagBoxCollList_.clear();
	doorList_.clear();
	floorList_.clear();
	scrapList_.clear();
	warningSignList_.clear();

	//�N���A�[������Ƀ��������l�߂�
	magnetList_.shrink_to_fit();
	enemyList_.shrink_to_fit();
	coinList_.shrink_to_fit();
	shootEnemyList_.shrink_to_fit();
	accessoryList_.shrink_to_fit();
	smokeList_.shrink_to_fit();
	gearList_.shrink_to_fit();
	midPointList_.shrink_to_fit();
	StageCollList_.shrink_to_fit();
	SouthStageCollList_.shrink_to_fit();
	NorthStageCollList_.shrink_to_fit();
	GoalCollList_.shrink_to_fit();
	AllStageCollList_.shrink_to_fit();
	SMagBoxCollList_.shrink_to_fit();
	NMagBoxCollList_.shrink_to_fit();
	doorList_.shrink_to_fit();
	floorList_.shrink_to_fit();

	scrapList_.shrink_to_fit();
	warningSignList_.shrink_to_fit();
}
