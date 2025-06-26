// ray.h MyRay 定义 + 射线发射（半球）
#pragma once
#include <embree4/rtcore.h>
#include"config.h"
#include <vector>
#include "types.h"
struct MyRay {
	RTCRayHit rayhit;
	float energy;        // 当前剩余能量
	float speed;         // 目前传播速度
	float time;          // 已传播时间 (s)
	int   depth;         // 反射次数 (0 表示直射)
	MyRay();
};

// 生成均匀分布在 y ≥ 0 半球表面的射线，带随机旋转
std::vector<MyRay> generateSphereRays(int count);

/// 生成一条自定义方向的射线（仅填充 Embree 必要字段）
/// \param origin   射线起点（世界坐标）
/// \param dirIn    射线方向，传入前可不归一化
/// \return         填充好 rayhit、time、depth 的 MyRay
MyRay makeRay(const Vec3f& origin,
	const Vec3f& dirIn);