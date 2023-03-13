#pragma once
#include "Gimmick.h"

class TutorialMovie :
    public Gimmick
{
public:
    //コンストラクタ
    TutorialMovie(Vector3 pos,std::string str,int num,TutorialType type);
    //デストラクタ
    ~TutorialMovie();

    //初期化
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, int num);
    //更新
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override {};
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList,bool flag);

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
    std::string MovieName_;
    TutorialType type_;
    Vector3 offset_;

    float movedPosY_;
    //可動限界値
    Limit moveLimitVal_;
    bool clearflag_;
};