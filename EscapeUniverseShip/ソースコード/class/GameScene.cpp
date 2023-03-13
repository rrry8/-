#include<DxLib.h>
#include "GameScene.h"
#include"Obj/Player.h"
#include"Obj/Drum.h"
#include"Obj/CheckPoint.h"
#include"Camera/Camera.h"
#include"Other/ImageMng.h"
#include"SceneManager.h"
#include"ResultScene.h"
#include"ClearScene.h"
#include"PauseScene.h"
#include"Transition/FadeInOut.h"

constexpr float ROT_CIRCLE_LENGTH = 100.0f;	//��鋘�̉�]���a

//�̂�������
constexpr int SAW_IMAGE_POS_X = 336;	//�摜�ʒu��
constexpr int SAW_IMAGE_POS_Y = 144;	//�摜�ʒu��
constexpr int SAW_IMAGE_SIZE_X = 80;	//�摜�T�C�Y��
constexpr int SAW_IMAGE_SIZE_Y = 80;	//�摜�T�C�Y��
constexpr float SAW_IMAGE_COLL_OFFSET = 24.0f;	//�����蔻��̃I�t�Z�b�g

GameScene::GameScene()
{
	Init();
}

GameScene::GameScene(std::string map,float addRot,int num)
{
	tmxObj_.LoadTmx(map);
	int min = 5;
	if (num == 2)
	{
		min = 6;
	}
	addSawRot_ = addRot;
	stageNum_ = map.substr(map.length() - min, num);

	//������
	Init();

	//�J�ڗp�ɕ`��
	DrawStage();
}

GameScene::~GameScene()
{
	//�j��
	Release();
}

void GameScene::Init(void)
{
	//�h������
	drum_ = std::make_unique<Drum>();
	drum_->Init(tmxObj_);

	//���ԃ|�C���g
	check_ = std::make_shared<CheckPoint>();
	check_->Init(tmxObj_);

	Vector2 worldarea = tmxObj_.GetWorldArea();
	Vector2 tilesize = tmxObj_.GetTileSize();
	Vector2 worldSize = { worldarea.x * tilesize.x,worldarea.y * tilesize.y };

	//�J����
	camera_ = std::make_unique<Camera>(worldSize);
	//�v���C���[
	player_ = std::make_unique<Player>(GetJoypadNum(), addSawRot_);
	//�S�[���ʒu
	player_->SetGoalPos(worldSize.x);

	//�^�C������̂������ǂݍ���
	tileImage_ = LoadGraph("./Image/Tile.png");
	sawImage_ = DerivationGraph(SAW_IMAGE_POS_X, SAW_IMAGE_POS_Y, SAW_IMAGE_SIZE_X, SAW_IMAGE_SIZE_Y, tileImage_);
	sawRotImage_ = DerivationGraph(SAW_IMAGE_POS_X, SAW_IMAGE_POS_Y + SAW_IMAGE_SIZE_X, SAW_IMAGE_SIZE_X, SAW_IMAGE_SIZE_Y, tileImage_);

	angle_ = 0.0;
	rotSawAngle_ = 0.0f;
	gameBG_ = MakeScreen(screenSizeX_, screenSizeY_, true);
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//�J�����X�V
	camera_->Update(player_->GetPos());

	//�h�����ʂ̍X�V
	drum_->Update();
	check_->Update();

	SCENE nowScene = player_->Update(tmxObj_, camera_->GetPos(), check_);

	if (nowScene == SCENE::OVER)
	{
		GetDrawScreenGraph(0, 0, screenSizeX_, screenSizeY_, gameBG_);
		//�h�����ʂ̃f�[�^�����Z�b�g
		drum_->Reset(tmxObj_);

		//�J�����������Z�b�g
		camera_->Update(player_->GetPos());
		DrawScreen();
		player_->SetAngle(static_cast<float>(rotSawAngle_));
		return std::make_unique<ResultScene>(std::move(scene), gameBG_);
	}
	else if (nowScene == SCENE::CLEAR)
	{
		StopSound();
		return std::make_unique<ClearScene>(stageNum_);
	}
	else if (nowScene == SCENE::PAUSE)
	{
		ChangeSoundVolume(80);
		GetDrawScreenGraph(0, 0, screenSizeX_, screenSizeY_, gameBG_);
		return std::make_unique<PauseScene>(std::move(scene), gameBG_);
	}

	//�h�����ʂ̐����t���O�X�V
	drum_->UpdateAlive(tmxObj_);

	//�̂������]�̍X�V
	angle_ -= 0.05;
	rotSawAngle_ += addSawRot_;

	//�`��
	DrawScreen();
	return scene;
}

void GameScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto tilesize = tmxObj_.GetTileSize();
	auto offset = camera_->GetPos();
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	//�}�b�v��`��
	for (const auto& data : tmxObj_.GetMapData())
	{
		for (int y = static_cast<int>(offset.y / tilesize.y) - 3; y < offset.y / tilesize.y + lpSceneMng.GetViewSize().y / tilesize.y + 3; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetViewSize().x / tilesize.x + 3; x++)
			{
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int y2 = y;
					if (y < 0)
					{
						y2 = 0;
					}
					int gid = data.second[y2 * static_cast<int>(tmxObj_.GetWorldArea().x) + x] - tmxObj_.GetFirstGID();
					if (gid >= 0)
					{
						SetDrawZ(0.35f);
						if (DrawSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}
						if (DrawRotSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}

						//�h�����ʂ͏��O
						if (drum_->CheckDrum(gid))
						{
							continue;
						}

						//���ԃ|�C���g
						if (check_->GetFlag() && check_->CheckCheckPoint(gid))
						{
							check_->ChangeID(gid);
							SetDrawZ(0.35f);
							DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
								lpImageMng.GetmapID("Tile.png")[gid], true);
							continue;
						}
						SetDrawZ(0.7f);
						//�w�i
						DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
							lpImageMng.GetmapID("Tile.png")[gid], true);
					}
				}
			}
		}
	}

	//�h�����ʕ`��
	SetDrawZ(0.3f);
	drum_->Draw(offset);

	//�v���C���[�`��
	SetDrawZ(0.45f);
	player_->Draw(camera_->GetPos());
}

void GameScene::DrawStage(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto tilesize = tmxObj_.GetTileSize();
	auto offset = camera_->GetPos();
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	//�}�b�v��`��
	for (const auto& data : tmxObj_.GetMapData())
	{
		for (int y = static_cast<int>(offset.y / tilesize.y); y < offset.y / tilesize.y + lpSceneMng.GetViewSize().y / tilesize.y; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetViewSize().x / tilesize.x + 3; x++)
			{
				SetDrawZ(0.35f);
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int gid = data.second[y * static_cast<int>(tmxObj_.GetWorldArea().x) + x] - tmxObj_.GetFirstGID();
					if (gid >= 0)
					{
						//�̂�����̕`��
						SetDrawZ(0.65f);
						if (DrawSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}
						if (DrawRotSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}

						//�h�����ʂ͏��O
						if (drum_->CheckDrum(gid))
						{
							continue;
						}

						//���ԃ|�C���g
						if (check_->GetFlag() && check_->CheckCheckPoint(gid))
						{
							check_->ChangeID(gid);
							SetDrawZ(0.5f);
							DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
								lpImageMng.GetmapID("Tile.png")[gid], true);
							continue;
						}
						SetDrawZ(0.7f);
						//�w�i
						DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
							lpImageMng.GetmapID("Tile.png")[gid], true);
					}
				}
			}
		}
	}

	SetDrawZ(0.5f);
	drum_->Draw(offset);
}

bool GameScene::DrawSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset)
{
	//�̂������ID�͕ʓr�`��
	if (gid == 282 || gid == 283 || gid == 284 || gid == 285 || gid == 286 ||
		gid == 311 || gid == 312 || gid == 313 || gid == 314 || gid == 315 ||
		gid == 340 || gid == 341 || gid == 342 || gid == 343 || gid == 344 ||
		gid == 369 || gid == 370 || gid == 371 || gid == 372 || gid == 373 ||
		gid == 398 || gid == 399 || gid == 400 || gid == 401 || gid == 402
		)
	{
		if (gid == 402)
		{
			DrawRotaGraph(
				static_cast<int>(x * tilesize.x - SAW_IMAGE_COLL_OFFSET - offset.x),
				static_cast<int>(y * tilesize.y - SAW_IMAGE_COLL_OFFSET - offset.y),
				1.0, angle_, sawImage_, true
			);
		}

		return true;
	}
	return false;
}

bool GameScene::DrawRotSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset)
{
	//�̂������ID�͕ʓr�`��
	if (gid == 427 || gid == 428 || gid == 429 || gid == 430 || gid == 431 ||
		gid == 456 || gid == 457 || gid == 458 || gid == 459 || gid == 460 ||
		gid == 485 || gid == 486 || gid == 487 || gid == 488 || gid == 489 ||
		gid == 514 || gid == 515 || gid == 516 || gid == 517 || gid == 518 ||
		gid == 543 || gid == 544 || gid == 545 || gid == 546 || gid == 547
		)
	{
		if (gid == 547)
		{
			//���W�A���ɕϊ�
			float radius = static_cast<float>(rotSawAngle_) * 3.14f / 180.0f;
			float addX = cos(radius) * ROT_CIRCLE_LENGTH;
			float addY = sin(radius) * ROT_CIRCLE_LENGTH;

			Vector2 sawPos = { 
				x * tilesize.x - SAW_IMAGE_COLL_OFFSET,
				y * tilesize.y - SAW_IMAGE_COLL_OFFSET
			};

			Vector2 sawRotPos = {
				sawPos.x ,
				sawPos.y
			};

			sawPos = {
				sawRotPos.x + addX,
				sawRotPos.y + addY 
			};

			//�`��
			DrawRotaGraphF(
				sawPos.x - offset.x,
				sawPos.y - offset.y,
				1.0, angle_, sawRotImage_, true
			);
		}

		return true;
	}
	return false;
}

void GameScene::Release(void)
{
    player_->Release();
	DeleteGraph(tileImage_);
	DeleteGraph(sawImage_);
	DeleteGraph(sawRotImage_);
	DeleteGraph(gameBG_);
	drum_->Release();
	if (check_ != nullptr)
	{
		check_->Release();
	}
}
