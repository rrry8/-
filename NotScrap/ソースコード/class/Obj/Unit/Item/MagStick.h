#pragma once
#include "../../Obj.h"
#include"../../../Common/Geometry.h"
#include "../../../../tmx/TmxObj.h"

class MagStickTrail;

//���΂̏��
enum class MAGSTATE
{
    None,   //���������Ă��Ȃ����
    South,  //�r��
    North,  //�m��
    Max
};

class MagStick :
    public Obj
{
public:
    MagStick(Vector3 playerpos,CntType type,CntData cntdata, int num);
    ~MagStick();

    //������
    bool Init(void) override;
    //�X�V
    void Update(void) override;

    void Update(GimmickList& gimmicklist,MagBoxList& list,Vector3 pos,Vector3 dir,State state,const CntData& cntdata, const EneymyList& enemy);
    //�`��
    void Draw(void) override;

    //�j��
    void Release(void);

    //���K�����ꂽ�x�N�g����Ԃ�
    Vector3 GetPlayerMoveVec(void);

    //�_�̎n�_�ƏI�_��Ԃ�
    Line GetLine(void);

    Vector3 GetMagBoxVec(void);

    //�G�t�F�N�g���~
    void StopEffect(void);

    std::vector<VERTEX2DSHADER>& GetTrailData(void);
private:

    //�}�E�X�̈ʒu���擾
    Vector3 GetCurrentMosusePosition(void);

    //�r�ɂƂm�ɂ̓����蔻��
    void CollMagnet(void);

    //���΂̔��Ƃ̔���
    void CollMagnetBox(GimmickList& gimmicklist);
    //�]����A�N�Z�T���\�Ƃ̓����蔻��
    void CollAccessory(GimmickList& gimmicklist);
    //�G���������鏈��
    void CollEnemy(const EneymyList& enemy);

    //�����̈ʒu��������߂�
    void SetLineInfo(void);

    //�G�t�F�N�g�Ǘ�
    void PlayEffect(void);

    //�͈͓��`�F�b�N
    bool CheckWithinRange(Vector3 playerpos, Vector3 gimmickpos);

    //�e������������
    void SlowEnemyBullet(const EneymyList& enemy);

    //�}�E�X�A�C�R���̈ʒu��_�̐�[�ɂ���(pad�̏ꍇ)
    void SetMouseIconPosition(void);

    //�ϐ�-----------------------------------------------

    //�n�_�ƏI�_
    Line line_;         //�^��
    Line rightLine_;    //�E
    Line leftLine_;     //��

    //�}�E�X�̈ʒu
    Vector3 endPos_;

    //�_�̊p�x
    double angle_;

    //�}�E�X�̈ʒu����n�_�ւ̃x�N�g���𐳋K�������l
    Vector3 normalVec_;

    //�v���C���[���ړ�����x�N�g��(���K��)
    Vector3 playerMoveVec_;

    //�_�̏�ԕϐ�
    MAGSTATE state_;

    //�_�̌��݂̒���
    float rodLength_;

    //�O�Ղ̏��
    std::unique_ptr<MagStickTrail> trail_;

    //xml
    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;

    //���΂̔����
    MagBoxList boxList_;

    //�G�t�F�N�g�֌W
    Effect noneEffect_;     //�ɂȂ��G�t�F�N�g
    Effect northEffect_;    //N��
    Effect southEffect_;    //S��
    Vector3 effectPos_;     //���ʈʒu
    float effectAngle_;     //���ʊp�x

    //�r�̂y�ʒu
    float armPosz_;

    //�v���C���[�̏��
    State playerState_;

    //���΂̔��ւ̕���
    Vector3 magBoxVec_;

    //�O�t���[���̉E�X�e�B�b�N�̌X����ۑ�
    Vector2 oldAddPos_;

    CntType cntType_;
    CntData cntData_;
    int num_;
};

