#include "ds18b20.h"
#include "delay.h"

//18B20  PB8

//复位DS18B20
void DS18B20_Rst(void)
{
    //SET PG11 OUTPUT
    DS18B20_IO_OUT();
    //拉低DQ
    DS18B20_DQ_OUT = 0;
    //拉低750us
    delay_us(750);
    //DQ=1
    DS18B20_DQ_OUT = 1;
    //延时15us
    delay_us(15);
}

//从DS18B20读取一个位
//返回值:1/0
//补充代码
u8 DS18B20_Read_Bit(void)
{
    u8 data;
    DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT = 0;
    delay_us(2);
    DS18B20_DQ_OUT = 1;
    DS18B20_IO_IN();//SET PA0 INPUT
    delay_us(12);

    if(DS18B20_DQ_IN)data = 1;
    else data = 0;

    delay_us(50);
    return data;
}


//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void)
{
    u8 retry = 0;
    DS18B20_IO_IN();	//SET PG11 INPUT

    while (DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    };

    if(retry >= 200)return 1;
    else retry = 0;

    while (!DS18B20_DQ_IN && retry < 240)
    {
        retry++;
        delay_us(1);
    };

    if(retry >= 240)return 1;

    return 0;
}

//从DS18B20读取一个字节
//返回值:读到的数据
u8 DS18B20_Read_Byte(void)
{
    u8 i, j, dat;
    dat = 0;

    for (i = 1; i <= 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }

    return dat;
}
//写一个字节到DS18B20
//dat:要写入的字节
void DS18B20_Write_Byte(u8 dat)
{
    u8 j;//8次
    u8 testb;
    DS18B20_IO_OUT();	//SET PG11 OUTPUT;

    for (j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;

        if (testb)
        {
            DS18B20_DQ_OUT = 0;
            delay_us(2);
            DS18B20_DQ_OUT = 1;
            delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT = 0;
            delay_us(60);
            DS18B20_DQ_OUT = 1;
            delay_us(2);
        }
    }
}
//开始温度转换
void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在
u8 DS18B20_Init(void)
{
    //时钟和GPIO初始化

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PORTG口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//PORTB.8 推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_8);   //输出1

    DS18B20_Rst();

    return DS18B20_Check();
}
//从ds18b20得到温度值
//精度:0.01C
//返回值:温度值 (-550~1250)
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL, TH;
    short tem;
    DS18B20_Start ();
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);
    TL = DS18B20_Read_Byte();
    TH = DS18B20_Read_Byte();

    if(TH > 7)
    {
        TH = ~TH;
        TL = ~TL;
        temp = 0;
    }
    else temp = 1;

    tem = TH;
    tem <<= 8;
    tem += TL;
    tem = (float)tem * 0.0625;

    if(temp)return tem;
    else return -tem;
}




