/************************************************************
 * @file lpc17xx_timer.c
 * @brief  contains all functions of timer peripheral
 * @version 1.0
 * @data 2010.6.3
 * @author 
 ************************************************************/
#include "lpc17xx_timer.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_cfg.h"

#ifdef _TIMER

static uint32_t getPClock(uint32_t timernum);
//static uint32_t convertUsecToVal(uint32_t timernum, uint32_t usec);
static uint32_t convertPtrToTimerNum(LPC_TIM_TypeDef *TIMx);

/****************************************************************
 * @brief get peripheral clock of each timer controller
 * @param[in] timernum: timer number
 * @return    Peripheral clock of timer
 ****************************************************************/
static uint32_t getPClock(uint32_t timernum)
{
    uint32_t clkdlycnt;

    switch (timernum)
    {
    case 0:
        clkdlycnt = CLKPWR_GetPCLKSEL(CLKPWR_PCLKSEL_TIMER0);
        break;
    case 1:
        clkdlycnt = CLKPWR_GetPCLKSEL(CLKPWR_PCLKSEL_TIMER1);
        break;
    case 2:
        clkdlycnt = CLKPWR_GetPCLKSEL(CLKPWR_PCLKSEL_TIMER2);
        break;
    case 3:
        clkdlycnt = CLKPWR_GetPCLKSEL(CLKPWR_PCLKSEL_TIMER3);
        break;
    }

    return clkdlycnt;
}

/*******************************************************
 * @brief convert a time to a timer count value
 * @param[in] timernum :timernum
 * @param[in] usec     :time in microseconds
 * @return  the number of required clock ticks to give 
            the time delay
 *******************************************************/
static uint32_t convertUSecToVal(uint32_t timernum, uint32_t usec)
{
    uint64_t clkdlycnt;

    clkdlycnt = (uint64_t)getPClock(timernum);

    clkdlycnt = (clkdlycnt * usec) / 1000000;

    return (uint32_t)clkdlycnt;
}

/**********************************************************
 * @brief convert a timer register to a timer number
 * @param[in] TIMx : pointer to LPC_TIM_TypeDef,
 * @return the timer number 
 **********************************************************/
static uint32_t convertPtrToTimerNum(LPC_TIM_TypeDef *TIMx)
{
    int32_t timernum = -1;

    if (TIMx == LPC_TIM0)
    {
        timernum = 0;
    }
    else if (TIMx == LPC_TIM1)
    {
        timernum = 1;
    }
    else if (TIMx == LPC_TIM2)
    {
        timernum = 2;
    }
    else if (TIMx == LPC_TIM3)
    {
        timernum = 3;
    }

    return timernum;
}

/******************************************************
 * @brief get interrupt status
 * @param[in] TIMx   : 0~3
 * @param[in] IntFlag: 
                        TIM_MR0_INT = 0,
                        TIM_MR1_INT = 1,
                        TIM_MR2_INT,
                        TIM_MR3_INT,
                        TIM_CR0_INT,
                        TIM_CR1_INT
 * @return FlagStatus :
                        SET   : interrupt
                        RESET : no interrupt
 ******************************************************/
FlagStatus TIM_GetIntStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag)
{
    uint8_t temp;

    temp = (TIMx->IR) & TIM_IR_CLR(IntFlag);
    if (temp)
        return SET;
    return RESET;
}

/*******************************************************
 * @brief  clear interrupt pending
 * @param[in] TIMx
 * @param[in] IntFlag
                        TIM_MR0_INT = 0,
                        TIM_MR1_INT = 1,
                        TIM_MR2_INT,
                        TIM_MR3_INT,
                        TIM_CR0_INT,
                        TIM_CR1_INT
 * @return    none
 *******************************************************/
void TIM_ClearIntPending(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag)
{
    TIMx->IR = TIM_IR_CLR(IntFlag);
}

/*******************************************************
 * @brief  clear capture interrupt pending
 * @param[in] TIMx
 * @param[in] IntFlag
                        TIM_MR0_INT = 0,
                        TIM_MR1_INT = 1,
                        TIM_MR2_INT,
                        TIM_MR3_INT,
                        TIM_CR0_INT,
                        TIM_CR1_INT
 * @return    none
 *******************************************************/
void TIM_ClearIntCapturePending(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag)
{
    TIMx->IR = (1 << (4 + IntFlag));
}

/*******************************************************
 * @brief configuration  for timer 
 * @param[in] TimerCounterMode : timer/counter mode
                                TIM_TIMER_MODE = 0,
                                TIM_COUNTER_RISING_MODE,
                                TIM_COUNTER_FALLING_MODE,
                                TIM_COUNTER_BOTH_MODE                          
 * @param[in] TIM_ConfigStruct pointer to TIM_TIMERCFG_Type
                               or TIM_COUNTERCFG_Type
 *
 *******************************************************/
