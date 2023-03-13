#pragma once
#include<memory>
#include"../../Geometry.h"

class Camera
{
public:
	Camera(Vector2 worldSize);
	~Camera();

	void Init(void);
	void Update(Vector2 playerPos);
	const Vector2& GetPos(void);

private:

	//‰ÂŽ‹”ÍˆÍ
	Vector2 viewPos_;

	Vector2 worldSize_;
};