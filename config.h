//config.h  仿真全局常量
#pragma once
// 冲击波模拟参数
constexpr float E0 = 3.0f;				// 初始能量（任意单位）
constexpr float SPEED_C = 340.0f;		// 传播速度，单位 m/s
constexpr float DECAY_K = 0.01f;        // 能量衰减常数 1/m（如使用）
constexpr int   MAX_BOUNCE = 5;			// 反射上限
constexpr float MIN_ENERGY_FRAC = 0.01f;// 截止能量比例
constexpr int   NUM_RAYS = 100'000;		// 射线总数
constexpr float RAY_EPS = 1e-3f;		// 起始偏移
