#pragma once
#include<string>
#include "TransitionScene.h"

class Player;
class Camera;
class Controller;

class MoveCountDown :
    public TransitionScene
{
public:
    MoveCountDown(double limitTime,std::string stage,std::string nonstage,Vector2 pos, UniqueScene beforeScene, UniqueScene afterScene);
    ~MoveCountDown();
private:
    //�J�ڍX�V
    bool UpdateTransition(double delta) override;
    //�e�L�X�g�̈ړ�
    void MoveText(bool flag);

    //�X�N���[���̕`��
    void DrawScreen() override;
    //�v���C���[�̕`��
    void DrawPlayer(void);

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�L�[���
    std::unique_ptr<Controller> controller_;

    //�^�C���̊p�x
    double wheelAngle_;

    //�X�e�[�W�e�L�X�g�̃p�X
    std::string stagePath_;     //�F��
    std::string nonStagePath_;  //�F�Ȃ�

    //�e�L�X�g�̈ʒu
    Vector2 textPos_;

    //�e�L�X�g�̈ړ�����
    Vector2 textVelocity_;

    //�_�Ŏ���
    double blinkTime_;

    double stepStopTime_;
};

