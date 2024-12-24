#include <REGX52.H>
#include "UART.H"
unsigned char Ri;
unsigned char cnt;
unsigned char StrBUFF[20];
void UART_Init(void)		//4800bps@12.000MHz
{
	Timer0Init();
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF3;		//设置定时初始值
	TH1 = 0xF3;		//设置定时重载值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
	EA=1;       //启动所有中断
	ES=1;       //启动串口中断
}
void Timer0Init(void)		//250微秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0x06;		//设置定时初始值
	TH0 = 0x06;		//设置定时重载值
	TF0 = 0;		//清除TF0标志
	//TR0 = 1;		//定时器0开始计时
	TR0=0;
	ET0=1;
}

void UART_SendByte(unsigned char Byte)
{
  SBUF=Byte;
	while(TI==0);
	TI=0;
}
void UART_SendString(unsigned char * str)
{
	unsigned char Si=0;
	while(str[Si] != '\0')
	{
		UART_SendByte(str[Si]);
		Si++;
	}
}
unsigned char* UART_GetString(void)
{
	return StrBUFF;
}
void Timer0_Routine() interrupt 1
{
	cnt++;
	if(cnt >= 100)
	{
		cnt = 0;
		TR0 = 0;//定时器停止计时
		StrBUFF[Ri] = '\0';
		Ri = 0;
	}
}

void UART_Routine() interrupt 4
{
  if(RI==1)
	{
		RI=0;
		cnt = 0;    //重置定时器
		TL0 = 0x06;		
		TH0 = 0x06;	
		TR0 = 1;  //定时器0开始计时
		StrBUFF[Ri++]=SBUF; 
	}
}
