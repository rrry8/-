#pragma once
#include "BaseScene.h"
#include "../../tmx/TmxObj.h"

constexpr auto SUB_ITEM_LEFT = 658;
constexpr auto SUB_ITEM_LEFT_VOL = 960;
constexpr auto KEYNAME_LEFT = 958;


enum class OptionState
{
    KeyConfig,
    Volume,
    Tips,
    Quit,
    Max
};

class OptionScene :
    public BaseScene
{
public:
    OptionScene(uniqueScene& oldscene);
    ~OptionScene();
    OptionState GetOptionState();
private:

    bool Init(void)override;
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Option; };
    void KeyChange(int subItem);
    
    Vector2Int MousePos;//マウスの座標
    uniqueScene mOldScene;//ひとつ前のシーン格納用
    TmxObj tmxObj_;
    bool keyChangeMode_;//キー変更状態に入っていたらtrue
    bool keyResetFlg;//キーがリセットされたらtrue
    int cnt;//警告表示用カウント
    OptionState state_;//設定画面の状態
    int vol;//音量格納用変数
};

