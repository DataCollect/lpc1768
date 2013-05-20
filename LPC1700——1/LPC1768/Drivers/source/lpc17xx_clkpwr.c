/*****************************************************************
 * @file      lpc17xx_clkpwr.c
 * @brief     contains all functions support for clock          
 *            and aontrol 
 * @version   0.0
 * @data      2010.5.21
 * @author    
 *****************************************************************/
#include "lpc17xx_clkpwr.h"
/*****************************************************************
 * @brief Set value of each Peripheral clock selection
 * @param[in] ClkType peripheral clock slection of each Type
 *              - CLKPWR_PCLKSEL_WDT   		: WDT
				- CLKPWR_PCLKSEL_TIMER0   	: Timer 0
				- CLKPWR_PCLKSEL_TIMER1   	: Timer 1
				- CLKPWR_PCLKSEL_UART0   	: UART 0
				- CLKPWR_PCLKSEL_UART1  	: UART 1
				- CLKPWR_PCLKSEL_PWM1   	: PWM 1
				- CLKPWR_PCLKSEL_I2C0   	: I2C 0
				- CLKPWR_PCLKSEL_SPI   		: SPI
				- CLKPWR_PCLKSEL_SSP1   	: SSP 1
				- CLKPWR_PCLKSEL_DAC   		: DAC
				- CLKPWR_PCLKSEL_ADC   		: ADC
				- CLKPWR_PCLKSEL_CAN1  		: CAN 1
				- CLKPWR_PCLKSEL_CAN2  		: CAN 2
				- CLKPWR_PCLKSEL_ACF   		: ACF
				- CLKPWR_PCLKSEL_QEI 		: QEI
				- CLKPWR_PCLKSEL_PCB   		: PCB
				- CLKPWR_PCLKSEL_I2C1   	: I2C 1
				- CLKPWR_PCLKSEL_SSP0   	: SSP 0
				- CLKPWR_PCLKSEL_TIMER2   	: Timer 2
				- CLKPWR_PCLKSEL_TIMER3   	: Timer 3
				- CLKPWR_PCLKSEL_UART2   	: UART 2
				- CLKPWR_PCLKSEL_UART3   	: UART 3
				- CLKPWR_PCLKSEL_I2C2   	: I2C 2
				- CLKPWR_PCLKSEL_I2S   		: I2S
				- CLKPWR_PCLKSEL_RIT   		: RIT
				- CLKPWR_PCLKSEL_SYSCON   	: SYSCON
				- CLKPWR_PCLKSEL_MC 		: MC
 * @param[in] Div value of divider
 *              - CLKPWR_PCLKSEL_CCLK_DIV_4 : PCLK_peripheral = CCLK/4
 * 				- CLKPWR_PCLKSEL_CCLK_DIV_1 : PCLK_peripheral = CCLK/1
 *				- CLKPWR_PCLKSEL_CCLK_DIV_2 : PCLK_peripheral = CCLK/2
 * @return none
 *****************************************************************/
void CLKPWR_SetPCLKDiv(uint32_t ClkType, uint32_t DivVal)
{
    uint32_t bitpos;

    bitpos = (ClkType < 32) ? (ClkType): (ClkType - 32);

    /* pclksel0 register */
    if (ClkType < 32)
    {
        LPC_SC->PCLKSEL0 &= (~(CLKPWR_PCLKSEL_BITMASK(bitpos)));
        LPC_SC->PCLKSEL0 |= (CLKPWR_PCLKSEL_SET(bitpos, DivVal));    
    }
    /* pclsel1 register */
    else
    {
        LPC_SC->PCLKSEL1 &= (~(CLKPWR_PCLKSEL_BITMASK(bitpos)));
        LPC_SC->PCLKSEL1 |= (CLKPWR_PCLKSEL_SET(bitpos, DivVal));

    }
}

