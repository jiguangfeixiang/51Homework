#include <REGX52.H>
#include "UART.h"

/**
 * @brief  串口初始化，9600bps@12.000MHz，启用中断
 * @param  无
 * @retval 无
 */
void UART_Init()
{
    SCON = 0x50;  // 模式1，允许接收，8位数据帧
    PCON |= 0x80; // 波特率倍速
    TMOD &= 0x0F; // 清空定时器1的设置
    TMOD |= 0x20; // 定时器1模式2，8位自动重装
    TH1 = 0xFD;   // 波特率9600bps，定时器重装值
    TL1 = 0xFD;   // 波特率初值
    ET1 = 0;      // 关闭定时器1中断
    TR1 = 1;      // 启动定时器1
    ES  = 1;      // 开启串口中断
    EA  = 1;      // 开启总中断
}

