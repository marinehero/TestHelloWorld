#include "../inc/typedef.h"


#ifndef __TP5602_H__
#define __TP5602_H__

//��ص�ѹ 0.01V
u16 GetBattVoltage(void);

//��ص��� 0-100
u8 GetBattCapacity(void);

//������ƿ��ѹ  0.1V
u8 GetInputVoltage(void);

//�Ƿ��������ⲿ��Դ
bool IsConnectedInputPower(void);

//��ѹ����Ƿ����ڹ���
bool IsBoostOut(void);

//��TP5602����PIN ���һ��������
//10~20 ms
void Tp5602KeyPress(void);

void Tp5602Funtion(void);
void TP5602InitPoweron(void);

#endif //__TP5602_H__
//==========================================================
//end files
