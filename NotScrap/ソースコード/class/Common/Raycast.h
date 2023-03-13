#pragma once
#include"Collision.h"
#include "Geometry.h"

class Raycast
{
public:
	struct Ray
	{
		Vector3 p;		//始点
		Vector3 vec;	//方向
	};

	//線分同士の衝突判定。車体と足場で使用
	bool CheckCollisionRay(Ray ray,Collision coll,Vector2 offset);

	//円との上下衝突判定。タイヤと足場で使用
	bool CheckCollisionCircleUpDown(Vector2 ray, Collision coll, Vector2 offset,float r,float& Correct);

	//線分と円の当たり判定
	bool CheckCircle(Line line, Vector3 pos, float r, Vector3 offset);

private:
	//線分と線分の当たり判定
	bool CheckRay(Ray ray, Line line);
};

