#pragma once
#include<memory>
#include<vector>
#include<string>
#include"../../Geometry.h"
#include"../Other/Raycast.h"

class Controller;
class Camera;
class TmxObj;
class CheckPoint;

//チュートリアルの時
enum class TUTORIAL_NUM
{
    None,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven
};

//大きさ
enum class PLAYER_SIZE
{
    Normal, //普通・デフォルト
    Small,  //小
    Big,    //大
    Max
};

//重力
enum class GRAVITY
{
    On,     //上
    Below,  //下・デフォルト
    Max
};

//プレイヤーの状態
enum class STATE
{
    None,   //足場に当たっているとき
    Fall,   //落下中
    Death,  //死亡
    Max
};

//各シーン
enum class SCENE
{
    GAME,
    OVER,
    CLEAR,
    PAUSE
};

//死亡時の構造体
struct DEATHMODE
{
    //位置
    Vector2 pos;
    double angle = 0.0;
    double accel = 0.0;
    double delta = 0.0;
    float initVel = 0.0f;
    float movePowX = 0.0f;
};

class Player
{
public:
    Player();
	Player(int cnttype,float plus);
	~Player();

    // 初期化処理
	bool Init(int cnttype);

    // 更新ステップ
    SCENE Update(TmxObj& tmx, Vector2 offset,std::shared_ptr<CheckPoint> check);

    // 更新ステップ
    SCENE UpdateTutorial(TmxObj& tmx, Vector2 offset,std::shared_ptr<CheckPoint> check);
   
    //遷移の時に使用
    void MoveRight(Vector2 offset,float con);

    // 描画処理
    void Draw(Vector2 offset);

    // 解放処理
    void Release(void);

    //プレイヤーの位置を返す
    Vector2 GetPos(void);

    //プレイヤーの位置をセット
    void SetPos(Vector2 pos);

    //プレイヤーの位置をセット
    void SetGoalPos(float pos);

    //チュートリアル時
    bool TutorialMove(TmxObj& tmx, Vector2 offset,int nowbutton);

    //プレイヤーの角度をセット
    void SetAngle(float angle);

private:
    //プレイヤー初期化
    void InitPlayer(void);

    //プレイヤー重力変更時の処理
    void ChangePlayerGravity(bool invflag, float invflaot, GRAVITY gravity);

    //プレイヤーの情報変更処理
    void ChangePlayerInfo(TmxObj& tmx, Vector2 offset);

    //プレイヤーの移動処理
    bool MovePlayer(TmxObj& tmx, Vector2 offset);

    //死亡時の処理
    SCENE MoveDeathMode(TmxObj& tmx, Vector2 offset);

    //クリア時の処理
    void MoveClearMode(TmxObj& tmx, Vector2 offset);

    //ゲームオーバー判定
    bool CheckGameOver(TmxObj& tmx, Vector2 offset);

    //車体と足場の判定
    bool CheckBodyScaff(TmxObj& tmx, Vector2 offset, Vector2 half);

    //車体と鋸の判定
    bool CheckBodySaw(TmxObj& tmx, Vector2 offset, Vector2 half);

    //車体とドラム缶の判定
    bool CheckBodyDrum(TmxObj& tmx, Vector2 offset, Vector2 half);

    //中間ポイント判定
    bool CheckCheckPoint(TmxObj& tmx, Vector2 offset, Vector2 half, std::shared_ptr<CheckPoint> check_);

    //中間ポイント判定
    bool CheckBodyRotSaw(TmxObj& tmx, Vector2 offset, Vector2 half);

    //エフェクト情報
    void SetSizeEffect(void);   //サイズをセット
    void SetPlayEffect(void);   //再生情報セット
    void StopEffect(void);      //エフェクトをストップする
    void SetEffectPos(Vector2 offset);

    //サウンドをストップする
    void StopSound();

    //プレイヤー情報
    Vector2 carPos_;              //位置

    Vector2 respawnPos_;          //リスポーン位置
    float accel_;                 //合計加速度
    float radius_;                //半径
    double angle_;                //タイヤの回転
    float sizeCon_;               //プレイヤーのサイズ倍率
    bool inversionFlag_;          //反転フラグ
    float inversionFloat_;        //反転フラグのfloat版
    PLAYER_SIZE nowSize_;              //プレイヤーの現在のサイズ
    GRAVITY nowGravity;           //プレイヤーの現在の重力
    STATE nowState_;              //プレイヤー現在の状態
    bool clearFlag_;              //クリアしているかどうかのフラグ
    bool centerFlag_;

    double delta_;                //デルタタイム

    Vector2 checkPos_;            //中間ポイントの位置

    Raycast raycast_;             //当たり判定

    double fallTime_;             //落下時間

    float goalPos_;               //ゴールの位置

    float rotAngle_;              //回転角度

    float plusRot_;               //足す回転角度

    //死亡時のモード
    DEATHMODE deathRight_;  //右のタイヤ
    DEATHMODE deathLeft_;   //左のタイヤ
    DEATHMODE deathCar_;    //車体

    //キー情報
	std::unique_ptr<Controller> controller_;
};
