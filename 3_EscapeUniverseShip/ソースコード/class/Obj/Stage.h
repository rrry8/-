#pragma once
#include<memory>
#include"../../Geometry.h"

class Controller;
class Player;

//�X�e�[�W�̐�
enum class STAGE
{
    None,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Eleven,
    Twelve,
    Tutorial,
    End
};

class Stage
{
public:
    Stage(int cnttype);
    ~Stage();

    // ����������
    bool Init(int cnttype);

    // �X�V�X�e�b�v
    STAGE Update();

    // �`�揈��
    void Draw(void);

    // �������
    void Release(void);

    //�I�𒆂̃X�e�[�W�̈ʒu��Ԃ�
    Vector2 GetPos(void);

private:

    //�I���X�e�[�W��ύX
    void ChangeStage(void);

    //�L�[���
    std::unique_ptr<Controller> controller_;

    //�I�𒆂̃X�e�[�W
    STAGE nowStage_;

    //�I�𒆂̃X�e�[�W�̈ʒu
    Vector2 pos_;

    //�����̑傫���̔{��
    double coefficient_;

    //���t���[�������{��
    double addCoefficient_;

    //�ӂ�ӂ�̎���
    double fluffyTime_;
};

