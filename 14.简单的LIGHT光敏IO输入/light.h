#ifndef __LIGHT_H
#define __LIGHT_H	 
#include "sys.h"
  

 
#define LIGHT  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键0
  

 
void LIGHT_Init(void);//IO初始化
  

#endif
