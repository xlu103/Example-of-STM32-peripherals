#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
 

#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡ����0
 

#define KEY0_PRES 	1	//KEY0����
 

 
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void);  	//����ɨ�躯��		


#endif
