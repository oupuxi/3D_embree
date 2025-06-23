// geometry.cpp
#include "geometry.h"
#include <array>
#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>
#include "result.h"
#include <sstream>



std::pair<unsigned int, size_t> addHouse(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float length, float width, float height,
    std::vector<Vec3f>& allVertices,
    std::vector<unsigned int>& allIndices,
    float tileSize,
    bool withRoof, bool withFloor)
{
    assert(scene && device && tileSize > 0.0f);

    const float hx = length * 0.5f;
    const float hy = height * 0.5f;
    const float hz = width * 0.5f;

    struct FaceInfo {
        // 面中心、两个边向量 (u,v)（用于铺网格）、外法线
        Vec3f origin;
        Vec3f du;            // 横向方向（u 轴）
        Vec3f dv;            // 纵向方向（v 轴）
        Vec3f normal;        // 朝外
        float lenU;          // 对应边长
        float lenV;
    };
    std::vector<FaceInfo> faces;

    // 前 (+Z) u: +X, v: +Y
    faces.push_back({ {center.x, center.y, center.z + hz},
                      {  tileSize, 0, 0}, {0, tileSize, 0}, { 0, 0, 1},
                      length, height });
    // 后 (-Z)
    faces.push_back({ {center.x, center.y, center.z - hz},
                      {-tileSize, 0, 0}, {0, tileSize, 0}, { 0, 0,-1},
                      length, height });
    // 右 (+X)
    faces.push_back({ {center.x + hx, center.y, center.z},
                      {0, tileSize, 0}, {0, 0,-tileSize}, { 1, 0, 0},
                      height, width });
    // 左 (-X)
    faces.push_back({ {center.x - hx, center.y, center.z},
                      {0, tileSize, 0}, {0, 0, tileSize}, {-1, 0, 0},
                      height, width });
    if (withRoof) {
        // 顶 (+Y) u: +X, v: +Z
        faces.push_back({ {center.x, center.y + hy, center.z},
                          { tileSize, 0, 0}, {0, 0, tileSize}, {0, 1, 0},
                          length, width });
    }
    if (withFloor) {
        // 底 (-Y)
        faces.push_back({ {center.x, center.y - hy, center.z},
                          { tileSize, 0, 0}, {0, 0,-tileSize}, {0,-1, 0},
                          length, width });
    }

    std::vector<Vec3f> vertices;
    std::vector<unsigned int> indices;

    for (const auto& f : faces)
    {
        const int nu = std::ceil(f.lenU / tileSize);
        const int nv = std::ceil(f.lenV / tileSize);

        // 记录此面开始的顶点偏移量
        const unsigned int vBase = static_cast<unsigned int>(vertices.size());

        // 生成格点
        for (int iv = 0; iv <= nv; ++iv)
            for (int iu = 0; iu <= nu; ++iu)
            {
                Vec3f p{
                    f.origin.x + (iu - nu * 0.5f) * f.du.x + (iv - nv * 0.5f) * f.dv.x,
                    f.origin.y + (iu - nu * 0.5f) * f.du.y + (iv - nv * 0.5f) * f.dv.y,
                    f.origin.z + (iu - nu * 0.5f) * f.du.z + (iv - nv * 0.5f) * f.dv.z
                };
                vertices.push_back(p);
            }

        // 三角形索引（保持 counter-clockwise = 法线朝 f.normal）
        for (int iv = 0; iv < nv; ++iv)
            for (int iu = 0; iu < nu; ++iu)
            {
                unsigned int i0 = vBase + iu + iv * (nu + 1);
                unsigned int i1 = vBase + (iu + 1) + iv * (nu + 1);
                unsigned int i2 = vBase + iu + (iv + 1) * (nu + 1);
                unsigned int i3 = vBase + (iu + 1) + (iv + 1) * (nu + 1);

                // 两个三角形：i0-i1-i2, i1-i3-i2   (逆时针看向 f.normal)
                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i2);

                indices.push_back(i1);
                indices.push_back(i3);
                indices.push_back(i2);
            }
    }

    // --- 创建 Embree 几何体 ---
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    void* vbuf = rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        sizeof(Vec3f), vertices.size());
    std::memcpy(vbuf, vertices.data(), sizeof(Vec3f) * vertices.size());

    void* ibuf = rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        3 * sizeof(unsigned int), indices.size() / 3);
    std::memcpy(ibuf, indices.data(), sizeof(unsigned int) * indices.size());

    rtcCommitGeometry(geom);
    unsigned int geomID = rtcAttachGeometry(scene, geom);
    
    rtcReleaseGeometry(geom);
    //// ------------------ 3. 立刻导出这面墙的 CSV -------------
    //// ② 调用 result 模块里的导出函数
    //exportWorldCSV(vertices, indices, "world_init1.csv");

    //std::cout << "[addHouse] 房子生成完成，geomID = " << geomID
    //    << "，总顶点数 = " << vertices.size()
    //    << "，总三角形数 = " << indices.size() / 3 << "\n";

    // 合并全局顶点和索引
    unsigned int indexOffset = static_cast<unsigned int>(allVertices.size());
    allVertices.insert(allVertices.end(), vertices.begin(), vertices.end());
    for (auto idx : indices) {
        allIndices.push_back(idx + indexOffset);
    }

    size_t nTriangles = indices.size() / 3; // ⚡ 每栋房子的三角形数量！

    return { geomID, nTriangles };  // ⚡ 返回几何体ID 和 三角形数量
}

