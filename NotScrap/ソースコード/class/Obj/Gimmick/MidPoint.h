#pragma once
#include "Gimmick.h"
class MidPoint :
    public Gimmick
{
public:
    MidPoint(Vector3& pos, int num);
    ~MidPoint(void);
    //������ 
    bool Init(void)override;
    bool Init(Vector3& pos);

    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update(bool& flg);

    //�`��
    void Draw(void)override;

    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    btRigidBody* GetRigidBody(void)override;
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };

    //���W��Ԃ�
    Vector3 GetPos(void)override { return pos_; };
    //�ړ�������ݒ�
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    objType GetObjType(void)override {
        return objType::MidPoint;
    }
private:
};

