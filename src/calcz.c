#include "calcz.h"
#include "calibration.h"
#include "itoa.h"
#include "n1202.h"
#include "main.h"


uint8_t FixDiapason; // ������� ���������� ���������� ���������

#define ZARRAYSIZE 6 // ������ ������� ����� ��������� = (ZARRAYSIZE*2 + 2)*0.1���
static Complex_t VArray[ZARRAYSIZE]; // ������ ��� �������� ��������� ����������
static Complex_t IArray[ZARRAYSIZE]; // ������ ��� �������� ��������� ����
uint8_t AdcEvCounter; //����� ���������
uint8_t Step; //��� ���������

Complex_t ZmSum; //��������� - ����� ����������� ��������� Z 
#if defined(MSE)
float SqZmSumReal; //��������� - ����� ��������� �������������� ����� Z
float SqZmSumImag;//��������� - ����� ��������� ������ ����� Z
#endif
uint32_t ZmMeasCount;//��������� - ���������� ��������� Z
uint8_t OverloadFlag;// ��������� - ���� ����������.

#define MFLAG_V 0x1 // ��� ��������� � ������ ��������� ����������
#define MFLAG_NO_SWITCH 0x2 // �� ����� ������������� �� ��������� ���� ���������. ����� ���������� ��������� �����. ����������� ����� ����� ��� ��������
#define MFLAG_M 0x4 //������ ���������
#define MFLAG_SLOW 0x8 // ��������� ����� ���������
#define MFLAG_I_OK 0x10 // ��������� ����� ���������
#define MFLAG_V_OK 0x20 // ��������� ����� ���������
static uint8_t MFlags;  // ����� ���������

static int8_t CheckDiapason(void); // ������� �������� ���������. ���������� 0 � ������ ����������� ������. ����� ���������� ����� ��� ������������� ������������� ���������.
void SetDiapason(void) // ������� ��������� ��������� ���� �� ��������� ���������. ������� ���������� � 0 ��� ��������� ���������. ����� ��������� ��� ����� ������������ ��������
{
  uint8_t Switches;
  if ( MFlags & MFLAG_V ) // ����������
    Switches = Diapasons[CurrentDiapason].VV;
  else
    Switches = Diapasons[CurrentDiapason].II;
  SetSwitches( Switches ); 
  ResetZm();
}

