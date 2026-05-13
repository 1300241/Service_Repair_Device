#include "Int_qs100.h"

uint8_t qs100_buff[QS100_BUFF_SIZE];
uint16_t qs100_buff_len = 0;
uint16_t qs100_current_len = 0;
uint8_t cmd_buff[256] = {0};

void Int_qs100_send_cmd(uint8_t *cmd)
{
    // 1. 发送命令
    HAL_UART_Transmit(&huart3, cmd, strlen((char *)cmd), 1000);
    memset(qs100_buff,0,QS100_BUFF_SIZE);
    qs100_buff_len = 0;

    // 2. 等待命令执行完毕 => OK或者ERROR
    uint8_t count = 0;
    while (strstr((char *)qs100_buff, "OK") == NULL && strstr((char *)qs100_buff, "ERROR") == NULL)
    {
        // 判断缓冲区是否已满
        if (qs100_buff_len >= QS100_BUFF_SIZE)
        {
            break;
        }

        // 需要继续进行接收
        HAL_UARTEx_ReceiveToIdle(&huart3, qs100_buff + qs100_buff_len, QS100_BUFF_SIZE - qs100_buff_len, &qs100_current_len, 1000);
        if (qs100_current_len + qs100_buff_len >= QS100_BUFF_SIZE)
        {
            qs100_buff_len = QS100_BUFF_SIZE;
            break;
        }
        
        count ++;
        if(count > QS100_MAX_RECV_COUNTS)
        {
            break;
        }
    }
    
}

/**
 * @brief 初始化qs100
 * 
 */
void Int_qs100_init(void)
{
    // 1. 硬件唤醒芯片
    HAL_GPIO_WritePin(NB_WKUP_GPIO_Port, NB_WKUP_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(NB_WKUP_GPIO_Port, NB_WKUP_Pin, GPIO_PIN_RESET);

    // 2. 软件重启
    HAL_UART_Transmit(&huart3, "AT+RB\r\n", 7, 1000);
    memset(qs100_buff,0,QS100_BUFF_SIZE);
    HAL_UART_Receive(&huart3, qs100_buff, QS100_BUFF_SIZE, 3000);
    debug_printf("qs100_buff: %s",qs100_buff);

    // 发送查询版本信息
    // memset(qs100_buf,0,512);
    // HAL_UART_Transmit(&huart3, "AT+CGMR\r\n", 9, 1000);
    // HAL_UARTEx_ReceiveToIdle(&huart3, qs100_buf, 512, &rx_len, 1000);
    // debug_printf("qs100_buf: %s",qs100_buf);

    // 3. 使用cmd命令 发送查询版本信息
    Int_qs100_send_cmd("AT+CGMR\r\n");
    debug_printf("qs100_buff: %s",qs100_buff);

    // 4. 设置打开回显
    Int_qs100_send_cmd("ATE1\r\n");
    debug_printf("qs100_buff: %s",qs100_buff);

    // 5. 测试回显效果
    Int_qs100_send_cmd("AT+CGMR\r\n");
    debug_printf("qs100_buff: %s",qs100_buff);
}

/**
 * @brief 查询芯片的附着状态(能否链接运营商网络) 判断是否已经连接到外网
 * 
 * @return IOT_Status: 0: 未连接 1: 已连接
 */
IOT_Status Int_qs100_get_ip(void)
{
    Int_qs100_send_cmd("AT+CGATT?\r\n");
    debug_printf("qs100_buff: %s",qs100_buff);
    if (strstr((char *)qs100_buff, "+CGATT:1") != NULL)
    {
        return IOT_OK;
    }
    return IOT_ERROR;
}

/**
 * @brief 打开socket
 * 
 * @return IOT_Status
 */
IOT_Status Int_qs100_open_socket(uint8_t *socket_id)
{
    // 0: 分配port端口号 不自己指定 避免出现冲突
    // 0: 不接收云服务器返回的消息
    Int_qs100_send_cmd("AT+NSOCR=STREAM,6,0,0\r\n");
    debug_printf("%s", qs100_buff);
    if (strstr((char *)qs100_buff, "OK") != NULL)
    {
        // 打开socket的函数中需要记录使用的socket编号
        char *p = strstr((char *)qs100_buff, "+NSOCR:");
        if(p != NULL)
        {
            p += 7;
            *socket_id = atoi((char *)p);
            debug_printf("socket_id: %d",*socket_id);
        }
        return IOT_OK;
    }
    return IOT_ERROR;
}

/**
 * @brief 连接到外网TCP云服务器
 * http://netlab.luatos.com/
 * @return IOT_Status
 */
IOT_Status Int_qs100_connect_server(uint8_t socket_id)
{
    memset(cmd_buff,0,256);
    sprintf((char *)cmd_buff,"AT+NSOCO=%d,%s,%d\r\n",socket_id,QS100_SERVER_IP,QS100_SERVER_PORT);
    Int_qs100_send_cmd(cmd_buff);
    debug_printf("%s", qs100_buff);
    if (strstr((char *)qs100_buff, "OK") != NULL)
    {
        return IOT_OK;
    }
    return IOT_ERROR;
}

/**
 * @brief 使用外网发送消息到云服务器
 * 
 * @param msg 消息内容
 * @param len 消息长度
 */
void Int_qs100_send_msg(uint8_t *msg, uint16_t len)
{
    // 1. 检查附着 是否能够连接到外网
    uint8_t count = 0;
    while (Int_qs100_get_ip() != IOT_OK && count < QS100_MAX_RECV_COUNTS)
    {
        HAL_Delay(1000);
        count ++;
    }

    uint8_t socket_id = 0;

    // 2. 创建socket
    while (Int_qs100_open_socket(&socket_id) != IOT_OK && count < QS100_MAX_RECV_COUNTS)
    {
        HAL_Delay(100);
        count ++;
    }

    // 3. 连接云服务器
    while (Int_qs100_connect_server(socket_id) != IOT_OK && count < QS100_MAX_RECV_COUNTS)
    {
        HAL_Delay(100);
        count ++;
    }
    
}


