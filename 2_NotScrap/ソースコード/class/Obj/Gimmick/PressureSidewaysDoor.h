#pragma once
//#include "../../Scene/GameScene.h"
#include "Gimmick.h"


class PressureSidewaysDoor :
    public Gimmick
{
public:
    //コンストラクタ
    PressureSidewaysDoor(Vector3 pos,int num);

    //デストラクタ
    ~PressureSidewaysDoor();

    //初期化 
    bool Init(void)override;
    bool Init(Vector3& pos);

    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update(bool& flg);

    //描画
    void Draw(void)override;

    btRigidBody* GetRigidBody(void)override;
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //モデルハンドルを返す
    int GetModelH(void)override { return num_; };
    //座標を返す
    Vector3 GetPos(void)override { return pos_; };
    //プレイヤーがボタンを踏んでいるか
    void SetStepOnFlg(bool flg) { flg_ = flg; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //オブジェクトタイプを返す
    objType GetObjType(void)override {
        return objType::PresureDoor;
    }
private:
    static constexpr int DOOR_SIZE_X = 450;



    //座標
    Vector3 doorPos_;

    //可動限界値
    Limit moveLimitVal_;

    //移動量
    int movePow_;

    btRigidBody* Doorbody_;
};

