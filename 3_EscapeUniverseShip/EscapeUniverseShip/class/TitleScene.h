#pragma once
#include "BaseScene.h"

class Controller;

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

private:
    //サウンド初期化
    void InitSound(void);

    //キー情報
    std::unique_ptr<Controller> controller_;

    //フォント情報
    double fluffyTime_; //時間
    double coefficient_;        //倍率
    double addFluffyTime_;       //足す値

    //タイトルテキストの位置ｙ
    int titlePosY_;
};