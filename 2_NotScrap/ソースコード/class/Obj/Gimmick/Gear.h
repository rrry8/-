#pragma once
#include <string>
#include "Gimmick.h"
using namespace std;
class Gear :
    public Gimmick
{
public:
    //コンストラクタ
    Gear(Vector3 pos,int num, string modelName);
    //デストラクタ
    ~Gear();

    //初期化
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, string modelName);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //描画
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override { return nullptr; };
    //座標を返す
    Vector3 GetPos()override { return pos_; };
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //モデルハンドルを返す
    int GetModelH(void)override { return num_; };

    //移動方向を設定
    void SetMoveVec(Vector3 vec)override {};
    //オブジェクトの種類を返す
    objType GetObjType(void)override {
        return objType::Gear;
    }
private:
    int num_;
    int time_;
    btRigidBody* Gearbody_;
    void SetGearModel(string modelName);
};

