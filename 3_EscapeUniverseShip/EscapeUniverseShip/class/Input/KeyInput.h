#pragma once
#include"Controller.h"

class KeyInput :
	public Controller
{
public:
	KeyInput();
	~KeyInput();

	virtual void Update(void) override;
	virtual ControllerType GetControllerType(void) override { return ControllerType::Key; };
private:
	//キーデータ
	std::array<char, 256U> keyData_;
	//入力テーブル
	std::map<KeyID, unsigned int> inputTable_;
};

