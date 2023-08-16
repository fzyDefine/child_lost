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
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE8;		//���ö�ʱ��ʼֵ
	TH1 = 0xFF;		//���ö�ʱ��ʼֵ
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ

    ES = 1;
    EA = 1;     //������ʱ��
}

void UART01_SendString(unsigned char *str)				//�����ַ���
{	
	while(*str !='\0')		
		UART01_SendByte(*str++);	

}

void UART01_SendByte(unsigned char c)				//����һ���ַ�
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
	
	if(RI == 1)
	{
		RI = 0;
		temp = SBUF;
		if(temp == '$')
		{
			RX_Count = 0;	
		}
			
		if(RX_Count <= 5)
		{
		   gpsRxBuffer[RX_Count++] = temp;
		}
		else if(gpsRxBuffer[0] == '$' &gpsRxBuffer[4] == 'M' && gpsRxBuffer[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
		{
			gpsRxBuffer[RX_Count++] = temp;
			if(temp == '\n')									   
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
				memcpy(Save_Data.GPS_Buffer, gpsRxBuffer, RX_Count); 	//��������
				Save_Data.isGetData = true;
				RX_Count = 0;
				memset(gpsRxBuffer, 0, gpsRxBufferLength);      //���				
			}
			
			if(RX_Count >= 75)
			{
				RX_Count = 75;
				gpsRxBuffer[RX_Count] = '\0';//��ӽ�����
			}			
		}
	}
	


}



void Uart02_Init()		//9600bps
{
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//��ʱ��ʱ��12Tģʽ
	T2L = 0xE8;		//���ö�ʱ��ʼֵ
	T2H = 0xFF;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ

    IE2 &= 0xFE;   //�ȶ���Ҫ���õ�λ�������� 1111 1110
    IE2 |= 0x01;   // | ESPI | ES2 ����UART2�ж�
    EA = 1;        //�������ж�

	P_SW2 &=~ 0X01;	//ѡ�� P10RX P11TX
}

/***********����*********/
void Uart02_SendData(unsigned char dat)
{
    while (busy); // �ȴ�ǰ������ݷ������
    busy = 1;
    S2BUF = dat; // д���ݵ�UART2���ݼĴ���
}

/************�����ַ���*******************/
void Uart02_SendString(char *s)
{
    while (*s) // ����ַ���������־
    {
        Uart02_SendData(*s++); // ���͵�ǰ�ַ�
    }
}

void Uart02_Routine() interrupt 8 using 2
{
    unsigned char mydata;
    if(S2CON & 0x01)          //����ǽ����ж�
    {
        S2CON &= 0xFE;       //�����жϱ�־λ��0  
        mydata = S2BUF;      //�����ջ����������ݱ��浽mydata������

        if(uart_index>=UART_MAX)
        {
            Clean_Uart02Buffer();
        }
        uart02_buffer[uart_index++] = mydata;


    }

	if(S2CON & 0x02)          //����Ƿ����ж�
    {
        S2CON &= 0xFD;       //�����жϱ�־��0  
		busy = 0;
    }

}






















