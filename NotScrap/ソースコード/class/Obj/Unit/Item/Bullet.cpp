#include<DxLib.h>
#include <EffekseerForDXLib.h>
#include "Bullet.h"
#include"../../../Common/AnimationController.h"
#include"../../../Common/ModelMng.h"
#include"../../../Scene/SceneManager.h"
#include "../../CheckHit.h"

constexpr double ALIVE_LIMIT_TIME = 10.0;	//�e�̐�������
constexpr float MOVE_SPEED = 5.0f;			//�e�̃X�s�[�h
constexpr float DIRECTION_DIVISION = 10.0f;	//�X�s�[�h�}�C�i�X���ɏ��Z���Ē���
constexpr double SPEEDDOWN_MAX_TIME = static_cast<double>(MOVE_SPEED * DIRECTION_DIVISION) / 60.0;	//�X�s�[�h�_�E�����N���鎞��
constexpr double ROT_TIME = 1.0f;			//��]��������܂ł̎���
constexpr float ROT_SPEED = 3.0f;			//���[�J����]���x
constexpr float BULLET_RADIUS = 30.0f;		//�����蔼�a
constexpr float DEATH_POS = 2000.0f;		//������̈ʒu
constexpr float MODEL_SCALE = 3.0f;

constexpr VECTOR AXIS_Z = { 0.0f,0.0f,1.0f };	//��]��(Z)

Bullet::Bullet(Vector3 direction, Vector3 enemypos,int num)
{
	num_ = num;

	Init(direction,enemypos);
}

Bullet::~Bullet()
{
}

bool Bullet::Init(void)
{
	return false;
}

bool Bullet::Init(Vector3 direction, Vector3 enemypos)
{
	MV1SetupCollInfo(lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num_], -1, 1, 1, 1);
	direction_ = direction;
	movePower_ = direction * MOVE_SPEED;
	pos_ = enemypos;
	aliveTime_ = 0.0f;
	aliveFlag_ = true;
	inversionFlag_ = false;
	stepSpeedDownTime_ = 0.0;

	//�ʒu�E��]�E�傫�����
	trans_.pos = pos_.toVECTOR();
	auto vec = atan2(direction_.y, direction_.x);
	auto rotY = Deg2RadF(-90.0f);
	trans_.quaRotLocal = Quaternion::Euler(VGet(0.0f, rotY, 0.0f));
	trans_.quaRot = Quaternion::Euler(VGet(0.0f, 0.0f, vec));
	goalRot_ = trans_.quaRot;

	auto matPos = MGetTranslate(trans_.pos);
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetScale(VGet(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE)));
	Quaternion q = trans_.quaRot.Mult(trans_.quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);
	MV1SetMatrix(lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num_], mat);

	//�����G�t�F�N�g
	explosion_.resource = LoadEffekseerEffect("./source/effect/explosion.efkefc", 25.0f);
	explosion_.playhandle = -1;

	return false;
}

void Bullet::Update(void)
{
	auto vec = atan2(direction_.y, direction_.x);
	auto radY = Deg2RadF(-90.0f);

	pos_ += movePower_;

	aliveTime_ += lpSceneMng.GetDeltaTime();

	if (inversionFlag_)
	{
		movePower_ -= (direction_ / DIRECTION_DIVISION);
	}

	//�X�e�[�W�ɏՓ˂����ꍇ����
	for (auto& stage : lpStage.GetALLStageCollList())
	{
		if (CollisionSphere()(pos_, BULLET_RADIUS, stage))
		{
			explosion_.playhandle = PlayEffekseer3DEffect(explosion_.resource);
			explosion_.pos = pos_;
			SetPosPlayingEffekseer3DEffect(explosion_.playhandle, explosion_.pos.x, explosion_.pos.y, explosion_.pos.z);
			PlaySoundMem(lpSoundMng.GetID("./source/Sound/explosion.mp3")[0], DX_PLAYTYPE_BACK);
			//������Ȃ��ꏊ�Ɉړ�
			pos_ = { 0,0,-DEATH_POS };
			aliveFlag_ = false;
		}
	}

	//�ʒu
	trans_.pos = pos_.toVECTOR();
	//���[�J����]
	trans_.quaRotLocal = trans_.quaRotLocal.Mult(AngleAxis(Deg2RadF(ROT_SPEED), AXIS_Z));
	//��]
	trans_.quaRot = trans_.quaRot.Slerp(goalRot_, (ROT_TIME - stepRotTime_) / ROT_TIME);

	//�ʒu�E��]�B�傫�����
	auto matPos = MGetTranslate(trans_.pos);
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetScale(VGet(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE)));
	Quaternion q = trans_.quaRot.Mult(trans_.quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);
	MV1SetMatrix(lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num_], mat);

	//��]��Ԃ̕b��
	if (stepRotTime_ > 0.0)
	{
		stepRotTime_ -= lpSceneMng.GetDeltaTime();
	}

	MV1RefreshCollInfo(lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num_]);
}

void Bullet::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[num_]);
}

bool Bullet::GetAliveFlag(void)
{
	return aliveFlag_;
}

void Bullet::SetAliveFlag(bool alive)
{
	aliveFlag_ = alive;
}

bool Bullet::CheckAlive(void)
{
	if (aliveTime_ >= ALIVE_LIMIT_TIME)
	{
		return true;
	}
	return false;
}

Vector3 Bullet::GetBulletPos(void)
{
	return pos_;
}

void Bullet::DownSpeed(void)
{
	if(!inversionFlag_)
	{
		//�ړ��X�s�[�h�_�E��
		movePower_ -= (direction_ / DIRECTION_DIVISION);
		stepSpeedDownTime_ += lpSceneMng.GetDeltaTime();
		if (stepSpeedDownTime_ > SPEEDDOWN_MAX_TIME)
		{
			inversionFlag_ = true;
			auto vec = atan2(-direction_.y, -direction_.x);
			goalRot_ = AngleAxis(vec, AXIS_Z);
			stepRotTime_ = ROT_TIME;
		}
	}
}

int Bullet::GetNumber(void)
{
	return num_;
}

bool Bullet::GetInversionFlag(void)
{
	return inversionFlag_;
}

