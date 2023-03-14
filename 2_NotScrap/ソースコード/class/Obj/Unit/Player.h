#pragma once
#include<memory>
#include<vector>
#include "Unit.h"
#include "../../../tmx/TmxObj.h"
#include "../../Common/TextureMap.h"
constexpr float CapDown = 110;
constexpr float CapTop = 30;
constexpr int CreateMax = 7;

class AnimationController;
class MagStick;
class Designer;
class Capsule;
class Collider;
class Coin;

enum class MagMouseDir
{
    Non,
    UpUp,
    DownDown,
    UpDown,
    DownUp,
};

enum class RespawnPoint
{
    Start,  //�X�^�[�g
    First,  //1�Ԗ�
    Second  //2�Ԗ�
};

enum class PlayerNum
{
    Player1,
    Player2,
};
class Player :
    public Unit
{
public:
    Player(InputUnique controller);
    Player(InputUnique controller,Vector3 pos,int num, CoinList& CoinList, EneymyList& EneymyList);
    ~Player();

    bool Init(void)override;
    bool Init(Vector3 pos)override;
    bool ReInit();
    bool ReCreateBullet(void);
    void Update(void)override;
    void Update(GimmickList& gimmicklist, const EneymyList& enemy);
    void InitUpdate(GimmickList& gimmicklist);
    void UpdateMagStick(const EneymyList& enemy);
    void StateUpdate();
    std::string GetHandlePath(void) override;
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList)override {};
    EnemyType GetEnemyType(void) override;
    int GetNumber(void)override { return number_; };
    void Draw(void)override;
    void Draw(int num);
    void MagStickDraw();
    void SetUnitList(EneymyList& list);
    void OpacityMagnet();
    //��������Ԃ�
    bool GetisAlive(void) override;
    void SetisAlive(bool isAlive);
    //void SetMagBoxList(MagBoxList& list);
   /* Vector3 SetMagBoxVec(void);*/
    //void EnemyPosSize();
    Vector3 GetPos(void);
    Vector3 ShakeGetPos(void);
    Line GetLine(void);
    
    //���ʂ�
    void SetDeathState(void);
    void SetCoinList(CoinList& list);
    void SetGimmickList(GimmickList& list);
    State GetState();
    void SetState(State state);

    std::vector<btRigidBody*> GetBreakmodel(void);

    //int GetCoinID();

    HitHandle GetCoinList();
    btRigidBody* GetBody();
    PlayerNum GetPlayerNum() { return Playernum_; };

    std::vector<VERTEX2DSHADER>& GetTrailData(void);