void TIM_ConfigStructInit(TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct)
{
    if (TimerCounterMode == TIM_TIMER_MODE)
    {
        TIM_TIMERCFG_Type * pTimerCfg = (TIM_TIMERCFG_Type *)TIM_ConfigStruct;
        pTimerCfg->PrescaleOption = TIM_PRESCALE_USVAL;
        pTimerCfg->PrescaleValue  = 1;
    }
    else
    {
        TIM_COUNTERCFG_Type *pCounterCfg = (TIM_COUNTERCFG_Type *)TIM_ConfigStruct;
        pCounterCfg->CountInputSelect = TIM_COUNTER_INCAP0;
    }
}

/***************************************************************
 * @brief initialize timer/counter 
 * @param[in] TIMx
 * @param[in] TimerCounterMode
                                TIM_TIMER_MODE = 0,
                                TIM_COUNTER_RISING_MODE,
                                TIM_COUNTER_FALLING_MODE,
                                TIM_COUNTER_BOTH_MODE
                                   
 * @param[in] TIM_ConfigStruct
                                pointer to TIM_TIMCFG_Type
 * @return    none
 ***************************************************************/
void TIM_Init(LPC_TIM_TypeDef *TIMx, TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct)
{
    TIM_TIMERCFG_Type *pTimerCfg;
    TIM_COUNTERCFG_Type *pCounterCfg;
    
    if (TIMx == LPC_TIM0)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM0, ENABLE);
        CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_TIMER0, CLKPWR_PCLKSEL_CCLK_DIV_4);
    }
    else if (TIMx == LPC_TIM1)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM1, ENABLE);
        CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_TIMER1, CLKPWR_PCLKSEL_CCLK_DIV_4);
    }
    else if (TIMx == LPC_TIM2)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM2, ENABLE);
        CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_TIMER2, CLKPWR_PCLKSEL_CCLK_DIV_4);
    }
    else if (TIMx == LPC_TIM3)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM3, ENABLE);
        CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_TIMER3, CLKPWR_PCLKSEL_CCLK_DIV_4);
    }

    TIMx->CCR = (~TIM_CTCR_MODE_MASK);
    TIMx->CCR |= TIM_TIMER_MODE;

    TIMx->TC  = 0;
    TIMx->PC  = 0;
    TIMx->PR  = 0;
    TIMx->TCR |= (1 << 1);    /* reset counter */
    TIMx->TCR &= (~(1 << 1)); /* release reset */

    if (TimerCounterMode == TIM_TIMER_MODE)
    {
        pTimerCfg = (TIM_TIMERCFG_Type *)TIM_ConfigStruct;
        if (pTimerCfg->PrescaleOption == TIM_PRESCALE_TICKVAL)
        {
            TIMx->PR = pTimerCfg->PrescaleValue -1;
        }
        else
        {
            TIMx->PR = convertUSecToVal(convertPtrToTimerNum(TIMx), 
                       pTimerCfg->PrescaleValue)-1;
        }
    }             
    else
    {
        pCounterCfg = (TIM_COUNTERCFG_Type *)TIM_ConfigStruct;
        TIMx->CCR &= ~TIM_CTCR_INPUT_MASK;
        if (pCounterCfg->CountInputSelect == TIM_COUNTER_INCAP1)
            TIMx->CCR |= _BIT(2);
    }

    TIMx->IR = 0xFFFFFFFF;  
}

/****************************************************************
 * @brief close timer/counter 
 * @param[in] TIMx
 * @return    none
 *
 ****************************************************************/
void TIM_DeInit(LPC_TIM_TypeDef *TIMx)
{
    TIMx->TCR = 0x00; /* disable timer/counter*/

    if (TIMx == LPC_TIM0) /* disable power */
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM0, DISABLE);    
    }
    else if (TIMx == LPC_TIM1)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM1, DISABLE);    
    }
    else if (TIMx == LPC_TIM2)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM2, DISABLE);    
    }
    else if (TIMx == LPC_TIM3)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCTIM3, DISABLE);    
    }
}

/********************************************************
 * @brief start/stop timer/counter
 * @param[in] TIMx
 * @param[in] NewState:
                      ENABLE : enable the timer
                      DISABLE: disable the timer

 * @return    none
 ********************************************************/
void TIM_Cmd(LPC_TIM_TypeDef *TIMx, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        TIMx->TCR |= TIM_ENABLE;
    }
    else
    {
        TIMx->TCR &= ~TIM_ENABLE;
    }
}

