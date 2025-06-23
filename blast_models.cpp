// 文件: blast_models.cpp
// 描述: 炸药冲击波物理模型函数实现
//------------------------------------------------------------------------------
#include "blast_models.h"
#include <cmath>
#include <array>
#include <algorithm>

//------------------------------------------------------------------------------
// 示例 K-B 表数据 (仅 3 条用于演示线性插值)
//------------------------------------------------------------------------------
struct KBEntry { float Z, Ps, td, alpha; };
static constexpr std::array<KBEntry, 3> g_kb_table = { {
    {0.2f, 1586.0f,  0.30f, 1.90f},
    {1.0f,  185.0f,  1.70f, 1.85f},
    {5.0f,    9.8f, 13.00f, 1.80f}
} };

//------------------------------------------------------------------------------
// Hopkinson 缩尺距离
//------------------------------------------------------------------------------
inline  float blast_scaled_distance(float R, float W) {
    return R / std::cbrt(W);
}

//------------------------------------------------------------------------------
// K-B 表线性插值查询
//------------------------------------------------------------------------------
BlastKBResult blast_kb_lookup(float Z) {
    // 边界处理
    if (Z <= g_kb_table.front().Z) {
        auto& e = g_kb_table.front();
        return { e.Ps, e.td, e.alpha };
    }
    if (Z >= g_kb_table.back().Z) {
        auto& e = g_kb_table.back();
        return { e.Ps, e.td, e.alpha };
    }
    // 查找所在区间
    auto it = std::upper_bound(g_kb_table.begin(), g_kb_table.end(), Z,
        [](float v, const KBEntry& e) { return v < e.Z; });
    size_t i1 = std::distance(g_kb_table.begin(), it);
    size_t i0 = i1 - 1;
    auto& a = g_kb_table[i0];
    auto& b = g_kb_table[i1];
    float t = (Z - a.Z) / (b.Z - a.Z);
    auto lerp = [t](float A, float B) { return A + t * (B - A); };
    return { lerp(a.Ps, b.Ps), lerp(a.td, b.td), lerp(a.alpha, b.alpha) };
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