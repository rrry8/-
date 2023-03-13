#pragma once
#include <array>
#include "Curve.h"

     //拘束条件を満たす曲線加減速の軌道を生成するクラス
     
     //移動距離の拘束条件を満たす曲線加速軌道を生成する
     //各時刻 $t$ における躍度 $j(t)$，加速度 $a(t)$，速度 $v(t)$，位置 $x(t)$
     //を提供する
     //最大加速度 $a_{\max}$ と始点速度 $v_s$
     //など拘束次第では目標速度に達することができない場合があるので注意する
     
class Designer
{
public:
	Designer(const float j_max, const float a_max, const float v_sat,
        const float v_start, const float v_target, const float dist,
        const float x_start = 0, const float t_start = 0);
    Designer();

    //引数の拘束条件から曲線を生成する．
    //この関数によって，すべての変数が初期化される．(漏れはない)
    void reset(const float j_max, const float a_max, const float v_sat,
        const float v_start, const float v_target, const float dist,
        const float x_start = 0, const float t_start = 0);


    /**
        * @brief 時刻 $t$ における躍度 $j$
        * @param t 時刻[s]
        * @return j 躍度[m/s/s/s]
        */
    float j(const float t) const
    {
        if (t < t2)
            return ac.j(t - t0);
        else
            return dc.j(t - t2);
    }
    /**
     * @brief 時刻 $t$ における加速度 $a$
     * @param t 時刻 [s]
     * @return a 加速度 [m/s/s]
     */
    float a(const float t) const
    {
        if (t < t2)
            return ac.a(t - t0);
        else
            return dc.a(t - t2);
    }
    /**
     * @brief 時刻 $t$ における速度 $v$
     * @param t 時刻 [s]
     * @return v 速度 [m/s]
     */
    float v(const float t) const
    {
        if (t < t2)
            return ac.v(t - t0);
        else
            return dc.v(t - t2);
    }
    /**
     * @brief 時刻 $t$ における位置 $x$
     * @param t 時刻 [s]
     * @return x 位置 [m]
     */
    float x(const float t) const
    {
        if (t < t2)
            return x0 + ac.x(t - t0);
        else
            return x3 - dc.x_end() + dc.x(t - t2);
    }
    /**
     * @brief 終点時刻 [s]
     */
    float t_end() const { return t3; }
    /**
     * @brief 終点速度 [m/s]
     */
    float v_end() const { return dc.v_end(); }
    /**
     * @brief 終点位置 [m]
     */
    float x_end() const { return x3; }
    /**
     * @brief 境界の時刻
     */
    float t_0() const { return t0; }
    float t_1() const { return t1; }
    float t_2() const { return t2; }
    float t_3() const { return t3; }

    //void printCsv(const float t_interval = 0.001f) const;
    //void printCsv(std::ostream& os, const float t_interval = 0.001f) const;

    const std::array<float, 8> getTimeStamp() const
    {
        return
        {
            {
            t0 + ac.t_0(),
            t0 + ac.t_1(),
            t0 + ac.t_2(),
            t0 + ac.t_3(),
            t2 + dc.t_0(),
            t2 + dc.t_1(),
            t2 + dc.t_2(),
            t2 + dc.t_3(),
            }
        };
    }
protected:
    float t0, t1, t2, t3; /**< @brief 境界点の時刻 [s] */
    float x0, x3;         /**< @brief 境界点の位置 [m] */
    Curve ac, dc; /**< @brief 曲線加速，曲線減速オブジェクト */
};

