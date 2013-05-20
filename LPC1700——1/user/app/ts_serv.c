#include "ts_serv.h"
#include "serial.h"
#include "lpc17xx_uart.h"
#include "ts_serv_ex.h"

#include "os.h"
#include "app_cfg.h"
#include "lib_mem.h"

OS_TCB  ts_serv_task_tcb;
CPU_STK ts_serv_task_stk[200];

void ts_recv_serv(uint8_t *recv_buff, uint32_t recv_nbr)
{
    //ts_send_byte_str((uint8_t *)recv_buff, recv_nbr);  
    //back_light_ctl(recv_buff);   
    //ts_init();
    uint32_t index;
    
    /*
    if (recv_buff[3] == 0x83)
    {
        index = (recv_buff[4] << 8) + recv_buff[5];
        if (index < 0x100 )
        {
            if (ts_com_cmd_serv[index] != 0)
                ts_com_cmd_serv[index]();
        }
        else
        {
            if (ts_com_data_serv[index - 0x110] != 0)
                ts_com_data_serv[index - 0x110](recv_buff);   
        }
    }
    */
    
    ts_send_back(recv_buff, recv_nbr);
    
}

uint8_t recv_buff_copy[TS_RECV_BUFF_LEN];

#define MIN_RECV_LEN  13u

#define START_CHAR ('@')
#define END_CHAR_1 (';')//('\r')//(0X0D) //cr
#define END_CHAR_2 ('!')//('\n')//(0x0a) //lf

void ts_recv_isr(void *recv_buff_0, void *recv_nbr_0)
{
    OS_ERR err;
    uint32_t i;
    uint32_t start_pos;
    uint8_t  *recv_buff = (uint8_t *)recv_buff_0;
    uint32_t *recv_nbr  = (uint32_t *)recv_nbr_0;
    uint32_t len;
    
    if (*recv_nbr >= TS_RECV_BUFF_LEN)
    {
        *recv_nbr = 0;
    }
    *recv_nbr += UART_Receive(LPC_UART0, recv_buff + *recv_nbr, \
                              TS_RECV_BUFF_LEN - *recv_nbr);
    
    start_pos = *recv_nbr - 1;
    if (*recv_nbr > MIN_RECV_LEN)   
    {
        for (i = 0; i < *recv_nbr-1; i++)
        {
            //if (recv_buff[i] == 0xa5 && recv_buff[i+1] == 0x5a)
            //if (recv_buff[i] == 0xa5 && recv_buff[i+1] == 0x5a)
            if (recv_buff[i] == START_CHAR)
            {
                start_pos = i;
                break;
            }
        }
    }
    else 
        return;

    if (start_pos < *recv_nbr - 1 )
    {  
        if (start_pos != 0)
        {
            for (i = 0; i < *recv_nbr - start_pos ; i++)
            {
                recv_buff[i] = recv_buff[i + start_pos];    
            }
            *recv_nbr -= start_pos;
        }

        if (*recv_nbr > MIN_RECV_LEN)
        {
            //if (*recv_nbr == recv_buff[2] + 3)
            if (recv_buff[*recv_nbr - 2] == END_CHAR_1 && recv_buff[*recv_nbr - 1] == END_CHAR_2)
            {
                len = *recv_nbr;
                
                recv_buff[len]     = recv_buff[4];
                recv_buff[len + 1] = recv_buff[5];
                recv_buff[len + 2] = recv_buff[6];
                
                Mem_Copy(recv_buff_copy, recv_buff, len + 3);
                *recv_nbr = 0;
                
                OSTaskQPost(&ts_serv_task_tcb,
                            (uint8_t *)recv_buff_copy,
                            len + 3,
                            OS_OPT_POST_FIFO,
                            &err);  
                
            }
        }
        else
            return;
    }
    else
    {
        *recv_nbr = 1;
         recv_buff[0] = recv_buff[start_pos];  
    }
   
}

void ts_serv_task(void *p_arg)
{
    void *p_msg;
    OS_MSG_SIZE msg_size;
    CPU_TS ts;
    OS_ERR err;
    
    (void)&p_arg;   
   
    while (1)
    {
       p_msg = OSTaskQPend(0,
                            OS_OPT_PEND_BLOCKING,
                            &msg_size,
                            &ts,
                            &err);
        
        if (err == OS_ERR_NONE)
        {  
            //ts_send_byte_str((uint8_t *)p_msg, msg_size);
            ts_recv_serv((uint8_t *)p_msg, (uint32_t)msg_size);            
        }
    }
}

void ts_serv_init(void)
{
    OS_ERR err;
    
    serial_irq_serv_register(ts_recv_isr, ts_send_byte_str_remain);  
    ts_serv_ex_init();
    
    OSTaskCreate(&ts_serv_task_tcb,
                "ts_serv_task",
                ts_serv_task,
                (void *)0,
                TS_SERV_TASK_PRIORITY,
                &ts_serv_task_stk[0],
                (CPU_STK_SIZE)10,
                (CPU_STK_SIZE)(sizeof(ts_serv_task_stk) / sizeof(ts_serv_task_stk[0])),
                (OS_MSG_QTY   )TS_SERV_TASK_Q_SIZE,
                (OS_TICK      )0u,
                (void        *)0u,
                (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR      *)&err);    
}
