#pragma once
#include "../../Scene/GameScene.h"
#include "Gimmick.h"


class PressureDoor :
    public Gimmick
{
public:
    //�R���X�g���N�^
    PressureDoor(Vector3& pos,int num);

    //�f�X�g���N�^
    ~PressureDoor();

    //������ 
    bool Init(void)override;
    bool Init(Vector3& pos);

    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update(bool& flg);

    //�`��
    void Draw(void)override;

    //�v���C���[���{�^���𓥂�ł��邩
    btRigidBody* GetRigidBody(void)override;
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };

    //���W��Ԃ�
    Vector3 GetPos(void)override { return pos_; };
    void SetStepOnFlg(bool flg);
    //�ړ�������ݒ�
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //�I�u�W�F�N�g�̎�ނ�Ԃ�
    objType GetObjType(void)override {
        return objType::PresureDoor;
    }
private:
    static constexpr int DOOR_SIZE_Y = 450;



    //���W
    Vector3 doorPos_;

    //�����E�l
    Limit moveLimitVal_;

    //�ړ���
    int movePow_;

    btRigidBody* Doorbody_;
};

