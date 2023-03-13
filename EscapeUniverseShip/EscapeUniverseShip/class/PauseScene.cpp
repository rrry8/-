#include<DxLib.h>
#include "PauseScene.h"
#include"SceneManager.h"
#include"SelectScene.h"
#include"Transition/FadeInOut.h"
#include"Other/ImageMng.h"
#include "Input/KeyInput.h"
#include "Input/PadInput.h"
#include "Other/SoundMng.h"

constexpr float PAUSE_OFFSET_X = 435.0f;  //ポーズのオフセットｘ
constexpr float PAUSE_OFFSET_Y = 100.0f;  //ポーズのオフセットｙ

//ふわふわの加算値
constexpr double ADD_FLUFFY_TIME = 0.004;  //ポーズのオフセットｙ

constexpr double FLUFFY_LIMIT_TIME = 0.35;    //ふわふわの上限時間

PauseScene::PauseScene(UniqueScene scene, int id)
{
    ownerScene_ = std::move(scene);
    gameBG_ = id;
    Init();
    DrawScreen();
}

PauseScene::~PauseScene()
{
    Release();
}

void PauseScene::Init(void)
{
    if (GetJoypadNum() == 0)
    {
        //キーボードの場合
        controller_ = std::make_unique<KeyInput>();
    }
    else
    {
        //パッドの場合
        controller_ = std::make_unique<PadInput>();
    }
    controller_->Update();

    coefficient_ = 1.0;
    fluffyTime_ = 0.0;
    addFluffy_ = ADD_FLUFFY_TIME;
    nowSelect_ = PAUSESELECT::Resume;
}

UniqueScene PauseScene::Update(UniqueScene scene)
{
    //コントローラ
    controller_->Update();

    auto controllerData = controller_->GetControllerData();

    bool pushFlag = false;
    if (controllerData[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Up][static_cast<int>(ControllerAge::Old)])
    {
        pushFlag = true;
    }

    if (controllerData[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Down][static_cast<int>(ControllerAge::Old)])
    {
        pushFlag = true;
    }

    if (pushFlag)
    {
        if (nowSelect_ == PAUSESELECT::Resume)
        {
            nowSelect_ = PAUSESELECT::Select;
        }
        else if (nowSelect_ == PAUSESELECT::Select)
        {
            nowSelect_ = PAUSESELECT::Resume;
        }
        PlaySoundMem(lpSoundMng.GetID("./sound/select.mp3")[0], DX_PLAYTYPE_BACK);
        //等倍
        coefficient_ = 1.0;
        addFluffy_ = abs(addFluffy_);
        fluffyTime_ = 0.0;
    }

    coefficient_ += addFluffy_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (FLUFFY_LIMIT_TIME <= fluffyTime_)
    {
        addFluffy_ = -addFluffy_;
        fluffyTime_ = 0.0;
    }

    if (controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
        if (nowSelect_ == PAUSESELECT::Resume)
        {
            ChangeSoundVolume(150);
            return std::move(ownerScene_);
        }
        else if (nowSelect_ == PAUSESELECT::Select)
        {
            ChangeSoundVolume(150);
            StopSound();
            return std::make_unique<FadeInOut>(1.0, std::move(scene), std::make_unique<SelectScene>());
        }
    }

    //描画
    DrawScreen();

    return scene;
}

void PauseScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //背景
    DrawGraph(0, 0, gameBG_, true);

    //ポーズ
    DrawGraphF(PAUSE_OFFSET_X, PAUSE_OFFSET_Y, lpImageMng.GetID("././Image/font/Pause.png")[0], true);

    Vector2 viewSize = lpSceneMng.GetViewSize();
    //Resume Game の位置
    Vector2 resumePos = { viewSize.x / 2, PAUSE_OFFSET_Y * 3.75f };
    //Return Game の位置
    Vector2 selectPos = { viewSize.x / 2, PAUSE_OFFSET_Y * 5.0f };

    if (nowSelect_ == PAUSESELECT::Resume)
    {
        //Resume Game
        DrawRotaGraphF(resumePos.x,
            resumePos.y,
            coefficient_, 0.0,
            lpImageMng.GetID("./Image/font/ResumeGame.png")[0],
            true);

        //Return Game
        DrawRotaGraphF(selectPos.x,
            selectPos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonreturnselect.png")[0],
            true);
    }
    else if(nowSelect_ == PAUSESELECT::Select)
    {
        //Resume Game
        DrawRotaGraphF(resumePos.x,
            resumePos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonResumeGame.png")[0],
            true);

        //Resume Game
        DrawRotaGraphF(selectPos.x,
            selectPos.y,
            coefficient_, 0.0,
            lpImageMng.GetID("./Image/font/returnselect.png")[0],
            true);
    }
}

void PauseScene::Release(void)
{
}