/****************************************************************
 * @brief get current value of each peripheral 
 * @param[in] ClkType peripheral clock selection of each type
 *              - CLKPWR_PCLKSEL_WDT   		: WDT
				- CLKPWR_PCLKSEL_TIMER0   	: Timer 0
				- CLKPWR_PCLKSEL_TIMER1   	: Timer 1
				- CLKPWR_PCLKSEL_UART0   	: UART 0
				- CLKPWR_PCLKSEL_UART1  	: UART 1
				- CLKPWR_PCLKSEL_PWM1   	: PWM 1
				- CLKPWR_PCLKSEL_I2C0   	: I2C 0
				- CLKPWR_PCLKSEL_SPI   		: SPI
				- CLKPWR_PCLKSEL_SSP1   	: SSP 1
				- CLKPWR_PCLKSEL_DAC   		: DAC
				- CLKPWR_PCLKSEL_ADC   		: ADC
				- CLKPWR_PCLKSEL_CAN1  		: CAN 1
				- CLKPWR_PCLKSEL_CAN2  		: CAN 2
				- CLKPWR_PCLKSEL_ACF   		: ACF
				- CLKPWR_PCLKSEL_QEI 		: QEI
				- CLKPWR_PCLKSEL_PCB   		: PCB
				- CLKPWR_PCLKSEL_I2C1   	: I2C 1
				- CLKPWR_PCLKSEL_SSP0   	: SSP 0
				- CLKPWR_PCLKSEL_TIMER2   	: Timer 2
				- CLKPWR_PCLKSEL_TIMER3   	: Timer 3
				- CLKPWR_PCLKSEL_UART2   	: UART 2
				- CLKPWR_PCLKSEL_UART3   	: UART 3
				- CLKPWR_PCLKSEL_I2C2   	: I2C 2
				- CLKPWR_PCLKSEL_I2S   		: I2S
				- CLKPWR_PCLKSEL_RIT   		: RIT
				- CLKPWR_PCLKSEL_SYSCON   	: SYSCON
				- CLKPWR_PCLKSEL_MC 		: MC
 * @return value of selected peripheral clock selection 
 ****************************************************************/
uint32_t CLKPWR_GetPCLKSEL(uint32_t ClkType)
{
    uint32_t bitpos, retval;

    if (ClkType < 32)
    {
        bitpos = ClkType;
        retval = LPC_SC->PCLKSEL0;
    }
    else
    {
        bitpos = ClkType - 32;
        retval = LPC_SC->PCLKSEL1;
    }

    retval = CLKPWR_PCLKSEL_GET(bitpos, retval);
    return retval;
}

/*********************************************************************//**
 * @brief 		Get current value of each Peripheral Clock
 * @param[in]	ClkType	Peripheral Clock Selection of each type,
 * 				should be one of the following:
 *				- CLKPWR_PCLKSEL_WDT   		: WDT
				- CLKPWR_PCLKSEL_TIMER0   	: Timer 0
				- CLKPWR_PCLKSEL_TIMER1   	: Timer 1
				- CLKPWR_PCLKSEL_UART0   	: UART 0
				- CLKPWR_PCLKSEL_UART1  	: UART 1
				- CLKPWR_PCLKSEL_PWM1   	: PWM 1
				- CLKPWR_PCLKSEL_I2C0   	: I2C 0
				- CLKPWR_PCLKSEL_SPI   		: SPI
				- CLKPWR_PCLKSEL_SSP1   	: SSP 1
				- CLKPWR_PCLKSEL_DAC   		: DAC
				- CLKPWR_PCLKSEL_ADC   		: ADC
				- CLKPWR_PCLKSEL_CAN1  		: CAN 1
				- CLKPWR_PCLKSEL_CAN2  		: CAN 2
				- CLKPWR_PCLKSEL_ACF   		: ACF
				- CLKPWR_PCLKSEL_QEI 		: QEI
				- CLKPWR_PCLKSEL_PCB   		: PCB
				- CLKPWR_PCLKSEL_I2C1   	: I2C 1
				- CLKPWR_PCLKSEL_SSP0   	: SSP 0
				- CLKPWR_PCLKSEL_TIMER2   	: Timer 2
				- CLKPWR_PCLKSEL_TIMER3   	: Timer 3
				- CLKPWR_PCLKSEL_UART2   	: UART 2
				- CLKPWR_PCLKSEL_UART3   	: UART 3
				- CLKPWR_PCLKSEL_I2C2   	: I2C 2
				- CLKPWR_PCLKSEL_I2S   		: I2S
				- CLKPWR_PCLKSEL_RIT   		: RIT
				- CLKPWR_PCLKSEL_SYSCON   	: SYSCON
				- CLKPWR_PCLKSEL_MC 		: MC

 * @return		Value of Selected Peripheral Clock
 **********************************************************************/
