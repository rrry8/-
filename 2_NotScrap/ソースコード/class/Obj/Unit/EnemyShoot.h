#pragma once
#include "Unit.h"
#include"../../Common/Quaternion.h"

class BulletManager;

//方向
enum class DIR
{
    LEFT,
    RIGHT,
    MAX
};

class EnemyShoot :
    public Unit
{
public:
    EnemyShoot(EnemyType type,Vector3 pos,int number);
    ~EnemyShoot();

    //初期化
    bool Init()override;

    //更新
    void Update(void)override;
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList) override;
    int GetNumber(void)override { return number_; };
    //描画
    void Draw(void)override;

    //位置取得
    Vector3 GetPosition(void);

    std::string GetHandlePath(void) override;

    //敵の種類を返す(EnemyShoot)
    EnemyType GetEnemyType(void) override;

    //生存情報を返す
    bool GetisAlive(void) override;

    std::shared_ptr<BulletManager> GetBulletData(void);
private:
    //回転
    void Rotate(void);
    //種類
    EnemyType type_;

    //サインカーブのカウンター
    double stepSin_;

    //プレイヤーの位置
    Vector3 playerPos_;

    //爆発エフェクト
    Effect explosion_;

    //構造体
    Transform trans_;

    //向いている方向
    DIR dir_;

    //回転時の目標値
    Quaternion goalRot_;

    //回転する時間
    float stepRotTime_;

    //弾管理
    std::shared_ptr<BulletManager> bullets_;

};
