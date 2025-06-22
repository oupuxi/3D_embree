// config.h — 全局常量 & 默认参数
#pragma once
#include <cmath>

namespace cfg {

	// ────────────────────────────────────────────────
	// 1. 标准大气（海平面，15 °C）
	// ────────────────────────────────────────────────
	constexpr float AIR_R			= 287.06f;   // J / (kg·K)   比气体常数
	constexpr float AIR_T0			= 288.15f;   // K            温度
	constexpr float AIR_GAMMA		= 1.4f;      // 比热比 γ
	constexpr float AIR_RHO0		= 1.225f;    // kg / m³      空气密度 ρ₀
	constexpr float AIR_P0			= 101325.f;  // Pa           大气压强 p₀
	constexpr float C0				= 340.29f;	 // 340.29 m/s   声速

	// ────────────────────────────────────────────────
	// 2. 爆源默认参数
	// ────────────────────────────────────────────────
	constexpr float TNT_MASS_KG = 2000.f;   // 当量质量 W_TNT

	// ────────────────────────────────────────────────
	// 3. 仿真控制
	// ────────────────────────────────────────────────
	constexpr size_t RAY_COUNT = 1'000'000; // 半球射线数
	constexpr int    MAX_BOUNCES = 8;         // 最大反射次数
	constexpr float  EPS = 1e-3f;     // 交点偏移

	// ────────────────────────────────────────────────
	// 4. Friedlander 波形缺省参数
	// ────────────────────────────────────────────────
	constexpr float FRIED_ALPHA = 0.35f;           // 衰减系数 α

	// ────────────────────────────────────────────────
	// 5. 输出 / 调试
	// ────────────────────────────────────────────────
	constexpr bool  OUT_PLY = true;  // 导出 hits.ply
	constexpr bool  OUT_SENSOR_CSV = true;  // 导出 sensor_*.csv
	constexpr float SENSOR_DT_US = 10.f;  // 采样间隔 (µs)
	constexpr bool  LOG_VERBOSE = false; // 详细日志

} // namespace cfg

