#pragma once
// blast_models.h —— 炸药冲击波物理模型函数及数据结构声明

#include <array>      // std::array
#include <algorithm>  // std::upper_bound（用于插值查表）
#include <cmath>      // std::pow, std::log10 等数学函数
#include <utility>    // 可选：如用 std::pair 等
#include "config.h"

//------------------------------------------------------------------------------
/// @brief Kingery–Bulmash 查表结果
/// @note 包含峰值超压、正相位持续时间、冲量、形状因子等
struct BlastKBResult {
	float Z;      ///< 缩尺距离 (m·kg^(-1/3))
	float Ps;     ///< 峰值超压 (kPa)
	float td;     ///< 正相位持续时间 (ms)
	float beta;   ///< Friedlander 形状因子 (无量纲)
	float I;      ///< 冲量 (kPa·ms)
	float Us;     ///< 冲击波前速度 (m/s)
	float ta;     ///< 峰值到达时间 (ms)
};

//------------------------------------------------------------------------------
/// @brief Kingery–Bulmash 数据表项结构
struct KBEntry {
	float Z;      ///< 缩尺距离 (m·kg^(-1/3))
	float Ps;     ///< 峰值超压 (kPa)
	float td;     ///< 正相位持续时间 (ms)
	float beta;   ///< Friedlander 形状因子 (无量纲)
	float I;      ///< 冲量 (kPa·ms)
	float Us;     ///< 冲击波前速度 (m/s)
	float ta;     ///< 峰值到达时间 (ms)
};

//------------------------------------------------------------------------------
/// @brief 示例 K–B 数据表（仅含 3 条样本记录）
/// @note 替换为完整 K–B 表后可用于插值查询
static constexpr std::array<KBEntry, 3> g_kb_table = { {
		//    Z       Ps       td     beta      I       Us      ta
		{ 0.2f, 1586.0f,  0.30f, 1.90f,  900.0f, 1030.0f,  0.30f },
		{ 1.0f,  185.0f,  1.70f, 1.85f,  600.0f,  500.0f,  4.50f },
		{ 5.0f,    9.8f, 13.00f, 1.80f,   80.0f,  200.0f, 17.00f }
} };

//------------------------------------------------------------------------------
/// @brief 计算缩尺距离 Z = R / W^(1/3)
/// @param R 距离 (m)
/// @param W TNT 当量质量 (kg)
/// @return 缩尺距离 Z (m·kg^(-1/3))
inline float blast_scaled_distance(float R, float W);

//------------------------------------------------------------------------------
/// @brief 根据缩尺距离查找 K–B 表值
/// @param Z 缩尺距离 (m·kg^(-1/3))
/// @return 插值得到的 BlastKBResult
BlastKBResult blast_kb_lookup(float Z);

//------------------------------------------------------------------------------
/// @brief Friedlander 爆炸波形 P(t) 模型
/// @param t    时间 (ms)
/// @param Ps   峰值超压 (kPa)
/// @param td   正相位持续时间 (ms)
/// @param beta Friedlander 形状因子
/// @return 指定时刻 t 的超压值 (kPa)，t 超出正相位返回 0
inline float blast_friedlander(float t, float Ps, float td, float beta);

//------------------------------------------------------------------------------
/// @brief 指数能量衰减模型 E = E0 * exp(-K * d)
/// @param E0 初始能量
/// @param K  衰减常数 (1/m)
/// @param d  距离 (m)
/// @return 衰减后的能量
inline float blast_exp_decay(float E0, float K, float d);

//------------------------------------------------------------------------------
/// @brief 估算质点速度 u_p ≈ Ps / (ρ * c0)
/// @param Ps  峰值超压 (Pa)
/// @param rho 空气密度 (kg/m^3)
/// @param c0  声速 (m/s)
/// @return 质点速度 (m/s)
inline float blast_particle_velocity(float Ps, float rho, float c0);

//------------------------------------------------------------------------------
/// @brief 反射修正系数 Cr = sqrt(c2 / c1)
/// @param c1 入射介质声速 (m/s)
/// @param c2 出射介质声速 (m/s)
/// @return 修正系数 Cr (无量纲)
inline float blast_impedance_correction(float c1, float c2);
