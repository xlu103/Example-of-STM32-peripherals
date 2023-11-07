#include "sys.h"
#include "key.h"	//GPIO初始化函数  
#include "delay.h"
int f;
int KeyValue;  
void KEY(void)  
{  
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟            
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  //初始化PD0-PD3  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;           //设置为推挽输出  
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                            
 GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;    //初始化PD4-PD7   
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	   
 GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
 GPIO_SetBits( GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);   //1111  
 GPIO_ResetBits( GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //0000
}  
//按键函数  
extern void Keyspress()  
{  


   if((GPIO_ReadInputData(GPIOA)&0xff)!=0x0f)   
{   
   delay_ms(20);  
 if((GPIO_ReadInputData(GPIOA)&0xff)!=0x0f)   
{   
   GPIO_SetBits(GPIOA,GPIO_Pin_0);   
  
   GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);  
  
   switch(GPIO_ReadInputData(GPIOA)&0xff)  
              {   
                case 0x11:KeyValue=12;break;   
   
                case 0x21:KeyValue=13;break;   
   
                case 0x41:KeyValue=14;break;   
   
                case 0x81:KeyValue=15;break;  
               }   
                GPIO_SetBits(GPIOA,GPIO_Pin_1);  
   
                GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3);  
   
                 switch(GPIO_ReadInputData(GPIOA)&0xff)  
               {   
                 case 0x12:KeyValue=8;break;  
   
                 case 0x22:KeyValue=9;break;   
   
   
                 case 0x42:KeyValue=10;break;  
   
                 case 0x82:KeyValue=11;break;   
   
                }   
                 GPIO_SetBits(GPIOA,GPIO_Pin_2);   
  
  
                 GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3);  
  
  
                  switch(GPIO_ReadInputData(GPIOA)&0xff)  
                   {   
                    case 0x14:KeyValue=4;break;  
   
                    case 0x24:KeyValue=5;break;  
   
                    case 0x44:KeyValue=6;break;  
   
                    case 0x84:KeyValue=7;break;  
                   }   
                    GPIO_SetBits(GPIOA,GPIO_Pin_3);   
   
                    GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);  
                                                
                   switch(GPIO_ReadInputData(GPIOA)&0xff)  
                    {  
                     case 0x18:KeyValue=0;break;  
  
                     case 0x28:KeyValue=1;break;   
  
                     case 0x48:KeyValue=2;break;  
  
                     case 0x88:KeyValue=3;break;  
                     }   
                   GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3);   
                   GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7);  
                   while((GPIO_ReadInputData(GPIOA)&0xff)!=0x0f);  
                   switch(KeyValue)   
                     {     
												case 1:f=0;break;  
                        case 12:f=1;break;//由于项目只用到12个按键，所以有4个按键的扫描程序是没有的  		
												case 13:f=2;break;  											 
                        case 14:f=3;break;  
											  case 8:f=4;break;  
											  case 9:f=5; break;  
                        case 10:f=6; break;        											 
                        case 4:f=7;break;  
                        case 5:f=8;break;  
											  case 6:f=9;break;  
    
                        default : f=15;
                    }  
  
            }    
      }  
  }   
 