#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
 

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//KEY0-KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 KEY_Scan()
{    
    static u8 key_up = 1;  //�������ɿ���־
    
    if(key_up && KEY0 == 0)
    {
        delay_ms(10);  //ȥ����ʱ������
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
    return 0;  //�ް�������
}
 
