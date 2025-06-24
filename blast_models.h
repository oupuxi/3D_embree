#pragma once
// blast_models.hըҩ���������ģ�ͺ��������ݽṹ����
#include <utility>   // std::pair
#include "config.h"
//------------------------------------------------------------------------------
/// @brief Kingery�CBulmash �����
/// @note ������ֵ��ѹ������λ����ʱ��� Friedlander ��״����
struct BlastKBResult {
	float Z;     ///< ���߾��� (m��kg^(-1/3))
    float Ps;    ///< ��ֵ��ѹ (kPa)
    float td;    ///< ����λ����ʱ�� (ms)
    float beta;  ///< Friedlander ��״���� (������)
    float I;     // ������Pa��s��
    float ta;    // ����ʱ��
};

//------------------------------------------------------------------------------
/// @brief �������߾��� Z = R / W^(1/3)
/// @param R ��ըԴ��Ŀ������ (m)
/// @param W TNT �������� (kg)
/// @return ��һ������ Z (m��kg^(-1/3))
inline  float blast_scaled_distance(float R, float W);

//------------------------------------------------------------------------------
/// @brief �������߾��� Z ���� K-B ��ֵ
/// @param Z ��һ������ (m��kg^(-1/3))
/// @return BlastKBResult ���� Ps��td��beta
BlastKBResult blast_kb_lookup(float Z);

//------------------------------------------------------------------------------
// @brief ���� Friedlander ���뱬ը���� P(t)
// @param t    ʱ�� (ms)
// @param Ps   ��ֵ��ѹ (kPa)
// @param td   ����λ����ʱ�� (ms)
// @param beta ��״���� (������)
// @return ʱ�� t �ĳ�ѹ (kPa)���� t<0 �� t>td ���� 0
inline float blast_friedlander(float t, float Ps, float td, float beta);

//------------------------------------------------------------------------------
/// @brief ָ������˥�� E = E0 * exp(-K * d)
/// @param E0 ��ʼ����
/// @param K  ˥������ (m^-1)
/// @param d  �������� (m)
/// @return ˥���������
inline float blast_exp_decay(float E0, float K, float d);

//------------------------------------------------------------------------------
/// @brief ���ݷ�ֵ��ѹ�����ʵ��ٶ� u_p �� Ps / (�� * c0)
/// @param Ps  ��ֵ��ѹ (Pa)
/// @param rho �����ܶ� (kg/m^3)
/// @param c0  ���� (m/s)
/// @return �ʵ��ٶ� (m/s)
inline float blast_particle_velocity(float Ps, float rho, float c0);

//------------------------------------------------------------------------------
/// @brief �迹����ϵ�� Cr = sqrt(c2 / c1)
/// @param c1 ����������� (m/s)
/// @param c2 ����������� (m/s)
/// @return �迹����ϵ�� (������)
inline float blast_impedance_correction(float c1, float c2);