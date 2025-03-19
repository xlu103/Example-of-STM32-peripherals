#ifndef __MQTT_USART_H
#define __MQTT_USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 usart2_len;         		//接收状态标记	
int uart2printf(int ch, FILE *f);
void mqttusartinit(int bound);
int32_t usart2_send(USART_TypeDef* USARTx,uint8_t * buf, uint16_t len);
#endif


