#include "motor_ctl.h"
#include "sys_monitor.h"
#include "vir_kbd.h"
#include "vir_limit.h"
#include "sensor_test.h"
#include "out_valve.h"
#include "ts_serv_ex.h"

#include "os.h"
#include "app_cfg.h"

#include "sys_cfg.h"

OS_TCB  sys_monitor_task_tcb;
CPU_STK sys_monitor_task_stk[200];

OS_Q kbd_q;
OS_Q lim_q;
OS_Q sensor_q;
OS_Q ts_q;

//suck
//spit
/*
#define LIM_MOTOR_UP_BIT   ((uint32_t)1)
#define LIM_MOTOR_DOWN_BIT ((uint32_t)2)
#define LIM_PISTON_BIT     ((uint32_t)4)
*/

typedef struct
{
    uint32_t kbd_msg;
    uint32_t lim_msg;
    uint32_t sensor_msg; 
    uint32_t ts_msg;
}sys_msg_t;
sys_msg_t sys_msg;

typedef struct
{
    uint32_t sts;
    uint32_t step;
    uint32_t stage;    
}sys_sts_t;

sys_sts_t sys_sts;
uint32_t  status;

//#define KEY_START  0X1U
//#define KEY_UP     0X02U
//#define KEY_DOWN   0X03U
// #define SENSOR_GLAND_P_OV_BIT   ((uint32_t)0x01)
// #define SENSOR_H_OV_BIT         ((uint32_t)0x02)
// #define SENSOR_BUSRT_P_OV_BIT   ((uint32_t)0x04)
// #define SENSOR_BUSRT_OK_BIT     ((uint32_t)0x08)

void suck_oil(sys_msg_t *msg, sys_sts_t * sts)
{
    if (sts->stage == 0)
    {
        if ((msg->lim_msg & LIM_MOTOR_DOWN_BIT) == 0)
        {
            motor_ctl(MOTOR_DOWN); 
        }
        close_piston_up_oil_valve();
        close_piston_down_oil_valve();
        close_diaphragm_oil_valve();
        open_main_oil_valve(); 
        sts->stage++;     
    }
    if (sts->stage == 1)
    {
        if (((msg->lim_msg & LIM_MOTOR_DOWN_BIT) == LIM_MOTOR_DOWN_BIT) ||
             (msg->kbd_msg == KEY_START))
        {
            sts->stage = 0; 
            close_main_oil_valve();
            motor_ctl(MOTOR_STOP);       
        }
                   
    }
}

#define STS_GLAND_UP               0X01u
#define STS_GLAND_DOWN             0X02U
#define STS_START_WORK             0x03u
#define STS_DIAPHRAGM_ADD_OIL      0x04u
#define STS_DIAPHRAGM_CALLBACK_OIL 0x05u
//
#define TS_DIAPHRAGM_ADD_OIL       0X01u
#define TS_DIAPHRAGM_CALLBACK_OIL  0X02u
#define TS_GLAND_UP                0x03u
#define TS_GLAND_DOWN              0x04u

