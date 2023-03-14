#include "PressureDoor.h"
#include "PressureSidewaysDoor.h"
#include "PressurePlate.h"
#include "../Stage.h"
//�R���X�g���N�^
PressurePlate::PressurePlate(Vector3 pPos, Vector3 dPos, DoorType type,int num)
{
	num_ = num;
	Init(pPos,dPos,type);
}

//�f�X�g���N�^
PressurePlate::~PressurePlate()
{
}

//������
bool PressurePlate::Init()
{
	////���f���ǂݍ���
	//modelH_ = MV1LoadModel("source/Model/Gimmick/Button.mv1");
	////lpModelMng.GetID("./source/model/Coin/Coin.mv1", 2);

	////modelH_ = lpModelMng.GetID("source/Model/Gimmick/Button.mv1")[0];
	////pos_ = { 600, 100,0 };
	////���f������
	//for (int i = 0; i < PLATE_NUM; i++)
	//{
	//	duplicateModelH_[i] = MV1DuplicateModel(modelH_);
	//}
	////���f���z�u
	////MV1SetPosition(duplicateModelH_[0], VGet(200.0, 100.0, 0.0));
	//MV1SetPosition(duplicateModelH_[0], VGet(1800.0, 1900.0, 0.0));
	//MV1SetPosition(duplicateModelH_[1], VGet(900.0, 1900.0, 0.0));
	return true;
}

bool PressurePlate::Init(Vector3 pPos, Vector3 dPos,DoorType type)
{
	pos_ = pPos;
	doorPos_ = dPos;
	type_ = type;
	//int i = 0;
	//for (auto& door : lpStage.GetDoorList())
	//{
	//	if (i == 0)
	//	{
	//		i++;
	//		continue;
	//	}
	//	door_ = std::make_unique<PressureDoor>(door.pos_,num_);
	//	i++;
	//}
	if (type == DoorType::SidewaysDoor)
	{
		Sdoor_ = std::make_unique<PressureSidewaysDoor>(doorPos_, num_);

	}
	else
	{
		door_ = std::make_unique<PressureDoor>(doorPos_, num_);
	}

	//onModelH_= lpModelMng.GetID("./source/Model/Gimmick/Button.mv1")[num_];
	MV1SetPosition(lpModelMng.GetID("./source/Model/Gimmick/Button.mv1")[num_], pos_.toVECTOR());
	MV1SetPosition(lpModelMng.GetID("./source/Model/Gimmick/OnButton.mv1")[num_], pos_.toVECTOR());
	MV1SetScale(lpModelMng.GetID("./source/Model/Gimmick/Button.mv1")[num_], VECTOR{ 1.8f,1.8f,1.8f });
	MV1SetScale(lpModelMng.GetID("./source/Model/Gimmick/OnButton.mv1")[num_], VECTOR{ 1.8f,1.8f,1.8f });
	MV1RefreshCollInfo(lpModelMng.GetID("./source/Model/Gimmick/Button.mv1")[num_]);
	return true;
}

//�X�V
void PressurePlate::Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)
{
	
	//door_->SetStepOnFlg(flg_);
	//door_->Update();
	
	flg_ = HitMagBox(flg_,gimmickList);
	//for (auto& door : lpStage.GetDoorList())
	//{
	//}
	if (type_ == DoorType::SidewaysDoor)
	{
		Sdoor_->Update(flg_);

	}
	else
	{
		door_->Update(flg_);
	}
	
	//MV1SetPosition(lpModelMng.GetID("source/Model/Gimmick/Door.mv1")[num_], doorPos_.toVECTOR()/*{ pos_.x, pos_.y, pos_.z }*/);
}


//�`��
void PressurePlate::Draw(void)
{
	//DrawSphere3D(pos_.toVECTOR(), 50, 32, 0xffffff, 0xffff00, true);
	if (flg_)
	{
		MV1DrawModel(lpModelMng.GetID("./source/Model/Gimmick/OnButton.mv1")[num_]);

	}	
	else 
	{
		MV1DrawModel(lpModelMng.GetID("./source/Model/Gimmick/Button.mv1")[num_]);


	}
	//for (auto& door : lpStage.GetDoorList())
	//{
	//}
	if (type_ == DoorType::SidewaysDoor)
	{
		Sdoor_->Draw();
	}
	else
	{
		door_->Draw();
	}
}

btRigidBody* PressurePlate::GetRigidBody(void)
{
	return nullptr;
}

bool PressurePlate::HitMagBox(bool flag,GimmickList& gimmickList)
{
	bool hitFlag = false;
	for (auto& box : gimmickList)
	{
		if (box->GetObjType() == objType::NMagBox || box->GetObjType() == objType::SMagBox)
		{
			auto hits = MV1CollCheck_Sphere(
				lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[box->GetModelH()],
				-1,
				pos_.toVECTOR(), 50);

			if (hits.HitNum > 0)
			{
				//TRACE("�G�ɓ�������\n");
				hitFlag = true;
			}
			//�Փ˔���p�̓��I�\���̂̃������[���
			MV1CollResultPolyDimTerminate(hits);
		}
	}
	return hitFlag;
}

