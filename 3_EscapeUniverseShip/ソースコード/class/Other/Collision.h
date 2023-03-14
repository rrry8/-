#pragma once
#include<list>
#include"../../Geometry.h"

using Collision = std::pair<Vector2, Vector2>;
using CollList = std::list<Collision>;

//first:位置 second:yの方向
using CollisionFIRST = std::pair<Vector2, Vector2>;

//second:判定するかどうかフラグ
using Collision2 = std::pair<CollisionFIRST, bool>;

//上記をlistにする
using DrumCollList = std::list<Collision2>;