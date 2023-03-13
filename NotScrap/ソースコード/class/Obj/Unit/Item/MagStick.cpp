#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "MagStick.h"
#include"../../../../_debug/_DebugConOut.h"
#include"../Enemy.h"
#include"BulletManager.h"
#include"MagStickTrail.h"
#include "../../Gimmick/Gimmick.h"
#include "../../../Common/ImageMng.h"
#include "../EnemyShoot.h"

constexpr float ROD_LENGTH = 512.0f;	//�_�̍ő�̒���
constexpr float MOUSEICON_HALFSIZE = 32.0f;	//�}�E�X�A�C�R���̑傫���̔���
constexpr float ROD_OFFSET_Y = 100.0f;		//�v���C���[�̈ʒu�͑����Ȃ̂ŃI�t�Z�b�g�𑫂�
constexpr float ROD_MINI_OFFSET_Y = 8.0f;	//�`��͏����������炷
constexpr float ROD_OFFSET_Z = 40.0f;		//�v���C���[�̌����Ă�������Řr�̈ʒu��ς���
constexpr float ARM_SCALE = 0.35f;			//�v���C���[�̊g�嗦
constexpr float LINE_WIDTH = 30.0f;			//���C�����m�̉���
constexpr int LOWER_LIMIT = 200;			//�X���̍Œᐔ�l
constexpr float WITHIN_RANGE = 3000.0f;		//���肷��͈�
constexpr float CENTER_POS_X = 960.0f;	//���S�_��
constexpr float CENTER_POS_Y = 670.0f;	//���S�_��

MagStick::MagStick(Vector3 playerpos,CntType type,CntData cntdata, int num)
{
	num_ = num;
	line_.p = playerpos;
	cntType_ = type;
	cntData_ = cntdata;
	Init();
}

MagStick::~MagStick()
{
	StopEffect();
}

bool MagStick::Init(void)
{
	armPosz_ = -ROD_OFFSET_Z;
	angle_ = 0.0;
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MagStickmodule");
	state_ = MAGSTATE::None;
	playerState_ = State::Idle;
	playerMoveVec_ = {0,0,0};
	rodLength_ = ROD_LENGTH;

	//�G�t�F�N�g������
	northEffect_.resource = LoadEffekseerEffect("./source/effect/north01.efkefc",20.0f);
	southEffect_.resource = LoadEffekseerEffect("./source/effect/south01.efkefc",20.0f);
	noneEffect_.resource = LoadEffekseerEffect("./source/effect/none01.efkefc", 20.0f);
	Effekseer_InitDistortion();
	effectAngle_ = 0.0f;

	endPos_ = line_.p;
	line_.p.x += 1.0f;
	oldAddPos_ = Vector2(ROD_LENGTH, 0.0f);

	trail_ = std::make_unique<MagStickTrail>();
	MV1SetPosition(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], (pos_.toVECTOR()));
	MV1SetScale(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], { ARM_SCALE,ARM_SCALE,ARM_SCALE });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], (pos_.toVECTOR()));
	MV1SetScale(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], { 30,30,30 });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], (pos_.toVECTOR()));
	MV1SetScale(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], { 40,100,100 });
	MV1SetOpacityRate(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 0.5);
	return false;
}

void MagStick::Update()
{
}

