#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../common/Light.h"

class Camera;
class Controller;
class Stage;
class Player;
class Enemy;

class CBufferMng;
class SocketServer;
class Goal;
class OriginalShader;
class GameCBuffer;
class GameUI;
enum class CntType;
class EffekseerEffectHandle;
class Effects;

class GameScene :
    public BaseScene
{
public:
    GameScene();
	GameScene(CntType type);
	~GameScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Game; };

private:
    //頂点情報初期化
    void InitBloomVerts(void);

    //輝度抽出
    void DrawBloom(int renderTarget);

    //ぼかし用
    void DrawGaussianBlur(int renderTarget,int writeRendertarget);  //別のスクリーンに描画
    void DrawGaussianBlur(int renderTarget);                        //同じスクリーンに描画

    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //ステージ情報
    std::unique_ptr<Stage> stage_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    //プレイヤー情報
    std::unique_ptr<Player> player_;

    //敵情報
    std::unique_ptr<Enemy> enemy_;

    //ゴール情報
    std::unique_ptr<Goal> goal_;
    //定数バッファ
    std::unique_ptr<GameCBuffer> cBuffer_;
    //描画用シェーダー
    std::unique_ptr<OriginalShader> origShader_;
    //深度シェーダー
	std::unique_ptr<OriginalShader> depthShader_;
    //UI
    std::unique_ptr<GameUI> gameUI_;
    //スクリーン
    int screenRenderTarget_;
   
    //インベントリの背景ハンドル
    int inventoryBackGroundHandle_;

    //ポストエフェクト用
    std::unique_ptr<OriginalShader> smartPhoneShader_;
    int psPostEffect_;
    float postEffectShader_;
    CntType cntType_;
    //デフォルト座標
    Vector3 defPos_;
    //デフォルトターゲット座標
    Vector3 defTargetPos_;

    //ブルーム用
    BlurData blurX_;                                //横
    BlurData blurY_;                                //縦
    int bloomRenderTarget_;                         //レンダーターゲット
    std::array<VERTEX2DSHADER, 4> bloomVerts_;      //ブルーム頂点情報
    std::array<VERTEX2D, 4> blurVerts_;             //ブラー頂点情報
    std::unique_ptr<OriginalShader> luminanceShader_;   //輝度抽出シェーダ

    //被写界深度用
    int depthRenderTarget_;     //深度値
    int blurRenderTarget_;      //弱ブラー
    int highBlurRenderTarget_;  //強ブラー
    float dofRange_;            //被写界深度範囲
    float dofFocus;             //フォーカス
    float dofLerp_;             //補間距離
    std::unique_ptr<OriginalShader> depthOfFieldShader_;

    //エフェクト
    Effect dust_;
};
