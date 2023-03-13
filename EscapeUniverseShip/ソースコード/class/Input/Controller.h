#pragma once
#include<array>
#include<map>
#include"KeyID.h"

enum class ControllerType
{
	Key,
	Pad,
	Max
};

enum class ControllerAge
{
	Now,
	Old,
	Max
};

using AgeBool = std::array<bool, static_cast<size_t>(ControllerAge::Max)>;
using ControllerData = std::map<KeyID, AgeBool>;

class Controller
{
public:
	Controller() {};
	virtual ~Controller() {};
	virtual void Update(void) = 0;
	virtual ControllerType GetControllerType(void) = 0;
	const ControllerData& GetControllerData(void)
	{
		return controllerData_;
	}
private:

protected:
	//コントローラのデータ
	ControllerData controllerData_;
};