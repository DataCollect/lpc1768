#ifndef __LPC17XX_CLKPWR_H__
#define __LPC17XX_CLKPWR_H__

#include "LPC17xx.h"
#include "lpc_types.h"

/*************************************************************
 * @brief system controls and status register (SCS)
 *************************************************************/
 /* oscrange:  0: the frequency range of the main oscillator is 1mhz to 20mhz 
  *            1: the frequency range of the main oscillator is 15mhz to 25mhz 
  ************************************************************/
#define CLKPWR_SCS_OSCRANGE      ((uint32_t)(0 << 4))  
#define CLKPWR_SCS_OSCEN         ((uint32_t)(1 << 5))  /* 0:disabled 1:enabled*/
#define CLKPWR_SCS_OSCSTAT       ((uint32_t)(1 << 6))  /* 0: not ready 1: is ready*/

/*************************************************************
 * @brief Macro defines for PLL0 clock source select register
 * Internal RC oscillator : IRC
 * Main oscillator		  : MAINOSC
 * RTC oscillator		  : RTC
 * Clock source selection bit mask:	BITMASK
 *************************************************************/
#define CLKPWR_CLKSRCSEL_CLKSRC_IRC			(0x00000000)
#define CLKPWR_CLKSRCSEL_CLKSRC_MAINOSC		(0x00000001)
#define CLKPWR_CLKSRCSEL_CLKSRC_RTC			(0x00000002)
#define CLKPWR_CLKSRCSEL_BITMASK			(0x00000003)

/*************************************************************
 * @brief Macro defines for clock output configuration register
 *************************************************************/
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_CPU		((uint32_t)(0x00))
#define CLKPWR_CLKOURCFG_CLKOUTSEL_MAINOSC  ((uint32_t)(0x01))
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_RC		((uint32_t)(0x02))
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_USB		((uint32_t)(0x03))
#define CLKPWR_CLKOUTCFG_CLKOUTSEL_RTC		((uint32_t)(0x04))

#define CLKPWR_CLKOUTCFG_CLKOUTDIV(n)		((uint32_t)((n&0x0F)<<4))

#define CLKPWR_CLKOUTCFG_CLKOUT_EN          ((uint32_t)(1<<8))

#define CLKPWR_CLKOUTCFG_CLKOUT_ACT			((uint32_t)(1<<9))

#define CLKPWR_CLKOUTCFG_BITMASK            ((uint32_t)(0x3FF))

/*************************************************************
 * @brief Macro defines for PLL0 control register 
 *************************************************************/
#define CLKPWR_PLL0CON_ENABLE		((uint32_t)(1 << 0))
#define CLKPWR_PLL0CON_CONNECT		((uint32_t)(1 << 1))
#define CLKPWR_PLL0CON_BITMASK		((uint32_t)(0x03))

/*************************************************************
 * @brief Macro defines for PLL0 configuration register 
 *************************************************************/
#define CLKPWR_PLL0CFG_MSEL(n)      ((uint32_t)(n&0x7fff))	      /* 6~512 */
#define CLKPWR_PPL0CFG_NSEL(n)		((uint32_t)((n<<16)&0xff0000))/* 1~32  */
#define CLKPWR_PLL0CFG_BITMASK		((uint32_t)(0xFF7FFF))

/*************************************************************
 * @brief Macro defines for PLL0 status register 
 * MSEL(n)
 * NSEL(n)
 * PLLE
 * PLLC
 * PLOCK
 *************************************************************/
#define CLKPWR_PLL0STAT_MSEL(n)		((uint32_t)(n&0x7FFF))     
#define CLKPWR_PLL0STAT_NSEL(n)		((uint32_t)((n>>16)&0xFF)) 
#define CLKPWR_PLL0STAT_PLLE		((uint32_t)(1<<24))
#define CLKPWR_PLL0STAT_PLLC		((uint32_t)(1<<25))
#define CLKPWR_PLL0STAT_PLOCK		((uint32_t)(1<<26))

