//types.h  Vec3f & һЩͳ�ƽṹ
#pragma once
// �򵥵� 3D �����ṹ�����ڱ�ʾ����
struct Vec3f {
    float x, y, z;
    Vec3f() = default;
    Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

struct FaceStat {
    int    hits = 0;
    double sumE = 0.0;
};
//static std::vector<FaceStat> g_faceStats;   // ��С = ��������
