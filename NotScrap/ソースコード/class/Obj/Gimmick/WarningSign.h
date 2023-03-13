#pragma once
#include "Gimmick.h"
class WarningSign :
    public Gimmick
{
public:
    WarningSign(Vector3& pos, int num);
    ~WarningSign(void);
    //初期化 
    bool Init(void)override;
    bool Init(Vector3& pos);

    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update(bool& flg);

    //描画
    void Draw(void)override;
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };
    btRigidBody* GetRigidBody(void)override;
    //モデルハンドル返す
    int GetModelH(void)override { return num_; };
    //座標を返す
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //オブジェクトタイプを返す
    objType GetObjType(void)override {
        return objType::WarningSign;
    }
private:
};

