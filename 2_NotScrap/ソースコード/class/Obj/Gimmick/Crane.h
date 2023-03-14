#pragma once
#include "Gimmick.h"
class Crane :
    public Gimmick
{
public:
    //コンストラクタ
    Crane();
    //デストラクタ
    ~Crane();

    //初期化
    bool Init(void)override;
    //bool Init(Vector3 pos, int num);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override {};
    void Update(bool& MoveStartFlag);
    //描画
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override { return Cranebody_; };
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //座標を返す
    Vector3 GetPos()override { return pos_; };
    //モデルハンドルを返す
    int GetModelH(void)override { return 0; };
    //移動方向を設定
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; };
    //オブジェクトの種類を返す
    objType GetObjType(void)override {
        return objType::Crane;
    };
private:
    btRigidBody* Cranebody_;
    int Movetime_;

};

