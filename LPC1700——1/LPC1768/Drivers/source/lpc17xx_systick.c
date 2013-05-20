/***********************************************************************
 * @file lpc17xx_systick.c
 * @brief contains all functions about system timer
 * @version 1.0
 * @data 2010.6.3
 * @author 
 **********************************************************************/
 #include "lpc17xx_systick.h"
 #include "lpc17xx_clkpwr.h"
 #include "lpc17xx_cfg.h"

#ifdef _SYSTICK

/*********************************************************************
 * @brief initial system tick using internal cpu clock source
 * @param[in] time: time interval(ms)
 * @return    none
 *
 *********************************************************************/
void SYSTICK_InternalInit(uint32_t time)
{
    uint32_t cclk;
    float maxtime;

    cclk = SystemCoreClock;
    maxtime = (1 << 24) / (SystemCoreClock / 1000);

    SysTick->CTRL |= STCTRL_CLKSOURCE;
    if (time <= maxtime)
    {
        /* RELOAD = (SystemCoreClock/1000) * time -1 */
        SysTick->LOAD  = (cclk / 1000) * time - 1;
    }
    else
    {
        /* RELOAD = (SystemCoreClock/1000) * time -1 */
        SysTick->LOAD  = (cclk / 1000) * 10 - 1;    
    }
}

/*******************************************************
 * @brief enable/disable system timer
 * @param[in] NewState :system tick counter status
                        ENABLE
                        DISABLE
 * @return    none
 *
 *******************************************************/
void SYSTICK_Cmd(FunctionalState NewState)
{
    if (NewState == ENABLE)
        SysTick->CTRL |= STCTRL_ENABLE;
    else
        SysTick->CTRL &= ~STCTRL_ENABLE; 
}

/*******************************************************
 * @brief enable/disable system tick interrupt
 * @param[in] NewState : System tick interrupt status
                         ENABLE
                         DISABLE 
 * @return    none
 ******************************************************/
void SYSTICK_IntCmd(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        SysTick->CTRL |= STCTRL_TICKINT;
    }
    else
    {
        SysTick->CTRL &= ~STCTRL_TICKINT;
    }
}

/**************************************************
 * @brief get current value of system timer
 * @param[in]  none
 * @return     current value of system Tick Counter
 *************************************************/
uint32_t SYSTICK_GetCurrentValue(void)
{
    return (SysTick->VAL);
}

/***************************************************
 * @brief clear counter flag
 * @param[in]  none
 * @return     none
 **************************************************/
void SYSTICK_ClearCounterFlag(void)
{
    SysTick->CTRL &= ~STCTRL_COUNTFLAG;
}



#endif
