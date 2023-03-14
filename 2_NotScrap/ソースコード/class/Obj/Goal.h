#pragma once
#include "Obj.h"
class Goal :
    public Obj
{
public:
    //�R���X�g���N�^
    Goal(int handle, Vector3 pos);
    //�f�X�g���N�^
    ~Goal();

    //������
    bool Init(void)override { return false; };    
    bool Init(int handle, Vector3 pos)override;
    //bool Init(std::string name, Vector3 pos);
    //�X�V
    void Update(void)override;
    void Draw(void)override;

private:
    std::string path_;
    int handle_;
};

