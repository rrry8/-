#include "Scrap.h"

Scrap::Scrap(Vector3 pos, Vector3 rot, int num)
{
    num_ = num;
    Init(pos,rot);
}

Scrap::~Scrap()
{
}

bool Scrap::Init(void)
{
    return false;
}

bool Scrap::Init(Vector3 pos,Vector3 rot)
{
    pos_ = pos;
    rot_ = rot;
    MV1SetPosition(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[num_], pos_.toVECTOR());
    MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[num_]
        , { Rad2DegF(rot_.x),Rad2DegF(rot_.y),Rad2DegF(rot_.z) });
    MV1SetScale(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[num_] ,{ 0.25,0.25,0.25 });

    return true;
}

void Scrap::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
}
void Scrap::Update()
{
    MV1SetPosition(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[num_], pos_.toVECTOR());
}

void Scrap::Draw(void)
{
    MV1DrawModel(lpModelMng.GetID("./source/model/Player/player_robot_new.mv1")[num_]);
}

btRigidBody* Scrap::GetRigidBody(void)
{
    return nullptr;
}
