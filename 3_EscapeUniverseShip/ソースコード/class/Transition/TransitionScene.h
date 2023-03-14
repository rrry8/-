#pragma once
#include "../BaseScene.h"

//���ʂ̑傫���̎��̃^�C���̉��Z�l
constexpr float NORMAL_OFFSET_WHEEL_RIGHT_X = 36.0f;
constexpr float NORMAL_OFFSET_WHEEL_LEFT_X = -45.0f;
constexpr float NORMAL_OFFSET_WHEEL_Y = 19.0f;

enum class Transition
{
	CrossFade,
	FadeInOut,
	MAX
};

class TransitionScene :
	public BaseScene
{
public:
	TransitionScene(UniqueScene beforeScene,UniqueScene afterScene);
	~TransitionScene();

private:
	void Init(void) override;
	UniqueScene Update(UniqueScene ownScene) override;
	virtual bool UpdateTransition(double delta) = 0;
	void Release(void) override;
protected:

	//�O�̃V�[��
	UniqueScene beforeScene_;

	//���̃V�[��
	UniqueScene afterScene_;

	double count_;
};

