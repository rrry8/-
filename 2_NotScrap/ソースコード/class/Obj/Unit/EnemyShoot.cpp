#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"../../Common/ImageMng.h"
#include "EnemyShoot.h"
#include"Item/BulletManager.h"
#include"../../Scene/SceneManager.h"
#include"../../../_debug/_DebugDispOut.h"
#include "../../Common/SoundMng.h"
#include "../Unit/Player.h"

constexpr float MOVE_SPEED = 50.0f;	//動く速度
constexpr float ROT_TIME = 1.5f;	//回転完了するまでの時間
constexpr float SPHERE_SIZE = 50.0f;	//当たり判定
constexpr float SIN_PERIOD = 60.0f;		//サインカーブ周期
constexpr float MODEL_SCALE = 0.5f;
constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

EnemyShoot::EnemyShoot(EnemyType type,Vector3 pos, int number)
{
	type_ = type;
	initPos_ = pos;
	number_ = number;
	Init();
}

EnemyShoot::~EnemyShoot()
{
}

bool EnemyShoot::Init()
{
	auto rotY = Deg2RadF(90.0f);
	pos_ = initPos_;
	trans_.quaRotLocal = Quaternion::Euler(VGet(0.0f, rotY, 0.0f));

	stepSin_ = 0.0;
	state_ = State::Idle;
	aliveFlag_ = true;
	dir_ = DIR::LEFT;
	goalRot_ = Quaternion(1.0, 0.0, rotY, 0.0);
	stepRotTime_ = 0.0f;

	//位置・回転。大きさ情報
	auto matPos = MGetTranslate((pos_ + Vector3{ 0.0f, sin(DX_PI_F * 2.0f / SIN_PERIOD *static_cast<float>(stepSin_)) * MOVE_SPEED, 0.0f }).toVECTOR());
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetScale(VGet(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE)));
	Quaternion q = trans_.quaRot.Mult(trans_.quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);
	MV1SetMatrix(lpModelMng.GetID("./source/Model/Enemy/shoot_Enemy.mv1")[number_], mat);

	//爆発エフェクト
	explosion_.resource = LoadEffekseerEffect("./source/effect/explosion.efkefc",25.0f);
	explosion_.playhandle = -1;

	//弾
	bullets_ = std::make_shared<BulletManager>(number_);

	return false;
}

void EnemyShoot::Update(void)
{
}

void EnemyShoot::Update(std::vector<std::shared_ptr<Player>>& player)
{
	//プレイヤーの位置更新
	playerPos_ = player[0]->GetPos();

	//弾移動
	bullets_->Update();

	if (player[0]->GetState() == State::Death)
	{
		aliveFlag_ = true;
		pos_ = initPos_;
	}
	//範囲外の場合、処理をしない
	if (!CheckWithinRange(pos_, player[0]->GetPos()) || !aliveFlag_)
	{
		return;
	}

	//反射された弾が当たった場合、衝突判定をする
	for (const auto& bullet : bullets_->GetBulletVector())
	{
		if (bullet->GetInversionFlag())
		{
			//当たり判定
			if (CollisionSphere()(pos_, SPHERE_SIZE, "./source/Model/Enemy/bullet.mv1", bullet->GetNumber()))
			{
				explosion_.playhandle = PlayEffekseer3DEffect(explosion_.resource);
				explosion_.pos = pos_;
				aliveFlag_ = false;
				lpSoundMng.PlayingSound("./source/Sound/EnemyDeath.mp3");
				break;
			}
		}
	}

	//回転
	Rotate();

	//更新
	auto matPos = MGetTranslate((pos_ + Vector3{ 0.0f, sin(static_cast<float>(stepSin_) / SIN_PERIOD) * MOVE_SPEED, 0.0f }).toVECTOR());
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetScale(VGet(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE)));
	Quaternion q = trans_.quaRot.Mult(trans_.quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);
	MV1SetMatrix(lpModelMng.GetID("./source/Model/Enemy/shoot_Enemy.mv1")[number_], mat);

	//サインカーブ更新
	stepSin_ += 1.0;

	//弾生成
	bullets_->Create(playerPos_, pos_);

	SetPosPlayingEffekseer3DEffect(explosion_.playhandle, explosion_.pos.x, explosion_.pos.y, explosion_.pos.z);
}

void EnemyShoot::Draw(void)
{
	//弾
	bullets_->Draw();
	//範囲外の場合、処理をしない
	if (!CheckWithinRange(pos_,playerPos_)||!aliveFlag_)
	{
		return;
	}
	//敵の描画
	MV1DrawModel(lpModelMng.GetID("./source/Model/Enemy/shoot_Enemy.mv1")[number_]);
}

Vector3 EnemyShoot::GetPosition(void)
{
	return pos_;
}

std::string EnemyShoot::GetHandlePath(void)
{
	std::string str = "./source/Model/Enemy/shoot_Enemy.mv1";
	return str;
}

EnemyType EnemyShoot::GetEnemyType(void)
{
	return type_;
}

bool EnemyShoot::GetisAlive(void)
{
	return aliveFlag_;
}

std::shared_ptr<BulletManager> EnemyShoot::GetBulletData(void)
{
	return bullets_;
}

void EnemyShoot::Rotate(void)
{
	Vector3 vec = (playerPos_ - pos_).Normalized();

	//プレイヤーの方向に回転する
	Quaternion rotZ;
	//角度が90度を超えた場合は反転させる
	if (abs(Rad2DegF(atan2(-vec.y, -vec.x))) < 90.0f)
	{
		//左向き
		if (dir_ != DIR::LEFT)
		{
			dir_ = DIR::LEFT;
			goalRot_ = AngleAxis(Deg2RadF(90.0f), AXIS_Y);
			stepRotTime_ = ROT_TIME;
		}
		rotZ = AngleAxis(atan2(-vec.y, -vec.x), AXIS_Z);
	}
	else
	{
		//右向き
		if (dir_ != DIR::RIGHT)
		{
			dir_ = DIR::RIGHT;
			goalRot_ = AngleAxis(Deg2RadF(-90.0f), AXIS_Y);
			stepRotTime_ = ROT_TIME;
		}
		rotZ = AngleAxis(atan2(vec.y, vec.x), AXIS_Z);
	}

	//補間して回転
	trans_.quaRotLocal = trans_.quaRotLocal.Slerp(goalRot_, (ROT_TIME - stepRotTime_) / ROT_TIME);
	trans_.quaRot = trans_.quaRot.Slerp(rotZ, (ROT_TIME - stepRotTime_) / ROT_TIME);

	stepRotTime_ -= lpSceneMng.GetDeltaTime();
}
