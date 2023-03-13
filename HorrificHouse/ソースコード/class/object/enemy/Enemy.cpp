#include<malloc.h>
#include "Enemy.h"
#include "../../common/manager/SoundMng.h"
#include "../../common/manager/ModelMng.h"
#include"Dijkstra.h"
#include "../gimmick/Gimmick.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"
#include"../../common/Capsule.h"
#include"../Item/Item.h"
#include"../../common/OriginalShader.h"

//�G�̑傫��
constexpr float SIZE_SCALE = 3.7f;

//�v���C���[�T���p�萔
constexpr float SEARCH_ANGLE	= 20.0f * (DX_PI_F / 180.0f);	//���G�p�x
constexpr float SEARCH_RANGE	= 8000.0f;	//���G�͈�
constexpr float SEARCH_OFFSET_Y = 300.0f;	//����̃I�t�Z�b�g����

//�v���C���[�ǐ՗p�萔
constexpr double HUNT_LIMIT_TIME = 3.0;	//�͈͊O�ɂ���Ƃ��̒ǐՂ��鎞��

//��]����
double ROTATION_TIME = 1.0f;
double ROTATION_TIME_gmov = 0.1f;//�Q�[���I�[�o�[���Ƀv���C���[�̂ق�������i���j

//�v���C���[�̃I�t�Z�b�g
constexpr float PLAYER_OFFSET_Y = 76.0f;		//����
constexpr float PLAYER_CAPSULE_OFFSET_RADIUS = 500.0f;	//�J�v�Z���̃I�t�Z�b�g���a

//�X�^���֌W
constexpr double STAN_MAX_LIMIT_TIME  = 7.6;	//�ő厞��
constexpr double STAN_MIN_LIMIT_TIME  = 0.5;	//�ŏ�����
constexpr double STAN_RESUSTANCE_TIME = 1.52;	//�X�^�����邽�тɈ�����鎞��
constexpr double STAN_HEAL_IME		  = 0.5;	//���₷����

Enemy::Enemy(const std::vector<std::shared_ptr<Gimmick>>& gList)
{
	gimmickList_ = gList;
	Init();
}

Enemy::Enemy(Vector3 pos)
{
	Init(pos);
	transform_.pos_ = pos;
}

Enemy::~Enemy()
{
	int a = 0;
}

void Enemy::Init(void)
{
	Vector3 vectorZero = { 0.0f,0.0f,0.0f };
	//�g�����X�t�H�[��
	transform_.Init("./resource/model/enemy/ghost3.mv1", vectorZero, {SIZE_SCALE, SIZE_SCALE, SIZE_SCALE }, vectorZero, vectorZero);
	transform_.quaRotLocal_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.quaRot_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.Update();

	//�A�j���[�V�����֌W
	animController_ = std::make_unique<AnimationController>(transform_.modelHandle_);
	animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Run, "./resource/model/enemy/anim/Run.mv1", 60.0f);
	animController_->Add((int)EnemyAnimation::FemaleDynamicPose, "./resource/model/enemy/anim/Female Dynamic Pose.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Damage, "./resource/model/enemy/anim/Damage.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Walk, "./resource/model/enemy/anim/Walk.mv1", 45.0f);
	animController_->Play((int)EnemyAnimation::Walk, true);

	//�����蔻��
	MV1SetupCollInfo(transform_.modelHandle_, -1, 1, 1, 1);
	

	huntFlag_ = false;		//�ǐՃt���O
	wallAlphaFlag_ = false;	//�Ǔ��߃t���O
	stepHuntTime_ = 0.0;	//�ǐՎ��ԃJ�E���g

	targetRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	dijkstra_ = std::make_unique<Dijkstra>();
	playerCapsule_ = Capsule();
	hitPlayerFlag_ = false;
	stanFlag_ = false;
	nowStanTime_ = STAN_MAX_LIMIT_TIME;
	stepStanTime_ = 0.0;
	searchDir_ = vectorZero;
}

