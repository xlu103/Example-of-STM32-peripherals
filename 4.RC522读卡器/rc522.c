#include "rc522.h"
#include "delay.h"

//#include <string.h> 
#define MAXRLEN 18                        
/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          	  pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

//		UsartPrintf(USART1,"读卡成功进行第1步！\r\n");
   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);


   SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
		
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll(unsigned char *pSnr)
{
	
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
//		UsartPrintf(USART1,"读卡成功进行第2步！\r\n");
    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

//		UsartPrintf(USART1,"读卡成功进行第3步！\r\n");
    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);

        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}



/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
    RST_H;
    delay_10ms(1);
    RST_L;
    delay_10ms(1);
    RST_H;
	  delay_10ms(10);
	


    WriteRawRC(CommandReg,PCD_RESETPHASE);
    
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
    WriteRawRC(TxAutoReg,0x40);     
    return MI_OK;
}
//////////////////////////////////////////////////////////////////////
//设置RC632的工作方式 
//////////////////////////////////////////////////////////////////////
char M500PcdConfigISOType(unsigned char type)
{
   if (type == 'A')                     //ISO14443_A
   { 
       ClearBitMask(Status2Reg,0x08);


       WriteRawRC(ModeReg,0x3D);//3F

       WriteRawRC(RxSelReg,0x86);//84

       WriteRawRC(RFCfgReg,0x7F);   //4F
	
   	   WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
	   WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	   WriteRawRC(TPrescalerReg,0x3E);
	   

  //     PcdSetTmo(106);
	   delay_10ms(1);
       PcdAntennaOn();
   }
   else{ return (char)-1; }
   
   return MI_OK;
}
/////////////////////////////////////////////////////////////////////
//功    能：读RC632寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     NSS_L;
     ucAddr = ((Address<<1)&0x7E)|0x80;

     for(i=8;i>0;i--)
     {
         SCK_L;
	 	 if(ucAddr&0x80)
         	MOSI_H;
		 else
				 MOSI_L;
         SCK_H;
         ucAddr <<= 1;
     }

     for(i=8;i>0;i--)
     {
         SCK_L;
         ucResult <<= 1;
         SCK_H;
		 if(READ_MISO == 1)
         	ucResult |= 1;
     }

     NSS_H;
     SCK_H;
     return ucResult;
}

/////////////////////////////////////////////////////////////////////
//功    能：写RC632寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr;

    SCK_L;
    NSS_L;
    ucAddr = ((Address<<1)&0x7E);

    for(i=8;i>0;i--)
    {
		if(ucAddr&0x80)
        	MOSI_H;
		else
			MOSI_L;
        SCK_H;
        ucAddr <<= 1;
        SCK_L;
    }

    for(i=8;i>0;i--)
    {
		if(value&0x80)
        	MOSI_H;
		else
			MOSI_L;
        SCK_H;
        value <<= 1;
        SCK_L;
    }
    NSS_H;
    SCK_H;
}

