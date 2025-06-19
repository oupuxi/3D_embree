//tracer.h   ① 单次求交② 多次反射循环③ 能量衰减& collectHitPoint
#pragma once
#include <embree4/rtcore.h>
#include "ray.h"

// 单条射线追踪
void traceRay(MyRay& ray, RTCScene scene);

// 可选扩展：多条射线批量处理
//void traceRays(std::vector<MyRay>& rays, RTCScene scene);

void traceRayMultiBounce(MyRay& ray,
    RTCScene scene,
    int      maxDepth = 5,
    float    minEnergyFrac = 0.01f); // < E0*1% 时终止