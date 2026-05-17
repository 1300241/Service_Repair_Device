#ifndef __APP_UPLOAD_DATA_H
#define __APP_UPLOAD_DATA_H

#include "Int_AT6558R.h"
#include "Int_qs100.h"
#include "Int_llcc68.h"
#include "Int_step.h"
#include "cJSON.h"

/**
 * @brief 初始化上传数据相关代码
 */
void App_Upload_Data_Init(void);

/**
 * @brief 上传数据的类型 json类型
 * 保存数据的结构 GPS_Data gps_data 其余信息使用变量 steps uuid(主控芯片编号 绝对独一无二)
 */
void App_upload_data(void);

#endif
