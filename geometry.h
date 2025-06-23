//geometry.h 生成墙/房子等 Embree 几何并 attach 场景
#pragma once
#include <embree4/rtcore.h>
#include"types.h"
#include <vector>
#include <utility> // for std::pair
#include <unordered_set>
// 提供一个创建场景的函数（返回已构建好的 scene）


// 一栋房子
// center： 包围盒中心；x边长length，z边长width，y高度height
// tileSize：每个瓦片的边长，默认 1 m
// withRoof：是否添加屋顶，默认添加
std::pair<unsigned int, size_t> addHouse(
    RTCScene scene, RTCDevice device,
    const Vec3f& center,            // 房子包围盒中心
    float length, float width,      // X, Z 方向边长（均 10 m）
    float height,
    std::vector<Vec3f>& allVertices,
    std::vector<unsigned int>& allIndices,                   // Y 高度（20 m）
    float tileSize = 1.0f,          // 每格大小，默认 1 m
    bool withRoof = true,
    bool withFloor = false);

// 声明：记录所有探测面 geomID 的集合
extern std::unordered_set<unsigned int> g_sensorGeomIDs;
