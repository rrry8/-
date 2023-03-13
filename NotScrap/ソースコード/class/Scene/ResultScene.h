#pragma once
#include "BaseScene.h"
#include "../Obj/Obj.h"
//#include "../../tmx/TmxObj.h"
struct time_UI
{
    int s;
    int s_one;
    int s_ten;
    int m = s;
    int m_one;
    int m_ten;
    int h = m;
    int h_one;
    int h_ten;
};
class Camera;
class Player;
class Coin;

using CoinList = std::list<std::shared_ptr<Coin>>;


class ResultScene :
    public BaseScene
{
public:
    ResultScene();
    ResultScene(CoinList list,double clearTime);
    ~ResultScene();
private:

    bool Init(void)override;
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Result; };

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    CoinList coinList_;
    bool getCoinflg_;
    TmxObj tmxObj_;
    float clearTime_;
    time_UI time_;

    std::unique_ptr<AnimationController> anim_;
    std::unique_ptr<AnimationController> anim2_;

    //エフェクト
    Effect Cracker_;    //クラッカー
    Effect Star_;       //星
    int effectTime_;
    double exRate;
    bool exFlg_;


    //マウスアイコンのサイズ
    Vector2Int graphSize_;

    bool colorChangeFlag_;

};

