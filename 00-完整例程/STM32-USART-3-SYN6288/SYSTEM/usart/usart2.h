#ifndef __MQTT_USART_H
#define __MQTT_USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 usart2_len;         		//����״̬���	
int uart2printf(int ch, FILE *f);
void mqttusartinit(int bound);
int32_t usart2_send(USART_TypeDef* USARTx,uint8_t * buf, uint16_t len);
#endif


