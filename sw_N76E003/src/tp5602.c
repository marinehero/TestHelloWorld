#include "../inc/tp5602.h"
#include "../inc/adc.h"
#include "../inc/main.h"


static u16 curBattVoltage;
static u8 curBattCapacity;
static u8 curInputVoltage;
static u8 second_count;
static u8 mathBattCapacityDlySecond;  //���������ʱ������
static bool tp5602_key_press_flag;
static bool isConnectInputPower_Old;  //������һ��������ڵ��ⲿ��Դ״̬
//==========================================================
//��ص�ѹ  0.01V
// u16 GetBattVoltage(void)
// {
// 	return (curBattVoltage);
// }

//��ص��� 0-100
u8 GetBattCapacity(void)
{
	return (curBattCapacity);
}

//������ƿ��ѹ  0.1V
u8 GetInputVoltage(void)
{
	return (curInputVoltage);
}

bool IsConnectedInputPower(void)
{
	return ((curInputVoltage > 90) ? true : false);
}

bool IsBoostOut(void)
{
	return ((P17) ? true : false);
}
//==========================================================
//==========================================================
void Tp5602KeyPress(void)
{
	if(curBattVoltage<300) return;
	P30=1;
	tp5602_key_press_flag = false;
}
//==========================================================
//�����ص�ǰʣ�����
code u16 voltageadd[]=
{
	420,//100
	415,//90
	410,//80
	400,//70
	395,//60
	385,//50
	375,//40 
	365,//30
	355,//20   
	345,//10   
	330 //0    
};

code u16 voltagedec[]=
{
	400,//100
	390,//90	
	380,//80	
	370,//70	
	360,//60
	350,//50
	340,//40
	330,//30
	320,//20
	310,//10
	300//0
};
static void CalculationCurBattCapacity(void)
{ 
	u8 i, tmp;
	u16 voltage;
	static u8 over_count = 0;
	
	if(mathBattCapacityDlySecond)
	{
		mathBattCapacityDlySecond --;
		return;
	}
	
	voltage = curBattVoltage;
	if(IsConnectedInputPower())
	{//���
		for(i = 0; i < 10; i ++)
		{
			if(voltage > voltageadd[i])
			{
				break;
			}
		}
		tmp = 100 - ((i > 10) ? 10 : i) * 10;
		if(tmp > curBattCapacity) 
		{
			if(++over_count > 5)
			{//����5Sȷ���Ƿ���߽�ֵ
				over_count = 0;
				curBattCapacity = tmp;
			}
		}
		else
		{
			over_count = 0;
		}
	}
	else
	{//�ŵ�
		for(i = 0; i < 10; i ++)
		{
			if(voltage > voltagedec[i])
			{
				break;
			}
		}
		tmp = 100 - ((i > 10) ? 10 : i) * 10;
		if(tmp < curBattCapacity) 
		{
			if((++over_count) > ((tmp == 0) ? 0 : 5))
			{//����5Sȷ���Ƿ���߽�ֵ
				over_count = 0;
				curBattCapacity = tmp;
			}
		}
		else
		{
			over_count = 0;
		}
	}
}
//==========================================================
//����ѭ����ÿ 10ms ����һ��
void Tp5602Funtion(void)
{
	if(P30)
	{
		if(false == tp5602_key_press_flag)
		{
			tp5602_key_press_flag = true;
		}
		else
		{
			P30=0;
		}
	}
	
	curInputVoltage = (u16)(((u32)GetAdc(ADC_CH_INPUT)*72)/GetAdc(ADC_CH_REF))+8;
	if(isConnectInputPower_Old != IsConnectedInputPower())
	{
		isConnectInputPower_Old = IsConnectedInputPower();
		mathBattCapacityDlySecond = 30;   //���ŵ�״̬�л���30S��,�����µ���״̬
	}
		
	if(++ second_count > 100)
	{
		second_count = 0;
		curBattVoltage = (u16)(((u32)GetAdc(ADC_CH_BATT)*122)/GetAdc(ADC_CH_REF));
		CalculationCurBattCapacity();
	}
}
//==========================================================
void TP5602InitPoweron(void)
{
	clr_P3M1_0;set_P3M2_0;
	clr_P30;
	set_P1M1_7;clr_P1M2_7;

	
	curBattVoltage = 0;
	curBattCapacity = 0;
	curInputVoltage = 0;
	second_count = 0;
	mathBattCapacityDlySecond = 0;
	isConnectInputPower_Old = false;
}
//==========================================================
//end files
