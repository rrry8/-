#pragma once
#include "Gimmick.h"
#include "../CheckHit.h"

class PressureSidewaysDoor;
class PressurePlate :
    public Gimmick
{
public:
	//コンストラクタ
	PressurePlate(Vector3 pPos, Vector3 dPos, DoorType type ,int num);

    //デストラクタ
	~PressurePlate();

    //初期化
    bool Init(void)override;
    bool Init(Vector3 pPos, Vector3 dPos, DoorType type);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
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
    //磁石箱の当たり判定
    bool HitMagBox(bool flag,GimmickList& gimmickList);
    //通常ドアのポインタ
    std::unique_ptr<PressureDoor> door_;
    //横移動ドアのポインタ
    std::unique_ptr<PressureSidewaysDoor> Sdoor_;
    //ドアの種類
    DoorType type_;
    //ギミックリスト
    GimmickList gimmickList_;
    //複製数
    int num_;

};

