#pragma once
#include "Gimmick.h"

class CheckPoint :
    public Gimmick
{
public:
    CheckPoint();
    ~CheckPoint();

    // 初期化処理
    void Init(TmxObj& tmx) override;

    // 更新ステップ
    void Update(void) override;

    //チェックポイントのIDチェック
    bool CheckCheckPoint(int gid);

    //IDの変更
    void ChangeID(int& gid);

    // 描画処理
    void Draw(Vector2 offset) override;

    // 解放処理
    void Release(void) override;

    //中間ポイント到達セット
    void SetFlag(bool flag);

    //中間ポイントに到達しているかゲット
    bool GetFlag(void);

private:
    //画像ハンドル
    int tileImage_;           //タイル
    int redCheckImage_;       //中間ポイント(通過前)
    int greenCheckImage_;     //中間ポイント(通過後)

    //チェックポイントに到達しているかどうか
    bool checkPointFlag_;
};
