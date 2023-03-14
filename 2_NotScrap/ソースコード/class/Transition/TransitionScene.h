#pragma once
#include "../../class/Scene/BaseScene.h"

// 明示的にキャストする必要がある
enum class Transition
{
    CrossFade,
    FadeInOut,
    In,
    Out,
    Max
};


class TransitionScene :
    public BaseScene
{
public:
    TransitionScene(uniqueScene beforScene,uniqueScene afterScene);
    TransitionScene(uniqueScene afterScene);
    ~TransitionScene();
private:
    bool Init(void) override;
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    virtual bool UpdateTransition(double delta) = 0;

protected:
    // 子供のクラスは触れるが外からは触れない
    Scene GetSceneID(void) override final{ return Scene::Transition; };

    uniqueScene beforScene_;
    uniqueScene afterScene_;

    double count;
};

