#include <REGX52.H>
#include "LCD.h"
#include "port.h"
#include "DS18B20.h"
#include "Delay.h"
#include <stdio.h>
#include "Key.h"
#include "DS1302.h"
#include "Timer0.h"
void Read_Temp();
void Temp_Show();
void Time_Set();
void TimeShow(void);
uchar KeyNum = 0, MODE, TimeSetSelect, TimeSetFlashFlag;
float T;
unsigned char Time[16];
char tmp_show[10];
int count = 0;
int main()
{
    Read_Temp();
    LCD_Init();
    Timer0Init();
    DS1302_Init();
    Delay(1000);
    LCD_Printf(1, 0, "  :  :  ");
    DS1302_SetTime();
    while (1) {
        // 读取按键设置时间
        // KeyNum = Key();
        // if (KeyNum == 1) {
        //     if (MODE == 0) {
        //         MODE = 1;
        //         TimeShow();
        //     }
        //     if (MODE == 1) {
        //         MODE = 0;
        //         Time_Set();
        //     }
        // }
        TimeShow();
        // 温度展示
        Temp_Show();
    }
}
void Read_Temp()
{
    DS18B20_ConvertT();
    T = DS18B20_ReadT();
}
// void Timer0_Routine() interrupt 1
// {
//     static unsigned int T0Count;
//     TL0 = 0x18; // 设置定时初值
//     TH0 = 0xFC; // 设置定时初值
//     T0Count++;
//     if (T0Count >= 500) // 每500ms进入一次
//     {
//         T0Count          = 0;
//         TimeSetFlashFlag = !TimeSetFlashFlag; // 闪烁标志位取反
//     }
// }
void Temp_Show()
{
    Read_Temp();

    sprintf(tmp_show, "temp:%.1f", T);
    LCD_Printf(0, 0, tmp_show);
    if (T<Temp_low | T> Temp_high) {
        BUZZER = 1;
        // 温度控制
        if (T < Temp_low) {

            LCD_Printf(0, 9, "  cold!");
            RL1   = 1;
            count = 1;

        } else if (T > Temp_high) {
            LCD_Printf(0, 9, "  hot!");
            count = 1;
            RL1   = 0;
        }
    } else {
        if (count == 1) {
            count = 0;
            LCD_Printf(0, 9, "                "); // 清空第 1 行（16 个空格）
        }
        LCD_Printf(0, 9, " Normal!"); // 显示 "Normal!"
        BUZZER = 0;
        RL1    = 0;
    }
}
void TimeShow(void) // 时间显示功能
{
    DS1302_ReadTime(); // 读取时间
    // LCD_ShowNum(1,1,DS1302_Time[0],2);//显示年
    // LCD_ShowNum(1,4,DS1302_Time[1],2);//显示月
    // LCD_ShowNum(1,7,DS1302_Time[2],2);//显示日
    // LCD_ShowNum(2,1,DS1302_Time[3],2);//显示时
    // LCD_ShowNum(2,4,DS1302_Time[4],2);//显示分
    // LCD_ShowNum(2,7,DS1302_Time[5],2);//显示秒
    sprintf(Time, "%02d:%02d:%02d", DS1302_Time[3], DS1302_Time[4], DS1302_Time[5]);
    LCD_Printf(1, 0, Time);
}
