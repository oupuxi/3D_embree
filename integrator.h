// integrator.h  ������tracer�ص�
#pragma once
#include <embree4/rtcore.h>
#include "ray.h"      // MyRay ����
// ����Ҫ�� forward ������ include material.h / result.h

namespace sim   // �� �Ժ����뻻�������Ҳ��
{

    /**
     * ��ε���׷��һ������
     *
     * @param ray            I/O: ���߽ṹ�壨�ᱻ�͵ظ��£�
     * @param scene          Embree ����
     * @param maxDepth       ����������������䣩
     * @param minEnergyFrac  �������� E0*frac ����ֹ
     */
    void traceRayMultiBounce(
        MyRay& ray,
        RTCScene scene,
        int      maxDepth = 5,
        float    minEnergyFrac = 0.01f);

} // namespace sim