/*************************************************************
 * @brief Macro defines for PLL0 feed register 
 *************************************************************/
#define CLKPWR_PLL0FEED_BITMASK     ((uint32_t)0xff)

/*************************************************************
 * @brief Macro defines for cpu clock configuration register 
 *************************************************************/
#define CLKPWR_CCLKCFG_CCLKSEL      (2UL)/* 0~255*/
#define CLKPWR_CCLKCFG_BITMASK      ((uint32_t)0xff)

/*************************************************************
 * @brief Macro defines for peripheral clock selection 
 *************************************************************/
#define CLKPWR_PCLKSEL_BITMASK(p)   _SBF(p, 0x03)
#define CLKPWR_PCLKSEL_SET(p,n)		_SBF(p,n)
#define CLKPWR_PCLKSEL_GET(p, n)	((uint32_t)((n>>p)&0x03))

/*************************************************************
 * @breif Peripheral clock selection definitions
 * Peripheral clok divider bit position for 
 * eg. WDT, TIMER0,...SYSCON,MC.
 *************************************************************/
#define	CLKPWR_PCLKSEL_WDT  	    ((uint32_t)(0))
#define	CLKPWR_PCLKSEL_TIMER0  		((uint32_t)(2))
#define	CLKPWR_PCLKSEL_TIMER1  		((uint32_t)(4))
#define	CLKPWR_PCLKSEL_UART0  		((uint32_t)(6))
#define	CLKPWR_PCLKSEL_UART1  		((uint32_t)(8))
#define	CLKPWR_PCLKSEL_PWM1  		((uint32_t)(12))
#define	CLKPWR_PCLKSEL_I2C0  		((uint32_t)(14))
#define	CLKPWR_PCLKSEL_SPI  		((uint32_t)(16))
#define	CLKPWR_PCLKSEL_SSP1  		((uint32_t)(20))
#define	CLKPWR_PCLKSEL_DAC  		((uint32_t)(22))
#define	CLKPWR_PCLKSEL_ADC  		((uint32_t)(24))
#define	CLKPWR_PCLKSEL_CAN1 		((uint32_t)(26))
#define	CLKPWR_PCLKSEL_CAN2 		((uint32_t)(28))
#define	CLKPWR_PCLKSEL_ACF  		((uint32_t)(30))
#define	CLKPWR_PCLKSEL_QEI  		((uint32_t)(32))
#define	CLKPWR_PCLKSEL_PCB  		((uint32_t)(36))
#define	CLKPWR_PCLKSEL_I2C1  		((uint32_t)(38))
#define	CLKPWR_PCLKSEL_SSP0  		((uint32_t)(42))
#define	CLKPWR_PCLKSEL_TIMER2  		((uint32_t)(44))
#define	CLKPWR_PCLKSEL_TIMER3  		((uint32_t)(46))
#define	CLKPWR_PCLKSEL_UART2  		((uint32_t)(48))
#define	CLKPWR_PCLKSEL_UART3  		((uint32_t)(50))
#define	CLKPWR_PCLKSEL_I2C2  		((uint32_t)(52))
#define	CLKPWR_PCLKSEL_I2S  		((uint32_t)(54))
#define	CLKPWR_PCLKSEL_RIT  		((uint32_t)(58))
#define	CLKPWR_PCLKSEL_SYSCON  		((uint32_t)(60))
#define	CLKPWR_PCLKSEL_MC  			((uint32_t)(62))

/** Peripheral Clock Selection register bit values **********/
#define CLKPWR_PCLKSEL_CCLK_DIV_4   ((uint32_t)(0))
#define CLKPWR_PCLKSEL_CCLK_DIV_1   ((uint32_t)(1))
#define CLKPWR_PCLKSEL_CCLK_DIV_2   ((uint32_t)(2))
/** when CCLK_DIV_8,except for CAN1,CAN2,and CAn filtering
 *  when "11" selects = CCLK/6.
 ***********************************************************/
