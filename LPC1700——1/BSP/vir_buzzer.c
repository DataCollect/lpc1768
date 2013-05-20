#include "os.h"
#include "out_beep.h"
#include "vir_buzzer.h"
#include "lpc_types.h"
#include "app_cfg.h"

OS_TCB  buzzer_task_tcb;
CPU_STK buzzer_task_stk[200];



//uint32_t buzzer_mode;
/*
*/
void buzzer_task(void *p_arg)
{
    void *p_msg;
    OS_MSG_SIZE msg_size;
    CPU_TS ts;
    OS_ERR err;
    uint32_t mode;
    
    (void)&p_arg;
    
    beep_off();
    //beep_on();
    
    while (1)
    {
        p_msg = OSTaskQPend(0,
                            OS_OPT_PEND_BLOCKING,
                            &msg_size,
                            &ts,
                            &err);
        mode = (uint32_t)p_msg; //(*((uint32_t *)p_msg));
        
        //mode = 0;
        if (err == OS_ERR_NONE)
        {
            switch(mode)
            {
            case VIR_BUZZER_SHORT:
                beep_on();
                OSTimeDly(200,
                          OS_OPT_TIME_DLY,
                          &err);
            
                beep_off();
                break;
            case VIR_BUZZER_TWO_SHORT:
                beep_on();
                OSTimeDly(50,
                          OS_OPT_TIME_DLY,
                          &err);
                beep_off();
                OSTimeDly(50,
                          OS_OPT_TIME_DLY,
                          &err);
                beep_on();
                OSTimeDly(50,
                          OS_OPT_TIME_DLY,
                          &err);
                beep_off();
                break;
            case VIR_BUZZER_LONG:
                beep_on();
                OSTimeDly(200,
                          OS_OPT_TIME_DLY,
                          &err);
                beep_off();
                break;
            default:
                
                break;
    
            }
        }
            
    }
}

void vir_buzzer_init(void)
{
    OS_ERR err;
    
    OSTaskCreate(&buzzer_task_tcb,
                "buzzer task",
                buzzer_task,
                (void *)0,
                BUZZER_TASK_PRIORITY,
                &buzzer_task_stk[0],
                (CPU_STK_SIZE)10,
                (CPU_STK_SIZE)(sizeof(buzzer_task_stk) / sizeof(buzzer_task_stk[0])),
                (OS_MSG_QTY   )BUZZER_TASK_Q_SIZE,
                (OS_TICK      )0u,
                (void        *)0u,
                (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR      *)&err);
    
    beep_init();
  
                 
}

void vir_buzzer_tweet(TweetMode mode)
{
    OS_ERR err;
    
    //buzzer_mode = mode;
    
    OSTaskQPost(&buzzer_task_tcb,
               (void *)mode,
               4,
               OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED,
               &err);
                
}
