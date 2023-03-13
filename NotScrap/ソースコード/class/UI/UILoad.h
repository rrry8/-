#pragma once
#include<array>
#include"../Common/Geometry.h"
#include "UIBase.h"
#define STR_POINT_X (screenSize_.x - 440)//NowLoadingのX座標
#define STR_POINT_Y (screenSize_.y - 64)//NowLoadingのY座標
//----loadingBarの頂点の座標---------
#define BAR_LEFT_TOP_X (500)//左上
#define BAR_LEFT_TOP_Y (600)
#define BAR_LEFT_DOWN_X (500)//左下
#define BAR_LEFT_DOWN_Y (650)
#define BAR_RIGHT_TOP_X (1500)//右上
#define BAR_RIGHT_TOP_Y (600)
#define BAR_RIGHT_DOWN_X (1500)//右下
#define BAR_RIGHT_DOWN_Y (650)
//-----------------------------------

class UILoad ://ロード画面用UIのクラス
    public UIBase
{
public:
    UILoad();
    ~UILoad();

private:
    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Load; }
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    void MyDrawGraph(int Imghandle, int pshandle);
    void PSInit(void);
    void PSUpdate(void);
    int LoadVol;//バーの幅
    int LoadingTime;//経過時間
    bool IsLoadingEnd;//ロードが終了したらtrue
    std::array<VERTEX2DSHADER, 4> verts;
    int cBuf;
    float* writer;
    int psH;//ピクセルシェーダ用ハンドル
};


