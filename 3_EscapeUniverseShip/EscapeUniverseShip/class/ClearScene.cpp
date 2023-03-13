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

constexpr float CLEAR_SIZE_X = 777;       //�N���A�̃T�C�Y��

constexpr float PRESS_Z_OFFSET = 200.0f;    //��������̃I�t�Z�b�g
constexpr float PRESS_Z_SIZE_Y = 58;        //����ē��̃T�C�Y��(�L�[�{�[�h)
constexpr float PRESS_A_SIZE_Y = 48;        //����ē��̃T�C�Y��(�p�b�h)

constexpr double FLUFFY_LIMIT_TIME = 0.25;    //�������ӂ�ӂ킪�؂�ւ�鎞��
constexpr double ADD_FLUFFY_TIME = 0.004;     //���ԉ��Z�l

constexpr float STAGE_NUM_OFFSET = 250.0f;  //�N���A�����X�e�[�W�̃X�e�[�WNo�I�t�Z�b�g

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
        //�L�[�{�[�h�̏ꍇ
        controller_ = std::make_unique<KeyInput>();
    }
    else
    {
        //�p�b�h�̏ꍇ
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

    //���Ԃ𒴂����ꍇ���������邩�傫������悤�ɂ���
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

    //�w�i
    DrawGraphF(0, 0, lpImageMng.GetID("./Image/clearBG.png")[0], true);

    //�X�e�[�W�N���A
    DrawGraphF((viewsize.x - CLEAR_SIZE_X) / 2, 50, lpImageMng.GetID("./Image/font/clear.png")[0], true);

    //�N���A�����X�e�[�WNo
    DrawRotaGraphF((viewsize.x) * 4 / 5, (viewsize.y - STAGE_NUM_OFFSET), 1.0, 0.0, lpImageMng.GetID(stageName_)[0], true);

    //����ē�
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
