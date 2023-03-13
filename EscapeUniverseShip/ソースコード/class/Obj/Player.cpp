#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "Player.h"
#include"../../tmx/TmxObj.h"
#include"../Camera/Camera.h"
#include"../Other/ImageMng.h"
#include"../Other/SoundMng.h"
#include"../Other/EffectMng.h"
#include"../Input/KeyInput.h"
#include"../Input/PadInput.h"
#include"../SceneManager.h"
#include"../BaseScene.h"
#include"CheckPoint.h"
#include"../../_debug/_DebugConOut.h"
#include"../../_debug/_DebugDispOut.h"
#include "../TutorialScene.h"

//自機の基礎スピード
constexpr float MOVE_POWER = 4.0f;

//加速度
constexpr float ADD_ACCEL = 0.15f;
constexpr double INIT_ACCEL = -12.5;

//３分の２
constexpr float TWO_DIV_THREE = 2.0f / 3.0f;

//４分の３
constexpr float THREE_DIV_FOUR = 3.0f / 4.0f;

//普通の大きさの時のタイヤの加算値
constexpr float NORMAL_OFFSET_WHEEL_RIGHT_X = 36.0f;
constexpr float NORMAL_OFFSET_WHEEL_LEFT_X = -45.0f;
constexpr float NORMAL_OFFSET_WHEEL_Y = 19.0f;

//車関係
constexpr float CAR_SIZE_X = 144.0f;    //サイズｘ
constexpr float CAR_SIZE_Y = 52.0f;     //サイズｙ
constexpr float RESPAWN_POS_X = 256.0f; //リスポーン地点ｘ
constexpr float RESPAWN_POS_Y = 665.0f; //リスポーン地点ｙ
constexpr float INIT_POS_X = -100.0f;   //初期地点ｘ
constexpr float INIT_POS_Y = 665.0f;    //初期地点ｙ

//着地エフェクトのオフセット
constexpr float EFFECT_OFFSET = 45.0f;

//のこぎりの回転半径
constexpr float ROT_CIRCLE_LENGTH = 100.0f;

//チュートリアルのオフセット
constexpr float TUTORIAL_OFFSET = 200.0f;

Player::Player()
{
    //プレイヤー初期化
    carPos_ = { -100.0f,665.0f };
    accel_ = 0.0f;
    radius_ = 20.0f;
    angle_ = 0.0;
    inversionFlag_ = false;
    inversionFloat_ = 1.0f;
    sizeCon_ = 1.0f;
    nowSize_ = PLAYER_SIZE::Normal;
    nowGravity = GRAVITY::Below;
    nowState_ = STATE::None;
    fallTime_ = 0.0;
    goalPos_ = 0.0f;
    centerFlag_ = false;
    clearFlag_ = false;
    delta_ = 0.0;
    plusRot_ = 0.0f;
    rotAngle_ = 0.0f;
}

Player::Player(int cnttype,float plus)
{
    plusRot_ = plus;
    Init(cnttype);
}

Player::~Player()
{
}

bool Player::Init(int cnttype)
{
    if (cnttype == 0)
    {
        //キーボードの場合
        controller_ = std::make_unique<KeyInput>();
    }
    else
    {
        //パッドの場合
        controller_ = std::make_unique<PadInput>();
    }
    respawnPos_ = { RESPAWN_POS_X, RESPAWN_POS_Y };
    carPos_ = respawnPos_;
    clearFlag_ = false;
    centerFlag_ = false;
    checkPos_ = { 0.0f,0.0f };
    rotAngle_ = 0.0f;
    SetSizeEffect();
    StopEffect();
    InitPlayer();
    return true;
}

void Player::InitPlayer(void)
{
    //プレイヤー初期化
    accel_ = 0.0f;
    delta_ = 0.0;
    radius_ = 20.0f;
    angle_ = 0.0;
    inversionFlag_ = false;
    inversionFloat_ = 1.0f;
    sizeCon_ = 1.0f;
    nowSize_ = PLAYER_SIZE::Normal;
    nowGravity = GRAVITY::Below;
    nowState_ = STATE::None;
    fallTime_ = 0.0;
}

void Player::ChangePlayerGravity(bool invflag,float invfloat,GRAVITY gravity)
{
    //プレイヤーの重力変更・リセット
    inversionFlag_ = invflag;
    inversionFloat_ = invfloat;
    nowGravity = gravity;
    nowState_ = STATE::Fall;
    accel_ = 0.0f;
    PlaySoundMem(lpSoundMng.GetID("./sound/gravity.mp3")[0], DX_PLAYTYPE_BACK);
    lpEffMng.SetPlayID("./Effect/inversion.efkefc");
}