void spit_oil(sys_msg_t *msg, sys_sts_t *sts)
{
    uint32_t status;
    
    status = sts->sts;
    if (status == STS_START_WORK)
    {
        if (sts->step == 1)
        {
            status = STS_GLAND_DOWN;
        }
        else if (sts->step == 3)
        {
            status = STS_DIAPHRAGM_ADD_OIL;
        }
        else if (sts->step == 5)
        {
            status = STS_GLAND_UP;     
        }
        
    }
    
    switch (status)
    {
    //case STS_START_WORK:
    case STS_DIAPHRAGM_ADD_OIL:
        if (sts->stage == 0)
        {
            if (((msg->sensor_msg & SENSOR_BURST_P_OV_BIT) == 0) ||
                ((msg->lim_msg & LIM_MOTOR_UP_BIT) == 0))
            {
                close_piston_down_oil_valve();
                close_diaphragm_oil_valve();
                close_main_oil_valve();
                open_diaphragm_oil_valve();
                motor_ctl(MOTOR_UP);
                sts->stage++;
            }
            else if (((msg->sensor_msg & SENSOR_BURST_P_OV_BIT) == SENSOR_BURST_P_OV_BIT) ||
                      ((msg->lim_msg & LIM_MOTOR_UP_BIT) == LIM_MOTOR_UP_BIT))
            {
                sts->stage++;
                open_diaphragm_oil_valve();
            }            
        }
        if (sts->stage == 1)
        {
            if (((msg->sensor_msg & SENSOR_BURST_P_OV_BIT) == SENSOR_BURST_P_OV_BIT) ||
                ((msg->lim_msg & LIM_MOTOR_UP_BIT) == LIM_MOTOR_UP_BIT) ||
                ((msg->sensor_msg & SENSOR_H_OV_BIT) == SENSOR_H_OV_BIT) ||
                (msg->kbd_msg == KEY_START))
            {
                //close_diaphragm_oil_valve(); 
                motor_ctl(MOTOR_STOP);  
                sts->stage = 0;
            }
        }
        break;
    case STS_GLAND_UP:
        if (sts->stage == 0)
        {
            if ((msg->lim_msg & LIM_MOTOR_UP_BIT) == 0)
            {
                close_piston_down_oil_valve();
                close_diaphragm_oil_valve();
                close_main_oil_valve();
                open_piston_up_oil_valve();
                motor_ctl(MOTOR_UP);
            }
            sts->stage++;
        }
        if (sts->stage == 1)
        {
            if (((msg->lim_msg & LIM_MOTOR_UP_BIT) == LIM_MOTOR_UP_BIT) || 
                ((msg->lim_msg & LIM_PISTON_BIT) == LIM_PISTON_BIT))  
            {
                close_piston_up_oil_valve();  
                motor_ctl(MOTOR_STOP);  
                sts->stage = 0;
// }    
            }
        }
        break;
    case STS_GLAND_DOWN:
        if (sts->stage == 0)
        {
            if (((msg->sensor_msg & SENSOR_GLAND_P_OV_BIT) == 0) ||
                ((msg->lim_msg & LIM_MOTOR_UP_BIT) == 0))
            {
                close_piston_down_oil_valve();
                close_diaphragm_oil_valve();
                close_main_oil_valve();
                open_piston_down_oil_valve();
                motor_ctl(MOTOR_UP);
                sts->stage++;
            }
            else if ((msg->sensor_msg & SENSOR_GLAND_P_OV_BIT) == SENSOR_GLAND_P_OV_BIT)
            {
                sts->stage++;
            }            
        }
        if (sts->stage == 1)
        {
            if (((msg->sensor_msg & SENSOR_GLAND_P_OV_BIT) == SENSOR_GLAND_P_OV_BIT) ||
                ((msg->lim_msg & LIM_MOTOR_UP_BIT) == LIM_MOTOR_UP_BIT) ||
                (msg->kbd_msg == KEY_START))
            {
                //close_piston_down_oil_valve();  
                motor_ctl(MOTOR_STOP);  
                sts->stage = 0;
            }
        }
        break;
    
    default:
        break;           
   
    }
}

void callback_oil(sys_msg_t *msg, sys_sts_t *sts)
{
    if (sts->stage == 0)
    {
        if ((msg->lim_msg & LIM_MOTOR_DOWN_BIT) == 0)
        {
            close_piston_down_oil_valve();
            close_diaphragm_oil_valve();
            close_main_oil_valve();
            open_diaphragm_oil_valve();
            motor_ctl(MOTOR_DOWN);
            sts->stage++;
        }
        else if ((msg->lim_msg & LIM_MOTOR_DOWN_BIT) == LIM_MOTOR_DOWN_BIT)
        {
            sts->stage++;
            open_diaphragm_oil_valve();
        }            
    }
    if (sts->stage == 1)
    {
        if (((msg->lim_msg & LIM_MOTOR_DOWN_BIT) == LIM_MOTOR_DOWN_BIT) ||
             (msg->kbd_msg == KEY_START))
        {
            close_diaphragm_oil_valve(); 
            motor_ctl(MOTOR_STOP);  
            sts->stage = 0;
        }
    }
 }
/*
kbd
short : start
two short : up
long : down
//0代表无键按下
//1: start
//2: up
//3: down
*/

