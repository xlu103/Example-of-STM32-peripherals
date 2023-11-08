#ifndef __RED_H
#define __RED_H	 
#include "sys.h"
  

 
#define RED  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键0
  

 
void RED_Init(void);//IO初始化
  

#endif
