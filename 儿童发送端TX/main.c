#include <STC15F2K60S2.H>
#include <STDLIB.H>
#include <STRING.H>
#include <stdio.h>
#include "Delay.h"
#include "UART.h"
#include "LCD1602.h"
#include "gps.h"
#include "24l01.h"
#include "ATcommand.h"


#define u8 unsigned char
#define u16 unsigned int

u8 recGps_buffer[40] = "1234";
u8 temp_buf[TX_PLOAD_WIDTH]={1,2,3,4,5,6,7,8,9,10};


u8 Key_scan()
{
	u8 key_value = 0;
	if(P31==0)
	{
		Delay_ms(20);
		if(P31==0)
		{
			key_value = 1;
		}
		while(!P31);
	}

	return key_value;
}	

void main()
{
	unsigned char i = 0;
	unsigned char key_v = 0;

	UART01_Init();
	Uart02_Init();
 
	Nrf2401_Init();
	Delay_ms(10);
	TX_Mode();	//发送模式

	clrStruct();	//清空GPS缓存数组

	
	while(1)
	{
		parseGpsBuffer();	//解析数据

		if(Save_Data.isUsefull)	//如果解析到的数据是有效的
		{
			Rec_LngLat(recGps_buffer,40);	//提取出经纬度信息放到recGps_buffer组里
		}
		else
		{
			UART01_SendString("Data Error!\r\n");
			
		}

		printGpsBuffer();	//串口打印数据
		
		Nrf_SendString(recGps_buffer,TX_PLOAD_WIDTH);//向从机发送GPS信息。
		delay_ms(500);
	}	
}

