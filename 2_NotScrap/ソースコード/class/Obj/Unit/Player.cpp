#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "Player.h"
#include"Item/MagStick.h"
#include "../Common/Capsule.h"
#include "../../Input/KeyInput.h"
#include "../../Scene/SceneManager.h"
#include "../../Common/Designer.h"
#include "../../Common/ModelMng.h"
#include "../../Common/AnimationController.h"
#include "../../Common/ImageMng.h"
#include "../Gimmick/PressurePlate.h"
#include "../Gimmick/PressureDoor.h"
#include "Enemy.h"
#include"EnemyShoot.h"
#include "../../Common/SoundMng.h"
#include"Item/Coin.h"
#include "Item/BulletManager.h"

//�v���C���[
constexpr float MOVE_LIMIT = 600.0f;
constexpr float PLAYER_SIZE = 30.0f;
constexpr float PLAYER_SPEED = 5.0f;
constexpr float PLAYER_RAD = -90.0f;
constexpr float PLAYER_SCALE = 0.35f;
constexpr float PLAYER_ANIM_JUMP = 30.0f;
constexpr float PLAYER_ANIM_IDLE = 20.0f;
constexpr float PLAYER_ANIM_RUN = 40.0f;
//�v���C���[bullet
constexpr btScalar BULLET_SIZE = PLAYER_SIZE;
constexpr btScalar BULLET_HEIGHT = 70;
constexpr btScalar BULLET_FRICTION = 2;
//�J�v�Z��
constexpr VECTOR CAPSULE_POS_DOWN = VECTOR(100.0f, 110.0f, 0.0f) ;
constexpr VECTOR CAPSULE_POS_UP = VECTOR{ 100.0f,	30.0f,	0.0f };
constexpr float CAPSULE_RADIUS = 30.0f;
constexpr float CAPSULE_OFFSET = 20.0f;
//���x����
constexpr float TARGET = 1000.0f;
constexpr float DISTANCE = 600.0f;
constexpr float TIME_ADD = 0.05f;
//�j��v���C���[
constexpr float BREAK_SCALE = 0.45f;
constexpr float BREAK_ADD_POS_Y = 40.0f;
constexpr float BREAK_SIZE = PLAYER_SIZE;
constexpr float BREAK_COLL_SIZE = 0.05f;
constexpr float BREAK_THRESHOLD = 0.1f;
//�G�t�F�N�g
constexpr float EFFECT_SPARK = 70.0f;
constexpr float EFFECT_EXPLOSION = 30.0f;
constexpr float EFFECT_TRAIL = 40.0f;
constexpr float EFFECT_TRAIL_ANGLE_R= 180.0f;
constexpr float EFFECT_TRAIL_ANGLE_L = 0.0f;
//�|�C���g���C�g
constexpr float POINTLIGHT_POS = -1000.0f;
constexpr float POINTLIGHT_RANGE = 2000.0f;
constexpr float POINTLIGHT_ATTEN1 = 0.0009f;
constexpr VECTOR POINTLIGHT_VEC0 = { -1.0f,-0.3f,0.0f };
constexpr VECTOR POINTLIGHT_VEC1 = { 1.0f,-0.3f,0.0f };
constexpr VECTOR POINTLIGHT_POS0 = { -90,110,0 };
constexpr VECTOR POINTLIGHT_POS1 = { 90,110,0 };
//����
constexpr float MID_SIZE_X = 50.0f;
constexpr float MID_SIZE_Y = 150.0f;
//�����蔻��
constexpr float SP_SIZE = 10.0f;
constexpr float HIT_RAY_DEF = 2.0f;
constexpr float HIT_RAY_SIZE = 3.5f;
//���΃X���C�h
constexpr float DRAG_MOVE_VEC = 30.0f;
constexpr float DRAG_MOVE_SIZE = 3.0f;
//���Ίɂ₩�㏸
constexpr float MAGNET_WEAK_MOVE_POW = 40.0f;
constexpr float MAGNET_WEAK_DOWN_POW = 130.0f;
constexpr float MAGNET_WEAK_NOT_HIT_POW_X =20.0f;
constexpr float MAGNET_WEAK_NOT_HIT_POW_Y = 60.0f;

constexpr float MAGNET_MOVE_VEC_POW = 60.0f;
//����
constexpr float FALL_DOWN_POW =1000.0f;
constexpr float FALL_SPEED_LIMIT = 1500.0f;
//�L�[�ړ�
constexpr float KEY_MOVE_POW = 3.0f;
constexpr float KEY_MOVE_SPARK_ON_LIMIT = 50.0f;
//���S
constexpr float DEATH_TIME_MAX = 60.0f;
constexpr float AMPLITUDE_ADD = 0.9f;
constexpr float AMPLITUDE_DEF = 100.0f;
constexpr int   DEATH_DELAY = 3;
constexpr float DEATH_POS_MAX_Y = -4500.0f;
constexpr float DEATH_POW_Y = 700.0f;
constexpr float DEATH_POW_Y2 = 700.0f;
constexpr float DEATH_POW_X = 10.0f;
constexpr float DEATH_POW_X2 = 5.0f;
constexpr float DEATH_POW_Z = 1.0f;
constexpr float DEATH_POW_Z2 = 5.0f;
constexpr float DEATH_TIME_LIMIT = 5.0f;
constexpr float DEATH_TIME_LIMIT_2 = 15.0f;
constexpr float DEATH_TIME_LIMIT_3 = 30.0f;
constexpr float DEATH_TIME_LIMIT_MAX = 120.0f;
constexpr int   DEATH_WAIT_TIME = 30;
//����
constexpr float OPACITY_DEF_RATE = 1.0f;
constexpr float OPACITY_RATE = 0.5f;

Player::Player(InputUnique controller)
{
	SetInput(std::move(controller));
	Init();
}

Player::Player(InputUnique controller, Vector3 pos,int num, CoinList& CoinList, EneymyList& EneymyList)
{
	ModelNum_ = num;
	Playernum_ = static_cast<PlayerNum>(num);

	SetInput(std::move(controller));
	SetCoinList(CoinList);
	SetUnitList(EneymyList);

	Init(pos);
}

Player::~Player()
{
	DeleteLightHandle(PointLightHandle_);
	magStick_->Release();
	//DeleteGraph();
}

bool Player::Init(void)
{
	Init({ 0,0,0 });
	return false;
}

