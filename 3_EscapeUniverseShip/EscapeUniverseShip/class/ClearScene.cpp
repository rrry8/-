#include<DxLib.h>
#include "ClearScene.h"
#include"SelectScene.h"
#include"SceneManager.h"
#include"Transition/FadeInOut.h"
#include"Input/Controller.h"
#include"Input/KeyInput.h"
#include"Input/PadInput.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"

constexpr float CLEAR_SIZE_X = 777;       //クリアのサイズｘ

constexpr float PRESS_Z_OFFSET = 200.0f;    //操作説明のオフセット
constexpr float PRESS_Z_SIZE_Y = 58;        //操作案内のサイズｙ(キーボード)
constexpr float PRESS_A_SIZE_Y = 48;        //操作案内のサイズｙ(パッド)

constexpr double FLUFFY_LIMIT_TIME = 0.25;    //文字がふわふわが切り替わる時間
constexpr double ADD_FLUFFY_TIME = 0.004;     //時間加算値

constexpr float STAGE_NUM_OFFSET = 250.0f;  //クリアしたステージのステージNoオフセット

ClearScene::ClearScene()
{
    Init();
    DrawScreen();
}

ClearScene::ClearScene(std::string mapnum)
{
    Init();
    stageName_ = "./Image/font/nonstage.png";
    stageName_.insert(stageName_.length() - 4, mapnum);
    DrawScreen();
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init(void)
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
    con_ = 1.0;
    fluffyTime_ = 0.0;
    addFluffy_ = ADD_FLUFFY_TIME;
    PlaySoundMem(lpSoundMng.GetID("./sound/clearscene.mp3")[0], DX_PLAYTYPE_LOOP);
}

UniqueScene ClearScene::Update(UniqueScene scene)
{
    controller_->Update();
    auto controllerData = controller_->GetControllerData();

    if (controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
        StopSoundMem(lpSoundMng.GetID("./sound/clearscene.mp3")[0]);
        return std::make_unique<FadeInOut>(1.0, std::move(scene), std::make_unique<SelectScene>());
    }

    con_ += addFluffy_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();

    //時間を超えた場合小さくするか大きくするようにする
    if (FLUFFY_LIMIT_TIME <= fluffyTime_)
    {
        addFluffy_ = -addFluffy_;
        fluffyTime_ = 0.0;
    }

    DrawScreen();
    return scene;
}

void ClearScene::DrawScreen(void)
{
    auto viewsize = lpSceneMng.GetViewSize();

    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //背景
    DrawGraphF(0, 0, lpImageMng.GetID("./Image/clearBG.png")[0], true);

    //ステージクリア
    DrawGraphF((viewsize.x - CLEAR_SIZE_X) / 2, 50, lpImageMng.GetID("./Image/font/clear.png")[0], true);

    //クリアしたステージNo
    DrawRotaGraphF((viewsize.x) * 4 / 5, (viewsize.y - STAGE_NUM_OFFSET), 1.0, 0.0, lpImageMng.GetID(stageName_)[0], true);

    //操作案内
    if (GetJoypadNum() == 0)
    {
        DrawRotaGraphF((viewsize.x) / 2, (viewsize.y - PRESS_Z_OFFSET + PRESS_Z_SIZE_Y), con_, 0.0, lpImageMng.GetID("./Image/font/presskey.png")[0], true);
    }
    else
    {
        DrawRotaGraphF((viewsize.x) / 2, (viewsize.y - PRESS_Z_OFFSET + PRESS_A_SIZE_Y), con_, 0.0, lpImageMng.GetID("./Image/font/presspad.png")[0], true);
    }
}

void ClearScene::Release(void)
{
}
