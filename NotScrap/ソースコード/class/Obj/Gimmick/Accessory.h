#pragma once
#include "Gimmick.h"
class Accessory :
    public Gimmick
{
public:
    //�R���X�g���N�^
    Accessory(Vector3 pos,int num);
    //�f�X�g���N�^
    ~Accessory();

    //������
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, int num);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //�`��
    void Draw(void)override;
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };
    btRigidBody* GetRigidBody(void)override;
    //���W��Ԃ�
    Vector3 GetPos(void)override;
    //�ړ�������ݒ�
    void SetMoveVec(Vector3 vec)override;
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    objType GetObjType(void)override {
        return objType::Accessory;
    }
private:
    btRigidBody* Acsbody_;
    float Zoffset_;
};