bool Player::Init(Vector3 pos)
{
	defpos_ = pos;
	resPoint_ = RespawnPoint::Start;
	ReInit();
	state_ = State::Idle;
	size_  = { PLAYER_SIZE,PLAYER_SIZE,0 };//��

	speed_ = PLAYER_SPEED;

	//tmx�t�@�C���ǂݍ���
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	//�m�[�h�̍쐬
	stateNode_		= stateDoc.first_node("Playermodule");
	FallNode_		= stateDoc.first_node("Fallmodule");
	UpMagNode_		= stateDoc.first_node("UpMag");
	DownMagNode_	= stateDoc.first_node("DownMag");
	MoveNode_		= stateDoc.first_node("Move");
	SpecialHitNode_ = stateDoc.first_node("SpecialHitmodule");
	MagnetWeakNode_ = stateDoc.first_node("MagnetWeak");

	//�I���W�i���V�F�[�_�[
	int pso = LoadPixelShader("./source/shader/ps_Sph.pso");
	int vso = LoadVertexShader("./source/shader/vs.vso");
	//�e�N�X�`���ǂݍ���
	texmap.diffuse		= LoadGraph("./source/Textures/Player/MergedBake_diffuse.png");
	texmap.norm			= LoadGraph("./source/Textures/Player/MergedBake_normal.png");
	texmap.rough		= LoadGraph("./source/Textures/Player/MergedBake_roughness.png");
	texmap.metallic		= LoadGraph("./source/Textures/Player/MergedBake_metalness.png");
	texmap.shp			= LoadGraph("./source/Textures/Player/sph.png");
	//�I���W�i���V�F�[�_�[�̃C���X�^���X
	//originalShader_ = std::make_unique<OriginalShader>(pso, vso,4,4);

	//�J�v�Z���̍쐬
	capsule_ = std::make_unique<Capsule>();
	capsule_->SetRelativePosDown(CAPSULE_POS_DOWN);
	capsule_->SetRelativePosTop(CAPSULE_POS_UP);
	capsule_->SetRadius(CAPSULE_RADIUS);
	

	//�W�����v�̌v�Z������
	DeltaDesignerStack_ = 0;
	v_target = TARGET;//�ڕW���x
	distance = DISTANCE;//�ړ�����
	ad_ = std::make_unique<Designer>(j_max, a_max, v_max, v_start, v_target, distance);

	//�j�󃂃f���̓ǂݍ���
	BreakmodelH_[0] = lpModelMng.GetID("./source/model/Player/player_ArmL.mv1")	 [ModelNum_];
	BreakmodelH_[1] = lpModelMng.GetID("./source/model/Player/player_ArmR.mv1")	 [ModelNum_];
	BreakmodelH_[2] = lpModelMng.GetID("./source/model/Player/player_Basket.mv1")[ModelNum_];
	BreakmodelH_[3] = lpModelMng.GetID("./source/model/Player/player_Body.mv1")  [ModelNum_];
	BreakmodelH_[4] = lpModelMng.GetID("./source/model/Player/player_FootL.mv1") [ModelNum_];
	BreakmodelH_[5] = lpModelMng.GetID("./source/model/Player/player_FootR.mv1") [ModelNum_];
	BreakmodelH_[6] = lpModelMng.GetID("./source/model/Player/player_Head.mv1")  [ModelNum_];

	//���f���̑傫��������
	float scale = BREAK_SCALE;
	for (int i = 0; i < CreateMax; i++)
	{
		MV1SetScale(BreakmodelH_[i], { scale,scale,scale });
	}

	//�v���C���[�̌����Ƒ傫��������
	auto radY = Deg2RadF(PLAYER_RAD);
	MV1SetScale(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_], { PLAYER_SCALE,PLAYER_SCALE,PLAYER_SCALE });
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_], { 0, radY, 0 });

	//�A�j���[�V�����ǉ�
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_]);
	animController_->Add ((int)ANIM_TYPE::JUMP, "./source/animation/jump_anim.mv1", PLAYER_ANIM_JUMP);
	animController_->Add ((int)ANIM_TYPE::IDLE, "./source/animation/idle_anim.mv1", PLAYER_ANIM_IDLE);
	animController_->Add ((int)ANIM_TYPE::RUN,  "./source/animation/walk_anim.mv1", PLAYER_ANIM_RUN);
	animController_->Play((int)ANIM_TYPE::IDLE);

	//�G�t�F�N�g
	explosion_.resource		= LoadEffekseerEffect("./source/effect/explosion.efkefc", EFFECT_EXPLOSION);
	explosion_.playhandle	= -1;
	trail_.resource			= LoadEffekseerEffect("./source/effect/trail.efkefc", EFFECT_TRAIL);
	trail_.playhandle		= -1;
	
	//���C�g�̍쐬
	PointLightHandle_ = CreateSpotLightHandle({0,POINTLIGHT_POS,0}, VGet(1.0f, 0.0f, 0.0f),
		DX_PI_F / 2.0f,
		DX_PI_F / 2.0f,
		POINTLIGHT_RANGE,
		0.0f,
		POINTLIGHT_ATTEN1,
		0.0f);

	state_		= State::Magnet;
	midSize_	= { MID_SIZE_X,MID_SIZE_Y,0.0f };
	MoveMagVec_ = { 0,-1,0 };

	//�擾�t���O������
	getCoinflg_    = false;
	getStepOnflg_  = false;
	//�ړ��t���O�̏�����
	MagDragflagLR_ = true;
	SetMagnetPow();
	Move();
	lpStage.CleanBulletObj(Playerbody_);
	ReInit();
	return false;
}

bool Player::ReInit()
{
	pos_= defpos_;//��
	SetRespawnPoint();
	Playerbody_ = lpStage.BulletCreateCapsule(pos_, BULLET_SIZE, BULLET_HEIGHT, BULLET_FRICTION, btVector3(1, 1, 0), btVector3(0, 0, 0), Playerbody_);
	MV1SetPosition(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_], pos_.toVECTOR());
	magStick_	= std::make_unique<MagStick>(pos_,input_->GetCntType(),input_->GetCntData(), ModelNum_);
	movePow_	= SetSpeed(SpeedMode::Normal);
	deathTime_	= 0.0f;
	amplitude_  = 0;

	state_	= State::Respawn;
	MMdir_  = MagMouseDir::Non;

	NowMagVec_	   = Vector3Zero;
	OldMagVec_	   = Vector3Zero;
	MoveMagVec_	   = Vector3Zero;
	OldMoveMagPos_ = Vector3Zero;

	MagDragflagLR_ = false;
	UpDragflag_	   = false;
	aliveFlag_       = false;
	
	
	//lpBulletMng.Release();
	auto radY = Deg2RadF(PLAYER_RAD);
	BreakPlayerbody_.resize(CreateMax);
	
	return false;
}

bool Player::ReCreateBullet(void)
{
	auto ve = btVector3{ 1,1,1 };
	for (int i = 0; i < CreateMax; i++)
	{
		BreakPlayerbody_[i] = (lpStage.BulletCreateBox(
			1,
			pos_ + Vector3{0,BREAK_ADD_POS_Y,0},
			btVector3(BREAK_SIZE, BREAK_SIZE, BREAK_SIZE),
			BREAK_THRESHOLD,
			BREAK_COLL_SIZE,
			BULLET_FRICTION,
			ve,
			ve,
			BreakPlayerbody_[i]));
	}
	return false;
}

void Player::Update(void)
{	
}

void Player::Update(GimmickList& gimmicklist,const EneymyList& enemy)
{
	//���񂾎��̏���
	if (deathUpdate())
	{
		//����ł����瑼�̍X�V�͍s��Ȃ�
		return;
	}
	//�ŏ��̍X�V
	InitUpdate(gimmicklist);
	
	//state�̍X�V
	StateUpdate();
	//���Η͂̃Z�b�g
	SetMagnetPow();
	//�ړ�
	Move();
	
	//�J�v�Z���Ƃ̂����蔻��
	HitCapsule(enemy);
	
	ColCheckMidPoint();

	//���Ζ_�̍X�V
	UpdateMagStick(enemy);
	//effect�̍X�V
	UpdateEffect();
	//���΂̓��ߏ���
	OpacityMagnet();
}

