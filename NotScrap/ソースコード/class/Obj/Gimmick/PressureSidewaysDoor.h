#pragma once
//#include "../../Scene/GameScene.h"
#include "Gimmick.h"


class PressureSidewaysDoor :
    public Gimmick
{
public:
    //�R���X�g���N�^
    PressureSidewaysDoor(Vector3 pos,int num);

    //�f�X�g���N�^
    ~PressureSidewaysDoor();

    //������ 
    bool Init(void)override;
    bool Init(Vector3& pos);

    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    void Update(bool& flg);

    //�`��
    void Draw(void)override;

    btRigidBody* GetRigidBody(void)override;
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h����Ԃ�
    int GetModelH(void)override { return num_; };
    //���W��Ԃ�
    Vector3 GetPos(void)override { return pos_; };
    //�v���C���[���{�^���𓥂�ł��邩
    void SetStepOnFlg(bool flg) { flg_ = flg; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //�I�u�W�F�N�g�^�C�v��Ԃ�
    objType GetObjType(void)override {
        return objType::PresureDoor;
    }
private:
    static constexpr int DOOR_SIZE_X = 450;



    //���W
    Vector3 doorPos_;

    //�����E�l
    Limit moveLimitVal_;

    //�ړ���
    int movePow_;

    btRigidBody* Doorbody_;
};

