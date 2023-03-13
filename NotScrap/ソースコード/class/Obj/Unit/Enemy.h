#pragma once
#include<memory>
#include "Unit.h"
#include "../../../tmx/TmxObj.h"

class Enemy :
    public Unit
{
public:
    Enemy(EnemyType type, Vector3 pos, int number);
    ~Enemy();

    bool Init(void)override;
    void Update(void)override;
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList)override;
    void Draw(void)override;
    EnemyType GetEnemyType(void) override;

    //�����]��
    void ChangeTurn(Vector3 moveVec);

    //�ʒu���擾
    Vector3 GetPosition(void);

    void SetPosition(Vector3 pos);

    //��Ԃ��Z�b�g����
    void SetState(State state);

    //��Ԏ擾
    State GetState(void);

    //���a�擾
    float GetRadius(void);

    //�_�̐������Z�b�g
    void SetRodLine(Line line);

    std::string GetHandlePath(void) override;

    //��������Ԃ�
    bool GetisAlive(void) override;

    int GetNumber(void)override { return number_; };
private:

    EnemyType type_;

    //xml
    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;

    //�_�̎��_����G�̈ʒu�̒����𔼌a�Ƃ���
    float radius_;

    //�_�̐���
    Line rodLine_;

    //�v���C���[�̈ʒu
    Vector3 playerPos_;

    btRigidBody* Enemybody_;

    int num;
};