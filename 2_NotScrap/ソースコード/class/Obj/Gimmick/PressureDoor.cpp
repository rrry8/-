#include "PressureDoor.h"
//�R���X�g���N�^
PressureDoor::PressureDoor(Vector3& pos,int num)
{
	num_ = num;
	Init(pos);
}

//�f�X�g���N�^
PressureDoor::~PressureDoor()
{
}


bool PressureDoor::Init(void)
{
	return false;
}

bool PressureDoor::Init(Vector3& pos)
{
	pos_ = pos;
	//�ړ���
	movePow_ = 5;
	size_ = {10,190,180};
	flg_ = false;
	moveLimitVal_ = { pos_.y + DOOR_SIZE_Y, pos_.y };
	MV1SetPosition(lpModelMng.GetID("./source/Model/Gimmick/Door.mv1")[num_], pos_.toVECTOR()/*{ pos_.x, pos_.y, pos_.z }*/);
	//lpStage.SetRigidBodies(modelH_);
	Doorbody_ = lpStage.BulletCreateBox(1,pos_,size_.tobtVec(),0 ,size_.y, btScalar(0.3), btVector3(0, 1, 0), btVector3(0, 0, 0), Doorbody_);
	return true;
}

//�X�V
void PressureDoor::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
}

void PressureDoor::Update(bool& flg)
{
	flg_ = flg;
	btTransform trans;
	Doorbody_->getMotionState()->getWorldTransform(trans);

	pos_ = Vector3(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y()), static_cast<float>(trans.getOrigin().z()));

	//flg_ = true;
	//�X�C�b�`�����܂ꂽ��
	if (flg_)
	{
		//����܂Ŕ����㏸
		auto vec = moveLimitVal_.upper  - pos_.y;
		Doorbody_->setLinearVelocity({ 0,vec,0});

		//if (pos.y <= moveLimitVal_.upper)
		//{
		//	/*pos.y += movePow_;*/
		//}
		//pos_ = Vector3(trans.getOrigin().x(), trans.getOrigin().y() - 80, trans.getOrigin().z());

	}
	//else	//�X�C�b�`�����܂�Ă��Ȃ�������
	//{
	//	////�����܂ŉ��~
	//	if (pos.y >= moveLimitVal_.lower)
	//	{
	//		pos.y -= movePow_;
	//	}
	//}
	//���W�X�V
	MV1SetPosition(lpModelMng.GetID("./source/Model/Gimmick/Door.mv1")[num_], pos_.toVECTOR()/*{ pos_.x, pos_.y, pos_.z }*/);
}


//�`��
void PressureDoor::Draw(void)
{
	//for (int i = 0; i < DOOR_MAX; i++)
	//{
	//	MV1DrawModel(duplicateModelH_[i]);
	//}
	MV1DrawModel(lpModelMng.GetID("./source/Model/Gimmick/Door.mv1")[num_]);
}

btRigidBody* PressureDoor::GetRigidBody(void)
{
	return nullptr;
}

//�v���C���[���{�^���𓥂�ł��邩
void PressureDoor::SetStepOnFlg(bool flg)
{
	flg_ = flg;
}
