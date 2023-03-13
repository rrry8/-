#include<DxLib.h>
#include "UIBase.h"
#include"../Scene/BaseScene.h"
#include"../Input/Controller.h"

UIBase::UIBase()
{
    GetDrawScreenSize(&screenSize_.x, &screenSize_.y);
    screenUIID_ = MakeScreen(screenSize_.x, screenSize_.y, true);
    GetMousePoint(&MousePos.x, &MousePos.y);
    MousePos.x += 32;//âÊëúÇ∏ÇÍÇƒÇÈï™ï‚ê≥
    MousePos.y += 32;
    offset_ = {400,0};
    IsHitItem = 0;
   
}

UIBase::~UIBase()
{
}

void UIBase::Draw(double delta)
{
    DrawGraph(0, 0, screenUIID_, true);
}

bool UIBase::KeyTrgDown(InputID id)
{
    if (cntData_[id][static_cast<int>(Trg::Now)] &&
        !cntData_[id][static_cast<int>(Trg::Old)])
    {
        return true;
    }
    return false;
}

bool UIBase::KeyTrgUp(InputID id)
{
    if (cntData_[id][static_cast<int>(Trg::Old)] &&
        !cntData_[id][static_cast<int>(Trg::Now)])
    {
        return true;
    }
    return false;
}

