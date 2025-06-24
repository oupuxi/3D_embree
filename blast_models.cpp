// 文件: blast_models.cpp
// 描述: 炸药冲击波物理模型函数实现
//------------------------------------------------------------------------------
#include "blast_models.h"
#include <cmath>
#include <array>
#include <algorithm>



//------------------------------------------------------------------------------
// Hopkinson 缩尺距离
//------------------------------------------------------------------------------
inline  float blast_scaled_distance(float R, float W) {
    return R / std::cbrt(W);
}

//------------------------------------------------------------------------------
// K-B 表线性插值查询
//------------------------------------------------------------------------------
inline BlastKBResult blast_kb_lookup(float Z){
    const auto& tbl = g_kb_table;
    // 边界处理
    if (Z <= tbl.front().Z) {
        auto& e = tbl.front();
        return { Z, e.Ps, e.td, e.beta, e.I, e.Us, e.ta };
    }
    if (Z >= tbl.back().Z) {
        auto& e = tbl.back();
        return { Z, e.Ps, e.td, e.beta, e.I, e.Us, e.ta };
    }

    // 找到第一个 Z_i >= Z
    auto it = std::upper_bound(tbl.begin(), tbl.end(), Z,
        [](float v, const KBEntry& e) { return v < e.Z; });
    size_t i1 = it - tbl.begin();
    size_t i0 = i1 - 1;
    auto& a = tbl[i0];
    auto& b = tbl[i1];

    // 插值
    float t = (Z - a.Z) / (b.Z - a.Z);
    auto lerp = [t](float A, float B) { return A + (B - A) * t; };

    return {
        Z,
        lerp(a.Ps,   b.Ps),   // 峰压
        lerp(a.td,   b.td),   // 正相位持续时间
        lerp(a.beta, b.beta), // 形状因子
        lerp(a.I,    b.I),    // 冲量
        lerp(a.Us,   b.Us),   // 波前速度
        lerp(a.ta,   b.ta)    // 峰值到达时间
    };
}



//------------------------------------------------------------------------------
// Friedlander 波形
//------------------------------------------------------------------------------
inline float blast_friedlander(float t, float Ps, float td, float beta) {
    if (t < 0.0f || t > td) return 0.0f;
    float xi = t / td;
    return Ps * (1.0f - xi) * std::exp(-beta * xi);
}

//------------------------------------------------------------------------------
// 指数能量衰减
//------------------------------------------------------------------------------
inline float blast_exp_decay(float E0, float K, float d) {
    return E0 * std::exp(-K * d);
}

//------------------------------------------------------------------------------
// Rankine–Hugoniot 强度–速度估算公式
//------------------------------------------------------------------------------
inline float blast_particle_velocity(float Ps, float rho, float c0) {
    return c0 * std::sqrt(1.0f + 6.0f * Ps / (rho * c0 * c0));
}

//------------------------------------------------------------------------------
// 阻抗修正系数 Cr = sqrt(c2 / c1)
//------------------------------------------------------------------------------
inline float blast_impedance_correction(float c1, float c2) {
    return std::sqrt(c2 / c1);
}