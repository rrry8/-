#pragma once
#include "BaseScene.h"
//#include <vector>
#include<memory>
#include "../Obj/Obj.h"
#include"../../tmx/TmxObj.h"
#include "btBulletDynamicsCommon.h"
#include "../Obj/Gimmick/Gimmick.h"
#include "../Obj/Stage.h"

static constexpr int PLAYER_MAX_NUM = 30;			//生成するプレイヤーの最大数

static constexpr int PLAYER_OFFSET = 40;			//生成するプレイヤーの最大数

class Camera;
class Crane;
class Scrap;
//struct OriginalShader;
struct StackPlayer
{
    int handle = 0;
    btRigidBody* body;
    Vector3 pos_ = Vector3Zero;
    bool isAlive =false;
    int time = 0;
};

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();
private:

    //初期化
    bool Init(void)override;
    //更新
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    //描画
    void DrawOwnScreen(double delta)override;
    //シーンID取得
    Scene GetSceneID(void) override { return Scene::Title; };

    //モデルのロード
    bool LoadModel(void);
    //プレイヤー生成
    void CreatePlyaers(void);
    //プレイヤー更新
    void UpdatePlyaers(void);
    //プレイヤー破棄
    void DestroyPlayers(void);
    //プレイヤーのアニメーション
    void AnimationPlayer(StackPlayer& player_, std::unique_ptr<AnimationController>& animController_, rapidxml::xml_node<>* node);

    //カメラ情報
    std::unique_ptr<Camera> camera_;
    //クレーン情報(箱)
    std::unique_ptr<Crane> crane_;
    //スクラップ
    std::list <std::unique_ptr<Scrap>> scrap_;
    //
    StackPlayer players_[PLAYER_MAX_NUM];

    //総フレーム数
    int frame_;

    btVector3 posw_;
    //プレイヤー
    StackPlayer player_;
    StackPlayer player2_;
    bool CreateFlag_;
    //クレーンの移動開始
    bool MoveStartCraneFlag_;
    //アニメーション
    std::unique_ptr<AnimationController> animController_;
    std::unique_ptr<AnimationController> animController2_;

    //xml
    std::vector<char>TitleVec_;
    rapidxml::xml_node<>* AnimationPlayerNode_;
    rapidxml::xml_document<> TitleDoc;
    rapidxml::xml_node<>* AnimationPlayerNode2_;
    //影マップハンドル
    int ShadowMapHandle_;

    TextureMap texmap;
};