private:

    //���͂̃Z�b�g
    bool SetInput(InputUnique input);
    //���ړ�
    void SetRunState(void);
    //�L�[���͂̊m�F
    Vector3 KeyCheckMove(void);
    //���΂̎��̃N���b�N�̓��͂��m�F
    void SetMagnetState(void);
    //���΂̋���
    void SetMagnetPow(void);
    //���΂̂������㏸
    void MagnetWeakMove(void);
    //�ړ�
    void Move(void);

    Vector3 GetMoveVec(void);
    //�J�v�Z���Ƃ̔���
    void HitCapsule(const EneymyList& enemy);
    //�J�v�Z���ƃX�e�[�W
    bool HitCapsuleAndStage(void);
    //�J�v�Z���ƃR�C��
    bool HitCapsuleAndCoin(void);
    //�J�v�Z���ƃ{�^��
    bool HitCapsuleAndPressurePlate(void);
    //�J�v�Z���ƓG
    bool HitCapsuleAndEnemy(void);
    //�J�v�Z���ƃS�[��
    bool HitCapsuleAndGoal(void);
    //�J�v�Z���ƃM�A
    bool HitCapsuleAndGear(void);
    //�e�Ƃ̔���
    void HitCapsuleAndBullet(const EneymyList& enemy);

    //�e
    void DrawShadow(float ShadowHeight, float ShadowSize);
    //����
    bool deathUpdate();
    //�v���C���[�̌�������
    void SetRotation(void);

    //�G�t�F�N�g�̍X�V
    void UpdateEffect(void);

    void ColCheckMidPoint(void);
    void SetRespawnPoint(void);
    //����
    InputUnique input_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    rapidxml::xml_node<>* UpMagNode_;
    rapidxml::xml_node<>* DownMagNode_;
    rapidxml::xml_node<>* MoveNode_;
    rapidxml::xml_node<>* MagnetWeakNode_;


    std::vector<int> PressurePlateList_;
    std::vector<Collider*> Colliders_;

    //std::unique_ptr<OriginalShader> originalShader_;

    std::unique_ptr<MagStick> magStick_;
    std::unique_ptr<Capsule> capsule_;
    std::unique_ptr<AnimationController> animController_;


    double DeltaDesignerStack_;     //���΂̍��v����
    std::unique_ptr<Designer> ad_;  //�����x�̌v�Z�N���X

    const float j_max = 120000;//�ő�������x(�ő���x)�̑傫��
    const float a_max = 120000; //�ő�����x�̑傫��
    const float v_max = 2300;//�ő呬�x
    const float v_start = 0;//�n�_���x
    float v_target;         //�ڕW���x
    float distance;      //mag�ňړ����鋗��

    Vector3 OldMoveMagPos_;
    Vector3 NowMagVec_;  //���̃}�O�l�b�g�x�N�g��
    Vector3 OldMagVec_;  //��̃}�O�l�b�g�x�N�g��
    Vector3 MoveMagVec_; //���΂Ŏg�������x�N�g��
    Vector3 moveVecPowMag_;  //���΂�pos�ɑ����l�̊i�[�ꏊ
    bool MagDragflagLR_;      //�n�ʂ����鎞�̃t���O
    //bool MagDragflagUD;      //�ǂ����鎞�̃t���O
    bool MagWeakUpflag_;
    float ad_MagVec_; //�}�E�X�̕����ւ̃x�N�g���i�[

    float olddistance_;  //�O��̈ړ�����
    EneymyList unitlist_;    //�G�̃��X�g
    MagBoxList Boxlist_;
    bool logflag_;       //�ł��΂��p�̃t���O

    Vector3 OldMoveMagVec_;      //�ɂ₩�ȗ��������邽�߂Ƀx�N�g����ێ�����
    Vector3 lastPos_;       //�v���C���[���Ō�Ɏ��񂾃|�C���g
    int CionGetID_; //�R�C����ID
    CoinList coinList_;//�R�C���̃��X�g
    int getPlateID;
    GimmickList GimmickList_;
    int coinH_;//�R�C���̃n���h��
    bool getCoinflg_;//COIN_MAX
    bool getStepOnflg_;
    Vector3 CollRayNormRecord_N_; //N�ɓ��������m�[�}���C�Y���ꂽ�@��
    Vector3 CollRayNormRecord_S_; //S�ɓ��������m�[�}���C�Y���ꂽ�@��

    Vector3 oldmoveVec_;  //��������邽�߂ɌÂ��������Ƃ��Ă���
    MagMouseDir MMdir_;   //���΂��g�������̈ړ��x�N�g���Ǝ��΂̃x�N�g���̕���

    //�G�t�F�N�g
    Effect explosion_;//����
    Effect trail_;

    btRigidBody* Playerbody_;   //bullet3�̃v���C���[body
    std::vector<btRigidBody*> BreakPlayerbody_;  //bullet3�̔j��v���C���[body
    btTransform trans;

    bool UpDragflag_;       //���ɃX���C�h���Ă������̃t���O
    Vector3 midPos_[2];     //���Ԓn�_�̍��W
    Vector3 midSize_;       //���Ԓn�_�̃T�C�Y
    RespawnPoint resPoint_; //���X�|�[���n�_

    Vector3 MagBoxVec_;     //���Δ��̃x�N�g��
    float deathTime_;       //���ʂ܂ł̎��Ԋi�[

    int BreakmodelH_[7];    //�j��v���C���[�n���h���z��

    float amplitude_;       //�h�炵���Ԋi�[

    int PointLightHandle_;      //�P�v���C���[�̃��C�g
    int PointLightHandle2_;     //�Q�v���C���[�̃��C�g

    int HitHandle_;             //���������n���h��

    Vector3 defpos_;            //�������W�i�[

    int ModelNum_;              //���f���̐���(�P�v���C���[�Ȃ�P)

    PlayerNum Playernum_;       //�ǂ̃v���C���[��enum�i�[
    
    TextureMap texmap;          //�e�N�X�`���i�[
protected:
};

