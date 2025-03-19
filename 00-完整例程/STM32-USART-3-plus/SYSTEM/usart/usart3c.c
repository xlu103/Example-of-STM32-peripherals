#include "usart3c.h"
#include "delay.h"
#include "usart.h"
//�ַ�������
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 screen_len=0;       //����״̬���	

//���ڳ�ʼ������
void usart3init(int bound)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3
  
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
  //USART3_RX	  GPIOB.11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11  

  //Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 
}



//����3�жϺ���
void USART3_IRQHandler(void)                	//����3�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
		{
					//while((USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET));//�ȴ��������
					Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
					USART3_RX_BUF[screen_len]=Res;
					screen_len=screen_len+1;
					if(screen_len>(USART3_REC_LEN-1))screen_len=0;//�������ݴ���,���¿�ʼ����
    }
		USART_ClearFlag(USART3, USART_IT_RXNE);		
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}
//���ڷ��ͺ���
int32_t screen_send(USART_TypeDef* USARTx , uint8_t * buf, uint16_t len)
{
		uint16_t i;
    for(i=0;i<len;i++){
			USART_SendData(USARTx,buf[i]);	
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );//�ȴ����ͽ���			
		}
    return i;
}