// 定义：真正开辟这块 set 的内存空间
std::unordered_set<unsigned int> g_sensorGeomIDs;

unsigned int addSensorMesh(
    RTCScene scene, RTCDevice device,
    const Vec3f& center, const Vec3f& normal,
    float width, float height, float tileSize)
{
    assert(scene && device);
    assert(width > 0 && height > 0);
    if (tileSize <= 0.0f) tileSize = 1.0f;

    // 构造局部坐标系
    Vec3f n = normalize(normal);
    Vec3f up = (fabs(n.y) < 0.99f ? Vec3f{ 0,1,0 } : Vec3f{ 1,0,0 });
    Vec3f u = normalize(cross(up, n));
    Vec3f v = cross(n, u);

    int tilesU = int(std::ceil(width / tileSize));
    int tilesV = int(std::ceil(height / tileSize));
    float du = width / tilesU;
    float dv = height / tilesV;

    // 创建局部网格数据
    std::vector<Vec3f> verts;
    verts.reserve((tilesU + 1) * (tilesV + 1));
    for (int iy = 0; iy <= tilesV; iy++) {
        for (int ix = 0; ix <= tilesU; ix++) {
            Vec3f p = center + (ix * du - width * 0.5f) * u + (iy * dv - height * 0.5f) * v;
            verts.push_back(p);
        }
    }

    std::vector<unsigned int> inds;
    inds.reserve(tilesU * tilesV * 6);
    for (int iy = 0; iy < tilesV; iy++) {
        for (int ix = 0; ix < tilesU; ix++) {
            unsigned i0 = iy * (tilesU + 1) + ix;
            unsigned i1 = i0 + 1;
            unsigned i2 = (iy + 1) * (tilesU + 1) + ix;
            unsigned i3 = i2 + 1;
            // 保证法线方向一致
            inds.insert(inds.end(), { i0, i1, i2, i1, i3, i2 });
        }
    }

    // 创建 Embree 几何
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    // 使用 Embree 拷贝机制，避免内存生命周期问题
    Vec3f* embreeVerts = (Vec3f*)rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0,
        RTC_FORMAT_FLOAT3,
        verts.size(), sizeof(Vec3f));
    memcpy(embreeVerts, verts.data(), verts.size() * sizeof(Vec3f));

    unsigned* embreeInds = (unsigned*)rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0,
        RTC_FORMAT_UINT3,
        inds.size() / 3, 3 * sizeof(unsigned));
    memcpy(embreeInds, inds.data(), inds.size() * sizeof(unsigned));

    rtcCommitGeometry(geom);
    unsigned geomID = rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);

    // 标记为探测面
    g_sensorGeomIDs.insert(geomID);
    return geomID;
}