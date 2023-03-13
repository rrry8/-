#include <Dxlib.h>
#include "Unit.h"
#include "../../Input//KeyInput.h"
Unit::Unit()
{
	
}

Unit::~Unit()
{
}


//�X�s�[�h�̃Z�b�g
Vector3 Unit::SetSpeed(SpeedMode speedmode_)
{
	//���̂Ƃ��됔���𒼐ړ���Ă���
	switch (speedmode_)
	{
	case SpeedMode::Non:
		movePow_ = { 0,0,0 };
		break;
	case SpeedMode::SlowSlow:
		movePow_ = { 5,5,0 };
		break;
	case SpeedMode::Slow:
		movePow_ = { 10.0f,10.0f,0 };
		break;
	case SpeedMode::Normal:
		movePow_ = { 100.0f,100.0f ,0 };
		break;
	case SpeedMode::Fast:
		movePow_ = { 20,20 ,0};
		break;
	case SpeedMode::Max:
		break;
	default:
		return movePow_;
		break;
	}
	return movePow_;
}
//�����X�s�[�h�̃Z�b�g
//float Unit::SetFallSpeed(SpeedMode speedmode_)
//{
//	//���̂Ƃ��됔���𒼐ړ���Ă���
//	switch (speedmode_)
//	{
//	case SpeedMode::Slow:
//		FallSpeed_ = 0.000f;
//		break;
//	case SpeedMode::Normal:
//		FallSpeed_ = 0.4f;
//		break;
//	case SpeedMode::Fast:
//		FallSpeed_ = 0.5f;
//		break;
//	case SpeedMode::Max:
//		break;
//	default:
//		return FallSpeed_;
//		break;
//	}
//	return FallSpeed_;
//}




