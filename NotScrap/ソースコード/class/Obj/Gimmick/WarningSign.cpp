#include "WarningSign.h"

WarningSign::WarningSign(Vector3& pos, int num)
{
	num_ = num;
	Init(pos);
}

WarningSign::~WarningSign(void)
{
}

bool WarningSign::Init(void)
{
	return true;
}

bool WarningSign::Init(Vector3& pos)
{
	pos_ = pos;
	//"./source/Model/Gimmick/Sign.mv1"
	MV1SetPosition(lpModelMng.GetID("./source/Model/Gimmick/sign.mv1")[num_], { pos_.x,pos.y,pos.z });
	//MV1SetRotationXYZ(lpModelMng.GetID("./source/Model/Gimmick/Sign.mv1")[num_],
	//	{ 0.0,AsoUtility::Deg2RadF(180.0f),0.0 });
	return true;
}

void WarningSign::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
}

void WarningSign::Update(bool& flg)
{
}

void WarningSign::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID("./source/Model/Gimmick/sign.mv1")[num_]);
}

btRigidBody* WarningSign::GetRigidBody(void)
{
	return nullptr;
}
