#pragma once
#include<DxLib.h>
#include<memory>
#include<array>
#include<string>
#include "../common/Geometry.h"
#include "../common/SceneID.h"

class BaseScene;
class OriginalShader;

using UniqueScene = std::unique_ptr<BaseScene>;


//�G�t�F�N�g�\����
struct Effect
{
	int resource = -1;     //�n���h��
	int playhandle = -1;   //�v���C�n���h��
	Vector3 pos; //�ʒu
	float angle = 0.0f; //�p�x
};

//�u���[�\����
struct BlurData
{
	int renderTarget = 1;   //�����_�����O�^�[�Q�b�g
	int vsHandle = -1;      //���_�V�F�[�_�n���h��
	Vector2 imageSize;      //�摜�T�C�Y
	std::unique_ptr<OriginalShader> origShader; //�`��p�V�F�[�_�N���X

	//������(make_unique��cpp���ŏ�����)
	void Init(Vector2Int screenSize, std::string vsPath)
	{
		float width, height;

		renderTarget = MakeScreen(screenSize.x, screenSize.y);
		vsHandle = LoadVertexShader(vsPath.c_str());
		GetGraphSizeF(renderTarget, &width, &height);
		imageSize = { width ,height };
	}
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// ����������
	virtual void Init(void) = 0;

	void InitVertext(void);

	// �X�V�X�e�b�v
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// �`�揈��
	virtual void Draw();
	virtual void DrawScreen(void) = 0;

	// �������
	virtual void Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;
	
private:

protected:

	void Loading();

	void AfterLoading();
	//�X�N���[�����
	int screenID_;

	//�A���x�h�����_�[�^�[�Q�b�g
	int albedRT_;
	//�@�������_�[�^�[�Q�b�g
	int normalSpecRT_;

	//�X�N���[���̑傫��
	int screenSizeX_;	//��
	int screenSizeY_;	//�c

	//Primitive2D�p�̒��_�f�[�^
	std::array<VERTEX2DSHADER, 4> verts_;
};