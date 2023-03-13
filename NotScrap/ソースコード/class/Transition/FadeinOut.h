#pragma once
#include "TransitionScene.h"
class FadeinOut :
    public TransitionScene
{
public:
    FadeinOut(double limitTime,uniqueScene beforScene, uniqueScene afterScene);
    FadeinOut(double limitTime, Transition tras, uniqueScene afterScene);
    ~FadeinOut();

private:
    double limitTime_;

    bool UpdateTransition(double delta)override;
    /*void Draw(double delta);*/
    void DrawOwnScreen(double delta)override;
    Transition tras_;
    bool ff = false;
};

