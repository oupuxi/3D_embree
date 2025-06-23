//types.h  Vec3f & 一些统计结构
#pragma once
// 简单的 3D 向量结构，用于表示坐标
struct Vec3f {
    float x, y, z;

    Vec3f() : x(0), y(0), z(0) {}
    Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

struct FaceStat {
    int    hits = 0;
    double sumE = 0.0;
};
//static std::vector<FaceStat> g_faceStats;   // 大小 = 三角形数
