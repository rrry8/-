#pragma once
#include "BaseScene.h"

class Controller;

//�I����
enum class SELECT
{
    Retry,
    Select,
    MAX
};

class ResultScene :
    public BaseScene
{
public:
    ResultScene(UniqueScene scene,int id);
    ~ResultScene();

    // ����������
    void Init(void) override;

    //�X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:
    //�L�[���
    std::unique_ptr<Controller> controller_;

    SELECT nowSelect_;

    UniqueScene ownerScene_;

    //�t�H���g���
    double fluffyTime_; //�ӂ�ӂ펞��
    double con_;        //�{��
    double addCoeff_;       //�����l

    //�w�i
    int gameBG_;
};