void Player::InitUpdate(GimmickList& gimmicklist)
{
	//�f�o�b�O���O�\��
	if (CheckHitKey(KEY_INPUT_1))
	{
		logflag_ = true;
	}
	//���X�|�[���������̕����ɕύX
	if (State::Respawn == state_)
	{
		state_ = State::Idle;
	}
	//�M�~�b�N���X�g�̍X�V
	GimmickList_ = gimmicklist;
	//bullet�̃v���C���[�̓����蔻��̍X�V
	Playerbody_->getMotionState()->getWorldTransform(trans);
	//pos�̍X�V
	Moved_Pos_		= pos_ = Vector3(static_cast<float>(trans.getOrigin().x()), static_cast<float>(trans.getOrigin().y() - 80.0f), static_cast<float>(trans.getOrigin().z()));
	//�ړ��ʂ����Z�b�g
	moveVecPow_		= Vector3Zero;
	moveVecPowMag_	= Vector3Zero;

	input_->Update();
	//input_->KeyConfig_Load("./source/Data/keyconfig/keyConfig.txt");
	// 
	animController_->Update(lpSceneMng.GetDeltaTime());


}

void Player::UpdateMagStick(const EneymyList& enemy)
{
	//���΂̖_�X�V
	magStick_->Update(GimmickList_, Boxlist_, pos_, moveVecPow_ - moveVecPowMag_, state_, input_->GetCntData(), enemy);
	//���΂̖_�̊p�x�X�V
	MagBoxVec_ = magStick_->GetMagBoxVec();
}

void Player::StateUpdate()
{
	//���΂̃X�e�[�^�X�Z�b�g
	SetMagnetState();
	//run�̃X�e�[�^�X�Z�b�g
	SetRunState();
	//���̃X�e�[�^�X�Z�b�g
	SetDeathState();
}

std::string Player::GetHandlePath(void)
{
	return std::string();
}

EnemyType Player::GetEnemyType(void)
{
	return EnemyType();
}

void Player::Draw()
{
	MagStickDraw();
	//capsule_->Draw();
	//���f���`��
	if (state_ == State::DismemberDeath || state_ == State::Death)
	{
		for (int i = 0; i < CreateMax; i++)
		{
			MV1DrawModel(BreakmodelH_[i]);
		}
	}
	else
	{
		//originalShader_->Draw(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_],
		//	texmap.diffuse,
		//	texmap.norm,
		//	texmap.rough,
		//	texmap.metallic,
		//	texmap.shp);
		MV1DrawModel(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_]);
	}
	//�e
	DrawShadow(300.0,100.0);
}

void Player::Draw(int num)
{
	MV1DrawModel(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_]);
	DrawEffekseer3D();
}

void Player::MagStickDraw()
{
	magStick_->Draw();
}

