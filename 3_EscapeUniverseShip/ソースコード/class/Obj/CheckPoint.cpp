#include<DxLib.h>
#include "CheckPoint.h"
#include"../Other/ImageMng.h"

//位置情報

CheckPoint::CheckPoint()
{
	checkPointFlag_ = false;
	tileImage_ =-1;
	greenCheckImage_ = -1;
	redCheckImage_ = -1;
}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Init(TmxObj& tmx)
{
	checkPointFlag_ = false;
	tileImage_ = LoadGraph("./Image/Tile.png");
	//タイルの中から取得
	greenCheckImage_ = DerivationGraph(261, 192, 20, 64, tileImage_);
	redCheckImage_ = DerivationGraph(293, 192, 20, 64, tileImage_);
}

void CheckPoint::Update(void)
{
}

bool CheckPoint::CheckCheckPoint(int gid)
{
	//指定されたIDは描画しない
	if (gid == 307 || gid == 308 ||
		gid == 336 || gid == 337 ||
		gid == 365 || gid == 366 ||
		gid == 394 || gid == 395
		)
	{
		return true;
	}
	return false;
}

void CheckPoint::ChangeID(int& gid)
{
	gid -= 2;
}

void CheckPoint::Draw(Vector2 offset)
{
}

void CheckPoint::Release(void)
{
	//タイル
	DeleteGraph(tileImage_);
	//赤い中間ポイント
	DeleteGraph(redCheckImage_);
}

void CheckPoint::SetFlag(bool flag)
{
	checkPointFlag_ = flag;
}

bool CheckPoint::GetFlag(void)
{
	return checkPointFlag_;
}
