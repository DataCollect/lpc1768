#ifndef __APP_CFG_H__
#define __APP_CFG_H__


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

//#define  APP_CFG_TASK_START_PRIO                           2u


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

//#define  APP_CFG_TASK_START_STK_SIZE                     128u


/*
*********************************************************************************************************
*                                          TASK STACK SIZES LIMIT
*********************************************************************************************************
*/

//#define  APP_CFG_TASK_START_STK_SIZE_PCT_FULL             90u

//#define  APP_CFG_TASK_START_STK_SIZE_LIMIT       (APP_CFG_TASK_START_STK_SIZE * (100u - APP_CFG_TASK_START_STK_SIZE_PCT_FULL))   / 100u


//#define  OS_CFG_STAT_TASK_PRIO            28u 
//#define  OS_CFG_TICK_TASK_PRIO            10u 
//#define  OS_CFG_TMR_TASK_PRIO             25u 

#define START_TASK_PRIORITY       2u
#define BUZZER_TASK_PRIORITY      14u
#define KBD_TASK_PRIORITY         13u
#define LIMIT_CHECK_TASK_PRIORITY 11U  //limit_check_task
#define MOTOR_CTL_TASK_PRIORITY   7U
#define SENSOR_TEST_TASK_PRIORITY 5u
#define TS_SERV_TASK_PRIORITY     4U
#define SYS_MONITOR_TASK_PRIORITY 6u

#define BUZZER_TASK_Q_SIZE        8u
#define KBD_TASK_Q_SIZE           4u
#define LIMIT_CHECK_TASK_Q_SIZE   4U  //limit_check_task
#define MOTOR_CTL_TASK_Q_SIZE     4U
#define SENSOR_TEST_TASK_Q_SIZE   51u
#define TS_SERV_TASK_Q_SIZE       70u
#define SYS_MONITOR_TASK_Q_SIZE   16u

#include "lib_cfg.h"


#endif
