#pragma once
#include "../../Obj.h"

class AnimationController;

class Bullet :
	public Obj
{
public:
	Bullet(Vector3 direction, Vector3 enemypos,int num);
	~Bullet();

	bool Init(void) override;
	bool Init(Vector3 direction, Vector3 enemypos);
	void Update(void) override;
	void Draw(void) override;

	//生存情報取得
	bool GetAliveFlag(void);

	//生存情報セット
	void SetAliveFlag(bool alive);

	//生存チェック(true:破棄/false:破棄しない)
	bool CheckAlive(void);

	//弾の位置を返す
	Vector3 GetBulletPos(void);

	//弾の飛ぶ方向をスピードダウンさせる・最終的には反転
	void DownSpeed(void);

	//何番目か取得
	int GetNumber(void);

	//反転フラグを取得
	bool GetInversionFlag(void);

private:
	//弾が飛ぶ方向 * スピード
	Vector3 movePower_;

	//弾が飛ぶ方向(正規化済み)
	Vector3 direction_;

	//弾の位置
	Vector3 pos_;

	//生存情報(生存:true/生存していない:false)
	bool aliveFlag_;

	//弾の生存時間
	double aliveTime_;

	//個数(コンストラクタの引数から代入)
	int num_;

	//進行方向フラグ(反転:true/そのまま:false)
	bool inversionFlag_;

	//反転の回数カウント(フレーム)
	double stepSpeedDownTime_;

	//爆発エフェクト
	Effect explosion_;

	//構造体
	Transform trans_;

	//回転時の目標値
	Quaternion goalRot_;

	//回転する時間
	double stepRotTime_;
};

