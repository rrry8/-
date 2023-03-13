#include<EffekseerForDXLib.h>
#include"../Scene/SceneManager.h"
#include "../Obj/Stage.h"
#include "../Obj/Unit/Player.h"
#include "Camera.h"

Camera::Camera()
{
	Init();
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//表示されるウィンドウの原点
	//カメラの初期設定
	mPos = { 0.0f,0.0f,-DIS_TARGET_CAMERA };
	//カメラ角度
	mAngles = {
		//30.0f *(DX_PI_F/180),
		Deg2RadF(30.0f),
		0.0f,
		0.0f
	};
	stopCameraPosY_ = -1000.0f;

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(Vector3 pPos)
{
	float revRad = Deg2RadF(180.0f);
	float dirX = sin(mAngles.y + revRad);
	float dirZ = cos(mAngles.y + revRad);
	VECTOR dir = VNorm({ dirX,0.0f,dirZ });
	VECTOR movePow = VScale(dir, DIS_TARGET_CAMERA);
	movePow.y += HEIGHT;
	switch (lpStage.GetStage())
	{
	case STAGE_NAME::STAGE_1:
	case STAGE_NAME::STAGE_2:
	case STAGE_NAME::STAGE_3:
	case STAGE_NAME::STAGE_4:
		stopCameraPosY_ = -1000.0f;
		break;
	case STAGE_NAME::STAGE_5:
		stopCameraPosY_ = -4363.0f;
		break;
	case STAGE_NAME::STAGE_6:
		stopCameraPosY_ = -1224.0f;
		break;
	case STAGE_NAME::STAGE_7:
		stopCameraPosY_ = -1895.0f;
		break;
	case STAGE_NAME::MAX:
		break;
	}
	if (pPos.y > stopCameraPosY_)
	{
		pPos.y += 300;
		mTargetPos = pPos;
		mPos = ConvertVECTORtoVector3(VAdd(mTargetPos.toVECTOR(), movePow));
	}
	SetCameraPositionAndTarget_UpVecY(
		mPos.toVECTOR(),
		mTargetPos.toVECTOR());
	Effekseer_Sync3DSetting();
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

Vector3 Camera::GetPos(void)
{
	return mPos;
}

void Camera::SetPos(Vector3 pos)
{
	float revRad = Deg2RadF(180.0f);
	float dirX = sin(mAngles.y + revRad);
	float dirZ = cos(mAngles.y + revRad);
	VECTOR dir = VNorm({ dirX,0.0f,dirZ });
	VECTOR movePow = VScale(dir, DIS_TARGET_CAMERA);
	movePow.y += HEIGHT;

	if (pos.y > stopCameraPosY_)
	{
		pos.y += 300;
		mTargetPos = pos;
		mPos = ConvertVECTORtoVector3(VAdd(mTargetPos.toVECTOR(), movePow));
	}
	SetCameraPositionAndTarget_UpVecY(
		mPos.toVECTOR(),
		mTargetPos.toVECTOR());
}

Vector3 Camera::GetTargetPos()
{
	return mTargetPos;
}

Vector3 Camera::GetAngles()
{
	return mAngles;
}
