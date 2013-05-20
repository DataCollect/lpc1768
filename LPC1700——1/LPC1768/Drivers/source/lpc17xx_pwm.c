/****************************************************************
 * @file   lpc17xx_pwm.c
 * @brief  contains all functions about pwm
 * @version 1.0
 * @data    2010.6.4
 * @author
 ****************************************************************/
#include "lpc17xx_pwm.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_cfg.h"

#ifdef _PWM

/***************************************************************
 * @brief check whether specified interrupt flag in pwm is set or not
 * @param[in] PWMx : 
 * @param[in] IntFlag : pwm interrupt flag, should be :
 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
 * @return    New state of pwm interrupt flag (SET or RESET)
 *****************************************************************/
IntStatus PWM_GetIntStatus(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag)
{
   return ((PWMx->IR & IntFlag) ? SET : RESET);   
}

/**********************************************************************
 * @brief clear the interrupt flag 
 * @param[in] PWMx
 * @param[in] IntFlag : pwm interrupt flag ,should be :
 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
 *
 * @return none
 *********************************************************************/
void PWM_ClearIntPending(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag)
{
    PWMx->IR = IntFlag; /*write 1 reset the interrupt,write 0 has no effect */    
}

/*********************************************************************
 * @brief Fills each PWM_InitStruct member with its default value:
 * @param[in]  PWMTimerCounterMode Timer or Counter mode 
                  PWM_MODE_TIMER = 0, 
                  PWM_MOD_COUNTER     
 * @param[in]  PWM_InitStruct : pointer to srtucture
 * @return     none
 ********************************************************************/
void PWM_ConfigStructInit(uint8_t PWMTimerCounterMode, void *PWM_InitStruct)
{
    PWM_TIMERCFG_Type *pTimerCfg; 
    
    pTimerCfg = (PWM_TIMERCFG_Type *)PWM_InitStruct;
    //pCounterCfg = (PWM_COUNTERCFG_Typ *)PWM_InitStruct; 
    
    if (PWMTimerCounterMode == PWM_MODE_TIMER)
    {
        pTimerCfg->PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
        pTimerCfg->PrescaleValue  = 1;
    }
      
}

/*********************************************************************
 * @brief Intializes the PWMx
 * @param[in] PWMx
 * @param[in] PWMTinerCounterMode : Timer or Counter mode , 
              PWM_MODE_TIMER
              PWM_MODE_COUNTER
 * @param[in] PWM_ConfigStruct : pointer to structure which will be initialized 
 * @return    none
 *********************************************************************/
void PWM_Init(LPC_PWM_TypeDef *PWMx, uint32_t PWMTimerCounterMode, void *PWM_ConfigStruct)
{
    PWM_TIMERCFG_Type *pTimerCfg;
    uint64_t clkdlycnt;

    pTimerCfg = (PWM_TIMERCFG_Type *)PWM_ConfigStruct;
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCPWM1, ENABLE);
    CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_PWM1, CLKPWR_PCLKSEL_CCLK_DIV_4);

    clkdlycnt = (uint64_t)CLKPWR_GetPCLKSEL(CLKPWR_PCLKSEL_PWM1);

    PWMx->IR = 0xFF & PWM_IR_BITMASK;
    PWMx->TCR = 0x00;
    PWMx->CTCR = 0x00;/* is used to select between timer and counter mode */
    PWMx->MCR = 0x00; /* pwm match control register */
    PWMx->CCR = 0x00; /* pwm capture control register */
    PWMx->PCR = 0x00; /* is used to enable and select the type of each pwm channel */
    PWMx->LER = 0x00; /* aloows the last value written to pwm match register to be effective */

    if (PWMTimerCounterMode == PWM_MODE_TIMER)
    {
        /* Absolute prescale value */
		if (pTimerCfg->PrescaleOption == PWM_TIMER_PRESCALE_TICKVAL)
		{
			PWMx->PR   = pTimerCfg->PrescaleValue - 1;
		}
		/* uSecond prescale value */
		else
		{
			clkdlycnt = (clkdlycnt * pTimerCfg->PrescaleValue) / 1000000;
			PWMx->PR = ((uint32_t) clkdlycnt) - 1;
		}
    }
}

/*******************************************************************
 * @brief De-initializes the PWM 
 * @param[in] PWMx
 * @return    none
 *******************************************************************/
void PWM_DeInit(LPC_PWM_TypeDef *PWMx)
{
    PWMx->TCR = 0x00;
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCPWM1, DISABLE);
}

/*******************************************************
 * @brief ENABLE/DISABLE PWM
 * @param[in] PWMx
 * @param[in] NewState :
                        ENABLE
                        DISABLE
 *
 * @return    none
 ********************************************************/
void PWM_Cmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        PWMx->TCR |= PWM_TCR_PWM_ENABLE;
    }
    else
    {
        PWMx->TCR &= ((~PWM_TCR_PWM_ENABLE) & PWM_TCR_BITMASK);    
    }
}