//�ǂ̃}�O�l�b�g�����s���邩
void Player::SetMagnetState(void)
{
	
	auto cntData = input_->GetCntData();
	//�O�̓��͂��c���Ă�
	OldMagVec_	= NowMagVec_;
	//�X�V
	NowMagVec_	= magStick_->GetPlayerMoveVec();
	
	//��������������������オ��
	if (NowMagVec_ != Vector3Zero)
	{
		if (state_ != State::MagnetWeak)
		{
			OldMoveMagPos_ = pos_;
			state_ = State::MagnetWeak;
			MMdir_ = MagMouseDir::Non;
			if ((NowMagVec_.x < 0 && NowMagVec_.y > 0) || NowMagVec_.x > 0 && NowMagVec_.y > 0)
			{
				if (magStick_->GetLine().vec().Normalized().x < 0 && magStick_->GetLine().vec().Normalized().y > 0 || magStick_->GetLine().vec().Normalized().x > 0 && magStick_->GetLine().vec().Normalized().y > 0)
				{
					//TRACE("%f\n", magStick_->GetLine().vec().Normalized().y);
					MMdir_ = MagMouseDir::DownUp;
				}
			}
			if ((NowMagVec_.x < 0 && NowMagVec_.y < 0) || NowMagVec_.x > 0 && NowMagVec_.y < 0)
			{
				if (magStick_->GetLine().vec().Normalized().x < 0 && magStick_->GetLine().vec().Normalized().y > 0 || magStick_->GetLine().vec().Normalized().x > 0 && magStick_->GetLine().vec().Normalized().y > 0)
				{
					//TRACE("%f\n", magStick_->GetLine().vec().Normalized().y);
					MMdir_ = MagMouseDir::UpUp;
				}
			}
		}
	}
	else
	{
		if (state_ != State::Magnet)
		{
			state_ = State::Fall;
		}
	}
	//
	//�N���b�N���͂�������}�O�l�b�g�ɂ���
	if (OldMagVec_ != Vector3Zero)
	{
		if (NowMagVec_ == Vector3Zero)
		{
			//if (cntData[InputID::Change][static_cast<int>(Trg::Now)])
			{
				if (state_ == State::MagnetWeak)
				{
					auto Range_of_rise = OldMoveMagPos_.y - pos_.y;
					//�������̈ړ���������
					//TRACE("�㏸���F%f", Range_of_rise);
					ad_->reset(j_max, a_max, v_max, v_start, v_target, distance - abs(Range_of_rise));
				}
				state_ = State::Magnet;
				MoveMagVec_ = OldMagVec_;
				//�������̃}�O�l�b�g�̎��A�n�ʂɓ��������獶�E�Ɉړ�������t���O
				if ((MoveMagVec_.x < 0 && MoveMagVec_.y > 0) || MoveMagVec_.x > 0 && MoveMagVec_.y > 0)
				{
					MagDragflagLR_ = true;
				}
				//���͂��Ȃ��Ȃ����Ƃ��N���b�N���Ă����痎��������
				if (cntData[InputID::BtnN][static_cast<int>(Trg::Now)] || cntData[InputID::BtnS][static_cast<int>(Trg::Now)])
				{
					state_ = State::Fall;
				}

			}

		}
	}

}
//���΂̋���
void Player::SetMagnetPow(void)
{
	if (state_ == State::Magnet)
	{
		trail_.playhandle = PlayEffekseer3DEffect(trail_.resource);
		trail_.pos = pos_;
		SetPosPlayingEffekseer3DEffect(trail_.playhandle, trail_.pos.x, trail_.pos.y, trail_.pos.z);
		SetRotationPlayingEffekseer3DEffect(trail_.playhandle, 0.0f, trail_.angle, 0.0f);

		CollRayNormRecord_N_ = { 0,0,0 };
		CollRayNormRecord_S_ = { 0,0,0 };
		if (DeltaDesignerStack_ < ad_->t_end())
		{
			//����
			{
				DeltaDesignerStack_ += TIME_ADD;//�f���^�^�C���łƂ��Ă��������x�����ɂȂ�Ȃ������̂ŌŒ�ɂ��Ă���
			}

			//�����Ɨ͂�������
			//ad_�łƂ��Ă����ړ���������O��̈ړ������������č������Ƃ��Ă���(���ꂪ�ړ����x�ɂȂ�)
			ad_MagVec_	   = ad_->x(static_cast<float>(DeltaDesignerStack_)) - olddistance_;
			OldMoveMagVec_ = MoveMagVec_;
			//�}�E�X�̕����̃x�N�g����MagVec�������Ĉړ��������������߂�
			moveVecPowMag_ += MoveMagVec_ * ad_MagVec_;
			//�������Ƃ邽�߂ɑO��̈ړ��������i�[
			olddistance_   = ad_->x(static_cast<float>(DeltaDesignerStack_));

			auto Dnode   = DownMagNode_->first_node("module");
			rapidxml::xml_node<>* node = stateNode_->first_node("module");
			auto SPcollN = CollisionRay()(Moved_Pos_, -MoveMagVec_, { speed_ * SP_SIZE,speed_ * SP_SIZE,0 }, node, lpStage.GetNorthStageCollList());
			auto SPcollS = CollisionRay()(Moved_Pos_, -MoveMagVec_, { speed_ * SP_SIZE,speed_ * SP_SIZE,0 }, node, lpStage.GetSouthStageCollList());
			auto Hitpos  = CollisionRay()(Moved_Pos_, -MoveMagVec_, { HIT_RAY_DEF * HIT_RAY_SIZE,speed_ * HIT_RAY_SIZE,0 }, node, lpStage.GetALLStageCollList(), true);

			if (!Hitpos.HitFlag)
			{
				if (UpDragflag_)
				{
					//���������������̉��ړ�
					//Y���[���ɂ��č��E�ɂ��������Ȃ�����
					MoveMagVec_.y = 0;
					moveVecPowMag_.y = 0;
					return;
				}
				//�������ĂȂ�
				if (!CollisionRay()(Moved_Pos_, Vector3{ -MoveMagVec_.x,1,0 }, Vector3{ speed_ ,speed_,0 }, Dnode, lpStage.GetALLStageCollList(), true).HitFlag)
				{

					if (!MagDragflagLR_)
					{
						if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/��02.mp3")[0]) == 0)
						{
							PlaySoundMem(lpSoundMng.GetID("./source/Sound/��02.mp3")[0], DX_PLAYTYPE_BACK);
						}
					}

				}
			}
			else
			{
				if (UpDragflag_)
				{
					//���������������̉��ړ�
					//Y���[���ɂ��č��E�ɂ��������Ȃ�����
					MoveMagVec_.y = 0;
					moveVecPowMag_.y = 0;
					return;
				}
				CollRayNormRecord_N_ = Vector3{ static_cast<float>(static_cast<int>(SPcollN.Normal.x)), static_cast<float>(static_cast<int>(SPcollN.Normal.y)), 0 };
				CollRayNormRecord_S_ = Vector3{ static_cast<float>(static_cast<int>(SPcollS.Normal.x)), static_cast<float>(static_cast<int>(SPcollS.Normal.y)), 0 };

				if (CollRayNormRecord_N_.y < 0 || CollRayNormRecord_S_.y < 0)
				{
					//���������������̉��ړ�
				//Y���[���ɂ��č��E�ɂ��������Ȃ�����
					UpDragflag_ = true;
					//TRACE("Uptrue\n");

				}
				if (CollRayNormRecord_N_.x < 0 || CollRayNormRecord_S_.x < 0 || CollRayNormRecord_N_.x > 0 || CollRayNormRecord_S_.x > 0)
				{
					//�n�ʂɉ���������Ƃ�
					//X���[���ɂ��ď㉺�ɂ��������Ȃ�����
					MagDragflagLR_ = false;
					if (moveVecPowMag_.y < 0)
					{
						auto Movedpos = Moved_Pos_;
						Movedpos.x -= MoveMagVec_.x * DRAG_MOVE_VEC;
						auto checkray = CollisionRay()(Movedpos, Vector3{ 0,1,0 }, Vector3{ 0,speed_ * DRAG_MOVE_SIZE,0 }, node, lpStage.GetALLStageCollList(), true).HitFlag;
						if (!checkray)
						{
							MoveMagVec_.x = 0;
						}

					}
				}
			}
		}
		else
		{
			//���Z�b�g
			state_ = State::Fall;
			MMdir_ = MagMouseDir::Non;

			OldMoveMagVec_  = { 0,0,0 };
			//MoveMagVec_	= { 0,0 ,0};

			DeltaDesignerStack_ = 0;
			olddistance_		= 0;
			
			MagDragflagLR_  = false;
			UpDragflag_		= false;
		}

	}
}

//�������㏸
void Player::MagnetWeakMove(void)
{
	if (state_ == State::MagnetWeak)
	{

		auto Wnode = MagnetWeakNode_->first_node("module");
		auto node  = stateNode_     ->first_node("module");
		//���C���������Ă�ԏ㏸����
		if (CollisionRay()(Moved_Pos_, { 0,-1,0 }, { 0, distance / 2 , 0 }, Wnode, lpStage.GetNorthStageCollList()).HitFlag ||
			CollisionRay()(Moved_Pos_, { 0,-1,0 }, { 0, distance / 2 , 0 }, Wnode, lpStage.GetSouthStageCollList()).HitFlag)		//���C
		{
			//�㏸���u���b�N���ђʂ��Ȃ��悤�ɏ�����Ƀ��C���΂�
			if (	!CollisionRay()(Moved_Pos_, -NowMagVec_, { speed_,speed_,0 } , node, lpStage.GetALLStageCollList(), true).HitFlag)		//re
			{
				//�㏸
				Playerbody_->applyCentralImpulse(-NowMagVec_.tobtVec() * MAGNET_WEAK_MOVE_POW);

			}
			if (Playerbody_->getInterpolationLinearVelocity().y() < 0)
			{
				if (!CollisionRay()(Moved_Pos_, -NowMagVec_, { 1    ,1      ,0 } , node, lpStage.GetALLStageCollList(), true).HitFlag)		//re
				{
					Playerbody_->applyCentralImpulse(-NowMagVec_.tobtVec() * MAGNET_WEAK_DOWN_POW);
				}
			}
		}
		else
		{
			//�������ĂȂ�
			//TRACE("�������ĂȂ�\n");
			//�㏸���u���b�N���ђʂ��Ȃ��悤�ɏ�����Ƀ��C���΂�
			Playerbody_->applyCentralImpulse(btVector3(-NowMagVec_.tobtVec().x()	,0								, 0) * MAGNET_WEAK_NOT_HIT_POW_X);
			//Playerbody_->applyCentralImpulse(-NowMagVec_.tobtVec() * 60);
			Playerbody_->applyCentralImpulse(btVector3(0							,-NowMagVec_.tobtVec().y()		, 0) * MAGNET_WEAK_NOT_HIT_POW_Y);
		}
	}
}

