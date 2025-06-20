//tracer.h   ① 单次求交② 多次反射循环③ 能量衰减& collectHitPoint
#pragma once
#include <embree4/rtcore.h>
#include "ray.h"
#include <span>
// 单条射线追踪
void traceRay(MyRay& ray, RTCScene scene);

// 递归反射 / 能量衰减
void traceRayMultiBounce(MyRay& ray,
                         RTCScene scene,
                         int      maxDepth = 5,
                         float    minEnergyFrac = 0.01f); // < E0*1% 时终止

/** ③ **批量接口** —— 先串行实现，未来可并行 / SIMD
    调用例：
        traceBatch(rays, scene, 5, 0.01f);
*/
void traceBatch(std::span<MyRay> rays,
    RTCScene         scene,
    int              maxDepth = 5,
    float            minEnergyFrac = 0.01f);