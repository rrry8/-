#pragma once
#include<DxLib.h>
#include<memory>
#include<array>
#include<string>
#include "../common/Geometry.h"
#include "../common/SceneID.h"

class BaseScene;
class OriginalShader;

using UniqueScene = std::unique_ptr<BaseScene>;


//エフェクト構造体
struct Effect
{
	int resource = -1;     //ハンドル
	int playhandle = -1;   //プレイハンドル
	Vector3 pos; //位置
	float angle = 0.0f; //角度
};

//ブラー構造体
struct BlurData
{
	int renderTarget = 1;   //レンダリングターゲット
	int vsHandle = -1;      //頂点シェーダハンドル
	Vector2 imageSize;      //画像サイズ
	std::unique_ptr<OriginalShader> origShader; //描画用シェーダクラス

	//初期化(make_uniqueはcpp側で初期化)
	void Init(Vector2Int screenSize, std::string vsPath)
	{
		float width, height;

		renderTarget = MakeScreen(screenSize.x, screenSize.y);
		vsHandle = LoadVertexShader(vsPath.c_str());
		GetGraphSizeF(renderTarget, &width, &height);
		imageSize = { width ,height };
	}
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// 初期化処理
	virtual void Init(void) = 0;

	void InitVertext(void);

	// 更新ステップ
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// 描画処理
	virtual void Draw();
	virtual void DrawScreen(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;
	
private:

protected:

	void Loading();

	void AfterLoading();
	//スクリーン情報
	int screenID_;

	//アルベドレンダーターゲット
	int albedRT_;
	//法線レンダーターゲット
	int normalSpecRT_;

	//スクリーンの大きさ
	int screenSizeX_;	//横
	int screenSizeY_;	//縦

	//Primitive2D用の頂点データ
	std::array<VERTEX2DSHADER, 4> verts_;
};