#define CLKPWR_PCLKSEL_CCLK_DIV_8 ((uint32_t)(3))

/***********************************************************
 * Power Control for Peripherals Definitions
 * power/clock control bit for PCTIME0,PCTIME1,...,PCENET,PCUSB
 ***********************************************************/
#define	 CLKPWR_PCONP_PCTIM0	((uint32_t)(1<<1))
#define	 CLKPWR_PCONP_PCTIM1	((uint32_t)(1<<2))
#define	 CLKPWR_PCONP_PCUART0  	((uint32_t)(1<<3))
#define	 CLKPWR_PCONP_PCUART1  	((uint32_t)(0<<4))
#define	 CLKPWR_PCONP_PCPWM1	((uint32_t)(1<<6))
#define	 CLKPWR_PCONP_PCI2C0	((uint32_t)(1<<7))
#define	 CLKPWR_PCONP_PCSPI  	((uint32_t)(1<<8))
#define	 CLKPWR_PCONP_PCRTC  	((uint32_t)(1<<9))
#define	 CLKPWR_PCONP_PCSSP1	((uint32_t)(0<<10))
#define	 CLKPWR_PCONP_PCAD  	((uint32_t)(1<<12))
#define	 CLKPWR_PCONP_PCAN1  	((uint32_t)(0<<13))
#define	 CLKPWR_PCONP_PCAN2 	((uint32_t)(0<<14))
#define	 CLKPWR_PCONP_PCGPIO 	((uint32_t)(1<<15))
#define	 CLKPWR_PCONP_PCRIT 	((uint32_t)(1<<16))
#define  CLKPWR_PCONP_PCMC 		((uint32_t)(1<<17))
#define  CLKPWR_PCONP_PCQEI 	((uint32_t)(0<<18))
#define	 CLKPWR_PCONP_PCI2C1  	((uint32_t)(1<<19))
#define	 CLKPWR_PCONP_PCSSP0	((uint32_t)(0<<21))
#define	 CLKPWR_PCONP_PCTIM2	((uint32_t)(0<<22))
#define	 CLKPWR_PCONP_PCTIM3	((uint32_t)(0<<23))
#define	 CLKPWR_PCONP_PCUART2  	((uint32_t)(0<<24))
#define	 CLKPWR_PCONP_PCUART3  	((uint32_t)(0<<25))
#define	 CLKPWR_PCONP_PCI2C2	((uint32_t)(1<<26))
#define	 CLKPWR_PCONP_PCI2S  	((uint32_t)(0<<27))
#define	 CLKPWR_PCONP_PCGPDMA  	((uint32_t)(1<<29))
#define	 CLKPWR_PCONP_PCENET	((uint32_t)(0<<30))
#define	 CLKPWR_PCONP_PCUSB  	((uint32_t)(0<<31))


#define  CLKPWR_PCONP_BITMASK	(0xEFEFF7DE)
/***************************************************************
 * @brief config the clk
 ***************************************************************/
#define CLOCK_SETUP     1		
#define PLL0_SETUP      1        
/**********************************************
 * @brief clk system initial
 **********************************************/
