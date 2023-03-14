#pragma once
#include "Obj.h"
class Goal :
    public Obj
{
public:
    //コンストラクタ
    Goal(int handle, Vector3 pos);
    //デストラクタ
    ~Goal();

    //初期化
    bool Init(void)override { return false; };    
    bool Init(int handle, Vector3 pos)override;
    //bool Init(std::string name, Vector3 pos);
    //更新
    void Update(void)override;
    void Draw(void)override;

private:
    std::string path_;
    int handle_;
};

