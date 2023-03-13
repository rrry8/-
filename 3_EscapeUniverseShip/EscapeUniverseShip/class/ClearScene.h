#pragma once
#include "BaseScene.h"
#include<string>

class Controller;

class ClearScene :
    public BaseScene
{
public:
    ClearScene();
    ClearScene(std::string mapnum);
    ~ClearScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

private:

    //フォント情報
    double fluffyTime_; //ふわふわする時間
    double con_;        //DrawRotaGraphの大きさの変数
    double addFluffy_; //ふわふわするときに足す値

    //背景のハンドル
    int gameBG_;

    //ステージの名前
    std::string stageName_;

    //キー情報
    std::unique_ptr<Controller> controller_;
};
