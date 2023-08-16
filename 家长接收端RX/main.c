#include <STC15F2K60S2.H>
#include <STDLIB.H>
#include <STRING.H>
#include <stdio.h>
#include <INTRINS.H>
#include "Delay.h"
#include "UART.h"
#include "LCD1602.h"
#include "gps.h"
#include "ATcommand.h"
#include "24l01.h"
#include "Timer0.h"

#define u8 unsigned char
#define u16 unsigned int

sbit BEEP = P2^4;

u8 ambulance_beep = 0;
u8 pwm_up = 10;
u8 add_flag = 1;

char *phone ="19827056722";
char send_msg_flag = 0;

char recbuf[TX_PLOAD_WIDTH] = "";
char cpybuf[TX_PLOAD_WIDTH] = "";

char error_times = 0;

void Delay350us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 4;
	j = 192;
	do
	{
		while (--j);
	} while (--i);
}

void Delay500us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 6;
	j = 93;
	do
	{
		while (--j);
	} while (--i);
}


void ambulance_voice()
{
	u8 i=8;  //控制蜂鸣器发出警音次数
	u8 j = 0;
    u16 y;
	EA = 0;
         
	for(;i>0;i--)  //初值调用变量i的初始赋值
	{
		for(y=0;y<2000;y++)  //700Hz频率，持续1秒
		{    
			BEEP=1;
			Delay350us();
			BEEP=0;
			Delay350us();
		}
		for(y=0;y<1600;y++)  //1000Hz频率，持续1秒
		{
			BEEP=1;
			Delay500us();
			BEEP=0;
			Delay500us();
		}    
	}
	i=0;  //清零
	BEEP=1;  //关闭蜂鸣器

	EA = 1;
}

void main()
{
	unsigned char i = 0;
	unsigned char rec_flag = 0;
	unsigned char start_flag = 0;

	UART01_Init();
	Uart02_Init();
	Timer0_Init();

	Nrf2401_Init();//Nrf初始化
	Delay_ms(10);
	RX_Mode();	
	BEEP = 1;

	LCD_Init();


/**/
	/*GSM*/
	LCD_ShowString(2,1,"SIM INITing");
	Wait_CREG();
	UART01_SendString("Set CREG succeed!\r\n");
	Set_MsgMode("0513");
	UART01_SendString("Set MSG_MODE succeed!\r\n");
	LCD_ShowString(2,1,"SIM INIT OK");
	clrStruct();	//清空GPS缓存数组

	send_msg_flag = 1;
	while(1)
	{

		memset(recbuf,0,TX_PLOAD_WIDTH);
		Nrf_RecString(recbuf,TX_PLOAD_WIDTH,&rec_flag);

		if(recbuf[0]!=0)
		{
			start_flag++;	//只要接收到了一次信号就代表启动成功
			memcpy(cpybuf,recbuf,TX_PLOAD_WIDTH);	//拷贝有效信息

			if(start_flag>100)
				start_flag = 1;
		}
		
		if( start_flag )	//报警
		{
			if(recbuf[0]== 0 && recbuf[1]== 0 && recbuf[2]== 0&&recbuf[3]== 0)
			{
				error_times++;
			}
			else
				error_times = 0;

			
			if(error_times >= 5)
			{
				if(send_msg_flag)
				{
					
					Send_Msg(phone,cpybuf);	//发送有效信息
					send_msg_flag = 0;
				}
				ambulance_voice();
			
			}
			else
				BEEP = 1;
		

		}
		else
			BEEP = 1;

		for(i=0;i<32;i++)
		{
			LCD_ShowChar(i/16+1,i%16+1,recbuf[i]);
		}
		Delay_ms(500);
	}	
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	static int j_ms = 0;

	if(j_ms++ == 200)
	{
		j_ms = 0;
		
	
		if(pwm_up >= 100 || pwm_up <= 0)
			add_flag = !add_flag;

		if(add_flag)
			pwm_up += 10;
		else
			pwm_up -= 10;
	}



	if(send_msg_flag)
	{
		T0Count++;
		if(T0Count>=3000)
		{
			T0Count=0;
			send_msg_flag = 1;
		}
	}
	else
		T0Count = 0;


}


