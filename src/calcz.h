#if !defined(__CALCZ_H__)
#define __CALCZ_H__
#include <math.h>
#include <stdlib.h>
#include "compl.h"
#include "adc.h"
#include "event.h"
extern uint8_t AutoDiapason; // ��������� ��������� � ����� ����������
#define AUTO_DIAPASON_ON    0x1 // ������� ��������� ���������
#define AUTO_DIAPASON_CHECK 0x2 // + ����� ��� ���������� ������ - ����� ����� ��� ����� ���������
#define AUTO_DIAPASON_RESET  0x4 // + ����� ����� ������� ���������

extern uint8_t OverloadFlag; // ���� ����������. ��������� ����� ������ CalculationZm

void ResetZm(void); // ������� ��������������� ������ �����������
void SetDiapason(void);// ������� ��������� ��������� �� ��������� ���������. ������� ���������� � 0 ��� ��������� ���������. ����� ��������� ��� ����� ������������ ��������

// ��������� �� ��������� ResultCosSum ResultSinSum ������� Zm. �� ��������� ResultMax ResultMin �������� �������� ���� ��� ��������
uint8_t CalculationZm(void); // ������� ��������� ����������� ������������� � �������� � ���� ����������

extern Complex_t ZmSum; //��������� - ����� ����������� ��������� Z 
#if defined(MSE)
extern float SqZmSumReal; //��������� - ����� ��������� �������������� ����� Z
extern float SqZmSumImag;//��������� - ����� ��������� ������ ����� Z
extern float MsePrev; // ���������� ������
#endif
extern uint32_t ZmMeasCount;//��������� - ���������� ��������� Z

#if defined(TEST_CALCZ)
void CalculationZ(void);
#endif

extern uint8_t AdcEvCounter; //����� ���������


#endif /* __CALCZ_H__ */
