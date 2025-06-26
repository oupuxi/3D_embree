// ray.cpp
#include "ray.h"
#include <cfloat>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <random>
#include "config.h"
MyRay::MyRay() {
	energy = 3;
	speed = cfg::C0;
	time = -1.0f;
	depth = 0;
	auto& r = rayhit.ray;
	r.tnear = 0.f;
	r.tfar = FLT_MAX;
	r.mask = 0xFFFFFFFF;
	r.time = 0.f;
	r.flags = 0;

	RTCHit& h = rayhit.hit;
	h.geomID = RTC_INVALID_GEOMETRY_ID;
	h.primID = RTC_INVALID_GEOMETRY_ID;
}

std::vector<MyRay> generateSphereRays(int count)
{
	std::vector<MyRay> rays;
	rays.reserve(count);

	const float phi = M_PI * (3.0f - std::sqrt(5.0f)); // 黄金角度
	const float offset = 2.0f / count;

	// 随机旋转矩阵
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, 2.0f * M_PI);
	float rand_angle = dist(gen);

	float cos_theta = std::cos(rand_angle);
	float sin_theta = std::sin(rand_angle);

	for (int i = 0; i < count * 2; ++i)  // 多生成两倍数量，过滤掉 y<0 的点
	{
		float y = 1.0f - i * offset;              // y 坐标 [-1,1]
		float r = std::sqrt(std::max(0.0f, 1.0f - y * y));  // r = sqrt(1 - y²)
		float theta = phi * i;

		float x = r * std::cos(theta);
		float z = r * std::sin(theta);

		if (y < 0) continue;  // 只保留 y >= 0 的半球

		// 随机绕 y 轴旋转
		float x_rot = cos_theta * x - sin_theta * z;
		float z_rot = sin_theta * x + cos_theta * z;

		MyRay ray;
		ray.rayhit.ray.org_x = 0.0f;
		ray.rayhit.ray.org_y = 0.0f;
		ray.rayhit.ray.org_z = 0.0f;
		ray.rayhit.ray.dir_x = x_rot;
		ray.rayhit.ray.dir_y = y;
		ray.rayhit.ray.dir_z = z_rot;
		ray.energy = 3;       // 初始化能量
		ray.speed = cfg::C0;   // 初始化速度
		ray.time = -1.0f;      // 尚未命中

		rays.push_back(ray);

		if (rays.size() >= size_t(count)) break;  // 收集够了就停止
	}

	return rays;
}

MyRay makeRay(const Vec3f& origin, const Vec3f& dirIn)
{
	// 1) 归一化方向
	Vec3f dir = dirIn;
	float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (len == 0.0f)
		throw std::runtime_error("makeRay(): zero-length direction");
	dir.x /= len;  dir.y /= len;  dir.z /= len;

	// 2) 填充 Embree RTCRayHit
	MyRay ray;                           // 默认构造会把 rayhit 清零
	RTCRay& r = ray.rayhit.ray;
	RTCHit& h = ray.rayhit.hit;

	r.org_x = origin.x;  r.org_y = origin.y;  r.org_z = origin.z;
	r.dir_x = dir.x;     r.dir_y = dir.y;     r.dir_z = dir.z;
	r.tnear = cfg::EPS;                             // 避免自相交
	r.tfar = std::numeric_limits<float>::infinity();
	r.time = 0.0f;
	r.mask = 0xFFFFFFFF;
	r.id = 0;
	r.flags = 0;

	h.geomID = RTC_INVALID_GEOMETRY_ID;
	h.primID = RTC_INVALID_GEOMETRY_ID;
	h.instID[0] = RTC_INVALID_GEOMETRY_ID;

	// 3) 其他仿真用字段，保持初始状态（但不赋能量/速度）
	ray.time = 0.0f;
	ray.depth = 0;

	return ray;
}
