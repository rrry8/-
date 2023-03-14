#pragma once
#include<memory>
#include<vector>
#include<string>
#include"../../Geometry.h"
#include"../Other/Raycast.h"

class Controller;
class Camera;
class TmxObj;
class CheckPoint;

//�`���[�g���A���̎�
enum class TUTORIAL_NUM
{
    None,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven
};

//�傫��
enum class PLAYER_SIZE
{
    Normal, //���ʁE�f�t�H���g
    Small,  //��
    Big,    //��
    Max
};

//�d��
enum class GRAVITY
{
    On,     //��
    Below,  //���E�f�t�H���g
    Max
};

//�v���C���[�̏��
enum class STATE
{
    None,   //����ɓ������Ă���Ƃ�
    Fall,   //������
    Death,  //���S
    Max
};

//�e�V�[��
enum class SCENE
{
    GAME,
    OVER,
    CLEAR,
    PAUSE
};

//���S���̍\����
struct DEATHMODE
{
    //�ʒu
    Vector2 pos;
    double angle = 0.0;
    double accel = 0.0;
    double delta = 0.0;
    float initVel = 0.0f;
    float movePowX = 0.0f;
};

class Player
{
public:
    Player();
	Player(int cnttype,float plus);
	~Player();

    // ����������
	bool Init(int cnttype);

    // �X�V�X�e�b�v
    SCENE Update(TmxObj& tmx, Vector2 offset,std::shared_ptr<CheckPoint> check);

    // �X�V�X�e�b�v
    SCENE UpdateTutorial(TmxObj& tmx, Vector2 offset,std::shared_ptr<CheckPoint> check);
   
    //�J�ڂ̎��Ɏg�p
    void MoveRight(Vector2 offset,float con);

    // �`�揈��
    void Draw(Vector2 offset);

    // �������
    void Release(void);

    //�v���C���[�̈ʒu��Ԃ�
    Vector2 GetPos(void);

    //�v���C���[�̈ʒu���Z�b�g
    void SetPos(Vector2 pos);

    //�v���C���[�̈ʒu���Z�b�g
    void SetGoalPos(float pos);

    //�`���[�g���A����
    bool TutorialMove(TmxObj& tmx, Vector2 offset,int nowbutton);

    //�v���C���[�̊p�x���Z�b�g
    void SetAngle(float angle);

private:
    //�v���C���[������
    void InitPlayer(void);

    //�v���C���[�d�͕ύX���̏���
    void ChangePlayerGravity(bool invflag, float invflaot, GRAVITY gravity);

    //�v���C���[�̏��ύX����
    void ChangePlayerInfo(TmxObj& tmx, Vector2 offset);

    //�v���C���[�̈ړ�����
    bool MovePlayer(TmxObj& tmx, Vector2 offset);

    //���S���̏���
    SCENE MoveDeathMode(TmxObj& tmx, Vector2 offset);

    //�N���A���̏���
    void MoveClearMode(TmxObj& tmx, Vector2 offset);

    //�Q�[���I�[�o�[����
    bool CheckGameOver(TmxObj& tmx, Vector2 offset);

    //�ԑ̂Ƒ���̔���
    bool CheckBodyScaff(TmxObj& tmx, Vector2 offset, Vector2 half);

    //�ԑ̂Ƌ��̔���
    bool CheckBodySaw(TmxObj& tmx, Vector2 offset, Vector2 half);

    //�ԑ̂ƃh�����ʂ̔���
    bool CheckBodyDrum(TmxObj& tmx, Vector2 offset, Vector2 half);

    //���ԃ|�C���g����
    bool CheckCheckPoint(TmxObj& tmx, Vector2 offset, Vector2 half, std::shared_ptr<CheckPoint> check_);

    //���ԃ|�C���g����
    bool CheckBodyRotSaw(TmxObj& tmx, Vector2 offset, Vector2 half);

    //�G�t�F�N�g���
    void SetSizeEffect(void);   //�T�C�Y���Z�b�g
    void SetPlayEffect(void);   //�Đ����Z�b�g
    void StopEffect(void);      //�G�t�F�N�g���X�g�b�v����
    void SetEffectPos(Vector2 offset);

    //�T�E���h���X�g�b�v����
    void StopSound();

    //�v���C���[���
    Vector2 carPos_;              //�ʒu

    Vector2 respawnPos_;          //���X�|�[���ʒu
    float accel_;                 //���v�����x
    float radius_;                //���a
    double angle_;                //�^�C���̉�]
    float sizeCon_;               //�v���C���[�̃T�C�Y�{��
    bool inversionFlag_;          //���]�t���O
    float inversionFloat_;        //���]�t���O��float��
    PLAYER_SIZE nowSize_;              //�v���C���[�̌��݂̃T�C�Y
    GRAVITY nowGravity;           //�v���C���[�̌��݂̏d��
    STATE nowState_;              //�v���C���[���݂̏��
    bool clearFlag_;              //�N���A���Ă��邩�ǂ����̃t���O
    bool centerFlag_;

    double delta_;                //�f���^�^�C��

    Vector2 checkPos_;            //���ԃ|�C���g�̈ʒu

    Raycast raycast_;             //�����蔻��

    double fallTime_;             //��������

    float goalPos_;               //�S�[���̈ʒu

    float rotAngle_;              //��]�p�x

    float plusRot_;               //������]�p�x

    //���S���̃��[�h
    DEATHMODE deathRight_;  //�E�̃^�C��
    DEATHMODE deathLeft_;   //���̃^�C��
    DEATHMODE deathCar_;    //�ԑ�

    //�L�[���
	std::unique_ptr<Controller> controller_;
};
