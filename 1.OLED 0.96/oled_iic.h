#ifndef __OLED_IIC_H
#define __OLED_IIC_H 
#include "sys.h"

 


#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


 

/*

STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA 
    
1. 调用 I2C_Configuration(); OLED_Init(); 初始化
2. OLED_CLS();//清屏
3. OLED_ShowCN(0, 0, 0); //显示中文 参数：x,y,编码文件的索引
4. OLED_ShowStr(); 显示字符串
*/


#endif
