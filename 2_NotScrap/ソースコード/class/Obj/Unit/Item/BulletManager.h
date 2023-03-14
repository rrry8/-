#pragma once
#include<memory>
#include<vector>
#include"../../../Common/Geometry.h"
#include"Bullet.h"
#include"../../../Common/Raycast.h"

class BulletManager
{
public:
	BulletManager(int enemyNum);
	~BulletManager();

	//������
	void Init(int enemyNum);

	//�X�V
	void Create(Vector3 playerpos, Vector3 enemypos);

	//�j��
	void Release(void);

	//�ړ�
	void Update(void);

	//�e�̕`��
	void Draw(void);

	//�e�ƃ��b�h�̔���
	void CollisionRodAndBullet(Line line,Line right,Line left);

	//�e���擾
	std::vector<std::shared_ptr<Bullet>>& GetBulletVector(void);

private:
	//�e�̒�`
	std::vector<std::shared_ptr<Bullet>> bullets_;

	//�e�̔��ˊԊu
	double stepBulletCreateTime_;

	Raycast raycast_;

	//�e�̐�
	int bulletNum_;

	//���Ԗڂ̓G���ۑ�
	int enemyNum_;
};