void MagStick::Update(GimmickList& gimmicklist,MagBoxList& boxList, Vector3 pos, Vector3 dir,State state,const CntData& cntdata, const EneymyList& enemy)
{
	//���΂̔����擾
	boxList_ = boxList;

	//�v���C���[�̈ʒu�ɖ_�̎n�_
	line_.p = pos;
	line_.p.y += ROD_OFFSET_Y;	//�I�t�Z�b�g�𑫂�

	//�v���C���[�̌����Ă�������ɂ���Ęr���ړ�������
	if (dir.x > 0.0f)
	{
		armPosz_ = -ROD_OFFSET_Z;
	}
	else if (dir.x < 0.0f)
	{
		armPosz_ = ROD_OFFSET_Z;
	}

	//��Ԏ擾
	playerState_ = state;

	//�R���g���[���f�[�^�X�V
	cntData_ = cntdata;

	state_ = MAGSTATE::None;
	if (cntData_[InputID::BtnN][static_cast<int>(Trg::Now)])
	{
		state_ = MAGSTATE::North;
		MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 1, lpImageMng.GetID("N")[0], false);
	}
	else if (cntData_[InputID::BtnS][static_cast<int>(Trg::Now)])
	{
		state_ = MAGSTATE::South;
		MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 1, lpImageMng.GetID("S")[0], false);
	}
	else
	{
		MV1SetTextureGraphHandle(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], 1, lpImageMng.GetID("NONtex")[0], false);
	}

	//�G�t�F�N�g�̍Đ�
	PlayEffect();

	//���C���̏��
	SetLineInfo();

	SlowEnemyBullet(enemy);

	//�p�b�h�̏ꍇ�͖_�̐�[���}�E�X�̈ʒu�ɂ���
	if (cntType_ == CntType::Pad)
	{
		SetMouseIconPosition();
	}
	else
	{
		endPos_ = GetCurrentMosusePosition();
	}
	
	//�ʒu�Ɖ�]���X�V
	MV1SetPosition(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], VGet(line_.p.x, line_.p.y + ROD_MINI_OFFSET_Y, line_.p.z + armPosz_));
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_], { 0, Deg2RadF(-90.0f), (float)angle_ });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], VGet(line_.p.x, line_.p.y + ROD_MINI_OFFSET_Y, line_.p.z + armPosz_ + 20));
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_], { 0, 0, (float)angle_ });

	MV1SetPosition(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], VGet(line_.p.x, line_.p.y + ROD_MINI_OFFSET_Y, line_.p.z + armPosz_ + 35));
	MV1SetRotationXYZ(lpModelMng.GetID("./source/model/Player/Stick.mv1")[num_], { 0, 0, (float)angle_ });

	//�e�I�u�W�F�N�g�Ƃ̔���
	CollMagnet();
	CollMagnetBox(gimmicklist);
	CollAccessory(gimmicklist);
	CollEnemy(enemy);

	//�O�Ղ̍X�V
	trail_->Update(line_);
}

void MagStick::Draw(void)
{
	//�v���C���[������ł���Ƃ��A�`������Ȃ�
	if (playerState_ == State::Death || playerState_ == State::DismemberDeath)
	{
		return;
	}

	MV1DrawModel(lpModelMng.GetID("./source/model/Player/Chocolate-Stick.mv1")[num_]);
	MV1DrawModel(lpModelMng.GetID("./source/model/Player/armonly_new.mv1")[num_]);

	//�O��
	trail_->Draw();
}

void MagStick::Release(void)
{
	DeleteEffekseerEffect(northEffect_.resource);
	DeleteEffekseerEffect(southEffect_.resource);
}

Vector3 MagStick::GetPlayerMoveVec(void)
{
	return playerMoveVec_;
}

Line MagStick::GetLine(void)
{
	return line_;
}

Vector3 MagStick::GetMagBoxVec(void)
{
	return magBoxVec_;
}

Vector3 MagStick::GetCurrentMosusePosition(void)
{
	VECTOR endPos;
	VECTOR screenPos;
	int mx, my;
	GetMousePoint(&mx, &my);
	auto screenLinePos =ConvertVECTORtoVector3(ConvWorldPosToScreenPos(line_.p.toVECTOR()));
	screenPos.x = (float)mx + MOUSEICON_HALFSIZE;
	screenPos.y = (float)my + MOUSEICON_HALFSIZE;
	screenPos.z = 1.0f;

	endPos = ConvScreenPosToWorldPos(screenPos);
	endPos.z = 0.0f;
	return  ConvertVECTORtoVector3(endPos);
}