void Enemy::Init(Vector3 pos)
{
	Vector3 vectorZero = { 0.0f,0.0f,0.0f };
	//�g�����X�t�H�[��
	transform_.Init("./resource/model/enemy/ghost3.mv1", pos, { SIZE_SCALE, SIZE_SCALE, SIZE_SCALE }, vectorZero, vectorZero);
	animController_ = std::make_unique<AnimationController>(transform_.modelHandle_);
	animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Run, "./resource/model/enemy/anim/Run.mv1", 60.0f);
	animController_->Add((int)EnemyAnimation::FemaleDynamicPose, "./resource/model/enemy/anim/Female Dynamic Pose.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Damage, "./resource/model/enemy/anim/Damage.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Walk, "./resource/model/enemy/anim/Walk.mv1", 45.0f);
	animController_->Play((int)EnemyAnimation::Walk, true);
	MV1SetupCollInfo(transform_.modelHandle_, -1, 1, 1, 1);

	transform_.quaRotLocal_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.quaRot_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.Update();
	MV1SetPosition(transform_.modelHandle_, transform_.pos_.toVECTOR());

	huntFlag_ = false;
	stepHuntTime_ = 0.0;

	targetRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	dijkstra_ = std::make_unique<Dijkstra>();
	playerCapsule_ = Capsule();
	stanFlag_ = false;
	nowStanTime_ = STAN_MAX_LIMIT_TIME;
	stepStanTime_ = 0.0;
	searchDir_ = vectorZero;
}

void Enemy::Release(void)
{
}

void Enemy::Update(void)
{
	animController_->Update(lpSceneMng.GetDeltaTime());
	//���̍��W�ݒ�
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/enemyWalkSE.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/WalkVoice.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/RunVoice.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/HeatBeat.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/chaseBGM.mp3")[0]);

	if (!huntFlag_)
	{
		//���̒�~
		lpSoundMng.StopSound("./resource/sound/HeatBeat.mp3");
		lpSoundMng.StopSound("./resource/sound/chaseBGM.mp3");
		lpSoundMng.StopSound("./resource/sound/RunVoice.mp3");
		if (!stanFlag_)
		{
			//���̍Đ�
			lpSoundMng.Play3DSound("./resource/sound/enemyWalkSE.mp3", transform_.pos_, 7000.0f);
			lpSoundMng.Play3DSound("./resource/sound/WalkVoice.mp3", transform_.pos_, 7000.0f);
		}

	}

	stepStanTime_ += lpSceneMng.GetDeltaTime();
}

void Enemy::Update(Capsule capsule, const Vector3 playerPos,bool hitFlash, ItemID itemID,const std::vector<std::shared_ptr<Gimmick>>& gimmick)
{
	playerCapsule_ = capsule;
	//�X�V
	Update();

	//�X�^�����͈ړ���ǐՓ������Ȃ�
	if (stanFlag_)
	{
		RecoverFlash();
		return;
	}
	else
	{
		DamageFlash(hitFlash);
	}

	//�ǐՒ����ǂ���
	if (!huntFlag_)
	{
		//�v���C���[��T��
		SearchPlayer(playerPos);
	}
	else
	{
		//�v���C���[��ǐ�
		goalPos_ = playerPos;
		HuntPlayer();
	}

	//�X�^�����Ԃ𒷂�����
	RecoverStanTime();

	bool wallAlphaFlag = false;
	for (const auto& g : gimmick)
	{
		if (g->GetType() == GIMMICK_TYPE::WALL)
		{
			wallAlphaFlag = g->GetIsLockFlg();
			break;
		}
	}

	//�ǂ𓧉߂����u�ԂɒǐՃ��[�h�ɂ���
	if (!wallAlphaFlag && !wallAlphaFlag_)
	{
		wallAlphaFlag_ = true;

		huntFlag_ = true;
		animController_->Play(EnemyAnimation::Run, true);
		//���̍Đ�
		lpSoundMng.Play3DSound("./resource/sound/RunVoice.mp3", transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/HeatBeat.mp3", transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/chaseBGM.mp3", transform_.pos_, 10000.0f);
	}

	//�_�C�N�X�g������
	MovePosition(playerPos,itemID,wallAlphaFlag);

	transform_.Update();
	hitPlayerFlag_ = CheckHitPlayer();
}
//�Q�[���I�[�o�[���̉��o�p�X�V����
void Enemy::PKMotion(Vector3 TargetPos)
{
	Update();
	animController_->Play(EnemyAnimation::Idle, true);
	Lock_On_Target(TargetPos);
	transform_.Update();
}

