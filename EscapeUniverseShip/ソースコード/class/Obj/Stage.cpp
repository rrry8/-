#include<DxLib.h>
#include<string>
#include "Stage.h"
#include"Player.h"
#include"../SceneManager.h"
#include"../Other/ImageMng.h"
#include"../Other/SoundMng.h"
#include"../Input/Controller.h"
#include"../Input/KeyInput.h"
#include"../Input/PadInput.h"

//セレクト関係
constexpr int SELECT_SIZE_Y = 145;    //セレクトのサイズY
constexpr int SELECT_OFFSET_X = 66;   //セレクトのオフセットX
constexpr int SELECT_OFFSET_Y = 40;   //セレクトのオフセットY

//操作説明関係
constexpr float OPERATION_SIZE_X = 388.0f; //操作説明のサイズ

//ステージ関係
constexpr int STAGE_SIZE_X = 221;                                   //ステージフォントのサイズX
constexpr int STAGE_SIZE_Y = 48;                                    //ステージフォントのサイズY
constexpr int STAGE_OFFSET_X = 118;                                 //ステージのオフセットX
constexpr int STAGE_OFFSET_Y = SELECT_SIZE_Y + SELECT_OFFSET_Y * 3; //ステージのオフセットX
constexpr int TEXT_BETWEEN_DISTANCE = 50;                           //フォント同士の間
constexpr int ALL_STAGE_NUM = 12;                                   //ステージの数
constexpr int STAGE_NUM_X = ALL_STAGE_NUM / 3;                      //ステージの数X方向
constexpr int STAGE_NUM_Y = ALL_STAGE_NUM / 4;                      //ステージの数Y方向

constexpr double FLUFFY_LIMIT_TIME = 0.5;           //ふわふわ最大時間
constexpr double FLUFFY_ADD_COEFFICIENT = 0.004;    //ふわふわの足す倍率

// ./Image/font/stageの文字数
constexpr size_t STAGE_PATH_NUM = 18;
// ./Image/font/nonstage.pngの文字数
constexpr size_t NONSELECT_STAGE_PATH_NUM = 21;

Stage::Stage(int cntType)
{
	Init(cntType);
}

Stage::~Stage()
{
    Release();
}

bool Stage::Init(int cntType)
{
    if (cntType == 0)
    {
        //キーボードの場合
        controller_ = std::make_unique<KeyInput>();
    }
    else
    {
        //パッドの場合
        controller_ = std::make_unique<PadInput>();
    }

    coefficient_ = 1.0;
    fluffyTime_ = 0.0;
    addCoefficient_ = FLUFFY_ADD_COEFFICIENT;
    nowStage_ = STAGE::One;
    controller_->Update();
    //サウンド音
    ChangeVolumeSoundMem(100, lpSoundMng.GetID("./sound/select.mp3")[0]);
    return true;
}

