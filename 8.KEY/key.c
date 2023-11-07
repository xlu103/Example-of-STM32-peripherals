#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
			
//������������ʼ������
void Voice_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 KEY_Scan()
{	
	if(KEY0 == 0)
	{
		delay_ms(20);//ȥ����
		if(KEY0 == 0)
		{
			while(KEY0 == 0);
			return KEY0_PRES;
		}
	}
	if(KEY1 == 0)
	{
		delay_ms(20);//ȥ����
		if(KEY1 == 0)
		{
			while(KEY1 == 0);
			return KEY1_PRES;
		}
	}
	if(KEY2 == 0)
	{
		delay_ms(20);//ȥ����
		if(KEY2 == 0)
		{
			while(KEY2 == 0);
			return KEY2_PRES;
		}
	}
	if(KEY3 == 0)
	{
		delay_ms(20);//ȥ����
		if(KEY3 == 0)
		{
			while(KEY3 == 0);
			return KEY3_PRES;
		}
	}
	
 	return 0;// �ް�������
}

u8 Choose_KEY()
{	
	while(1)
	{
		if(KEY0 == 0)
		{
			delay_ms(20);//ȥ����
			if(KEY0 == 0)
			{
				while(KEY0 == 0);
				return KEY0_PRES;
			}
		}
		if(KEY1 == 0)
		{
			delay_ms(20);//ȥ����
			if(KEY1 == 0)
			{
				while(KEY1 == 0);
				return KEY1_PRES;
			}
		}
		if(KEY2 == 0)
		{
			delay_ms(20);//ȥ����
			if(KEY2 == 0)
			{
				while(KEY2 == 0);
				return KEY2_PRES;
			}
		}
		if(KEY3 == 0)
		{
			delay_ms(20);//ȥ����
			if(KEY3 == 0)
			{
				while(KEY3 == 0);
				return KEY3_PRES;
			}
		}
	}
}
