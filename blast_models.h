#pragma once
// blast_models.h ���� ըҩ���������ģ�ͺ��������ݽṹ����

#include <array>      // std::array
#include <algorithm>  // std::upper_bound�����ڲ�ֵ���
#include <cmath>      // std::pow, std::log10 ����ѧ����
#include <utility>    // ��ѡ������ std::pair ��
#include "config.h"

//------------------------------------------------------------------------------
/// @brief Kingery�CBulmash �����
/// @note ������ֵ��ѹ������λ����ʱ�䡢��������״���ӵ�
struct BlastKBResult {
	float Z;      ///< ���߾��� (m��kg^(-1/3))
	float Ps;     ///< ��ֵ��ѹ (kPa)
	float td;     ///< ����λ����ʱ�� (ms)
	float beta;   ///< Friedlander ��״���� (������)
	float I;      ///< ���� (kPa��ms)
	float Us;     ///< �����ǰ�ٶ� (m/s)
	float ta;     ///< ��ֵ����ʱ�� (ms)
};

//------------------------------------------------------------------------------
/// @brief Kingery�CBulmash ���ݱ���ṹ
struct KBEntry {
	float Z;      ///< ���߾��� (m��kg^(-1/3))
	float Ps;     ///< ��ֵ��ѹ (kPa)
	float td;     ///< ����λ����ʱ�� (ms)
	float beta;   ///< Friedlander ��״���� (������)
	float I;      ///< ���� (kPa��ms)
	float Us;     ///< �����ǰ�ٶ� (m/s)
	float ta;     ///< ��ֵ����ʱ�� (ms)
};

//------------------------------------------------------------------------------
/// @brief ʾ�� K�CB ���ݱ����� 3 ��������¼��
/// @note �滻Ϊ���� K�CB �������ڲ�ֵ��ѯ
static constexpr std::array<KBEntry, 3> g_kb_table = { {
		//    Z       Ps       td     beta      I       Us      ta
		{ 0.2f, 1586.0f,  0.30f, 1.90f,  900.0f, 1030.0f,  0.30f },
		{ 1.0f,  185.0f,  1.70f, 1.85f,  600.0f,  500.0f,  4.50f },
		{ 5.0f,    9.8f, 13.00f, 1.80f,   80.0f,  200.0f, 17.00f }
} };

//------------------------------------------------------------------------------
/// @brief �������߾��� Z = R / W^(1/3)
/// @param R ���� (m)
/// @param W TNT �������� (kg)
/// @return ���߾��� Z (m��kg^(-1/3))
inline float blast_scaled_distance(float R, float W);

//------------------------------------------------------------------------------
/// @brief �������߾������ K�CB ��ֵ
/// @param Z ���߾��� (m��kg^(-1/3))
/// @return ��ֵ�õ��� BlastKBResult
BlastKBResult blast_kb_lookup(float Z);

//------------------------------------------------------------------------------
/// @brief Friedlander ��ը���� P(t) ģ��
/// @param t    ʱ�� (ms)
/// @param Ps   ��ֵ��ѹ (kPa)
/// @param td   ����λ����ʱ�� (ms)
/// @param beta Friedlander ��״����
/// @return ָ��ʱ�� t �ĳ�ѹֵ (kPa)��t ��������λ���� 0
inline float blast_friedlander(float t, float Ps, float td, float beta);

//------------------------------------------------------------------------------
/// @brief ָ������˥��ģ�� E = E0 * exp(-K * d)
/// @param E0 ��ʼ����
/// @param K  ˥������ (1/m)
/// @param d  ���� (m)
/// @return ˥���������
inline float blast_exp_decay(float E0, float K, float d);

//------------------------------------------------------------------------------
/// @brief �����ʵ��ٶ� u_p �� Ps / (�� * c0)
/// @param Ps  ��ֵ��ѹ (Pa)
/// @param rho �����ܶ� (kg/m^3)
/// @param c0  ���� (m/s)
/// @return �ʵ��ٶ� (m/s)
inline float blast_particle_velocity(float Ps, float rho, float c0);

//------------------------------------------------------------------------------
/// @brief ��������ϵ�� Cr = sqrt(c2 / c1)
/// @param c1 ����������� (m/s)
/// @param c2 ����������� (m/s)
/// @return ����ϵ�� Cr (������)
inline float blast_impedance_correction(float c1, float c2);
