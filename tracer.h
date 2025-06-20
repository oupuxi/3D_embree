//tracer.h   �� �����󽻢� ��η���ѭ���� ����˥��& collectHitPoint
#pragma once
#include <embree4/rtcore.h>
#include "ray.h"
#include <span>
// ��������׷��
void traceRay(MyRay& ray, RTCScene scene);

// �ݹ鷴�� / ����˥��
void traceRayMultiBounce(MyRay& ray,
                         RTCScene scene,
                         int      maxDepth = 5,
                         float    minEnergyFrac = 0.01f); // < E0*1% ʱ��ֹ

/** �� **�����ӿ�** ���� �ȴ���ʵ�֣�δ���ɲ��� / SIMD
    ��������
        traceBatch(rays, scene, 5, 0.01f);
*/
void traceBatch(std::span<MyRay> rays,
    RTCScene         scene,
    int              maxDepth = 5,
    float            minEnergyFrac = 0.01f);