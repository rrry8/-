#pragma once
#include "Gimmick.h"
class Scrap :
    public Gimmick
{
public:
    Scrap(Vector3 pos,Vector3 rot, int num);
    ~Scrap();
    bool Init(void)override;
    bool Init(Vector3 pos, Vector3 rot);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update();
    //描画
    void Draw(void)override;

    btRigidBody* GetRigidBody(void)override;
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //モデルハンドルを返す
    int GetModelH(void)override { return num_; };
    //座標を返す
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //オブジェクトタイプを返す
    objType GetObjType(void)override {
        return objType::PressurePlate;
    }
private:
    btRigidBody* Scrapbody_;
};