void Player::Move(void)
{
	//�������̂������㏸
	MagnetWeakMove();

	if (moveVecPow_ != Vector3{ 0,0,0 })
	{
		if ((abs(Playerbody_->getInterpolationLinearVelocity().x()) < MOVE_LIMIT))
		{
			/*Playerbody_->applyForce(moveVecPow_.tobtVec(), trans.getOrigin());*/
			Playerbody_->applyCentralImpulse(moveVecPow_.tobtVec());
		}
	}

	if (moveVecPowMag_ != Vector3{ 0,0,0 })
	{
		Playerbody_->setLinearVelocity((-moveVecPowMag_.tobtVec()) * MAGNET_MOVE_VEC_POW);
	}

	if (State::Fall == state_)
	{

		if (Playerbody_->getInterpolationLinearVelocity().y() > 0)
		{
			Playerbody_->applyForce(btVector3(0, -10, 0) * FALL_DOWN_POW, trans.getOrigin());
		}

		if (Playerbody_->getInterpolationLinearVelocity().x() > FALL_SPEED_LIMIT)
		{
			Playerbody_->applyForce(btVector3(-10, 0, 0) * FALL_DOWN_POW, trans.getOrigin());
		}
		if (Playerbody_->getInterpolationLinearVelocity().x() < -FALL_SPEED_LIMIT)
		{
			Playerbody_->applyForce(btVector3(10, 0, 0) * FALL_DOWN_POW,  trans.getOrigin());
		}
	}
	MV1SetPosition(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_], VGet(pos_.x, pos_.y, pos_.z));

	//�v���C���[�̌�������
	SetRotation();
}

//����
Vector3 Player::KeyCheckMove(void)
{
	//�ړ��ʂ̏�����
	moveVec_ = Vector3Zero;
	float Vec = 1;
	auto cntData = input_->GetCntData();
	auto snode  = SpecialHitNode_->first_node("module");
	auto Fnode  = FallNode_->first_node("module");

	if (cntData[InputID::Right][static_cast<int>(Trg::Now)])
	{
		moveVec_.x = Vec;
		if (CollisionRay()(Moved_Pos_, { 1,0,0 }, { KEY_MOVE_POW,0,0 }, Fnode, lpStage.GetALLStageCollList(), true).HitFlag)		//rei
		{
			movePow_ = SetSpeed(SpeedMode::SlowSlow);
		}
		
	}
	if (cntData[InputID::Left][static_cast<int>(Trg::Now)])
	{
		moveVec_.x = -Vec;
		if (CollisionRay()(Moved_Pos_, { -1,0,0 }, { KEY_MOVE_POW,0,0 }, Fnode, lpStage.GetALLStageCollList(), true).HitFlag)
		{
			movePow_ = SetSpeed(SpeedMode::SlowSlow);
		}
	}
	//�����x�N�g����Ԃ�
	return moveVec_;
}

//�ړ�
void Player::SetRunState(void)
{

	if (state_ != State::Magnet)
	{
		if (state_ != State::MagnetWeak)
		{
			if (state_ == State::Fall)
			{
				//��������
				auto Fnode = FallNode_->first_node("module");
				if (CollisionRay()(Moved_Pos_, { 0	,-1, 0 }, { 0,speed_,0 }, Fnode, lpStage.GetALLStageCollList(), true).HitFlag ||
					CollisionRay()(Moved_Pos_, { 1	,-1, 0 }, { 1,speed_,0 }, Fnode, lpStage.GetALLStageCollList(), true).HitFlag ||
					CollisionRay()(Moved_Pos_, {-1	,-1, 0 }, { 1,speed_,0 }, Fnode, lpStage.GetALLStageCollList(), true).HitFlag)		//rei
				{
					state_ = State::Run;
				}
			}
		}
	}

	if (state_ == State::Fall)
	{
		movePow_ = SetSpeed(SpeedMode::Slow);

	}
	if (state_ == State::Magnet)
	{
		movePow_ = SetSpeed(SpeedMode::Slow);
	}
	else if (state_ == State::MagnetWeak)
	{
		movePow_ = SetSpeed(SpeedMode::Slow);

	}
	else
	{
		movePow_ = SetSpeed(SpeedMode::Normal);
	}
	//�L�[���͂����邩
	if (KeyCheckMove() != Vector3{ 0,0 ,0 })
	{
		//�����x�N�g������
		oldmoveVec_ = moveVec_;
		moveVec_ = KeyCheckMove();
		//�����x�N�g���ɗ͂�������
		moveVecPow_ += moveVec_ * movePow_;
		auto node = MoveNode_->first_node("module");
		if (state_ != State::Magnet)
		{
			if (state_ != State::Fall)
			{
				animController_->Play((int)ANIM_TYPE::RUN);
				if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/walk_trim.mp3")[0]) == 0)
				{
					//spark_.playhandle = PlayEffekseer3DEffect(spark_.resource);
					PlaySoundMem(lpSoundMng.GetID("./source/Sound/walk_trim.mp3")[0], DX_PLAYTYPE_BACK);
				}

			}

		}
	}
	else
	{
		animController_->Play((int)ANIM_TYPE::IDLE);
	}
}
//���͐ؑ�
bool Player::SetInput(InputUnique input)
{
	if (input)
	{
		input_ = std::move(input);
		return true;
	}
	return false;
}

//�v���C���[���W
Vector3 Player::GetPos(void)
{
	return pos_;
}

Vector3 Player::ShakeGetPos(void)
{
	auto pos = pos_;
	if (state_ == State::DismemberDeath)
	{
		if (deathTime_ < DEATH_TIME_MAX)
		{
			if (amplitude_ > 0.0f)
			{
				pos.y += ((static_cast<int>(deathTime_) % DEATH_DELAY) - 1) * amplitude_;
			}
			amplitude_ *= AMPLITUDE_ADD;
			if (amplitude_ < 1.0f)
			{
				amplitude_ = 0.0f;
			}
			return pos;
		}
		
	}
	return pos;
}
//MagStick�̃��C
Line Player::GetLine()
{
	return magStick_->GetLine();
}
//����ł��邩
void Player::SetDeathState(void)
{
	//���ɍs�������̎��S����
	if (trans.getOrigin().y() < DEATH_POS_MAX_Y)
	{
		//if (logflag_)TRACE("death\n");
		state_ = State::DismemberDeath;
	}
	//���Z�b�g�{�^�����������Ƃ��̎��S
	auto cntData = input_->GetCntData();

	if (cntData[InputID::Reset][static_cast<int>(Trg::Now)])
	{
		state_ = State::DismemberDeath;
	}
}
void Player::SetCoinList(CoinList& list)
{
	coinList_ = list;
}

void Player::SetGimmickList(GimmickList& list)
{
	GimmickList_ = list;
}

State Player::GetState()
{
	return state_;
}

void Player::SetState(State state)
{
	state_ = state;
}

std::vector<btRigidBody*> Player::GetBreakmodel(void)
{
	return BreakPlayerbody_;
}

HitHandle Player::GetCoinList(void)
{
	return { coinList_,getCoinflg_ };
}

btRigidBody* Player::GetBody()
{
	return Playerbody_;
}

std::vector<VERTEX2DSHADER>& Player::GetTrailData(void)
{
	return magStick_->GetTrailData();
}

