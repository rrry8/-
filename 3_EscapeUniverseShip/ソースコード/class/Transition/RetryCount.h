#pragma once
#include<memory>
#include "TransitionScene.h"

class Controller;

class RetryCount :
    public TransitionScene
{
public:
    RetryCount(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene);
    ~RetryCount();
private:
    bool UpdateTransition(double delta) override;

    void DrawScreen() override;

    double fluffyTime_;
    double coefficient_;
    double addFluffyTime_;

    //�L�[���
    std::unique_ptr<Controller> controller_;
};

