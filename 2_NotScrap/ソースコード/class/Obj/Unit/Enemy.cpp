#include <DxLib.h>
#include "Enemy.h"
#include "../CheckHit.h"
#include "../../Input/KeyInput.h"
#include "../Unit/Player.h"

constexpr float COLL_SPHERE_SIZE = 50.0f;

Enemy::Enemy(EnemyType type, Vector3 pos, int number)
{
	type_ = type;
	initPos_ = pos;
	number_ = number;
	Init();

}

Enemy::~Enemy()
{
}

bool Enemy::Init()
{
	//b2Vec3 gra(0.0f, -10.0f,0.0f);
	//pos_ = { 11000.0f,250.0f ,0 };//��
	speed_ = 1.0f;
	movePow_ = { 3.0f,8.0f,0.0f };
	moveVec_ = { 1,0,0 };
	pos_ = initPos_;

	state_ = State::Run;
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("Enemymodule");
	FallNode_ = stateDoc.first_node("Fallmodule");
	SpecialHitNode_ = stateDoc.first_node("SpecialHitmodule");
	MV1SetPosition(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_],pos_.toVECTOR());
	MV1SetScale(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_], { 0.35,0.35,0.35 });
	auto radY = Deg2RadF(-90.0f);

	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_], { 0, radY, 0 });
	Enemybody_ = lpStage.BulletCreateBox(1, pos_, btVector3(40, 40, 40),40,0.1, btScalar(1), btVector3(1, 1, 0), btVector3(0, 0, 0), Enemybody_);

	return false;
}

void Enemy::Update()
{
}

void Enemy::Update(std::vector<std::shared_ptr<Player>>& PlyaerList)
{
	btTransform trans;
	Enemybody_->getMotionState()->getWorldTransform(trans);
	pos_ = Vector3(trans.getOrigin().x(), trans.getOrigin().y() - COLL_SPHERE_SIZE / 2, trans.getOrigin().z());
	playerPos_ = PlyaerList[0]->GetPos();
	//playerPos2_ = PlyaerList[1]->GetPos();
	for (auto& player : PlyaerList)
	{
		//playerPos_ = playerpos;
		//SetState(State::Run);

		if (player->GetState() == State::Death)
		{
			aliveFlag_ = true;
			pos_ = initPos_;
		}
	}
		//�͈͊O�̏ꍇ�A���������Ȃ�
		if (!CheckWithinRange(pos_, PlyaerList[0]->GetPos()))
		{
			return;
		}
	

	auto node = stateNode_->first_node("module");
	switch (state_)
	{
	case State::Idle:
		break;
	case State::Run:
	{
		//�����x�N�g���ɗ͂�������
		moveVecPow_ = moveVec_ * movePow_;

		//�i�s�����ɕǂ�����ꍇ���]
		if (!CollisionRay()(pos_, moveVec_, {20,0,0}, node, lpStage.GetALLStageCollList()).HitFlag)	//�J�v�Z��
		{
			if ((abs(Enemybody_->getInterpolationLinearVelocity().x()) < num))
			{
				Enemybody_->applyForce(moveVecPow_.tobtVec(), trans.getOrigin());
			}
		}
		else
		{
			ChangeTurn(moveVec_);
			auto radY = Deg2RadF(moveVec_.x * -90.0f);
			MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_], { 0, radY, 0 });
		}

		//�������΂Ȃ瑬�x���ቺ����
		auto Snode = SpecialHitNode_->first_node("module");
		if (CollisionRay()(pos_, Vector3{ 0.0f,-1.0f,0.0f }, { 0.0f,movePow_.y,0.0f }, Snode, lpStage.GetNorthStageCollList()).HitFlag		//�J�v�Z��
			|| CollisionRay()(pos_, Vector3{ 0.0f,-1.0f,0.0f }, { 0.0f,movePow_.y,0.0f }, Snode, lpStage.GetSouthStageCollList()).HitFlag)		//rei
		{
			movePow_.x = 1000;
			num = 400;
		}
		else
		{
			movePow_.x = 1500;
			num = 800;
		}
		//CheckFall();
	}
	break;
	//case State::Fall:
	//	Fall();
	//	break;
	case State::Magnet:
		break;
	case State::Respawn:
		break;
	case State::Death:
		break;
	case State::Stick:
	{
		auto lineangle = rodLine_.vec();
		auto q = atan2(lineangle.y, lineangle.x);
		float addx = cos(q) * radius_;
		float addy = sin(q) * radius_;

		auto spherePos = VAdd(pos_.toVECTOR(), VGet(0.0f, 50.0f, 0.0f));

		MV1_COLL_RESULT_POLY_DIM colls{};
		for (auto& s : lpStage.GetALLStageCollList())
		{
			for (auto& s2 : s)
			{
				colls = MV1CollCheck_Sphere(s2, -1, spherePos, COLL_SPHERE_SIZE);
				//���������u�Ԃ�for�����甲���o��
				if (colls.HitNum >= 1)
				{
					break;
				}
			}

			if (colls.HitNum >= 1)
			{
				break;
			}
		}

		if (colls.HitNum < 1)
		{
			btVector3 bt = btVector3((trans.getOrigin().x()) - (rodLine_.p.x + addx), (trans.getOrigin().y()) - (rodLine_.p.y + addy), 0);
			/*pos_.x = rodLine_.p.x + addx;
			pos_.y = rodLine_.p.y + addy;*/
			btScalar add = 3000;
			if (bt.x() > 0)
			{
				if (bt.x() > 50)
				{
					bt += btVector3{ add,0,0 };
				}
			}
			else
			{

				if (bt.x() < -50)
				{
					bt += btVector3{ -add,0,0 };
				}
			}

			if (bt.y() > 0)
			{

				if (bt.y() > 50)
				{
					bt += btVector3{ 0,add,0 };
				}
			}
			else if (bt.y() < 0)
			{

				if (bt.y() < -50)
				{
					bt += btVector3{ 0,-add,0 };
				}
			}
			//TRACE("%f\n", Enemybody_->getInterpolationLinearVelocity().y());
			Enemybody_->setLinearVelocity((-bt));
		}
		else
		{

			//SetState(State::Run);

			//for (int i = 0; i < colls.HitNum; i++)
			//{
			//	auto coll = colls.Dim[i];
			//	int cnt = 0;
			//	//�|���S���Ƃ̕�����`�F�b�N
			//	for (int tryCnt = 0; tryCnt < 100; tryCnt++)
			//	{
			//		bool hitFlag = HitCheck_Sphere_Triangle(spherePos, COLL_SPHERE_SIZE, coll.Position[0], coll.Position[1], coll.Position[2]);
			//		if (hitFlag)
			//		{
			//			Moved_Pos_.x = VAdd(pos_.toVECTOR(), VScale(coll.Normal, 1.0f)).x;
			//			Moved_Pos_.y = VAdd(pos_.toVECTOR(), VScale(coll.Normal, 1.0f)).y;
			//			pos_ = Moved_Pos_;
			//			spherePos = VAdd(pos_.toVECTOR(), VGet(0.0f, 50.0f, 0.0f));
			//			cnt++;
			//			continue;
			//		}
			//		break;
			//	}
			//}
		}
		//�����蔻��̌�n��������
		MV1CollResultPolyDimTerminate(colls);
	}
	break;
	case State::Max:
		break;
	default:
		break;
	}

	MV1SetPosition(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_], pos_.toVECTOR());
	MV1RefreshCollInfo(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_]);
}

