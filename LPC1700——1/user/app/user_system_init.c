/**********************************************
 * @file user_system_init.c
 * @brief init the system 
 * @version 1.0
 * @data
 * @author
 ***********************************************/

#include "user_system_init.h"
//#include "init_out.h"
//#include "init_in.h"
#include "ts_serv.h"
//#include "sensor_test.h"
//#include "sys_cfg.h"
#include "sys_monitor.h"

void init_user_system(void)
{
   //init_in();
   //init_out();
   //sys_cfg_init();
   ts_serv_init();
   //sensor_test_init();
   //sys_monitor_init();
   //sys_cfg_init();   
}
