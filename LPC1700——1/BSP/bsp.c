#include "lpc17xx.h"
#include "lpc_types.h"
#include "bsp.h"

// /*******************************************************************************
// * Function Name :void SysTickInit(void)
// * Description   :ϵͳ��ʱ��ʱ������
// * Input         :
// * Output        :
// * Other         :ʱ��Ϊ1ms
// * Date          :2011.11.03  12:59:13
// *******************************************************************************/
// void SysTickInit(void)
// {
// 	SysTick_Config(SystemCoreClock / 1000);			//uCOSʱ��1ms
// }

// /*******************************************************************************
// * Function Name :void InterruptOrder(void)
// * Description   :�ж����������ȼ�
// * Input         :
// * Output        :
// * Other         :
// * Date          :2011.10.27  11:50:05
// *******************************************************************************/
// void NVIC_Configuration(void)
// {
// 	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4);//���ȼ�����  ȫΪ��ռʽ���ȼ�
// }


/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :ϵͳʱ��� ��ʼ��
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
* Description   :��ʱ��� ����ֵ
* Input         :�����ļ���ֵ
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

