#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"
#include "../Scene/MenuScene.h"


class UIMenu :
    public UIBase
{
public:
    UIMenu();
    ~UIMenu();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Menu; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    TmxObj tmxObj_;
    Raycast raycast_;
    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    MenuState menuState_;//İ’è‰æ–Ê‚Ì¡ŠJ‚¢‚Ä‚¢‚é€–Ú
   // uniqueScene mOptionScene;

};

