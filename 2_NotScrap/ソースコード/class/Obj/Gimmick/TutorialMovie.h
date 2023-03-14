#pragma once
#include "Gimmick.h"

class TutorialMovie :
    public Gimmick
{
public:
    //�R���X�g���N�^
    TutorialMovie(Vector3 pos,std::string str,int num,TutorialType type);
    //�f�X�g���N�^
    ~TutorialMovie();

    //������
    bool Init(void)override { return false; };
    bool Init(Vector3 pos, int num);
    //�X�V
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList)override {};
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList, GimmickList& gimmickList,bool flag);

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
    std::string MovieName_;
    TutorialType type_;
    Vector3 offset_;

    float movedPosY_;
    //�����E�l
    Limit moveLimitVal_;
    bool clearflag_;
};