void Enemy::Draw(void)
{
	transform_.Draw();

#ifdef _DEBUG

	VECTOR ePos = transform_.pos_.toVECTOR();
	ePos.y += SEARCH_OFFSET_Y;

	Vector3 angle = transform_.rot_;

	float x1 = sinf(angle.y);
	float z1 = cosf(angle.y);

	float x2 = sinf(angle.y - SEARCH_ANGLE);
	float z2 = cosf(angle.y - SEARCH_ANGLE);

	float x3 = sinf(angle.y + SEARCH_ANGLE);
	float z3 = cosf(angle.y + SEARCH_ANGLE);

	VECTOR centerEndPos = transform_.pos_.toVECTOR();
	VECTOR leftEndPos = transform_.pos_.toVECTOR();
	VECTOR rightEndPos = transform_.pos_.toVECTOR();

	centerEndPos.x += x1 * SEARCH_RANGE;
	centerEndPos.y += SEARCH_OFFSET_Y;
	centerEndPos.z += z1 * SEARCH_RANGE;

	leftEndPos.x += x2 * SEARCH_RANGE;
	leftEndPos.y += SEARCH_OFFSET_Y;
	leftEndPos.z += z2 * SEARCH_RANGE;

	rightEndPos.x += x3 * SEARCH_RANGE;
	rightEndPos.y += SEARCH_OFFSET_Y;
	rightEndPos.z += z3 * SEARCH_RANGE;

	//�X�e�[�W�Ɏ��E���Փ˂��Ă���ꍇ�͔͈͂��Փ˓_�܂łƂ���
	MV1_COLL_RESULT_POLY lineHit{};

	//�ڂ̑O�̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, centerEndPos);
	if (lineHit.HitFlag)
	{
		centerEndPos = lineHit.HitPosition;
	}
	targetPos_ = { centerEndPos.x ,centerEndPos.y ,centerEndPos.z };
	//���̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, leftEndPos);
	if (lineHit.HitFlag)
	{
		leftEndPos = lineHit.HitPosition;
	}

	//�ڂ̑O�̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, rightEndPos);
	if (lineHit.HitFlag)
	{
		rightEndPos = lineHit.HitPosition;
	}

	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		auto gType = gimmick->GetType();
		switch (gType)
		{
		case GIMMICK_TYPE::NONE:
		case GIMMICK_TYPE::FRAME:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::TEXT:
		case GIMMICK_TYPE::WALL:
		case GIMMICK_TYPE::WATER:
			continue;
		case GIMMICK_TYPE::DL_DOOR:
		case GIMMICK_TYPE::DOOR:
			break;
		default:
			break;
		}
		modelNum = gimmick->GetModelNum();
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, centerEndPos);
		if (lineHit.HitFlag)
		{
			centerEndPos = lineHit.HitPosition;
		}

		//���̐�
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, leftEndPos);
		if (lineHit.HitFlag)
		{
			leftEndPos = lineHit.HitPosition;
		}

		//�ڂ̑O�̐�
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, rightEndPos);
		if (lineHit.HitFlag)
		{
			rightEndPos = lineHit.HitPosition;
		}
	}
	DrawLine3D(ePos, centerEndPos, 0xffff00);
	DrawLine3D(ePos, leftEndPos, 0xffff00);
	DrawLine3D(ePos, rightEndPos, 0xffff00);

	DrawTriangle3D(ePos, leftEndPos, centerEndPos, 0x888800,true);	//��
	DrawTriangle3D(ePos, centerEndPos, rightEndPos, 0x888800, true);	//�E

	DrawFormatString(0, 300, 0xffffff, "�G�̍��W : (%.1f, %.1f, %.1f)", transform_.pos_.x, transform_.pos_.y, transform_.pos_.z);

	dijkstra_->Draw();
#endif // _DEBUG
}

Vector3 Enemy::GetPosition(void)
{
	return transform_.pos_;
}

Vector3 Enemy::GetDirectionVec(void)
{
	auto vec = (searchDir_ - transform_.pos_);
	vec.Normalize();
	return vec;
}

Vector3 Enemy::GetTargetLine(void)
{
	return targetPos_;
}

bool Enemy::GetHitPlayerFlag(void)
{
	return hitPlayerFlag_;
}

bool Enemy::GetHuntFlag(void)
{
	return huntFlag_;
}

void Enemy::SearchPlayer(const Vector3 playerPos)
{
	//�v���C���[�����E�ɂ��邩�ǂ����`�F�b�N
	if (Search(playerPos))
	{
		//�ǐՃ��[�h�ɂ���
		huntFlag_ = true;
		//�v���C���[�̈ʒu�Ƀm�[�h��ǉ�
		dijkstra_->AddHuntNode({ playerPos.x,playerPos.y - PLAYER_OFFSET_Y,playerPos.z }, transform_.pos_);
		animController_->Play(EnemyAnimation::Run, true);

		//���̍Đ�
		lpSoundMng.Play3DSound("./resource/sound/RunVoice.mp3",transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/HeatBeat.mp3", transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/chaseBGM.mp3", transform_.pos_, 10000.0f);
	}
}

