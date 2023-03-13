#include "MenuScene.h"
#include "../UI/SceneUIManager.h"
#include "../../_debug/_DebugConOut.h"
#include "LoadScene.h"

MenuScene::MenuScene(uniqueScene& oldscene)
{
	mOldScene = std::move(oldscene);//ひとつ前のシーンの情報を保持
	Init();
	DrawOwnScreen(0.0);
}

MenuScene::~MenuScene()
{
}

bool MenuScene::Init(void)
{
	return false;
}

uniqueScene MenuScene::Update(double delta, uniqueScene ownScene)
{
	cntData_ = lpCntMng.GetController()->GetCntData();
	//if (CheckHitKey(KEY_INPUT_SPACE))
	//auto a = mouseInput & usedController[InputID::ClickLeft][static_cast<int>(Trg::Now)];
	if (KeyTrgDown(InputID::Decide))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
		switch (lpSceneUIMng.GetHitItem_Main())//今カーソルが乗っている項目を取得
		{
		case UI_MAIN_ITEM::M_GAME:

			state_ = MenuState::TO_GAME;
			return lpSceneUIMng.Init(std::move(mOldScene));
			break;
		case UI_MAIN_ITEM::M_RESTART:

			state_ = MenuState::RESTART;
			//return lpSceneUIMng.Init(std::move(mOldScene));
			lpStage.Remove();
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					0.5,
					std::move(ownScene),
					lpSceneUIMng.Init(std::move(std::make_unique<LoadScene>(lpSceneUIMng.GetStageNum())))
					)));
			break;
		case UI_MAIN_ITEM::M_OPTION:
			state_ = MenuState::OPTION;
			lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
			return lpSceneUIMng.Init((std::move(std::make_unique<OptionScene>(ownScene)))); // 実体
			
			break;
		case UI_MAIN_ITEM::M_SELECT:
			state_ = MenuState::TO_SELECT;
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/streetofrage.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/streetofrage.mp3")[0]);
			}
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/machikouba.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/machikouba.mp3")[0]);
			}
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					0.5,
					std::move(ownScene),
					lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(1))))
					)));
		default:
			break;
		}
	}
	if (KeyTrgDown(InputID::Menu))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI選択音.mp3");
		return lpSceneUIMng.Init(std::move(mOldScene));
	}

	DrawOwnScreen(delta);

	return ownScene;
}

void MenuScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	mOldScene->Draw(delta);

	lpCntMng.GetController()->Draw();
}
