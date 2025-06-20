// integrator.h  功能与tracer重叠
#pragma once
#include <embree4/rtcore.h>
#include "ray.h"      // MyRay 定义
// 视需要再 forward 声明或 include material.h / result.h

namespace sim   // ← 以后你想换别的名字也行
{

    /**
     * 多次弹跳追踪一条射线
     *
     * @param ray            I/O: 射线结构体（会被就地更新）
     * @param scene          Embree 场景
     * @param maxDepth       最大弹跳次数（含首射）
     * @param minEnergyFrac  能量低于 E0*frac 即终止
     */
    void traceRayMultiBounce(
        MyRay& ray,
        RTCScene scene,
        int      maxDepth = 5,
        float    minEnergyFrac = 0.01f);

} // namespace sim
