#pragma once
#include "Gimmick.h"
class MagBox :
    public Gimmick
{
public:
    //コンストラクタ
    MagBox(Vector3 pos, objType type, int num);
    //デストラクタ
    ~MagBox();

    //初期化
 	bool Init(void)override { return false; };    
    bool Init(Vector3 pos, int num);
    bool ReInit(void);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //描画
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override;
    //座標を設定
    void SetPos(Vector3 pos)override { pos_ = pos; };

    //座標を返す
    Vector3 GetPos()override { return pos_; };
    //モデルハンドルを返す
    int GetModelH(void)override { return num_; };

    //移動方向を設定
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; };
    //オブジェクトの種類を返す
    objType GetObjType(void)override {
        return magboxtype_;
    };
    //生存フラグを設定
    void SetIsAlive(bool flag)override
    {
        isAlive_ = flag;
    }
private:
    btRigidBody* MagBoxbody_;
    objType magboxtype_;
    Vector3 defPos_;
    
};

