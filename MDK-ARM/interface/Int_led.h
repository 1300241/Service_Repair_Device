#ifndef __INT_LED_H
#define __INT_LED_H

#include "main.h"

// 24bit数据表示不同的颜色
#define LED_RED     0x00FF00   // R=255, G=0,   B=0
#define LED_ORANGE  0xA5FF00   // R=255, G=165, B=0   (标准橙色)
#define LED_YELLOW  0xFFFF00   // R=255, G=255, B=0
#define LED_GREEN   0xFF0000   // R=0,   G=255, B=0
#define LED_CYAN    0xFF00FF   // R=0,   G=255, B=255
#define LED_BLUE    0x0000FF   // R=0,   G=0,   B=255
#define LED_PURPLE  0x00FFFF   // R=255, G=0,   B=255 (品红/紫)

/**
 * @brief 发送数据到LED -> 32位数据只有24位有效
 * 
 * @param data 24bit数据表示不同的颜色
 */
void Int_led_send_data(uint32_t data);

/**
 * @brief 闪烁单颜色LED     闪烁中亮和灭的时间都是ms
 * 
 * @param color 24bit数据表示不同的颜色
 */
void Int_led_blink(uint32_t color, uint16_t ms);

/**
 * @brief 循环一圈彩虹色 => 每次切换颜色的时间是ms
 * 
 * @param ms 切换颜色时间
 */
void Int_led_rainbow(uint16_t ms);

#endif
