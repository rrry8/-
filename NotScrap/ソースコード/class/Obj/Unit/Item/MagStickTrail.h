#pragma once

#include<vector>
#include<list>
#include"../../../Common/Geometry.h"
#include"../../../Input/Controller.h"

enum class MAGSTATE;

class MagStickTrail
{
public:
    MagStickTrail();
    ~MagStickTrail();

    //初期化
    bool Init(void);
    //更新
    void Update(Line playerpos);
    //描画
    void Draw(void);
    //頂点データを取得
    std::vector<VERTEX2DSHADER>& GetVertexData(void);

    //中心点取得
    Vector2 GetCenterPos(void);

private:

    //頂点データを設定
    void SetVertex(const std::list<Vector2>& ePos);

    //軌跡のデータをvectorで管理
    std::vector<VERTEX2DSHADER> trail_;

    //位置を記憶してから描画(削除や挿入を繰り返すためlist)
    std::list<Vector2> posList_;
};
