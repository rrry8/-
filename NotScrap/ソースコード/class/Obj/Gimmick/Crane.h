#pragma once
#include "Gimmick.h"
class Crane :
    public Gimmick
{
public:
    //�R���X�g���N�^
    Crane();
    //�f�X�g���N�^
    ~Crane();

    //������
    bool Init(void)override;
    //bool Init(Vector3 pos, int num);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override {};
    void Update(bool& MoveStartFlag);
    //�`��
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override { return Cranebody_; };
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���W��Ԃ�
    Vector3 GetPos()override { return pos_; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return 0; };
    //�ړ�������ݒ�
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; };
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    objType GetObjType(void)override {
        return objType::Crane;
    };
private:
    btRigidBody* Cranebody_;
    int Movetime_;

};

