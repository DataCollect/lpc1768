#include "os.h"
#include "lpc17xx.h"

//#include "key_op.h"
//#include "in_key.h"
//#include "in_water_level.h"
//#include "in_motor_limit.h"

#include "user_system_init.h"
//#include "in_adc_op.h"

#include "vir.h"
#include "app_cfg.h"
#include "ts_serv_ex.h"

OS_TCB  start_task_tcb;
CPU_STK start_task_stk[200];

void start_task(void *p_arg)
{
    OS_ERR err;
    
    (void)(&p_arg);
    
    OS_CPU_SysTickInit(SystemCoreClock / OSCfg_TickRate_Hz);
    
    vir_init();
    init_user_system();
    
    while (1)
    {
        OSTimeDly(1000u,
                  OS_OPT_TIME_DLY,
                  &err);
        //serial0_send_byte('1');
        //dis_tst();
        //ts_send_byte('?');
    }
    
}

int main(void)
{
    //uint32_t loop = 1;
	//float tmp; 
    //Sensor_t ssr; 
	//ssr.unit = MM;
	//ssr.sensor = H2O_5;
    OS_ERR err;    
    
    OSInit(&err);
    OSTaskCreate(&start_task_tcb,
             "start_task",
             start_task,
             (void *)0,
             START_TASK_PRIORITY,
             &start_task_stk[0],
             10,
             sizeof(start_task_stk) / sizeof(start_task_stk[0]) ,
             4,
             0,
             (void *)0,
             OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR,
             &err);
    OSStart(&err);                                           /* Start multitasking. */
    
    (void)&err;
    
    return (0);
}
