#ifndef __LPC17XX_ADC_H__
#define __LPC17XX_ADC_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/* A/D Control Register */
/* selects which of AD0..7 to be sampled and converted */
#define ADC_CR_CH_SEL(n)  ((1UL << n)) 
/* the PCLK_ADC0 is divided by this value to produce the clock for A/D converter */
#define ADC_CR_CLKDIV(n)  ((n << 8 ))
/* the ad does repeatecd conversions enable */
#define ADC_CR_BURST      ((1UL << 16))
/* the adc is in power-down mode */
#define ADC_CR_PDN        ((1UL << 21))
#define ADC_CR_START_MASK	((7UL<<24))
/* adc start mask bit */
#define ADC_START_MASKBIT ((7UL << 24))
/* select start mode */
#define ADC_CR_START_MODE_SEL(sel) ((sel << 24))
#define ADC_CR_START_NOW ((1UL << 24))
#define ADC_CR_EDGE      ((1UL << 27))

/* a/d global data register */
#define ADC_GDR_RESULT(n)  (((n >> 4) & 0xFFF))
#define ADC_GDR_CHN(n)     ((n >> 24) & 0x07)
/* this bit in burst mode if the results of one or more conversions
   was lost and overwritten before the conversion that produced the 
   result in the REsULT bits */
#define ADC_GDR_OVERRUN    ((1UL << 30))
/* this bit is set to 1 when an a/d conversion completes*/
#define ADC_GDR_DONE       ((1UL << 31)) 

/* a/d interrupt enable register*/
#define ADC_INTEN_CH(n)   ((1UL << n))
#define ADC_INTEN_GLOBAL  ((1UL << 8))

/* A/D data registers */
#define ADC_DR_RESULT(n)  (((n >> 4) & 0xFFF))
#define ADC_DR_CHN(n)     ((n >> 24) & 0x07)
/* this bit in burst mode if the results of one or more conversions
   was lost and overwritten before the conversion that produced the 
   result in the REsULT bits */
#define ADC_DR_OVERRUN    ((1UL << 30))
/* this bit is set to 1 when an a/d conversion completes*/
#define ADC_DR_DONE       ((1UL << 31))
#define ADC_GSTAT_DONE    (255ul)

/* a/d status register */ 
#define ADC_ADSTAT_DONE_CH(n)    ((1UL << n))
#define ADC_ADSTAT_OVERRUN_CH(n) (((n>>8)& 0xFF))
#define ADC_ADSTAT_ADINT		 ((1UL<<16))

/* a/d trim register */
#define ADC_ADTRIM_ADCOFFS(n)    (((n&0xFF) << 4)) 

/* @brief channel selection */
typedef enum
{
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7
}ADC_CHANNEL_SELECTION;

/* @brief type of start option */ 
typedef enum
{
    ADC_START_NOSTAT = 0,
    ADC_START_NOW,
    ADC_START_ON_EINT0,
    ADC_START_ON_CAP01,
    ADC_START_ON_MAT01,
    ADC_START_ON_MAT03,
    ADC_START_ON_MAT10,
    ADC_START_ON_MAT11
}ADC_START_OPT; 

/* @brief adc interrupt type */
typedef enum
{
    ADC_ADINTEN0 = 0,
    ADC_ADINTEN1,
    ADC_ADINTEN2,
    ADC_ADINTEN3,
    ADC_ADINTEN4,
    ADC_ADINTEN5,
    ADC_ADINTEN6,
    ADC_ADINTEN7,
    ADC_ADGINTEN
}ADC_TYPE_INT_OPT;

/* @brief adc data status */
typedef enum
{
    AD_DATA_BURST = 0,
    ADC_DATA_DONE
}ADC_DATA_STATUS;

void ADC_Init(LPC_ADC_TypeDef *ADCx, uint32_t rate);
void ADC_DeInit(LPC_ADC_TypeDef *ADCx);

void ADC_StartCmd(LPC_ADC_TypeDef *ADCx, uint8_t start_mode);
void ADC_ChannelCmd(LPC_ADC_TypeDef *ADCx, uint8_t Channel, FunctionalState NewState);
void ADC_BurstCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState);

void ADC_IntConfig(LPC_ADC_TypeDef *ADCx, ADC_TYPE_INT_OPT IntType, FunctionalState NewState);

uint16_t ADC_ChannelGetData(LPC_ADC_TypeDef *ADCx, uint8_t channel);
FlagStatus ADC_ChannelGetStatus(LPC_ADC_TypeDef *ADCx, uint8_t channel, uint32_t StatusType);
uint32_t ADC_GlobalGetData(LPC_ADC_TypeDef *ADCx);
//FlagStatus ADC_GlobalGetStatus(LPC_ADC_TypeDef *ADCx, uint32_t StatusType);
uint32_t ADC_GlobalGetStatus(LPC_ADC_TypeDef *ADCx);

#endif
