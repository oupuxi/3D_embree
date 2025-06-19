// geometry.cpp
#include "geometry.h"
#include <array>
#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>
#include"result.h"
#include <sstream>
RTCScene createSimpleScene(RTCDevice device)
{
    assert(device);  // 保底断言

    RTCScene scene = rtcNewScene(device);

    // 定义三角形顶点和索引
    std::array<float, 9> vertices = {
        -.1f, 0.5f, 5.f,
        1.f,  0.f,  5.f,
        0.f,  2.f, 5.f
       
    };
    std::array<unsigned int, 3> indices = { 0, 1, 2 };

    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    rtcSetSharedGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        vertices.data(), 0, 3 * sizeof(float), vertices.size() / 3);

    rtcSetSharedGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        indices.data(), 0, 3 * sizeof(unsigned int), indices.size() / 3);

    rtcCommitGeometry(geom);
    rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);  // 挂到 scene 后释放
    rtcCommitScene(scene);     // 构建 BVH

    return scene;
}

unsigned int addWallPlaneXY(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float width, float height,
    float tileSize)
{
    assert(scene && device);
    assert(tileSize > 0.0f);

    int tilesX = std::ceil(width / tileSize);
    int tilesY = std::ceil(height / tileSize);

    float dx = width / tilesX;
    float dy = height / tilesY;

    static std::vector<Vec3f> vertices;
    static std::vector<unsigned int> indices;

    float x0 = center.x - width * 0.5f;
    float y0 = center.y - height * 0.5f;
    float z = center.z; // ✅ 固定 Z 平面，无厚度

    // 构造顶点
    for (int iy = 0; iy <= tilesY; ++iy) {
        for (int ix = 0; ix <= tilesX; ++ix) {
            float x = x0 + ix * dx;
            float y = y0 + iy * dy;
            vertices.emplace_back(x, y, z);



        }
    }

    // ✅ 关键修改：反转三角形顶点顺序，让法线朝 -Z（爆炸源方向）
    for (int iy = 0; iy < tilesY; ++iy) {
        for (int ix = 0; ix < tilesX; ++ix) {
            int i0 = iy * (tilesX + 1) + ix;
            int i1 = i0 + 1;
            int i2 = i0 + (tilesX + 1);
            int i3 = i2 + 1;

            // 输出第一个三角形三个点
            if (iy == 0 && ix == 0) {
                const Vec3f& v0 = vertices[i0];
                const Vec3f& v1 = vertices[i1];
                const Vec3f& v2 = vertices[i2];

                Vec3f edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
                Vec3f edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
                Vec3f normal = {
                    edge1.y * edge2.z - edge1.z * edge2.y,
                    edge1.z * edge2.x - edge1.x * edge2.z,
                    edge1.x * edge2.y - edge1.y * edge2.x
                };

                std::cout << "[法线测试1] Triangle 0 法线: (" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
            }


            // 原顺序（朝 +Z）：i0, i2, i1 和 i1, i2, i3
            // ✅ 改为朝 -Z：
            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i2);


            /*if (iy == 0 && ix == 0) {
                std::cout << "三角形顶点样例: "
                    << "v0 = (" << vertices[i0].x << ", " << vertices[i0].y << ", " << vertices[i0].z << "), "
                    << "v1 = (" << vertices[i1].x << ", " << vertices[i1].y << ", " << vertices[i1].z << "), "
                    << "v2 = (" << vertices[i2].x << ", " << vertices[i2].y << ", " << vertices[i2].z << ")\n";
            }*/
            if (iy == 0 && ix == 0) {
                const Vec3f& v0 = vertices[i0];
                const Vec3f& v1 = vertices[i1];
                const Vec3f& v2 = vertices[i2];

                Vec3f edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
                Vec3f edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
                Vec3f normal = {
                    edge1.y * edge2.z - edge1.z * edge2.y,
                    edge1.z * edge2.x - edge1.x * edge2.z,
                    edge1.x * edge2.y - edge1.y * edge2.x
                };

                std::cout << "[法线测试2] Triangle 0 法线: (" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
            }
        }
    }

    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    rtcSetSharedGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        vertices.data(), 0, sizeof(Vec3f), vertices.size());

    rtcSetSharedGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        indices.data(), 0, 3 * sizeof(unsigned int), indices.size() / 3);

    rtcCommitGeometry(geom);
    unsigned int geomID = rtcAttachGeometry(scene, geom);
    std::cout << "[addWallPlaneXY] attach 返回 geomID = " << geomID << "\n";
    rtcReleaseGeometry(geom);

    std::cout << "[addWallPlaneXY] 接收 device: " << device << "\n";
    std::cout << "[addWallPlaneXY] 接收 scene : " << scene << "\n";

    return geomID;
}

