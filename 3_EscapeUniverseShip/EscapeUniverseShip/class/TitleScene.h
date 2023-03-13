#pragma once
#include "BaseScene.h"

class Controller;

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:
    //�T�E���h������
    void InitSound(void);

    //�L�[���
    std::unique_ptr<Controller> controller_;

    //�t�H���g���
    double fluffyTime_; //����
    double coefficient_;        //�{��
    double addFluffyTime_;       //�����l

    //�^�C�g���e�L�X�g�̈ʒu��
    int titlePosY_;
};