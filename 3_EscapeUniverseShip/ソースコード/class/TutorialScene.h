#pragma once
#include<memory>
#include "GameScene.h"
#include"../tmx/TmxObj.h"

class Controller;

class TutorialScene :
    public GameScene
{
public:
    TutorialScene();
    ~TutorialScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;
    void DrawStage(void);

    // 解放処理
    void Release(void) override;

private:
    //Tmx情報
    TmxObj tmxObj_;

    //重力フラグ
    bool gravFlag_;

    //何番目のテキストを表示するか
    int textNum_;

    //キー情報
    std::unique_ptr<Controller> controller_;
};

