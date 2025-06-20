//tracer.cpp
#include "tracer.h"
#include <cmath>      // for std::exp
#include "config.h"   // 包含 E0, SPEED_C, K 等仿真常量
#include "result.h"
#include "material.h"

void traceRay(MyRay& ray, RTCScene scene)
{
    // ✅ 使用三参数形式的交点查询 + 上下文（兼容所有 Embree 4.x）
    RTCRayQueryContext context;
    rtcInitRayQueryContext(&context);

    RTCIntersectArguments args;
    rtcInitIntersectArguments(&args);
    args.context = &context;

    rtcIntersect1(scene, &ray.rayhit, &args);

    // ✅ 命中处理
    if (ray.rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    {
        float distance = ray.rayhit.ray.tfar;
        ray.time = distance / ray.speed;

        // ✅ 能量衰减（指数形式）
        ray.energy *= std::exp(-K * distance);
    }
    else
    {
        ray.time = -1.0f;    // 未命中标志
        ray.energy = 0.0f;   // 无剩余能量
    }
}


void traceRayMultiBounce(MyRay& ray,
    RTCScene scene,
    int      maxDepth,
    float    minEnergyFrac)
{
    const float minE = E0 * minEnergyFrac;

    while (ray.depth < maxDepth && ray.energy > minE)
    {

        traceRay(ray, scene);              // 复用一次交点函数

        if (ray.time < 0.f) break;         // 未命中

        // ① 更新峰值能量等 —— 已在 collectHitPoint() 里做
        collectHitPoint(ray);

        // ② 计算反射
        const RTCRay& r = ray.rayhit.ray;
        const RTCHit& h = ray.rayhit.hit;

        Vec3f n = { h.Ng_x, h.Ng_y, h.Ng_z };
        // Embree 的 Ng 未归一化
        float invLen = 1.0f / std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        n.x *= invLen; n.y *= invLen; n.z *= invLen;

        Vec3f d = { r.dir_x, r.dir_y, r.dir_z };
        float dotDN = d.x * n.x + d.y * n.y + d.z * n.z;
        Vec3f refl = {
            d.x - 2 * dotDN * n.x,
            d.y - 2 * dotDN * n.y,
            d.z - 2 * dotDN * n.z };

        // ③ 能量 × 反射系数
        float R = getReflectionCoeff(h.geomID);   // material.h
        ray.energy *= R;

        // ④ 为下一次 bounce 准备 RTCRayHit
        float hitX = r.org_x + r.tfar * r.dir_x;
        float hitY = r.org_y + r.tfar * r.dir_y;
        float hitZ = r.org_z + r.tfar * r.dir_z;

        ray.rayhit.ray.org_x = hitX + 1e-3f * n.x;   // EPS = 1 mm
        ray.rayhit.ray.org_y = hitY + 1e-3f * n.y;
        ray.rayhit.ray.org_z = hitZ + 1e-3f * n.z;

        ray.rayhit.ray.dir_x = refl.x;
        ray.rayhit.ray.dir_y = refl.y;
        ray.rayhit.ray.dir_z = refl.z;

        ray.depth++;
    }
}

void traceBatch(std::span<MyRay> rays, RTCScene scene, int maxDepth, float minEnergyFrac)
{
}

