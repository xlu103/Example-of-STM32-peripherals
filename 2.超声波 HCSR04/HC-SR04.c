#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Delay.h"


uint32_t Distance=0;

void HC_SR04_Init(void)
{
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;                    //PB13，触发信号引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //外部中断引脚，Echo，接收信号PB14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿和下降沿都触发，上升沿开始计数，下降沿停止计数
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line14);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

}

uint32_t HC_SR04_Compute(void)
{
	uint32_t Average_Distance=0;
	for(uint8_t i=0;i<8;i++)
	{
		
		GPIO_SetBits(GPIOB, GPIO_Pin_13);                 //启动信号
		delay_us(20);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		delay_us(65);                                    //两次启动之间时间间隔       
		Average_Distance=Average_Distance+Distance;
	}
	Average_Distance=Average_Distance/8;               //8次平均值，数据更稳定
	
return Average_Distance;
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 1)       //判断上升沿
		{
			num=0;
			TIM_SetCounter(TIM2,0);                                
			TIM_Cmd(TIM2, ENABLE);                                  //启动定时器计时
		}
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)  //判断下降沿
		{
			TIM_Cmd(TIM2, DISABLE);                                //关闭定时器定时
			Distance=(num*1000+TIM_GetCounter(TIM2))*0.34/2;        //计算距离单位mm  340m/s=0.34mm/us   进入中断的次数加上计数器上已经计的次数=总时间
		}
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
