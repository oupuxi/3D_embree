//material.h  反射系数查询
#pragma once
#include <unordered_map>
#include <embree4/rtcore_geometry.h>
inline std::unordered_map<unsigned, RTCGeometry> g_geomToMat; // 若以后要用

inline float getReflectionCoeff(unsigned geomID)
{
    // 先统一 0.5，后续根据 geomID 查表
    return 0.5f;
}

