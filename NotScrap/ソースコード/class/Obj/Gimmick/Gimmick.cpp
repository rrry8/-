#include "Gimmick.h"

Gimmick::Gimmick()
{
	flg_ = false;
	isAlive_ = false;
	num_ = 0;
}

Gimmick::~Gimmick()
{
}

void Gimmick::SetMoveVec(Vector3 vec)
{
	moveVec_ = vec;
}
