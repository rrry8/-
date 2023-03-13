#pragma once
#include <memory>
#include "../Common/Geometry.h"
#include"../Input/Controller.h"
#include "../Common/SoundMng.h"
#include "../Input/ControllerMng.h"

class BaseScene;

using uniqueScene = std::unique_ptr<BaseScene>;
using UniqueController = std::unique_ptr<Controller>;

class AnimationController;

enum class Scene
{
	Transition,
	Title,
	Select,
	Game,
	Result,
	Option,
	Load,
	Menu,
	Max
};

class BaseScene
{
public:
	//コンストラクタ
	BaseScene();
	virtual ~BaseScene();

	//初期化
	virtual bool Init(void) = 0;
	void InitShader(void);

	//更新
	virtual uniqueScene Update(double delta, uniqueScene ownScene) = 0;

	//描画
	virtual void Draw(double delta);
	virtual void DrawOwnScreen(double delta) = 0;
	virtual void DrawPostEffect(int shaderhandle,int target, int normalhandle);

	bool KeyTrgDown(InputID id);
	bool KeyTrgUp(InputID id);

	//シーンIDを返す
	virtual Scene GetSceneID(void) = 0;
private:
protected:
	int screenID_;
	Vector2Int screenSize_;
	Vector2 cursorPos_;
	std::shared_ptr<AnimationController> animController_[3];//player,enemy,coinをタイトルで表示するための配列（仮
	CntData cntData_;

	//シェーダ関係
	std::array<VERTEX2DSHADER, 4> verts_;	//頂点情報

};
