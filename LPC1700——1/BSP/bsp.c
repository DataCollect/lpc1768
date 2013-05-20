#include "lpc17xx.h"
#include "lpc_types.h"
#include "bsp.h"

// /*******************************************************************************
// * Function Name :void SysTickInit(void)
// * Description   :系统定时器时间配置
// * Input         :
// * Output        :
// * Other         :时基为1ms
// * Date          :2011.11.03  12:59:13
// *******************************************************************************/
// void SysTickInit(void)
// {
// 	SysTick_Config(SystemCoreClock / 1000);			//uCOS时基1ms
// }

// /*******************************************************************************
// * Function Name :void InterruptOrder(void)
// * Description   :中断向量，优先级
// * Input         :
// * Output        :
// * Other         :
// * Date          :2011.10.27  11:50:05
// *******************************************************************************/
// void NVIC_Configuration(void)
// {
// 	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4);//优先级设置  全为抢占式优先级
// }


/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :系统时间戳 初始化
* Input         :
* Output        :
* Other         :
* Date          :2012.6.15  13:14:59
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
}
#endif


/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :读时间戳 计数值
* Input         :读到的计数值
* Output        :
* Other         :
* Date          :2012.6.15  13:14:59
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return (SysTick->VAL);
}
#endif

