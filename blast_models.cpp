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
struct KBEntry { float Z, Ps, td, beta; };
static constexpr std::array<KBEntry,3> g_kb_table = {{
    {0.2f, 1586.0f,  0.30f, 1.90f},
    {1.0f,  185.0f,  1.70f, 1.85f},
    {5.0f,    9.8f, 13.00f, 1.80f}
}};

//------------------------------------------------------------------------------
// Hopkinson ���߾���
//------------------------------------------------------------------------------
inline constexpr float blast_scaled_distance(float R, float W) {
    return R / std::cbrt(W);
}

//------------------------------------------------------------------------------
// K-B �����Բ�ֵ��ѯ
//------------------------------------------------------------------------------
BlastKBResult blast_kb_lookup(float Z) {
    // �߽紦��
    if (Z <= g_kb_table.front().Z) {
        auto &e = g_kb_table.front();
        return {e.Ps, e.td, e.beta};
    }
    if (Z >= g_kb_table.back().Z) {
        auto &e = g_kb_table.back();
        return {e.Ps, e.td, e.beta};
    }
    // ������������
    auto it = std::upper_bound(g_kb_table.begin(), g_kb_table.end(), Z,
        [](float v, const KBEntry &e){ return v < e.Z; });
    size_t i1 = std::distance(g_kb_table.begin(), it);
    size_t i0 = i1 - 1;
    auto &a = g_kb_table[i0];
    auto &b = g_kb_table[i1];
    float t = (Z - a.Z) / (b.Z - a.Z);
    auto lerp = [t](float A, float B){ return A + t * (B - A); };
    return { lerp(a.Ps, b.Ps), lerp(a.td, b.td), lerp(a.beta, b.beta) };
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
// �ʵ��ٶȹ���
//------------------------------------------------------------------------------
inline float blast_particle_velocity(float Ps, float rho, float c0) {
    return Ps / (rho * c0);
}

//------------------------------------------------------------------------------
// ���迹 R/T ϵ������
//------------------------------------------------------------------------------
inline std::pair<float,float> blast_impedance_rt(float Z1, float Z2) {
    float R = (Z2 - Z1) / (Z1 + Z2);
    float T = 2.0f * Z2 / (Z1 + Z2);
    return { R, T };
}

//------------------------------------------------------------------------------
// Mach ���沨�߹���
//------------------------------------------------------------------------------
inline constexpr float blast_mach_stem_height(float R) {
    return 0.24f * R;
}
