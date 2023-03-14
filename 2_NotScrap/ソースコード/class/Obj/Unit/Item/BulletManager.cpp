#include "BulletManager.h"
#include"../../../Scene/SceneManager.h"
#include"../../CheckHit.h"
#include"../../../../_debug/_DebugDispOut.h"

constexpr float BULLET_CREATE_TIME = 2.0f;	//�e�̐�������
constexpr int ENEMY_BULLET_MAX_NUM = 8;		//�e�̍ő吔

BulletManager::BulletManager(int enemyNum)
{
	Init(enemyNum);
}

BulletManager::~BulletManager()
{
}

void BulletManager::Init(int enemyNum)
{
	stepBulletCreateTime_ = 0.0f;
	enemyNum_ = enemyNum;
	//�G�����Ԗڂ��ɂ���ĊǗ�����e�̐��l�͕ς��
	bulletNum_ = enemyNum * ENEMY_BULLET_MAX_NUM;
	bullets_.reserve(ENEMY_BULLET_MAX_NUM);
}

void BulletManager::Create(Vector3 playerpos, Vector3 enemypos)
{
	//��莞�ԂŒe�𐶐�
	if (stepBulletCreateTime_ >= BULLET_CREATE_TIME)
	{
		//�e�̔��˕����擾
		auto direction = (playerpos - enemypos).Normalized();
		stepBulletCreateTime_ = 0.0;

		bool createFlag = false;
		//�v�f�̂����A�������Ă��Ȃ����͎̂g���܂킷
		for (auto& bullet : bullets_)
		{
			if (!bullet->GetAliveFlag())
			{
				bullet->Init(direction, enemypos);
				bullet->SetAliveFlag(true);
				createFlag = true;
				break;
			}
		}

		//�S�Đ������Ă���ꍇ�͒ǉ�����
		//����𒴂��Ă���ꍇ�͒ǉ����Ȃ�
		if (!createFlag)
		{
			//�G�����Ԗڂ��Ŕ�r����l���ς��
			if (bulletNum_ < ENEMY_BULLET_MAX_NUM * (enemyNum_ + 1))
			{
				bullets_.emplace_back(std::make_unique<Bullet>(direction, enemypos, bulletNum_));
				bulletNum_++;
			}
		}
	}
	else
	{
		stepBulletCreateTime_ += lpSceneMng.GetDeltaTime();
	}
}

void BulletManager::Release(void)
{
}

void BulletManager::Update(void)
{
	for (auto& bullet : bullets_)
	{
		if (bullet->GetAliveFlag())
		{
			//�e�̍X�V
			bullet->Update();

			//�e�̔j���`�F�b�N
			if (bullet->CheckAlive())
			{
				//�������Ԃ��߂��Ă����ꍇ�j������
				bullet->SetAliveFlag(false);
			}
		}
	}
}

void BulletManager::Draw()
{
	//�e�̕`��
	for (auto& bullet : bullets_)
	{
		if (bullet->GetAliveFlag())
		{
			bullet->Draw();
		}
	}
}

void BulletManager::CollisionRodAndBullet(Line line, Line right, Line left)
{
	for (auto& bullet : bullets_)
	{
		int num = bullet->GetNumber();
		if (CollisionRay()(line, lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num]).HitFlag ||
			CollisionRay()(right, lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num]).HitFlag ||
			CollisionRay()(left, lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num]).HitFlag)
		{
			//�������Ă���Ȃ�X�s�[�h�_�E�����Ă����čŏI�I�Ƀx�N�g�������]����
			bullet->DownSpeed();
		}
	}
}

std::vector<std::shared_ptr<Bullet>>& BulletManager::GetBulletVector(void)
{
	return bullets_;
}
