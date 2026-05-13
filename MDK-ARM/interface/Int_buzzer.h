#ifndef __INT_BUZZER_H
#define __INT_BUZZER_H

#include "tim.h"

#define BUZZER_FREQ_1 261
#define BUZZER_FREQ_2 293
#define BUZZER_FREQ_3 329
#define BUZZER_FREQ_4 349
#define BUZZER_FREQ_5 392
#define BUZZER_FREQ_6 440

/**
 * @brief 打开蜂鸣器 发出声音
 * 
 * @param 
 */
void Int_buzzer_on(void);

/**
 * @brief 关闭蜂鸣器 停止发出声音
 * 
 * @param 
 */
void Int_buzzer_off(void);

/**
 * @brief 设置蜂鸣器的频率
 * 
 * @param freq 频率
 */
void Int_buzzer_set(uint16_t freq);

/**
 * @brief 发出小星星的音乐声音
 */
void Int_buzzer_music(void);

#endif