//���񂾎��̏���
bool Player::deathUpdate()
{
	if (state_ == State::Death)
	{
		//���Z�b�g
		ReInit();
	}

	if (state_ == State::DismemberDeath)
	{
		
		if (deathTime_ == 0)
		{
			
			lpSoundMng.PlayingSound("./source/Sound/���{�b�g����������3.mp3");
			magStick_->StopEffect();
			amplitude_ = AMPLITUDE_DEF;
			explosion_.playhandle = PlayEffekseer3DEffect(explosion_.resource);
			explosion_.pos = pos_;
			SetPosPlayingEffekseer3DEffect(explosion_.playhandle, explosion_.pos.x, explosion_.pos.y, explosion_.pos.z);
		}
		deathTime_++;
		btTransform transB[7];
		BreakPlayerbody_[0]->getMotionState()->getWorldTransform(transB[0]);
		BreakPlayerbody_[1]->getMotionState()->getWorldTransform(transB[1]);
		BreakPlayerbody_[2]->getMotionState()->getWorldTransform(transB[2]);
		BreakPlayerbody_[3]->getMotionState()->getWorldTransform(transB[3]);
		BreakPlayerbody_[4]->getMotionState()->getWorldTransform(transB[4]);
		BreakPlayerbody_[5]->getMotionState()->getWorldTransform(transB[5]);
		BreakPlayerbody_[6]->getMotionState()->getWorldTransform(transB[6]);

		if (deathTime_ < DEATH_TIME_LIMIT)
		{
			BreakPlayerbody_[0]->applyForce(btVector3{  DEATH_POW_X,	DEATH_POW_Y, DEATH_POW_Z }, transB[0].getOrigin());
			BreakPlayerbody_[1]->applyForce(btVector3{ -DEATH_POW_X2,	DEATH_POW_Y,-DEATH_POW_Z }, transB[1].getOrigin());
			BreakPlayerbody_[2]->applyForce(btVector3{  DEATH_POW_X2,	DEATH_POW_Y, DEATH_POW_Z }, transB[2].getOrigin());
			BreakPlayerbody_[3]->applyForce(btVector3{ -DEATH_POW_X2,	DEATH_POW_Y,-DEATH_POW_Z }, transB[3].getOrigin());
			BreakPlayerbody_[4]->applyForce(btVector3{  DEATH_POW_X2,	DEATH_POW_Y, 0 },			transB[4].getOrigin());
			BreakPlayerbody_[5]->applyForce(btVector3{  DEATH_POW_X,	DEATH_POW_Y, 0 },			transB[5].getOrigin());
			BreakPlayerbody_[6]->applyForce(btVector3{ -DEATH_POW_X,	DEATH_POW_Y,-DEATH_POW_Z2 },transB[6].getOrigin());
		}

		/*float aa = 700;
		BreakPlayerbody_[0]->applyForce(btVector3{ 1,aa,1 }, transB[0].getOrigin());
		BreakPlayerbody_[1]->applyForce(btVector3{ -1,aa,-1 }, transB[1].getOrigin());
		BreakPlayerbody_[2]->applyForce(btVector3{ 1,aa,1 }, transB[2].getOrigin());
		BreakPlayerbody_[3]->applyForce(btVector3{ -1,aa,-1 }, transB[3].getOrigin());
		BreakPlayerbody_[4]->applyForce(btVector3{ 1,aa,0 }, transB[4].getOrigin());
		BreakPlayerbody_[5]->applyForce(btVector3{ 1,aa,0 }, transB[5].getOrigin());
		BreakPlayerbody_[6]->applyForce(btVector3{ -1,aa,-5 }, transB[6].getOrigin());*/
		if (deathTime_ < DEATH_TIME_LIMIT_2)
		{
			BreakPlayerbody_[0]->applyCentralImpulse(btVector3{ -DEATH_POW_X,	DEATH_POW_Y2,-DEATH_POW_Z2 });
			BreakPlayerbody_[1]->applyCentralImpulse(btVector3{ -DEATH_POW_X2,	DEATH_POW_Y2, DEATH_POW_Z2 });
			BreakPlayerbody_[2]->applyCentralImpulse(btVector3{  DEATH_POW_X2,	DEATH_POW_Y2,-DEATH_POW_Z2 });
			BreakPlayerbody_[3]->applyCentralImpulse(btVector3{ -DEATH_POW_X2,	DEATH_POW_Y2, DEATH_POW_Z2 });
			BreakPlayerbody_[4]->applyCentralImpulse(btVector3{  DEATH_POW_X,	DEATH_POW_Y2,-DEATH_POW_Z2 });
			BreakPlayerbody_[5]->applyCentralImpulse(btVector3{  DEATH_POW_X,	DEATH_POW_Y2,-DEATH_POW_Z2 });
			BreakPlayerbody_[6]->applyCentralImpulse(btVector3{ -DEATH_POW_X,	DEATH_POW_Y2, DEATH_POW_Z2 });
		}
		if (deathTime_ < DEATH_TIME_LIMIT_3)
		{
			WaitTimer(DEATH_WAIT_TIME);
		}

		// �p�x�擾
		btMatrix3x3 rot;
		btVector3 euler;
		//MV1SetRotationXYZ(Model_Handle, btRotVGet(euler));
		for (int i = 0; i < CreateMax; i++)
		{
			rot = transB[i].getBasis();
			rot.getEulerZYX(euler[2], euler[1], euler[0]);
			MV1SetRotationXYZ(BreakmodelH_[i],  VGet(static_cast<float>(euler.x()),static_cast<float>(euler.y()), static_cast<float>(euler.z())));
			MV1SetPosition(BreakmodelH_[i],		VGet(static_cast<float>(transB[i].getOrigin().x()), static_cast<float>(transB[i].getOrigin().y()), static_cast<float>(transB[i].getOrigin().z())));
		}
		if (deathTime_ > DEATH_TIME_LIMIT_MAX)
		{
			state_ = State::Death;
		}
		return true;
	}
	return  false;
}
//�v���C���[�̌�������
void Player::SetRotation()
{
	//�ړ��������쐬
	auto sdpos = moveVecPow_ - moveVecPowMag_;
	//�ړ�����������
	if (sdpos.x < 0)
	{
		auto radY = Deg2RadF(-PLAYER_RAD);
		MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_], { 0, radY, 0 });
		SetLightDirectionHandle(PointLightHandle_, POINTLIGHT_VEC0);
		//SetLightDirectionHandle(PointLightHandle2_, { -1,-0.3,0 });
		trail_.angle = Deg2RadF(EFFECT_TRAIL_ANGLE_R);

		auto poss = pos_ + Vector3{ POINTLIGHT_POS0.x,POINTLIGHT_POS0.y,POINTLIGHT_POS0.z };
		SetLightPositionHandle(PointLightHandle_, poss.toVECTOR());
		//SetLightPositionHandle(PointLightHandle2_, poss.toVECTOR());

	}
	else if (sdpos.x > 0)
	{
		auto radY = Deg2RadF(PLAYER_RAD);
		MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/nonarm.mv1")[ModelNum_], { 0, radY, 0 });
		SetLightDirectionHandle(PointLightHandle_, POINTLIGHT_VEC1);
		//SetLightDirectionHandle(PointLightHandle2_, { 1,-0.3,0 });
		auto poss = pos_ + Vector3{ POINTLIGHT_POS1.x,POINTLIGHT_POS1.y,POINTLIGHT_POS1.z };
		SetLightPositionHandle(PointLightHandle_, poss.toVECTOR());
		//SetLightPositionHandle(PointLightHandle2_, poss.toVECTOR());
		trail_.angle = Deg2RadF(EFFECT_TRAIL_ANGLE_L);
	}

}

