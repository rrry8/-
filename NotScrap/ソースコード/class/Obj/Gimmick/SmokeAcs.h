#pragma once
#include "Gimmick.h"
class SmokeAcs :
    public Gimmick
{
public:
    //�R���X�g���N�^
    SmokeAcs(Vector3 pos);
    //�f�X�g���N�^
    ~SmokeAcs();

    //������
    bool Init(void)override { return false; };
    bool Init(Vector3 pos);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //�`��
    void Draw(void)override;

    btRigidBody* GetRigidBody(void)override { return nullptr; };
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };
    //���W��Ԃ�
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //�I�u�W�F�N�g�^�C�v��Ԃ�
    objType GetObjType(void)override {
        return objType::Smoke;
    }

private:
    Effect Smoke_;
    int time_;
    bool flag_;

};

