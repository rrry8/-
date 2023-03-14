#pragma once
#include "Gimmick.h"

class TutorialMovie;
class PressureDoor;
class Controller;

class Tutorial :
    public Gimmick
{
public:
    //�R���X�g���N�^
    Tutorial(Vector3 pos, Vector3 doorPos, int num , EneymyList& list, TutorialType type);
    //�f�X�g���N�^
    ~Tutorial();

    //������
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, int num);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override;
    //�`��
    void Draw(void)override;
    btRigidBody* GetRigidBody(void)override { return nullptr; };
    //���W��ݒ�
    void SetPos(Vector3 pos)override { pos_ = pos; };
    //���f���n���h���Ԃ�
    int GetModelH(void)override { return num_; };
    //���W��Ԃ�
    Vector3 GetPos(void)override { return pos_; };
    void SetMoveVec(Vector3 vec)override { moveVec_ = vec; }
    //�I�u�W�F�N�g�^�C�v��Ԃ�
    objType GetObjType(void)override {
        return objType::Tutorial;
    }
private:
    int image_;
    int time_;
    int num_;
    //�G�t�F�N�g
    Effect wave_;
    Effect correct_;
    Effect correct2_;
    bool flag_;
    int effcnt_;

    bool OpenflagR_;
    bool OpenflagL_;
    TutorialType type_;
    Vector3 offset_;
    std::unique_ptr<PressureDoor> door_;
    float movedPosY_;
    EneymyList Enemylist_;
    //�����E�l
    Limit moveLimitVal_;

    std::unique_ptr<TutorialMovie>TutorialMovie_;

    std::unique_ptr<Controller> controller_;
};
