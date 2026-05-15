#include "App_upload_data.h"

/**
 * @brief 初始化上传数据相关代码
 */
void App_Upload_Data_Init(void)
{
    // 初始化相关的接口
    Int_GPS_Init();
    Int_qs100_init();
    Int_llcc68_init();
    Int_step_init();
}

/**
 * @brief 上传数据
 * 保存数据的结构 GPS_Data gps_data 其余信息使用变量 steps uuid(主控芯片编号 绝对独一无二)
 */
void App_upload_data(void)
{
    uint32_t step_count = Int_step_get_count();
    uint32_t uuid = (HAL_GetUIDw2() << 16) | (HAL_GetUIDw1() << 8) | HAL_GetUIDw0();

    
}

