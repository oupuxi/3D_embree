//config.h  ����ȫ�ֳ���
#pragma once
// �����ģ�����
constexpr float E0 = 3.0f;				// ��ʼ���������ⵥλ��
constexpr float SPEED_C = 340.0f;		// �����ٶȣ���λ m/s
constexpr float DECAY_K = 0.01f;        // ����˥������ 1/m����ʹ�ã�
constexpr int   MAX_BOUNCE = 5;			// ��������
constexpr float MIN_ENERGY_FRAC = 0.01f;// ��ֹ��������
constexpr int   NUM_RAYS = 100'000;		// ��������
constexpr float RAY_EPS = 1e-3f;		// ��ʼƫ��
