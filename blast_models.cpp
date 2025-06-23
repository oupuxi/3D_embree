// �ļ�: blast_models.cpp
// ����: ըҩ���������ģ�ͺ���ʵ��
//------------------------------------------------------------------------------
#include "blast_models.h"
#include <cmath>
#include <array>
#include <algorithm>

//------------------------------------------------------------------------------
// ʾ�� K-B ������ (�� 3 ��������ʾ���Բ�ֵ)
//------------------------------------------------------------------------------
struct KBEntry { float Z, Ps, td, alpha; };
static constexpr std::array<KBEntry, 3> g_kb_table = { {
    {0.2f, 1586.0f,  0.30f, 1.90f},
    {1.0f,  185.0f,  1.70f, 1.85f},
    {5.0f,    9.8f, 13.00f, 1.80f}
} };

//------------------------------------------------------------------------------
// Hopkinson ���߾���
//------------------------------------------------------------------------------
inline  float blast_scaled_distance(float R, float W) {
    return R / std::cbrt(W);
}

//------------------------------------------------------------------------------
// K-B �����Բ�ֵ��ѯ
//------------------------------------------------------------------------------
BlastKBResult blast_kb_lookup(float Z) {
    // �߽紦��
    if (Z <= g_kb_table.front().Z) {
        auto& e = g_kb_table.front();
        return { e.Ps, e.td, e.alpha };
    }
    if (Z >= g_kb_table.back().Z) {
        auto& e = g_kb_table.back();
        return { e.Ps, e.td, e.alpha };
    }
    // ������������
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
// Friedlander ����
//------------------------------------------------------------------------------
inline float blast_friedlander(float t, float Ps, float td, float beta) {
    if (t < 0.0f || t > td) return 0.0f;
    float xi = t / td;
    return Ps * (1.0f - xi) * std::exp(-beta * xi);
}

//------------------------------------------------------------------------------
// ָ������˥��
//------------------------------------------------------------------------------
inline float blast_exp_decay(float E0, float K, float d) {
    return E0 * std::exp(-K * d);
}

//------------------------------------------------------------------------------
// Rankine�CHugoniot ǿ�ȨC�ٶȹ��㹫ʽ
//------------------------------------------------------------------------------
inline float blast_particle_velocity(float Ps, float rho, float c0) {
    return c0 * std::sqrt(1.0f + 6.0f * Ps / (rho * c0 * c0));
}

//------------------------------------------------------------------------------
// �迹����ϵ�� Cr = sqrt(c2 / c1)
//------------------------------------------------------------------------------
inline float blast_impedance_correction(float c1, float c2) {
    return std::sqrt(c2 / c1);
}