SCENE Player::Update(TmxObj& tmx, Vector2 offset, std::shared_ptr<CheckPoint> check)
{
    //コントローラ
    controller_->Update();
    SetEffectPos(offset);
    UpdateEffekseer2D();

    auto usedController = controller_->GetControllerData();
    if (usedController[KeyID::Pause][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Pause][static_cast<int>(ControllerAge::Old)])
    {
        StopEffect();
        return SCENE::PAUSE;
    }

    SCENE scene = SCENE::GAME;

    //プレイヤーの移動
    if (nowState_ != STATE::Death)
    {
        //プレイヤーの状態変化
        if (!clearFlag_)
        {
            ChangePlayerInfo(tmx, offset);
        }
        bool overFlag = MovePlayer(tmx, offset);
        if (!overFlag)
        {
            if (respawnPos_.x == 256.0f)
            {
                Vector2 halfCarSize = { CAR_SIZE_X / 2.0f,CAR_SIZE_Y / 2.0f };
                if (check != nullptr)
                {
                    CheckCheckPoint(tmx, offset, halfCarSize, check);
                }
            }
        }
        else
        {
            deathCar_.pos = carPos_;
            deathLeft_.pos = carPos_ + Vector2{ NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_, NORMAL_OFFSET_WHEEL_Y * sizeCon_ };
            deathRight_.pos = carPos_ + Vector2{ NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_, NORMAL_OFFSET_WHEEL_Y * sizeCon_ };
            nowState_ = STATE::Death;
            PlaySoundMem(lpSoundMng.GetID("./sound/coll.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/explosion.efkefc");
        }
    }
    else
    {
        scene = MoveDeathMode(tmx, offset);
        if (scene == SCENE::OVER)
        {
            StopEffect();
            UpdateEffekseer2D();
        }
    }

    //ゴール地点に到達した場合
    if (carPos_.x >= goalPos_ - lpSceneMng.GetViewSize().x && !clearFlag_)
    {
        StopSound();
        clearFlag_ = true;
        accel_ = 0.0f;
        
        MoveClearMode(tmx, offset);
    }

    if (clearFlag_)
    {
        if (carPos_.x >= goalPos_ + lpSceneMng.GetViewSize().x)
        {
            InitPlayer();
            return SCENE::CLEAR;
        }
        if (carPos_.x >= goalPos_ - lpSceneMng.GetViewSize().x / 2.0f && !centerFlag_)
        {
            carPos_.x -= MOVE_POWER;
            delta_ += lpSceneMng.GetDeltaTime();
        }
        else
        {
            carPos_.x -= MOVE_POWER / 4.0f;
        }
        //画面真ん中にいって０．５秒後に右に移動
        if (delta_ > 0.5 && !centerFlag_)
        {
            lpEffMng.SetPlayID("./Effect/acceleration.efkefc");
            PlaySoundMem(lpSoundMng.GetID("./sound/dash.mp3")[0], DX_PLAYTYPE_BACK);
            delta_ = 0.0;
            centerFlag_ = true;
        }

        //ｘ軸を使用しての鉛直投げ上げ
        if (centerFlag_)
        {
            delta_ += lpSceneMng.GetDeltaTime();
            accel_ += 1.0f;
            auto addPos = static_cast<float>((INIT_ACCEL) + ((1.0 / 2.0) * (accel_ * (std::pow(delta_, 2.0)))));
            carPos_.x += addPos;
        }
    }

    angle_ += 1.0 * inversionFloat_;
    rotAngle_ += plusRot_;

    return scene;
}

SCENE Player::UpdateTutorial(TmxObj& tmx, Vector2 offset, std::shared_ptr<CheckPoint> check)
{
    //コントローラ
    controller_->Update();
    SetEffectPos(offset);
    UpdateEffekseer2D();

    SCENE scene = SCENE::GAME;

    //プレイヤーの移動
    if (nowState_ != STATE::Death)
    {
        //プレイヤーの状態変化
        bool overFlag = MovePlayer(tmx, offset);
        if (!overFlag)
        {
            Vector2 halfCarSize = { CAR_SIZE_X / 2.0f,CAR_SIZE_Y / 2.0f };
        }
        else
        {
            deathCar_.pos = carPos_;
            deathLeft_.pos = carPos_ + Vector2{ NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_, NORMAL_OFFSET_WHEEL_Y * sizeCon_ };
            deathRight_.pos = carPos_ + Vector2{ NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_, NORMAL_OFFSET_WHEEL_Y * sizeCon_ };
            nowState_ = STATE::Death;
            StopEffect();
        }
    }
    else
    {
        scene = MoveDeathMode(tmx, offset);
    }

    //画面外に出たとき
    if (carPos_.x >= goalPos_ + TUTORIAL_OFFSET)
    {
        InitPlayer();
        return SCENE::CLEAR;
    }

    angle_ += 1.0 * inversionFloat_;

    return scene;
}

void Player::MoveRight(Vector2 offset, float con)
{
    carPos_.x += MOVE_POWER * con;
}

void Player::Draw(Vector2 offset)
{
    DrawEffekseer2D();
    //右タイヤの合計加算値Ｘ
    auto AddWheelRX = NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_;
    //左タイヤの合計加算値Ｘ
    auto AddWheelLX = NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_;
    //タイヤの合計加算値Ｙ
    auto AddWheelRY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;
    //タイヤの合計加算値Ｙ
    auto AddWheelLY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;

    if (nowState_ != STATE::Death)
    {
        //自機
        //右のタイヤ
        DrawRotaGraphF(carPos_.x + AddWheelRX - offset.x,
            carPos_.y + AddWheelRY * inversionFloat_ - offset.y,
            static_cast<double>(sizeCon_), angle_, lpImageMng.GetID("./Image/Wheel.png")[0], true);
        //左のタイヤ
        DrawRotaGraphF(carPos_.x + AddWheelLX - offset.x,
            carPos_.y + AddWheelLY * inversionFloat_ - offset.y,
            static_cast<double>(sizeCon_), angle_, lpImageMng.GetID("./Image/Wheel.png")[0], true);
        //車体
        DrawRotaGraphF(carPos_.x - offset.x,
            carPos_.y - offset.y,
            static_cast<double>(sizeCon_), 0.0, lpImageMng.GetID("./Image/Car.png")[0], true, false, inversionFlag_);
    }
    else
    {
        //自機
        //右のタイヤ
        DrawRotaGraphF(deathRight_.pos.x - offset.x,
            deathRight_.pos.y - offset.y,
            sizeCon_, deathRight_.angle / 1.5, lpImageMng.GetID("./Image/Wheel.png")[0], true);
        //左のタイヤ
        DrawRotaGraphF(deathLeft_.pos.x - offset.x,
            deathLeft_.pos.y - offset.y,
            static_cast<double>(sizeCon_), deathLeft_.angle / 1.5, lpImageMng.GetID("./Image/Wheel.png")[0], true);
        //車体
        DrawRotaGraphF(deathCar_.pos.x - offset.x,
            deathCar_.pos.y - offset.y,
            static_cast<double>(sizeCon_), deathCar_.angle / 1.5, lpImageMng.GetID("./Image/Car.png")[0], true, false, inversionFlag_);
    }
}

void Player::Release(void)
{
}

Vector2 Player::GetPos(void)
{
    return carPos_;
}

void Player::SetPos(Vector2 pos)
{
    carPos_ = pos;
}

void Player::SetGoalPos(float pos)
{
    goalPos_ = pos;
}

bool Player::TutorialMove(TmxObj& tmx, Vector2 offset,int nowButton)
{
    //コントローラ
    controller_->Update();
    auto usedController = controller_->GetControllerData();

    bool pressFlag = false;

    auto tutorialNum = static_cast<TUTORIAL_NUM>(nowButton);

    switch (tutorialNum)
    {
    case TUTORIAL_NUM::One:
        if (usedController[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Up][static_cast<int>(ControllerAge::Old)])
        {
            pressFlag = true;
            if (nowGravity == GRAVITY::Below)
            {
                ChangePlayerGravity(true, -1.0f, GRAVITY::On);
            }
        }
        break;
    case TUTORIAL_NUM::Two:
        if (usedController[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Down][static_cast<int>(ControllerAge::Old)])
        {
            pressFlag = true;
            if (nowGravity == GRAVITY::On)
            {
                ChangePlayerGravity(false, 1.0f, GRAVITY::Below);
            }
        }
        break;
    case TUTORIAL_NUM::Three:

        if (usedController[KeyID::Left][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Left][static_cast<int>(ControllerAge::Old)])
        {
            pressFlag = true;
            if (nowSize_ == PLAYER_SIZE::Normal)
            {
                nowState_ = STATE::Fall;
                nowSize_ = PLAYER_SIZE::Small;
                radius_ /= 2.0f;
                sizeCon_ /= 2.0f;
                PlaySoundMem(lpSoundMng.GetID("./sound/small.mp3")[0], DX_PLAYTYPE_BACK);
                lpEffMng.SetPlayID("./Effect/SmallSize.efkefc");
            }
            else if (nowSize_ == PLAYER_SIZE::Big)
            {
                nowState_ = STATE::Fall;
                nowSize_ = PLAYER_SIZE::Normal;
                radius_ /= 2.0f;
                sizeCon_ /= 2.0f;
                if (nowGravity == GRAVITY::Below)
                {
                    carPos_.y += 20.0f;
                }
                else
                {
                    carPos_.y -= 20.0f;
                }
                PlaySoundMem(lpSoundMng.GetID("./sound/small.mp3")[0], DX_PLAYTYPE_BACK);
                lpEffMng.SetPlayID("./Effect/SmallSize.efkefc");
            }

        }

        break;
    case TUTORIAL_NUM::Four:
    {
        UpdateEffekseer2D();
        float Correct = 0.0f;
        auto CheckCorrectCircle = [&](Vector2 wheelpos) {
            Vector2 ray = wheelpos - offset;
            //_dbgDrawCircle(static_cast<int>(ray.x), static_cast<int>(ray.y), static_cast<int>(radius_), 0xff0000, false);
            for (auto coll : tmx.GetCollList())
            {
                if (nowGravity == GRAVITY::Below)
                {
                    if (raycast_.CheckCollisionCircleUpDown(ray, coll, offset, radius_, Correct))
                    {
                        return true;
                    }
                }
                else
                {
                    if (raycast_.CheckCollisionCircleDownUp(ray, coll, offset, radius_, Correct))
                    {
                        return true;
                    }
                }
            }
            return false;
        };
        if (usedController[KeyID::Right][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Right][static_cast<int>(ControllerAge::Old)])
        {
            if (nowSize_ == PLAYER_SIZE::Normal)
            {
                nowState_ = STATE::Fall;
                nowSize_ = PLAYER_SIZE::Big;
                radius_ *= 2.0f;
                sizeCon_ *= 2.0f;
                PlaySoundMem(lpSoundMng.GetID("./sound/big.mp3")[0], DX_PLAYTYPE_BACK);
                lpEffMng.SetPlayID("./Effect/BigSize.efkefc");
            }
            else if (nowSize_ == PLAYER_SIZE::Small)
            {
                nowState_ = STATE::Fall;
                nowSize_ = PLAYER_SIZE::Normal;
                radius_ *= 2.0f;
                sizeCon_ *= 2.0f;
                PlaySoundMem(lpSoundMng.GetID("./sound/big.mp3")[0], DX_PLAYTYPE_BACK);
            }

            //右タイヤの合計加算値Ｘ
            auto AddWheelRX = NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_;
            //左タイヤの合計加算値Ｘ
            auto AddWheelLX = NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_;
            //タイヤの合計加算値Ｙ
            auto AddWheelY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;

            //それぞれのタイヤの位置
            Vector2 belowRightWheel = { carPos_.x + AddWheelRX,carPos_.y + AddWheelY };
            Vector2 belowLeftWheel = { carPos_.x + AddWheelLX,carPos_.y + AddWheelY };
            Vector2 onRightWheel = { carPos_.x + AddWheelRX,carPos_.y - AddWheelY };
            Vector2 onLeftWheel = { carPos_.x + AddWheelLX,carPos_.y - AddWheelY };

            //大きくしたときのめりこみ補正
            if (nowGravity == GRAVITY::On)
            {
                if (CheckCorrectCircle(onRightWheel) || CheckCorrectCircle(onLeftWheel))
                {
                    carPos_.y += Correct;
                }
            }
            else
            {
                if (CheckCorrectCircle(belowRightWheel) || CheckCorrectCircle(belowLeftWheel))
                {
                    carPos_.y -= Correct;
                }
            }
        }
        if (nowSize_ == PLAYER_SIZE::Big)
        {
            pressFlag = true;
        }
    }
        break;
    case TUTORIAL_NUM::Five:
        if (usedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Down][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Right][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Right][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Left][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Left][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Up][static_cast<int>(ControllerAge::Old)]
            )
        {
            pressFlag = true;
        }
        break;
    case TUTORIAL_NUM::Six:
        if (usedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Down][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Right][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Right][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Left][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Left][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Up][static_cast<int>(ControllerAge::Old)]
            )
        {
            pressFlag = true;
        }
    case TUTORIAL_NUM::Seven:
        if (usedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Down][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Right][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Right][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Left][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Left][static_cast<int>(ControllerAge::Old)]
            || usedController[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Up][static_cast<int>(ControllerAge::Old)]
            )
        {
            pressFlag = true;
        }
    default:
        break;
    }
    return pressFlag;
}

