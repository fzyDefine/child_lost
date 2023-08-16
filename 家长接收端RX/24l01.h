#ifndef __24L01_H__
#define __24L01_H__
#define BYTE unsigned char
#define uchar unsigned char
#define TX_ADR_WIDTH    5   // 5 bytes TX(RX) address width
#define TX_PLOAD_WIDTH  40  // 	5 bytes TX payload

sbit LED = P2^0;

sbit CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^4;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^7;

//****************************************************************//
// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//***************************************************************//
//                   FUNCTION's PROTOTYPES  //
/****************************************************************/
 BYTE SPI_RW(BYTE byte);                                // Single SPI read/write
 BYTE SPI_Read(BYTE reg);                               // Read one byte from nRF24L01
 BYTE SPI_RW_Reg(BYTE reg, BYTE byte);                  // Write one byte to register 'reg'
 BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);  // Writes multiply bytes to one register
 BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);   // Read multiply bytes from one register

void TX_Mode(void);
void RX_Mode(void);
void Nrf2401_Init(void);
void Nrf_RecString(unsigned char* pstr,unsigned char buffer_len,unsigned char *rec_flag);

//接收数据这样写
//void main(void)
//{
//	uchar sta;
//	Nrf2401_Init();								//初始化IO引脚
//	delay_ms(1000);
//	RX_Mode();								//配置为接收模式
//	while(1)
//	{
//		sta=SPI_Read(STATUS);		//读取芯片的状态寄存器
//	  if((sta & 0x40) != 0)		//是否发生了接收数据完成的中断
//	  {												//是的话，将收到的数据存到数组rx_buf里
//		   SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
//		}
//		if((rx_buf[0]==0x23) && (rx_buf[1]==0x23) && (rx_buf[2]==0x23) && (rx_buf[3]==0x23) && (rx_buf[4]==0x23))
//		{												//判断收到的数据是不是0x23
//			LED = ~LED;
//		}
//		SPI_RW_Reg(FLUSH_RX,0xff);	//清除接收缓冲器
//		SPI_RW_Reg(WRITE_REG+STATUS,sta);//清除接收中断标志
//		delay_ms(1000);
//	}
//}

//*****************************************************************/
#endif   