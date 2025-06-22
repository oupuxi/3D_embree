// result.cpp 或放 main.cpp 下方
#include "result.h"
#include <iostream>
#include "ray.h"
#include <embree4/rtcore.h>
#include <fstream>
#include <vector>
#include <iomanip>


// ✅ 唯一定义！
std::vector<float> g_peakEnergy;
std::vector<size_t> houseOffsets;   // 保存每个房子(geomID)对应的起始三角形偏移量

void processAndPrintRay(const MyRay& ray, int index)
{
    const RTCRay& r = ray.rayhit.ray;
    const RTCHit& h = ray.rayhit.hit;

    if (ray.time >= 0.f) {
        float t = r.tfar;
        float px = r.org_x + t * r.dir_x;
        float py = r.org_y + t * r.dir_y;
        float pz = r.org_z + t * r.dir_z;

        float Ng_x = h.Ng_x, Ng_y = h.Ng_y, Ng_z = h.Ng_z;
        bool backface = (r.dir_x * Ng_x + r.dir_y * Ng_y + r.dir_z * Ng_z) > 0.f;

        if (index >= 0) std::cout << "Ray[" << index << "] 命中信息：\n";
        else std::cout << "命中信息如下：\n";

        std::cout << "  几何体 ID       : " << h.geomID << "\n";
        std::cout << "  原始元 ID       : " << h.primID << "\n";
        std::cout << "  命中距离        : " << t << " m\n";
        std::cout << "  命中位置        : (" << px << ", " << py << ", " << pz << ")\n";
        std::cout << "  barycentric 坐标: (u = " << h.u << ", v = " << h.v << ")\n";
        std::cout << "  命中三角形的法线方向 Ng     : (" << Ng_x << ", " << Ng_y << ", " << Ng_z << ")\n";
        std::cout << "  是否背面命中    : " << (backface ? "是" : "否") << "\n";
        std::cout << "  传播时间        : " << ray.time << " s\n";
        std::cout << "  剩余能量        : " << ray.energy << "\n";
    }
    else {
        /*if (index >= 0) std::cout << "Ray[" << index << "] ❌ 未命中。\n";
        else std::cout << "❌ 未命中。\n";*/
    }
}

namespace {
    struct PointRec {
        float x, y, z;
        float energy, time;
    };
    std::vector<PointRec> g_points;   // 全局缓冲
}

// 收集
//extern std::vector<size_t> houseOffsets;  // 在 result.h 里加 extern

void collectHitPoint(const MyRay& ray)
{
    if (ray.time < 0.f) return;   // 未命中跳过
    const auto& r = ray.rayhit.ray;
    const auto& h = ray.rayhit.hit;

    g_points.push_back({
        r.org_x + r.tfar * r.dir_x,
        r.org_y + r.tfar * r.dir_y,
        r.org_z + r.tfar * r.dir_z,
        ray.energy,
        ray.time
        });

    // 【改正】用偏移量 + primID
    size_t globalPrimID = houseOffsets[h.geomID] + h.primID;
    if (globalPrimID < g_peakEnergy.size())
    {
        g_peakEnergy[globalPrimID] = std::max(g_peakEnergy[globalPrimID], ray.energy);
    }
}


// 写 PLY（ASCII，便于调试）
void writePLY(const std::string& filename, bool colorByEnergy)
{
    if (g_points.empty()) { std::cerr << "[PLY] 空点集，未输出\n"; return; }

    // 能量归一化到 [0,1] 映射为颜色
    float eMin = g_points.front().energy, eMax = eMin;
    if (colorByEnergy) {
        for (auto& p : g_points) { eMin = std::min(eMin, p.energy); eMax = std::max(eMax, p.energy); }
        if (eMax == eMin) eMax += 1e-6f;
    }

    std::ofstream ofs(filename);
    ofs << "ply\nformat ascii 1.0\n"
        << "element vertex " << g_points.size() << "\n"
        << "property float x\nproperty float y\nproperty float z\n";
    if (colorByEnergy)
        ofs << "property uchar red\nproperty uchar green\nproperty uchar blue\n";
    ofs << "end_header\n";

    for (auto& p : g_points) {
        ofs << std::fixed << std::setprecision(4)
            << p.x << ' ' << p.y << ' ' << p.z;
        if (colorByEnergy) {
            float t = (p.energy - eMin) / (eMax - eMin);           // 0..1
            uint8_t r = uint8_t(255 * (1 - t));
            uint8_t g = uint8_t(255 * t);
            uint8_t b = 0;
            ofs << ' ' << int(r) << ' ' << int(g) << ' ' << int(b);
        }
        ofs << '\n';
    }
    ofs.close();
    std::cout << "[PLY] 输出 " << filename << " ，共 " << g_points.size() << " 点\n";
}

void exportWorldCSV(const std::vector<Vec3f>& verts,
    const std::vector<unsigned>& idx,
    const std::string& filename)
{
    if (idx.size() % 3 != 0)
    {
        std::cerr << "[CSV] index 数量不是 3 的倍数！\n";
        return;
    }

    std::ofstream ofs(filename);
    if (!ofs)
    {
        std::cerr << "[CSV] 无法写入 " << filename << "\n";
        return;
    }

    ofs << std::fixed << std::setprecision(6);
    ofs << "tri,x0,y0,z0,x1,y1,z1,x2,y2,z2,energy\n";

    const size_t nTri = idx.size() / 3;

    // 🔥 打印调试信息
    std::cout << "[Debug] 总三角形数: " << nTri << "\n";
    std::cout << "[Debug] g_peakEnergy 大小: " << g_peakEnergy.size() << "\n";
    std::cout << "[Debug] houseOffsets: ";
    for (auto h : houseOffsets)
        std::cout << h << " ";
    std::cout << "\n";

    if (g_peakEnergy.size() < nTri)
    {
        std::cerr << "[警告] g_peakEnergy.size() 小于 三角形总数！可能越界！\n";
    }

    for (size_t t = 0; t < nTri; ++t)
    {
        const Vec3f& v0 = verts[idx[3 * t]];
        const Vec3f& v1 = verts[idx[3 * t + 1]];
        const Vec3f& v2 = verts[idx[3 * t + 2]];

        ofs << t << ','
            << v0.x << ',' << v0.y << ',' << v0.z << ','
            << v1.x << ',' << v1.y << ',' << v1.z << ','
            << v2.x << ',' << v2.y << ',' << v2.z << ',';

        bool found = false;

        for (size_t gid = 0; gid < houseOffsets.size(); ++gid)
        {
            size_t start = houseOffsets[gid];
            size_t end = (gid + 1 < houseOffsets.size()) ? houseOffsets[gid + 1] : g_peakEnergy.size();

            if (t >= start && t < end)
            {
                size_t primID = t - start;
                size_t globalPrimID = houseOffsets[gid] + primID;

                if (globalPrimID < g_peakEnergy.size())
                {
                    ofs << g_peakEnergy[globalPrimID];
                }
                else
                {
                    std::cerr << "[警告] globalPrimID 越界！t=" << t
                        << ", gid=" << gid
                        << ", globalPrimID=" << globalPrimID
                        << ", g_peakEnergy.size()=" << g_peakEnergy.size() << "\n";
                    ofs << 0.0f;
                }
                found = true;
                break;
            }
        }

        if (!found)
        {
            std::cerr << "[警告] 三角形 " << t << " 没找到对应房子！写 0\n";
            ofs << 0.0f;
        }

        ofs << '\n';
    }

    std::cout << "[CSV] 导出完成：" << filename << "，共 " << nTri << " 个三角形！\n";
}

