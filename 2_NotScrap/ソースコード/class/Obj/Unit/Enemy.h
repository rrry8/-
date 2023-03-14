#pragma once
#include<memory>
#include "Unit.h"
#include "../../../tmx/TmxObj.h"

class Enemy :
    public Unit
{
public:
    Enemy(EnemyType type, Vector3 pos, int number);
    ~Enemy();

    bool Init(void)override;
    void Update(void)override;
    void Update(std::vector<std::shared_ptr<Player>>& PlyaerList)override;
    void Draw(void)override;
    EnemyType GetEnemyType(void) override;

    //方向転換
    void ChangeTurn(Vector3 moveVec);

    //位置を取得
    Vector3 GetPosition(void);

    void SetPosition(Vector3 pos);

    //状態をセットする
    void SetState(State state);

    //状態取得
    State GetState(void);

    //半径取得
    float GetRadius(void);

    //棒の線分をセット
    void SetRodLine(Line line);

    std::string GetHandlePath(void) override;

    //生存情報を返す
    bool GetisAlive(void) override;

    int GetNumber(void)override { return number_; };
private:

    EnemyType type_;

    //xml
    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;

    //棒の視点から敵の位置の長さを半径とする
    float radius_;

    //棒の線分
    Line rodLine_;

    //プレイヤーの位置
    Vector3 playerPos_;

    btRigidBody* Enemybody_;

    int num;
};