unsigned int addWallPlaneXYSafe(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float width, float height,
    float tileSize)
{
    assert(scene && device);
    assert(tileSize > 0.0f);

    int tilesX = std::ceil(width / tileSize);
    int tilesY = std::ceil(height / tileSize);

    float dx = width / tilesX;
    float dy = height / tilesY;

    std::vector<Vec3f> vertices;
    std::vector<unsigned int> indices;

    float x0 = center.x - width * 0.5f;
    float y0 = center.y - height * 0.5f;
    float z = center.z;

    // 构造顶点
    for (int iy = 0; iy <= tilesY; ++iy) {
        for (int ix = 0; ix <= tilesX; ++ix) {
            float x = x0 + ix * dx;
            float y = y0 + iy * dy;
            vertices.emplace_back(x, y, z);
        }
    }

    // 构造三角形索引，确保法线朝 -Z（爆炸方向）
    for (int iy = 0; iy < tilesY; ++iy) {
        for (int ix = 0; ix < tilesX; ++ix) {
            int i0 = iy * (tilesX + 1) + ix;
            int i1 = i0 + 1;
            int i2 = i0 + (tilesX + 1);
            int i3 = i2 + 1;

            // 逆时针：v0 v1 v2 -> 法线朝 +Z
            indices.push_back(i2);
            indices.push_back(i1);
            indices.push_back(i0);

            indices.push_back(i2);
            indices.push_back(i3);
            indices.push_back(i1);
        }
    }

    // 创建几何体
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    // 分配新顶点缓冲区并复制
    void* vbuf = rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        sizeof(Vec3f), vertices.size());
    std::memcpy(vbuf, vertices.data(), sizeof(Vec3f) * vertices.size());

    // 分配新索引缓冲区并复制
    void* ibuf = rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        3 * sizeof(unsigned int), indices.size() / 3);
    std::memcpy(ibuf, indices.data(), sizeof(unsigned int) * indices.size());

    rtcCommitGeometry(geom);
    unsigned int geomID = rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);

    // ------------------ 3. 立刻导出这面墙的 CSV -------------
    // ② 调用 result 模块里的导出函数
    exportWorldCSV(vertices, indices, "world_init.csv");


    std::cout << "[addWallPlaneXYSafe] attach geomID = " << geomID << "\n";
    return geomID;
}


unsigned int addSquarePlane(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float size)
{
    assert(scene && device);

    float half = size * 0.5f;
    float z = center.z;

    static std::vector<Vec3f> vertices = {
        {center.x - half, center.y - half, z},  // v0
        {center.x + half, center.y - half, z},  // v1
        {center.x - half, center.y + half, z},  // v2
        {center.x + half, center.y + half, z}   // v3
    };

    const Vec3f& v0 = vertices[0];
    const Vec3f& v1 = vertices[2];
    const Vec3f& v2 = vertices[1];
    Vec3f edge1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
    Vec3f edge2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };
    Vec3f normal = {
        edge1.y * edge2.z - edge1.z * edge2.y,
        edge1.z * edge2.x - edge1.x * edge2.z,
        edge1.x * edge2.y - edge1.y * edge2.x
    };
    std::cout << "[addSquarePlane] 第一个三角形法线 = (" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";


    // ✅ 顺时针构造，让法线朝 -Z
    static std::vector<unsigned int> indices = {
        0, 2, 1,
        1, 2, 3
    };

    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    rtcSetSharedGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        vertices.data(), 0, sizeof(Vec3f), vertices.size());

    rtcSetSharedGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        indices.data(), 0, 3 * sizeof(unsigned int), indices.size() / 3);

    rtcCommitGeometry(geom);
    unsigned int geomID = rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);

    std::cout << "[addSquarePlane] 添加正方形面，法线朝 -Z，geomID = " << geomID << "\n";
    return geomID;
}

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
