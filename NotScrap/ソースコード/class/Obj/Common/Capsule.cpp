#include <DxLib.h>
#include "Capsule.h"

Capsule::Capsule()
{
	mRadius = 0.0f;
	mRelativePosTop = { 0.0f, 0.0f, 0.0f };
	mRelativePosDown = { 0.0f, 0.0f, 0.0f };
}

void Capsule::Draw(void)
{

	// ã‚Ì‹…‘Ì
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, mRadius, 5, COLOR, COLOR, false);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, mRadius, 5, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ðŒq‚®ü(X+)
	dir = { 1.0f, 0.0f, 0.0f };
	s = VAdd(pos1, VScale(dir, mRadius));
	e = VAdd(pos2, VScale(dir, mRadius));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ðŒq‚®ü(X-)
	dir = { -1.0f, 0.0f, 0.0f };
	s = VAdd(pos1, VScale(dir, mRadius));
	e = VAdd(pos2, VScale(dir, mRadius));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ðŒq‚®ü(Z+)
	dir = { 0.0f, 0.0f, 1.0f };
	s = VAdd(pos1, VScale(dir, mRadius));
	e = VAdd(pos2, VScale(dir, mRadius));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ðŒq‚®ü(Z-)
	dir = { 0.0f, 0.0f, -1.0f };
	s = VAdd(pos1, VScale(dir, mRadius));
	e = VAdd(pos2, VScale(dir, mRadius));
	DrawLine3D(s, e, COLOR);

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(GetCenter(), 5.0f, 10, COLOR, COLOR, true);

}

void Capsule::SetRelativePosTop(VECTOR pos)
{
	mRelativePosTop = pos;
}

void Capsule::SetRelativePosDown(VECTOR pos)
{
	mRelativePosDown = pos;
}

VECTOR Capsule::GetPosTop(void)
{
	return mRelativePosTop;
}

VECTOR Capsule::GetPosDown(void)
{
	return mRelativePosDown;
}

//VECTOR Capsule::GetRelativePos(VECTOR pos)
//{
//	VECTOR relativePos = mTransformParent->quaRot.PosAxis(pos);
//	return VAdd(mTransformParent->pos, relativePos);
//}

float Capsule::GetRadius(void)
{
	return mRadius;
}

void Capsule::SetRadius(float radius)
{
	mRadius = radius;
}

float Capsule::GetHeight(void)
{
	return mRelativePosTop.y;
}

VECTOR Capsule::GetCenter(void)
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}

//Capsule Capsule::Copy(Transform* base)
//{
//	Capsule ret = Capsule(base);
//	ret.SetRelativePosDown(mRelativePosDown);
//	ret.SetRelativePosTop(mRelativePosTop);
//	ret.SetRadius(mRadius);
//	return ret;
//}

//void Capsule::SetTransformParent(Transform* parent)
//{
//	mTransformParent = parent;
//}
