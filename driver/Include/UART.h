#ifndef __UART_H
#define __UART_H
extern unsigned char StrBUFF[20];
void UART_Init(void);
void UART_SendByte(unsigned char Byte);
void UART_SendString(unsigned char * str);
void Timer0Init(void)	;
unsigned char* UART_GetString(void);
#endif