#include "../inc/typedef.h"


#ifndef __KEY_H__
#define __KEY_H__


typedef enum
{
	KEY_VALUE_NOKEY  = 0x01,
	KEY_VALUE_ON_OFF = 0x00
}enumKeyValue;

typedef enum
{
	KEY_STATUS_NOPRESS = 0,
	KEY_STATUS_PRESS,     //����
	KEY_STATUS_DOUBLE,    //˫��
	KEY_STATUS_KEEP,      //����
	KEY_STATUS_KEEPED     //��������Ӧ,�����ظ���Ӧ
}enumKeyStatus;

//���ط�0��ʾ�����Ѿ���Ӧ��,���Խ�����һ��״̬
typedef char (KEY_CALLBACK)(enumKeyValue value, enumKeyStatus status, u8 second);

#define GetCurKeyValue() keyValue

extern  enumKeyValue keyValue;       //��ǰ��ֵ
extern  u8 _hc165_map;

void KeyFuntion(void);

void SetKeyCallback(KEY_CALLBACK * pCallback);
void KeyInit(void);



#endif

//==========================================================

//end files
