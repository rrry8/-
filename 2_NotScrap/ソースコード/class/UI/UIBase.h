#pragma once
#include <memory>
#include "../Common/Geometry.h"
#include"../Input/Controller.h"
#include"../Scene/BaseScene.h"



class UIBase;
using uniqueSceneUI = std::unique_ptr<UIBase>;


class UIBase
{
public:
	UIBase();
	~UIBase();

	//初期化
	virtual bool Init(void) = 0;

	//更新
	virtual void Update(double delta) = 0;

	//描画
	virtual void Draw(double delta);
	virtual void DrawOwnScreen(double delta) = 0;
	bool KeyTrgDown(InputID id);
	bool KeyTrgUp(InputID id);
	//シーンIDを返す
	virtual Scene GetSceneID(void) = 0;
	//ロードが終了したかどうか
	virtual bool GetLoadingEndFlg(void) = 0;
	virtual void SetIsNext(bool flg)=0;
private:
protected:
	int screenUIID_;
	Vector2Int screenSize_;
	Vector2 cursorPos_;
	std::shared_ptr<Controller> controller_;
	std::shared_ptr<AnimationController> animController_;//タイトルロゴをアニメーションする時に使う
	Vector2Int MousePos;//マウスカーソルの座標
	int IsHitItem;//何番目に当たったかを判別するための変数
	Vector2Int offset_;//UIをずらしたりするときの為のオフセット用
	CntData cntData_;
};

