#include "UILoad.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"


UILoad::UILoad()
{
	Init();
	DrawOwnScreen(0.0);
}

UILoad::~UILoad()
{
	DeleteShader(psH);
	DeleteShaderConstantBuffer(cBuf);
}

bool UILoad::Init(void)
{
	LoadVol = 0;
	LoadingTime = 0;
	IsLoadingEnd = false;
	PSInit();
	psH = LoadPixelShader("./source/shader/UVScroll.pso");
	return true;
}

void UILoad::Update(double delta)
{
	//TRACE("非同期読み込みの数:%d\n", GetASyncLoadNum());
	if (GetASyncLoadNum() == 0)//非同期読み込みの残数がいくつか調べる
	{
		//残数が0なら
		//TRACE("非同期読み込み終了\n");
		if (LoadVol >= 1000)//バーが最大値になったら
		{
			IsLoadingEnd = true;//ロード終了
		}
		else//バーが最大値以下の時
		{
			//バーをマックスにする
			LoadVol += 30;
			if (LoadVol > 1000)
			{
				LoadVol = 1000;
			}
		}
	}
	else
	{
		IsLoadingEnd = false;
		//ロードの進捗具合を表現してる
		if (LoadVol < 950)
		{
			if (LoadVol < 1000 / (GetASyncLoadNum() + 1))//残りの数で最大値を割って、バーの増えられる最大値を制限（１で割っても意味ないから+1）
			{
				LoadVol += 1;	
			}
			if (LoadVol < 1000 && (GetASyncLoadNum()) == 1)//ロードが最大以下　＆＆　+1してない状態の残数が１
			{
				LoadVol += 1;	//最後の一個になったら↑のと合わせて+2ずつ増やす
			}
		}
	}
	LoadingTime++;
	DrawOwnScreen(delta);
}

void UILoad::DrawOwnScreen(double delta)
{
	DrawGraph(STR_POINT_X, STR_POINT_Y, lpImageMng.GetID("./source/UIImage/string/LOAD/NOWLOADING.png")[0], true);
	switch (LoadingTime / 20 % 3)//・・・を増やしたり減らしたりしてるよ
	{
	case 0:
		DrawStringToHandle(STR_POINT_X + 340, STR_POINT_Y, ".", 0x000000, lpSceneMng.GetFontHandle(), 0xffffff, 0);
		break;
	case 1:
		DrawStringToHandle(STR_POINT_X + 340, STR_POINT_Y, "..", 0x000000, lpSceneMng.GetFontHandle(), 0xffffff, 0);
		break;
	case 2:
		DrawStringToHandle(STR_POINT_X + 340, STR_POINT_Y, "...", 0x000000, lpSceneMng.GetFontHandle(), 0xffffff, 0);
		break;
	default:
		break;
	}
	//--デバッグ用--
	//_dbgDrawFormatString(500, 300, 0xffffff, "load経過時間:%d秒", LoadingTime / 60);
	//_dbgDrawFormatString(500, 316, 0xffffff, "loading:%d％", (LoadVol/10));
	
	//--ロードバーの枠--
	DrawModiGraph(
		BAR_LEFT_TOP_X, BAR_LEFT_TOP_Y,//左上
		BAR_RIGHT_TOP_X, BAR_RIGHT_TOP_Y,//右上
		BAR_RIGHT_DOWN_X, BAR_RIGHT_DOWN_Y,//右下
		BAR_LEFT_DOWN_X, BAR_LEFT_DOWN_Y,//左下
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_Bar.png")[0], true);
	int StrWidth, StrLen;
	StrLen = strlen("100");
	StrWidth = GetDrawStringWidthToHandle("100", StrLen, lpSceneMng.GetFontHandle());
	DrawModiGraph(
		1300, 670,//左上
		1500, 670,//右上（ロード開始時は左端）
		1500, 720,//右下（ロード開始時は左端）
		1300, 720,//左下
		lpImageMng.GetID("./source/UIImage/Load/Icons/Arrows03.png")[0], true);
	DrawFormatStringToHandle(1340, 670, 0xffffff, lpSceneMng.GetFontHandle(), "%d", (LoadVol / 10));
	DrawStringToHandle(1340 + StrWidth, 670, "％", 0xffffff, lpSceneMng.GetFontHandle());

	MyDrawGraph(-1, psH);
}

bool UILoad::GetLoadingEndFlg(void)
{
	return IsLoadingEnd;
}

void UILoad::SetIsNext(bool flg)
{
}


void UILoad::MyDrawGraph(int Imghandle, int pshandle) {
	
	verts[1].pos.x = static_cast<float>(BAR_LEFT_TOP_X + LoadVol);
	verts[3].pos.x = static_cast<float>(BAR_LEFT_TOP_X + LoadVol);
	
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, 0);
	writer[0] += 0.01f;
	//if (writer[0] > 1.5f)
	//{
	//	writer[0] = 0.0f;
	//}
	UpdateShaderConstantBuffer(cBuf);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(pshandle);
	SetUseTextureToShader(0, lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_ProgressBar.png")[0]);
	SetUseTextureToShader(1, lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/Bar_normal.png")[0]);
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	
}

void UILoad::PSInit(void)
{
	for (auto& v : verts) {
		v.rhw = 1.0f;//３Dの場合これが1じゃない
		v.dif = GetColorU8(255, 255, 255, 255);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		//v.spc = GetColorU8(0, 0, 0, 0);
		v.spc = GetColorU8(255, 255, 255, 255);
	}
	// Zの字
		//左上
	verts[0].pos.x = BAR_LEFT_TOP_X;
	verts[0].pos.y = BAR_LEFT_TOP_Y;
	verts[1].pos.x = BAR_LEFT_TOP_X + LoadVol;
	verts[1].pos.y = BAR_LEFT_TOP_Y;
	verts[2].pos.x = BAR_LEFT_TOP_X;
	verts[2].pos.y = BAR_LEFT_TOP_Y + 50;
	verts[3].pos.x = BAR_LEFT_TOP_X + LoadVol;
	verts[3].pos.y = BAR_LEFT_TOP_Y + 50;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	cBuf = CreateShaderConstantBuffer(sizeof(float) * 4);
	writer = static_cast<float*>(GetBufferShaderConstantBuffer(cBuf));
	writer[0] = 0.0f;

}

void UILoad::PSUpdate(void)
{

}

