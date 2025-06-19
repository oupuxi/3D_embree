//tracer.h   �� �����󽻢� ��η���ѭ���� ����˥��& collectHitPoint
#pragma once
#include <embree4/rtcore.h>
#include "ray.h"

// ��������׷��
void traceRay(MyRay& ray, RTCScene scene);

// ��ѡ��չ������������������
//void traceRays(std::vector<MyRay>& rays, RTCScene scene);

void traceRayMultiBounce(MyRay& ray,
    RTCScene scene,
    int      maxDepth = 5,
    float    minEnergyFrac = 0.01f); // < E0*1% ʱ��ֹ