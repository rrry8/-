#pragma once
#include <functional>
#include "../Obj.h"
#include "../../Common/Raycast.h"
#include "../CheckHit.h"
#include "../../Input/Controller.h"

using InputUnique = std::unique_ptr<Controller>;

class Player;

//スピード情報
enum class SpeedMode
{
    Non,
    SlowSlow,
    Slow,
    Normal,
    Fast,
    UP,
    Max
};

class Unit :
    public Obj
{
public:
    Unit();
    ~Unit();

    //敵更新
    virtual void Update(std::vector<std::shared_ptr<Player>>& PlyaerList) = 0;
    virtual std::string GetHandlePath() = 0;
    virtual EnemyType GetEnemyType(void)=0;
    virtual bool GetisAlive(void) = 0;
    virtual int GetNumber(void) = 0;
private:
   
protected:

    //スピードのセット
    Vector3 SetSpeed(SpeedMode speedmode_);

    Raycast raycast_;
    State state_;
    Vector3 moveVecPow_;
    SpeedMode speedmode_ = SpeedMode::Normal;
    SpeedMode FallSpeedmode = SpeedMode::Normal;
    Vector3 AllMovedVecPow_;
    rapidxml::xml_node<>* FallNode_;
    rapidxml::xml_node<>* SpecialHitNode_;

    Vector3 Moved_Pos_;

    Vector3 initPos_;

    //生存情報
    bool aliveFlag_;

    //数情報
    int number_;
};

