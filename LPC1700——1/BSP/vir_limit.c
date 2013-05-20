#include "in_limit.h"
#include "vir_limit.h"
#include "vir_buzzer.h"
#include "serial.h"

#include "motor_ctl.h"

#include "os.h"
#include "app_cfg.h"

OS_TCB  limit_check_task_tcb;
CPU_STK limit_check_task_stk[200];

static LIM_SERV_FUNC lim_serv = NULL;
void lim_serv_register(LIM_SERV_FUNC serv)
{
    lim_serv = serv;
}

/*
#define LIM_MOTOR_UP_BIT   ((uint32_t)1)
#define LIM_MOTOR_DOWN_BIT ((uint32_t)2)
#define LIM_PISTON_BIT     ((uint32_t)4)
*/
void limit_check_task(void *p_arg)  
{
    OS_ERR err;

    uint32_t lim_cur_val  = 0;
    uint32_t lim_last_val = 0;
    
    (void)&p_arg;    
    
    while (1)
    {
        if (is_piston_limit())
        {
            if ((lim_cur_val & LIM_PISTON_BIT) == 0)
            {
                lim_cur_val |= LIM_PISTON_BIT;
            }
        }
        else
        {
            if ((lim_cur_val & LIM_PISTON_BIT) == LIM_PISTON_BIT)
            {
                lim_cur_val &= ~LIM_PISTON_BIT;
            }    
        }
        
        if (is_motor_up_limit())
        {
            if ((lim_cur_val & LIM_MOTOR_UP_BIT) == 0)
            {
                lim_cur_val |= LIM_MOTOR_UP_BIT;
            }
        }
        else
        {
            if ((lim_cur_val & LIM_MOTOR_UP_BIT) == LIM_MOTOR_UP_BIT)
            {
                lim_cur_val &= ~LIM_MOTOR_UP_BIT;
            }  
        }
        
        if (is_motor_down_limit())
        {
            if ((lim_cur_val & LIM_MOTOR_DOWN_BIT) == 0)
            {
                lim_cur_val |= LIM_MOTOR_DOWN_BIT;
            }
        }
        else
        {
            if ((lim_cur_val & LIM_MOTOR_DOWN_BIT) == LIM_MOTOR_DOWN_BIT)
            {
                lim_cur_val &= ~LIM_MOTOR_DOWN_BIT;
            }  
        }
        
        if (lim_cur_val != lim_last_val)
        {
            lim_last_val = lim_cur_val;
            vir_buzzer_tweet((TweetMode)1);
            lim_serv(lim_last_val);
            
        }
        
        OSTimeDly(5,
                  OS_OPT_TIME_DLY,
                  &err);
    }
}

void vir_limit_check_init(void)
{
    OS_ERR err;
    
    limit_init();
    
    OSTaskCreate(&limit_check_task_tcb,
                "limit_check task",
                limit_check_task,
                (void *)0,
                LIMIT_CHECK_TASK_PRIORITY,
                &limit_check_task_stk[0],
                (CPU_STK_SIZE)10,
                (CPU_STK_SIZE)(sizeof(limit_check_task_stk) / sizeof(limit_check_task_stk[0])),
                (OS_MSG_QTY   )LIMIT_CHECK_TASK_Q_SIZE,
                (OS_TICK      )0u,
                (void        *)0u,
                (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR      *)&err);  
            
}
