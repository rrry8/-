#include "BulletManager.h"
#include"../../../Scene/SceneManager.h"
#include"../../CheckHit.h"
#include"../../../../_debug/_DebugDispOut.h"

constexpr float BULLET_CREATE_TIME = 2.0f;	//弾の生成時間
constexpr int ENEMY_BULLET_MAX_NUM = 8;		//弾の最大数

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
	//敵が何番目かによって管理する弾の数値は変わる
	bulletNum_ = enemyNum * ENEMY_BULLET_MAX_NUM;
	bullets_.reserve(ENEMY_BULLET_MAX_NUM);
}

void BulletManager::Create(Vector3 playerpos, Vector3 enemypos)
{
	//一定時間で弾を生成
	if (stepBulletCreateTime_ >= BULLET_CREATE_TIME)
	{
		//弾の発射方向取得
		auto direction = (playerpos - enemypos).Normalized();
		stepBulletCreateTime_ = 0.0;

		bool createFlag = false;
		//要素のうち、生存していないものは使いまわす
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

		//全て生存している場合は追加する
		//上限を超えている場合は追加しない
		if (!createFlag)
		{
			//敵が何番目かで比較する値が変わる
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
			//弾の更新
			bullet->Update();

			//弾の破棄チェック
			if (bullet->CheckAlive())
			{
				//生存時間を過ぎていた場合破棄する
				bullet->SetAliveFlag(false);
			}
		}
	}
}

void BulletManager::Draw()
{
	//弾の描画
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
			//当たっているならスピードダウンしていって最終的にベクトルが反転する
			bullet->DownSpeed();
		}
	}
}

std::vector<std::shared_ptr<Bullet>>& BulletManager::GetBulletVector(void)
{
	return bullets_;
}
