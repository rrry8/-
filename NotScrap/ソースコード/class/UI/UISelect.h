#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"
#include"SceneUIManager.h"

#define ITEMNUM 4
struct time_
{
    int s =0;
    int s_one = 0;
    int s_ten = 0;
    int m = s;
    int m_one = 0;
    int m_ten = 0;
    int h = m;
    int h_one = 0;
    int h_ten = 0;
};
struct bestTime_
{
    int s = 0;
    int s_one = 0;
    int s_ten = 0;
    int m = s;
    int m_one = 0;
    int m_ten = 0;
    int h = m;
    int h_one = 0;
    int h_ten = 0;
};

class Camera;
struct UIEffect
{
    Vector2Int offset_;//ずらす
    int extendVol_[ITEMNUM];//拡大
    bool move_L;//左右移動フラグ
    bool move_R;//左右移動フラグ
    bool extend_[ITEMNUM];//拡縮フラグ
};
class UISelect :
    public UIBase
{
public:
    UISelect();
    ~UISelect();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Select; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    void OffsetUpdate(void);
    void TimeUpdate(int stageNum);
    void CoinStateUpdate(int stageNum);
    TmxObj tmxObj_;
    Raycast raycast_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    //カメラ情報
    std::unique_ptr<Camera> camera_;

    UIEffect uiEffect_;
    int slideOffset_x;
    int clearTime_;
    int bestClearTime_;
    time_ time_;
    bestTime_ bestTime_;
    bool getCoin_[COIN_MAX];
};