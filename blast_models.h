#pragma once
// blast_models.h炸药冲击波物理模型函数及数据结构声明
#include <utility>   // std::pair
#include "config.h"
//------------------------------------------------------------------------------
/// @brief Kingery–Bulmash 查表结果
/// @note 包含峰值超压、正相位持续时间和 Friedlander 形状因子
struct BlastKBResult {
	float Z;     ///< 缩尺距离 (m·kg^(-1/3))
    float Ps;    ///< 峰值超压 (kPa)
    float td;    ///< 正相位持续时间 (ms)
    float beta;  ///< Friedlander 形状因子 (无量纲)
    float I;     // 冲量（Pa·s）
    float ta;    // 到达时间
};

//------------------------------------------------------------------------------
/// @brief 计算缩尺距离 Z = R / W^(1/3)
/// @param R 爆炸源到目标点距离 (m)
/// @param W TNT 当量质量 (kg)
/// @return 归一化距离 Z (m·kg^(-1/3))
inline  float blast_scaled_distance(float R, float W);

//------------------------------------------------------------------------------
/// @brief 根据缩尺距离 Z 查找 K-B 表值
/// @param Z 归一化距离 (m·kg^(-1/3))
/// @return BlastKBResult 包含 Ps、td、beta
BlastKBResult blast_kb_lookup(float Z);

//------------------------------------------------------------------------------
// @brief 计算 Friedlander 理想爆炸波形 P(t)
// @param t    时间 (ms)
// @param Ps   峰值超压 (kPa)
// @param td   正相位持续时间 (ms)
// @param beta 形状因子 (无量纲)
// @return 时刻 t 的超压 (kPa)，若 t<0 或 t>td 返回 0
inline float blast_friedlander(float t, float Ps, float td, float beta);

//------------------------------------------------------------------------------
/// @brief 指数能量衰减 E = E0 * exp(-K * d)
/// @param E0 初始能量
/// @param K  衰减常数 (m^-1)
/// @param d  传播距离 (m)
/// @return 衰减后的能量
inline float blast_exp_decay(float E0, float K, float d);

//------------------------------------------------------------------------------
/// @brief 根据峰值超压估算质点速度 u_p ≈ Ps / (ρ * c0)
/// @param Ps  峰值超压 (Pa)
/// @param rho 空气密度 (kg/m^3)
/// @param c0  声速 (m/s)
/// @return 质点速度 (m/s)
inline float blast_particle_velocity(float Ps, float rho, float c0);

//------------------------------------------------------------------------------
/// @brief 阻抗修正系数 Cr = sqrt(c2 / c1)
/// @param c1 入射介质声速 (m/s)
/// @param c2 出射介质声速 (m/s)
/// @return 阻抗修正系数 (无量纲)
inline float blast_impedance_correction(float c1, float c2);