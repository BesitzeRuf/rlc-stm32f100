#if !defined(__IO_H__)
#define __IO_H__

#include "gpio.h"

#define PWR_ON    A, 1, OUT_PUSHPULL|OUT2MHZ, 1  // ���������-��������� �������
#define K3        A, 6, OUT_PUSHPULL|OUT2MHZ, 0  // ���� �������� ��������� ����� ����
#define K2        A, 7, OUT_PUSHPULL|OUT2MHZ, 1  // ���� �������� ��� ���������
#define ADC       A, 2, AINPUT, 0 // ������� ���������� ������ - 2 ���������� ����
#define INSTR_OUT A, 3, AINPUT, 0 // ������ ����� ����� ���� - ������������ ��� ��������� �������� �� ����������� ���� - 3 ������ ����
#define DAC_OFF   A, 4, AINPUT, 0 // �������� ������� ��������� - ��� 1 �����
#define DAC_M     A, 5, AINPUT, 0 // �������� ������ ������ - ��� 2 �����

#define V_BAT     B, 0, AINPUT, 0 // ���������� ������� ����� ��������  - 8 ���������� ����
#define BUT1      B, 7, INPUT_PULL, PULL_UP // ������ ������
#define BUT2      B, 6, INPUT_PULL, PULL_UP // ������ ������
#define PROBE1    B, 5, INPUT_PULL, PULL_UP // ��������� ������������� ����
#define PROBE2    B, 4, INPUT_PULL, PULL_UP // ��������� ������������� ����
#define IV1       B, 1, OUT_PUSHPULL|OUT2MHZ, 0 // �������� � iv ���������������
#define IV2       B, 2, OUT_PUSHPULL|OUT2MHZ, 0 // �������� � iv ���������������

#define LCD_LED   A, 12, OUT_PUSHPULL|OUT2MHZ, 0 // ���������
#if defined(LCD_USART)
#define LCD_CLK   A, 8, AOUT_PUSHPULL|OUT2MHZ, 0 // �������� ������ �������
#define LCD_DA    A, 9, AOUT_PUSHPULL|OUT2MHZ, 0 // ������ �������
#else
#define LCD_CLK   A, 8, OUT_PUSHPULL|OUT2MHZ, 0
#define LCD_DA    A, 9, OUT_PUSHPULL|OUT2MHZ, 0
#endif
#define LCD_CE    A, 10, OUT_PUSHPULL|OUT2MHZ, 0 // ����� ��������� �������
#define LCD_RESET A, 11, OUT_PUSHPULL|OUT2MHZ, 0 // ����� �������

#define I_V_MEAS  B, 11, OUT_PUSHPULL|OUT2MHZ, 0 // ������������� ��������� ���� ��� ����������
#define K1        B, 10, OUT_PUSHPULL|OUT2MHZ, 0 // ���� �������� ������������� ����� ���������
#define PWM       B, 9, AOUT_PUSHPULL|OUT2MHZ, 0 // �������� ��������� ��������
#define BUT3      B, 8, INPUT_PULL, PULL_UP // ������ ������
#define CP0       B, 13, AOUT_PUSHPULL|OUT10MHZ, 0 // ������ �� ��������� �����
#define CP1       B, 14, AOUT_PUSHPULL|OUT10MHZ, 0 // ������ �� ��������� �����

#endif /* __IO_H__ */