/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
//    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
 i = 2000;
    do 
    {
         n = ReadRawRC(ComIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(ReadRawRC(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}


/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn()
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}

//等待卡离开
void WaitCardOff(void)
{
	char          status;
  unsigned char	TagType[2];

	while(1)
	{
		status = PcdRequest(REQ_ALL, TagType);
		if(status)
		{
			status = PcdRequest(REQ_ALL, TagType);
			if(status)
			{
				status = PcdRequest(REQ_ALL, TagType);
				if(status)
				{
					return;
				}
			}
		}
		delay_10ms(10);
	}
}

///////////////////////////////////////////////////////////////////////
// Delay 10ms
///////////////////////////////////////////////////////////////////////
void delay_10ms(unsigned int _10ms)
{
	unsigned int i, j;

	for(i=0; i<_10ms; i++)
	{
		for(j=0; j<60000; j++);
	}
}

/**

SDA（ss）——》A7

SCK——》A6

MOSI——》A5

MISO——》A4

GND——》GND

RST——》A1

3.3V——》3.3V
*/

extern u8 putong[];
extern int flag_b;//标志心跳，0为不跳，1为跳
extern char devPubTopic[];
unsigned char card1[5] = {0x20, 0x25, 0x35, 0x55, 0x3F}; //卡序列号
unsigned char card2[4] = {0xDE, 0x8A, 0xDF, 0x2B}; //卡序列号
unsigned char card3[4] = {0xD2, 0xB2, 0x73, 0x5B};
unsigned char card4[4] = {0xB7, 0x29, 0xDF, 0x2B};
unsigned char snr, buf[16] = "liu-bridge", bufr[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char Read_Data[16], PassWd[6], WriteData[16], RevBuffer[30], MLastSelectedSnr[4], NewKey[16];
	
char cardID[4];
extern char at_2[];
unsigned char kahao;
char CARD_BUFF_RC522[16];
extern  u8 Read_Card[5];
//  我的卡的序列号
unsigned char m_card_1 [15] = {0x20, 0xF6, 0xCD, 0x7E};

void rc522_gpio_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(MF522_RST_CLK, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Pin = MF522_RST_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(MF522_RST_PORT, &GPIO_InitStructure);

    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(MF522_MISO_CLK, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Pin = MF522_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(MF522_MISO_PORT, &GPIO_InitStructure);

    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(MF522_MOSI_CLK, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Pin = MF522_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(MF522_MOSI_PORT, &GPIO_InitStructure);

    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(MF522_SCK_CLK, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Pin = MF522_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(MF522_SCK_PORT, &GPIO_InitStructure);

    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(MF522_NSS_CLK, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Pin = MF522_NSS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(MF522_NSS_PORT, &GPIO_InitStructure);
    //UsartPrintf(USART1, "串口初始化完毕！\r\n");


//    //初始化蜂鸣器管脚
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
//    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA,&GPIO_InitStruct);

//    //红色灯  PC14
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
//    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOC,&GPIO_InitStruct);

//    //板子上的灯  PC13
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
//    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOC,&GPIO_InitStruct);

//    //白色灯   PC15
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
//    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOC,&GPIO_InitStruct);

}


/*
功能：蜂鸣器响，用于提示用户刷卡成功过！！
void Buzz(int count,long delay);
count:次数
delay:每次之间的延时
*/
void buzz(int count, long delay)
{
    int i = 0;

    for(i = 0; i < count; i++)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        delay_ms(delay);

        GPIO_ResetBits(GPIOC, GPIO_Pin_13); //这里要把0放下面，不然一直叫，吵死了
        delay_ms(delay);

    }
}



/*
void ledW();
count:次数
delay:每次之间的延时
flag:0代表常灭，1代表长亮，2代表要闪烁
*/
void ledW(int count, long delay, int flag)
{
    int i = 0;

    if(flag == 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    }
    else if(flag == 1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_8);
            delay_ms(delay);
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);
            delay_ms(delay);
        }
    }
}


/*
void ledW();
count:次数
delay:每次之间的延时
flag:0代表常灭，1代表长亮，2代表要闪烁
*/
void ledR(int count, long delay, int flag)
{
    int i = 0;

    if(flag == 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
    }
    else if(flag == 1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_9);
            delay_ms(delay);
            GPIO_ResetBits(GPIOB, GPIO_Pin_9);
            delay_ms(delay);
        }
    }
}


void rc522_init()
{
    PcdReset();
    PcdAntennaOff();
    PcdAntennaOn();
    M500PcdConfigISOType( 'A' );
    //UsartPrintf(USART1, "卡初始化完毕！\r\n");
}
void mfrc522Init()
{
    rc522_gpio_init();
    rc522_init();
}

char status;
    unsigned char rfidflag;
    unsigned char discnt;
   

char readCard()
{
    
 kahao = 0;
   
        status = PcdRequest(REQ_ALL, TagType);

        if(!status)//如果读到了卡
        {
            //UsartPrintf(USART1, "\r\n已检测到卡\r\n");
            status = PcdAnticoll(SelectedSnr);

            if(!status)
            {
                status = PcdSelect(SelectedSnr);

                if(!status)
                {
                    snr = 1;  //扇区号1
                    status = PcdAuthState(KEYA, (snr * 4 + 3), DefaultKey, SelectedSnr); // 校验1扇区密码，密码位于每一扇区第3块
                    {
                        if(!status)
                        {
                            //PcdAuthState(KEYA, (snr * 4 + 3), DefaultKey, SelectedSnr);
                            status = PcdRead((snr * 4 + 0), bufr);
                            findcard();

                            //将卡号在串口1打印出来
															//Usart_SendString(USART1, (unsigned char *)SelectedSnr, strlen((const char *)SelectedSnr));
 
												
													
                           // UsartPrintf(USART1, "\r\n%d",SelectedSnr[0]);
														//UsartPrintf(USART1, "%d",SelectedSnr[1]);
														//UsartPrintf(USART1, "%d",SelectedSnr[2]);
														//UsartPrintf(USART1, "%d\r\n",SelectedSnr[3]);
														

                            //UsartPrintf(USART1, "\r\n\r\n");
                            //UsartPrintf(USART1, "\r\n\r\n");


														sprintf(CARD_BUFF_RC522,"%d%d%d%d",SelectedSnr[0],SelectedSnr[1],SelectedSnr[2],SelectedSnr[3]);


                            WaitCardOff();//等待卡离开
                            delay_10ms(100);//延时避免在离开时又被读同
                            //readCard();
																return 1;
                        }
                    }
                }
            }
        }
				return 0;
    
}

void findcard()
{
    unsigned char ii;

    for (ii = 0; ii < 5; ii++)	 //比较卡序列号
    {
        if (SelectedSnr[ii] = card1[ii])
        {
            kahao = 0;
            break;
        }
        else
        {
            if (ii == 3)
            {
                kahao = 1;
                goto A;
                break;
            }
        }
    }

    for (ii = 0; ii < 4; ii++)
    {
        if (SelectedSnr[ii] != card2[ii])
        {
            kahao = 0;
            break;
        }
        else
        {
            if (ii == 3)
            {
                kahao = 2;
                goto A;
                break;
            }
        }
    }

    for (ii = 0; ii < 4; ii++)
    {
        if (SelectedSnr[ii] != card3[ii])
        {
            kahao = 0;
            break;
        }
        else
        {
            if (ii == 3)
            {
                kahao = 3;
                goto A;
                break;
            }
        }
    }

    for (ii = 0; ii < 4; ii++)
    {
        if (SelectedSnr[ii] != card4[ii])
        {
            kahao = 0;
            break;
        }
        else
        {
            if (ii == 3)
            {
                kahao = 4;
                goto A;
                break;
            }
        }
    }

A:

    if(kahao == 1)
    {
        kahao = 1;
    }

    if(kahao == 2)
    {
        kahao = 2;
    }

    if(kahao == 3)
    {
        kahao = 3;
    }

    if(kahao == 4)
    {
        kahao = 4;
    }
}


