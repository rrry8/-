#pragma once
#include<memory>
#include"../../Geometry.h"

class Controller;
class Player;

//ステージの数
enum class STAGE
{
    None,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Eleven,
    Twelve,
    Tutorial,
    End
};

class Stage
{
public:
    Stage(int cnttype);
    ~Stage();

    // 初期化処理
    bool Init(int cnttype);

    // 更新ステップ
    STAGE Update();

    // 描画処理
    void Draw(void);

    // 解放処理
    void Release(void);

    //選択中のステージの位置を返す
    Vector2 GetPos(void);

private:

    //選択ステージを変更
    void ChangeStage(void);

    //キー情報
    std::unique_ptr<Controller> controller_;

    //選択中のステージ
    STAGE nowStage_;

    //選択中のステージの位置
    Vector2 pos_;

    //文字の大きさの倍率
    double coefficient_;

    //毎フレーム足す倍率
    double addCoefficient_;

    //ふわふわの時間
    double fluffyTime_;
};

