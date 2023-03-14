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
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update();
    //�`��
    void Draw(void)override;

    btRigidBody* GetRigidBody(void)override;
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };
    //���W��Ԃ�
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //�I�u�W�F�N�g�^�C�v��Ԃ�
    objType GetObjType(void)override {
        return objType::PressurePlate;
    }
private:
    btRigidBody* Scrapbody_;
};

