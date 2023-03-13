#include<DxLib.h>
#include"SceneManager.h"
#include "ResultScene.h"
#include"Input/Controller.h"
#include"Input/KeyInput.h"
#include"Input/PadInput.h"
#include"GameScene.h"
#include"SelectScene.h"
#include"Transition/FadeInOut.h"
#include"Transition/RetryCount.h"
#include"SceneManager.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"

//ゲームオーバーフォントのサイズ
constexpr float GameoverSizeX = 657.0f;
constexpr float GameoverSizeY = 337.0f;

//リトライのサイズ
constexpr int RetrySizeY = 63;

//Return Selectのサイズ
constexpr int SelectSizeX = 794;
constexpr int SELECT_SIZE_Y = 72;

//オフセット
constexpr float OffsetY = 50.0f;

//ふわふわの変化するまでの時間
constexpr double FLUFFY_LIMIT_TIME = 0.35;

ResultScene::ResultScene(UniqueScene scene,int id)
{
    ownerScene_ = std::move(scene);
    gameBG_ = id;
    Init();
    DrawScreen();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init(void)
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

    con_ = 1.0;
    fluffyTime_ = 0.0;
    addCoeff_ = 0.004;

    nowSelect_ = SELECT::Retry;
}

UniqueScene ResultScene::Update(UniqueScene scene)
{
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
        if (nowSelect_ == SELECT::Retry)
        {
            nowSelect_ = SELECT::Select;
        }
        else if (nowSelect_ == SELECT::Select)
        {
            nowSelect_ = SELECT::Retry;
        }

        PlaySoundMem(lpSoundMng.GetID("./sound/select.mp3")[0], DX_PLAYTYPE_BACK);
        con_ = 1.0;
        addCoeff_ = abs(addCoeff_);
        fluffyTime_ = 0.0;
    }

    if (controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);

        if (nowSelect_ == SELECT::Retry)
        {
            return std::make_unique<RetryCount>(1.0, std::move(scene), std::move(ownerScene_));
        }
        else if (nowSelect_ == SELECT::Select)
        {
            //音停止
            StopSound();
            return std::make_unique<FadeInOut>(1.0, std::move(scene), std::make_unique<SelectScene>());
        }
    }

    con_ += addCoeff_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (FLUFFY_LIMIT_TIME <= fluffyTime_)
    {
        addCoeff_ = -addCoeff_;
        fluffyTime_ = 0.0;
    }

    DrawScreen();
    return scene;
}

void ResultScene::DrawScreen(void)
{
    auto viewsize = lpSceneMng.GetViewSize();

    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //背景
    DrawGraph(0, 0,gameBG_, true);

    //ゲームオーバーフォント
    DrawGraphF(viewsize.x / 2.0f - GameoverSizeX / 2, OffsetY, lpImageMng.GetID("./Image/font/gameover.png")[0], true);

    Vector2 retryPos = { viewsize.x / 2,GameoverSizeY + OffsetY * 2.0f + RetrySizeY / 2.0f };
    Vector2 returnPos = { viewsize.x / 2,GameoverSizeY + RetrySizeY + OffsetY * 3.0f + SELECT_SIZE_Y / 2.0f };

    //フォント表示
    if (nowSelect_ == SELECT::Retry)
    {
        DrawRotaGraphF(retryPos.x,
            retryPos.y,
            con_,0.0,
            lpImageMng.GetID("./Image/font/retry.png")[0],
            true);

        DrawRotaGraphF(returnPos.x,
            returnPos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonreturnselect.png")[0],
            true);
    }
    else if (nowSelect_ == SELECT::Select)
    {
        DrawRotaGraphF(retryPos.x,
            retryPos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonretry.png")[0],
            true);

        DrawRotaGraphF(returnPos.x,
            returnPos.y,
            con_,0.0,
            lpImageMng.GetID("./Image/font/returnselect.png")[0],
            true);
    }
}

void ResultScene::Release(void)
{
}