// result.h  ������Ϣ��ӡ / PLY & CSV ���� / �����ۻ�
#pragma once
#include "ray.h"
#include "types.h"
#include<vector>
#include <string>
/** ��ӡ�������ߵ�������Ϣ�������ã�*/
void processAndPrintRay(const MyRay& ray, int index = -1);

/** �ռ�һ����Ч���У����ں������� PLY / CSV */
void collectHitPoint(const MyRay& ray);         // �ۻ��������е�

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
// ---------------- ȫ���ۻ����� ----------------
extern std::vector<float>  g_peakEnergy;   ///< ÿ�������εķ�ֵ����
extern std::vector<size_t> houseOffsets;   ///< houseID �� globalPrimID ƫ����
