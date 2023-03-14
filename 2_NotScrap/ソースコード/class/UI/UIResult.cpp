#include <DxLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Scene/SelectScene.h"
#include "UIResult.h"
#include"../Input/KeyInput.h"
#include"../Camera/Camera.h"
#include "SceneUIManager.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../Obj/CheckHit.h"
#include "../Input/ControllerMng.h"

constexpr int GAMECLEAR_POS_X = 371;
constexpr int GAMECLEAR_POS_Y = 150;

UIResult::UIResult()
{
	Init();
	DrawOwnScreen(0.0);
}


UIResult::~UIResult()
{
	DeleteShader(shaderHandle_);
}

bool UIResult::Init(void)
{
	tmxObj_.LoadTmx("././tmx/resultUI.tmx");

	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MouseCursormodule");

	shaderHandle_ = LoadPixelShader("./source/shader/PS_UV.pso");
	cbuff_ = CreateShaderConstantBuffer(sizeof(float) * 8);
	writer_ = static_cast<float*>(GetBufferShaderConstantBuffer(cbuff_));

	//横幅情報
	writer_[0] = 0.0f;

	return true;
}

void UIResult::Update(double delta)
{
	auto node = stateNode_->first_node("module");

	GetMousePoint(&MousePos.x, &MousePos.y);
	if (lpCntMng.GetController()->GetCntType() == CntType::Key)
	{
		MousePos.x += 32;//画像ずれてる分補正
		MousePos.y += 32;
	}
	int i = (int)UI_MAIN_ITEM::QUIT;
	for (auto& coll : tmxObj_.GetItemCollList())
	{
		if (CheckCollM()(raycast_, MousePos, node, coll))
		{
			lpSceneUIMng.SetHitItem_Main((UI_MAIN_ITEM)i);
			break;
		}
		else
		{
			lpSceneUIMng.SetHitItem_Main(UI_MAIN_ITEM::NONE);
		}
		i++;
	}

	//文字拡縮
	if (exFlg_)
	{
		if (exRate < 1.1)
		{
			exRate += 0.01;
		}
		else
		{
			exFlg_ = false;
		}
	}
	else
	{
		if (exRate > 1.0)
		{
			exRate -= 0.01;
		}
		else
		{
			exFlg_ = true;
		}
	}
	DrawOwnScreen(delta);
}


void UIResult::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenUIID_);
	ClsDrawScreen();

	//GameClear
	DrawShader(GAMECLEAR_POS_X, GAMECLEAR_POS_Y, shaderHandle_, lpImageMng.GetID("./source/UIImage/string/RESULT/GAMECLEAR.png")[0], -1);

	//文字の後ろにある背景
	DrawExtendGraph(1000 - 374 / 2, 450 - 72 / 2, 1000 + 374 / 2, 450 + 72 / 2, lpImageMng.GetID("./source/UIImage/string/mojihaikei.png")[0], true);
	//クリアタイム
	DrawRotaGraph(1000, 450, 0.7, 0, lpImageMng.GetID("./source/UIImage/string/RESULT/クリアタイム.png")[0],true,false,false);
	//文字の後ろにある背景
	DrawExtendGraph(1000 - 445 / 2, 600 - 77 / 2, 1000 + 445 / 2, 600 + 77 / 2, lpImageMng.GetID("./source/UIImage/string/mojihaikei.png")[0], true);
	//獲得したコイン
	DrawRotaGraph(1000, 600, 0.7, 0, lpImageMng.GetID("./source/UIImage/string/RESULT/獲得したコイン.png")[0],true,false,false);
	//セレクトへ
	DrawExtendGraph(1390, 928, 1582, 992, lpImageMng.GetID("./source/UIImage/string/RESULT/セレクトへ.png")[0],true);
	//タイトルへ
	DrawExtendGraph(1646, 928, 1822, 992, lpImageMng.GetID("./source/UIImage/string/RESULT/タイトルへ.png")[0],true);

}

void UIResult::DrawShader(float x, float y, int shader, int image, int norm)
{
	std::array<VERTEX2DSHADER, 4> verts_;   //頂点情報

	//
	for (auto& v : verts_)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(0, 0, 0, 0);
		v.pos.z = 0.0f;
		v.rhw = 1.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
	}

	int gx, gy;
	GetGraphSize(image, &gx, &gy);

	//頂点更新
	verts_[0].pos.x = x;
	verts_[0].pos.y = y;
	verts_[1].pos.x = x + gx;
	verts_[1].pos.y = y;
	verts_[2].pos.x = x;
	verts_[2].pos.y = y + gy;
	verts_[3].pos.x = x + gx;
	verts_[3].pos.y = y + gy;

	//UV座標
	verts_[0].u = 0.0f;
	verts_[0].v = 0.0f;
	verts_[1].u = 1.0f;
	verts_[1].v = 0.0f;
	verts_[2].u = 0.0f;
	verts_[2].v = 1.0f;
	verts_[3].u = 1.0f;
	verts_[3].v = 1.0f;

	SetShaderConstantBuffer(cbuff_, DX_SHADERTYPE_PIXEL, 0);
	writer_[0] += 0.01f;
	if (writer_[0] > 1.0f)
	{
		writer_[0] = 0.0f;
	}

	UpdateShaderConstantBuffer(cbuff_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUsePixelShader(shader);
	SetUseTextureToShader(0, image);
	//SetUseTextureToShader(1, norm);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool UIResult::GetLoadingEndFlg(void)
{
	return true;
}

void UIResult::SetIsNext(bool flg)
{
}
