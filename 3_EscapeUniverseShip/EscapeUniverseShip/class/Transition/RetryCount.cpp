#include"DxLib.h"
#include "RetryCount.h"
#include"../SceneManager.h"
#include"../Other/ImageMng.h"
#include "../Input/KeyInput.h"
#include "../Input/PadInput.h"

//操作説明のサイズ
constexpr int PRESS_Z_SIZE_X = 389; //キーボードx
constexpr int PRESS_Z_SIZE_Y = 58;  //キーボードy
constexpr int PRESS_A_SIZE_X = 504; //パッドx
constexpr int PRESS_A_SIZE_Y = 48;  //パッドy

//ふわふわ時間
constexpr double FLUFFY_LIMIT_TIME = 0.25;

RetryCount::RetryCount(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene):
	TransitionScene(std::move(beforeScene), std::move(afterScene))
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
    addFluffyTime_ = 0.004;
	DrawScreen();
}

RetryCount::~RetryCount()
{
}

bool RetryCount::UpdateTransition(double delta)
{
    controller_->Update();
    auto UsedController = controller_->GetControllerData();
    //決定でゲームシーンへ
    if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        return true;
    }

    coefficient_ += addFluffyTime_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (FLUFFY_LIMIT_TIME <= fluffyTime_)
    {
        addFluffyTime_ = -addFluffyTime_;
        fluffyTime_ = 0.0;
    }

    return false;
}

void RetryCount::DrawScreen()
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    afterScene_->Draw();

    auto viewsize = lpSceneMng.GetViewSize();

    if (GetJoypadNum() == 0)
    {
        DrawRotaGraphF((viewsize.x) / 2.0f, (viewsize.y / 2.0f + PRESS_Z_SIZE_Y), coefficient_, 0.0, lpImageMng.GetID("./Image/font/presskey.png")[0], true);
    }
    else
    {
        DrawRotaGraphF((viewsize.x) / 2.0f, (viewsize.y / 2.0f + PRESS_A_SIZE_Y), coefficient_, 0.0, lpImageMng.GetID("./Image/font/presspad.png")[0], true);
    }
}