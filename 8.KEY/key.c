#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
 

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//KEY0-KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 KEY_Scan()
{    
    static u8 key_up = 1;  //按键按松开标志
    
    if(key_up && KEY0 == 0)
    {
        delay_ms(10);  //去抖动时间缩短
        if(KEY0 == 0)
        {
            key_up = 0;
            return KEY0_PRES;
        }
    }
    else if(KEY0 == 1)
    {
        key_up = 1;
    }
    return 0;  //无按键按下
}
 
