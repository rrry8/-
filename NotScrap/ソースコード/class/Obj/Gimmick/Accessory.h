#pragma once
#include "Gimmick.h"
class Accessory :
    public Gimmick
{
public:
    //コンストラクタ
    Accessory(Vector3 pos,int num);
    //デストラクタ
    ~Accessory();

    //初期化
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, int num);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //描画
    void Draw(void)override;
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //モデルハンドルを返す
    int GetModelH(void)override { return num_; };
    btRigidBody* GetRigidBody(void)override;
    //座標を返す
    Vector3 GetPos(void)override;
    //移動方向を設定
    void SetMoveVec(Vector3 vec)override;
    //オブジェクトの種類を返す
    objType GetObjType(void)override {
        return objType::Accessory;
    }
private:
    btRigidBody* Acsbody_;
    float Zoffset_;
};