void Player::ChangePlayerInfo(TmxObj& tmx, Vector2 offset)
{
    auto controllerData = controller_->GetControllerData();
    bool gravityFlag = false;
    //上入力した場合
    if (controllerData[KeyID::Up][static_cast<int>(ControllerAge::Now)] &&!controllerData[KeyID::Up][static_cast<int>(ControllerAge::Old)])
    {
        if (nowGravity == GRAVITY::Below)
        {
            ChangePlayerGravity(true, -1.0f, GRAVITY::On);
        }
    }
    else if (controllerData[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Down][static_cast<int>(ControllerAge::Old)])
    {
        if (nowGravity == GRAVITY::On)
        {
            ChangePlayerGravity(false, 1.0f, GRAVITY::Below);
        }
    }

    if (controllerData[KeyID::Button][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Button][static_cast<int>(ControllerAge::Old)])
    {
        gravityFlag = true;
    }

    if (gravityFlag)
    {
        if (nowGravity == GRAVITY::Below)
        {
            ChangePlayerGravity(true, -1.0f, GRAVITY::On);
        }
        else if (nowGravity == GRAVITY::On)
        {
            ChangePlayerGravity(false, 1.0f, GRAVITY::Below);
        }
    }

    float correct = 0.0f;

    auto CheckCorrectCircle = [&](Vector2 wheelpos) {
        Vector2 ray = wheelpos - offset;
        for (const auto& coll : tmx.GetCollList())
        {
            if (nowGravity == GRAVITY::Below)
            {
                if (raycast_.CheckCollisionCircleUpDown(ray, coll, offset, radius_, correct))
                {
                    return true;
                }
            }
            else 
            {
                if (raycast_.CheckCollisionCircleDownUp(ray, coll, offset, radius_, correct))
                {
                    return true;
                }
            }
        }
        return false;
    };

    if ((controllerData[KeyID::Left][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Left][static_cast<int>(ControllerAge::Old)]))
    {
        if (nowSize_ == PLAYER_SIZE::Normal)
        {
            nowState_ = STATE::Fall;
            nowSize_ = PLAYER_SIZE::Small;
            radius_ /= 2.0f;
            sizeCon_ /= 2.0f;
            PlaySoundMem(lpSoundMng.GetID("./sound/small.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/SmallSize.efkefc");
        }
        else if (nowSize_ == PLAYER_SIZE::Big)
        {
            nowState_ = STATE::Fall;
            nowSize_ = PLAYER_SIZE::Normal;
            radius_ /= 2.0f;
            sizeCon_ /= 2.0f;
            if (nowGravity == GRAVITY::Below)
            {
                carPos_.y += 20.0f;
            }
            else
            {
                carPos_.y -= 20.0f;
            }
            PlaySoundMem(lpSoundMng.GetID("./sound/small.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/SmallSize.efkefc");
        }
    }
    if ((controllerData[KeyID::Right][static_cast<int>(ControllerAge::Now)] && !controllerData[KeyID::Right][static_cast<int>(ControllerAge::Old)]))
    {
        if (nowSize_ == PLAYER_SIZE::Normal)
        {
            nowState_ = STATE::Fall; 
            nowSize_ = PLAYER_SIZE::Big;
            radius_ *= 2.0f;
            sizeCon_ *= 2.0f;
            PlaySoundMem(lpSoundMng.GetID("./sound/big.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/BigSize.efkefc");
        }
        else if (nowSize_ == PLAYER_SIZE::Small)
        {
            nowState_ = STATE::Fall;
            nowSize_ = PLAYER_SIZE::Normal;
            radius_ *= 2.0f;
            sizeCon_ *= 2.0f;
            PlaySoundMem(lpSoundMng.GetID("./sound/big.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/BigSize.efkefc");
        }

        //右タイヤの合計加算値Ｘ
        auto addWheelRX = NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_;
        //左タイヤの合計加算値Ｘ
        auto addWheelLX = NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_;
        //タイヤの合計加算値Ｙ
        auto addWheelY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;

        //それぞれのタイヤの位置
        Vector2 belowRightWheel = { carPos_.x + addWheelRX,carPos_.y + addWheelY };
        Vector2 belowLeftWheel = { carPos_.x + addWheelLX,carPos_.y + addWheelY };
        Vector2 onRightWheel = { carPos_.x + addWheelRX,carPos_.y - addWheelY };
        Vector2 onLeftWheel = { carPos_.x + addWheelLX,carPos_.y - addWheelY };

        //大きくしたときのめりこみ補正
        if (nowGravity == GRAVITY::On)
        {
            if (CheckCorrectCircle(onRightWheel) || CheckCorrectCircle(onLeftWheel))
            {
                //上が重力の場合、上の線で補正してしまっているから修正必
                carPos_.y += correct;
            }
        }
        else
        {
            if (CheckCorrectCircle(belowRightWheel) || CheckCorrectCircle(belowLeftWheel))
            {
                carPos_.y -= correct;
            }
        }
    }
}

bool Player::MovePlayer(TmxObj& tmx, Vector2 offset)
{
    //右方向に自動進行
    carPos_.x += MOVE_POWER;

    //めり込みの補正値
    float correct = 0.0f;

    //右タイヤの合計加算値Ｘ
    auto addWheelRX = NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_;
    //左タイヤの合計加算値Ｘ
    auto addWheelLX = NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_;
    //タイヤの合計加算値Ｙ
    auto addWheelY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;

    //スピードと加速の合計
    auto totalSpeed = nowState_ == STATE::Fall ? MOVE_POWER + accel_ : 0.0f;
    totalSpeed = nowGravity == GRAVITY::Below ? totalSpeed : -totalSpeed;

    auto CheckCorrectCircle = [&](Vector2 wheelpos) {
        Vector2 ray = wheelpos - offset;
        for (const auto& coll : tmx.GetCollList())
        {
            if (raycast_.CheckCollisionCircleUpDown(ray, coll, offset, radius_, correct))
            {
                return true;
            }
        }
        return false;
    };
    auto checkDrumCircleHitFlag = [&](Vector2 wheelpos) {
        Vector2 ray = wheelpos - offset;
        for (auto& coll : tmx.GetCollDrum())
        {
            if (raycast_.CheckCollisionCircleUpDown(ray, coll, offset, radius_, correct))
            {
                if (coll.second)
                {
                    if (nowSize_ == PLAYER_SIZE::Big)
                    {
                        PlaySoundMem(lpSoundMng.GetID("./sound/drum.mp3")[0], DX_PLAYTYPE_BACK);
                        coll.second = false;
                        return false;
                    }
                    return true;
                }
            }
        }
        return false;
    };

    //それぞれのタイヤの位置
    Vector2 belowRightWheel = { carPos_.x + addWheelRX,carPos_.y + addWheelY };
    Vector2 belowLeftWheel = { carPos_.x + addWheelLX,carPos_.y + addWheelY };
    Vector2 onRightWheel = { carPos_.x + addWheelRX,carPos_.y - addWheelY };
    Vector2 onLeftWheel = { carPos_.x + addWheelLX,carPos_.y - addWheelY };

    //タイヤが当たってないときは落下の状態にする
    if (nowState_ == STATE::None)
    {
        if (nowGravity == GRAVITY::Below)
        {
            if (!CheckCorrectCircle(belowRightWheel) && !CheckCorrectCircle(belowLeftWheel)&&
                !checkDrumCircleHitFlag(belowRightWheel) && !checkDrumCircleHitFlag(belowLeftWheel))
            {
                nowState_ = STATE::Fall;
            }
        }
        else if (nowGravity == GRAVITY::On)
        {
            if (!CheckCorrectCircle(onRightWheel) && !CheckCorrectCircle(onLeftWheel)&&
                !checkDrumCircleHitFlag(onRightWheel) && !checkDrumCircleHitFlag(onLeftWheel))
            {
                nowState_ = STATE::Fall;
            }
        }
    }
    else if (nowState_ == STATE::Fall)
    {
        fallTime_ += lpSceneMng.GetDeltaTime();
        //落下中
        if (nowGravity == GRAVITY::Below)
        {
            //重力が下の場合
            carPos_.y += totalSpeed;
            belowRightWheel = { carPos_.x + addWheelRX,carPos_.y + addWheelY };
            belowLeftWheel = { carPos_.x + addWheelLX,carPos_.y + addWheelY };
            //めり込みの補正
            if (CheckCorrectCircle(belowRightWheel) || CheckCorrectCircle(belowLeftWheel)||
                checkDrumCircleHitFlag(belowRightWheel) || checkDrumCircleHitFlag(belowLeftWheel))
            {
                nowState_ = STATE::None;
                accel_ = 0.0f;
                carPos_.y -= correct;
                if (fallTime_ > 0.1)
                {
                    PlaySoundMem(lpSoundMng.GetID("./sound/fall.mp3")[0], DX_PLAYTYPE_BACK);
                    lpEffMng.SetPlayID("./Effect/smoke.efkefc");
                }
                fallTime_ = 0.0;
            }
        }
        else if (nowGravity == GRAVITY::On)
        {
            //重力が上の場合
            carPos_.y += totalSpeed;
            onRightWheel = { carPos_.x + addWheelRX,carPos_.y - addWheelY };
            onLeftWheel = { carPos_.x + addWheelLX,carPos_.y - addWheelY };
            //めり込みの補正
            if (CheckCorrectCircle(onRightWheel) || CheckCorrectCircle(onLeftWheel)||
                checkDrumCircleHitFlag(onRightWheel) || checkDrumCircleHitFlag(onLeftWheel))
            {
                nowState_ = STATE::None;
                accel_ = 0.0f;
                carPos_.y += correct;
                if (fallTime_ > 0.1)
                {
                    PlaySoundMem(lpSoundMng.GetID("./sound/fall.mp3")[0], DX_PLAYTYPE_BACK);
                    lpEffMng.SetPlayID("./Effect/smokeRever.efkefc");
                }
                fallTime_ = 0.0;
            }
        }
        accel_ += ADD_ACCEL;
    }

    return CheckGameOver(tmx, offset);
}

SCENE Player::MoveDeathMode(TmxObj& tmx, Vector2 offset)
{
    auto addWheelY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;
    
    //車体の挙動
    if (deathCar_.initVel == 0.0f)
    {
        deathCar_.initVel = static_cast<float>(GetRand(30)) + 0.01f;
        deathRight_.initVel = static_cast<float>(GetRand(15)) + 0.01f;
        deathLeft_.initVel = static_cast<float>(GetRand(15)) + 0.01f;
    }

    if (deathCar_.movePowX == 0.0f)
    {
        deathCar_.movePowX = static_cast<float>(GetRand(20)) - 5.01f;
        deathRight_.movePowX = static_cast<float>(GetRand(10)) - 5.01f;
        deathLeft_.movePowX = static_cast<float>(GetRand(10)) - 5.01f;
    }

    deathCar_.delta += lpSceneMng.GetDeltaTime();
    deathCar_.pos.y += static_cast<float>((-deathCar_.initVel) + ((1.0 / 2.0) * (deathCar_.accel * (std::pow(deathCar_.delta, 2.0)))));
    deathCar_.accel += 1.0;
    deathCar_.angle += 0.5;

    //右ホイールの挙動
    deathRight_.delta += lpSceneMng.GetDeltaTime();
    deathRight_.pos.x += deathRight_.movePowX;
    deathRight_.accel += 1.0f;
    deathRight_.angle += 0.25;
    if (deathRight_.pos.y < 704.0f - addWheelY)
    {
        deathRight_.pos.y += static_cast<float>((-deathRight_.initVel) + ((1.0 / 2.0) * (deathRight_.accel * (std::pow(deathRight_.delta, 2.0)))));
    }

    if(deathRight_.pos.y > 704.0f - addWheelY)
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/fall.mp3")[0], DX_PLAYTYPE_BACK);
        deathRight_.pos.y = 704.0f - addWheelY;
    }

    //左ホイールの挙動
    deathLeft_.delta += lpSceneMng.GetDeltaTime();
    deathLeft_.pos.x += deathLeft_.movePowX;
    deathLeft_.accel += 1.0;
    deathLeft_.angle += 0.15;
    if (deathLeft_.pos.y < 704.0f - addWheelY)
    {
        deathLeft_.pos.y += static_cast<float>((-deathLeft_.initVel) + ((1.0 / 2.0) * (deathLeft_.accel * (std::pow(deathLeft_.delta, 2.0)))));
    }

    if(deathLeft_.pos.y > 704.0f - addWheelY)
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/fall.mp3")[0], DX_PLAYTYPE_BACK);
        deathLeft_.pos.y = 704.0f - addWheelY;
    }

    auto usedController = controller_->GetControllerData();

    //上入力した場合
    if (usedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !usedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        deathCar_.pos.y = lpSceneMng.GetViewSize().y;
    }

    //初期化
    if (deathCar_.pos.y >= lpSceneMng.GetViewSize().y)
    {
        InitPlayer();
        nowState_ = STATE::None;
        carPos_ = respawnPos_;

        //加速度
        deathCar_.accel = 0.0;
        deathRight_.accel = 0.0;
        deathLeft_.accel = 0.0;

        //デルタ
        deathCar_.delta = lpSceneMng.GetDeltaTime();
        deathRight_.delta = lpSceneMng.GetDeltaTime();
        deathLeft_.delta = lpSceneMng.GetDeltaTime();

        //初速度
        deathCar_.initVel = 0.0f;
        deathRight_.initVel = 0.0f;
        deathLeft_.initVel = 0.0f;

        //横移動
        deathCar_.movePowX = 0.0f;
        deathRight_.movePowX = 0.0f;
        deathLeft_.movePowX = 0.0f;

        return SCENE::OVER;
    }

    return SCENE::GAME;
}

void Player::MoveClearMode(TmxObj& tmx, Vector2 offset)
{
    if (nowGravity == GRAVITY::On)
    {
        ChangePlayerGravity(false, 1.0f, GRAVITY::Below);
    }
    //めりこみ補正
    float correct = 0.0f;
    auto CheckCorrectCircle = [&](Vector2 wheelpos) {
        Vector2 ray = wheelpos - offset;
        for (const auto& coll : tmx.GetCollList())
        {
            if (nowGravity == GRAVITY::Below)
            {
                if (raycast_.CheckCollisionCircleUpDown(ray, coll, offset, radius_, correct))
                {
                    return true;
                }
            }
            else
            {
                if (raycast_.CheckCollisionCircleDownUp(ray, coll, offset, radius_, correct))
                {
                    return true;
                }
            }
        }
        return false;
    };
    if (nowSize_ != PLAYER_SIZE::Normal)
    {
        if (nowSize_ == PLAYER_SIZE::Big)
        {
            nowState_ = STATE::Fall;
            nowSize_ = PLAYER_SIZE::Normal;
            radius_ /= 2.0f;
            sizeCon_ /= 2.0f;
            if (nowGravity == GRAVITY::Below)
            {
                carPos_.y += 20.0f;
            }
            else
            {
                carPos_.y -= 20.0f;
            }
            PlaySoundMem(lpSoundMng.GetID("./sound/small.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/SmallSize.efkefc");
        }
        else if (nowSize_ == PLAYER_SIZE::Small)
        {
            nowState_ = STATE::Fall;
            nowSize_ = PLAYER_SIZE::Normal;
            radius_ *= 2.0f;
            sizeCon_ *= 2.0f;
            PlaySoundMem(lpSoundMng.GetID("./sound/big.mp3")[0], DX_PLAYTYPE_BACK);
            lpEffMng.SetPlayID("./Effect/BigSize.efkefc");
        }

        //右タイヤの合計加算値Ｘ
        auto addWheelRX = NORMAL_OFFSET_WHEEL_RIGHT_X * sizeCon_;
        //左タイヤの合計加算値Ｘ
        auto addWheelLX = NORMAL_OFFSET_WHEEL_LEFT_X * sizeCon_;
        //タイヤの合計加算値Ｙ
        auto addWheelY = NORMAL_OFFSET_WHEEL_Y * sizeCon_;

        //それぞれのタイヤの位置
        Vector2 belowRightWheel = { carPos_.x + addWheelRX,carPos_.y + addWheelY };
        Vector2 belowLeftWheel = { carPos_.x + addWheelLX,carPos_.y + addWheelY };
        Vector2 onRightWheel = { carPos_.x + addWheelRX,carPos_.y - addWheelY };
        Vector2 onLeftWheel = { carPos_.x + addWheelLX,carPos_.y - addWheelY };

        //大きくしたときのめりこみ補正
        if (nowGravity == GRAVITY::On)
        {
            if (CheckCorrectCircle(onRightWheel) || CheckCorrectCircle(onLeftWheel))
            {
                carPos_.y += correct;
            }
        }
        else
        {
            if (CheckCorrectCircle(belowRightWheel) || CheckCorrectCircle(belowLeftWheel))
            {
                carPos_.y -= correct;
            }
        }
    }
}

bool Player::CheckGameOver(TmxObj& tmx, Vector2 offset)
{
    //車体のサイズの半分
    Vector2 halfCarSize = { CAR_SIZE_X / 2.0f,CAR_SIZE_Y / 2.0f };
    bool gameOverFlag = false;
    //車体と足場の判定
    if (!gameOverFlag)
    {
        gameOverFlag = CheckBodyScaff(tmx, offset, halfCarSize);
    }
    if (!gameOverFlag)
    {
        gameOverFlag = CheckBodySaw(tmx, offset, halfCarSize);
    }
    if (!gameOverFlag)
    {
        gameOverFlag = CheckBodyDrum(tmx, offset, halfCarSize);
    }
    if (!gameOverFlag)
    {
        gameOverFlag = CheckBodyRotSaw(tmx, offset, halfCarSize);
    }
    return gameOverFlag;
}

bool Player::CheckBodyScaff(TmxObj& tmx, Vector2 offset, Vector2 half)
{
    //車体と足場の判定
    auto CheckMoveRect = [&](Vector2 origin, Vector2 moveVec) {
        Raycast::Ray ray = { carPos_ + origin * sizeCon_ - offset,moveVec * sizeCon_ };
        for (const auto& coll : tmx.GetCollList())
        {
            if (raycast_.CheckCollisionRay(ray, coll, offset))
            {
                return true;
            }
        }
        return false;
    };

    //当たっているかフラグ
    bool hitFlag = false;

    //下
    if (CheckMoveRect({ half.x * THREE_DIV_FOUR,half.y * THREE_DIV_FOUR }, { -CAR_SIZE_X * THREE_DIV_FOUR,0.0f })&&!hitFlag)
    {
        hitFlag = true;
    }
    //左
    if (CheckMoveRect({ -half.x * THREE_DIV_FOUR,-half.y * THREE_DIV_FOUR + 5.0f }, { 0.0f,CAR_SIZE_Y * THREE_DIV_FOUR - 5.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //右
    if (CheckMoveRect({ half.x * THREE_DIV_FOUR,half.y * THREE_DIV_FOUR }, { 0.0f,-CAR_SIZE_Y * THREE_DIV_FOUR + 5.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //上
    if (CheckMoveRect({ -half.x * THREE_DIV_FOUR,-half.y * THREE_DIV_FOUR + 5.0f }, { CAR_SIZE_X * THREE_DIV_FOUR,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }

    //当たっていた場合リザルトへ。中間ポイント突破していたら半分くらいからスタート
    if (hitFlag)
    {
        return true;
    }
    return false;
}

bool Player::CheckBodySaw(TmxObj& tmx, Vector2 offset, Vector2 half)
{
    //車体と鋸の判定
    auto CheckRectTrap = [&](Vector2 origin, Vector2 moveVec) {
        Raycast::Ray ray = { carPos_ + origin * sizeCon_ - offset,moveVec * sizeCon_ };
        for (const auto& coll : tmx.GetCollTrap())
        {
            if (raycast_.CheckCollisionCircleAll(ray, coll.first, offset, coll.second.x))
            {
                return true;
            }
        }
        return false;
    };

    bool hitFlag = false;
    if (CheckRectTrap({ half.x * THREE_DIV_FOUR,half.y * THREE_DIV_FOUR }, { -CAR_SIZE_X * THREE_DIV_FOUR,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //左
    if (CheckRectTrap({ -half.x * THREE_DIV_FOUR,-half.y * THREE_DIV_FOUR + 5.0f }, { 0.0f,CAR_SIZE_Y * THREE_DIV_FOUR - 5.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //右
    if (CheckRectTrap({ half.x * THREE_DIV_FOUR,half.y * THREE_DIV_FOUR }, { 0.0f,-CAR_SIZE_Y * THREE_DIV_FOUR + 5.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //上
    if (CheckRectTrap({ -half.x * THREE_DIV_FOUR,-half.y * THREE_DIV_FOUR + 5.0f }, { CAR_SIZE_X * THREE_DIV_FOUR,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    if (hitFlag)
    {
        return true;
    }
    return false;
}

bool Player::CheckBodyDrum(TmxObj& tmx, Vector2 offset, Vector2 half)
{
    auto CheckDrum = [&](Vector2 origin, Vector2 moveVec) {
        Raycast::Ray ray = { carPos_ + origin * sizeCon_ - offset,moveVec * sizeCon_ };
        for (auto& coll : tmx.GetCollDrum())
        {
            if (coll.second)
            {
                if (raycast_.CheckCollisionRayL(ray, coll, offset))
                {
                    if (nowSize_ == PLAYER_SIZE::Big)
                    {
                        PlaySoundMem(lpSoundMng.GetID("./sound/drum.mp3")[0], DX_PLAYTYPE_BACK);
                        coll.second = false;
                        return false;
                    }
                    return true;
                }
            }
        }
        return false;
    };

    bool flag = false;
    //下
    if (CheckDrum({ half.x - 5.0f,half.y * THREE_DIV_FOUR }, { -CAR_SIZE_X + 5.0f,0.0f }) && !flag)
    {
        flag = true;
    }
    //上
    if (CheckDrum({ -half.x,-half.y * THREE_DIV_FOUR + 5.0f }, { CAR_SIZE_X - 5.0f,0.0f }) && !flag)
    {
        flag = true;
    }

    if (flag)
    {
        return true;
    }
    return false;
}

bool Player::CheckCheckPoint(TmxObj& tmx, Vector2 offset, Vector2 half,std::shared_ptr<CheckPoint> check)
{
    auto CheckCheckPoint = [&](Vector2 origin, Vector2 moveVec) {
        Raycast::Ray ray = { carPos_ + origin * sizeCon_ - offset,moveVec * sizeCon_ };
        for (const auto& coll : tmx.GetCollCheck())
        {
            if (raycast_.CheckCollisionRay(ray, coll, offset))
            {
                PlaySoundMem(lpSoundMng.GetID("./sound/checkPoint.mp3")[0], DX_PLAYTYPE_BACK);
                lpEffMng.SetPlayID("./Effect/check.efkefc");
                checkPos_ = coll.first;
                return true;
            }
        }
        return false;
    };

    //当たっているかフラグ
    bool hitFlag = false;
    //下
    if (CheckCheckPoint({ half.x - 5.0f,half.y * THREE_DIV_FOUR }, { -CAR_SIZE_X + 5.0f,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //上
    if (CheckCheckPoint({ -half.x,-half.y * THREE_DIV_FOUR + 5.0f }, { CAR_SIZE_X - 5.0f,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }

    if (hitFlag)
    {
        for (const auto& coll : tmx.GetCollCheck())
        {
            respawnPos_ = { coll.first.x,coll.first.y + 24.0f };
        }
        check->SetFlag(hitFlag);
    }
    return hitFlag;
}

bool Player::CheckBodyRotSaw(TmxObj& tmx, Vector2 offset, Vector2 half)
{
    //車体と鋸の判定
    auto CheckRectTrap = [&](Vector2 origin, Vector2 moveVec) {
        Raycast::Ray ray = { carPos_ + origin * sizeCon_ - offset,moveVec * sizeCon_ };
        for (auto coll : tmx.GetCollRotateTrap())
        {
            float radius = rotAngle_ * 3.14f / 180.0f;
            float add_x = cos(radius) * ROT_CIRCLE_LENGTH;
            float add_y = sin(radius) * ROT_CIRCLE_LENGTH;

            Vector2 sawpos = {
                coll.first.x,
               coll.first.y };

            Vector2 sawRotpos = {
                sawpos.x ,
                sawpos.y };

            sawpos = {
                sawRotpos.x + add_x,
                sawRotpos.y + add_y };

            coll.first = sawpos;
            if (raycast_.CheckCollisionCircleAll(ray, coll.first, offset, coll.second.x))
            {
                return true;
            }
        }
        return false;
    };

    bool hitFlag = false;
    //下
    if (CheckRectTrap({ half.x * THREE_DIV_FOUR,half.y * THREE_DIV_FOUR }, { -CAR_SIZE_X * THREE_DIV_FOUR,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //左
    if (CheckRectTrap({ -half.x * THREE_DIV_FOUR,-half.y * THREE_DIV_FOUR + 5.0f }, { 0.0f,CAR_SIZE_Y * THREE_DIV_FOUR - 5.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //右
    if (CheckRectTrap({ half.x * THREE_DIV_FOUR,half.y * THREE_DIV_FOUR }, { 0.0f,-CAR_SIZE_Y * THREE_DIV_FOUR + 5.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    //上
    if (CheckRectTrap({ -half.x * THREE_DIV_FOUR,-half.y * THREE_DIV_FOUR + 5.0f }, { CAR_SIZE_X * THREE_DIV_FOUR,0.0f }) && !hitFlag)
    {
        hitFlag = true;
    }
    if (hitFlag)
    {
        return true;
    }
    return false;
}

void Player::SetSizeEffect(void)
{
    lpEffMng.SetSizeID("./Effect/BigSize.efkefc", 20.0f);
    lpEffMng.SetSizeID("./Effect/SmallSize.efkefc", 15.0f);
    lpEffMng.SetSizeID("./Effect/smoke.efkefc", 25.0f);
    lpEffMng.SetSizeID("./Effect/smokeRever.efkefc", 25.0f);
    lpEffMng.SetSizeID("./Effect/explosion.efkefc", 25.0f);
    lpEffMng.SetSizeID("./Effect/inversion.efkefc", 30.0f);
    lpEffMng.SetSizeID("./Effect/acceleration.efkefc", 20.0f);
    lpEffMng.SetSizeID("./Effect/check.efkefc", 20.0f);
}

void Player::SetPlayEffect(void)
{
    lpEffMng.SetPlayID("./Effect/BigSize.efkefc");
    lpEffMng.SetPlayID("./Effect/SmallSize.efkefc");
    lpEffMng.SetPlayID("./Effect/smoke.efkefc");
    lpEffMng.SetPlayID("./Effect/smokeRever.efkefc");
    lpEffMng.SetPlayID("./Effect/explosion.efkefc");
    lpEffMng.SetPlayID("./Effect/inversion.efkefc");
    lpEffMng.SetPlayID("./Effect/acceleration.efkefc");
    lpEffMng.SetPlayID("./Effect/check.efkefc");
}

void Player::StopEffect(void)
{
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/BigSize.efkefc", 20.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/SmallSize.efkefc", 15.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/smoke.efkefc", 25.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/smokeRever.efkefc", 25.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/explosion.efkefc", 25.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/inversion.efkefc", 30.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/acceleration.efkefc", 20.0f)[0]);
    StopEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/check.efkefc", 20.0f)[0]);
}

void Player::SetEffectPos(Vector2 offset)
{
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/BigSize.efkefc", 20.0f)[0], carPos_.x - offset.x, carPos_.y - offset.y, 0);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/SmallSize.efkefc", 15.0f)[0], carPos_.x - offset.x, carPos_.y - offset.y, 0);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/smoke.efkefc", 25.0f)[0], carPos_.x - offset.x, carPos_.y + (EFFECT_OFFSET * sizeCon_) - offset.y,1.0f);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/smokeRever.efkefc", 25.0f)[0], carPos_.x - offset.x, carPos_.y + (-EFFECT_OFFSET * sizeCon_) - offset.y, 0);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/explosion.efkefc", 25.0f)[0], carPos_.x - offset.x, carPos_.y - offset.y, 0);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/inversion.efkefc", 30.0f)[0], carPos_.x - offset.x, carPos_.y - offset.y, 0);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/acceleration.efkefc", 20.0f)[0], carPos_.x + 200.0f - offset.x, carPos_.y - offset.y, 0);
    SetPosPlayingEffekseer2DEffect(lpEffMng.GetPlayID("./Effect/check.efkefc", 20.0f)[0], checkPos_.x + 8.0f - offset.x, checkPos_.y + 64.0f - offset.y, 0);
}

void Player::StopSound()
{
    if (CheckSoundMem(lpSoundMng.GetID("./sound/early.mp3")[0]))
    {
        StopSoundMem(lpSoundMng.GetID("./sound/early.mp3")[0]);
    }
    if (CheckSoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0]))
    {
        StopSoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0]);
    }
    if (CheckSoundMem(lpSoundMng.GetID("./sound/last.mp3")[0]))
    {
        StopSoundMem(lpSoundMng.GetID("./sound/last.mp3")[0]);
    }
    if (CheckSoundMem(lpSoundMng.GetID("./sound/ex.mp3")[0]))
    {
        StopSoundMem(lpSoundMng.GetID("./sound/ex.mp3")[0]);
    }
}

void Player::SetAngle(float angle)
{
    rotAngle_ = angle;
}
