#pragma once
#include"../../Geometry.h"
#include"Collision.h"

class Raycast
{
public:
	//レイ構造体
	struct Ray
	{
		Vector2 p;		//始点
		Vector2 vec;	//方向
	};

	//線構造体
	struct Line
	{
		Vector2 p;		//始点
		Vector2 end;	//終点
		Vector2 vec() { return end - p; }
	};

	//線分同士の衝突判定。車体と足場で使用
	bool CheckCollisionRay(Ray ray,Collision coll,Vector2 offset);

	//線分同士の左衝突判定
	bool CheckCollisionRayL(Ray ray,Collision2 coll,Vector2 offset);

	//円との上下衝突判定。タイヤと足場で使用
	bool CheckCollisionCircleUpDown(Vector2 ray, Collision coll, Vector2 offset,float r,float& correct);

	//円との上下左右衝突判定。車体とのこぎりで使用
	bool CheckCollisionCircleAll(Ray ray, Vector2 coll, Vector2 offset,float r);

	//円との上下衝突判定。タイヤと足場で使用
	bool CheckCollisionCircleDownUp(Vector2 ray, Collision coll, Vector2 offset, float r, float& correct);

	//円との上下衝突判定2。タイヤと足場で使用
	bool CheckCollisionCircleUpDown(Vector2 ray, Collision2 coll, Vector2 offset, float r, float& correct);

private:

	//線分と線分の当たり判定
	bool CheckRay(Ray ray, Line line);

	//線分と円の当たり判定
	bool CheckCircle(Vector2 pos, Line line, float r, float& correct);

	//線分と円の当たり判定
	bool CheckSaw(Ray ray, Vector2 coll, float r);

};
