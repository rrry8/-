#pragma once
#include "Gimmick.h"

class TutorialMovie;
class PressureDoor;
class Controller;

class Tutorial :
    public Gimmick
{
public:
    //コンストラクタ
    Tutorial(Vector3 pos, Vector3 doorPos, int num , EneymyList& list, TutorialType type);
    //デストラクタ
    ~Tutorial();

    //初期化
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, int num);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //描画
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override { return nullptr; };
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //モデルハンドル返す
    int GetModelH(void)override { return num_; };
    //座標を返す
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //オブジェクトタイプを返す
    objType GetObjType(void)override {
        return objType::Tutorial;
    }
private:
    int image_;
    int time_;
    int num_;
    //エフェクト
    Effect wave_;
    Effect correct_;
    Effect correct2_;
    bool flag_;
    int effcnt_;

    bool OpenflagR_;
    bool OpenflagL_;
    TutorialType type_;
    Vector3 offset_;
    std::unique_ptr<PressureDoor> door_;
    float movedPosY_;
    EneymyList Enemylist_;
    //可動限界値
    Limit moveLimitVal_;

    std::unique_ptr<TutorialMovie>TutorialMovie_;

    std::unique_ptr<Controller> controller_;
};