void MagStick::CollMagnet(void)
{
	playerMoveVec_ = { 0, 0 ,0 };
	auto node = stateNode_->first_node("module");

	Vector3 centerPos;
	centerPos = ConvertVECTORtoVector3(ConvScreenPosToWorldPos(VGet(trail_->GetCenterPos().x, trail_->GetCenterPos().y, 1.0f)));

	//z���͎g��Ȃ��̂Ń[���N���A
	centerPos.z = 0.0f;


	effectPos_ = line_.p;
	//��[����v���C���[�ւ̃x�N�g��
	Vector3 endToCenterVec = centerPos - endPos_;
	effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);

	//s��n�̎��΂ɏՓ˂��Ă���ꍇ�A���̃x�N�g���������擾
	if (CollisionRay()(line_, node, lpStage.GetSouthStageCollList()).HitFlag ||
		CollisionRay()(rightLine_, node, lpStage.GetSouthStageCollList()).HitFlag ||
		CollisionRay()(leftLine_, node, lpStage.GetSouthStageCollList()).HitFlag)
	{
		if (state_ == MAGSTATE::North)
		{
			//�����񂹂���
			effectPos_ = line_.p;
			endToCenterVec = centerPos - endPos_;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = -normalVec_;
		}
		else if (state_ == MAGSTATE::South)
		{
			//��������
			effectPos_ = line_.end;
			endToCenterVec = endPos_ - centerPos;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = normalVec_;
		}
		else if (state_ == MAGSTATE::None)
		{
			if (IsEffekseer3DEffectPlaying(noneEffect_.playhandle) == -1)
			{
				noneEffect_.playhandle = PlayEffekseer3DEffect(noneEffect_.resource);
			}
		}
	}
	else if (CollisionRay()(line_, node, lpStage.GetNorthStageCollList()).HitFlag ||
		CollisionRay()(rightLine_, node, lpStage.GetNorthStageCollList()).HitFlag ||
		CollisionRay()(leftLine_, node, lpStage.GetNorthStageCollList()).HitFlag)
	{

		if (state_ == MAGSTATE::North)
		{
			//��������
			effectPos_ = line_.end;
			endToCenterVec = endPos_ - centerPos;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = normalVec_;
		}
		else if (state_ == MAGSTATE::South)
		{
			//�����񂹂���
			effectPos_ = line_.p;
			endToCenterVec = centerPos - endPos_;
			effectAngle_ = atan2(endToCenterVec.y, endToCenterVec.x);
			playerMoveVec_ = -normalVec_;
		}
		else if (state_ == MAGSTATE::None)
		{
			if (IsEffekseer3DEffectPlaying(noneEffect_.playhandle) == -1)
			{
				noneEffect_.playhandle = PlayEffekseer3DEffect(noneEffect_.resource);
			}
		}
	}
	else
	{
		if (IsEffekseer3DEffectPlaying(noneEffect_.playhandle) == 0)
		{
			StopEffekseer3DEffect(noneEffect_.playhandle);
		}
	}
}

