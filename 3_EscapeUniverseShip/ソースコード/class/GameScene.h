#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include"../tmx/TmxObj.h"

class Player;
class Camera;
class Drum;
class CheckPoint;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    GameScene(std::string map,float addRot,int num);
	~GameScene();

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
    //鋸回転
    bool DrawRotSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset);

    //Tmx情報
    TmxObj tmxObj_;
    std::string stageNum_;
protected:
    //のこぎり描画
    virtual bool DrawSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset);

    //プレイヤー情報
    std::unique_ptr<Player> player_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    //ドラム缶情報
    std::unique_ptr<Drum> drum_;

    //中間ポイント情報
    std::shared_ptr<CheckPoint> check_;

    //のこぎりの回転(アニメーション用)
    double angle_;

    //のこぎりの位置回転用
    double rotSawAngle_;
    //回転加算値
    float addSawRot_;

    int sawImage_;      //のこぎりのハンドル
    int sawRotImage_;   //位置回転するのこぎり

    //タイルのハンドル
    int tileImage_;

    //遷移時の背景ハンドル
    int gameBG_;
};