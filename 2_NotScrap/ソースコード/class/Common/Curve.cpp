#include "Curve.h"
#include <cmath>    //< for std::sqrt, std::cbrt, std::pow
#include <complex>  //< for std::complex
#include <iostream> //< for std::cout
/**
      * @brief 初期化付きのコンストラクタ．
      *
      * @param j_max   最大躍度の大きさ [m/s/s/s], 正であること
      * @param a_max   最大加速度の大きさ [m/s/s], 正であること
      * @param v_start 始点速度 [m/s]
      * @param v_end   終点速度 [m/s]
      */
Curve::Curve(const float j_max, const float a_max, const float v_start,const float v_end)
{
    reset(j_max, a_max, v_start, v_end);
}

Curve::Curve()
{
    jm = am = t0 = t1 = t2 = t3 = v0 = v1 = v2 = v3 = x0 = x1 = x2 = x3 = 0;
}

Curve::~Curve()
{
}
/**
        * @brief 引数の拘束条件から曲線を生成する．
        * この関数によって，すべての変数が初期化される．(漏れはない)
        *
        * @param j_max   最大躍度の大きさ [m/s/s/s], 正であること
        * @param a_max   最大加速度の大きさ [m/s/s], 正であること
        * @param v_start 始点速度 [m/s]
        * @param v_end   終点速度 [m/s]
        */
void Curve::reset(const float j_max, const float a_max, const float v_start, const float v_end)
{
    /* 符号付きで代入 */
    am = (v_end > v_start) ? a_max : -a_max; //< 最大加速度の符号を決定
    jm = (v_end > v_start) ? j_max : -j_max; //< 最大躍度の符号を決定
    /* 初期値と最終値を代入 */
    v0 = v_start; //< 代入
    v3 = v_end;   //< 代入
    t0 = 0;       //< ここでは初期値をゼロとする
    x0 = 0;       //< ここでは初期値はゼロとする
    /* 速度が曲線となる部分の時間を決定 */
    const auto tc = a_max / j_max;
    /* 等加速度直線運動の時間を決定 */
    const auto tm = (v3 - v0) / am - tc;
    /* 等加速度直線運動の有無で分岐 */
    if (tm > 0)
    {
        /* 速度: 曲線 -> 直線 -> 曲線 */
        t1 = t0 + tc;
        t2 = t1 + tm;
        t3 = t2 + tc;
        v1 = v0 + am * tc / 2;                //< v(t) を積分
        v2 = v1 + am * tm;                    //< v(t) を積分
        x1 = x0 + v0 * tc + am * tc * tc / 6; //< x(t) を積分
        x2 = x1 + v1 * tm;                    //< x(t) を積分
        x3 = x0 + (v0 + v3) / 2 * (t3 - t0); //< v(t) グラフの台形の面積より
    }
    else
    {
        /* 速度: 曲線 -> 曲線 */
        const auto tcp = std::sqrt((v3 - v0) / jm); //< 変曲までの時間
        t1 = t2 = t0 + tcp;
        t3 = t2 + tcp;
        v1 = v2 = (v0 + v3) / 2; //< 対称性より中点となる
        x1 = x2 = x0 + v1 * tcp + jm * tcp * tcp * tcp / 6; //< x(t) を積分
        x3 = x0 + 2 * v1 * tcp; //< 速度 v(t) グラフの面積より
    }
}
/**
         * @brief 走行距離から達しうる終点速度を算出する関数
         *
         * @param j_max 最大躍度の大きさ [m/s/s/s], 正であること
         * @param a_max 最大加速度の大きさ [m/s/s], 正であること
         * @param vs    始点速度 [m/s]
         * @param vt    目標速度 [m/s]
         * @param d     走行距離 [m]
         * @return ve   終点速度 [m/s]
         */
