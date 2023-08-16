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
	TX_Mode();	//����ģʽ

	clrStruct();	//���GPS��������

	
	while(1)
	{
		parseGpsBuffer();	//��������

		if(Save_Data.isUsefull)	//�������������������Ч��
		{
			Rec_LngLat(recGps_buffer,40);	//��ȡ����γ����Ϣ�ŵ�recGps_buffer����
		}
		else
		{
			UART01_SendString("Data Error!\r\n");
			
		}

		printGpsBuffer();	//���ڴ�ӡ����
		
		Nrf_SendString(recGps_buffer,TX_PLOAD_WIDTH);//��ӻ�����GPS��Ϣ��
		delay_ms(500);
	}	
}

