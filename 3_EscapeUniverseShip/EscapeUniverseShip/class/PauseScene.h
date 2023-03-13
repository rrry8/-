#pragma once
#include "BaseScene.h"

//�I����
enum class PAUSESELECT
{
    Resume,
    Select,
    MAX
};

class Controller;

class PauseScene :
    public BaseScene
{
public:
    PauseScene(UniqueScene scene, int id);
    ~PauseScene();

    // ����������
    void Init(void) override;

    //�X�V
    UniqueScene Update(UniqueScene scene) override;

    //�`��
    void DrawScreen(void) override;

    //���
    void Release(void) override;

private:
    //�L�[���
    std::unique_ptr<Controller> controller_;

    //�O�̃V�[��
    UniqueScene ownerScene_;

    //�t�H���g���
    double fluffyTime_;     //�ӂ�ӂ펞��
    double coefficient_;    //�{��
    double addFluffy_;      //�����l

    //���݂̑I����
    PAUSESELECT nowSelect_;

    //�w�i
    int gameBG_;
};
