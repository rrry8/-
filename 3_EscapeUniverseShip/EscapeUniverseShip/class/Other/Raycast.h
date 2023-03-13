#pragma once
#include"../../Geometry.h"
#include"Collision.h"

class Raycast
{
public:
	//���C�\����
	struct Ray
	{
		Vector2 p;		//�n�_
		Vector2 vec;	//����
	};

	//���\����
	struct Line
	{
		Vector2 p;		//�n�_
		Vector2 end;	//�I�_
		Vector2 vec() { return end - p; }
	};

	//�������m�̏Փ˔���B�ԑ̂Ƒ���Ŏg�p
	bool CheckCollisionRay(Ray ray,Collision coll,Vector2 offset);

	//�������m�̍��Փ˔���
	bool CheckCollisionRayL(Ray ray,Collision2 coll,Vector2 offset);

	//�~�Ƃ̏㉺�Փ˔���B�^�C���Ƒ���Ŏg�p
	bool CheckCollisionCircleUpDown(Vector2 ray, Collision coll, Vector2 offset,float r,float& correct);

	//�~�Ƃ̏㉺���E�Փ˔���B�ԑ̂Ƃ̂�����Ŏg�p
	bool CheckCollisionCircleAll(Ray ray, Vector2 coll, Vector2 offset,float r);

	//�~�Ƃ̏㉺�Փ˔���B�^�C���Ƒ���Ŏg�p
	bool CheckCollisionCircleDownUp(Vector2 ray, Collision coll, Vector2 offset, float r, float& correct);

	//�~�Ƃ̏㉺�Փ˔���2�B�^�C���Ƒ���Ŏg�p
	bool CheckCollisionCircleUpDown(Vector2 ray, Collision2 coll, Vector2 offset, float r, float& correct);

private:

	//�����Ɛ����̓����蔻��
	bool CheckRay(Ray ray, Line line);

	//�����Ɖ~�̓����蔻��
	bool CheckCircle(Vector2 pos, Line line, float r, float& correct);

	//�����Ɖ~�̓����蔻��
	bool CheckSaw(Ray ray, Vector2 coll, float r);

};
