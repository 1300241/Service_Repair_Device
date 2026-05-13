#ifndef __INT_QS100_H
#define __INT_QS100_H

#include "usart.h"
#include "Com_debug.h"
#include "stdlib.h"

#define QS100_DATA_MAX_LEN 512
#define QS100_BUFF_SIZE 1024
#define QS100_MAX_RECV_COUNTS 5

#define QS100_SEQUENCE_ID 1

// 连接的云服务器地址
#define QS100_SERVER_IP "115.120.239.161"
#define QS100_SERVER_PORT 26232

typedef enum
{
    IOT_OK = 0,
    IOT_ERROR,
}IOT_Status;

/**
 * @brief 初始化qs100
 * 
 */
void Int_qs100_init(void);

/**
 * @brief 查询芯片的附着状态 判断是否已经连接到外网
 * 
 * @return IOT_Status: 0: 未连接 1: 已连接
 */
IOT_Status Int_qs100_get_ip(void);

/**
 * @brief 打开socket
 * 
 * @return IOT_Status
 */
IOT_Status Int_qs100_open_socket(uint8_t *socket_id);

/**
 * @brief 关闭socket
 * 
 * @param socket_id socket id
 * @return IOT_Status
 */
IOT_Status Int_qs100_close_socket(uint8_t socket_id);

/**
 * @brief 连接到外网TCP云服务器
 * http://netlab.luatos.com/
 * @return IOT_Status
 */
IOT_Status Int_qs100_connect_server(uint8_t socket_id);

/**
 * @brief 使用外网发送消息到云服务器
 * 
 * @param socket_id socket id
 * @param data 消息内容
 * @param len 消息长度
 * @return IOT_Status
 */
IOT_Status Int_qs100_send(uint8_t socket_id, uint8_t *data, uint16_t len);

/**
 * @brief 使用外网发送消息到云服务器
 * 
 * @param msg 消息内容
 * @param len 消息长度
 */
IOT_Status Int_qs100_send_msg(uint8_t *msg, uint16_t len);

#endif // __INT_QS100_H
