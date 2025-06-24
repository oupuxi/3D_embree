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
inline BlastKBResult blast_kb_lookup(float Z) {
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
// 质点速度估算
//------------------------------------------------------------------------------
inline float blast_particle_velocity(float Ps, float rho, float c0) {
	return Ps / (rho * c0);
}

//------------------------------------------------------------------------------
// 声阻抗 R/T 系数计算
//------------------------------------------------------------------------------
inline std::pair<float, float> blast_impedance_rt(float Z1, float Z2) {
	float R = (Z2 - Z1) / (Z1 + Z2);
	float T = 2.0f * Z2 / (Z1 + Z2);
	return { R, T };
}

//------------------------------------------------------------------------------
// Mach 干涉波高估算
//------------------------------------------------------------------------------
inline constexpr float blast_mach_stem_height(float R) {
	return 0.24f * R;
}
