#pragma once
#include"../../Geometry.h"

class TmxObj;

class Gimmick
{
public:
    Gimmick();
    virtual ~Gimmick();

    // 初期化処理
    virtual void Init(TmxObj& tmx) = 0;

    // 更新ステップ
    virtual void Update(void) = 0;

    // 描画処理
    virtual void Draw(Vector2 offset) = 0;

    // 解放処理
    virtual void Release(void) = 0;

};
