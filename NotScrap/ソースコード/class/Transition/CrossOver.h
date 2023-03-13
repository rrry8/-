#pragma once
#include "TransitionScene.h"

class CrossOver :
    public TransitionScene
{
public:
    CrossOver(double limitTime, uniqueScene beforScene, uniqueScene afterScene);
    ~CrossOver();
private:
    double limitTime_;

    //bool Init(void) override;
    bool UpdateTransition(double delta);
    void DrawOwnScreen(double delta)override;

};

