#ifndef LCD_H
#define LCD_H
#include "port.h"




void DelayMS(uchar ms);
void LCD_Write_CMD(uchar cmd);
void LCD_Write_DATA(uchar DATA);
void LCD_GOTO_XY(uchar x, uchar y);
void LCD_Printf(uchar x, uchar y, uchar *str);
void LCD_Init();

#endif // LCD_H
