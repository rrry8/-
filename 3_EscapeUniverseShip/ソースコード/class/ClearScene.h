#pragma once
#include "BaseScene.h"
#include<string>

class Controller;

class ClearScene :
    public BaseScene
{
public:
    ClearScene();
    ClearScene(std::string mapnum);
    ~ClearScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:

    //�t�H���g���
    double fluffyTime_; //�ӂ�ӂ킷�鎞��
    double con_;        //DrawRotaGraph�̑傫���̕ϐ�
    double addFluffy_; //�ӂ�ӂ킷��Ƃ��ɑ����l

    //�w�i�̃n���h��
    int gameBG_;

    //�X�e�[�W�̖��O
    std::string stageName_;

    //�L�[���
    std::unique_ptr<Controller> controller_;
};
