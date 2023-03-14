#pragma once
#include<memory>
#include<vector>
#include "Unit.h"
#include "../../../tmx/TmxObj.h"
#include "../../Common/TextureMap.h"
constexpr float CapDown = 110;
constexpr float CapTop = 30;
constexpr int CreateMax = 7;

class AnimationController;
class MagStick;
class Designer;
class Capsule;
class Collider;
class Coin;

enum class MagMouseDir
{
    Non,
    UpUp,
    DownDown,
    UpDown,
    DownUp,
};

enum class RespawnPoint
{
    Start,  //スタート
    First,  //1番目
    Second  //2番目
};

enum class PlayerNum
{
    Player1,
    Player2,
};
class Player :
    public Unit
{
public:
    Player(InputUnique controller);
    Player(InputUnique controller,Vector3 pos,int num, CoinList& CoinList, EneymyList& EneymyList);
    ~Player();

    bool Init(void)override;
    bool Init(Vector3 pos)override;
    bool ReInit();
    bool ReCreateBullet(void);
    void Update(void)override;
    void Update(GimmickList& gimmicklist, const EneymyList& enemy);
    void InitUpdate(GimmickList& gimmicklist);
    void UpdateMagStick(const EneymyList& enemy);
    void StateUpdate();
    std::string GetHandlePath(void) override;
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList)override {};
    EnemyType GetEnemyType(void) override;
    int GetNumber(void)override { return number_; };
    void Draw(void)override;
    void Draw(int num);
    void MagStickDraw();
    void SetUnitList(EneymyList& list);
    void OpacityMagnet();
    //生存情報を返す
    bool GetisAlive(void) override;
    void SetisAlive(bool isAlive);
    //void SetMagBoxList(MagBoxList& list);
   /* Vector3 SetMagBoxVec(void);*/
    //void EnemyPosSize();
    Vector3 GetPos(void);
    Vector3 ShakeGetPos(void);
    Line GetLine(void);
    
    //死ぬか
    void SetDeathState(void);
    void SetCoinList(CoinList& list);
    void SetGimmickList(GimmickList& list);
    State GetState();
    void SetState(State state);

    std::vector<btRigidBody*> GetBreakmodel(void);

    //int GetCoinID();

    HitHandle GetCoinList();
    btRigidBody* GetBody();
    PlayerNum GetPlayerNum() { return Playernum_; };

    std::vector<VERTEX2DSHADER>& GetTrailData(void);
private:

    //入力のセット
    bool SetInput(InputUnique input);
    //横移動
    void SetRunState(void);
    //キー入力の確認
    Vector3 KeyCheckMove(void);
    //磁石の時のクリックの入力を確認
    void SetMagnetState(void);
    //磁石の挙動
    void SetMagnetPow(void);
    //磁石のゆっくり上昇
    void MagnetWeakMove(void);
    //移動
    void Move(void);

    Vector3 GetMoveVec(void);
    //カプセルとの判定
    void HitCapsule(const EneymyList& enemy);
    //カプセルとステージ
    bool HitCapsuleAndStage(void);
    //カプセルとコイン
    bool HitCapsuleAndCoin(void);
    //カプセルとボタン
    bool HitCapsuleAndPressurePlate(void);
    //カプセルと敵
    bool HitCapsuleAndEnemy(void);
    //カプセルとゴール
    bool HitCapsuleAndGoal(void);
    //カプセルとギア
    bool HitCapsuleAndGear(void);
    //弾との判定
    void HitCapsuleAndBullet(const EneymyList& enemy);

    //影
    void DrawShadow(float ShadowHeight, float ShadowSize);
    //死んだ
    bool deathUpdate();
    //プレイヤーの向く方向
    void SetRotation(void);

    //エフェクトの更新
    void UpdateEffect(void);

    void ColCheckMidPoint(void);
    void SetRespawnPoint(void);
    //入力
    InputUnique input_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    rapidxml::xml_node<>* UpMagNode_;
    rapidxml::xml_node<>* DownMagNode_;
    rapidxml::xml_node<>* MoveNode_;
    rapidxml::xml_node<>* MagnetWeakNode_;


    std::vector<int> PressurePlateList_;
    std::vector<Collider*> Colliders_;

    //std::unique_ptr<OriginalShader> originalShader_;

    std::unique_ptr<MagStick> magStick_;
    std::unique_ptr<Capsule> capsule_;
    std::unique_ptr<AnimationController> animController_;


    double DeltaDesignerStack_;     //磁石の合計時間
    std::unique_ptr<Designer> ad_;  //加速度の計算クラス

    const float j_max = 120000;//最大加加速度(最大躍度)の大きさ
    const float a_max = 120000; //最大加速度の大きさ
    const float v_max = 2300;//最大速度
    const float v_start = 0;//始点速度
    float v_target;         //目標速度
    float distance;      //magで移動する距離

    Vector3 OldMoveMagPos_;
    Vector3 NowMagVec_;  //今のマグネットベクトル
    Vector3 OldMagVec_;  //後のマグネットベクトル
    Vector3 MoveMagVec_; //磁石で使う方向ベクトル
    Vector3 moveVecPowMag_;  //磁石のposに足す値の格納場所
    bool MagDragflagLR_;      //地面を滑る時のフラグ
    //bool MagDragflagUD;      //壁を滑る時のフラグ
    bool MagWeakUpflag_;
    float ad_MagVec_; //マウスの方向へのベクトル格納

    float olddistance_;  //前回の移動距離
    EneymyList unitlist_;    //敵のリスト
    MagBoxList Boxlist_;
    bool logflag_;       //でっばぐ用のフラグ

    Vector3 OldMoveMagVec_;      //緩やかな落下をするためにベクトルを保持する
    Vector3 lastPos_;       //プレイヤーが最後に死んだポイント
    int CionGetID_; //コインのID
    CoinList coinList_;//コインのリスト
    int getPlateID;
    GimmickList GimmickList_;
    int coinH_;//コインのハンドル
    bool getCoinflg_;//COIN_MAX
    bool getStepOnflg_;
    Vector3 CollRayNormRecord_N_; //Nに当たったノーマライズされた法線
    Vector3 CollRayNormRecord_S_; //Sに当たったノーマライズされた法線

    Vector3 oldmoveVec_;  //慣性をやるために古い方向をとってくる
    MagMouseDir MMdir_;   //磁石を使った時の移動ベクトルと磁石のベクトルの方向

    //エフェクト
    Effect explosion_;//爆発
    Effect trail_;

    btRigidBody* Playerbody_;   //bullet3のプレイヤーbody
    std::vector<btRigidBody*> BreakPlayerbody_;  //bullet3の破壊プレイヤーbody
    btTransform trans;

    bool UpDragflag_;       //横にスライドしていいかのフラグ
    Vector3 midPos_[2];     //中間地点の座標
    Vector3 midSize_;       //中間地点のサイズ
    RespawnPoint resPoint_; //リスポーン地点

    Vector3 MagBoxVec_;     //磁石箱のベクトル
    float deathTime_;       //死ぬまでの時間格納

    int BreakmodelH_[7];    //破壊プレイヤーハンドル配列

    float amplitude_;       //揺らし時間格納

    int PointLightHandle_;      //１プレイヤーのライト
    int PointLightHandle2_;     //２プレイヤーのライト

    int HitHandle_;             //当たったハンドル

    Vector3 defpos_;            //初期座標格納

    int ModelNum_;              //モデルの数字(１プレイヤーなら１)

    PlayerNum Playernum_;       //どのプレイヤーかenum格納
    
    TextureMap texmap;          //テクスチャ格納
protected:
};

