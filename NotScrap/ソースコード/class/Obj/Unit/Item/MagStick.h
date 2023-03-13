#pragma once
#include "../../Obj.h"
#include"../../../Common/Geometry.h"
#include "../../../../tmx/TmxObj.h"

class MagStickTrail;

//磁石の状態
enum class MAGSTATE
{
    None,   //何も押していない状態
    South,  //Ｓ極
    North,  //Ｎ極
    Max
};

class MagStick :
    public Obj
{
public:
    MagStick(Vector3 playerpos,CntType type,CntData cntdata, int num);
    ~MagStick();

    //初期化
    bool Init(void) override;
    //更新
    void Update(void) override;

    void Update(GimmickList& gimmicklist,MagBoxList& list,Vector3 pos,Vector3 dir,State state,const CntData& cntdata, const EneymyList& enemy);
    //描画
    void Draw(void) override;

    //破棄
    void Release(void);

    //正規化されたベクトルを返す
    Vector3 GetPlayerMoveVec(void);

    //棒の始点と終点を返す
    Line GetLine(void);

    Vector3 GetMagBoxVec(void);

    //エフェクトを停止
    void StopEffect(void);

    std::vector<VERTEX2DSHADER>& GetTrailData(void);
private:

    //マウスの位置を取得
    Vector3 GetCurrentMosusePosition(void);

    //Ｓ極とＮ極の当たり判定
    void CollMagnet(void);

    //磁石の箱との判定
    void CollMagnetBox(GimmickList& gimmicklist);
    //転がるアクセサリ―との当たり判定
    void CollAccessory(GimmickList& gimmicklist);
    //敵をくっつける処理
    void CollEnemy(const EneymyList& enemy);

    //線分の位置や方向を定める
    void SetLineInfo(void);

    //エフェクト管理
    void PlayEffect(void);

    //範囲内チェック
    bool CheckWithinRange(Vector3 playerpos, Vector3 gimmickpos);

    //弾を減速させる
    void SlowEnemyBullet(const EneymyList& enemy);

    //マウスアイコンの位置を棒の先端にする(padの場合)
    void SetMouseIconPosition(void);

    //変数-----------------------------------------------

    //始点と終点
    Line line_;         //真ん中
    Line rightLine_;    //右
    Line leftLine_;     //左

    //マウスの位置
    Vector3 endPos_;

    //棒の角度
    double angle_;

    //マウスの位置から始点へのベクトルを正規化した値
    Vector3 normalVec_;

    //プレイヤーが移動するベクトル(正規化)
    Vector3 playerMoveVec_;

    //棒の状態変数
    MAGSTATE state_;

    //棒の現在の長さ
    float rodLength_;

    //軌跡の情報
    std::unique_ptr<MagStickTrail> trail_;

    //xml
    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;

    //磁石の箱情報
    MagBoxList boxList_;

    //エフェクト関係
    Effect noneEffect_;     //極なしエフェクト
    Effect northEffect_;    //N極
    Effect southEffect_;    //S極
    Vector3 effectPos_;     //共通位置
    float effectAngle_;     //共通角度

    //腕のＺ位置
    float armPosz_;

    //プレイヤーの状態
    State playerState_;

    //磁石の箱への方向
    Vector3 magBoxVec_;

    //前フレームの右スティックの傾きを保存
    Vector2 oldAddPos_;

    CntType cntType_;
    CntData cntData_;
    int num_;
};

