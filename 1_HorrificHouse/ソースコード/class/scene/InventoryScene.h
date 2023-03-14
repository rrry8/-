#pragma once
#include "BaseScene.h"
#include <vector>
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"
#include "../common/SocketServer.h"

class Controller;
class MouseIcon;
class Item;
class OriginalShader;
struct InventoryObj;
enum class ItemID;
enum class ControllerType;

constexpr int WEIGHTS_NUM = 8;              //重みの数
constexpr double  PHOTO_LERP_TIME = 0.15;   //移動補間時間

struct Photo
{
    int handle = -1;        //画像ハンドル
    bool addFlag = false;   //追加フラグ
    bool expansionFlag = false;
    Vector2 addPos;         //カーソルが合っている場合の追加位置
    double stepLerpTime = PHOTO_LERP_TIME;  //補間時間

};

class InventoryScene :
    public BaseScene
{
public:
    InventoryScene(UniqueScene ownScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo);
    ~InventoryScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;
    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Inventory; };

private:
    //ブラー関係初期化
    void InitBlur(void);

    //頂点初期化
    void InitVerts(void);

    //ブラーの重み計算
    void CalcBlurWeights(float* weights);

    //写真とマウスの位置の当たり判定
    void CollMousePosAndPhoto(void);

    //背景描画
    void DrawBackGround(void);

    //写真描画
    void DrawPhoto(std::string name,Vector2 pos,Vector2 imageSize);

    //インベントリ初期化
    void InitInventory(void);
    //インベントリ更新
    void UpdateInventory(void);
    //インベントリ描画
    void DrawInventory(void);
    //テーブルの初期化
    void InitTable(void);

    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //フレームバッファや背景描画用
    std::array<VERTEX2D, 4> totalBlurVerts_;
    
    //テーブル
    std::map<std::string , std::string> inventoryTable_;    //インベントリの外枠のテーブル
    std::map<std::string, std::string> optionTable_;        //操作のテーブル
    std::map<std::string, Vector3> itemTable_;              //アイテムの枠テーブル
    std::map<ItemID, std::string> explanationTable_;        //説明のテーブル

    // アイテムリスト
    std::vector<InventoryObj> inventoryList_;

    //写真情報
    std::map<std::string,Photo> photoTbl_;         //ベクター
    std::unique_ptr<OriginalShader> photoOrigShader_; //描画用シェーダクラス
    std::unique_ptr<OriginalShader> origShader_;

    int psPhotoHandle_;                               //ピクセルシェーダ

    //ブラー情報
    BlurData blurX_;               //横
    BlurData blurY_;               //縦
    int psBlurXandYHandle_;        //ピクセルシェーダ
    int mainRenderTarget_;         //メインレンダリングターゲット

    //前のシーン(主にゲームシーン)
    UniqueScene beforeScene_;

    //背景画像(前のシーンの画面)
    int backGroundHandle_;

    int font_;
    //tmx
    TmxObj tmxObj_;
    //当たったID
    ItemID hitItemID_;
    //ヒント
    bool isDrawTips_;
};
