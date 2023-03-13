#include <DxLib.h>
#include "BaseScene.h"
#include"../Input/Controller.h"
#include "SceneManager.h"
#include "../Common/ImageMng.h"

//歪ませたい範囲
constexpr float DISTORTION_SIZE_X = 500; //x
constexpr float DISTORTION_SIZE_Y = 500; //y

//スクリーンサイズ
constexpr float SCREEN_SIZE_X = 1920.0f; //x
constexpr float SCREEN_SIZE_Y = 1080.0f; //y

BaseScene::BaseScene()
{
	InitShader();
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
	screenID_ = MakeScreen(screenSize_.x, screenSize_.y, true);
}

BaseScene::~BaseScene()
{
}

void BaseScene::InitShader(void)
{
	//頂点初期化
	Vector2Int screenSize = lpSceneMng.GetScreenSize();
	for (auto& vert : verts_)
	{
		vert.rhw = 1.0;
		vert.dif = GetColorU8(255, 255, 255, 255);
		vert.spc = GetColorU8(0, 0, 0, 0);
		vert.su = 0.0;
		vert.sv = 0.0;
		vert.pos.z = 0.0f;
	}

	verts_[0].pos.x = 0.0f;
	verts_[0].pos.y = 0.0f;
	verts_[0].u = 0.0f;
	verts_[0].v = 0.0f;

	verts_[1].pos.x = screenSize.x;
	verts_[1].pos.y = 0.0f;
	verts_[1].u = 1.0f;
	verts_[1].v = 0.0f;

	verts_[2].pos.x = 0.0f;
	verts_[2].pos.y = screenSize.y;
	verts_[2].u = 0.0f;
	verts_[2].v = 1.0f;

	verts_[3].pos.x = screenSize.x;
	verts_[3].pos.y = screenSize.y;
	verts_[3].u = 1.0f;
	verts_[3].v = 1.0f;

}

void BaseScene::Draw(double delta)
{
	DrawGraph(0, 0, screenID_, true);
}

void BaseScene::DrawPostEffect(int shaderhandle, int renderTarget, int normalhandle)
{
	SetUsePixelShader(shaderhandle);
	SetUseTextureToShader(0, renderTarget);
	SetUseTextureToShader(1, normalhandle);

	//描画(透過)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool BaseScene::KeyTrgDown(InputID id)
{
	if (cntData_[id][static_cast<int>(Trg::Now)] &&
		!cntData_[id][static_cast<int>(Trg::Old)])
	{
		return true;
	}
	return false;
}

bool BaseScene::KeyTrgUp(InputID id)
{
	if (cntData_[id][static_cast<int>(Trg::Old)] &&
		!cntData_[id][static_cast<int>(Trg::Now)])
	{
		return true;
	}
	return false;
}
