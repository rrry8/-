#include <DxLib.h>
#include "MagBox.h"
#include "../Stage.h"
#include"../../../_debug/_DebugConOut.h"

MagBox::MagBox(Vector3 pos, objType type , int num)
{
    magboxtype_ = type;
    num_ = num;
    Init(pos, num);
}

MagBox::~MagBox()
{
}

bool MagBox::Init(Vector3 pos , int num)
{

    defPos_ = pos;
    pos_ = defPos_;
    size_ = { 85.0f,85.0f,85.0f };
    if (magboxtype_ == objType::NMagBox)
    {
        MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[num_], pos_.toVECTOR());
    }
    if (magboxtype_ == objType::SMagBox)
    {
        MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[num_], pos_.toVECTOR());
    }
    MagBoxbody_ = lpStage.BulletCreateBox(10000, pos_, size_.tobtVec(),0.1f, size_.y, btScalar(0.3), btVector3(1, 1, 0), btVector3(0, 0, 1), MagBoxbody_);
    isAlive_ = true;
    return true;
}

void MagBox::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
    for (auto& player : PlyaerList)
    {
        if (player->GetState() == State::Death)
        {
            ReInit();
        }

    }
    if (isAlive_ == false)
    {
        ReInit();
    }
    //TRACE("box%f\n", moveVec_.x);
    //SetBoxMoveVec();
    //moveVec_ = vec;
  //  for (auto& box : lpStage.GetNMagBoxCollList())
  //{
  //     // TRACE("1%d\n", box.handle);
  //    if (modelH_ == box.handle)
  //    {
  //        //TRACE("2%d\n", box.handle);

  //        moveVec_ = box.vec;
  //        TRACE("%f\n", box.vec.x);

  //    }
  //}
   // TRACE("pos.x%f pos.y%f\n", pos_.x, pos_.y);
    btQuaternion qrot(0, 0, 0, 1);

    btTransform trans;
    MagBoxbody_->getMotionState()->getWorldTransform(trans);
    pos_ = Vector3(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));
    //if (moveVec_ != Vector3{ 0,0,0 })
    //{
    //    MagBoxbody_->applyCentralImpulse(moveVec_.tobtVec()*10/*btVector3{0,100000,0}*/);
    //   // MagBoxbody_->applyForce(moveVec_.tobtVec()*00, trans.getOrigin());

    //}
   
    MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[num_], pos_.toVECTOR());
    MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[num_], pos_.toVECTOR());

    btMatrix3x3 rot = trans.getBasis();
    btVector3 euler;
    rot.getEulerZYX(euler[2], euler[1], euler[0]);

    MV1SetRotationXYZ(lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[num_], VGet(static_cast<float>(euler.x()), static_cast<float>(euler.y()), static_cast<float>(euler.z())));
    MV1RefreshCollInfo(lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[num_]);
    MV1SetRotationXYZ(lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[num_], VGet(static_cast<float>(euler.x()), static_cast<float>(euler.y()), static_cast<float>(euler.z())));
    MV1RefreshCollInfo(lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[num_]);
}

void MagBox::Draw(void)
{
    if (magboxtype_ == objType::NMagBox)
    {
        MV1DrawModel(lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[num_]);
    }
    if (magboxtype_ == objType::SMagBox)
    {
        MV1DrawModel(lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[num_]);
    }
}

btRigidBody* MagBox::GetRigidBody(void)
{
    return MagBoxbody_;
}



bool MagBox::ReInit(void)
{
    pos_ = defPos_;
    size_ = { 85.0f,85.0f,85.0f };
    if (magboxtype_ == objType::NMagBox)
    {
        MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[num_], pos_.toVECTOR());
    }
    if (magboxtype_ == objType::SMagBox)
    {
        MV1SetPosition(lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[num_], pos_.toVECTOR());
    }
    MagBoxbody_ = lpStage.BulletCreateBox(10000, pos_, size_.tobtVec(), 0.1f, size_.y, btScalar(0.3), btVector3(1, 1, 0), btVector3(0, 0, 1), MagBoxbody_);
    isAlive_ = true;

    return true;
}

