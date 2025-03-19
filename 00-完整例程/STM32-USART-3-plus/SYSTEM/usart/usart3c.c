#include "usart3c.h"
#include "delay.h"
#include "usart.h"
//字符串定义
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 screen_len=0;       //接收状态标记	

//串口初始化函数
void usart3init(int bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3
  
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
  //USART3_RX	  GPIOB.11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}



//串口3中断函数
void USART3_IRQHandler(void)                	//串口3中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
		{
					//while((USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET));//等待接收完成
					Res =USART_ReceiveData(USART3);	//读取接收到的数据
					USART3_RX_BUF[screen_len]=Res;
					screen_len=screen_len+1;
					if(screen_len>(USART3_REC_LEN-1))screen_len=0;//接收数据错误,重新开始接收
    }
		USART_ClearFlag(USART3, USART_IT_RXNE);		
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
//串口发送函数
int32_t screen_send(USART_TypeDef* USARTx , uint8_t * buf, uint16_t len)
{
		uint16_t i;
    for(i=0;i<len;i++){
			USART_SendData(USARTx,buf[i]);	
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );//等待发送结束			
		}
    return i;
}

