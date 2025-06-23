// vec_math.h
#pragma once
#include "types.h"
#include <cmath>

inline float dot(const Vec3f& a, const Vec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3f cross(const Vec3f& a, const Vec3f& b) {
    return Vec3f{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

inline float length(const Vec3f& v) {
    return std::sqrt(dot(v, v));
}

inline Vec3f normalize(const Vec3f& v) {
    float len = length(v);
    return (len > 0.f) ? (v / len) : Vec3f{ 0, 0, 0 };
}
