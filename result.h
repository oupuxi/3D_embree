// result.h  命中信息打印 / PLY & CSV 导出 / 能量累积
#pragma once
#include "ray.h"
#include "types.h"
#include<vector>
#include <string>
/** 打印单条射线的命中信息（调试用）*/
void processAndPrintRay(const MyRay& ray, int index = -1);

/** 收集一次有效命中，用于后续导出 PLY / CSV */
void collectHitPoint(const MyRay& ray);         // 累积单个命中点

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
// ---------------- 全局累积数据 ----------------
extern std::vector<float>  g_peakEnergy;   ///< 每个三角形的峰值能量
extern std::vector<size_t> houseOffsets;   ///< houseID → globalPrimID 偏移量
