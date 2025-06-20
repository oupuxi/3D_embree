// ray.cpp
#include "ray.h"
#include <cfloat>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <random>

MyRay::MyRay() {
    energy = E0;
    speed = SPEED_C;
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
        ray.energy = E0;       // 初始化能量
        ray.speed = SPEED_C;   // 初始化速度
        ray.time = -1.0f;      // 尚未命中

        rays.push_back(ray);

        if (rays.size() >= size_t(count)) break;  // 收集够了就停止
    }

    return rays;
}
