// result.h  命中信息打印 / PLY & CSV 导出 / 能量累积
#pragma once
#include "ray.h"
#include "types.h"
#include <string>
void processAndPrintRay(const MyRay& ray, int index = -1);

// 输出点云
void collectHitPoint(const MyRay& ray);         // 累积单个命中点
//void writePLY(const std::string& filename,      // 将全部命中写成 PLY
//    bool colorByEnergy = true);
// 每个三角形统计一次峰值
extern std::vector<float> g_peakPressure;

/**
 * 将当前世界几何导出为 CSV：
 *  列顺序：triID,x0,y0,z0,x1,y1,z1,x2,y2,z2
 *
 * @param verts   顶点数组（构建场景时就已有）
 * @param idx     三角形索引数组（3 * nTri）
 * @param path    输出文件名，例如 "world_init.csv"
 */
void exportWorldCSV(const std::vector<Vec3f>& verts,
    const std::vector<unsigned>& idx,
    const std::string& filename = "world_init.csv");
// 全局能量表 只声明
extern std::vector<float> g_peakEnergy;
extern std::vector<size_t> houseOffsets;  
