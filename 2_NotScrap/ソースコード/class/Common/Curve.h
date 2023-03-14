#pragma once

class Curve
{
public:
	Curve(const float j_max, const float a_max, const float v_start, const float v_end);
	Curve();
    ~Curve();
    //引数の拘束条件から曲線を生成する．
    //この関数によって，すべての変数が初期化される．(漏れはない)
    void reset(const float j_max, const float a_max, const float v_start, const float v_end);

   
    /**
         * @brief 時刻 $t$ における躍度 $j$
         * @param t 時刻 [s]
         * @return j 躍度 [m/s/s/s]
         */
    float j(const float t) const
    {
        if (t <= t0)
            return 0;
        else if (t <= t1)
            return jm;
        else if (t <= t2)
            return 0;
        else if (t <= t3)
            return -jm;
        else
            return 0;
    }
    /**
         * @brief 時刻 $t$ における加速度 $a$
         * @param t 時刻 [s]
         * @return a 加速度 [m/s/s]
         */
    float a(const float t) const
    {
        if (t <= t0)
            return 0;
        else if (t <= t1)
            return jm * (t - t0);
        else if (t <= t2)
            return am;
        else if (t <= t3)
            return -jm * (t - t3);
        else
            return 0;
    }
    /**
     * @brief 時刻 $t$ における速度 $v$
     * @param t 時刻 [s]
     * @return v 速度 [m/s]
     */
    float v(const float t) const
    {
        if (t <= t0)
            return v0;
        else if (t <= t1)
            return v0 + jm / 2 * (t - t0) * (t - t0);
        else if (t <= t2)
            return v1 + am * (t - t1);
        else if (t <= t3)
            return v3 - jm / 2 * (t - t3) * (t - t3);
        else
            return v3;
    }
    /**
     * @brief 時刻 $t$ における位置 $x$
     * @param t 時刻 [s]
     * @return x 位置 [m]
     */
    float x(const float t) const
    {
        if (t <= t0)
            return x0 + v0 * (t - t0);
        else if (t <= t1)
            return x0 + v0 * (t - t0) + jm / 6 * (t - t0) * (t - t0) * (t - t0);
        else if (t <= t2)
            return x1 + v1 * (t - t1) + am / 2 * (t - t1) * (t - t1);
        else if (t <= t3)
            return x3 + v3 * (t - t3) - jm / 6 * (t - t3) * (t - t3) * (t - t3);
        else
            return x3 + v3 * (t - t3);
    }
    /**
     * @brief 終点時刻 [s]
     */
    float t_end() const { return t3; }
    /**
     * @brief 終点速度 [m/s]
     */
    float v_end() const { return v3; }
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

    //走行距離から達しうる終点速度を算出する関数
    static float calcVelocityEnd(const float j_max, const float a_max,
        const float vs, const float vt, const float d);
    //走行距離から達しうる最大速度を算出する関数
    static float calcVelocityMax(const float j_max, const float a_max,
        const float vs, const float ve, const float d);
    //速度差から変位を算出する関数
    static float calcMinDistance(const float j_max, const float a_max,
        const float v_start, const float v_end);
    
protected:
    float jm;             /**< @brief 躍度定数 [m/s/s/s] */
    float am;             /**< @brief 加速度定数 [m/s/s] */
    float t0, t1, t2, t3; /**< @brief 時刻定数 [s] */
    float v0, v1, v2, v3; /**< @brief 速度定数 [m/s] */
    float x0, x1, x2, x3; /**< @brief 位置定数 [m] */
};

