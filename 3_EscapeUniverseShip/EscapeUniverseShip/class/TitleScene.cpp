#include<DxLib.h>
#include "TitleScene.h"
#include"SelectScene.h"
#include"SceneManager.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"
#include"Transition/FadeInOut.h"
#include"Input/KeyInput.h"
#include"Input/PadInput.h"

//�ē��̑傫��
constexpr int PRESS_Z_SIZE_X = 389; //�L�[�{�[�h��
constexpr int PRESS_Z_SIZE_Y = 58;  //�L�[�{�[�h��
constexpr int PRESS_A_SIZE_X = 504; //�p�b�h��
constexpr int PRESS_A_SIZE_Y = 48;  //�p�b�h��

//�I�t�Z�b�g��
constexpr int PRESS_OFFSET_Y = 200;  //

//�ӂ�ӂ�ő厞��
constexpr double FLUFFY_LIMIT_TIME = 0.25;

//�^�C�g���̃T�C�Y
constexpr int TITLE_SIZE_Y = 384;
//�^�C�g���̈ʒu��
constexpr int TITLE_POS_X = 157;

TitleScene::TitleScene()
{
    Init();
    DrawScreen();
}

TitleScene::~TitleScene()
{
    Release();
}

void TitleScene::Init()
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

    //��������
    InitSound();

    PlaySoundMem(lpSoundMng.GetID("./sound/titlescene.mp3")[0], DX_PLAYTYPE_LOOP);
    coefficient_ = 1.0;
    fluffyTime_ = 0.0;
    addFluffyTime_ = 0.004;
    titlePosY_ = -TITLE_SIZE_Y;
}

UniqueScene TitleScene::Update(UniqueScene scene)
{
    //�R���g���[��
    controller_->Update();
    auto UsedController = controller_->GetControllerData();
    if (titlePosY_ != 50)
    {
        titlePosY_ += 2;
        if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
        {
            titlePosY_ = 50;
        }
    }
    else
    {
        if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
        {
            PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
            StopSoundMem(lpSoundMng.GetID("./sound/titlescene.mp3")[0]);
            return std::make_unique<FadeInOut>(0.5,std::move(scene),std::make_unique<SelectScene>());
        }
        coefficient_ += addFluffyTime_;
        fluffyTime_ += lpSceneMng.GetDeltaTime();
        if (FLUFFY_LIMIT_TIME <= fluffyTime_)
        {
            addFluffyTime_ = -addFluffyTime_;
            fluffyTime_ = 0.0;
        }
    }

    //�`��
    DrawScreen();

    return scene;
}

void TitleScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //�w�i
    DrawGraph(0, 0, lpImageMng.GetID("./Image/BG.png")[0], true);
    DrawGraph(TITLE_POS_X, titlePosY_, lpImageMng.GetID("./Image/font/title2.png")[0], true);

    auto viewSize = lpSceneMng.GetViewSize();

    //�w��
    if (GetJoypadNum() == 0)
    {
        DrawRotaGraphF((viewSize.x) / 2.0f, (viewSize.y - PRESS_OFFSET_Y + PRESS_Z_SIZE_Y), coefficient_, 0.0, lpImageMng.GetID("./Image/font/presskey.png")[0], true);
    }
    else
    {
        DrawRotaGraphF((viewSize.x) / 2.0f, (viewSize.y - PRESS_OFFSET_Y + PRESS_A_SIZE_Y), coefficient_, 0.0, lpImageMng.GetID("./Image/font/presspad.png")[0], true);
    }
}

void TitleScene::Release(void)
{
}

void TitleScene::InitSound(void)
{
    //BGM�̑傫���ύX
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/decide.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/titlescene.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/early.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/mid.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/last.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/ex.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/selectscene.mp3")[0]);
    ChangeVolumeSoundMem(200, lpSoundMng.GetID("./sound/coll.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/fall.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/big.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/small.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/drum.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/checkPoint.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/clearscene.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/dash.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/gravity.mp3")[0]);
}
