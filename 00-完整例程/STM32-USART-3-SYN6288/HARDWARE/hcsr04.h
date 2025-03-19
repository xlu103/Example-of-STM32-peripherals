#ifndef __HCSR04_H
#define __HCSR04_H	 
//超声波硬件接口定义
#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_11
#define HCSR04_ECHO     GPIO_Pin_10

#define TRIG_Send  PBout(11)
#define ECHO_Reci  PBin(10)

void Hcsr04Init();
float  Hcsr04GetLength();


/*
1. 调用  void Hcsr04Init(); 初始化
2. 调用 float  Hcsr04GetLength(); 获取值
*/ 
#endif