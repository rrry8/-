#include "Goal.h"

Goal::Goal(int handle, Vector3 pos)
{
	Init(handle,pos);
}

Goal::~Goal()
{
}

bool Goal::Init(int handle, Vector3 pos)
{
	handle_ = handle;
	pos_ = pos;
	MV1SetPosition(handle_, pos_.toVECTOR());
	return false;
}

//bool Goal::Init(std::string name, Vector3 pos)
//{
//	path_ = name;
//	pos_ = pos;
//	MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/Goal.mv1")[0], pos_.toVECTOR());
//	return false;
//}

void Goal::Update(void)
{
	//MV1SetPosition(modelH_, pos_.toVECTOR());
}

void Goal::Draw(void)
{
	MV1DrawModel(handle_);
}