float Curve::calcVelocityEnd(const float j_max, const float a_max, const float vs, const float vt, const float d)
{
    /* 速度が曲線となる部分の時間を決定 */
    const auto tc = a_max / j_max;
    /* 最大加速度の符号を決定 */
    const auto am = (vt > vs) ? a_max : -a_max;
    const auto jm = (vt > vs) ? j_max : -j_max;
    /* 等加速度直線運動の有無で分岐 */
    const auto d_triangle = (vs + am * tc / 2) * tc; //< distance @ tm == 0
    const auto v_triangle = jm / am * d - vs;        //< v_end @ tm == 0
    // logd << "d_tri: " << d_triangle << std::endl;
    // logd << "v_tri: " << v_triangle << std::endl;
    if (d * v_triangle > 0 && std::abs(d) > std::abs(d_triangle))
    {
        /* 曲線・直線・曲線 */
        //logd << "v: curve - straight - curve" << std::endl;
        /* 2次方程式の解の公式を解く */
        const auto amtc = am * tc;
        const auto D = amtc * amtc - 4 * (amtc * vs - vs * vs - 2 * am * d);
        const auto sqrtD = std::sqrt(D);
        return (-amtc + (d > 0 ? sqrtD : -sqrtD)) / 2;
    }
    /* 曲線・曲線 (走行距離が短すぎる) */
    /* 3次方程式を解いて，終点速度を算出;
     * 簡単のため，値を一度すべて正に変換して，計算結果に符号を付与して返送 */
    const auto a = std::abs(vs);
    const auto b = (d > 0 ? 1 : -1) * jm * d * d;
    const auto aaa = a * a * a;
    const auto c0 = 27 * (32 * aaa * b + 27 * b * b);
    const auto c1 = 16 * aaa + 27 * b;
    if (c0 >= 0)
    {
        /* ルートの中が非負のとき */
        //logd << "v: curve - curve (accel)" << std::endl;
        const auto c2 = std::cbrt((std::sqrt(c0) + c1) / 2);
        return (d > 0 ? 1 : -1) * (c2 + 4 * a * a / c2 - a) / 3; //< 3次方程式の解
    }
    else
    {
        /* ルートの中が負のとき */
        //logd << "v: curve - curve (decel)" << std::endl;
        const auto c2 = std::pow(std::complex<float>(c1 / 2, std::sqrt(-c0) / 2),
            float(1) / 3);
        return (d > 0 ? 1 : -1) * (c2.real() * 2 - a) / 3; //< 3次方程式の解
    }
}
/**
         * @brief 走行距離から達しうる最大速度を算出する関数
         *
         * @param j_max 最大躍度の大きさ [m/s/s/s], 正であること
         * @param a_max 最大加速度の大きさ [m/s/s], 正であること
         * @param vs    始点速度 [m/s]
         * @param ve    終点速度 [m/s]
         * @param d     走行距離 [m]
         * @return vm   最大速度 [m/s]
         */
float Curve::calcVelocityMax(const float j_max, const float a_max, const float vs, const float ve, const float d)
{
    /* 速度が曲線となる部分の時間を決定 */
    const auto tc = a_max / j_max;
    const auto am = (d > 0) ? a_max : -a_max; /*< 加速方向は移動方向に依存 */
    /* 2次方程式の解の公式を解く */
    const auto amtc = am * tc;
    const auto D = amtc * amtc - 2 * (vs + ve) * amtc + 4 * am * d +
        2 * (vs * vs + ve * ve);
    if (D < 0)
    {
        /* 拘束条件がおかしい */
        //loge << "Error! D = " << D << " < 0" << std::endl;
        /* 入力のチェック */
        if (vs * ve < 0)
            //loge << "Invalid Input! vs: " << vs << ", ve: " << ve << std::endl;
            return vs;
    }
    const auto sqrtD = std::sqrt(D);
    return (-amtc + (d > 0 ? sqrtD : -sqrtD)) / 2; //< 2次方程式の解
}
/**
        * @brief 速度差から変位を算出する関数
        *
        * @param j_max   最大躍度の大きさ [m/s/s/s], 正であること
        * @param a_max   最大加速度の大きさ [m/s/s], 正であること
        * @param v_start 始点速度 [m/s]
        * @param v_end   終点速度 [m/s]
        * @return d      変位 [m]
        */
float Curve::calcMinDistance(const float j_max, const float a_max, const float v_start, const float v_end)
{
    /* 符号付きで代入 */
    const auto am = (v_end > v_start) ? a_max : -a_max;
    const auto jm = (v_end > v_start) ? j_max : -j_max;
    /* 速度が曲線となる部分の時間を決定 */
    const auto tc = a_max / j_max;
    /* 等加速度直線運動の時間を決定 */
    const auto tm = (v_end - v_start) / am - tc;
    /* 始点から終点までの時間を決定 */
    const auto t_all =
        (tm > 0) ? (tc + tm + tc) : (2 * std::sqrt((v_end - v_start) / jm));
    return (v_start + v_end) / 2 * t_all; //< 速度グラフの面積により
}
