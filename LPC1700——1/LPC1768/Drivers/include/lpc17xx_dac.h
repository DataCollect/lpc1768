#ifndef __LPC17XX_DAC_H__
#define __LPC17XX_DAC_H__

#include "lpc17xx.h"
#include "lpc_types.h"

#define DAC_VALUE(n) 		((uint32_t)((n&0x3FF)<<6))
/* the settling time of the DAC is 2.5us and the maximum current is 350ua */
#define DAC_BIAS_EN			((uint32_t)(1<<16))  

/* @brief Current option in DAC configuration  option */
typedef enum
{
    DAC_MAX_CURRENT_700uA = 0,
    DAC_MAX_CURRENT_350uA
}DAC_CURRENT_OPT;   

void DAC_Init(LPC_DAC_TypeDef *DACx); 
void DAC_UpdateValue(LPC_DAC_TypeDef *DACx, uint32_t dac_value);
void DAC_SetBias(LPC_DAC_TypeDef *DACx, uint32_t bias); 


#endif