#define SCS_VAL 	    (CLKPWR_SCS_OSCRANGE | CLKPWR_SCS_OSCEN)
#define CLKSRCSEL_VAL   CLKPWR_CLKSRCSEL_CLKSRC_MAINOSC          
#define PLL0CFG_VAL     (CLKPWR_PLL0CFG_MSEL(11ul) | CLKPWR_PPL0CFG_NSEL(0))          
#define CCLKCFG_VAL     CLKPWR_CCLKCFG_CCLKSEL         
#define PCLKSEL0_VAL	(0x00000000)		   
#define PCLKSEL1_VAL	(0x00000000)		    
#define PCONP_VAL		(CLKPWR_PCONP_PCTIM0  | CLKPWR_PCONP_PCTIM1  \
                       | CLKPWR_PCONP_PCUART0 | CLKPWR_PCONP_PCUART1 \
					   | CLKPWR_PCONP_PCPWM1  | CLKPWR_PCONP_PCI2C0  \
					   | CLKPWR_PCONP_PCSPI   | CLKPWR_PCONP_PCRTC   \
					   | CLKPWR_PCONP_PCSSP1  | CLKPWR_PCONP_PCAD    \
					   | CLKPWR_PCONP_PCAN1   | CLKPWR_PCONP_PCAN2   \
					   | CLKPWR_PCONP_PCGPIO  | CLKPWR_PCONP_PCRIT   \
					   | CLKPWR_PCONP_PCMC    | CLKPWR_PCONP_PCQEI   \
					   | CLKPWR_PCONP_PCI2C1  | CLKPWR_PCONP_PCSSP0  \
					   | CLKPWR_PCONP_PCTIM2  |	CLKPWR_PCONP_PCTIM3  \
					   | CLKPWR_PCONP_PCUART2 | CLKPWR_PCONP_PCUART3 \
					   | CLKPWR_PCONP_PCI2C2  |	CLKPWR_PCONP_PCI2S   \
					   | CLKPWR_PCONP_PCGPDMA | CLKPWR_PCONP_PCENET  \
                       | CLKPWR_PCONP_PCUSB )  
					  

						 		    
#define CLKOUTCFG_VAL	 (0x00000000)

/*********************************************
 * @brief flash accelerator configuration 
 *********************************************/
#define FLASH_SETUP   1
#define FLASHCFG_VAL  (0x0000303A)

/**********************************************
 * @brief defines  clocks
 **********************************************/
#define XTAL           (12000000UL)   /* Osillator frequency */
#define OSC_CLK		   (      XTAL)	  /* MAIN OSCILLATOR frequency */
#define RTC_CLK        (   32768UL)	  /* RTC  OSCILLATOR frequency */
#define IRC_CLK        ( 4000000UL)	  /* internal RC oscillator frequency */

/* F_CCO = (2 * M * F_IN) / N */
#define __M            (((PLL0CFG_VAL      ) & 0x7FFF) + 1)   /* .0~.14 */
#define __N            (((PLL0CFG_VAL >> 16) & 0x00FF) + 1)   /* .16~   */
#define __FCCO(__F_IN) ((2 * __M * __F_IN) / __N)			  /* 275mhz ~ 550mhz */
#define __CCLK_DIV     (((CCLKCFG_VAL       )& 0x00FF) + 1)

/* #define cpu clock frequency according to settings */
#if (PLL0_SETUP)
    #if   ((CLKSRCSEL_VAL & 0x03) == 1)
	    #define __CORE_CLK (__FCCO(OSC_CLK) / __CCLK_DIV)  
	#elif ((CLKSRCSEL_VAL & 0x03) == 2)
	    #define __CORE_CLK (__FCCO(RTC_CLK) / __CCLK_DIV)
	#else
	    #define __CORE_CLK (__FCCO(IRC_CLK) / __CCLK_DIV)
	#endif
#else
    #if   ((CLKSRCSEL_VAL & 0x03) == 1)
	    #define __CORE_CLK (OSC_CLK        / __CCLK_DIV)  
	#elif ((CLKSRCSEL_VAL & 0x03) == 2)
	    #define __CORE_CLK (RTC_CLK        / __CCLK_DIV)
	#else
	    #define __CORE_CLK (IRC_CLK        / __CCLK_DIV)
	#endif 
#endif

/***************************************************************
 * public functions
 ***************************************************************/
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
extern void CLKPWR_SetPCLKDiv(uint32_t ClkType, uint32_t DivVal);
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
extern uint32_t CLKPWR_GetPCLKSEL(uint32_t ClkType);

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
extern uint32_t CLKPWR_GetPCLK (uint32_t ClkType);

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
extern void CLKPWR_ConfigPPWR(uint32_t PPType, FunctionalState NewState);



#endif

