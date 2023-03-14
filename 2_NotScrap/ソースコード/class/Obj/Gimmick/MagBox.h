#pragma once
#include "Gimmick.h"
class MagBox :
    public Gimmick
{
public:
    //�R���X�g���N�^
    MagBox(Vector3 pos, objType type, int num);
    //�f�X�g���N�^
    ~MagBox();

    //������
 	bool Init(void)override { return false; };    
    bool Init(Vector3 pos, int num);
    bool ReInit(void);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //�`��
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override;
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };

    //���W��Ԃ�
    Vector3 GetPos()override { return pos_; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };

    //�ړ�������ݒ�
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; };
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    objType GetObjType(void)override {
        return magboxtype_;
    };
    //�����t���O��ݒ�
    void SetIsAlive(bool flag)override
    {
        isAlive_ = flag;
    }
private:
    btRigidBody* MagBoxbody_;
    objType magboxtype_;
    Vector3 defPos_;
    
};

