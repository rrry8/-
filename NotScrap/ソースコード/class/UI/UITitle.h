#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"

#define ITEM_LEFT_BOX (1550)
#define ITEM_LEFT_SINGLE (1718)
#define ITEM_LEFT_MULTI (1718)
#define ITEM_LEFT_OPTION (1718)
#define ITEM_LEFT_QUIT (1718)

#define ItemMax (4)

class Camera;


class UITitle :
    public UIBase
{
public:
    UITitle();
    ~UITitle();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Title; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;

    TmxObj tmxObj_;
    Raycast raycast_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    //カメラ情報
    std::unique_ptr<Camera> camera_;
    int modelH;//タイトルの看板のモデルハンドル
    bool hitItem[ItemMax];//当たった項目がtrue
    int BrightVol;//点滅の明るさ
    bool BrightFlg;//点滅フラグ
    double exRate;//拡縮の量
    bool exFlg;//緑の枠の拡縮フラグ
    bool exFlg_;//文字の拡縮フラグ
    double moveVol[ItemMax];//項目の枠の緑の部分を動かすための変数
    int fastOffset_[ItemMax];//項目を順にスライドするための変数
    bool IsNext_;//タイトルで最初に何か入力されたらtrue
    bool IsNextScene_;//タイトルで何か項目が選択された場合にtrue
};