void Player::UpdateEffect(void)
{
	trail_.pos = pos_;
	SetPosPlayingEffekseer3DEffect(trail_.playhandle, trail_.pos.x, trail_.pos.y, trail_.pos.z);
}

void Player::ColCheckMidPoint(void)
{
	int max = 2;
	for (int m = 0; m < max; m++) 
	{
		//
		if ((pos_.x - size_.x) <= (midPos_[m].x + midSize_.x) &&
			(pos_.x + size_.x) >= (midPos_[m].x - midSize_.x) &&
			(pos_.y - size_.y) <= (midPos_[m].y + midSize_.y) &&
			(pos_.y + size_.y) >= (midPos_[m].y - midSize_.y))
		{
			if (m == 0)
			{
				resPoint_ = RespawnPoint::First;
			}
			if (m == 1)
			{
				resPoint_ = RespawnPoint::Second;
			}
		}
	}
}

void Player::SetRespawnPoint(void)
{
	int i = 0;
	int m = 0;
	for (auto& mid : lpStage.GetMidPointList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		midPos_[m] = mid.pos_;
		m++;
	}
	if (resPoint_ == RespawnPoint::First)
	{
		pos_ = { midPos_[0].x,midPos_[0].y + 200,0 };
	}
	if (resPoint_ == RespawnPoint::Second)
	{
		pos_ = { midPos_[1].x,midPos_[1].y + 200,0 };
	}
}


//�ړ������Ɨ͂�Ԃ�
Vector3 Player::GetMoveVec(void)
{
	return moveVecPow_;
}
//�J�v�Z���Ƃ̂����蔻��
void Player::HitCapsule(const EneymyList& enemy)
{
	HitCapsuleAndStage();
	HitCapsuleAndCoin();
	HitCapsuleAndPressurePlate();
	HitCapsuleAndEnemy();
	HitCapsuleAndGoal();
	HitCapsuleAndGear();
	HitCapsuleAndBullet(enemy);
}
//�J�v�Z���ƃ}�b�v
bool Player::HitCapsuleAndStage()
{
	bool hitFlag = false;
	pos_ = Moved_Pos_;
	//�J�v�Z�����ړ�������
	capsule_->SetRelativePosDown(VGet(pos_.x, pos_.y + CapDown, pos_.z));
	capsule_->SetRelativePosTop(VGet(pos_.x, pos_.y + CapTop + CAPSULE_OFFSET, pos_.z));
	//�J�v�Z���Ƃ̏Փ˔���
	for (auto& Stagelist : lpStage.GetALLStageCollList())
	{
		for (auto StageCollId : Stagelist)
		{
			auto hits = MV1CollCheck_Capsule(
				StageCollId,
				-1,
				capsule_->GetPosTop(),
				capsule_->GetPosDown(),
				capsule_->GetRadius());
			for (int i = 0; i < hits.HitNum; i++)
			{
				auto hit = hits.Dim[i];
				//�|���S���Ƃ̕�����`�F�b�N
				for (int tryCnt = 0; tryCnt < 10; tryCnt++)
				{
					hitFlag = HitCheck_Capsule_Triangle(
						capsule_->GetPosTop(),
						capsule_->GetPosDown(),
						capsule_->GetRadius(),
						hit.Position[0],
						hit.Position[1],
						hit.Position[2]
					);
					if (hitFlag)
					{
						//�߂荞��ł���␮
						Moved_Pos_.x = VAdd(Moved_Pos_.toVECTOR(), VScale(hit.Normal, 1.0f)).x;
						//Moved_Pos_.y = VAdd(Moved_Pos_.toVECTOR(), VScale(hit.Normal, 1.0f)).y;
						//�v���C���[�ɓK��
						pos_ = Moved_Pos_;
						//�J�v�Z���̍X�V
						capsule_->SetRelativePosDown(VGet(pos_.x, pos_.y + CapDown, pos_.z));
						capsule_->SetRelativePosTop(VGet(pos_.x, pos_.y + CapTop+ CAPSULE_OFFSET, pos_.z));
						continue;
					}
					break;
				}
			}
			//�Փ˔���p�̓��I�\���̂̃������[���
			MV1CollResultPolyDimTerminate(hits);
		}
		

	}
	return hitFlag;
}
//�J�v�Z���ƃR�C��
bool Player::HitCapsuleAndCoin(void)
{
	bool hitFlag = false;
	for (auto& coin_ : coinList_)
	{
		auto num = coin_->GetCoinNumber();
		auto hits = MV1CollCheck_Capsule(
			lpModelMng.GetID("./source/model/Coin/Coin.mv1")[coin_->GetCoinNumber()],
			-1,
			capsule_->GetPosTop(),
			capsule_->GetPosDown(),
			capsule_->GetRadius());

		if (hits.HitNum > 0)
		{
			coin_->SetCoinfFlg(true);
		}

		//�Փ˔���p�̓��I�\���̂̃������[���
		MV1CollResultPolyDimTerminate(hits);
	}
	return false;
}
//�J�v�Z���ƃ{�^��
bool Player::HitCapsuleAndPressurePlate(void)
{
	bool hitFlag = false;

	pos_ = Moved_Pos_;
	//�J�v�Z�����ړ�������
	capsule_->SetRelativePosDown(VGet(pos_.x, pos_.y + CapDown, pos_.z));
	capsule_->SetRelativePosTop(VGet(pos_.x, pos_.y + CapTop + CAPSULE_OFFSET, pos_.z));
	for (auto& gimmick : GimmickList_)
	{
		if (gimmick->GetObjType() != objType::PressurePlate)
		{
			continue;
		}
		auto hits = MV1CollCheck_Capsule(
			lpModelMng.GetID("./source/Model/Gimmick/Button.mv1")[gimmick->GetModelH()],
			-1,
			capsule_->GetPosTop(),
			capsule_->GetPosDown(),
			capsule_->GetRadius());
		if (hits.HitNum > 0)
		{
			gimmick->SetFlg(true);
		}
		else
		{
			gimmick->SetFlg(false);
		}
		MV1CollResultPolyDimTerminate(hits);
	}
	return false;
}
//�J�v�Z���ƓG
bool Player::HitCapsuleAndEnemy(void)
{
	bool hitFlag = false;
	for (auto& Id : unitlist_)
	{
		std::string name = Id->GetHandlePath();
		if (Id->GetEnemyType() != EnemyType::WALK)
		{
			continue;
		}
		auto EnemyId = dynamic_cast<Enemy*>(Id.get());

		auto hits = MV1CollCheck_Capsule(
			lpModelMng.GetID(EnemyId->GetHandlePath())[EnemyId->GetNumber()],
			-1,
			capsule_->GetPosTop(),
			capsule_->GetPosDown(),
			capsule_->GetRadius());

		if (hits.HitNum > 0)
		{
			//TRACE("�G�ɓ�������\n");
			WaitTimer(120);
			state_ = State::DismemberDeath;
		}
		//�Փ˔���p�̓��I�\���̂̃������[���
		MV1CollResultPolyDimTerminate(hits);
	}
	return false;
}
//�S�[���ƃJ�v�Z��
bool Player::HitCapsuleAndGoal()
{
	bool hitFlag = false;
	for (auto& GoalId :lpStage.GetGoalCollList())
	{
		auto hits = MV1CollCheck_Capsule(
			GoalId.first,
			-1,
			capsule_->GetPosTop(),
			capsule_->GetPosDown(),
			capsule_->GetRadius());

		if (hits.HitNum > 0)
		{
			state_ = State::Goal;
		}
		//�Փ˔���p�̓��I�\���̂̃������[���
		MV1CollResultPolyDimTerminate(hits);
	}
	return false;
}

