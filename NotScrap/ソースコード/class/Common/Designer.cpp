#include <iostream>
#include <algorithm>
#include "Designer.h"
/**
         * @brief 初期化付きコンストラクタ
         *
         * @param j_max     最大躍度の大きさ [m/s/s/s]，正であること
         * @param a_max     最大加速度の大きさ [m/s/s], 正であること
         * @param v_sat     飽和速度の大きさ [m/s]，正であること
         * @param v_start   始点速度 [m/s]
         * @param v_target  目標速度 [m/s]
         * @param v_end     終点速度 [m/s]
         * @param dist      移動距離 [m]
         * @param x_start   始点位置 [m] (オプション)
         * @param t_start   始点時刻 [s] (オプション)
         */
Designer::Designer(const float j_max, const float a_max, const float v_sat, const float v_start, const float v_target, const float dist, const float x_start, const float t_start)
{
    reset(j_max, a_max, v_sat, v_start, v_target, dist, x_start, t_start);
}

Designer::Designer()
{
    t0 = t1 = t2 = t3 = x0 = x3 = 0;
}
/**
         * @brief 引数の拘束条件から曲線を生成する．
         * この関数によって，すべての変数が初期化される．(漏れはない)
         *
         * @param j_max     最大躍度の大きさ [m/s/s/s]，正であること
         * @param a_max     最大加速度の大きさ [m/s/s], 正であること
         * @param v_sat     飽和速度の大きさ [m/s]，正であること
         * @param v_start   始点速度 [m/s]
         * @param v_target  目標速度 [m/s]
         * @param v_end     終点速度 [m/s]
         * @param dist      移動距離 [m]
         * @param x_start   始点位置 [m] (オプション)
         * @param t_start   始点時刻 [s] (オプション)
         */
void Designer::reset(const float j_max, const float a_max, const float v_sat, const float v_start, const float v_target, const float dist, const float x_start, const float t_start)
{
    /* 最大速度の仮置き */
    float v_max = dist > 0 ? std::max({ v_start, v_sat, v_target })
        : std::min({ v_start, -v_sat, v_target });
    /* 走行距離から終点速度$v_e$を算出 */
    float v_end = v_target;
    const auto dist_min =
        Curve::calcMinDistance(j_max, a_max, v_start, v_end);
    // logd << "dist_min: " << dist_min << std::endl;
    if (std::abs(dist) < std::abs(dist_min))
    {
        //logd << "vs -> ve != vt" << std::endl;
        /* 目標速度$v_t$に向かい，走行距離$d$で到達し得る終点速度$v_e$を算出 */
        v_end =
            Curve::calcVelocityEnd(j_max, a_max, v_start, v_target, dist);
        v_max = v_end; //< 走行距離の拘束を満たすため，飽和速度まで加速できない
        // logd << "ve: " << v_end << std::endl;
    }
    /* 曲線を生成 */
    ac.reset(j_max, a_max, v_start, v_max); //< 加速
    dc.reset(j_max, a_max, v_max, v_end);   //< 減速
    /* 飽和速度まで加速すると走行距離の拘束を満たさない場合の処理 */
    const auto d_sum = ac.x_end() + dc.x_end();
    if (std::abs(dist) < std::abs(d_sum))
    {
        //logd << "vs -> vm -> ve" << std::endl;
        /* 走行距離から最大速度$v_m$を算出; 下記v_maxは上記v_max以下になる */
        v_max = Curve::calcVelocityMax(j_max, a_max, v_start, v_end, dist);
        /* 無駄な減速を回避 */
        v_max = dist > 0 ? std::max({ v_start, v_max, v_end })
            : std::min({ v_start, v_max, v_end });
        ac.reset(j_max, a_max, v_start, v_max); //< 加速
        dc.reset(j_max, a_max, v_max, v_end);   //< 減速
    }
    /* 各定数の算出 */
    x0 = x_start;
    x3 = x_start + dist;
    t0 = t_start;
    t1 = t0 + ac.t_end(); //< 曲線加速終了の時刻
    t2 = t0 + ac.t_end() +
        (dist - ac.x_end() - dc.x_end()) / v_max; //< 等速走行終了の時刻
    t3 = t0 + ac.t_end() + (dist - ac.x_end() - dc.x_end()) / v_max +
        dc.t_end(); //< 曲線減速終了の時刻
   /* 出力のチェック */
    const float e = 0.01f; //< 数値誤差分
    bool show_info = false;
    /* 終点速度 */
    if (std::abs(v_start - v_end) > e + std::abs(v_start - v_target))
    {
        std::cerr << "Error: Velocity Target!" << std::endl;
        show_info = true;
    }
    /* 最大速度 */
    if (std::abs(v_max) >
        e + std::max({ v_sat, std::abs(v_start), std::abs(v_end) }))
    {
        std::cerr << "Error: Velocity Saturation!" << std::endl;
        show_info = true;
    }
    /* タイムスタンプ */
    if (!(t0 <= t1 + e && t1 <= t2 + e && t2 <= t3 + e))
    {
        //loge << "Error: Time Point Relationship!" << std::endl;
        show_info = true;
    }

}
