#ifndef __HCSR04_H
#define __HCSR04_H	 
//������Ӳ���ӿڶ���
#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_11
#define HCSR04_ECHO     GPIO_Pin_10

#define TRIG_Send  PBout(11)
#define ECHO_Reci  PBin(10)

void Hcsr04Init();
float  Hcsr04GetLength();


/*
1. ����  void Hcsr04Init(); ��ʼ��
2. ���� float  Hcsr04GetLength(); ��ȡֵ
*/ 
#endif