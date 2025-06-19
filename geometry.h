//geometry.h ��װ�����幹�����̣���������������һ�仰���ܹ����������� 
#pragma once
#include <embree4/rtcore.h>
#include"types.h"
#include <vector>
#include <utility> // for std::pair
// �ṩһ�����������ĺ����������ѹ����õ� scene��
RTCScene createSimpleScene(RTCDevice device);

unsigned int addSquarePlane(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float size);  // size �Ǳ߳�


unsigned int addWallPlaneXY(RTCScene scene, RTCDevice device,
    const Vec3f& center, // X, Y ����λ�ã�Z ��ǽ����ƽ��λ��
    float width, float height,
    float tileSize = 1.0f);
// �� Embree ���������һ�� XY ƽ���ϵġ���Ƭ����ǽ�壨��������
// center: ǽ�����ĵ�λ��
// width, height: ǽ�Ŀ��
// tileSize: ÿ��С��Ƭ�ı߳�����������ϸ�̶ֳȣ�
// ����ֵ�Ǹü������ڳ����е� geomID
unsigned int addWallPlaneXYSafe(RTCScene scene, RTCDevice device,
    const Vec3f& center,
    float width, float height,
    float tileSize);

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
