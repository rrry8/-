#pragma once
#include <memory>
#include "Controller.h"
#include "KeyInput.h"
#include "PadInput.h"
#define lpCntMng ControllerMng::GetInstance()

class ControllerMng
{
public:
	static ControllerMng& GetInstance()
	{
		static ControllerMng s_Instance;
		return s_Instance;
	}

	ControllerMng();
	~ControllerMng();
	void SetUseController(CntType type);
	Controller* GetController();
	bool GetKeyChangeFlg(void);
	void SetKeyChangeFlg(bool flg);
private:
	Controller* controller_;

};