/*********************************************************
 *  @brief reset timer/counter
 *  @param[in] TIMX
 *  @return     none
 *********************************************************/
void TIM_ResetCounter(LPC_TIM_TypeDef *TIMx)
{
    TIMx->TCR |= TIM_RESET;
    TIMx->TCR &= ~TIM_RESET;
}

/**********************************************************
 *  @breif configuration for match register
 *  @param[in] TIMx
 *  @param[in] TIM_MatchConfigStruct: pointer to TIM_MACTHCFG_Type
 *  @return    none
 **********************************************************/
void TIM_ConfigMatch(LPC_TIM_TypeDef *TIMx, TIM_MATCHCFG_Type *TIM_MatchConfigStruct)
{
    switch (TIM_MatchConfigStruct->MatchChannel)
    {
        case 0:
            TIMx->MR0 = TIM_MatchConfigStruct->MatchValue;
            break;
        case 1:
            TIMx->MR1 = TIM_MatchConfigStruct->MatchValue;
            break;
        case 2:
            TIMx->MR2 = TIM_MatchConfigStruct->MatchValue;
            break;
        case 3:
            TIMx->MR3 = TIM_MatchConfigStruct->MatchValue;
            break;
    }

    TIMx->MCR &= ~TIM_MCR_CHANNEL_MASKBIT(TIM_MatchConfigStruct->MatchChannel);
    if (TIM_MatchConfigStruct->IntOnMatch)
		TIMx->MCR |= TIM_INT_ON_MATCH(TIM_MatchConfigStruct->MatchChannel);

	/* reset on MRn */
	if (TIM_MatchConfigStruct->ResetOnMatch)
		TIMx->MCR |= TIM_RESET_ON_MATCH(TIM_MatchConfigStruct->MatchChannel);

	/* stop on MRn */
	if (TIM_MatchConfigStruct->StopOnMatch)
		TIMx->MCR |= TIM_STOP_ON_MATCH(TIM_MatchConfigStruct->MatchChannel);

    TIMx->EMR 	&= ~TIM_EM_MASK(TIM_MatchConfigStruct->MatchChannel);
	TIMx->EMR   |= TIM_EM_SET(TIM_MatchConfigStruct->MatchChannel, \
                   TIM_MatchConfigStruct->ExtMatchOutputType);
}

/*********************************************************************
 * @brief update match value
 * @param[in] TIMx
 * @param[in] MatchChannel
 * @param[in] MatchValue
 * @return    none
 *********************************************************************/
void TIM_UpdateMatchValue(LPC_TIM_TypeDef *TIMx,uint8_t MatchChannel, uint32_t MatchValue)
{
    switch (MatchChannel)
    {
    case 0:
        TIMx->MR0 = MatchValue ;
        break;
    case 1:
        TIMx->MR1 = MatchValue ;
        break; 
    case 2:
        TIMx->MR2 = MatchValue ;
        break; 
    case 3:
        TIMx->MR3 = MatchValue ;
        break;   
    }
}

/*****************************************************
 * @brief configuration for capture register
 * @param[in] TIMx
 * @param[in] TIM_CaptureConfigStruct: pointer to TIM_CAPTURECFG_Type        
 * @return    none
 *****************************************************/
void TIM_ConfigCapture(LPC_TIM_TypeDef *TIMx, TIM_CAPTURECFG_Type *TIM_CaptureConfigStruct)
{
    TIMx->CCR &= ~TIM_CCR_CHANNEL_MASKBIT(TIM_CaptureConfigStruct->CaptureChannel);

	if (TIM_CaptureConfigStruct->RisingEdge)
		TIMx->CCR |= TIM_CAP_RISING(TIM_CaptureConfigStruct->CaptureChannel);

	if (TIM_CaptureConfigStruct->FallingEdge)
		TIMx->CCR |= TIM_CAP_FALLING(TIM_CaptureConfigStruct->CaptureChannel);

	if (TIM_CaptureConfigStruct->IntOnCaption)
		TIMx->CCR |= TIM_INT_ON_CAP(TIM_CaptureConfigStruct->CaptureChannel);
}

/******************************************************************************
 * @brief read value of capture register
 * @param[in] TIMx
 * @param[in] CaptureChannel:
                            TIM_COUNTER_INCAP0
                            TIM_COUNTER_INCAP1
 * @return    value of capture register
 ******************************************************************************/
uint32_t TIM_GetCaptureValue(LPC_TIM_TypeDef *TIMx, TIM_COUNTER_INPUT_OPT CaptureChannel)
{
    if (CaptureChannel == 0)
        return (TIMx->CR0);
    else
        return (TIMx->CR1);
}


#endif
