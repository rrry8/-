#pragma once
#include<memory>
#include"../../Common/Geometry.h"

class Controller;

class MouseIcon
{
public:
	MouseIcon();
	~MouseIcon();

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//破棄
	void Release(void);

	//マウスアイコンのサイズ取得
	static float GetMouseIconSize(void);

private:

	//マウスアイコンの描画
	void DrawMouseIcon(int shader,int icon,int norm);

	//色変更処理
	void ChangeColor(void);

	//画面内に補正
	void CorrectScreen(void);

	//アイコンの位置(マウスの位置)
	Vector2 pos_;

	//頂点情報等
	std::array<VERTEX2DSHADER, 4> verts_;

	//マウスアイコンのサイズ
	Vector2 graphSize_;

	bool colorChangeFlag_;

	//シェーダハンドル
	int shaderHandle_;

	//定数バッファ
	int cbuff_;

	//渡す情報
	float* writer_;

	std::unique_ptr<Controller> controller_;
};