void chk_msg(sys_msg_t *msg, sys_sts_t *sts)
{
    if (sts->sts == 0) 
    {
        sts->step  = 0;
        sts->stage = 0;
        
        if (msg->kbd_msg > 0)
        {
            if (msg->kbd_msg == KEY_START)
            {
                sts->sts = STS_START_WORK;
            }
            else if (msg->kbd_msg == KEY_UP)
            {
                sts->sts = STS_GLAND_UP;            
            }
            else //if (msg->kbd_msg == KEY_DOWN)
            {
                sts->sts = STS_GLAND_DOWN;
            }  
            
            msg->kbd_msg = 0;             
        }
        else if (msg->ts_msg > 0) //调试
        {
            if (msg->ts_msg == TS_DIAPHRAGM_ADD_OIL)
            {
                sts->sts = STS_DIAPHRAGM_ADD_OIL;
            }
            else if (msg->ts_msg == TS_DIAPHRAGM_CALLBACK_OIL)
            {
                sts->sts = STS_DIAPHRAGM_CALLBACK_OIL;    
            }   
            else if (msg->ts_msg == TS_GLAND_UP)
            {
                sts->sts = STS_GLAND_UP;   
            }
            else if (msg->ts_msg == TS_GLAND_DOWN)
            {
                sts->sts = STS_GLAND_DOWN;    
            }                
        }
    } 
    
    if (sts->sts == STS_START_WORK)
    {
        if (sts->step == 0)     //吸油
        {
            suck_oil(msg, sts); 
            if (sts->stage == 0)
                sts->step++;
        }            
        if (sts->step == 1)    //压头向下
        {
            spit_oil(msg, sts);
            if (sts->stage == 0)
            {
                sts->step++;
                //sts->sts  = 0;
            }
        }  
        if (sts->step == 2)   //吸油
        {
            suck_oil(msg, sts);
            if (sts->stage == 0)
            {
               sts->step++;     
            }    
        }
        if (sts->step == 3)   //膜片加油
        {
            spit_oil(msg, sts);
            if (sts->stage == 0)
            {
                sts->step++;
                //sts->sts  = 0;
            }
        }
        if (sts->step == 4)   //膜片回油
        {
            callback_oil(msg, sts);
            if (sts->stage == 0)
            {
               sts->step++;     
            }    
        }
        if (sts->step == 5)  //压头抬起来
        {
            spit_oil(msg, sts);
            if (sts->stage == 0)
            {
                sts->step++;
                
            }    
        } 
        if (sts->step == 6)  
        {
            suck_oil(msg, sts);
            if (sts->stage == 0)
            {
               sts->sts  = 0;     
            }       
        }            
    }
    else if (sts->sts == STS_GLAND_UP)
    {
        if (sts->step == 0)
        {
            suck_oil(msg, sts); 
            if (sts->stage == 0)
                sts->step++;
        }            
        if (sts->step == 1) 
        {
            spit_oil(msg, sts);
            if (sts->stage == 0)
            {
                sts->step = 0;
                sts->sts  = 0;
            }
        }            
    }
    else if (sts->sts == STS_GLAND_DOWN)
    {
        if (sts->step == 0)
        {
            suck_oil(msg, sts); 
            if (sts->stage == 0)
                sts->step++;
        }            
        if (sts->step == 1) 
        {
            spit_oil(msg, sts);
            if (sts->stage == 0)
            {
                sts->step = 0;
                sts->sts  = 0;
            }
        }        
    } 
    else if (sts->sts == STS_DIAPHRAGM_ADD_OIL)   
    {
        if (sts->step == 0)
        {
            suck_oil(msg, sts);
            if (sts->stage == 0)
                sts->step++;
        }
        if (sts->step == 1)
        {
            spit_oil(msg, sts);
            if (sts->stage == 0)
            {
                sts->step = 0;
                sts->sts  = 0;
            }    
        }
    } 
    else if (sts->sts == STS_DIAPHRAGM_CALLBACK_OIL)  
    {
        if (sts->step == 0)
        {
            callback_oil(msg, sts);
            if (sts->stage == 0)
            {
               sts->step = 0;
               sts->sts  = 0;       
            }
        }
    }        
}

