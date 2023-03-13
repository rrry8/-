#include "Camera.h"
#include"../Obj/Player.h"
#include"../SceneManager.h"

constexpr float DIV_SIZE = 5.0f;

Camera::Camera(Vector2 worldSize)
{
	Init();
	worldSize_ = worldSize;
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//表示されるウィンドウの原点
	viewPos_ = { 0,0 };
}

void Camera::Update(Vector2 playerPos)
{
	viewPos_.x = playerPos.x - lpSceneMng.GetViewSize().x / DIV_SIZE;
	if (viewPos_.x < 0.0f)
	{
		viewPos_.x = 0.0f;
	}
	if (viewPos_.x > worldSize_.x - lpSceneMng.GetViewSize().x)
	{
		viewPos_.x = worldSize_.x - lpSceneMng.GetViewSize().x;
	}
}

const Vector2& Camera::GetPos(void)
{
	return viewPos_;
}