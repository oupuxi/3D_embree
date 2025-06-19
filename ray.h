// ray.h 
#pragma once
#include <embree4/rtcore.h>
#include"config.h"
#include <vector>
struct MyRay {
    RTCRayHit rayhit;
    float energy;        // 剩余能量
    float speed;         // 目前传播速度
    float time;          // 到当前点累计时间
    int   depth;         // 已经弹跳次数
    MyRay();
};

// 生成均匀分布在 y ≥ 0 半球表面的射线，带随机旋转
std::vector<MyRay> generateSphereRays(int count);