bool Enemy::Search(const Vector3 playerPos)
{
	bool hitFlag = false;
	VECTOR ePos = transform_.pos_.toVECTOR();
	ePos.y += SEARCH_OFFSET_Y;

	auto angle = transform_.rot_;

	//�I�_
	VECTOR centerEndPos = transform_.pos_.toVECTOR();
	VECTOR leftEndPos = transform_.pos_.toVECTOR();
	VECTOR rightEndPos = transform_.pos_.toVECTOR();

	centerEndPos.x += sinf(angle.y) * SEARCH_RANGE;
	centerEndPos.y += SEARCH_OFFSET_Y;
	centerEndPos.z += cosf(angle.y) * SEARCH_RANGE;

	leftEndPos.x += sinf(angle.y - SEARCH_ANGLE) * SEARCH_RANGE;
	leftEndPos.y += SEARCH_OFFSET_Y;
	leftEndPos.z += cosf(angle.y - SEARCH_ANGLE) * SEARCH_RANGE;

	rightEndPos.x += sinf(angle.y + SEARCH_ANGLE) * SEARCH_RANGE;
	rightEndPos.y += SEARCH_OFFSET_Y;
	rightEndPos.z += cosf(angle.y + SEARCH_ANGLE) * SEARCH_RANGE;

	MV1_COLL_RESULT_POLY lineHit{};

	searchDir_ = ConvertVECTORtoVector3(centerEndPos);

	//�X�e�[�W�̃R���W�����ɍ��킹�đ傫���ύX
	//�ڂ̑O�̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, centerEndPos);
	if (lineHit.HitFlag)
	{
		centerEndPos = lineHit.HitPosition;
	}

	//���̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, leftEndPos);
	if (lineHit.HitFlag)
	{
		leftEndPos = lineHit.HitPosition;
	}

	//�E�̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, rightEndPos);
	if (lineHit.HitFlag)
	{
		rightEndPos = lineHit.HitPosition;
	}
	
	//�ʂ̃����o�[�S���ӏ�-----------------------------------------------------------------------------------------
	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		auto gType = gimmick->GetType();
		switch (gType)
		{
		case GIMMICK_TYPE::NONE:
		case GIMMICK_TYPE::FRAME:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::TEXT:
			continue;
		case GIMMICK_TYPE::WALL:
		case GIMMICK_TYPE::DL_DOOR:
		case GIMMICK_TYPE::DOOR:
			break;
		default:
			break;
		}
		modelNum = gimmick->GetModelNum();
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, centerEndPos);
		if (lineHit.HitFlag)
		{
			centerEndPos = lineHit.HitPosition;
		}

		//���̐�
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, leftEndPos);
		if (lineHit.HitFlag)
		{
			leftEndPos = lineHit.HitPosition;
		}

		//�ڂ̑O�̐�
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, rightEndPos);
		if (lineHit.HitFlag)
		{
			rightEndPos = lineHit.HitPosition;
		}
	}
	//(�ʂ̃����o�[�S���ӏ�)----------------------------------------------------------------------------------------

	int hit = 0;

	//�G�̎��E�ɏՓ˂��Ă��邩�ǂ�������
	hit = HitCheck_Capsule_Triangle(
		playerCapsule_.topPos,
		playerCapsule_.downPos,
		playerCapsule_.radius,
		ePos,
		leftEndPos,
		centerEndPos);

	if (hit)
	{
		return true;
	}

	hit = HitCheck_Capsule_Triangle(
		playerCapsule_.topPos,
		playerCapsule_.downPos,
		playerCapsule_.radius,
		ePos,
		centerEndPos,
		rightEndPos);

	if (hit)
	{
		return true;
	}

	return false;
}

void Enemy::HuntPlayer(void)
{
	//�������ĎO�b�ȓ��Ȃ�true
	if (stepHuntTime_ < HUNT_LIMIT_TIME)
	{
		if (!Search(goalPos_))
		{
			//���E�O�Ȃ玞�Ԍo��
			stepHuntTime_ += lpSceneMng.GetDeltaTime();
		}
		else
		{
			//���E���̏ꍇ�͎��Ԃ�0.0�ɂ���
			stepHuntTime_ = 0.0;
		}
	}
	else
	{
		//3�b�ȏ�o���Ă���ꍇ�͒ǐՃt���O����������
		if (!Search(goalPos_))
		{
			huntFlag_ = false;
			lpSoundMng.StopSound("./resource/sound/HeatBeat.mp3");
			lpSoundMng.StopSound("./resource/sound/chaseBGM.mp3");
			animController_->Play(EnemyAnimation::Walk, true);
			dijkstra_->ConnectNearNode(transform_.pos_);
		}
		stepHuntTime_ = 0.0;
	}
}

