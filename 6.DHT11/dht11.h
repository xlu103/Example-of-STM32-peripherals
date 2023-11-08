#ifndef __DHT11_H
#define __DHT11_H 

#include "sys.h"   

//IO方向设置
#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;}

////IO操作函数											   
#define	DHT11_DQ_OUT PAout(11) //数据端口	PA11 
#define	DHT11_DQ_IN  PAin(11)  //数据端口	PA11

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(void);//读取温湿度

#endif


