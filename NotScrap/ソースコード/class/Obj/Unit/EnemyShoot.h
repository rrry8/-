#pragma once
#include "Unit.h"
#include"../../Common/Quaternion.h"

class BulletManager;

//����
enum class DIR
{
    LEFT,
    RIGHT,
    MAX
};

class EnemyShoot :
    public Unit
{
public:
    EnemyShoot(EnemyType type,Vector3 pos,int number);
    ~EnemyShoot();

    //������
    bool Init()override;

    //�X�V
    void Update(void)override;
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList) override;
    int GetNumber(void)override { return number_; };
    //�`��
    void Draw(void)override;

    //�ʒu�擾
    Vector3 GetPosition(void);

    std::string GetHandlePath(void) override;

    //�G�̎�ނ�Ԃ�(EnemyShoot)
    EnemyType GetEnemyType(void) override;

    //��������Ԃ�
    bool GetisAlive(void) override;

    std::shared_ptr<BulletManager> GetBulletData(void);
private:
    //��]
    void Rotate(void);
    //���
    EnemyType type_;

    //�T�C���J�[�u�̃J�E���^�[
    double stepSin_;

    //�v���C���[�̈ʒu
    Vector3 playerPos_;

    //�����G�t�F�N�g
    Effect explosion_;

    //�\����
    Transform trans_;

    //�����Ă������
    DIR dir_;

    //��]���̖ڕW�l
    Quaternion goalRot_;

    //��]���鎞��
    float stepRotTime_;

    //�e�Ǘ�
    std::shared_ptr<BulletManager> bullets_;

};