uint32_t CLKPWR_GetPCLK (uint32_t ClkType)
{
	uint32_t retval, div;

	retval = SystemCoreClock;
	div = CLKPWR_GetPCLKSEL(ClkType);

	switch (div)
	{
	case 0:
		div = 4;
		break;

	case 1:
		div = 1;
		break;

	case 2:
		div = 2;
		break;

	case 3:
		div = 8;
		break;
	}
	retval /= div;

	return retval;
}

/*****************************************************************
 * @brief config power supply for each peripheral
 * @param[in] PType
 *            Type of peripheral used to enable power
 *              -  CLKPWR_PCONP_PCTIM0 		: Timer 0
				-  CLKPWR_PCONP_PCTIM1 		: Timer 1
				-  CLKPWR_PCONP_PCUART0  	: UART 0
				-  CLKPWR_PCONP_PCUART1   	: UART 1
				-  CLKPWR_PCONP_PCPWM1 		: PWM 1
				-  CLKPWR_PCONP_PCI2C0 		: I2C 0
				-  CLKPWR_PCONP_PCSPI   	: SPI
				-  CLKPWR_PCONP_PCRTC   	: RTC
				-  CLKPWR_PCONP_PCSSP1 		: SSP 1
				-  CLKPWR_PCONP_PCAD   		: ADC
				-  CLKPWR_PCONP_PCAN1   	: CAN 1
				-  CLKPWR_PCONP_PCAN2   	: CAN 2
				-  CLKPWR_PCONP_PCGPIO 		: GPIO
				-  CLKPWR_PCONP_PCRIT 		: RIT
				-  CLKPWR_PCONP_PCMC 		: MC
				-  CLKPWR_PCONP_PCQEI 		: QEI
				-  CLKPWR_PCONP_PCI2C1   	: I2C 1
				-  CLKPWR_PCONP_PCSSP0 		: SSP 0
				-  CLKPWR_PCONP_PCTIM2 		: Timer 2
				-  CLKPWR_PCONP_PCTIM3 		: Timer 3
				-  CLKPWR_PCONP_PCUART2  	: UART 2
				-  CLKPWR_PCONP_PCUART3   	: UART 3
				-  CLKPWR_PCONP_PCI2C2 		: I2C 2
				-  CLKPWR_PCONP_PCI2S   	: I2S
				-  CLKPWR_PCONP_PCGPDMA   	: GPDMA
				-  CLKPWR_PCONP_PCENET 		: Ethernet
				-  CLKPWR_PCONP_PCUSB   	: USB
 * @param[in] NewState  
 *            state of peripheral power :
 *              - ENABLE	: Enable power  for this peripheral
 * 				- DISABLE	: Disable power for this peripheral
 *****************************************************************/
void CLKPWR_ConfigPPWR(uint32_t PPType, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        LPC_SC->PCONP |= (PPType & CLKPWR_PCONP_BITMASK);
    }
    else if (NewState == DISABLE)
    {
        LPC_SC->PCONP &= ((~PPType) & CLKPWR_PCONP_BITMASK);    
    }    
}
