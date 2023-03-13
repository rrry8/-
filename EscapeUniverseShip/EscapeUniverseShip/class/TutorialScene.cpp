#include<DxLib.h>
#include<string>
#include "TutorialScene.h"
#include"SelectScene.h"
#include"ClearScene.h"
#include"SceneManager.h"
#include"Obj/Player.h"
#include"Camera/Camera.h"
#include"Obj/Drum.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"
#include "Input/KeyInput.h"
#include "Input/PadInput.h"

//�e�L�X�g�̈ʒu
constexpr int TEXT_POS_X = 306;
constexpr int TEXT_POS_Y = 50;

//�傫��
constexpr int WORLD_SIZE_X = 3520.0f;
constexpr int WORLD_SIZE_Y = 3200.0f;

//�e�`���[�g���A�����o��ʒu
constexpr float TUTORIAL_APPEARANCE_POS_1 = 616.0f;
constexpr float TUTORIAL_APPEARANCE_POS_2 = 1224.0f;
constexpr float TUTORIAL_APPEARANCE_POS_3 = 1708.0f;
constexpr float TUTORIAL_APPEARANCE_POS_4 = 2484.0f;
constexpr float TUTORIAL_APPEARANCE_POS_5 = 3200.0f;
constexpr float TUTORIAL_APPEARANCE_POS_6 = 3600.0f;
constexpr float TUTORIAL_APPEARANCE_POS_7 = 3800.0f;

//�S�[���̈ʒu
constexpr float GOAL_POS = 3520.0f;

constexpr double ADD_ROTATE_NUM = 0.05;

TutorialScene::TutorialScene()
{
    Init();
    DrawScreen();
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Init(void)
{
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

    tmxObj_.LoadTmx("./tmx/tutorial.tmx");
    player_ = std::make_unique<Player>(GetJoypadNum(),0.0f);
	player_->SetGoalPos(GOAL_POS);
	camera_ = std::make_unique<Camera>(Vector2(WORLD_SIZE_X, WORLD_SIZE_Y));
	drum_ = std::make_unique<Drum>();
	drum_->Init(tmxObj_);

	tileImage_ = LoadGraph("./Image/Tile.png");
	sawImage_ = DerivationGraph(336, 144, 80, 80, tileImage_);

	textNum_ = 0;
	gravFlag_ = false;
}

UniqueScene TutorialScene::Update(UniqueScene scene)
{
	Vector2 pos = player_->GetPos();
	if (gravFlag_)
	{
		if (player_->TutorialMove(tmxObj_,camera_->GetPos(),textNum_))
		{
			PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
			gravFlag_ = false;
		}
	}

	if (!gravFlag_)
	{
		//�J�����X�V
		camera_->Update(pos);

		//�h�����ʂ̍X�V
		drum_->Update();

		//�v���C���[�̍X�V
		player_->UpdateTutorial(tmxObj_, camera_->GetPos(), nullptr);

		//�h�����ʂ̐����t���O�X�V
		drum_->UpdateAlive(tmxObj_);

		//�̂������]�̍X�V
		angle_ -= ADD_ROTATE_NUM;
	}

	DrawScreen();
	//616,1224,1708,2484
	if (pos.x == TUTORIAL_APPEARANCE_POS_1)
	{
		gravFlag_ = true;
		textNum_ = 1;
	}
	if (pos.x == TUTORIAL_APPEARANCE_POS_2)
	{
		gravFlag_ = true;
		textNum_ = 2;
	}
	if (pos.x == TUTORIAL_APPEARANCE_POS_3)
	{
		gravFlag_ = true;
		textNum_ = 3;
	}
	if (pos.x == TUTORIAL_APPEARANCE_POS_4)
	{
		gravFlag_ = true;
		textNum_ = 4;
	}
	if (pos.x == TUTORIAL_APPEARANCE_POS_5)
	{
		gravFlag_ = true;
		textNum_ = 5;
	}
	if (pos.x == TUTORIAL_APPEARANCE_POS_6)
	{
		if (GetJoypadNum() == 0)
		{
			gravFlag_ = true;
			textNum_ = 7;
		}
		else
		{
			gravFlag_ = true;
			textNum_ = 6;

		}
	}

	if (pos.x > TUTORIAL_APPEARANCE_POS_7)
	{
		return std::make_unique<SelectScene>();
	}
    return scene;
}

void TutorialScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto& tilesize = tmxObj_.GetTileSize();
	auto& offset = camera_->GetPos();
	//�}�b�v��`��
	for (const auto& data : tmxObj_.GetMapData())
	{
		for (int y = static_cast<int>(offset.y / tilesize.y) - 3; y < offset.y / tilesize.y + lpSceneMng.GetViewSize().y / tilesize.y + 3; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetViewSize().x / tilesize.x + 3; x++)
			{
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int checkY = y;
					if (y < 0)
					{
						checkY = 0;
					}
					int gid = data.second[checkY * tmxObj_.GetWorldArea().x + x] - tmxObj_.GetFirstGID();
					if (gid >= 0)
					{
						//�̂�����̕`��
						SetDrawZ(0.65f);
						if (DrawSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}

						//�h�����ʂ͏��O
						if (drum_->CheckDrum(gid))
						{
							continue;
						}

						//���ԃ|�C���g
						SetDrawZ(0.7f);
						DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
							lpImageMng.GetmapID("Tile.png")[gid], true);
					}
				}
			}
		}
	}

	//�h�����ʕ`��
	SetDrawZ(0.5f);
	drum_->Draw(offset);

	std::string textName = "././Image/font/tutorial.png";
	std::string strTextNum = std::to_string(textNum_);
	textName.insert(textName.length() - 4, strTextNum);

	if (gravFlag_)
	{
		DrawGraph(TEXT_POS_X, TEXT_POS_Y,lpImageMng.GetID(textName)[0], true);
	}

	SetDrawZ(0.45f);
	player_->Draw(camera_->GetPos());
}

void TutorialScene::DrawStage(void)
{
}

void TutorialScene::Release(void)
{
}
