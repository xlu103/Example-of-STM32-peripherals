#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "usart3c.h"

extern u16 USART_RX_STA;
extern u16 usart1_len;
extern u8 isSuccess;
extern u8 USART_RX_BUF[];
extern u16 usart2_len;
extern u8 USART2_RX_BUF[]; 
int main(void)
{

		
		float len=0;
	
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
    mqttusartinit(9600);
    uart_init(9600);
    usart3init(9600);
    delay_init();
	while(1){
		
		if(isSuccess){
			// 在这里处理串口一接受的数据

			// USART_RX_BUF 变量存储的就是串口1 的数据

			isSuccess=0;
			printf("send:%s\r\n",USART_RX_BUF);
			usart2_send(USART2,USART_RX_BUF,usart1_len+1);
			USART_RX_STA=0;
		}
		if(usart2_len>0){
			//usart2_len=0;
			//printf("receive:%s\r\n",USART2_RX_BUF);
			
		}
	}
	 
	 
}


