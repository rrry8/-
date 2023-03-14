#include<DxLib.h>
#include "Drum.h"
#include"../Camera/Camera.h"
#include"../SceneManager.h"
#include"../../tmx/TmxObj.h"

constexpr float OFFSET_X = 32.0f;	//オフセットｘ
constexpr float OFFSET_Y = 40.0f;	//オフセットｙ
constexpr int ALPHA_FULL = 255;		//点滅時の濃いα値
constexpr int ALPHA_HALF = 100;		//点滅時の薄いα値
constexpr double BLINK_TIME = 0.05;	//点滅時間

constexpr float INIT_ACCEL = -10.0f;	//初速度(下方向がYの正の方向のため上方向はマイナス値)

constexpr float MOVE_POWER_X = 2.0f;	//移動スピード

constexpr int DRUM_IMAGE_POS_X = 304;	//画像位置ｙ
constexpr int DRUM_IMAGE_POS_Y = 304;	//画像位置ｘ
constexpr int DRUM_IMAGE_SIZE_X = 64;	//画像サイズｘ
constexpr int DRUM_IMAGE_SIZE_Y= 80;	//画像サイズｙ

constexpr float OFF_SCREEN_OFFSET = 80.0f;	//画面外に出たときのオフセット

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
				255,	//α値
				drum.second,
				std::chrono::system_clock::now()
			}
		);
	}

	//ハンドル読み込み
	tileImage_ = LoadGraph("./Image/Tile.png");
	drumImage_ = DerivationGraph(DRUM_IMAGE_POS_X, DRUM_IMAGE_POS_Y, DRUM_IMAGE_SIZE_X, DRUM_IMAGE_SIZE_Y, tileImage_);

	//時間
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

	// 物体の位置 = 初速度 * 時間 + 1/2( 加速度 * 時間の二乗 ) y(pos.y) = vo(初速度)t(時間)－1 / 2 * g(accel) * t^2
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
			//デルタ計算
			eltime = nowTime_ - drum.elapTime;
			msec = std::chrono::duration_cast<std::chrono::microseconds>(eltime).count();
			delta = msec / 1000000.0;

			//座標更新
			drum.pos.x += MOVE_POWER_X;
			drum.pos.y += static_cast<float>((INIT_ACCEL) +
				((1.0 / 2.0) * (drum.accel * (std::pow(delta, 2.0)))));

			//回転量加算
			drum.angle += ADD_ANGLE;

			//加速量加算
			drum.accel += 1.0;

			//0.05秒ごとに点滅
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
	// 物体の位置 = 初速度 * 時間 + 1/2( 加速度 * 時間の二乗 ) y(pos.y) = vo(初速度)t(時間)－1 / 2 * g(accel) * t^2
	for (auto& drum : drum_)
	{
		if (drum.pos.y > lpSceneMng.GetViewSize().y + 80.0f)
		{
			continue;
		}
		if (drum.alive)
		{
			//マップ上のドラム缶
			DrawRotaGraph(static_cast<int>(drum.pos.x - offset.x), static_cast<int>(drum.pos.y - offset.y), 1.0, 0.0, drumImage_, true);
		}
		else
		{
			//衝突時のドラム缶
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
	//ドラム缶は別途描画
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
