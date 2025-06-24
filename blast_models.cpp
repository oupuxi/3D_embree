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
inline BlastKBResult blast_kb_lookup(float Z) {
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
// �ʵ��ٶȹ���
//------------------------------------------------------------------------------
inline float blast_particle_velocity(float Ps, float rho, float c0) {
	return Ps / (rho * c0);
}

//------------------------------------------------------------------------------
// ���迹 R/T ϵ������
//------------------------------------------------------------------------------
inline std::pair<float, float> blast_impedance_rt(float Z1, float Z2) {
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