void MagStick::CollMagnetBox(GimmickList& gimmicklist)
{
	auto node = stateNode_->first_node("module");
	for (auto& magbox : gimmicklist)
	{
		if (!CheckWithinRange(line_.p, magbox->GetPos()))
		{
			continue;
		}
		int jyougenn = 500;
		if (magbox->GetObjType() == objType::NMagBox)
		{
			
			if (CollisionRay()(line_, node, lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(rightLine_, node, lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(leftLine_, node, lpModelMng.GetID("./source/Model/Magnet/N_Box.mv1")[magbox->GetModelH()]).HitFlag)
			{
				btTransform trans;
				magbox->GetRigidBody()->getMotionState()->getWorldTransform(trans);
				normalVec_.y = 0;
				if (state_ == MAGSTATE::North)
				{

					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						magbox->GetRigidBody()->applyCentralForce(normalVec_.tobtVec()*50000000);

					}
				}
				else if (state_ == MAGSTATE::South)
				{
					
					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						magbox->GetRigidBody()->applyCentralForce(-normalVec_.tobtVec()*50000000);
					}
				}
			}
			else
			{
				magbox->SetMoveVec(Vector3Zero);
			}
		}

		if (magbox->GetObjType() == objType::SMagBox)
		{
			if (CollisionRay()(line_, node, lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(rightLine_, node, lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[magbox->GetModelH()]).HitFlag ||
				CollisionRay()(leftLine_, node, lpModelMng.GetID("./source/Model/Magnet/S_Box.mv1")[magbox->GetModelH()]).HitFlag)
			{
				btTransform trans;
				magbox->GetRigidBody()->getMotionState()->getWorldTransform(trans);
				normalVec_.y = 0;
				if (state_ == MAGSTATE::North)
				{

					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						
						magbox->GetRigidBody()->applyCentralForce(-normalVec_.tobtVec() * 50000000);
					}
				}
				else if (state_ == MAGSTATE::South)
				{

					if ((abs(magbox->GetRigidBody()->getInterpolationLinearVelocity().x()) < jyougenn))
					{
						magbox->GetRigidBody()->applyCentralForce(normalVec_.tobtVec() * 50000000);
					}
				}
			}
		}
	}
}

void MagStick::CollAccessory(GimmickList& gimmicklist)
{
	auto node = stateNode_->first_node("module");
	for (auto& acs : gimmicklist)
	{
		if (!CheckWithinRange(line_.p, acs->GetPos()))
		{
			continue;
		}
		if (acs->GetObjType() == objType::Accessory)
		{
			auto lineAcs = line_;
			auto rightlineAcs = rightLine_;
			auto leftlineAcs = leftLine_;
			lineAcs.p.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			rightlineAcs.p.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			leftlineAcs.p.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			
			lineAcs.end.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			rightlineAcs.end.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;
			leftlineAcs.end.z += MV1GetPosition(lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).z;

			if (CollisionRay()(lineAcs, node, lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).HitFlag ||
				CollisionRay()(rightlineAcs, node, lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).HitFlag ||
				CollisionRay()(leftlineAcs, node, lpModelMng.GetID("./source/Model/Stage/Barrel2.mv1")[acs->GetModelH()]).HitFlag)
			{
				if (state_ == MAGSTATE::North)
				{
					acs->GetRigidBody()->applyCentralImpulse(normalVec_.tobtVec() * 15);

				}
				else if (state_ == MAGSTATE::South)
				{
					acs->GetRigidBody()->applyCentralImpulse(-normalVec_.tobtVec() * 15);

				}
			}
		}
	}
}

void MagStick::CollEnemy(const EneymyList& enemy)
{
	auto node = stateNode_->first_node("module");
	for (auto &unit : enemy)
	{
		if (unit->GetEnemyType() != EnemyType::WALK)
		{
			continue;
		}
		auto enemy = dynamic_cast<Enemy*>(unit.get());
		enemy->SetRodLine(line_);

		//�G�̃n���h���擾
		auto name = enemy->GetHandlePath();
		auto id = enemy->GetNumber();
		//���΂��L���ŁA�_�ɂ������Ă����ꍇ�A�G�̏�Ԃ𒣂�t���ɂ���
		if (CollisionRay()(line_,	   node, name,id).HitFlag ||
			CollisionRay()(leftLine_,  node, name,id).HitFlag ||
			CollisionRay()(rightLine_, node, name,id).HitFlag)
		{
			if (state_ != MAGSTATE::None)
			{
				if (enemy->GetState() != State::Stick)
				{
					enemy->SetState(State::Stick);
					auto enemyToplayerVec = enemy->GetPosition() - line_.p;
					rodLength_ = enemyToplayerVec.Magnitude();

					//�����_�̍ő�͈͂𒴂��悤�Ƃ��Ă����ꍇ�͗}����
					if (rodLength_ > ROD_LENGTH)
					{
						rodLength_ = ROD_LENGTH;
					}
				}
				continue;
			}
		}
		//����t����ԂŎ��΂��ғ����Ă��Ȃ��ꍇ�͓G�̏�Ԃ𗎉��ɂ���
		if (enemy->GetState() == State::Stick)
		{
			if (state_ == MAGSTATE::None)
			{
				rodLength_ = ROD_LENGTH;
				enemy->SetState(State::Run);
			}
		}
		else
		{
			rodLength_ = ROD_LENGTH;
		}
	}
}

