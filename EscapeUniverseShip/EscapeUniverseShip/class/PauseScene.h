#pragma once
#include "BaseScene.h"

//選択肢
enum class PAUSESELECT
{
    Resume,
    Select,
    MAX
};

class Controller;

class PauseScene :
    public BaseScene
{
public:
    PauseScene(UniqueScene scene, int id);
    ~PauseScene();

    // 初期化処理
    void Init(void) override;

    //更新
    UniqueScene Update(UniqueScene scene) override;

    //描画
    void DrawScreen(void) override;

    //解放
    void Release(void) override;

private:
    //キー情報
    std::unique_ptr<Controller> controller_;

    //前のシーン
    UniqueScene ownerScene_;

    //フォント情報
    double fluffyTime_;     //ふわふわ時間
    double coefficient_;    //倍率
    double addFluffy_;      //足す値

    //現在の選択肢
    PAUSESELECT nowSelect_;

    //背景
    int gameBG_;
};
