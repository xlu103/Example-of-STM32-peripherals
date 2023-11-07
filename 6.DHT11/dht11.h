#ifndef __DHT11_H
#define __DHT11_H 

#include "sys.h"   

//IO��������
#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;}

////IO��������											   
#define	DHT11_DQ_OUT PAout(11) //���ݶ˿�	PA11 
#define	DHT11_DQ_IN  PAin(11)  //���ݶ˿�	PA11

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(void);//��ȡ��ʪ��

#endif