void MagStick::SetLineInfo(void)
{
	Vector3 centerPos = ConvertVECTORtoVector3(ConvScreenPosToWorldPos(VGet(CENTER_POS_X, CENTER_POS_Y, 1.0f)));
	//z���͎g��Ȃ��̂Ń[���N���A
	centerPos.z = 0.0f;
	auto d = endPos_ - centerPos;

	//�}�E�X�̈ʒu�ƃv���C���[�̈ʒu���ꏏ�������ꍇ�A�������Œ肳����
	if (d == Vector3{ 0.0f,0.0f,0.0f })
	{
		d = Vector3{ 1.0f,1.0f,0.0f };
	}

	//�p�x�X�V
	angle_ = atan2(-d.y, -d.x);

	//���K��
	normalVec_ = d.Normalized();

	//�^�񒆂̐����̈ʒu�X�V
	line_.end = line_.p + normalVec_ * rodLength_;
	line_.end.z = 0.0f;

	//�_�̕����x�N�g�����擾
	auto q = atan2(line_.vec().y, line_.vec().x);

	//90�x
	auto deg90 = 90.0f * (DX_PI_F / 180.0f);

	//�E�̐����̈ʒu�X�V
	float addx = cos(q + deg90) * LINE_WIDTH;
	float addy = sin(q + deg90) * LINE_WIDTH;
	pos_.x = line_.p.x + addx;
	pos_.y = line_.p.y + addy;
	Vector3 endpos = Vector3{ line_.end.x + addx,line_.end.y + addy,0 };
	rightLine_.p = pos_;
	rightLine_.end = endpos;

	//���̐����̈ʒu�X�V
	addx = cos(q - deg90) * LINE_WIDTH;
	addy = sin(q - deg90) * LINE_WIDTH;
	pos_.x = line_.p.x + addx;
	pos_.y = line_.p.y + addy;
	endpos = Vector3{ line_.end.x + addx,line_.end.y + addy,0 };
	leftLine_.p = pos_;
	leftLine_.end = endpos;
}

void MagStick::PlayEffect(void)
{
	//���΂̏�ԂŃG�t�F�N�g�ύX
	switch (state_)
	{
	case MAGSTATE::None:
		if (!IsEffekseer3DEffectPlaying(northEffect_.playhandle))
		{
			StopEffekseer3DEffect(northEffect_.playhandle);
		}
		if (!IsEffekseer3DEffectPlaying(southEffect_.playhandle))
		{
			StopEffekseer3DEffect(southEffect_.playhandle);
		}
		SetPosPlayingEffekseer3DEffect(noneEffect_.playhandle, effectPos_.x, effectPos_.y, effectPos_.z - 40.0f);
		SetRotationPlayingEffekseer3DEffect(noneEffect_.playhandle, 0, 0, effectAngle_);
		break;
	case MAGSTATE::South:
	{
		if (IsEffekseer3DEffectPlaying(southEffect_.playhandle))
		{
			if (!IsEffekseer3DEffectPlaying(northEffect_.playhandle))
			{
				StopEffekseer3DEffect(northEffect_.playhandle);
			}
			southEffect_.playhandle = PlayEffekseer3DEffect(southEffect_.resource);
		}
		SetPosPlayingEffekseer3DEffect(southEffect_.playhandle, effectPos_.x, effectPos_.y, effectPos_.z - 40.0f);
		SetRotationPlayingEffekseer3DEffect(southEffect_.playhandle, 0, 0,effectAngle_);
		break;
	}
	case MAGSTATE::North:
		if (IsEffekseer3DEffectPlaying(northEffect_.playhandle))
		{
			if (!IsEffekseer3DEffectPlaying(southEffect_.playhandle))
			{
				StopEffekseer3DEffect(southEffect_.playhandle);
			}
			northEffect_.playhandle = PlayEffekseer3DEffect(northEffect_.resource);
		}
		SetPosPlayingEffekseer3DEffect(northEffect_.playhandle, effectPos_.x, effectPos_.y, effectPos_.z - 40.0f);
		SetRotationPlayingEffekseer3DEffect(northEffect_.playhandle, 0, 0, effectAngle_);
		break;
	default:
		break;
	}
}

