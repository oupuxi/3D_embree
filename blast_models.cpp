// �ļ�: blast_models.cpp
// ����: ըҩ���������ģ�ͺ���ʵ��
//------------------------------------------------------------------------------
#include "blast_models.h"
#include <cmath>
#include <array>
#include <algorithm>



//------------------------------------------------------------------------------
// Hopkinson ���߾���
//------------------------------------------------------------------------------
inline  float blast_scaled_distance(float R, float W) {
    return R / std::cbrt(W);
}

//------------------------------------------------------------------------------
// K-B �����Բ�ֵ��ѯ
//------------------------------------------------------------------------------
inline BlastKBResult blast_kb_lookup(float Z){
    const auto& tbl = g_kb_table;
    // �߽紦��
    if (Z <= tbl.front().Z) {
        auto& e = tbl.front();
        return { Z, e.Ps, e.td, e.beta, e.I, e.Us, e.ta };
    }
    if (Z >= tbl.back().Z) {
        auto& e = tbl.back();
        return { Z, e.Ps, e.td, e.beta, e.I, e.Us, e.ta };
    }

    // �ҵ���һ�� Z_i >= Z
    auto it = std::upper_bound(tbl.begin(), tbl.end(), Z,
        [](float v, const KBEntry& e) { return v < e.Z; });
    size_t i1 = it - tbl.begin();
    size_t i0 = i1 - 1;
    auto& a = tbl[i0];
    auto& b = tbl[i1];

    // ��ֵ
    float t = (Z - a.Z) / (b.Z - a.Z);
    auto lerp = [t](float A, float B) { return A + (B - A) * t; };

    return {
        Z,
        lerp(a.Ps,   b.Ps),   // ��ѹ
        lerp(a.td,   b.td),   // ����λ����ʱ��
        lerp(a.beta, b.beta), // ��״����
        lerp(a.I,    b.I),    // ����
        lerp(a.Us,   b.Us),   // ��ǰ�ٶ�
        lerp(a.ta,   b.ta)    // ��ֵ����ʱ��
    };
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