/****************************************************************
 * @brief configures match 
 * @param[in] PWMx
 * @param[in] PWM_MatchConfigStruct : pointer to a PWM_MATCHCFG_Type
              structure that contains the cinfiguration infirmation
              for the specified PWM match function
 * @return    none
 ****************************************************************/
void PWM_ConfigMatch(LPC_PWM_TypeDef *PWMx, PWM_MATCHCFG_Type *PWM_MatchConfigStruct)
{
    if (PWM_MatchConfigStruct->IntOnMatch == ENABLE)   //interrupt on match
    {
        PWMx->MCR |= PWM_MCR_INT_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
    }
    else
    {
        PWMx->MCR &= (~PWM_MCR_INT_ON_MATCH(PWM_MatchConfigStruct->MatchChannel))
					 & PWM_MCR_BITMASK;
    }

    //reset on MRn
	if (PWM_MatchConfigStruct->ResetOnMatch == ENABLE)
	{
		PWMx->MCR |= PWM_MCR_RESET_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		PWMx->MCR &= (~PWM_MCR_RESET_ON_MATCH(PWM_MatchConfigStruct->MatchChannel))
				     & PWM_MCR_BITMASK;
	}

	//stop on MRn
	if (PWM_MatchConfigStruct->StopOnMatch == ENABLE)
	{
		PWMx->MCR |= PWM_MCR_STOP_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		PWMx->MCR &= (~PWM_MCR_STOP_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) 
					& PWM_MCR_BITMASK;
	}
}

/**********************************************************************************
 * @brief update value for pwm channel with update type option 
 * @param[in] PWMx
 * @param[in] MatchChannel Match channel
 * @param[in] MatchValue : Match value
 * @return    none
 *********************************************************************************/
void PWM_MatchUpdate(LPC_PWM_TypeDef *PWMx, uint8_t MatchChannel, uint32_t MatchValue)
{
    switch (MatchChannel)
    {
    case 0:
        PWMx->MR0 = MatchValue;
        break;
    case 1:
        PWMx->MR1 = MatchValue;
        break;
    case 2:
        PWMx->MR2 = MatchValue;
        break;
    case 3:
        PWMx->MR3 = MatchValue;
        break;
    case 4:
        PWMx->MR4 = MatchValue;
        break;
    case 5:
        PWMx->MR5 = MatchValue;
        break;
    case 6:
        PWMx->MR6 = MatchValue;
        break;

    }

    PWMx->LER |= PWM_LER_EN_MATCHn_LATCH(MatchChannel);
}

/*************************************************************
 * @brief configure Edge mode for each channel
 * @param[in] PWMx 
 * @param[in] PWMChannel: pwm channel: 2 to 6
 * @param[in] ModeOption: pwm mode option
                          PWM_CHANNEL_SINGLE_EDGE
                          PWM_CHANNEL_DUAL
 * @return    none
 ************************************************************/
void PWM_ChannelConfig(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, uint8_t ModeOption)
{
    if (ModeOption == PWM_CHANNEL_SINGLE_EDGE)      //single edge mode
    {
       PWMx->PCR &=((~PWM_PCR_PWMSELn(PWMChannel)) & (PWM_PCR_BITMASK));
        //PWMx->PCR |= PWM_PCR_PWMSELn(PWMChannel);
    }
    else                                          //double edge mode
    {
        PWMx->PCR |= PWM_PCR_PWMSELn(PWMChannel);
    }
}

/***************************************************************
 * @brief Enable/Disable PWM channel output
 * @param[in] PWMx
 * @param[in] PWMChannel: pwm channel:1 to 6
 * @param[in] NewState  : new state of this function:
                          ENABLE
                          DISABLE
 * @return    none
 ***************************************************************/
void PWM_ChannelCmd(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        PWMx->PCR |= PWM_PCR_PWMENAn(PWMChannel);
    }
    else
    {
        PWMx->PCR &= ((~PWM_PCR_PWMENAn(PWMChannel)) & (PWM_PCR_BITMASK));
    }
}

/*********************************************************************
 * @brief 		Enable/Disable Counter in PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	NewState New State of this function:
 * 							- ENABLE: Enable Counter in PWM peripheral
 * 							- DISABLE: Disable Counter in PWM peripheral
 * @return 		None
 **********************************************************************/
void PWM_CounterCmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState)
{
	
	if (NewState == ENABLE)
	{
		PWMx->TCR	|=  PWM_TCR_COUNTER_ENABLE;
	}
	else
	{
		PWMx->TCR &= (~PWM_TCR_COUNTER_ENABLE) & PWM_TCR_BITMASK;
	}
}

/*********************************************************************
 * @brief 		Reset Counter in PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @return 		None
 **********************************************************************/
void PWM_ResetCounter(LPC_PWM_TypeDef *PWMx)
{
	PWMx->TCR |= PWM_TCR_COUNTER_RESET;
	PWMx->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
}

#endif
