//geometry.h ����ǽ/���ӵ� Embree ���β� attach ����
#pragma once
#include <embree4/rtcore.h>
#include"types.h"
#include <vector>
#include <utility> // for std::pair
#include <unordered_set>
// �ṩһ�����������ĺ����������ѹ����õ� scene��


// һ������
// center�� ��Χ�����ģ�x�߳�length��z�߳�width��y�߶�height
// tileSize��ÿ����Ƭ�ı߳���Ĭ�� 1 m
// withRoof���Ƿ�����ݶ���Ĭ�����
std::pair<unsigned int, size_t> addHouse(
    RTCScene scene, RTCDevice device,
    const Vec3f& center,            // ���Ӱ�Χ������
    float length, float width,      // X, Z ����߳����� 10 m��
    float height,
    std::vector<Vec3f>& allVertices,
    std::vector<unsigned int>& allIndices,                   // Y �߶ȣ�20 m��
    float tileSize = 1.0f,          // ÿ���С��Ĭ�� 1 m
    bool withRoof = true,
    bool withFloor = false);

// ��������¼����̽���� geomID �ļ���
extern std::unordered_set<unsigned int> g_sensorGeomIDs;
