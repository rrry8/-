#include<DxLib.h>
#include<cmath>
#include "MoveCountDown.h"
#include"../Camera/Camera.h"
#include"../Obj/Player.h"
#include "../Other/ImageMng.h"
#include"../Other/SoundMng.h"
#include"../SceneManager.h"
#include"../Input/KeyInput.h"
#include"../Input/PadInput.h"

//�e�L�X�g�֌W
constexpr float TEXT_GOAL_POS_X = 530.0f;	//�X�e�[�W�I����̈ړ��ꏊx
constexpr float TEXT_GOAL_POS_Y = 150.0f;	//�X�e�[�W�I����̈ړ��ꏊy
constexpr float TEXT_MOVE_POWER = 2.0f;		//�ړ����x

//���ߗ�
constexpr double ALPHA_COEFFICIENT = 0.199218875;

//�_�Ŏ���
static constexpr double BLINK_LIMIT = 0.25;

//��]���
constexpr float WHEEL_ADD_ANGLE = 1.0f;

//���[���h�T�C�Y
constexpr float WORLD_SIZE_X = 3000.0f;
constexpr float WORLD_SIZE_Y = 720.0f;

MoveCountDown::MoveCountDown(double LimitTime, std::string stage, std::string nonstage,Vector2 pos, UniqueScene beforeScene, UniqueScene afterScene):
	TransitionScene(std::move(beforeScene), std::move(afterScene))
{
	camera_ = std::make_unique<Camera>(Vector2(WORLD_SIZE_X, WORLD_SIZE_Y));
	player_ = std::make_unique<Player>();
	if (GetJoypadNum() == 0)
	{
		//�L�[�{�[�h�̏ꍇ
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//�p�b�h�̏ꍇ
		controller_ = std::make_unique<PadInput>();
	}

	stagePath_ = stage;
	nonStagePath_ = nonstage;
	textPos_ = pos;
	blinkTime_ = 0.0;
	stepStopTime_ = 0.0;
	Vector2 goalPos = { TEXT_GOAL_POS_X,TEXT_GOAL_POS_Y };
	textVelocity_ = (goalPos - textPos_).Normalized() * TEXT_MOVE_POWER;

	DrawScreen();
}

MoveCountDown::~MoveCountDown()
{
}

bool MoveCountDown::UpdateTransition(double delta)
{
	controller_->Update();
	auto usedController = controller_->GetControllerData();

	auto carPos = player_->GetPos();
	const auto& offset = camera_->GetPos();
	count_++;
	camera_->Update(player_->GetPos());


	if (carPos.x != lpSceneMng.GetViewSize().x + 256.0f)
	{
		float moveCoefficient = 1.0f;
		if (usedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)])
		{
			moveCoefficient = 4.0f;
		}
		player_->MoveRight(camera_->GetPos(), moveCoefficient);
		carPos = player_->GetPos();
		wheelAngle_ += WHEEL_ADD_ANGLE;
	}
	else
	{
		stepStopTime_ += delta;
		if (stepStopTime_ >= 1.5)
		{
			return true;
		}
	}
	if (carPos.x >= lpSceneMng.GetViewSize().x + 256.0f)
	{
		player_->SetPos({ lpSceneMng.GetViewSize().x + 256.0f ,665.0f });
	}

	MoveText(textVelocity_.x > 0);

	//�t�H���g�_��
	blinkTime_ += lpSceneMng.GetDeltaTime();
	if (blinkTime_ > BLINK_LIMIT * 2.0)
	{
		blinkTime_ = 0.0;
	}

	return false;
}

void MoveCountDown::MoveText(bool flag)
{
	textPos_ += textVelocity_;
	//�e�L�X�g�ړ�
	if (flag)
	{
		if (textPos_.x > TEXT_GOAL_POS_X)
		{
			textPos_.x = TEXT_GOAL_POS_X;
		}
		if (textPos_.y < TEXT_GOAL_POS_Y)
		{
			textPos_.y = TEXT_GOAL_POS_Y;
		}
	}
	else
	{
		if (textPos_.x < TEXT_GOAL_POS_X)
		{
			textPos_.x = TEXT_GOAL_POS_X;
		}
		if (textPos_.y < TEXT_GOAL_POS_Y)
		{
			textPos_.y = TEXT_GOAL_POS_Y;
		}
	}
}

void MoveCountDown::DrawScreen()
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto offset = camera_->GetPos();
	DrawGraphF(-offset.x, -offset.y, lpImageMng.GetID("./Image/BGtransition.png")[0], true);
	auto carPos = player_->GetPos();
	auto alpha = static_cast<int>(carPos.x * ALPHA_COEFFICIENT * 2.0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforeScene_->DrawplusOffset(-offset);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
	afterScene_->DrawplusOffset({ lpSceneMng.GetViewSize().x - offset.x,-offset.y });

	//�v���C���[�̕`��
	DrawPlayer();

	//�e�L�X�g�̕`��A�F�t���͒���(����I�ɓ_��)
	if (blinkTime_ < BLINK_LIMIT)
	{
		DrawGraphF(textPos_.x, textPos_.y, lpImageMng.GetID(nonStagePath_)[0], true);
	}
	else if (blinkTime_ < BLINK_LIMIT * 2.0)
	{
		DrawGraphF(textPos_.x, textPos_.y, lpImageMng.GetID(stagePath_)[0], true);
	}
}

void MoveCountDown::DrawPlayer(void)
{
	auto offset = camera_->GetPos();
	auto carPos = player_->GetPos();
	//�E�^�C���̍��v���Z�l�w
	auto addWheelRX = NORMAL_OFFSET_WHEEL_RIGHT_X;
	//���^�C���̍��v���Z�l�w
	auto addWheelLX = NORMAL_OFFSET_WHEEL_LEFT_X;
	//�^�C���̍��v���Z�l�x
	auto addWheelY = NORMAL_OFFSET_WHEEL_Y;

	//���@
	//�E�̃^�C��
	DrawRotaGraphF(carPos.x + addWheelRX - offset.x, carPos.y + addWheelY - offset.y, 1.0, wheelAngle_, lpImageMng.GetID("././Image/Wheel.png")[0], true);
	//���̃^�C��
	DrawRotaGraphF(carPos.x + addWheelLX - offset.x, carPos.y + addWheelY - offset.y, 1.0, wheelAngle_, lpImageMng.GetID("././Image/Wheel.png")[0], true);
	//�ԑ�
	DrawRotaGraphF(carPos.x - offset.x, carPos.y - offset.y, 1.0, 0.0, lpImageMng.GetID("././Image/Car.png")[0], true);
}
