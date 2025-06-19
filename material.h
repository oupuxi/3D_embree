//material.h  ����ϵ����ѯ
#pragma once
#include <unordered_map>
#include <embree4/rtcore_geometry.h>
inline std::unordered_map<unsigned, RTCGeometry> g_geomToMat; // ���Ժ�Ҫ��

inline float getReflectionCoeff(unsigned geomID)
{
    // ��ͳһ 0.5���������� geomID ���
    return 0.5f;
}

