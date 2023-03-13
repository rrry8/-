#include "PressureSidewaysDoor.h"
//コンストラクタ
PressureSidewaysDoor::PressureSidewaysDoor(Vector3 pos,int num)
{
	num_ = num;
	Init(pos);
}

//デストラクタ
PressureSidewaysDoor::~PressureSidewaysDoor()
{
}


bool PressureSidewaysDoor::Init(void)
{
	return false;
}

bool PressureSidewaysDoor::Init(Vector3& pos)
{
	pos_ = pos;
	//移動量
	movePow_ = 5;
	size_ = { 190,10,180};
	flg_ = false;
	moveLimitVal_ = { pos_.x + DOOR_SIZE_X , pos_.x};
	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/Floor_Tile.mv1")[num_], pos_.toVECTOR()/*{ pos_.x, pos_.y, pos_.z }*/);
	//lpStage.SetRigidBodies(modelH_);
	Doorbody_ = lpStage.BulletCreateBox(0,pos_,size_.tobtVec(),0 ,size_.y, btScalar(0.3), btVector3(1, 1, 0), btVector3(0, 0, 0), Doorbody_);
	return true;
}

//更新
void PressureSidewaysDoor::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
}

void PressureSidewaysDoor::Update(bool& flg)
{
	flg_ = flg;
	
	btTransform trans;
	Vector3 vec;
	if (CheckHitKey(KEY_INPUT_6))
	{
		flg_ = true;
	}
	Doorbody_->getMotionState()->getWorldTransform(trans);

	//pos_ = Vector3(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));

	//flg_ = true;
	pos_ = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
	//スイッチが踏まれたら
	if (flg_)
	{
		//上限まで扉を上昇
		/*auto vec = moveLimitVal_.upper  - pos_.y;
		Doorbody_->setLinearVelocity({ 0,vec,0});*/

		if (pos_.x <= moveLimitVal_.upper)
		{
			pos_.x += movePow_;
		}
		//pos_ = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());

	}
	else	//スイッチが踏まれていなかったら
	{
		////下限まで下降
		if (pos_.x>= moveLimitVal_.lower)
		{
			pos_.x -= movePow_;
		}
	}
	 
	trans.setOrigin(pos_.tobtVec());
	Doorbody_->getMotionState()->setWorldTransform(trans);
	Doorbody_->getMotionState()->getWorldTransform(trans);

	pos_ = Vector3(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));
	//座標更新
	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/Floor_Tile.mv1")[num_], pos_.toVECTOR()/*{ pos_.x, pos_.y, pos_.z }*/);
}


//描画
void PressureSidewaysDoor::Draw(void)
{
	//for (int i = 0; i < DOOR_MAX; i++)
	//{
	//	MV1DrawModel(duplicateModelH_[i]);
	//}
	MV1DrawModel(lpModelMng.GetID("./source/Model/Stage/Floor_Tile.mv1")[num_]);
}

btRigidBody* PressureSidewaysDoor::GetRigidBody(void)
{
	return nullptr;
}

