#ifndef __RC522_H_H
#define __RC522_H_H



/*



SDA（ss）——》A7

SCK——》A6

MOSI——》A5

MISO——》A4

GND——》GND

RST——》A1

3.3V——》3.3V

1. 调用 mfrc522Init();  初始化
2. 调用 readCard(); 读卡
3. 在 main函数中 引用  extern char CARD_BUFF_RC522[16];
4. 读卡：
if(readCard()){

// 串口输出 CARD_BUFF_RC522

}


*/
extern char PcdReset(void);
extern void PcdAntennaOn(void);
extern void PcdAntennaOff(void);
extern char PcdRequest(unsigned char req_code,unsigned char *pTagType);   
extern char PcdAnticoll(unsigned char *pSnr);
extern char PcdSelect(unsigned char *pSnr);         
extern char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);     
extern char PcdRead(unsigned char addr,unsigned char *pData);     
extern char PcdWrite(unsigned char addr,unsigned char *pData);    
extern char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);   
extern char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);                                 
extern char PcdHalt(void);
extern char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit);
extern void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
extern void WriteRawRC(unsigned char Address,unsigned char value);
extern unsigned char ReadRawRC(unsigned char Address); 
extern void SetBitMask(unsigned char reg,unsigned char mask); 
extern void ClearBitMask(unsigned char reg,unsigned char mask); 
extern char M500PcdConfigISOType(unsigned char type);
extern void delay_10ms(unsigned int _10ms);
extern void WaitCardOff(void);
/////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		          0x3F


#define     REQ_ALL               0x52
#define     KEYA                  0x60

/////////////////////////////////////////////////////////////////////
//和MF522通讯时返回的错误代码
/////////////////////////////////////////////////////////////////////
#define MI_OK                          (char)0
#define MI_NOTAGERR                    (char)(-1)
#define MI_ERR                         (char)(-2)


extern void rc522_gpio_init(void);
extern void rc522_init(void);
extern char readCard(void);
extern void findcard(void);
extern void mfrc522Init(void);
extern void biBiBi(int count,long delay);
extern void ledW(int count,long delay,int flag);
extern void ledR(int count,long delay,int flag);

#define MF522_RST_PIN                    GPIO_Pin_1
#define MF522_RST_PORT                   GPIOA
#define MF522_RST_CLK                    RCC_APB2Periph_GPIOA  
  
#define MF522_MISO_PIN                   GPIO_Pin_4
#define MF522_MISO_PORT                  GPIOA
#define MF522_MISO_CLK                   RCC_APB2Periph_GPIOA  

#define MF522_MOSI_PIN                   GPIO_Pin_5
#define MF522_MOSI_PORT                  GPIOA
#define MF522_MOSI_CLK                   RCC_APB2Periph_GPIOA  

#define MF522_SCK_PIN                    GPIO_Pin_6
#define MF522_SCK_PORT                   GPIOA
#define MF522_SCK_CLK                    RCC_APB2Periph_GPIOA 

#define MF522_NSS_PIN                    GPIO_Pin_7
#define MF522_NSS_PORT                   GPIOA
#define MF522_NSS_CLK                    RCC_APB2Periph_GPIOA 


#define RST_H                            GPIO_SetBits(MF522_RST_PORT, MF522_RST_PIN)
#define RST_L                            GPIO_ResetBits(MF522_RST_PORT, MF522_RST_PIN)
#define MOSI_H                           GPIO_SetBits(MF522_MOSI_PORT, MF522_MOSI_PIN)
#define MOSI_L                           GPIO_ResetBits(MF522_MOSI_PORT, MF522_MOSI_PIN)
#define SCK_H                            GPIO_SetBits(MF522_SCK_PORT, MF522_SCK_PIN)
#define SCK_L                            GPIO_ResetBits(MF522_SCK_PORT, MF522_SCK_PIN)
#define NSS_H                            GPIO_SetBits(MF522_NSS_PORT, MF522_NSS_PIN)
#define NSS_L                            GPIO_ResetBits(MF522_NSS_PORT, MF522_NSS_PIN)
#define READ_MISO                        GPIO_ReadInputDataBit(MF522_MISO_PORT, MF522_MISO_PIN)
 
#define TRUE 1
#define FALSE 0

#define NOT_SUB '2'		// 未预约
#define NOT_REG '1' 	// 未注册
#define SUCC__ '0'		// 成功
#define OUT__ '3'			// 出馆
#endif