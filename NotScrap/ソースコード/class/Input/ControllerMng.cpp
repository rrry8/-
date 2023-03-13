#include "ControllerMng.h"

ControllerMng::ControllerMng()
{
	
}

ControllerMng::~ControllerMng()
{
}

void ControllerMng::SetUseController(CntType type)
{
	switch (type)
	{
		
	case CntType::Key:
		controller_ = new KeyInput();
		break;
	case CntType::Pad:
		controller_ = new PadInput();
		break;
	case CntType::Max:
		break;
	default:
		break;
	}
}

Controller* ControllerMng::GetController()
{
	
	return controller_;
}

bool ControllerMng::GetKeyChangeFlg(void)
{
	return controller_->GetKeyChangeFlg();
}

void ControllerMng::SetKeyChangeFlg(bool flg)
{
	controller_->SetKeyChangeFlg(flg);
}