bool Player::HitCapsuleAndGear(void)
{
	for (auto& gimmick : GimmickList_)
	{
		if (gimmick->GetObjType() != objType::Gear)
		{
			continue;
		}
		auto hits = MV1CollCheck_Capsule(
			lpModelMng.GetID("./source/Model/Gimmick/Gear.mv1")[gimmick->GetModelH()],
			-1,
			capsule_->GetPosTop(),
			capsule_->GetPosDown(),
			capsule_->GetRadius());
		if (hits.HitNum > 0)
		{
			//TRACE("�G�ɓ�������\n");
			state_ = State::DismemberDeath;
		}
		MV1CollResultPolyDimTerminate(hits);
	}
	return false;
}

void Player::HitCapsuleAndBullet(const EneymyList& enemy)
{
	for (auto& e : enemy)
	{
		if (e->GetEnemyType() != EnemyType::SHOOT)
		{
			continue;
		}

		auto shoot = dynamic_cast<EnemyShoot*>(e.get());
		auto bullet = shoot->GetBulletData();
		for (const auto& b : bullet->GetBulletVector())
		{
			if (!b->GetAliveFlag())
			{
				continue;
			}
			auto hits = MV1CollCheck_Capsule(
				lpModelMng.GetID("./source/Model/Enemy/bullet.mv1")[b->GetNumber()],
				-1,
				capsule_->GetPosTop(),
				capsule_->GetPosDown(),
				capsule_->GetRadius());

			if (hits.HitNum > 0)
			{
				state_ = State::DismemberDeath;
			}
			//�Փ˔���p�̓��I�\���̂̃������[���
			MV1CollResultPolyDimTerminate(hits);
		}
	}
}

//�G�̃��X�g
void Player::SetUnitList(EneymyList& list)
{
	unitlist_ = list;
}
void Player::OpacityMagnet()
{
	auto Fnode = FallNode_->first_node("module");
	auto SPcollN = CollisionRay()(Moved_Pos_, { 0,1,0 }, { 0,speed_,0 }, Fnode, lpStage.GetNorthStageCollList(),HitHandle_);
	auto SPcollS = CollisionRay()(Moved_Pos_, { 0,1,0 }, { 0,speed_,0 }, Fnode, lpStage.GetSouthStageCollList(),HitHandle_);
	if (SPcollN.HitFlag)
	{
		//MV1SetMeshOpacityRate(HitHandle_,SPcollN.MaterialIndex, 0.5);
		MV1SetOpacityRate(HitHandle_, OPACITY_RATE);
	}
	else if (SPcollS.HitFlag)
	{
		//MV1SetMeshOpacityRate(HitHandle_,SPcollS.MaterialIndex, 0.5);
		MV1SetOpacityRate(HitHandle_, OPACITY_RATE);
	}
	else
	{
		MV1SetOpacityRate(HitHandle_, OPACITY_DEF_RATE);
	}
}
bool Player::GetisAlive(void)
{
	return aliveFlag_;
}
void Player::SetisAlive(bool isAlive)
{
	aliveFlag_ = isAlive;
}
//void Player::SetMagBoxList(MagBoxList& list)
//{
//	Boxlist_ = list;
//}
void Player::DrawShadow(float ShadowHeight, float ShadowSize)
{
	float SHADOW_HEIGHT = ShadowHeight;
	float SHADOW_SIZE = ShadowSize;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	//int ModelHandle;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�

	for (auto& Stagelist : lpStage.GetALLStageCollList())
	{
		for (auto StageCollId : Stagelist)
		{
			// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
			HitResDim = MV1CollCheck_Capsule(
				StageCollId,
				-1,
				VGet(pos_.x, pos_.y, pos_.z),
				VAdd(
					VGet(pos_.x, pos_.y, pos_.z),
					VGet(0.0f, -SHADOW_HEIGHT, 0.0f)
				),
				SHADOW_SIZE
			);

			// ���_�f�[�^�ŕω��������������Z�b�g
			Vertex[0].dif = GetColorU8(255, 255, 255, 255);
			Vertex[0].spc = GetColorU8(0, 0, 0, 0);
			Vertex[0].su = 0.0f;
			Vertex[0].sv = 0.0f;
			Vertex[1] = Vertex[0];
			Vertex[2] = Vertex[0];

			// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
			HitRes = HitResDim.Dim;
			for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
			{
				// �|���S���̍��W�͒n�ʃ|���S���̍��W
				Vertex[0].pos = HitRes->Position[0];
				Vertex[1].pos = HitRes->Position[1];
				Vertex[2].pos = HitRes->Position[2];

				// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
				SlideVec = VScale(HitRes->Normal, 0.5f);
				Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
				Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
				Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

				// �|���S���̕s�����x��ݒ肷��
				Vertex[0].dif.a = 0;
				Vertex[1].dif.a = 0;
				Vertex[2].dif.a = 0;
				if (HitRes->Position[0].y > pos_.y - SHADOW_HEIGHT)
					Vertex[0].dif.a = 255 * (1.0f - fabs(HitRes->Position[0].y - pos_.y) / SHADOW_HEIGHT);
				if (HitRes->Position[1].y > pos_.y - SHADOW_HEIGHT)
					Vertex[1].dif.a = 255 * (1.0f - fabs(HitRes->Position[1].y - pos_.y) / SHADOW_HEIGHT);
				if (HitRes->Position[2].y > pos_.y - SHADOW_HEIGHT)
					Vertex[2].dif.a = 255 * (1.0f - fabs(HitRes->Position[2].y - pos_.y) / SHADOW_HEIGHT);

				// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
				Vertex[0].u = (HitRes->Position[0].x - pos_.x) / (SHADOW_SIZE * 2.0f) + 0.5f;
				Vertex[0].v = (HitRes->Position[0].z - pos_.z) / (SHADOW_SIZE * 2.0f) + 0.5f;
				Vertex[1].u = (HitRes->Position[1].x - pos_.x) / (SHADOW_SIZE * 2.0f) + 0.5f;
				Vertex[1].v = (HitRes->Position[1].z - pos_.z) / (SHADOW_SIZE * 2.0f) + 0.5f;
				Vertex[2].u = (HitRes->Position[2].x - pos_.x) / (SHADOW_SIZE * 2.0f) + 0.5f;
				Vertex[2].v = (HitRes->Position[2].z - pos_.z) / (SHADOW_SIZE * 2.0f) + 0.5f;

				// �e�|���S����`��
				DrawPolygon3D(Vertex, 1, lpImageMng.GetID("source/image/Shadow.png")[0], TRUE);
			}

			// ���o�����n�ʃ|���S�����̌�n��
			MV1CollResultPolyDimTerminate(HitResDim);
		}
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);

}
