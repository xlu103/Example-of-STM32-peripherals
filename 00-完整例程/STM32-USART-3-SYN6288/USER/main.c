#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "usart3c.h"
#include "syn6288.h"

extern u16 USART_RX_STA;
extern u16 usart1_len;
extern u8 isSuccess;
extern u8 USART_RX_BUF[];
extern u16 usart2_len;
extern u8 USART2_RX_BUF[]; 



/**************芯片设置命令*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令

int main(void)
{

		
		float len=0;
	
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
    mqttusartinit(9600);
    uart_init(9600);
    usart3init(9600);
    delay_init();
	
	SYN_FrameInfo(2, "[v7][m1][t5]欢迎使用绿深旗舰店SYN");
		delay_ms(1500);
		delay_ms(1500);
		delay_ms(1500);
		delay_ms(1500);
		delay_ms(1500);
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


