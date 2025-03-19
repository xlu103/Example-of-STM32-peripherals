#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "HC-SR04.h"


int main(void)
{
	//OLED_Init();
	Timer_Init();	
	HC_SR04_Init();
	//OLED_ShowString(1, 1, "D=");
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	while (1)
	{
		HC_SR04_Compute();
	}
}
