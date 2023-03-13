#include "MagStickTrail.h"
#include"MagStick.h"
#include"../../../Common/ImageMng.h"
#include "../../../Scene/SceneManager.h"
#include "../../../UI/mouse/MouseIcon.h"
#include"../../../UI/mouse/MouseIcon.h"

constexpr int INDEX_MAX = 24;			//頂点の最大数
constexpr float ROD_LENGTH = 350.0f;	//棒の最長さ
constexpr float CENTER_POS_X = 960.0f;	//中心点ｘ
constexpr float CENTER_POS_Y = 670.0f;	//中心点ｙ

MagStickTrail::MagStickTrail()
{
	Init();
}

MagStickTrail::~MagStickTrail()
{
}

bool MagStickTrail::Init(void)
{
	trail_.resize(INDEX_MAX * 2);
	return false;
}

void MagStickTrail::Update(Line player)
{
	//マウス位置
	int mx, my;
	GetMousePoint(&mx, &my);
	//マウスアイコン半分サイズ
	float mSize = MouseIcon::GetMouseIconSize() / 2.0f;

	Vector2 mPos = { static_cast<float>(mx) + mSize,static_cast<float>(my) + mSize };

	//プレイヤーの位置からマウス位置へのベクトル
	Vector2 vecN = Vector2(mPos.x - CENTER_POS_X, mPos.y - CENTER_POS_Y).Normalized();

	//プレイヤーの位置からマウス位置へのベクトルを格納
	posList_.push_front(vecN);

	//頂点数を超えている場合、古い頂点は削除
	if (posList_.size() >= INDEX_MAX)
	{
		posList_.pop_back();
	}

	//頂点情報更新
	SetVertex(posList_);

}

void MagStickTrail::Draw(void)
{
}

std::vector<VERTEX2DSHADER>& MagStickTrail::GetVertexData(void)
{
	return trail_;
}

Vector2 MagStickTrail::GetCenterPos(void)
{
	return { CENTER_POS_X,CENTER_POS_Y };
}

void MagStickTrail::SetVertex(const std::list<Vector2>& ePos)
{
	Vector2 screenSize = Vector2(static_cast<float>(lpSceneMng.GetScreenSize().x), static_cast<float>(lpSceneMng.GetScreenSize().y));

	int minusAlpha = static_cast<int>(255.0f / static_cast<float>(ePos.size()));
	COLOR_U8 diffuse = GetColorU8(255, 255, 255, 255);

	auto itr = ePos.begin();

	for (int i = 0; i < ePos.size(); i++)
	{
		//根本
		trail_[i * 2].dif = diffuse;
		trail_[i * 2].spc = GetColorU8(255, 255, 255, 255);
		trail_[i * 2].pos.x = CENTER_POS_X;
		trail_[i * 2].pos.y = CENTER_POS_Y;
		trail_[i * 2].pos.z = 0.0f;
		trail_[i * 2].u = trail_[i * 2].pos.x / screenSize.x;
		trail_[i * 2].v = trail_[i * 2].pos.y / screenSize.y;
		trail_[i * 2].su = 0.0f;
		trail_[i * 2].sv = 0.0f;
		trail_[i * 2].rhw = 1.0f;

		//剣先
		trail_[i * 2 + 1].dif = diffuse;
		trail_[i * 2 + 1].spc = GetColorU8(255, 255, 255, 255);
		trail_[i * 2 + 1].pos.x = CENTER_POS_X + itr->x * ROD_LENGTH;
		trail_[i * 2 + 1].pos.y = CENTER_POS_Y + itr->y * ROD_LENGTH;
		trail_[i * 2 + 1].pos.z = 0.0f;
		trail_[i * 2 + 1].u = trail_[i * 2 + 1].pos.x / screenSize.x;
		trail_[i * 2 + 1].v = trail_[i * 2 + 1].pos.y / screenSize.y;
		trail_[i * 2 + 1].su = 0.0f;
		trail_[i * 2 + 1].sv = 0.0f;
		trail_[i * 2 + 1].rhw = 1.0f;
		itr++;

		diffuse.a -= minusAlpha;
	}

}
