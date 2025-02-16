#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
 

#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键0
 

#define KEY0_PRES 	1	//KEY0按下
 

 
void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);  	//按键扫描函数		


#endif
