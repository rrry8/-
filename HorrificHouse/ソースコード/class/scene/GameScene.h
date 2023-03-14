#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../common/Light.h"

class Camera;
class Controller;
class Stage;
class Player;
class Enemy;

class CBufferMng;
class SocketServer;
class Goal;
class OriginalShader;
class GameCBuffer;
class GameUI;
enum class CntType;
class EffekseerEffectHandle;
class Effects;

class GameScene :
    public BaseScene
{
public:
    GameScene();
	GameScene(CntType type);
	~GameScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Game; };

private:
    //���_��񏉊���
    void InitBloomVerts(void);

    //�P�x���o
    void DrawBloom(int renderTarget);

    //�ڂ����p
    void DrawGaussianBlur(int renderTarget,int writeRendertarget);  //�ʂ̃X�N���[���ɕ`��
    void DrawGaussianBlur(int renderTarget);                        //�����X�N���[���ɕ`��

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�X�e�[�W���
    std::unique_ptr<Stage> stage_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�G���
    std::unique_ptr<Enemy> enemy_;

    //�S�[�����
    std::unique_ptr<Goal> goal_;
    //�萔�o�b�t�@
    std::unique_ptr<GameCBuffer> cBuffer_;
    //�`��p�V�F�[�_�[
    std::unique_ptr<OriginalShader> origShader_;
    //�[�x�V�F�[�_�[
	std::unique_ptr<OriginalShader> depthShader_;
    //UI
    std::unique_ptr<GameUI> gameUI_;
    //�X�N���[��
    int screenRenderTarget_;
   
    //�C���x���g���̔w�i�n���h��
    int inventoryBackGroundHandle_;

    //�|�X�g�G�t�F�N�g�p
    std::unique_ptr<OriginalShader> smartPhoneShader_;
    int psPostEffect_;
    float postEffectShader_;
    CntType cntType_;
    //�f�t�H���g���W
    Vector3 defPos_;
    //�f�t�H���g�^�[�Q�b�g���W
    Vector3 defTargetPos_;

    //�u���[���p
    BlurData blurX_;                                //��
    BlurData blurY_;                                //�c
    int bloomRenderTarget_;                         //�����_�[�^�[�Q�b�g
    std::array<VERTEX2DSHADER, 4> bloomVerts_;      //�u���[�����_���
    std::array<VERTEX2D, 4> blurVerts_;             //�u���[���_���
    std::unique_ptr<OriginalShader> luminanceShader_;   //�P�x���o�V�F�[�_

    //��ʊE�[�x�p
    int depthRenderTarget_;     //�[�x�l
    int blurRenderTarget_;      //��u���[
    int highBlurRenderTarget_;  //���u���[
    float dofRange_;            //��ʊE�[�x�͈�
    float dofFocus;             //�t�H�[�J�X
    float dofLerp_;             //��ԋ���
    std::unique_ptr<OriginalShader> depthOfFieldShader_;

    //�G�t�F�N�g
    Effect dust_;
};
