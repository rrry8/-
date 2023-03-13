#pragma once
#include "Gimmick.h"
class SmokeAcs :
    public Gimmick
{
public:
    //コンストラクタ
    SmokeAcs(Vector3 pos);
    //デストラクタ
    ~SmokeAcs();

    //初期化
    bool Init(void)override { return false; };
    bool Init(Vector3 pos);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //描画
    void Draw(void)override;

    btRigidBody* GetRigidBody(void)override { return nullptr; };
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //モデルハンドルを返す
    int GetModelH(void)override { return num_; };
    //座標を返す
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //オブジェクトタイプを返す
    objType GetObjType(void)override {
        return objType::Smoke;
    }

private:
    Effect Smoke_;
    int time_;
    bool flag_;

};

