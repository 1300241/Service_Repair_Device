#include "Int_led.h"

// 发送0/1 采用的是高低电平持续时间
// 0 => 高电平0.295us 低电平0.595us
// 1 => 高电平0.595us 低电平0.295us

// 需要ns级的延迟函数 => HAL中的HAL_Delay(1); 单位是ms
// STM32能否实现ns级的延时？ 首先芯片的主频时钟达到ns级 72Mhz => 13.8ns

static void Int_led_delay_295ns(void)
{
    // 千万不能写while循环和for循环
    __NOP();
    __NOP();
}

/**
 * @brief 发送一个bit数据
 * 
 * @param bit 0 或 1
 */
static void Int_led_send_bit(uint8_t bit)
{
    if(bit == 0)
    {
        // 逻辑时间与实际时间不一致
        // （1）函数调用 本身需要3-10个时钟周期 => 采用直接编写代码的方式 直接使用寄存器操作 大大减少时间占用
        // （2）需要直接调用nop => 调用的个数需要减少几个
        // 发送0
        GPIOA->ODR |= GPIO_ODR_ODR15;   // 拉高
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        Int_led_delay_295ns();
        GPIOA->ODR &= ~GPIO_ODR_ODR15;   // 拉低
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        Int_led_delay_295ns();
        Int_led_delay_295ns();
    }
    else
    {
        // 发送1
        GPIOA->ODR |= GPIO_ODR_ODR15;   // 拉高
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        Int_led_delay_295ns();
        Int_led_delay_295ns();
        GPIOA->ODR &= ~GPIO_ODR_ODR15;   // 拉低
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        // __NOP();__NOP();__NOP();__NOP();__NOP();
        Int_led_delay_295ns();
    }
}

/**
 * @brief 发送数据到LED -> 32位数据只有24位有效
 * 
 * @param data 24bit数据表示不同的颜色
 */
void Int_led_send_data(uint32_t data)
{
    // 循环24次
    for(uint8_t i = 0; i < 24; i++)
    {
        if(data & (1 << (23-i)))
        {
            // 发送1
            Int_led_send_bit(1);
        }
        else
        {
            // 发送0
            Int_led_send_bit(0);
        }
    }
}


/**
 * @brief 闪烁单颜色LED
 * 
 * @param color 24bit数据表示不同的颜色
 */
void Int_led_blink(uint32_t color, uint16_t ms)
{
    // 1、让LED灯以对应的颜色亮
    Int_led_send_data(color);
    // 2、延时ms
    HAL_Delay(ms);
    // 3、关闭LED
    Int_led_send_data(0);
    // 4、延时ms
    HAL_Delay(ms);
}


/**
 * @brief 循环一圈彩虹色 => 每次切换颜色的时间是ms
 * 
 * @param ms 切换颜色时间
 */
void Int_led_rainbow(uint16_t ms)
{
    // 红橙黄绿青蓝紫
    uint32_t data = 0;
    for(uint16_t h=0; h < 360; h += 5)
    {
        if(h <= 60)
        {
            // 红 => 黄
            // 绿色逐渐加深 红色是满的 蓝色是0
            data = ((uint16_t)((h * 1.0 / 60) * 255.0) << 16) + (255 << 8) + 0;
        }
        else if(h <= 120)
        {
            // 黄 => 绿
            // 绿色是满的 红色逐渐减小 蓝色是0
            data = (255 << 16) + ((uint16_t)((120 - h) * 255.0 / 60) << 8) + 0;
        }
        else if(h <= 180)
        {
            // 绿 => 青
            // 绿色是满的 红色是0 蓝色逐渐加深
            data = (255 << 16) + 0 + ((uint16_t)((h - 120) * 255.0 / 60) << 0);
        }
        else if(h <= 240)
        {
            // 青 => 蓝
            // 绿色逐渐减小 红色是0 蓝色是满的
            data = ((uint16_t)((240 - h) * 255.0 / 60) << 16) + 0 + (255 << 0);
        }
        else if(h <= 300)
        {
            // 蓝 => 紫
            // 绿色是0 红色逐渐加深 蓝色是满的
            data = 0 + ((uint16_t)((h - 240) * 255.0 / 60) << 8) + (255 << 0);
        }
        else if(h <= 360)
        {
            // 紫 => 红
            // 绿色是0 红色是满的 蓝色逐渐减小
            data = 0 + (255 << 8) + ((uint16_t)((360 - h) * 255.0 / 60) << 0);
        }
        Int_led_send_data(data);
        HAL_Delay(ms);
    }

    // 关闭掉
    Int_led_send_data(0);
}
