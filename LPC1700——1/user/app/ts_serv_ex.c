#include "ts_serv_ex.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
//#include "sys_cfg.h"

#include "os.h"
#include "lib_str.h"

#define REG_WR  0X80
#define REG_RD  0X81

#define WR_RAM_CMD    0X82
#define RD_RAM_CMD    0X83

#define SEND_LEN      0X02
#define SEND_CMD      0X03
#define SEND_ADDR     0X04
#define SEND_ADDRH    0X04
#define SEND_ADDRL    0x05
#define SEND_RD_LEN   0x06
#define RECV_RD_LEN   0X06
 
typedef enum
{
    SPD_UP_ADR         = 0X0110u,
    SPD_DOWN_ADR       = 0X0111u,
    BURST_P_LIM_ADR    = 0X0112u,
    BURST_H_LIM_ADR    = 0X0113u,
    BURST_ORIGIN_P_ADR = 0X0114u,
    BUSRT_P_SCALE_ADR  = 0X0115u,
    GLAND_P_LIM_ADR    = 0X0116u,
    BACK_LIGHT_ADR     = 0X0117u,
    
    CAL_TST_P_ADR      = 0X0118U,
    CAL_STD_P_ADR      = 0x0119u,
    CAL_TST_H_ADR      = 0X011AU,
    CAL_STD_H_ADR      = 0x011bu,
    
    TST_BURST_P_ADR    = 0X011C,
    TST_BUSRT_H_ADR    = 0X011D,
    TST_GLAND_P_ADR    = 0x011e,
    TST_STATUS_ADR     = 0X011F    
    
}COM_ADR_t;

OS_MUTEX send_mutex;
OS_SEM send_sem;

#define SEND_BUF_MAX_LEN  64
uint8_t  send_buf[SEND_BUF_MAX_LEN]; 
uint32_t send_buf_len;
uint32_t  send_buf_index;

void ts_send_byte(uint8_t dat) //touch screen
{
    UART_SendByte(LPC_UART0, dat);
    //while ((LPC_UART0->LSR & 0x40) == 0);
}

void ts_send_dbyte(uint16_t dat)
{
    ts_send_byte((dat >> 8) & 0xff);
    ts_send_byte(dat & 0x00ff);
}

void ts_serv_ex_init(void)
{
    OS_ERR err;
    
    OSSemCreate(&send_sem,
                "send_sem",
                (OS_SEM_CTR)0,
                &err);
    
    OSMutexCreate(&send_mutex,
                  "send_mutex",
                  &err);    
}

/*
CPU_CHAR  *Str_Copy_N (       CPU_CHAR    *pstr_dest,
                       const  CPU_CHAR    *pstr_src,
                              CPU_SIZE_T   len_max)
*/
void send_str(uint8_t *buf, uint32_t *buf_index, uint32_t buf_len)
{
    uint32_t len;
    uint32_t i;
    
    if (buf_len >= 16)
        len = 16;    
    else
        len = buf_len;
    
    UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, DISABLE);
    for (i = 0; i < len; i++)
        ts_send_byte(buf[*buf_index + i]);
    UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, ENABLE);    
    *buf_index += len;    
}

void ts_send_byte_str(uint8_t *pstr, uint32_t len)
{
    OS_ERR err;
    CPU_TS ts;
   
//     OSMutexPend(&send_mutex,
//                 0,
//                 OS_OPT_PEND_BLOCKING,
//                 &ts,
//                 &err);
    Mem_Copy(send_buf, pstr, len);
    send_buf_len = len;      
    send_buf_index = 0;
    send_str(send_buf, &send_buf_index, send_buf_len); 
    
    (void)OSSemPend(&send_sem,
                    0,
                    OS_OPT_PEND_BLOCKING,
                    &ts,
                    &err);    
}

void ts_send_byte_str_remain(void)
{
    OS_ERR err;
    OS_SEM_CTR ctr;
    
    if (send_buf_index < send_buf_len)
    {
        send_str(send_buf, &send_buf_index, send_buf_len - send_buf_index);   
    }
    else
    {
        ctr = OSSemPost(&send_sem,
                    OS_OPT_POST_1 + OS_OPT_POST_NO_SCHED,
                    &err);    
    }   
}

void ts_send_dbyte_str(uint16_t *pstr, uint32_t len)
{
    while (len--)
    {
        ts_send_dbyte(*pstr++);
    }
}

//A5 5A 06 83 00 17 01 00 23
void ts_set_head(uint8_t *buf)
{
    *buf++ = 0xA5;
    *buf   = 0x5A;    
}

