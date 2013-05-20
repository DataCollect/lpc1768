#include <stdint.h>
#include "LPC17xx.h"
#include "lpc17xx_clkpwr.h" 
/*********************************************************************
 * @brief Clock Variable definitions
 *********************************************************************/
uint32_t SystemCoreClock = __CORE_CLK;    /* system clock frequency  */

/**
 *initialize the system
 *
 * @param none
 * @param none
 * @brief setup the mcu system.
 * initialize the system
 */
void SystemInit(void)
{
    /* DISCONNECT PLL0 AND DISABLED PLL0 */
	if ((LPC_SC->PLL0STAT & CLKPWR_PLL0STAT_PLLC) == CLKPWR_PLL0STAT_PLLC)
	{
	    LPC_SC->PLL0CON  &= (~CLKPWR_PLL0CON_CONNECT);
		LPC_SC->PLL0FEED  = 0xAA;
		LPC_SC->PLL0FEED  = 0x55;	
	}
	LPC_SC->PLL0CON &= (~CLKPWR_PLL0CON_ENABLE);
	LPC_SC->PLL0FEED = 0xAA;
	LPC_SC->PLL0FEED = 0x55;
	while ((LPC_SC->PLL0STAT & CLKPWR_PLL0STAT_PLLC) == CLKPWR_PLL0STAT_PLLC);
		
#if (CLOCK_SETUP)
    /* enabled mainosc and config the  pclksel0 register and pclksel1 register*/
    LPC_SC->SCS = SCS_VAL;
	if (LPC_SC->SCS & CLKPWR_SCS_OSCEN)
	{
	    while ((LPC_SC->SCS & CLKPWR_SCS_OSCSTAT) == 0);
	}

 	LPC_SC->CCLKCFG = CCLKCFG_VAL;

	LPC_SC->PCLKSEL0 = PCLKSEL0_VAL;
	LPC_SC->PCLKSEL1 = PCLKSEL1_VAL;
	
	/*
	 * 1 select the  clk source of pll0
	 * 2 write to pll0cfg (m,n)	,and feed it
	 * 3 enabled the pll0 ,and feed it
	 * 4 wait for pll0 achive lock
	 * 5 enabled and connect the pll0
	 */
#if (PLL0_SETUP)
	LPC_SC->CLKSRCSEL = CLKSRCSEL_VAL;
	LPC_SC->PLL0CFG   = PLL0CFG_VAL;
	LPC_SC->PLL0FEED  = 0xAA;
	LPC_SC->PLL0FEED  = 0x55;

	LPC_SC->PLL0CON   = CLKPWR_PLL0CON_ENABLE;
	LPC_SC->PLL0FEED  = 0xAA;
	LPC_SC->PLL0FEED  = 0x55;
	while (!(LPC_SC->PLL0STAT & CLKPWR_PLL0STAT_PLOCK));
	
	LPC_SC->PLL0CON   = (CLKPWR_PLL0CON_ENABLE | CLKPWR_PLL0CON_CONNECT);
	LPC_SC->PLL0FEED  = 0xAA;
	LPC_SC->PLL0FEED  = 0x55;
#endif

	LPC_SC->PCONP     = PCONP_VAL;
	LPC_SC->CLKOUTCFG = CLKOUTCFG_VAL;
#endif

#if (FLASH_SETUP)
	LPC_SC->FLASHCFG = FLASHCFG_VAL;
#endif

	SCB->VTOR  = 0x00000000 & 0x3FFFFF80;
}
