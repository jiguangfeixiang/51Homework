#ifndef __PORT_H__
#define __PORT_H__
#include <REGX52.H>
#define uchar unsigned char // 以后unsigned char就可以用uchar代替
#define uint  unsigned int  // 以后unsigned int 就可以用uint 代替

#define Temp_low  40
#define Temp_high 50
sbit SETTING = P1 ^ 0;
sbit ADD     = P1 ^ 1;
sbit SUB     = P1 ^ 2;
sbit OK      = P1 ^ 3;

sbit RL1 = P1 ^ 5;

sbit DS_temp = P2 ^ 1;
/// @brief
sbit BUZZER = P2 ^ 0;

sbit LCD_RS      = P1 ^ 4;
sbit LCD_ENABLE  = P1 ^ 6;
sbit LCD_DISABLE = P1 ^ 7;

sbit DS1302_RST  = P3 ^ 0;
sbit DS1302_SCLK = P3 ^ 1;
sbit DS1302_IO   = P3 ^ 2;

#define LCD_PORT P0
#endif