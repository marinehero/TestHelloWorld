#include "../inc/key.h"
#include "../inc/delay.h"

#define DOUBLE_KEY_TIME        30         //˫����Ч�ж�ʱ��

static  u8 keyShakeTime;             //����ʱ��
static  u8 keyRepeatTime;            //����ʱ��
static  u8 doubleTime;               //˫���ж�ʱ��
static  u8 keyTimeSecond;            //�������µ�ʱ��(S)
static  enumKeyStatus keyStatus;     //����״̬
static  KEY_CALLBACK * pKeyCallback; //�����ص�

enumKeyValue keyValue;              //��ǰ��ֵ
static  enumKeyValue keyValueDouble; //˫�����
//==========================================================
static void KeyScan(void)
{
	u8 tmp;
	
	if(doubleTime) doubleTime --;
	else
	{
		keyValueDouble = KEY_VALUE_NOKEY;
	}
	
	tmp = P14;
	if(KEY_VALUE_NOKEY != tmp)
	{
		if(0 == keyShakeTime)
		{
			keyShakeTime = 0x80 + 2;  //20ms
		}
		else
		if(0x80 == keyShakeTime)
		{
			if(keyValue != tmp)
			{
				keyValue = tmp;
				if((keyValueDouble != keyValue) || (doubleTime == 0))
				{
					keyValueDouble = keyValue;
					keyStatus = KEY_STATUS_PRESS;
					doubleTime = DOUBLE_KEY_TIME; 
				}
				else
				{
					keyStatus = KEY_STATUS_DOUBLE;
					keyValueDouble = KEY_VALUE_NOKEY;
				}
				
				keyRepeatTime = 5;
				keyTimeSecond = 0;
			}
		}
	}
	else
	{
		if(KEY_VALUE_NOKEY != keyValue)
		{
			keyShakeTime = 0;
			keyRepeatTime = 0;
			keyValue = KEY_VALUE_NOKEY;
			keyStatus = KEY_STATUS_NOPRESS;
		}
	}
}

void KeyFuntion(void)
{
	if(keyShakeTime & 0x7f)   keyShakeTime --;
	if(keyRepeatTime & 0x7f)  keyRepeatTime --;
	
	KeyScan();
	if(KEY_VALUE_NOKEY == keyValue) return;
	if(keyRepeatTime & 0x7f) return;
				
	if(0 == keyRepeatTime)
	{//step
		keyRepeatTime = 0x80 + 100;  //1S
		if(pKeyCallback) 
		{
			pKeyCallback(keyValue, keyStatus, 0);
		}
		
		keyStatus = KEY_STATUS_KEEP;
		keyTimeSecond = 3;
	}
	else
	{//link
		keyRepeatTime = 0x80 + 25;  //0.25S
	  if(keyTimeSecond < 0xff) keyTimeSecond ++;
		
		if(pKeyCallback) 
		{
			if( pKeyCallback(keyValue, keyStatus, keyTimeSecond >> 2))
			{
				keyStatus = KEY_STATUS_KEEPED;
			}
		}
	}
}

void SetKeyCallback(KEY_CALLBACK * pCallback)
{
	pKeyCallback = pCallback;
}

void KeyInit(void)
{
	set_P1M1_4;clr_P1M2_4;
	
	keyShakeTime = 0;
	keyRepeatTime = 0;
	doubleTime = 0;
	keyTimeSecond = 0;
	keyStatus = KEY_STATUS_NOPRESS;
	keyValue = KEY_VALUE_NOKEY;
	pKeyCallback = NULL;
}
//==========================================================

//end files

