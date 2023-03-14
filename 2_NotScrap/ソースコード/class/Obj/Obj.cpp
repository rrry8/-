#include "Obj.h"

Obj::Obj()
{
	isMove_ = false;

}

Obj::~Obj()
{
}

void Obj::Draw(void)
{
}

bool Obj::CheckWithinRange(Vector3 pos, Vector3 playerpos)
{
	if (pos.x - MOVE_RANGE < playerpos.x && playerpos.x < pos.x + MOVE_RANGE &&
		pos.y - MOVE_RANGE < playerpos.y && playerpos.y < pos.y + MOVE_RANGE)
	{
		return true;
	}
	return false;
}
bool Obj::CheckWithinRangeXonly(Vector3 pos, Vector3 playerpos)
{
	if (pos.x - MOVE_RANGE < playerpos.x && playerpos.x < pos.x + MOVE_RANGE)
	{
		return true;
	}
	return false;
}

bool Obj::CheckWithinRangeXonly(Vector3 pos, Vector3 playerpos,float size)
{
	if (pos.x - size < playerpos.x && playerpos.x < pos.x + size)
	{
		return true;
	}
	return false;
}



