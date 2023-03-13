#pragma once
#include "BaseScene.h"

class Controller;

//選択肢
enum class SELECT
{
    Retry,
    Select,
    MAX
};

class ResultScene :
    public BaseScene
{
public:
    ResultScene(UniqueScene scene,int id);
    ~ResultScene();

    // 初期化処理
    void Init(void) override;

    //更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

private:
    //キー情報
    std::unique_ptr<Controller> controller_;

    SELECT nowSelect_;

    UniqueScene ownerScene_;

    //フォント情報
    double fluffyTime_; //ふわふわ時間
    double con_;        //倍率
    double addCoeff_;       //足す値

    //背景
    int gameBG_;
};