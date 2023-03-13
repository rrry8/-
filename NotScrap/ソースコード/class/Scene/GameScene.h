#pragma once
#include <memory>
#include <list>
#include<array>
#include "BaseScene.h"
#include "../Common/Geometry.h"
#include "../Obj/Obj.h"
#include"../../tmx/TmxObj.h"

constexpr int COIN_MAX = 3;
constexpr int DOOR_MAX = 2;

class Camera;
class Player;
class Enemy;
class EnemyShoot;
class Coin;
class PressurePlate;
class PressureDoor;
class Gear;
class MagBox;
class Goal;

class GameScene :
	public BaseScene

{
public:
	GameScene();
	~GameScene();

	State GetPlayerState(void);
	Vector3 GetShakePlayer(void);
	int GetScreenID(void);
	std::vector<VERTEX2DSHADER>& GetTrailData(void);

	//カメラの位置取得
	Vector3 GetCameraPos(void);

	//カメラの注視点位置取得
	Vector3 GetCameraTargetPos(void);
private:
	//関数---------------------------------------
	bool Init(void)override;											//初期化
	void Init3D(void);

	uniqueScene Update(double delta, uniqueScene ownScene)override;		//更新

	void DrawOwnScreen(double delta)override;							//描画

	Scene GetSceneID(void) override { return Scene::Game; };			//シーンIDを返す

	//プレイヤー分解処理
	std::shared_ptr<Player> DismemberPlayer(std::shared_ptr<Player>& player);

	//プレイヤー死亡処理
	std::shared_ptr<Player> DeathPlayer(std::shared_ptr<Player>& player);

	//プレイヤーの更新
	uniqueScene UpdatePlayer(uniqueScene& ownScene);

	//オブジェクト生成
	void CreateObject(void);
	void CreateGimmickObject(void);	//ギミック
	void CreateEneymyObject(void);	//敵
	void CreateCoinObject(void);	//コイン
	void CreateGoalObject(void);	//ゴール
	void CreatePlayerObject(void);	//プレイヤー

	//範囲内チェック(範囲内：true/範囲外：false)
	bool CheckWithinRange(Vector3 objpos, Vector3 playerpos);

	//オブジェクト更新
	uniqueScene UpdateObject(uniqueScene& ownScene);

	//オブジェクト描画
	void DrawObject(void);

	//歪み描画
	void DrawDistortion(int ps, int screen, int tex1, const std::vector<VERTEX2DSHADER>& vertex);

	//変数----------------------------------------

	//クラス情報
	std::unique_ptr<PressurePlate> pressurePlate_;		//感圧版
	std::unique_ptr<PressureDoor> pressureDoor_;		//ドア
	std::vector<std::unique_ptr<Goal>> goalList_;		//ゴール
	std::vector<std::shared_ptr<Player>> playerList_;	//プレイヤー
	EneymyList enemyList_;								//敵
	GimmickList gimmickList_;							//ギミック

	MagBoxList magBoxList_;								//磁石の箱
	CoinList coinList_;									//コイン
	std::unique_ptr<Camera> camera_;					//カメラ

	//ライトハンドル
	int lightHandle_;

	int ShadowMapHandle_;
	//クリア時間
	double clearTime_;
	double bestClearTime_;
	//フェードインするフラグ
	bool fadeFlag_;

	//シェーダハンドル
	int psCrackedScreen_;		//画面割れ

	//シェーダ関係
	int ps;		//ピクセル
	int renderTarget_;	//レンダリングターゲット

	//エフェクト
	Effect dust_;
	float Adjust;

	Vector2 pos;

	//シェーダ関係
	int vsDistortionHandle_;	//歪み(ピクセルシェーダ)
	int psDistortionHandle_;	//歪み(頂点シェーダ)
	//定数
	int cBuff_;
	float* writer_;
};
