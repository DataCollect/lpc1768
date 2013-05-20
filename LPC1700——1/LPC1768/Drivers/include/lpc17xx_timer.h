#ifndef __LPC17XX_TIMER_H__
#define __LPC17XX_TIMER_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/* clear the interrupt pending */
#define TIM_IR_CLR(n)  (_BIT(n))

/* timer interrupt registers definitions  */
#define TIM_MATCH_INT(n)  (_BIT(n & 0x0F))
#define TIM_CAP_INT(N)    (_BIT((n & 0x0F) + 4))

/* timer control register definitions */
#define TIM_ENABLE    ((uint32_t)(1 << 0))
#define TIM_RESET     ((uint32_t)(1 << 1))

/* timer match control register */
#define TIM_INT_ON_MATCH(n)   (_BIT(n * 3))
#define TIM_RESET_ON_MATCH(n)  (_BIT(((n * 3) + 1)))
#define TIM_STOP_ON_MATCH(n)  (_BIT(((n * 3) + 2)))
#define	TIM_MCR_CHANNEL_MASKBIT(n)		((uint32_t)(7 << (n * 3)))

/* capture control register */
#define TIM_CAP_RISING(n)   (_BIT((n * 3)))
#define TIM_CAP_FALLING(n) (_BIT(((n * 3) + 1)))
#define TIM_INT_ON_CAP(n)  (_BIT(((n * 3) + 2)))
#define	TIM_CCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<(n*3)))

/* count control register  */
#define TIM_CTCR_MODE_MASK (0x03)
#define TIM_COUNTER_MODE   ((uint8_t)(1))
#define TIM_CTCR_INPUT_MASK 0xC

/* external match register */
#define TIM_EM(n)    			_BIT(n)
#define TIM_EM_NOTHING    	((uint8_t)(0x0))
#define TIM_EM_LOW         	((uint8_t)(0x1))
#define TIM_EM_HIGH        	((uint8_t)(0x2))
#define TIM_EM_TOGGLE      	((uint8_t)(0x3))
/* setting for the MAT.n change state bits */
#define TIM_EM_SET(n,s) 	(_SBF(((n << 1) + 4), (s & 0x03)))
/* MAT.n change state bits */
#define TIM_EM_MASK(n) 		(_SBF(((n << 1) + 4), 0x03))


/* @brief timer/counter OPERATING MODE */
typedef enum
{   
    TIM_TIMER_MODE = 0 ,
    TIM_COUNTER_RISING_MODE ,
    TIM_COUNTER_FALLING_MODE,
    TIM_COUNTER_BOTH_MODE
}TIM_MODE_OPT;

/* @brief :interrupt type*/
typedef enum
{
    TIM_MR0_INT = 0,
    TIM_MR1_INT = 1,
    TIM_MR2_INT,
    TIM_MR3_INT,
    TIM_CR0_INT,
    TIM_CR1_INT
}TIM_INT_Type;




/* @brief timer/counter prescale */
typedef enum
{
    TIM_PRESCALE_TICKVAL = 0,
    TIM_PRESCALE_USVAL
}TIM_PRESCALE_OPT;

/* @brief counter input option */
typedef enum
{
    TIM_COUNTER_INCAP0 = 0,
    TIM_COUNTER_INCAP1
}TIM_COUNTER_INPUT_OPT;

/* @brief configuration structure */
typedef struct
{
    uint8_t PrescaleOption;
    uint8_t Reserved[3];
    uint32_t PrescaleValue;
}TIM_TIMERCFG_Type;

/* @brief configuration structure in counter mode */
typedef struct
{
    uint8_t CounterOption;
    uint8_t CountInputSelect;
    uint8_t Reserved[2];
}TIM_COUNTERCFG_Type;

/* @brief match channel configuraton structure */
typedef struct
{
    uint8_t MatchChannel;
    uint8_t IntOnMatch;
    uint8_t StopOnMatch;
    uint8_t ResetOnMatch;
    uint8_t ExtMatchOutputType;
    uint8_t Reserved[3];
    uint32_t MatchValue;
}TIM_MATCHCFG_Type;

/* @brief capture input configuration structure */
typedef struct
{
    uint8_t CaptureChannel;
    uint8_t RisingEdge;
    uint8_t FallingEdge;
    uint8_t IntOnCaption;
}TIM_CAPTURECFG_Type;

void TIM_Init(LPC_TIM_TypeDef *TIMx, TIM_MODE_OPT TimerCounterMode, void *TIM_ConfigStruct);
void TIM_DeInit(LPC_TIM_TypeDef *TIMx);

void TIM_ClearIntPending(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag);
void TIM_ClearIntCapturePending(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag);
FlagStatus TIM_GetIntStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag);
FlagStatus TIM_GetIntCaptureStatus(LPC_TIM_TypeDef *TIMx, TIM_INT_Type IntFlag);

void TIM_ConfigStructInit(TIM_MODE_OPT TimerCounterMode, void *TIM_ConFigStruct);
void TIM_ConfigMatch(LPC_TIM_TypeDef *TIMx, TIM_MATCHCFG_Type *TIM_MatchConfigStruct);
void TIM_UpdateMatchValue(LPC_TIM_TypeDef *TIMx,uint8_t MatchChannel, uint32_t MatchValue);
//void TIM_SetMatchExt(LPC_TIM_TypeDef *TIMx,TIM_EXTMATCH_OPT ext_match );
void TIM_ConfigCapture(LPC_TIM_TypeDef *TIMx, TIM_CAPTURECFG_Type *TIM_CaptureConfigStruct);
void TIM_Cmd(LPC_TIM_TypeDef *TIMx, FunctionalState NewState);

uint32_t TIM_GetCaptureValue(LPC_TIM_TypeDef *TIMx, TIM_COUNTER_INPUT_OPT CaptureChannel);
void TIM_ResetCounter(LPC_TIM_TypeDef *TIMx);

#endif

