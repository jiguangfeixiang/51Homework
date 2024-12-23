#include <REGX52.H>
#include "LCD1602.h"
#include "port.h"
#include "DS18B20.h"
#include "Delay.h"
#include <stdio.h>
#include "DS1302.h"
#include "AT24C02.h"
#include "UART.h"
void Read_Temp();
void Temp_Show();
void TimeShow(void);
void Night_Off_RL1(void);
void Show_Reveive_data();
void Save_Data();
float T, last_T = 0;
unsigned char *FLASH_ADRESS = (unsigned char *)0x00;
unsigned char Time[10], CharCount = 0;
uchar tmp_show[10];
char *Receive_String;
uchar count = 0, night_flag = 0;

int main()
{
    Read_Temp();
    Save_Data(); // 温度不一样就开始存储到Flash芯片里 时间格式24:60:60  T(4位)
    LCD_Init();

    DS1302_Init();
    Delay(1000);
    DS1302_SetTime();
    LCD_ShowString(2, 1, "  :  :  ");
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
        // 时间展示
        TimeShow();
        // 温度展示
        Temp_Show();
        // 夜晚控制
        Night_Off_RL1();
        // 温度不一样就存储到FLASH中
        Save_Data();
        // 接收数据并展示
        Show_Reveive_data();
    }
}
void Show_Reveive_data()
{
}
void Save_Data()
{
    uchar i;
    if (last_T != T) {
        sprintf((char *)tmp_show, "%02d:%02d:%02d %.1f", DS1302_Time[3], DS1302_Time[4], DS1302_Time[5], T);
        for (i = 0; tmp_show[i] != '\0'; i++) {
            AT24C02_WriteByte(*FLASH_ADRESS, tmp_show[i]);
            FLASH_ADRESS++;
        }
        last_T = T;
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

    sprintf(tmp_show, "temp:%.1f", T);
    LCD_ShowString(1, 0, tmp_show);
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
/**
 * @brief  串口中断服务函数
 * @param  无
 * @retval 无
 */
void UART_ISR() interrupt 4
{
    uchar next;
    // 处理接收中断
    if (RI) {
        RI   = 0; // 清接收中断标志
        next = SBUF;
        if (next != '\n') {
            Receive_String[CharCount++] = next;

        } else {
            Receive_String[CharCount] = '\0';
            sprintf(Temp_Show, "temp:%s", Receive_String);
            LCD_ShowString(1, 9, Temp_Show);

            CharCount                 = 0;
        }
    }
    // // 处理发送中断
    // if (TI) {
    //     TI = 0; // 清发送中断标志

    //     if (UART_TX_Head != UART_TX_Tail) {
    //         // 发送下一个字节
    //         SBUF         = UART_TX_Buffer[UART_TX_Tail];
    //         UART_TX_Tail = (UART_TX_Tail + 1) % UART_TX_BUFFER_SIZE;
    //     } else {
    //         UART_TX_Busy = 0; // 无数据可发送，结束发送
    //     }
    // }
}
