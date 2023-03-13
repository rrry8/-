#pragma once
#include <string>
#include "Gimmick.h"
using namespace std;
class Gear :
    public Gimmick
{
public:
    //�R���X�g���N�^
    Gear(Vector3 pos,int num, string modelName);
    //�f�X�g���N�^
    ~Gear();

    //������
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, string modelName);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //�`��
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override { return nullptr; };
    //���W��Ԃ�
    Vector3 GetPos()override { return pos_; };
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };

    //�ړ�������ݒ�
    void SetMoveVec(Vector3 vec)override {};
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    objType GetObjType(void)override {
        return objType::Gear;
    }
private:
    int num_;
    int time_;
    btRigidBody* Gearbody_;
    void SetGearModel(string modelName);
};

