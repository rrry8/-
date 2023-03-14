#pragma once
#include "BaseScene.h"

class Camera;

enum class Point
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	MAX
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene(bool multiFlg);//multiFlg : true(MULTIPLAY)/false(SINGLEPLAY)
	~SelectScene();
private:
	//初期化
	bool Init(void)override;
	//更新
	uniqueScene Update(double delta, uniqueScene ownScene)override;
	//描画
	void DrawOwnScreen(double delta)override;
	void MovePlayer();
	void UpdateCamera();

	//シーンIDを返す
	Scene GetSceneID(void) override { return Scene::Select; };

	//カメラ情報
	std::unique_ptr<Camera> camera_;
	//カメラの位置
	Vector3 cameraPos_;

	//プレイヤー
	Vector3 pos_;		//位置
	Vector3 GoalPos_;	//目標地点
	Point point_;		//
	//アニメーション
	std::unique_ptr<AnimationController> animController_;

	//ステージの番号
	int	stageNum_;

	Vector2Int selectMapOffset_;

	int playerHandle_;
};
