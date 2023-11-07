#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define 	NET_LED		 PAout(5)	//网络指示灯 - PA5
#define 	BEEP 		 PAout(6)	//蜂鸣器报警 - PA6

void LED_Init(void);//初始化
		 				    
#endif