void ts_send_back(uint8_t *buff, uint8_t len)
{
    OS_ERR err;
    CPU_TS ts;

    OSMutexPend(&send_mutex,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
    
    ts_send_byte_str(buff, len);
    
    OSMutexPost(&send_mutex,
                    OS_OPT_POST_NONE,
                    &err);
    
}

typedef struct
{
    uint8_t  addrh;
    uint8_t  addrl;
    uint8_t *send_dat_buf;
    uint32_t send_dat_len;
}wr_ram_t;

/* A5 5A len 82 addh addl xx xx  */
void write_ram(wr_ram_t *pdat)
{
    uint8_t buf[SEND_BUF_MAX_LEN];
    uint8_t i;
    uint8_t tmp;
    OS_ERR err;
    CPU_TS ts;
    
//     (void)OSSemPend(&send_sem,
//                     0,
//                     OS_OPT_PEND_BLOCKING,
//                     &ts,
//                     &err);
    OSMutexPend(&send_mutex,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
    
    ts_set_head(buf);
    buf[SEND_CMD]   = WR_RAM_CMD;
    buf[SEND_ADDRH] = pdat->addrh;
    buf[SEND_ADDRL] = pdat->addrl;
    Mem_Copy(buf + 6, pdat->send_dat_buf, pdat->send_dat_len);
    
    for (i = 6; i < 6 + pdat->send_dat_len; i+=2) //逆序
    {
        tmp      = buf[i+1];
        buf[i+1] = buf[i];
        buf[i]   = tmp;        
    }
    
    buf[SEND_LEN] = 3 + pdat->send_dat_len;    
    
    ts_send_byte_str(buf, buf[SEND_LEN] + 3);
    
    OSMutexPost(&send_mutex,
                    OS_OPT_POST_NONE,
                    &err);
}

typedef struct
{
    uint8_t  addrh;
    uint8_t  addrl;
    uint32_t rd_len;
}rd_ram_t;

/*A5 5A LEN 0X83 ADR_H ADR_L RD_LEN*/
void read_ram(rd_ram_t *rd_ram_cmd)
{
    uint8_t tmp_buf[7];
    OS_ERR err;
    CPU_TS ts;
    
//     (void)OSSemPend(&send_sem,
//                     0,
//                     OS_OPT_PEND_BLOCKING,
//                     &ts,
//                     &err);
    OSMutexPend(&send_mutex,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
    
    ts_set_head(tmp_buf); 
    tmp_buf[SEND_CMD]    = RD_RAM_CMD;
    tmp_buf[SEND_ADDRH]  = rd_ram_cmd->addrh;
    tmp_buf[SEND_ADDRL]  = rd_ram_cmd->addrl;
    tmp_buf[SEND_RD_LEN] = rd_ram_cmd->rd_len;
    tmp_buf[SEND_LEN]    = 0x04;    
    
    ts_send_byte_str(tmp_buf, sizeof(tmp_buf)); 
    
    OSMutexPost(&send_mutex,
                    OS_OPT_POST_NONE,
                    &err);
}

typedef struct
{
    uint8_t addr;
    uint8_t dat;
}wr_reg_t;

void write_reg(wr_reg_t *wr_reg_cmd)
{
    uint8_t tmp_buf[6];
    
    OS_ERR err;
    CPU_TS ts;
    
    OSMutexPend(&send_mutex,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
//     (void)OSSemPend(&send_sem,
//                     0,
//                     OS_OPT_PEND_BLOCKING,
//                     &ts,
//                     &err);
    
    ts_set_head(tmp_buf); 
    tmp_buf[SEND_CMD]      = 0x80;          // cmd
    tmp_buf[SEND_ADDR]     = wr_reg_cmd->addr;
    tmp_buf[SEND_ADDR + 1] = wr_reg_cmd->dat;
    tmp_buf[SEND_LEN]      = 3;  
    
    ts_send_byte_str(tmp_buf, tmp_buf[SEND_LEN] + 3);  
    
    OSMutexPost(&send_mutex,
                    OS_OPT_POST_NONE,
                    &err);    
}

#define BK_LIGH_ADDR 0x01
void back_light_ctl(uint8_t *buf)
{
    wr_reg_t cmd;
    
    cmd.addr = BK_LIGH_ADDR;
    cmd.dat  = *(buf + 8);
    
    write_reg(&cmd);        
}


uint32_t send_cal_p_enable;
void ts_cal_p_resume(void)
{
    send_cal_p_enable = 1;   
}

void ts_cal_p_suspend(void)
{
    send_cal_p_enable = 0;
}

void get_cal_p(void)
{
    rd_ram_t  cmd;

    cmd.addrh  = CAL_TST_P_ADR >> 8;
    cmd.addrl  = CAL_TST_P_ADR & 0X00FF;
    cmd.rd_len = 0x02;

    read_ram(&cmd);        
}

void cal_p(uint8_t *buf)
{
    // 校准
}


void dis_cal_p(uint16_t *p) //显示实测数据
{
    wr_ram_t dat;

    dat.addrh = CAL_TST_P_ADR >> 8;
    dat.addrl = CAL_TST_P_ADR & 0x00ff;
    dat.send_dat_buf = (uint8_t *)p;
    dat.send_dat_len = 2; 
    write_ram(&dat);            
}

///////////////////////////////////////////////////////////
uint32_t send_cal_h_enable;
void ts_cal_h_resume(void)
{
    send_cal_h_enable = 1;   
}

void ts_cal_h_suspend(void)
{
    send_cal_h_enable = 0;
}

void get_cal_h(void)
{
    rd_ram_t  cmd;

    cmd.addrh  = CAL_TST_H_ADR >> 8;
    cmd.addrl  = CAL_TST_H_ADR & 0X00FF;
    cmd.rd_len = 0x02;

    read_ram(&cmd);        
}

void cal_h(uint8_t *buf)
{
    // 校准
}

void dis_cal_h(uint16_t *h) //显示实测数据
{
    wr_ram_t dat;

    dat.addrh = CAL_TST_H_ADR >> 8;
    dat.addrl = CAL_TST_H_ADR & 0x00ff;
    dat.send_dat_buf = (uint8_t *)h;
    dat.send_dat_len = 2; 
    write_ram(&dat);            
}

uint32_t dis_tst_enable;
void ts_dis_tst_resume(void)
{
    dis_tst_enable = 1;
}

void ts_dis_tst_suspend(void)
{
    dis_tst_enable = 0;
}

void dis_tst(void *rst)
{                           
    //uint32_t 
    //uint8_t tmp_buf[];
    wr_ram_t dat;

    //OS_ENTER_CRITICAL();
    //OS_EXIT_CRITICAL();
    if (dis_tst_enable == 0)
        return;
    dat.addrh = TST_BURST_P_ADR >> 8;
    dat.addrl = TST_BURST_P_ADR & 0x00ff;
    dat.send_dat_buf = (uint8_t *)rst;
    dat.send_dat_len = 6; 
    write_ram(&dat);   
    
}

static TS_SERV_FUNC ts_q_serv = NULL;
void ts_serv_register(TS_SERV_FUNC serv)
{
    ts_q_serv = serv;
}

//void ts_debug_diaphragm_oil()
void ts_debug_add_oil(void)
{
    uint32_t msg;
    
    msg = 0x01u;
    ts_q_serv(msg);
}

void ts_debug_sub_oil(void)
{
    uint32_t msg;
    
    msg = 0x02u;
    ts_q_serv(msg);
}

void ts_debug_gland_up(void)
{
    uint32_t msg;
    
    msg = 0x03u;
    ts_q_serv(msg);    
}

void ts_debug_gland_down(void)
{
    uint32_t msg;
    
    msg = 0x04u;
    ts_q_serv(msg);    
}

TS_COM_CMD_FUNC  const ts_com_cmd_serv[]  = \
{
    0, 0, 0, 0, 0,            /* 0- 4*/
    ts_cal_p_resume, ts_cal_h_resume, get_cal_p, ts_cal_p_suspend, get_cal_h, /* 5-9*/
    ts_cal_h_suspend, ts_dis_tst_resume, 0, ts_dis_tst_suspend, 0,            /* a-e */
    0, ts_debug_add_oil, ts_debug_sub_oil, ts_debug_gland_up, ts_debug_gland_down                                                             /* f-0x12*/
};

TS_COM_DATA_FUNC const ts_com_data_serv[] = \
{
    0, 0, 0, 0, 0,      /*0-4*/
    0, 0, back_light_ctl, 0, cal_p, /*5-9*/
    0, cal_h, 0, 0, 0,              /*a-e*/
    0, 0, 0, 0, 0,                  /*f-13*/  
    0, 0, 0, 0, 0                   /*14-19*/
};                                  /*1a-1e */                                                                                                                                                                                                                                                                                                                         
