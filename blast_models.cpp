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
	BlastKBResult result;
	if (g_kb_table.empty()) {
		// 数据表为空，返回默认结果
		return result;
	}

	// 查找第一个 Z >= 输入值 的表项
	auto it = std::lower_bound(
		g_kb_table.begin(), g_kb_table.end(), Z,
		[](const KBEntry& entry, float value) { return entry.Z < value; }
	);

	if (it == g_kb_table.begin()) {
		// 输入值小于或等于表格最小 Z，直接使用第一个表项
		const auto& e = *it;
		result.Z = Z;
		result.Ps = e.Ps;
		result.td = e.td;
		result.I = e.I;
	}
	else if (it == g_kb_table.end()) {
		// 输入值大于表格最大 Z，直接使用最后一个表项
		const auto& e = g_kb_table.back();
		result.Z = Z;
		result.Ps = e.Ps;
		result.td = e.td;
		result.I = e.I;
	}
	else {
		// 在两个相邻表项之间线性插值
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
