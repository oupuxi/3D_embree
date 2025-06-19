//geometry.h 封装几何体构建过程，让你在主函数中一句话就能构建整个场景 
#pragma once
#include <embree4/rtcore.h>
#include"types.h"
#include <vector>
#include <utility> // for std::pair
// 提供一个创建场景的函数（返回已构建好的 scene）
RTCScene createSimpleScene(RTCDevice device);

unsigned int addSquarePlane(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float size);  // size 是边长


unsigned int addWallPlaneXY(RTCScene scene, RTCDevice device,
    const Vec3f& center, // X, Y 控制位置，Z 是墙所在平面位置
    float width, float height,
    float tileSize = 1.0f);
// 在 Embree 场景中添加一个 XY 平面上的“瓦片化”墙体（三角网格）
// center: 墙面中心点位置
// width, height: 墙的宽高
// tileSize: 每个小瓦片的边长（决定网格细分程度）
// 返回值是该几何体在场景中的 geomID
unsigned int addWallPlaneXYSafe(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float width, float height,
    float tileSize);

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