STAGE Stage::Update()
{
    controller_->Update();

    ChangeStage();

    coefficient_ += addCoefficient_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (FLUFFY_LIMIT_TIME <= fluffyTime_)
    {
        addCoefficient_ = -addCoefficient_;
        fluffyTime_ = 0.0;
    }
    auto controllerData = controller_->GetControllerData();

	//決定でゲームシーンへ
	if (controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
	{
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
        return nowStage_;
	}
	if (controllerData[KeyID::Cancel][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Cancel][static_cast<int>(ControllerAge::Old)])
	{
        return STAGE::End;
	}
    if (controllerData[KeyID::Button][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Button][static_cast<int>(ControllerAge::Old)])
    {
        return STAGE::Tutorial;
    }
    return STAGE::None;
}

void Stage::Draw()
{
    std::string backGroundPath = "./Image/BGstage.png";
    std::string stage = std::to_string(static_cast<int>(nowStage_));
    backGroundPath.insert(backGroundPath.length() - 4, stage);

    //ステージ背景の描画
    DrawGraph(0, 0, lpImageMng.GetID(backGroundPath)[0], true);

    //SelectStageの描画
    DrawGraphF(SELECT_OFFSET_X, SELECT_OFFSET_Y, lpImageMng.GetID("./Image/font/stageselect.png")[0], true);

    auto view = lpSceneMng.GetViewSize();

    //操作説明の描画
    if (GetJoypadNum() == 0)
    {
        //キー
        DrawGraphF(view.x / 2.0f - OPERATION_SIZE_X / 2, view.y * 3 / 4, lpImageMng.GetID("./Image/font/keyoperation.png")[0], true);
        //チュートリアル
        DrawGraphF(view.x / 2.0f - OPERATION_SIZE_X / 2 + 61.0f, view.y * 3 / 4 + 23.0f, lpImageMng.GetID("././Image/font/operatetutorial.png")[0], true);
    }
    else
    {
        //キー
        DrawGraphF(view.x / 2.0f - OPERATION_SIZE_X / 2, view.y * 3 / 4, lpImageMng.GetID("./Image/font/padoperation.png")[0], true);
        //チュートリアル
        DrawGraphF(view.x / 2.0f - OPERATION_SIZE_X / 2 + 103.0f, view.y * 3 / 4 + 23.0f, lpImageMng.GetID("././Image/font/operatetutorialpad.png")[0], true);
    }

    //ステージ1~の描画
    for (int x = 0; x < STAGE_NUM_X; x++)
    {
        for (int y = 0; y < STAGE_NUM_Y; y++)
        {
            int n = (y * 4) + (x + 1);
            if (static_cast<int>(nowStage_) != n)
            {
                std::string id = "./Image/font/nonstage.png";
                std::string num = std::to_string(n);
                id.insert(NONSELECT_STAGE_PATH_NUM, num);
                DrawGraph(
                    STAGE_OFFSET_X + STAGE_SIZE_X * x + TEXT_BETWEEN_DISTANCE * x,
                    STAGE_OFFSET_Y + STAGE_SIZE_Y * y + TEXT_BETWEEN_DISTANCE * y,
                    lpImageMng.GetID(id)[0],
                    true);
            }
            else
            {
                std::string id = "./Image/font/stage.png";
                std::string num = std::to_string((y * 4) + (x + 1));
                id.insert(STAGE_PATH_NUM, num);
                DrawRotaGraph(
                    (STAGE_OFFSET_X + STAGE_SIZE_X * x + TEXT_BETWEEN_DISTANCE * x) + STAGE_SIZE_X / 2,
                    (STAGE_OFFSET_Y + STAGE_SIZE_Y * y + TEXT_BETWEEN_DISTANCE * y) + STAGE_SIZE_Y / 2,
                    coefficient_,
                    0.0,
                    lpImageMng.GetID(id)[0],
                    true);

                pos_ = { static_cast<float>((STAGE_OFFSET_X + STAGE_SIZE_X * x + TEXT_BETWEEN_DISTANCE * x) + STAGE_SIZE_X / 2),
                    static_cast<float>((STAGE_OFFSET_Y + STAGE_SIZE_Y * y + TEXT_BETWEEN_DISTANCE * y) + STAGE_SIZE_Y / 2) };
            }
        }
    }
}

void Stage::Release(void)
{
}

Vector2 Stage::GetPos(void)
{
    return pos_ - Vector2{ STAGE_SIZE_X / 2.0f, STAGE_SIZE_Y / 2.0f };
}

void Stage::ChangeStage(void)
{
    //コントローラデータ
    auto controllerData = controller_->GetControllerData();
    //移動キーを押したかどうかフラグ
    bool moveFlag = false;
    int stage = static_cast<int>(nowStage_);
    if (controllerData[KeyID::Left][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Left][static_cast<int>(ControllerAge::Old)])
    {
        moveFlag = true;
        if (stage > static_cast<int>(STAGE::One))
        {
            if (stage % STAGE_NUM_X == 1)
            {
                stage += STAGE_NUM_X;
            }
            stage--;
        }
        else
        {
            stage += STAGE_NUM_X - 1;
        }
    }
    if (controllerData[KeyID::Right][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Right][static_cast<int>(ControllerAge::Old)])
    {
        moveFlag = true;
        if (stage < static_cast<int>(STAGE::Twelve))
        {
            if (stage % STAGE_NUM_X == 0)
            {
                stage -= STAGE_NUM_X;
            }
            stage++;
        }
        else
        {
            stage -= STAGE_NUM_X - 1;
        }
    }

    if (controllerData[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Up][static_cast<int>(ControllerAge::Old)])
    {
        moveFlag = true;
        if (stage > static_cast<int>(STAGE::Four))
        {
            stage -= STAGE_NUM_X;
        }
        else
        {
            stage += STAGE_NUM_X * (STAGE_NUM_Y - 1);
        }
    }
    if (controllerData[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Down][static_cast<int>(ControllerAge::Old)])
    {
        moveFlag = true;
        if (stage < static_cast<int>(STAGE::Nine))
        {
            stage += STAGE_NUM_X;
        }
        else
        {
            stage -= STAGE_NUM_X * (STAGE_NUM_Y - 1);
        }
    }

    if (moveFlag)
    {
        std::string bg = "./Image/BGstage.png";
        auto num = std::to_string(stage);
        bg.insert(bg.length() - 4, num);

        coefficient_ = 1.0;
        addCoefficient_ = abs(addCoefficient_);
        fluffyTime_ = 0.0;
        PlaySoundMem(lpSoundMng.GetID("./sound/select.mp3")[0], DX_PLAYTYPE_BACK);
    }

    nowStage_ = static_cast<STAGE>(stage);
}