bool MagStick::CheckWithinRange(Vector3 playerpos, Vector3 gimmickpos)
{
	if (playerpos.x - WITHIN_RANGE < gimmickpos.x && gimmickpos.x < playerpos.x + WITHIN_RANGE &&
		playerpos.y - WITHIN_RANGE < gimmickpos.y && gimmickpos.y < playerpos.y + WITHIN_RANGE)
	{
		return true;
	}
	return false;
}

void MagStick::SlowEnemyBullet(const EneymyList& enemy)
{
	//�e������������
	for (auto& e : enemy)
	{
		//�e�����G�̃f�[�^�ȊO��continue
		if (e->GetEnemyType() != EnemyType::SHOOT)
		{
			continue;
		}

		auto shoot = dynamic_cast<EnemyShoot*>(e.get());
		auto bullet = shoot->GetBulletData();
		if (state_ != MAGSTATE::None)
		{
			bullet->CollisionRodAndBullet(line_, rightLine_, leftLine_);
		}
	}
}

void MagStick::SetMouseIconPosition(void)
{
	//PAD�ڑ���
	DINPUT_JOYSTATE rStick_Input;
	//�E�X�e�B�b�N�̓��͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);
	if ((rStick_Input.Rx != 0 || rStick_Input.Ry != 0) &&
		(abs(rStick_Input.Rx) >= LOWER_LIMIT || abs(rStick_Input.Ry) >= LOWER_LIMIT))
	{
		Vector2 vec = Vector2{ static_cast<float>(rStick_Input.Rx),static_cast<float>(rStick_Input.Ry) };
		vec.Normalize();
		Vector2 addPos = vec * ROD_LENGTH;
		auto screenPos = ConvWorldPosToScreenPos(VGet(line_.p.x, line_.p.y, line_.p.z));
		auto totalWorldPos = ConvScreenPosToWorldPos(VGet(screenPos.x + addPos.x, screenPos.y + addPos.y, screenPos.z));
		//�}�E�X�̈ʒu�X�V
		endPos_.x = totalWorldPos.x;
		endPos_.y = totalWorldPos.y;
		endPos_.z = 0.0f;

		oldAddPos_ = addPos;
	}
	else
	{
		//�X���Ă��Ȃ��ꍇ�A�O�t���[���̃v���C���[�̈ʒu���瑫��
		auto screenPos = ConvWorldPosToScreenPos(VGet(line_.p.x, line_.p.y, line_.p.z));
		auto totalWorldPos = ConvScreenPosToWorldPos(VGet(screenPos.x + oldAddPos_.x, screenPos.y + oldAddPos_.y, screenPos.z));
		endPos_.x = totalWorldPos.x;
		endPos_.y = totalWorldPos.y;
	}
}

void MagStick::StopEffect(void)
{
	if (!IsEffekseer3DEffectPlaying(southEffect_.playhandle))
	{
		StopEffekseer3DEffect(southEffect_.playhandle);
	}
	if (!IsEffekseer3DEffectPlaying(northEffect_.playhandle))
	{
		StopEffekseer3DEffect(northEffect_.playhandle);
	}
	if (!IsEffekseer3DEffectPlaying(noneEffect_.playhandle))
	{
		StopEffekseer3DEffect(noneEffect_.playhandle);
	}
}

std::vector<VERTEX2DSHADER>& MagStick::GetTrailData(void)
{
	return trail_->GetVertexData();
}
