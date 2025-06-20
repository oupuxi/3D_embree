// result.h  ������Ϣ��ӡ / PLY & CSV ���� / �����ۻ�
#pragma once
#include "ray.h"
#include "types.h"
#include <string>
void processAndPrintRay(const MyRay& ray, int index = -1);

// �������
void collectHitPoint(const MyRay& ray);         // �ۻ��������е�
//void writePLY(const std::string& filename,      // ��ȫ������д�� PLY
//    bool colorByEnergy = true);
// ÿ��������ͳ��һ�η�ֵ
extern std::vector<float> g_peakPressure;

/**
 * ����ǰ���缸�ε���Ϊ CSV��
 *  ��˳��triID,x0,y0,z0,x1,y1,z1,x2,y2,z2
 *
 * @param verts   �������飨��������ʱ�����У�
 * @param idx     �������������飨3 * nTri��
 * @param path    ����ļ��������� "world_init.csv"
 */
void exportWorldCSV(const std::vector<Vec3f>& verts,
    const std::vector<unsigned>& idx,
    const std::string& filename = "world_init.csv");
// ȫ�������� ֻ����
extern std::vector<float> g_peakEnergy;
extern std::vector<size_t> houseOffsets;  
