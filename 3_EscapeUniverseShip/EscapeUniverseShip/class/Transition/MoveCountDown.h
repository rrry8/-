#pragma once
#include<string>
#include "TransitionScene.h"

class Player;
class Camera;
class Controller;

class MoveCountDown :
    public TransitionScene
{
public:
    MoveCountDown(double limitTime,std::string stage,std::string nonstage,Vector2 pos, UniqueScene beforeScene, UniqueScene afterScene);
    ~MoveCountDown();
private:
    //遷移更新
    bool UpdateTransition(double delta) override;
    //テキストの移動
    void MoveText(bool flag);

    //スクリーンの描画
    void DrawScreen() override;
    //プレイヤーの描画
    void DrawPlayer(void);

    //プレイヤー情報
    std::unique_ptr<Player> player_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    //キー情報
    std::unique_ptr<Controller> controller_;

    //タイヤの角度
    double wheelAngle_;

    //ステージテキストのパス
    std::string stagePath_;     //色つき
    std::string nonStagePath_;  //色なし

    //テキストの位置
    Vector2 textPos_;

    //テキストの移動方向
    Vector2 textVelocity_;

    //点滅時間
    double blinkTime_;

    double stepStopTime_;
};

