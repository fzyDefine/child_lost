/**
 * @file    BasicFun.h
 * @author  ZhuoYue
 * @brief   ������ʱ���������������ּĴ����Ķ���
 * @version 0.1
 * @date    2022-10-30
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __BASICFUN_H_
#define __BASICFUN_H_

#include <STC15F2K60S2.H> // STC15F2K60S2.H ������ͷ�ļ�
#include <STDIO.H>
#include <INTRINS.H>	  // ʱ�����
#include <STRING.H>       // �ַ�������
#include <STDLIB.H>       // �ڴ洦��
#include <STDARG.H>       // �ɱ����

#define u16 unsigned int
#define u8 unsigned char

#define bool u8
#define true  1
#define false 0

#define LOCK_REGISTER      P2    // ������������ַ

#define Y_LOCK           0X00    // Y0
#define Y_LED            0X80    // ������Y4��Ӧ38����������
#define Y_ULN2003        0XA0    // ������Y5��Ӧ38����������   
#define Y_DSPLAYCOM      0XC0    // ������Y6��Ӧ38����������
#define Y_DSPLAYINPUT    0XE0    // ������Y7��Ӧ38����������
#define Delay delay_ms

void Delay1ms();				// @11.0592MHz 1������ʱ����������
void delay_ms(u16 n_ms); 		// @11.0592MHz ���뼶��ʱ����������
void LockPort();         		// ���浱ǰ�˿�
void ChoosePort(u8 Y_device);   // ѡ��ĳ�豸

void Timer0Init(void);          // ��ʱ��
void Timer1Init(void);

#endif // !__BASICFUN__