// ������� �������� �������� ���������, �������� 6 �������� ���������� � ���� ����� �� ���� �� ����� � ���������
// ��������� �  ZmSum, ����� ����������� �������� �������� � ������ ������ SqZmSumReal  SqZmSumImag
// ������� ���������� �������� ��� ���������� ���������� ��������� ���������� ��� ����
// ������� ���������� 0, ���� ���������� ������� ������������ �������������
// ����� ������������ ��a����� !=0
uint8_t CalculationZm(void)
{
  int8_t DeltaDiapason;
  int8_t NewDiapason = CurrentDiapason;

  DeltaDiapason = CheckDiapason(); // �������� ���������
#if 0 // ���������� ����� ���������� 
  {
  char Buf[5];
  itoa(Buf, DeltaDiapason);
  LcdChr(X_POSITION*8+Y_POSITION*7+4, Buf);
  }
#endif

  if ( ( FixDiapason == 0) && // �������������� ����� ��������� ��������
       ( DeltaDiapason != 0) ) // ���� ��������� ����� ��������� 
  {
     NewDiapason = CurrentDiapason + DeltaDiapason; // ����� �������� ������������ ��������

    if ( NewDiapason < 0 ) // ��������� �� ������������ �������� �������� �� ����������
      NewDiapason = 0;
    if ( NewDiapason > 16) // ��������� �������������� �������� �� ����
      NewDiapason = 16;

#if !defined(TEST_ADC) && !defined(AAA)
    while ( Calibration.InvalidFlag == 0 && 
         *((uint32_t*)&Calibration.Data.Z.stdm[NewDiapason + 17 * CurrentFreq]) == 0xFFFFFFFF )
    {// �������� ��� ����������� ���������� �������� �� ������� ����������
      if ( NewDiapason > 5 )
        NewDiapason--;
      else
        NewDiapason++;
    }
#endif
  }

  if ( NewDiapason != CurrentDiapason) // ��������� ����� ���������.
  {
    ResetZm(); // ���������� ��������� ���������������
    CurrentDiapason = NewDiapason; // ������ ��������
    if (MFlags & MFLAG_SLOW) // ��������� ��������� ��� ���� �������� �� �� �������� �������� �������.
    {
      Step = 6;
      MFlags &= MFLAG_V;
      return 6;
    }
    if (MFlags & MFLAG_V) // ��������� ����������
    {
      SetSwitches(Diapasons[NewDiapason].VV); // ������������ �������� ����������
    }
    else
    {
      SetSwitches(Diapasons[NewDiapason].II); // �������� �������� ��������
    }
    AdcDelay(1); // ����������� �������� � SetSwitches - 2 ������� ������ - ����� �� ����� ��������
    return 1;// ���� �������� ��������. ��������� �� �����
  }
  else // �������� ������ �� �����
  {
    if ( MFlags & (MFLAG_I_OK|MFLAG_V_OK) ) // ���� ��� ��������������
    {
      // ������������� �� ������ ��������
      if ( MFlags & MFLAG_V ) // ���� ��������� ����������
      {
        SetSwitches(Diapasons[CurrentDiapason].II); // �������� ���
        MFlags ^= (MFLAG_V|MFLAG_V_OK); // ���� �������� ������ ����������
      }
      else
      {
        SetSwitches(Diapasons[CurrentDiapason].VV); // �������� ����������
        MFlags |= (MFLAG_V|MFLAG_I_OK);
      }
      AdcDelay(1); // ����������� �������� � SetSwitches - 2 ������� ������ - ����� �� ����� ��������
      return 2; // ����������� � ���������� �� ��� ��� ��������
    }
    //�������� �������� 
    if ( (MFlags & MFLAG_M) == 0)
    {
      // �������������� �������� ����� ������� ���������
      AdcDelay(2);
      MFlags |= MFLAG_M;
      return 3; // �������� ����� ����������
    }
    // ���������� ��������
  }
  
  { // �������� ������ � ������
    Complex_t* Item;
    if ( MFlags & MFLAG_V ) // ����������
    {
      Item = &VArray[AdcEvCounter];
      Item->Re =  -ResultCosSum;
      Item->Im = ResultSinSum; // ��������� ���������
    }
    else
    {
      Item = &IArray[AdcEvCounter];
    
      Item->Re =  ResultCosSum;
      Item->Im = -ResultSinSum;
    }
  }

  if (AdcEvCounter == (ZARRAYSIZE-1)) // ������ ��������
  {
    if ( MFlags & MFLAG_NO_SWITCH ) // �� ����� �������������. ����� �������� ���������
    {
      goto GetResult; // ��� ������� ���������. ���������� ���������
    }
    
    // ������������� �� ������ �������� 
    if ( MFlags & MFLAG_V ) // ����������
    {
      SetSwitches(Diapasons[CurrentDiapason].II); // ������������� �� ��������� ����
      MFlags &= ~MFLAG_V;
    }
    else
    {
      SetSwitches(Diapasons[CurrentDiapason].VV); // ������������� �� ��������� ����������
      MFlags |= MFLAG_V;
    }
    AdcEvCounter = 0;
    MFlags |= MFLAG_NO_SWITCH;
    return 4; // ��������� ��������� ����������  ��� ����
  }
  AdcEvCounter++; // ��������� ��� �������� ��������� ������� �������
  return 5;
      
GetResult:
  { // ��������� ��������� ���� ��� ����������. ��� ������ �������
    uint8_t j; 

    AdcEvCounter = 0;
    MFlags &= ~MFLAG_NO_SWITCH;

    for (j=0; j<ZARRAYSIZE; j++) // ������ �������� ���� � ������������� ����������
    {
      Complex_t In;

      if (IArray[j].Re == 0.0f && IArray[j].Im == 0.0f) // �� 0 ������ ������
        continue;
      In = ComplexDiv(VArray[j],IArray[j]);

      ZmSum.Re = ZmSum.Re +  In.Re; // ����� ������������� ������� ��������
      ZmSum.Im = ZmSum.Im +  In.Im;
#if defined(MSE)
      {
      float Tmp;
      Tmp = In.Re;
      SqZmSumReal += Tmp*Tmp; // ����� ��� �������� ����������
      Tmp = In.Im;
      SqZmSumImag += Tmp*Tmp; // ����� ��� �������� ����������
      }
#endif
      ZmMeasCount += 1; // ���������� ����������� ����� ������������� �� 1
    }
  }
  MFlags |= MFLAG_SLOW;
  return 0; // ���������� ����� �������� 
}

int8_t CheckDiapason(void)
{
  int8_t Ret = 0;
  
  if ( Step == 0 )
    Step++;
  if (MFlags & MFLAG_SLOW)
    Step = 1;

  // ��� ����� ���� �������� �� 0 ��� �� �� ����������� ���������������� � �� �����������
  if ( ResultMin < 200 - 2048 || ResultMax > 3800 - 2048) // ���������� ��� 
  {
    Ret = Step; // ��� ���������� ����� ������ �������� �����������
    OverloadFlag = 15; // ������ ���� ��������� 12 ������. ������������ �������� ��� ����������
  }
  else 
  {
    uint16_t Delta =  ResultMax - ResultMin;

    if ( Delta < 900 ) // ���������� ���
    {
        if (Delta < 20 )
          Ret = -Step;
        else if ( Delta < 90)
          Ret = -3;
        else if (Delta < 250 )
          Ret = -2;
        else  
          Ret = -1;
    }
    if ( OverloadFlag > 0 ) // ��������� ���������� ��������� - ����� �������� ���� ����������
      OverloadFlag--;
  }
  if ( Step > 1  )
    Step--;

  if ((MFlags & MFLAG_V) == 0)  // ��� ������� �������� � ������ �������
	{
    Ret = Ret * -1;
  }
  return Ret;
}

