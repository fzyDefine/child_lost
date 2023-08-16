#include "uart.h"
#include "gps.h"
#include "ATcommand.h"

char idata gpsRxBuffer[gpsRxBufferLength];
unsigned char RX_Count = 0;

bit busy;
xdata u8 uart02_buffer[UART_MAX];
u8 uart_index = 0;

void Clean_Uart02Buffer()
{
	uart_index = 0;
	memset(uart02_buffer,0,UART_MAX);
}


void UART01_Init(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE8;		//设置定时初始值
	TH1 = 0xFF;		//设置定时初始值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时

    ES = 1;
    EA = 1;     //开启定时器
}

void UART01_SendString(unsigned char *p)				//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{

		};
		TI=0;
	}   
}

void UART01_SendByte(unsigned char c)				//发送一个字符
{ 
    TI=0;   
    SBUF=c;   
    while(TI==0);   
    TI=0;   
}


void Uart01_Routine(void) interrupt 4 using 3	  		
{ 
	unsigned char temp = 0;
	char i = 0;
	ES=0;
	
	if(RI==1)
	{
		RI = 0;
		temp = SBUF;

		UART01_SendByte(temp);

//		if(temp == '$')
//		{
//			RX_Count = 0;	
//		}
//		
//		if(RX_Count <= 5)
//		{
//		   gpsRxBuffer[RX_Count++] = temp;
//		}

//		else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
//		{
//			gpsRxBuffer[RX_Count++] = temp;
//			if(temp == '\n')									   
//			{
//				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
//				memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count); 	//保存数据
//				Save_Data.isGetData = true;
//				RX_Count = 0;
//				memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空				
//			}
//			
//			if(RX_Count >= 75)
//			{
//				RX_Count = 75;
//				gpsRxBuffer[RX_Count] = '\0';//添加结束符
//			}			
//		}

	}
		
	ES=1; 
}



void Uart02_Init()		//9600bps
{
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0xE8;		//设置定时初始值
	T2H = 0xFF;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时

    IE2 &= 0xFE;   //先对需要设置的位进行清零 1111 1110
    IE2 |= 0x01;   // | ESPI | ES2 允许UART2中断
    EA = 1;        //允许总中断

	P_SW2 &=~ 0X01;	//选择 P10RX P11TX
}

/***********发送*********/
void Uart02_SendData(unsigned char dat)
{
    while (busy); // 等待前面的数据发送完成
    busy = 1;
    S2BUF = dat; // 写数据到UART2数据寄存器
}

/************发送字符串*******************/
void Uart02_SendString(char *s)
{
    while (*s) // 检测字符串结束标志
    {
        Uart02_SendData(*s++); // 发送当前字符
    }
}

void Uart02_Routine() interrupt 8 using 2
{
    unsigned char mydata;
    if(S2CON & 0x01)          //如果是接收中断
    {
        S2CON &= 0xFE;       //接收中断标志位清0  
        mydata = S2BUF;      //将接收缓冲区的数据保存到mydata变量中

        if(uart_index>=UART_MAX)
        {
            Clean_Uart02Buffer();
        }
        uart02_buffer[uart_index++] = mydata;


    }

	if(S2CON & 0x02)          //如果是发送中断
    {
        S2CON &= 0xFD;       //发送中断标志清0  
		busy = 0;
    }

}






















