#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

 
 
void TIM3_PWM_Init(u16 arr,u16 psc);


/** 
 	TIM3_PWM_Init(1999,719);	 //不分频。PWM频率=72000000/900=80Khz
	 
   	while(1)
	{
 		delay_ms(10);	 
		while(1){	
		TIM_SetCompare2(TIM3,1950); //对应0度	    2.5%
  	  	delay_ms(1000);	
		TIM_SetCompare2(TIM3, 1900); //对应45度		5%	
    	delay_ms(1000);
		TIM_SetCompare2(TIM3, 1850); //对应90度		7.5%
   		delay_ms(1000);
		TIM_SetCompare2(TIM3, 1800); //对应135度	10%
		delay_ms(1000);
		TIM_SetCompare2(TIM3,1750); //对应180度		12.5%
		delay_ms(1000);
	}	 
	
	注意：与iic一起使用 需要对在设置波特率（角度）之前关闭
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,DISABLE);
	
	后再开启，保证iic正常使用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	注意：设置完角度之后，需要延时


*/
#endif
