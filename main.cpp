// main.cpp  仿真主流程（设备 + 场景 + 射线 + 统计）

#include <iostream>
#include<cassert>
#include"geometry.h"
#include"ray.h"
#include"tracer.h"
#include"result.h"

int main()
{
	// 1. 初始化 Embree 设备-----------------------------------
	RTCDevice device = rtcNewDevice(nullptr);
	assert(device);
	// 2. 创建场景---------------------------------------------
	RTCScene scene = rtcNewScene(device);
	// 3. 构建房子------------------------------------------------
	std::vector<Vec3f> allVertices;
	std::vector<unsigned int> allIndices;

	// ➔ 每建一栋房子，记录返回的三角形数量
	auto [geomID0, numTriangles0] = addHouse(scene, device, { 15, 10,  15 }, 10.0f, 10.0f, 20.0f, allVertices, allIndices, 1.0f, true, false);
	houseOffsets.push_back((allIndices.size() / 3) - numTriangles0);

	auto [geomID1, numTriangles1] = addHouse(scene, device, { 15, 10, -15 }, 10.0f, 10.0f, 20.0f, allVertices, allIndices, 1.0f, true, false);
	houseOffsets.push_back((allIndices.size() / 3) - numTriangles1);

	auto [geomID2, numTriangles2] = addHouse(scene, device, { -15, 10,  15 }, 10.0f, 10.0f, 20.0f, allVertices, allIndices, 1.0f, true, false);
	houseOffsets.push_back((allIndices.size() / 3) - numTriangles2);

	auto [geomID3, numTriangles3] = addHouse(scene, device, { -15, 10, -15 }, 10.0f, 10.0f, 20.0f, allVertices, allIndices, 1.0f, true, false);
	houseOffsets.push_back((allIndices.size() / 3) - numTriangles3);

	rtcCommitScene(scene);// 场景构建完成

	/*
	*
	* // 4. 初始化每个三角形的峰值能量表------------------------------
	size_t numTriangles = allIndices.size() / 3;
	g_peakEnergy.assign(numTriangles, 0.0f);
	// 导出初始场景世界几何（不带能量）
	exportWorldCSV(allVertices, allIndices, "world_4houses_init.csv");
	// 5. 生成射线----------------------------------------------
	auto rays = generateSphereRays(100000);
	std::cout << "生成了 " << rays.size() << " 条射线，开始追踪...\n";


	const float kMinEnergyFrac = 0.01f; // 剩能低于 1% 立即终止

	for (size_t i = 0; i < rays.size(); ++i)
	{
		traceRayMultiBounce(rays[i], scene, cfg::MAX_BOUNCES, kMinEnergyFrac);

		// （可选）只打印首个命中；如需打印全部 bounce，请在 integrator 里打印
		if (rays[i].time >= 0.0f)
			processAndPrintRay(rays[i], static_cast<int>(i));
		// 注意：collectHitPoint() 已在 traceRayMultiBounce 内部执行，别再调用
	}
	// 8. 导出带能量信息的几何 CSV
	exportWorldCSV(allVertices, allIndices, "world_4houses_energy.csv");

	// 可选
	// 每栋楼总能量统计
	for (size_t geomID = 0; geomID < houseOffsets.size(); ++geomID) {
		size_t start = houseOffsets[geomID];
		size_t end = (geomID + 1 < houseOffsets.size()) ? houseOffsets[geomID + 1] : g_peakEnergy.size();

		double sumEnergy = 0.0;
		for (size_t i = start; i < end; ++i) {
			sumEnergy += g_peakEnergy[i];
		}

		std::cout << "房子 " << geomID << " 总能量: " << sumEnergy << "\n";
	}
	*/

	rtcReleaseScene(scene);
	rtcReleaseDevice(device);
	std::cout << "仿真结束，文件已导出！\n";
	return 0;
}