void Enemy::MovePosition(Vector3 playerPos,ItemID itemID,bool alphaFlag)
{
	//�ړ�����
	Vector3 pos = dijkstra_->Update(huntFlag_, { playerPos.x,playerPos.y - PLAYER_OFFSET_Y,playerPos.z }, itemID,alphaFlag);
	Vector3 moveVec = (pos - transform_.pos_).Normalized();
	
	//Y����]����
	Quaternion rotY = AngleAxis(atan2(moveVec.x, moveVec.z), { 0.0f, 1.0f, 0.0f });

	//�p�x�������߂�
	double angleDeg = AngleDifference(rotY, targetRot_);

	//臒l
	if (angleDeg >= 1.0)
	{
		stepRotTime_ = ROTATION_TIME;
		targetRot_ = rotY;
	}

	stepRotTime_ -= lpSceneMng.GetDeltaTime();

	//��]���
	transform_.quaRot_ = transform_.quaRot_.Slerp(targetRot_, (ROTATION_TIME - stepRotTime_) / ROTATION_TIME);
	transform_.pos_ = pos;
}

bool Enemy::CheckHitPlayer(void)
{
	//�G�ƃv���C���[���Փ˂����ꍇ�A�Q�[���I�[�o�[��
	auto hits = MV1CollCheck_Capsule(transform_.modelHandle_, -1, playerCapsule_.topPos, playerCapsule_.downPos, playerCapsule_.radius + PLAYER_CAPSULE_OFFSET_RADIUS);
	bool hitFlag = false;

	if (hits.HitNum > 0)
	{
		hitFlag = true;
	}
	//��n��
	MV1CollResultPolyDimTerminate(hits);
	return hitFlag;
}

void Enemy::DamageFlash(bool hitFlash)
{
	if (hitFlash)
	{
		stepStanTime_ = 0.0;
		stanFlag_ = true;
		animController_->Play(EnemyAnimation::Damage, false);
		lpSoundMng.StopSound("./resource/sound/enemyWalkSE.mp3");
		lpSoundMng.Play3DSound("./resource/sound/WalkVoice.mp3", transform_.pos_, 7000.0f);
	}
}

void Enemy::RecoverFlash(void)
{
	if (stepStanTime_ > nowStanTime_)
	{
		huntFlag_ = false;
		stanFlag_ = false;
		animController_->Play(EnemyAnimation::Walk, true);
		//�X�^�����ԒZ�k
		ReduceStanTime();
	}
}

void Enemy::RecoverStanTime(void)
{
	nowStanTime_ += lpSceneMng.GetDeltaTime() / 6.0;
	if (nowStanTime_ > STAN_MAX_LIMIT_TIME)
	{
		nowStanTime_ = STAN_MAX_LIMIT_TIME;
	}
}

void Enemy::ReduceStanTime(void)
{
	nowStanTime_ -= STAN_RESUSTANCE_TIME;

	//�Œ�X�^�����Ԃ�0.5�b
	if (nowStanTime_ < STAN_MIN_LIMIT_TIME)
	{
		nowStanTime_ = STAN_MIN_LIMIT_TIME;
	}
}

void Enemy::Lock_On_Target(Vector3 targetPos)
{
	Vector3 moveVec = (targetPos - transform_.pos_).Normalized();

	//Y����]����
	Quaternion rotY = AngleAxis(atan2(moveVec.x, moveVec.z), { 0.0f, 1.0f, 0.0f });

	//�p�x�������߂�
	double angleDeg = AngleDifference(rotY, targetRot_);

	//臒l����
	if (angleDeg >= 1.0)
	{
		stepRotTime_ = ROTATION_TIME_gmov;
		targetRot_ = rotY;
	}

	stepRotTime_ -= lpSceneMng.GetDeltaTime();

	transform_.quaRot_ = transform_.quaRot_.Slerp(targetRot_, (ROTATION_TIME_gmov - stepRotTime_) / ROTATION_TIME_gmov);
}
