#pragma once
#include "BaseScene.h"


class LoadScene :
    public BaseScene
{
public:
    LoadScene();
    LoadScene(int num);
    ~LoadScene();

    bool Init(void)override;
    bool Init(int num);
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Load; };

private:
    int stageNum;
    int tutorialNum_;
};

