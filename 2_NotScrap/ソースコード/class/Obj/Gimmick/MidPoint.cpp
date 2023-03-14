#include "MidPoint.h"

MidPoint::MidPoint(Vector3& pos, int num)
{
	num_ = num;
	Init(pos);
}

MidPoint::~MidPoint(void)
{
}

bool MidPoint::Init(void)
{
	return false;
}

bool MidPoint::Init(Vector3& pos)
{
	pos_ = pos;
	MV1SetPosition(lpModelMng.GetID("./source/Model/Stage/MidPoint.mv1")[num_],pos_.toVECTOR());
	return true;
}

void MidPoint::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
}

void MidPoint::Update(bool& flg)
{
}

void MidPoint::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID("./source/Model/Stage/MidPoint.mv1")[num_]);
}

btRigidBody* MidPoint::GetRigidBody(void)
{
	return nullptr;
}
