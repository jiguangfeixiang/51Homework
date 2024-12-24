#include <REGX52.H>
#include "LCD1602.h"
#include "port.h"
#include "DS18B20.h"
#include "Delay.h"
#include <stdio.h>
#include "DS1302.h"
#include "AT24C02.h"
#include "UART.h"
#include "string.h"
#include <stdlib.h>
void Read_Temp();
void Temp_Show();
void TimeShow(void);
void Night_Off_RL1(void);
void Send_Temp_History();
void Save_Data();
uchar *Read_Data();
float T = 0, last_T = 0;
char FLASH_SIZE         = 0;
unsigned char CharCount = 0;
uchar tmp_show[10];
uchar count = 0, night_flag = 0;
int main()
{
    Read_Temp();
    // Save_Data(); // 温度不一样就开始存储到Flash芯片里 时间格式24:60:60  T(4位)
    LCD_Init();
    UART_Init();
    DS1302_Init();
    DS1302_SetTime();
    LCD_ShowString(2, 1, "  :  :  ");
    while (1) {
        // 时间展示
        TimeShow();
        // 温度展示
        Temp_Show();
        // 夜晚控制
        Night_Off_RL1();
        // 温度不一样就存储到FLASH中
        if (T != last_T) {

            Save_Data();
        }
        // 接收到AT指令就把温度历史记录发送给串口终端
        // Send_Temp_History();
        Read_Data();
    }
}
// void Send_Temp_History()
// {
//     char *SendString;
//     Receive_String = UART_GetString();
//     if (strstr(Receive_String, "AT+T")) {
//         SendString = Read_Data();
//         LCD_ShowString(2, 1, SendString);
//         UART_SendString(SendString);
//         UART_SendString("\r\n");
//     }
// }
void Save_Data()
{
    uchar i;
    uchar temp_show_size = 0;
    sprintf((char *)tmp_show, "%.0f|", T);
    temp_show_size = strlen(tmp_show);
    for (i = 0; tmp_show[i] != '\0'; i++) {
        // 为了保持演示效果，35-55我存入，否则一些混乱数据就存进去了
        if (T >= 35 && T <= 55) {
            AT24C02_WriteByte(FLASH_SIZE, tmp_show[i]);
            Delay(10);
            FLASH_SIZE++;
        }
    }
    last_T = T;
    // char *test = "abcd";
    // char c;
    // char disp[2];
    // for (i = 0; test[i] != '\0'; i++) {
    //     AT24C02_WriteByte(i, test[i]);
    //     Delay(10);
    // }
    // for (i = 0; i < 4; i++) {
    //     c       = AT24C02_ReadByte(i);
    //     disp[0] = c;
    //     disp[1] = '\0';
    //     LCD_ShowString(2, 10 + i,disp);
    // }
}
uchar *Read_Data()
{
    uchar i;
    char ch[21];
    char *test;
    uchar *Receive_String;
    uchar *location;
    Receive_String = UART_GetString();
    // memset(StrBUFF, 0, 20);
    if (strstr(Receive_String, "AT+T")) {
        for (i = 0; i < FLASH_SIZE; i++) {
            // 发送数据到电脑端
            ch[i] = AT24C02_ReadByte(i);
            // UART
        }
        ch[FLASH_SIZE] = '\0';
        // LCD_ShowString(2, 1, ch);
        // LCD_ShowString(2, 10, (char *)FLASH_SIZE);
        UART_SendString(ch);
        UART_SendString("\r\n");
    }
    if (strstr(Receive_String, "AT+Stop")) {
        return;
    }
    if (strstr(Receive_String, "AT+Begin")) {
        RL1 = 1;
        return;
    }
    if (strstr(Receive_String, "AT+End")) {
        RL1 = 0;
        return;
    }
    if (strstr(Receive_String, "t")) {
        // sprintf(test, "%d%d", Receive_String[1], Receive_String[2]);
        // DS1302_Time[3] = atoi(test);
        // sprintf(test, "%d%d", Receive_String[3], Receive_String[4]);
        // DS1302_Time[4] = atoi(test);
        // sprintf(test, "%d%d", Receive_String[5], Receive_String[6]);
        // DS1302_Time[5] = atoi(test);
        // DS1302_SetTime();
        return;
    }
    return ch;
}
void Read_Temp()
{
    DS18B20_ConvertT();
    T = DS18B20_ReadT();
}
void Night_Off_RL1(void)
{
    if (DS1302_Time[3] >= 22 | DS1302_Time[3] <= 7) {
        RL1        = 0;
        BUZZER     = 0; // 晚上22-早上7点关掉加热，蜂鸣器不响
        night_flag = 1;
    } else {
        night_flag = 0;
    }
}
void Temp_Show()
{
    Read_Temp();

    sprintf(tmp_show, "temp:%3.0f", T);
    LCD_ShowString(1, 1, tmp_show);
    if (T<Temp_low | T> Temp_high) {
        if (night_flag == 0) {
            BUZZER = 1;
        }
        // 温度控制
        if (T < Temp_low) {

            LCD_ShowString(1, 9, "  cold!");
            RL1   = 1;
            count = 1;

        } else if (T > Temp_high) {
            LCD_ShowString(1, 9, "  hot!");
            count = 1;
            RL1   = 0;
        }
    } else {
        if (count == 1) {
            count = 0;
            LCD_ShowString(1, 9, "                "); // 清空第 1 行（16 个空格）
        }
        LCD_ShowString(1, 9, " Normal!"); // 显示 "Normal!"
        BUZZER = 0;
        RL1    = 0;
    }
}
void TimeShow(void) // 时间显示功能
{
    DS1302_ReadTime();                    // 读取时间
                                          // LCD_ShowNum(1,1,DS1302_Time[0],2);//显示年
                                          // LCD_ShowNum(1,4,DS1302_Time[1],2);//显示月
                                          // LCD_ShowNum(1,7,DS1302_Time[2],2);//显示日
                                          // LCD_ShowNum(2,1,DS1302_Time[3],2);//显示时
                                          // LCD_ShowNum(2,4,DS1302_Time[4],2);//显示分
                                          // LCD_ShowNum(2,7,DS1302_Time[5],2);//显示秒
    LCD_ShowNum(2, 1, DS1302_Time[3], 2); // 显示时
    LCD_ShowNum(2, 4, DS1302_Time[4], 2); // 显示分
    LCD_ShowNum(2, 7, DS1302_Time[5], 2); // 显示秒
}
