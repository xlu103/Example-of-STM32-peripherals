#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"


//��ʼ����������
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	Adc1_Init();
}

//��ȡ���������ĵ�ѹֵ
u16 Smog_Get_Vol(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)
	{
		temp_val+=Get_Adc1(SMOG_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//�õ�ƽ��ֵ
//	printf("Smog_ADC_Val:%d\r\n", temp_val);
    return (u16)temp_val;
}


u16 Smog_Trans_Concentration(void)
{
    u16 ulVal = 0;
    u16 temp_val = Smog_Get_Vol();
	u16 Rs;
//	Rs = SMOG_PIN46_R*(4096.0/temp_val - 1);
	Rs=(temp_val*100/4096);
	// printf("Smog_Rs_Val:%d\r\n", Rs);
	
	ulVal = Rs;//������д��ʽ����Rs��������Ũ��
	//OLED_ShowNum(48,0,ulVal,2,16);
    return ulVal;
}

