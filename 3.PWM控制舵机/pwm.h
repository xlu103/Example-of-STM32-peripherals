#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

 
 
void TIM3_PWM_Init(u16 arr,u16 psc);


/** 
 	TIM3_PWM_Init(1999,719);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	 
   	while(1)
	{
 		delay_ms(10);	 
		while(1){	
		TIM_SetCompare2(TIM3,1950); //��Ӧ0��	    2.5%
  	  	delay_ms(1000);	
		TIM_SetCompare2(TIM3, 1900); //��Ӧ45��		5%	
    	delay_ms(1000);
		TIM_SetCompare2(TIM3, 1850); //��Ӧ90��		7.5%
   		delay_ms(1000);
		TIM_SetCompare2(TIM3, 1800); //��Ӧ135��	10%
		delay_ms(1000);
		TIM_SetCompare2(TIM3,1750); //��Ӧ180��		12.5%
		delay_ms(1000);
	}	 
	
	ע�⣺��iicһ��ʹ�� ��Ҫ�������ò����ʣ��Ƕȣ�֮ǰ�ر�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,DISABLE);
	
	���ٿ�������֤iic����ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	ע�⣺������Ƕ�֮����Ҫ��ʱ


*/
#endif