void Enemy::Draw(void)
{
	//�͈͊O�̏ꍇ�A���������Ȃ�
	if (!CheckWithinRange(pos_,playerPos_))
	{
		return;
	}
	MV1DrawModel(lpModelMng.GetID("./source/model/Enemy/walk_Enemy.mv1")[number_]);
}

EnemyType Enemy::GetEnemyType(void)
{
	return type_;
}

void Enemy::ChangeTurn(Vector3 moveVec)
{
	moveVec_.x = -moveVec.x;
}

Vector3 Enemy::GetPosition(void)
{
	return pos_;
}

void Enemy::SetPosition(Vector3 pos)
{
	pos_ = pos;
}

void Enemy::SetState(State state)
{
	state_ = state;

	if (state_ == State::Stick)
	{
		auto vec = pos_ - rodLine_.p;
		radius_ = vec.Magnitude();
	}
}

State Enemy::GetState(void)
{
	return state_;
}

float Enemy::GetRadius(void)
{
	return radius_;
}

void Enemy::SetRodLine(Line line)
{
	rodLine_ = line;
}


std::string Enemy::GetHandlePath(void)
{
	std::string str = "./source/model/Enemy/walk_Enemy.mv1";
	return str;
}

bool Enemy::GetisAlive(void)
{
	return aliveFlag_;
}

//void Enemy::CheckFall()
//{
//	auto Fnode = FallNode_->first_node("module");
//	if (!CollisionRay()(pos_, Vector3{ 0.0f,-1.0f,0.0f }, {0.0f,movePow_.y,0.0f}, Fnode, lpStage.GetALLStageCollList()).HitFlag &&
//		!CollisionRay()(pos_, Vector3{1.0f,-1.0f,0.0f}, {-movePow_.y,movePow_.y,0.0f}, Fnode, lpStage.GetALLStageCollList()).HitFlag)		//�J�v�Z��
//	{
//		SetState(State::Fall);
//	}
//}
//
//void Enemy::Fall()
//{
//	auto Fnode = FallNode_->first_node("module");
//
//	if (!CollisionRay()(pos_, Vector3{ 0.0f,-1.0f,0.0f }, { 0.0f,movePow_.y + FallPow_,0.0f }, Fnode, lpStage.GetALLStageCollList()).HitFlag)
//	{
//		//�������ĂȂ��ꍇ�A����
//		pos_.y -= (movePow_.y + FallPow_);
//		FallPow_ += 0.2f;
//	}
//	else
//	{
//		//���������ꍇ�A��Ԃ�ύX
//		state_ = State::Run;
//		FallPow_ = 0.0f;
//	}
//}
