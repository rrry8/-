#pragma once
#include "../../Obj.h"

class AnimationController;

class Bullet :
	public Obj
{
public:
	Bullet(Vector3 direction, Vector3 enemypos,int num);
	~Bullet();

	bool Init(void) override;
	bool Init(Vector3 direction, Vector3 enemypos);
	void Update(void) override;
	void Draw(void) override;

	//�������擾
	bool GetAliveFlag(void);

	//�������Z�b�g
	void SetAliveFlag(bool alive);

	//�����`�F�b�N(true:�j��/false:�j�����Ȃ�)
	bool CheckAlive(void);

	//�e�̈ʒu��Ԃ�
	Vector3 GetBulletPos(void);

	//�e�̔�ԕ������X�s�[�h�_�E��������E�ŏI�I�ɂ͔��]
	void DownSpeed(void);

	//���Ԗڂ��擾
	int GetNumber(void);

	//���]�t���O���擾
	bool GetInversionFlag(void);

private:
	//�e����ԕ��� * �X�s�[�h
	Vector3 movePower_;

	//�e����ԕ���(���K���ς�)
	Vector3 direction_;

	//�e�̈ʒu
	Vector3 pos_;

	//�������(����:true/�������Ă��Ȃ�:false)
	bool aliveFlag_;

	//�e�̐�������
	double aliveTime_;

	//��(�R���X�g���N�^�̈���������)
	int num_;

	//�i�s�����t���O(���]:true/���̂܂�:false)
	bool inversionFlag_;

	//���]�̉񐔃J�E���g(�t���[��)
	double stepSpeedDownTime_;

	//�����G�t�F�N�g
	Effect explosion_;

	//�\����
	Transform trans_;

	//��]���̖ڕW�l
	Quaternion goalRot_;

	//��]���鎞��
	double stepRotTime_;
};

