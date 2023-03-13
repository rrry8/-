#pragma once
#include<memory>
#include"../Geometry.h"
class BaseScene;
class Camera;

using UniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// ����������
	virtual void Init(void) = 0;

	// �X�V
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// �`�揈��
	virtual void Draw();
	//�I�t�Z�b�g�t��
	virtual void DrawplusOffset(Vector2 offset);
	virtual void DrawScreen(void) = 0;

	// �������
	virtual void Release(void) = 0;

	//�T�E���h�̒�~
	virtual void StopSound();

	//�T�E���h�̉��ʕύX
	virtual void ChangeSoundVolume(int volume);

private:

protected:

	//�`�悷����
	int screenID_;
	//��ʃT�C�Y��
	int screenSizeX_;
	//��ʃT�C�Y��
	int screenSizeY_;
};