void ResetZm(void)
{
  ZmSum.Re = 0;
  ZmSum.Im = 0;
#if defined(MSE)
  SqZmSumReal = 0;
  SqZmSumImag = 0;
#endif
  ZmMeasCount = 0;
  AdcEvCounter = 0;
}


#if defined(TEST_CALCZ) && defined(TEST_ADC)
#include "n1202.h"
#include "out_float.h"
#include "itoa.h"

#if  defined(AUTO_RESET)
float PrevImMse;
float PrevReMse;
#endif

void CalculationZ(void)
{
  int8_t rr;

  if ( Event == EV_FUNC_FIRST )
  {
    ResetZm();
    LcdClear();
		FixDiapason = 0;
    return;
  }
  if ((Event & EV_MASK) == EV_KEY_PRESSED) 
  {
    switch (Event & KEY_MASK)
    {
      case KEY_ADC:
        goto redraw;
      case KEY_UP:
        if ( CurrentDiapason < 16 )
        {
					FixDiapason = 1;
          CurrentDiapason++;
          ResetZm();
          LcdClear();
        }
        return;
      case KEY_DOWN:
        if ( CurrentDiapason > 0 )
        {
					FixDiapason = 1;
          CurrentDiapason--;
          ResetZm();
          LcdClear();
        }
        return;
          
      default:
        CurrentFunc(TestAdcMenu);
        return;
    }
  }
  return;

redraw:
  rr = CalculationZm();
  if ( rr == 0)
  {
    float Av;
#if defined(MSE)
    float mse;
#if  defined(AUTO_RESET)
    uint8_t ResetFlag = 0;
#endif
#endif

    Av = ZmSum.Re/ZmMeasCount; // ������ �������� �������� �����
    OutFloat(X_POSITION*0+Y_POSITION*2+MUL2, Av , ' '); // ����� �������� �����
#if defined(MSE)
    mse = sqrtf((SqZmSumReal/ZmMeasCount - Av*Av)/ZmMeasCount/(ZmMeasCount - 1)); // ������ ���������� �������� �����
    Av = mse/Av*1000; // ���������� � ���������
    if ( Av < 0)
      Av = Av * -1;
    OutFloat(X_POSITION*5+Y_POSITION*4, Av , '%'); // ����� �������������� ���������� �������� ��������
#if  defined(AUTO_RESET)
    if (ZmMeasCount > 12 && Av > 1e-2f )
    {
      if (Av > PrevReMse ) // ������ �����������
        ResetFlag++; // ������ ���� ������
      PrevReMse = Av; // ��������� ���������� ����������
    }
#endif
#endif

    Av = ZmSum.Im/ZmMeasCount;  // ������ ������� ��������
    OutFloat(X_POSITION*0+Y_POSITION*5+MUL2, Av , ' '); // ����� ������ �����
#if defined(MSE)
    mse = sqrtf((SqZmSumImag/ZmMeasCount - Av*Av)/ZmMeasCount/(ZmMeasCount - 1)); // ������ ���������� ������ �����
    Av = mse/Av*1000; // ���������� � ���������
    if ( Av < 0)
      Av = Av * -1;
    OutFloat(X_POSITION*5+Y_POSITION*7, Av , '%'); // ����� ������� ������� ������ �����
#if  defined(AUTO_RESET)
    if (ZmMeasCount > 12 && Av > 1e-2f )
    {
      if (Av > PrevImMse ) // ������ �����������
        ResetFlag++; // ������ ���� ������
      PrevImMse = Av; // ��������� ���������� ����������
    }
    if ( ResetFlag )
    {
      ResetZm();
      PrevReMse = PrevImMse = 10000.0f;
    }
#endif
#endif
  }

  {
  char Buf[4];

  ShowDiapason(); //�������� ���������
  ShowFrenq();
  ShowOverload(); // ����������

  itoa(Buf, rr); // �� ��� �������  CalculationZm 
  LcdChr(X_POSITION*0+Y_POSITION*1+4, Buf);
 
  itoa(Buf, ZmMeasCount); // ���������� ���������
  LcdChr(X_POSITION*12+Y_POSITION*1+4, Buf);
  }
}
#endif
