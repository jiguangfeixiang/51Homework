#include "LCD.h"

// 延时函数
void DelayMS(uchar ms)
{
    uint i, j;
    for (i = 0; i < ms; i++) // 使用 ms 作为循环变量
        for (j = 0; j < 112; j++);
}

// 写命令函数
void LCD_Write_CMD(uchar cmd)
{
    LCD_ENABLE = ~LCD_ENABLE;
    LCD_RS     = 0;
    LCD_PORT   = cmd; // 传送命令到 LCD_PORT
    DelayMS(2);       // 延时 2ms，确保命令稳定传送
    LCD_ENABLE = 1;
    DelayMS(2); // 再次延时 2ms
    LCD_ENABLE = 0;
}

// 写数据函数
void LCD_Write_DATA(uchar DATA)
{
    LCD_ENABLE = 0;
    LCD_RS     = 1;    // rs = 1，表示写数据
    LCD_PORT   = DATA; // 传送数据到 LCD_PORT
    DelayMS(2);        // 延时 2ms
    LCD_ENABLE = 1;
    DelayMS(2); // 延时 2ms
    LCD_ENABLE = 0;
}

// 设置光标位置
void LCD_GOTO_XY(uchar x, uchar y)
{
    if (x == 0) {
        LCD_Write_CMD(0x80 + y); // 0x80 为地址起始点
    } else if (x == 1) {
        LCD_Write_CMD(0xC0 + y); // 0xC0 为第二行起始地址
    }
}

// 打印字符串
void LCD_Printf(uchar x,uchar y,uchar *str)
{
    LCD_GOTO_XY(x, y);
    while (*str != '\0') {
        LCD_Write_DATA(*str++); // 逐个字符写入
    }
}

// LCD 初始化
void LCD_Init()
{
    LCD_Write_CMD(0x38); // 16x2 显示，5x7 点阵，8 位数据口
    LCD_Write_CMD(0x0C); // 开启显示，不显示光标
    LCD_Write_CMD(0x06); // 地址加1，写入数据后光标右移
    LCD_Write_CMD(0x01); // 清屏
}

// LCD 测试
// void LCD_Test()
// {
//     LCD_GOTO_XY(0, 0);
//     LCD_Printf("Hello World!");
//     LCD_GOTO_XY(1, 0);
//     LCD_Printf("test");
// }
