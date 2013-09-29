#if !defined(__GPIP_H__)
#define __GPIP_H__
#include "stm32f10x.h"
// ����������� ���� ������ ���������
//#define PIN_NAME PORT, PIN, OUT_MODE_SPEED, SET
// PORT - A, B, C, D � �� - ���� ��
// PIN - ����� ���� � �����
// OUT_MODE_SPEED - ����� ������ � �������� 
// SET - �������� ����� ������������� ��� ������ ��� �������� ��� �����
// ��������. ����� ����� ������ ��� ������. ������������ | � �������
#define OUT10MHZ GPIO_CRL_MODE0_0  // ����� ������ ���������������� - � ���� MODE
#define OUT2MHZ  GPIO_CRL_MODE0_1  // ���������������
#define OUT50MHZ (GPIO_CRL_MODE0_1|GPIO_CRL_MODE0_0)  // ����������������

// ������
#define OUT_PUSHPULL 0 // ���������� �����
#define AOUT_PUSHPULL GPIO_CRL_CNF0_1 //�������������� ����������
#define OUT_OPENDRAIN GPIO_CRL_CNF0_0 // �������� ����
#define AUOT_OPENDRAIN (GPIO_CRL_CNF0_0|GPIO_CRL_CNF0_1) //�������������� �������� ����
//
#define AINPUT 0  //���������� ����
#define INPUT GPIO_CRL_CNF0_0  // ���� �������� 
#define INPUT_PULL GPIO_CRL_CNF0_1 //���� �������� � ���������

//��������
#define PULL_DOWN 0 // �������� � ����� - ���������� ���������� �������� ��������-������
#define PULL_UP   1 // �������� � �������


#define _TO_GPIO_CRL(DEST_PORT, PORT, PIN, MODE, OUT) \
  ((&GPIO##DEST_PORT->CRL == &GPIO##PORT->CRL)*(PIN<8)*(MODE))<<(PIN*4)
#define _TO_GPIO_CRH(DEST_PORT, PORT, PIN, MODE, OUT) \
  ((&GPIO##DEST_PORT->CRH == &GPIO##PORT->CRH)*(PIN>=8)*(MODE))<<((PIN-8)*4)
//������� ������������ ��� ���������� ������������ �������� ��� �������� ���������. ������� ������ ������������ ����� |
#define TO_GPIO_CRL(DEST_PORT, PORT_DESC) _TO_GPIO_CRL(DEST_PORT, PORT_DESC)
#define TO_GPIO_CRH(DEST_PORT, PORT_DESC) _TO_GPIO_CRH(DEST_PORT, PORT_DESC)

#define _TO_GPIO_BSRR(DEST_PORT, PORT, PIN, MODE, OUT) /*((OUT)==0)?0:*/((&GPIO##DEST_PORT->BSRR == &GPIO##PORT->BSRR)*(OUT<<PIN))
//������������� ������� ���������-������. ������������ ���� ����� ������������� ��� ��������
#define TO_GPIO_BSRR(DEST_PORT, PORT_DESC) _TO_GPIO_BSRR(DEST_PORT, PORT_DESC)

#define _GPIO_SET(PORT, PIN, MODE, OUT) GPIO##PORT->BSRR = (1 << PIN)
#define _GPIO_ISSET(PORT, PIN, MODE, OUT) (GPIO##PORT->IDR & (1 << PIN))
#define _GPIO_RESET(PORT, PIN, MODE, OUT) GPIO##PORT->BRR = (1 << PIN)
#define _GPIO_TOGGLE(PORT, PIN, MODE, OUT) GPIO##PORT->ODR ^= (1 << PIN)


//����� ������� ������ � ������
#define GPIO_SET(PIN_DESC) _GPIO_SET(PIN_DESC)
#define GPIO_RESET(PIN_DESC) _GPIO_RESET(PIN_DESC)
#define GPIO_TOGGLE(PIN_DESC) _GPIO_TOGGLE(PIN_DESC)
#define GPIO_ISSET(PIN_DESC) _GPIO_ISSET(PIN_DESC)

#define _GPIO_BB(PORT, PIN, MODE, OUT) (*(uint32_t*)(PERIPH_BB_BASE + ((uint32_t)&(GPIO##PORT->ODR) -  PERIPH_BASE)*32 + PIN*4))
// ������ ������ � ������ ����� bit-bang
#define GPIO_BB(PIN_DESC) _GPIO_BB(PIN_DESC)
#endif /* __GPIP_H__ */
