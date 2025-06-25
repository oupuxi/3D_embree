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
	BlastKBResult result;
	if (g_kb_table.empty()) {
		// ���ݱ�Ϊ�գ�����Ĭ�Ͻ��
		return result;
	}

	// ���ҵ�һ�� Z >= ����ֵ �ı���
	auto it = std::lower_bound(
		g_kb_table.begin(), g_kb_table.end(), Z,
		[](const KBEntry& entry, float value) { return entry.Z < value; }
	);

	if (it == g_kb_table.begin()) {
		// ����ֵС�ڻ���ڱ����С Z��ֱ��ʹ�õ�һ������
		const auto& e = *it;
		result.Z = Z;
		result.Ps = e.Ps;
		result.td = e.td;
		result.I = e.I;
	}
	else if (it == g_kb_table.end()) {
		// ����ֵ���ڱ����� Z��ֱ��ʹ�����һ������
		const auto& e = g_kb_table.back();
		result.Z = Z;
		result.Ps = e.Ps;
		result.td = e.td;
		result.I = e.I;
	}
	else {
		// ���������ڱ���֮�����Բ�ֵ
		const auto& hi = *it;
		const auto& lo = *(it - 1);
		float t = (Z - lo.Z) / (hi.Z - lo.Z);
		result.Z = Z;
		result.Ps = lo.Ps + t * (hi.Ps - lo.Ps);
		result.td = lo.td + t * (hi.td - lo.td);
		result.I = lo.I + t * (hi.I - lo.I);
	}

	return result;
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
