#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include"../tmx/TmxObj.h"

class Player;
class Camera;
class Drum;
class CheckPoint;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    GameScene(std::string map,float addRot,int num);
	~GameScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;
    void DrawStage(void);

    // �������
    void Release(void) override;

private:
    //����]
    bool DrawRotSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset);

    //Tmx���
    TmxObj tmxObj_;
    std::string stageNum_;
protected:
    //�̂�����`��
    virtual bool DrawSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset);

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�h�����ʏ��
    std::unique_ptr<Drum> drum_;

    //���ԃ|�C���g���
    std::shared_ptr<CheckPoint> check_;

    //�̂�����̉�](�A�j���[�V�����p)
    double angle_;

    //�̂�����̈ʒu��]�p
    double rotSawAngle_;
    //��]���Z�l
    float addSawRot_;

    int sawImage_;      //�̂�����̃n���h��
    int sawRotImage_;   //�ʒu��]����̂�����

    //�^�C���̃n���h��
    int tileImage_;

    //�J�ڎ��̔w�i�n���h��
    int gameBG_;
};