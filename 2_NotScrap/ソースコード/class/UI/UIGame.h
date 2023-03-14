#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"

class Camera;

class UIGame :
    public UIBase
{
public:
    UIGame();
    ~UIGame();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Game; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    TmxObj tmxObj_;
    Raycast raycast_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    //ÉJÉÅÉâèÓïÒ
    std::unique_ptr<Camera> camera_;

};