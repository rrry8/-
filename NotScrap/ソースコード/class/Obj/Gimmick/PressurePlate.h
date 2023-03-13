#pragma once
#include "Gimmick.h"
#include "../CheckHit.h"

class PressureSidewaysDoor;
class PressurePlate :
    public Gimmick
{
public:
	//�R���X�g���N�^
	PressurePlate(Vector3 pPos, Vector3 dPos, DoorType type ,int num);

    //�f�X�g���N�^
	~PressurePlate();

    //������
    bool Init(void)override;
    bool Init(Vector3 pPos, Vector3 dPos, DoorType type);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
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
    //���Δ��̓����蔻��
    bool HitMagBox(bool flag,GimmickList& gimmickList);
    //�ʏ�h�A�̃|�C���^
    std::unique_ptr<PressureDoor> door_;
    //���ړ��h�A�̃|�C���^
    std::unique_ptr<PressureSidewaysDoor> Sdoor_;
    //�h�A�̎��
    DoorType type_;
    //�M�~�b�N���X�g
    GimmickList gimmickList_;
    //������
    int num_;

};

