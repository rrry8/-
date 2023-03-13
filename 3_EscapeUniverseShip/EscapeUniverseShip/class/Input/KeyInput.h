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
	//�L�[�f�[�^
	std::array<char, 256U> keyData_;
	//���̓e�[�u��
	std::map<KeyID, unsigned int> inputTable_;
};

