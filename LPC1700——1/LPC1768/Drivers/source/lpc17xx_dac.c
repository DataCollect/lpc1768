/*********************************************************
 * @file   lpc17xx_dac.c
 * @brief  contains all functions about dac
 * @version 1.0
 * @data    2010.6.5
 * @author
 ********************************************************/
#include "lpc17xx_dac.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_cfg.h"

#ifdef _DAC

/*******************************************************
 * @brief initial adc configuration
 * @param[in] DACx  
 * @return    none
 *******************************************************/
void DAC_Init(LPC_DAC_TypeDef *DACx)
{
    CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_DAC, CLKPWR_PCLKSEL_CCLK_DIV_4);

    DAC_SetBias(DACx, DAC_MAX_CURRENT_700uA);
}

/********************************************************
 * @brief update value to dac
 * @param[in]  DACx
 * @param[in]  dac_value
 * @return     none
 *******************************************************/
void DAC_UpdateValue(LPC_DAC_TypeDef *DACx, uint32_t dac_value)
{
    uint32_t tmp;

    tmp = DACx->DACR & DAC_BIAS_EN;
    tmp |= DAC_VALUE(dac_value);

    DACx->DACR = tmp;
}

/**********************************************************
 * @brief Set Maximum current for dac
 * @param[in] DACx
 * @param[in] bias  : 0 is 700uA
                      1 is 350uA
 * @return    none
 **********************************************************/
void DAC_SetBias(LPC_DAC_TypeDef *DACx, uint32_t bias)
{
    DACx->DACR &=~DAC_BIAS_EN;
	if (bias  == DAC_MAX_CURRENT_350uA)
	{
		DACx->DACR |= DAC_BIAS_EN;
	}
}

#endif
