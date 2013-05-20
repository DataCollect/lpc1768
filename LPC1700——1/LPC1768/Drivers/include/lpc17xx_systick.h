#ifndef __LPC17XX_SYSTICK_H__
#define __LPC17XX_SYSTICK_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/* system timer control and status register */
#define STCTRL_ENABLE     ((uint32_t)(1 << 0))
#define STCTRL_TICKINT    ((uint32_t)(1 << 1))
#define STCTRL_CLKSOURCE  ((uint32_t)(1 << 2))
#define STCTRL_COUNTFLAG  ((uint32_t)(1 << 16))

/* system timer reload value  register */
#define STRELOAD_RELOAD(n)  ((uint32_t)(n & 0x00FFFFFF))

/* system timer current value  register */
#define STCURR_CURRENT(n)  ((uint32_t)(n & 0x00FFFFFF))

/* system timer calibration register */
//

#define ST_CLKSOURSE_EXT     ((uint32_t)(0))
#define ST_CLKSOURSE_CPU     ((uint32_t)(1)) 

void SYSTICK_InternalInit(uint32_t time);

void SYSTICK_Cmd(FunctionalState NewState);
void SYSTICK_IntCmd(FunctionalState NewState);
uint32_t SYSTICK_GetCurrentValue(void);
void SYSTICK_ClearCounterFlag(void);

#endif
