#pragma once
#include<vector>
#include<chrono>
#include"Gimmick.h"
#include"../../Geometry.h"

class Camera;

//ドラムの構造体
struct DrumObj
{
    Vector2 pos;     //位置
    double accel;    //加速度
    double angle;    //角度
    double time;     //点滅の秒数
    int alpha;       //α値
    bool alive = false;    //生存フラグ
    std::chrono::system_clock::time_point elapTime;    //当たってから経った時間
};

class Drum:
    public Gimmick
{
public:
    Drum();
    ~Drum();

    // 初期化処理
    void Init(TmxObj& tmx) override;

    //状況リセット
    void Reset(TmxObj& tmx);

    // 更新ステップ
    void Update(void) override;

    // 更新ステップ
    void UpdateAlive(TmxObj& tmx);

    // 描画処理
    void Draw(Vector2 offset) override;

    // 解放処理
    void Release(void) override;

    //ドラム缶のＩＤかどうかチェック
    bool CheckDrum(int gid);

private:

    //タイルハンドル
    int tileImage_;

    //ドラム缶のハンドル
    int drumImage_;

    //ドラム缶の情報
    std::vector<DrumObj> drum_;

    //デルタ
    std::chrono::system_clock::time_point nowTime_, oldTime_;
};