#ifndef __LPC17XX_PWM_H__
#define __LPC17XX_PWM_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/* PWM1IR: pwm interrupt Register */
/* interrupt flag for PWM match channel n */
#define PWM_IR_PWMMRn(n) ((uint32_t)((n<4) ? (1<<n) : (1<<(n+4))))
/* interrupt flag for capture input n */
#define PWM_IR_PWMCAPn(n) ((uint32_t)(1 << (n+4)))
#define PWM_IR_BITMASK    ((uint32_t)(0x0000073F))

/* PWM1TCR: pwm timer control register */
#define PWM_TCR_BITMASK				((uint32_t)(0x0000000B))
/* enable the pwm timer counter and pwm prescale counter */
#define PWM_TCR_COUNTER_ENABLE      ((uint32_t)(1 << 0))
/* RESET the pwm timer counter and pwm prescale counter */
#define PWM_TCR_COUNTER_RESET       ((uint32_t)(1 << 1))
/* enable the pwm mode */
#define PWM_TCR_PWM_ENABLE          ((uint32_t)(1 << 3))

/* PWM1CTCR : the count control register */
#define PWM_CTCR_BITMASK			((uint32_t)(0x0000000F))
/* counter/timer mode select */
#define PWM_CTCR_MODE(n)  ((uint32_t)(n & 0x03))
/* count input select*/
#define PWM_CTCR_INPUT_SELECT(n) ((uint32_t)(n & 0x03) << 2)

/* PWM1MCR: pwm match control register */
#define PWM_MCR_BITMASK				((uint32_t)(0x001FFFFF))
/* generate a PWM interrupt when a MATCHn occurs */
#define PWM_MCR_INT_ON_MATCH(n)     ((uint32_t)(1<<(((n&0x7)<<1) + (n&0x07))))
/* reset the PWM when a MATCHn occurs */
#define PWM_MCR_RESET_ON_MATCH(n)   ((uint32_t)(1<<(((n&0x7)<<1) + (n&0x07)+1)))
/* stop the PWM when a MATCHn occurs */
#define PWM_MCR_STOP_ON_MATCH(n)    ((uint32_t)(1<<(((n&0x7)<<1) + (n&0x07)+2)))

/* PWM1CCR: pwm capture control register */
#define PWM_CCR_BITMASK				((uint32_t)(0x0000003F))
/* PCAPn is rising edge sensitive */
#define PWM_CCR_CAP_RISING(n) 	 	((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1))))
/* PCAPn is falling edge sensitive */
#define PWM_CCR_CAP_FALLING(n) 		((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+1)))
/* PWM interrupt is generated on a PCAP event */
#define PWM_CCR_INT_ON_CAP(n)  		((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+2)))

/* PWM1PCR :PWM control register */
#define PWM_PCR_BITMASK			(uint32_t)0x00007E7C
/*PWM output n is a single edge controlled output */
#define PWM_PCR_PWMSELn(n)   	((uint32_t)(((n&0x7)<2) ? 0 : (1<<n)))
/* enable PWM output n */
#define PWM_PCR_PWMENAn(n)   	((uint32_t)(((n&0x7)<1) ? 0 : (1<<(n+8))))

/* PWM1LER: pwm latch enable register */
#define PWM_LER_BITMASK				((uint32_t)(0x0000007F))
/** PWM MATCHn register update control */
#define PWM_LER_EN_MATCHn_LATCH(n)   ((uint32_t)((n<7) ? (1<<n) : 0))


typedef enum {
    PWM_CHANNEL_SINGLE_EDGE,	/* PWM Channel Single edge mode */
    PWM_CHANNEL_DUAL_EDGE		/* PWM Channel Dual edge mode */
} PWM_CHANNEL_EDGE_OPT;

/* @brief configuration structure in pwm timer mode*/
typedef struct
{
    uint8_t PrescaleOption;    /* prescale option : PWM_TIMER_PRESCALE_TICKVAL/PWM_TIMER_PREASCAL_USVAL */
    uint8_t Reserved[3];
    uint32_t PrescaleValue;  /* prescale value */
}PWM_TIMERCFG_Type;

/* @brief pwm match channel configuration structure */
typedef struct
{
    uint8_t MatchChannel;  /* 0---6*/
    uint8_t IntOnMatch;    /* enable or disable */
    uint8_t StopOnMatch;   /* enable or disable */
    uint8_t ResetOnMatch;  /* enable or disable */
}PWM_MATCHCFG_Type;

/* @brief pwm tc  mode selection */
typedef enum
{
    PWM_MODE_TIMER = 0, /* timer mode */
    PWM_MOD_COUNTER     /* counter mode */
}PWM_TC_MODE_OPT;

/* @brief pwm timer/counter presacle otion */
typedef enum
{
    PWM_TIMER_PRESCALE_TICKVAL = 0,   /* prescale in absolute value */
    PWM_TIMER_PRESCALE_USVAL          /* prescale in microsecond value */
}PWM_TIMER_PRESCALE_OPT;

/* @brief Pwm interrupt status type */
typedef enum
{
    PWM_INTSTAT_MR0 = PWM_IR_PWMMRn(0),
    PWM_INTSTAT_MR1 = PWM_IR_PWMMRn(1),
    PWM_INTSTAT_MR2 = PWM_IR_PWMMRn(2),
    PWM_INTSTAT_MR3 = PWM_IR_PWMMRn(3),
    PWM_INTSTAT_CAP0 = PWM_IR_PWMCAPn(0),
    PWM_INTSTAT_CAP1 = PWM_IR_PWMCAPn(1),
    PWM_INTSTAT_MR4 = PWM_IR_PWMMRn(4),
    PWM_INTSTAT_MR5 = PWM_IR_PWMMRn(5),
    PWM_INTSTAT_MR6 = PWM_IR_PWMMRn(6)
}PWM_INTSTAT_TYPE;

void PWM_PinConfig(LPC_PWM_TypeDef *PWMx, uint8_t PWM_Channel, uint8_t PinselOption);
IntStatus PWM_GetIntStatus(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag);
void PWM_ClearIntPending(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag);
void PWM_ConfigStructInit(uint8_t PWMTimerCounterMode, void *PWM_InitStruct);
void PWM_Init(LPC_PWM_TypeDef *PWMx, uint32_t PWMTimerCounterMode, void *PWM_InitStruct);
void PWM_DeInit(LPC_PWM_TypeDef *PWMx);
void PWM_Cmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState);
void PWM_ConfigMatch(LPC_PWM_TypeDef *PWMx, PWM_MATCHCFG_Type *PWM_MatchConfigStruct);
void PWM_MatchUpdate(LPC_PWM_TypeDef *PWMx, uint8_t MatchChannel, uint32_t MatchValue);
void PWM_ChannelConfig(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, uint8_t ModeOption);
void PWM_ChannelCmd(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, FunctionalState NewState);

void PWM_CounterCmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState);
void PWM_ResetCounter(LPC_PWM_TypeDef *PWMx);



#endif

