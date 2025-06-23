//geometry.h 生成墙/房子等 Embree 几何并 attach 场景
#pragma once
#include <embree4/rtcore.h>
#include"types.h"
#include <vector>
#include <utility> // for std::pair
#include <unordered_set>
#include "geometry.h"
#include "vec_math.h"

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

/**
  在任意空间方向上生成一块矩形“探测面”（不会反射，只记录事件）
    center   : 中心点坐标
    normal   : 朝外法线（建议单位向量；内部会自动归一）
    width    : 沿 u 轴长度 (m)
    height   : 沿 v 轴长度 (m)
    tileSize : 网格尺寸 (m)；默认 1.0 → 每 1 m² 拆成 2 三角形
  返回：新建几何在 scene 中的 geomID
 */
unsigned int addSensorMesh(
    RTCScene scene, RTCDevice device,
    const Vec3f& center, const Vec3f& normal,
    float width, float height, float tileSize = 1.0f);