void sys_monitor_task(void *p_arg)
{
    OS_ERR err;
    OS_PEND_DATA pend_data_tbl[4];
    OS_OBJ_QTY   nbr_rdy;
    
//     CPU_SR_ALLOC();
    (void)&p_arg;

    sys_sts.sts   = 0;
    sys_sts.step  = 0;
    sys_sts.stage = 0;
       
    while (1)
    {
        pend_data_tbl[0].PendObjPtr = (OS_PEND_OBJ *)&kbd_q;
        pend_data_tbl[1].PendObjPtr = (OS_PEND_OBJ *)&lim_q;
        pend_data_tbl[2].PendObjPtr = (OS_PEND_OBJ *)&sensor_q;
        pend_data_tbl[3].PendObjPtr = (OS_PEND_OBJ *)&ts_q;
        
        nbr_rdy = OSPendMulti((OS_PEND_DATA *)&pend_data_tbl[0],
                               4,
                               0,
                               OS_OPT_PEND_BLOCKING,
                               &err);
         
        if (err == OS_ERR_NONE)
        {
            sys_msg.kbd_msg = 0;
            sys_msg.ts_msg  = 0;
           
            if (nbr_rdy)    
            {
                if (pend_data_tbl[0].RdyObjPtr == pend_data_tbl[0].PendObjPtr) 
                {
                    sys_msg.kbd_msg = (uint32_t)pend_data_tbl[0].RdyMsgPtr;    
                } 
                
                if (pend_data_tbl[1].RdyObjPtr == pend_data_tbl[1].PendObjPtr) 
                {
                    sys_msg.lim_msg = (uint32_t)pend_data_tbl[1].RdyMsgPtr;                    
                }   
                
                if (pend_data_tbl[2].RdyObjPtr == pend_data_tbl[2].PendObjPtr) 
                {
                    sys_msg.sensor_msg = (uint32_t)pend_data_tbl[2].RdyMsgPtr;                    
                } 
                
                if (pend_data_tbl[3].RdyObjPtr == pend_data_tbl[3].PendObjPtr) 
                {
                    sys_msg.ts_msg = (uint32_t)pend_data_tbl[3].RdyMsgPtr;                    
                }
                chk_msg(&sys_msg, &sys_sts);                
            }                
        }
    }
}

static void kbd_serv(uint32_t msg)
{
    OS_ERR err;
    
    OSQPost(&kbd_q,
            (void *)msg,
            sizeof(msg),
            OS_OPT_POST_FIFO,
            &err);    
}

static void lim_serv(uint32_t msg)
{
    OS_ERR err;
    
    OSQPost(&lim_q,
            (void *)msg,
            sizeof(msg),
            OS_OPT_POST_FIFO,
            &err);     
}

static void sensor_serv(uint32_t  msg)
{
    OS_ERR err;
    
    OSQPost(&sensor_q,
            (void *)msg,
            sizeof(msg),
            OS_OPT_POST_FIFO,
            &err);      
}

static void ts_serv(uint32_t  msg)
{
    OS_ERR err;
    
    OSQPost(&ts_q,
            (void *)msg,
            sizeof(msg),
            OS_OPT_POST_FIFO,
            &err);      
}

void sys_monitor_init(void)
{
    OS_ERR err;
    
    //serv_motor_init();
    OSQCreate(&kbd_q,
              "kbd_q",
              (OS_MSG_QTY)4,
              &err);
    
    OSQCreate(&lim_q,
              "lim_q",
              (OS_MSG_QTY)4,
              &err);
    
    OSQCreate(&sensor_q,
              "sensor_q",
              (OS_MSG_QTY)4,
              &err);
    
    OSQCreate(&ts_q,
              "ts_q",
              (OS_MSG_QTY)4,
              &err);
    
    kbd_serv_register(kbd_serv);
    lim_serv_register(lim_serv);
    sensor_serv_register(sensor_serv);
    ts_serv_register(ts_serv);
    
    OSTaskCreate(&sys_monitor_task_tcb,
                "sys_monitor_task",
                sys_monitor_task,
                (void *)0,
                SYS_MONITOR_TASK_PRIORITY,
                &sys_monitor_task_stk[0],
                (CPU_STK_SIZE)10,
                (CPU_STK_SIZE)(sizeof(sys_monitor_task_stk) / sizeof(sys_monitor_task_stk[0])),
                (OS_MSG_QTY   )0u,
                (OS_TICK      )0u,
                (void        *)0u,
                (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR      *)&err);
    
    
}
