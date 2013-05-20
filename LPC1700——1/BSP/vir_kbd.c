#include "vir_kbd.h"
#include "in_key.h"

#include "vir_buzzer.h"
#include "motor_ctl.h"

#include "os.h"
#include "app_cfg.h"

#include "lpc_types.h"

/*
short : start
two short : up
long : down
*/
OS_TCB  kbd_task_tcb;
CPU_STK kbd_task_stk[200];

static KBD_SERV_FUNC kbd_serv = NULL;
void kbd_serv_register(KBD_SERV_FUNC serv)
{
    kbd_serv = serv;
}

// 0:          0代表无键按下
//             1: start
//             2: up
//             3: down

void kbd_task(void *p_arg)
{
    OS_ERR err;
    uint32_t old_kbd_val = 255;
    uint32_t kbd_val;
    //uint32_t msg;
    (void)&p_arg;
    
    while (1)
    {
        kbd_val = read_kbd();
        if (kbd_val != 0)
        {
            if (kbd_val != old_kbd_val)
            {
                OSTimeDly(100,
                          OS_OPT_TIME_DLY,
                          &err);
                kbd_val = read_kbd();
                if (kbd_val != 0)
                {
                    old_kbd_val = kbd_val;
                    vir_buzzer_tweet((TweetMode)kbd_val);
                    kbd_serv(kbd_val);
                    //motor_ctl((CtlCmd)kbd_val);
                }
            }
        }
        else
        {
            old_kbd_val = 255;
        }
        OSTimeDly(10,
                  OS_OPT_TIME_DLY,
                  &err);
    }
}



void vir_kbd_init(void)
{
    OS_ERR err;
    
    OSTaskCreate(&kbd_task_tcb,
                "kbd task",
                kbd_task,
                (void *)0,
                KBD_TASK_PRIORITY,
                &kbd_task_stk[0],
                (CPU_STK_SIZE)10,
                (CPU_STK_SIZE)(sizeof(kbd_task_stk) / sizeof(kbd_task_stk[0])),
                (OS_MSG_QTY   )KBD_TASK_Q_SIZE,
                (OS_TICK      )0u,
                (void        *)0u,
                (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR      *)&err);
    kbd_init();
    
}
