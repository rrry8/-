#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/Geometry.h"
class Camera
{
public:
	//カメラの高さ
	static constexpr float HEIGHT = 70;

	//注視点からカメラまでのXZ距離
	static constexpr float DIS_TARGET_CAMERA = 1500.0f;


	Camera();
	~Camera();

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//追従カメラ
	void SetBeforeDraw(Vector3 pPos);

	//描画
	void Draw(void);

	//開放
	void Release(void);

	//カメラ座標を返す
	Vector3 GetPos();
	//カメラをセット
	void SetPos(Vector3 pos);
	//注視点を返す
	Vector3 GetTargetPos();

	//角度を返す
	Vector3 GetAngles();


private:
	//カメラの位置
	Vector3 mPos;

	//カメラ角度(rad)
	Vector3 mAngles;

	//注視点
	Vector3 mTargetPos;
	//カメラが追従をやめるライン（Y）
	float stopCameraPosY_;
};

