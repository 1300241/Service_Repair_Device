#ifndef __INT_MPU6050_H
#define __INT_MPU6050_H

#include "i2c.h"
#include "Com_debug.h"
#include "Com_Filter.h"

#define MPU6050_I2C_ADDR_W (0x68 << 1)
#define MPU6050_I2C_ADDR_R (0x68 << 1) + 1

// 寄存器地址
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_USER_CTRL 0x6A
#define MPU6050_FIFO_EN 0x23
#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_CONFIG 0x1A
#define MPU6050_POWER_MANAGEMENT_1 0x6B
#define MPU6050_POWER_MANAGEMENT_2 0x6C
#define MPU6050_WHO_AM_I 0x75

// 数据寄存器地址
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40

// 数值转换
#define MPU6050_GYRO_SCALE 16.4f
#define MPU6050_ACCEL_SCALE 16384.0f

typedef struct
{
    float gyro_x;
    float gyro_y;
    float gyro_z;
}MPU6050_Gyro;

typedef struct
{
    float accel_x;
    float accel_y;
    float accel_z;
}MPU6050_Accel;

/**
 * @brief 初始化MPU6050
 */
void Int_mpu6050_init(void);

/**
 * @brief 获取陀螺仪数据
 * 
 * @param gyro 陀螺仪数据
 */
void Int_mpu6050_get_gyro(MPU6050_Gyro *gyro);

void Int_mpu6050_get_gyro_with_filter(MPU6050_Gyro *gyro);

/**
 * @brief 获取加速度计数据
 * 
 * @param accel 加速度计数据
 */
void Int_mpu6050_get_accel(MPU6050_Accel *accel);

void Int_mpu6050_get_accel_with_filter(MPU6050_Accel *accel);

#endif
