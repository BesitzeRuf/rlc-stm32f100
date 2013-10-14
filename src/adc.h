#if !defined(__SIGNALS_H__)
#define __SIGNALS_H__
#include "stm32f10x.h"

struct FrenqSetup_t;
// ������� ������������� ��������� � ��������� ���������� �� ���������
typedef void (*InitFrenq_t) (const struct FrenqSetup_t*);
void InitForCalcualtingWithDacFill(const struct FrenqSetup_t* FSetup);
void InitForCalcualtingWithoutDacFill(const struct FrenqSetup_t* FSetup);

void Init(void); // init the measure system. Do once at start

// ��������� �������������
typedef struct FrenqSetup_t
{
  InitFrenq_t InitFunc;   // ������� �������������
  int16_t  DacPrescaler;  //��������� ��� ��� ��������
  int16_t  AdcPrescaler;  // ��������� ��� ��� ��������
  int16_t*  DacSource; /* Source for DAC DMA. pointer as integer */
  int16_t  DacSize;   /* Size for DAC DMA */
  int16_t  RealTableSize; /* Size of the real sin table */
  const int16_t*  Sin; /* Table for fourier factor for 1/4 period */
  const int32_t*  CosSin; /* Table for fourier factors (sin and cos) for 1 period */
  uint16_t OptimalADCSize; // Optimal size for easy geting 10Hz frenq for measure
  uint16_t OptimalCallCount; // Optimal interrupt count for easy geting 10Hz frenq for measure
}
FrenqSetup_t;

void CalcualtingWithDacFill(int16_t Cur); // ������� ��������� ��������� ����������. ��� ��������� ����� ������ ��� ���
void CalcualtingWithoutDacFill(int16_t Cur); // ������ ��� ��� �� ����������� - ����� �� �����

#define ADC_ARRAY_SIZE 496
#define SLOW_ARRAY_SIZE 500
#define ARRAY_SIZE 500
extern int16_t ADCResults[ARRAY_SIZE];
extern int16_t DACInput[ARRAY_SIZE];


#define FRENQ0   4
#define FRENQ100 0
#define FRENQ1K  1
#define FRENQ10K 2
#define FRENQ50K 3
extern uint8_t CurrentFreq; // ������ ������� �� �������
void SetTheFrenq(uint8_t Frenq); //������� ��������� �������. ����� ������������ ������� ����. �� ����

// ������� �������-��������� ��� ��������� � ���������
extern const int16_t DacArrayDC; // dac input dor DC
extern const int16_t DacSinArray50[11]; // dac input 50K
extern const int32_t SinCosArray50[11]; // Sin Cos array 50K
extern const int16_t DacSinArray10[55]; // Dac input 10K
extern const int32_t SinCosArray10[55]; // Sin Cos array 10K

#define BEGIN_FLAG 0x8000
#define END_FLAG 0x4000
extern const int16_t SinArray1[41];  // 1/4 period Sin array 1K for sin cos and dac calculation
extern const int16_t SinArray100[401]; // 1/4 period Sin array 100Hz for sin cos and dac calculation

// ���������� �����
#define INST_OUT ADC_SQR3_SQ1_0|ADC_SQR3_SQ1_1  // 3 �����
#define MAIN_OUT ADC_SQR3_SQ1_1 // 2 �����
#define PWR_OUT  ADC_SQR3_SQ1_3 // 8 �����
#define REF_OUT  ADC_SQR3_SQ1_4|ADC_SQR3_SQ1_0 // 17 �����
#define SetAdcInput(Inp)   ADC1->SQR3 = Inp;
//extern uint32_t AdcInput; // ������� ����. 

// ���������� �������� ���������� ���������
#if defined(PWRMEAS)
extern uint16_t ResultPowerAdc; // �������
extern uint16_t ResultRefAdc;  // �����
#endif
extern int16_t ResultMin; // ����������� �������� ��� -- ������������ �� ������ ���������
extern int16_t ResultMax;  // ������������ �������� ���
extern int32_t ResultAverage; // ������� �������� ��������� - ��� ��������� ���������� ������������
extern int64_t ResultCosSum; // ����� ������������ �� �������
extern int64_t ResultSinSum; // ����� ������������ �� �����
//extern uint16_t ResultPowerAdc; //��������� ��������� ������� 3.3�
//extern uint16_t ResultRefAdc;  //��������� ��������� ���������� �����


//������� ������ ������������ ��� ���������� ���������
#define K1_SWITCH 0x01
#define K2_SWITCH 0x02
#define K3_SWITCH 0x04
#define R_SWITCH1 0x08
#define R_SWITCH2 0x10
#define I_SWITCH  0x80 // ������� ��� �������  ��� ����� �������� ��� � �� ����������

#define R_100 0
#define R_1K  R_SWITCH1
#define R_10K R_SWITCH2
#define R_100K (R_SWITCH2|R_SWITCH1)
void SetSwitches(uint8_t Switches); // ��������� ������. ���������� ������� ����

typedef struct Diapason_t
{
  uint8_t VV;
  uint8_t II;
}Diapason_t; // ��������� ����������� ���������. ���� - ���������� ������ ��� ���� � ����������
extern const Diapason_t Diapasons[18];

extern int8_t CurrentDiapason; // ������� �������� ���������
void AdcDelay(int8_t Count); // ���������� count ������ ���������

#if defined(TEST_ADC)
void TestMenu(void);
void TestAdcMenu(void);
extern uint32_t HandlerTicks;
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC
#endif


#endif /* __SIGNALS_H__*/

