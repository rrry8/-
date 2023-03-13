#include<DxLib.h>
#include "Drum.h"
#include"../Camera/Camera.h"
#include"../SceneManager.h"
#include"../../tmx/TmxObj.h"

constexpr float OFFSET_X = 32.0f;	//�I�t�Z�b�g��
constexpr float OFFSET_Y = 40.0f;	//�I�t�Z�b�g��
constexpr int ALPHA_FULL = 255;		//�_�Ŏ��̔Z�����l
constexpr int ALPHA_HALF = 100;		//�_�Ŏ��̔������l
constexpr double BLINK_TIME = 0.05;	//�_�Ŏ���

constexpr float INIT_ACCEL = -10.0f;	//�����x(��������Y�̐��̕����̂��ߏ�����̓}�C�i�X�l)

constexpr float MOVE_POWER_X = 2.0f;	//�ړ��X�s�[�h

constexpr int DRUM_IMAGE_POS_X = 304;	//�摜�ʒu��
constexpr int DRUM_IMAGE_POS_Y = 304;	//�摜�ʒu��
constexpr int DRUM_IMAGE_SIZE_X = 64;	//�摜�T�C�Y��
constexpr int DRUM_IMAGE_SIZE_Y= 80;	//�摜�T�C�Y��

constexpr float OFF_SCREEN_OFFSET = 80.0f;	//��ʊO�ɏo���Ƃ��̃I�t�Z�b�g

constexpr double ADD_ANGLE = 0.5;

Drum::Drum()
{
	tileImage_ = -1;
	drumImage_ = -1;
}

Drum::~Drum()
{
}

void Drum::Init(TmxObj& tmx)
{
	for (auto& drum : tmx.GetCollDrum())
	{
		drum_.push_back(
			{
				{ drum.first.first.x + OFFSET_X,drum.first.first.y + OFFSET_Y },
				0.0,
				0.0,
				0.0,
				255,	//���l
				drum.second,
				std::chrono::system_clock::now()
			}
		);
	}

	//�n���h���ǂݍ���
	tileImage_ = LoadGraph("./Image/Tile.png");
	drumImage_ = DerivationGraph(DRUM_IMAGE_POS_X, DRUM_IMAGE_POS_Y, DRUM_IMAGE_SIZE_X, DRUM_IMAGE_SIZE_Y, tileImage_);

	//����
	nowTime_ = std::chrono::system_clock::now();
	oldTime_ = nowTime_;
}

void Drum::Reset(TmxObj& tmx)
{
	drum_.clear();
	for (auto& drum : tmx.GetCollDrum())
	{
		drum.second = true;
		drum_.push_back(
			{
				{ drum.first.first.x + OFFSET_X,drum.first.first.y + OFFSET_Y },
				0.0,
				0.0,
				0.0,
				255,
				drum.second,
				std::chrono::system_clock::now()
			}
		);
	}
}

void Drum::Update()
{
	nowTime_ = std::chrono::system_clock::now();
	auto eltime = nowTime_ - oldTime_;
	auto msec = std::chrono::duration_cast<std::chrono::microseconds>(eltime).count();
	auto delta = msec / 1000000.0;

	// ���̂̈ʒu = �����x * ���� + 1/2( �����x * ���Ԃ̓�� ) y(pos.y) = vo(�����x)t(����)�|1 / 2 * g(accel) * t^2
	for (auto& drum : drum_)
	{
		if (drum.pos.y > lpSceneMng.GetViewSize().y + OFF_SCREEN_OFFSET)
		{
			continue;
		}
		if (drum.alive)
		{
			drum.elapTime = nowTime_;
		}
		else
		{
			//�f���^�v�Z
			eltime = nowTime_ - drum.elapTime;
			msec = std::chrono::duration_cast<std::chrono::microseconds>(eltime).count();
			delta = msec / 1000000.0;

			//���W�X�V
			drum.pos.x += MOVE_POWER_X;
			drum.pos.y += static_cast<float>((INIT_ACCEL) +
				((1.0 / 2.0) * (drum.accel * (std::pow(delta, 2.0)))));

			//��]�ʉ��Z
			drum.angle += ADD_ANGLE;

			//�����ʉ��Z
			drum.accel += 1.0;

			//0.05�b���Ƃɓ_��
			if (drum.time >= BLINK_TIME)
			{
				if (drum.alpha == ALPHA_FULL)
				{
					drum.alpha = ALPHA_HALF;
				}
				else
				{
					drum.alpha = ALPHA_FULL;
				}
				drum.time = 0.0;
			}
			else
			{
				drum.time += lpSceneMng.GetDeltaTime();
			}
		}
	}
}

void Drum::Draw(Vector2 offset)
{
	// ���̂̈ʒu = �����x * ���� + 1/2( �����x * ���Ԃ̓�� ) y(pos.y) = vo(�����x)t(����)�|1 / 2 * g(accel) * t^2
	for (auto& drum : drum_)
	{
		if (drum.pos.y > lpSceneMng.GetViewSize().y + 80.0f)
		{
			continue;
		}
		if (drum.alive)
		{
			//�}�b�v��̃h������
			DrawRotaGraph(static_cast<int>(drum.pos.x - offset.x), static_cast<int>(drum.pos.y - offset.y), 1.0, 0.0, drumImage_, true);
		}
		else
		{
			//�Փˎ��̃h������
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, drum.alpha);
			DrawRotaGraph(static_cast<int>(drum.pos.x - offset.x), static_cast<int>(drum.pos.y - offset.y), 1.0, drum.angle, drumImage_, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void Drum::Release(void)
{
	DeleteGraph(tileImage_);
	DeleteGraph(drumImage_);
}

bool Drum::CheckDrum(int gid)
{
	//�h�����ʂ͕ʓr�`��
	if (gid == 21 || gid == 22 || gid == 50 || gid == 51 || gid == 79 || gid == 80 ||
		gid == 24 || gid == 25 || gid == 53 || gid == 54 || gid == 82 || gid == 83 ||
		gid == 27 || gid == 28 || gid == 56 || gid == 57 || gid == 85 || gid == 86 ||
		gid == 404 || gid == 405 || gid == 433 || gid == 434 || gid == 462 || gid == 463)
	{
		return true;
	}

	if (gid == 570 || gid == 571 || gid == 572 || gid == 573 || 
		gid == 599 || gid == 600 ||gid == 601 || gid == 602 || 
		gid == 628 || gid == 629 || gid == 630 || gid == 631 ||
		gid == 657 || gid == 658 || gid == 659 || gid == 660 || 
		gid == 686 || gid == 687 ||gid == 688 || gid == 689 )
	{
		return true;
	}
	return false;
}

void Drum::UpdateAlive(TmxObj& tmx)
{
	int num = 0;
	for (auto& drum : tmx.GetCollDrum())
	{
		if (!drum.second && drum_[num].alive)
		{
			drum_[num].alive = drum.second;
		}
		num++;
	}
}
