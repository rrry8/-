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

//3Dオブジェクト情報
struct Object {
	int path_;	//ファイルパス
	string objectName;	//オブジェクト名
	Vector3 pos_;	//座標
	Vector3 rotate_;	//回転
	Object(int path, string name, const Vector3& pos, const Vector3& rotate) :
		path_(path),
		objectName(name),
		pos_(pos),
		rotate_(rotate) {};
};

//3Dオブジェクト情報(1対1)
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

enum class STAGE_NAME	//ステージの名前
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
	//コンストラクタ
	Stage(void);

	//デストラクタ
	~Stage(void);
	void Remove(void);
	//初期化
	bool Init(int num);
	void InitBullet(int Gravity);
	//更新
	void Update(void);

	//描画
	void Draw(void);

	//非同期読み込み
	bool ReInit(void);
	//bulletのマップの作成
	void SetRigidBodies(void);

	btRigidBody* BulletCreateMeshShape(string name, int num, Vector3 pos, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bulletのカプセルの作成
	btRigidBody* BulletCreateCapsule(Vector3 pos, btScalar radius, btScalar height, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bulletのボックスの作成
	btRigidBody* BulletCreateBox(int mass, Vector3 pos, btVector3 size, float Threshold, float collsize, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	btRigidBody* BulletCreateBox(int mass, Vector3 pos, btQuaternion qrot, btVector3 size, float Threshold, float collsize, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bulletの球の作成
	btRigidBody* BulletCreateSphere(Vector3 pos, btScalar radius, btScalar Friction, btVector3 LinearVec, btVector3 AngularVec, btRigidBody* body);
	//bulletのrigidbodyの作成
	btRigidBody* CreateRigidBody(double mass, const btTransform& init_trans, btCollisionShape* shape, int index);
	btRigidBody* CreateRigidBody(double mass, btVector3 LinearVec, btVector3 AngularVec, const btTransform& init_trans, btCollisionShape* shape, int index);
	//bulletのbodyを空にする
	void CleanBullet(void);
	//bulletの引数だけなくす
	void CleanBulletObj(btRigidBody* body);

	//当たり判定を返す
	//S極
	std::vector<int>& GetSouthStageCollList(void);

	//N極
	std::vector<int>& GetNorthStageCollList(void);

	//ステージ
	std::vector<int>& GetStageCollList(void);

	//Box
	std::vector<CollObj>& GetNMagBoxCollList(void);
	std::vector<CollObj>& GetSMagBoxCollList(void);

	//ゴール判定用
	std::vector<loadgim>& GetGoalCollList(void);

	//すべての当たり判定
	std::vector<std::vector<int>>& GetALLStageCollList(void);

	//リストを返す
	std::vector<Object>& GetMagnetList(void);

	//コイン
	std::vector<Object>& GetCoinList(void);

	//敵
	std::vector<Object>& GetEnemyList(void);

	//敵：射撃
	std::vector<Object>& GetShootEnemyList(void);

	//小物
	std::vector<Object>& GetAccessoryList(void);

	//煙エフェクト
	std::vector<Object>& GetSmokeList(void);

	//歯車
	std::vector<Object>& GetGearList(void);

	//ボタンと扉
	std::vector<PairObject>& GetDoorList(void);

	//ボタンと床
	std::vector<PairObject>& GetFloorList(void);

	//中間
	std::vector<Object>& GetMidPointList(void);

	//警告看板
	std::vector<Object>& GetWarningSignList(void);


	//スクラップ
	std::vector<Object>& GetScrapList(void);

	//選択されたステージ状態を返す
	STAGE_NAME GetStage(void);

	// Bullet
	btDynamicsWorld* g_dynamicsworld;	//!< Bulletワールド

	//Wstringに変換
	wstring ConvertToWstring(string& str);

private:
	//SceneManager* mSceneManager;
	//情報リスト
	//磁石
	std::vector<Object> magnetList_;

	//コイン
	std::vector<Object> coinList_;

	//敵
	std::vector<Object> enemyList_;

	//敵：射撃
	std::vector<Object> shootEnemyList_;

	//小物
	std::vector<Object> accessoryList_;

	//煙エフェクト
	std::vector<Object> smokeList_;

	//歯車
	std::vector<Object> gearList_;

	//中間
	std::vector<Object> midPointList_;

	//警告看板
	std::vector<Object> warningSignList_;

	//スクラップ
	std::vector<Object> scrapList_;

	//ボタンと扉
	std::vector<PairObject> doorList_;

	//ボタンと床
	std::vector<PairObject> floorList_;

	//当たり判定モデル
	int collisionHandle_;
	//背景モデル
	int backGroundHandle_;

	//当たり判定用リスト
	//磁石
	std::vector<int> SouthStageCollList_;	//S極

	//N
	std::vector<int> NorthStageCollList_;	//N極

	//ステージ
	std::vector<int> StageCollList_;

	//磁石の箱
	std::vector<CollObj> NMagBoxCollList_;
	std::vector<CollObj> SMagBoxCollList_;

	//すべての当たり判定リスト
	std::vector<std::vector<int>> AllStageCollList_;

	//ゴール地点
	std::vector<loadgim> GoalCollList_;

	//ステージの種類
	STAGE_NAME stageName_;

	// Bullet
	btRigidBody* btstageBody_;
	btAlignedObjectArray<btCollisionShape*> g_collisionshapes;
	int GravityPow_;

	void SetStagePos(Vector3 pos);

	//Object削除
	void ClearList(void);
	//磁石モデル読み込み
	void LoadMagnet(void);
	//データ読み込み
	void LoadObjectData(std::vector<Object>& objList, const TCHAR* fName);
	//2つのデータ読み込み
	void LoadPairObjectData(std::vector<PairObject>& objList, const TCHAR* fName, const TCHAR* fName2);

	//S極読み込み
	void LoadSMagnet(wstring magnetName, const TCHAR* fName);
	//半透明S極壁
	void LoadSWall_TMagnet(wstring magnetName, const TCHAR* fName);

	//S極読み込み
	void LoadNMagnet(wstring magnetName, const TCHAR* fName);
	//半透明N極壁
	void LoadNWall_TMagnet(wstring magnetName, const TCHAR* fName);

	//箱型磁石読み込み
	void LoadNBoxMagnet(wstring magnetName, const TCHAR* fName);
	void LoadSBoxMagnet(wstring magnetName, const TCHAR* fName);

	//デバッグ表示
	void DebugDraw(void);
};
