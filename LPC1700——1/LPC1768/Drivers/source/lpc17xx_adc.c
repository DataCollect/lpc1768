/******************************************************************
 * @file lpc17xx_adc.c
 * @brief contains all functions about adc
 * @version 1.0
 * @data 2010.6.3
 * @author 
 ******************************************************************/
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_cfg.h"

#ifdef _ADC

/*******************************************************
 * @brief init adc :set bit PCADC, 
                    set Clock for adc ,
                    set clock frenquency
 * @param[in]  ADCx : LPC_ADC
 * @param[in]  rate : ADC conversion rate
 * @return     none
 ******************************************************/
void ADC_Init(LPC_ADC_TypeDef *ADCx, uint32_t rate)
{
    uint32_t tmp, temp, ADCPclk;

    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAD, ENABLE);

    ADCx->ADCR = 0;

    tmp = ADC_CR_PDN;

    ADCPclk = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_ADC);
    /*
    a fully conversion requires 65 of these clocks
    adc clock = pclk_adc0/(clkdiv + 1);
    adc rate  = adc_clock / 65;
    so :
    adc clock = ad rate * 65;
    (clkdiv+1) = pclk_adc0 / adc clock;

    */
    if (rate > 200000U)
        rate = 200000U;
    temp = rate * 65;
    temp = (ADCPclk * 2 + temp) / (2 * temp) - 1;

    tmp |= ADC_CR_CLKDIV(temp + 94);  //22---1mhz 46---500khz 94---250khz

    ADCx->ADCR = tmp;
}

/*******************************************************
 * @brief close adc : first close pdn ,second close power
 * @param[in] ADCx
 * @return    none
 *******************************************************/
void ADC_DeInit(LPC_ADC_TypeDef *ADCx)
{
    ADCx->ADCR &= ~ADC_CR_PDN;

    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAD, DISABLE);
}

/*******************************************************
 * @brief get ad conversion result from ad data register
 * @param[in] channel : number which want to read back 
                        the result
 * @return    none
 *******************************************************/
uint32_t ADC_GetData(uint32_t channel)
{
    uint32_t adc_result;

    if (channel < 8)
    {
        adc_result = *(uint32_t *)(&LPC_ADC->ADDR0 + channel);

        return ADC_GDR_RESULT(adc_result);    

    }
    return ADC_GDR_RESULT(0);
}

/********************************************************************
 * @brief set start mode for adc 
 * @param[in] ADCx
 * @param[in] start_mode:
                        ADC_START_NOSTAT = 0,
                        ADC_START_NOW,
                        ADC_START_ON_EINT0,
                        ADC_START_ON_CAP01,
                        ADC_START_ON_MAT01,
                        ADC_START_ON_MAT03,
                        ADC_START_ON_MAT10,
                        ADC_START_ON_MAT11
 * @return none
 ********************************************************************/
void ADC_StartCmd(LPC_ADC_TypeDef *ADCx, uint8_t start_mode)
{
    ADCx->ADCR &= ~ADC_CR_START_MASK;
    ADCx->ADCR |= ADC_CR_START_MODE_SEL((uint32_t)start_mode);
}

/*********************************************************************//**
* @brief 		ADC Burst mode setting
* @param[in]	ADCx pointer to LPC_ADC_TypeDef, should be: LPC_ADC
* @param[in]	NewState
* 				-	1: Set Burst mode
* 				-	0: reset Burst mode
* @return 		None
**********************************************************************/
void ADC_BurstCmd(LPC_ADC_TypeDef *ADCx, FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_ADCx(ADCx));

	ADCx->ADCR &= ~ADC_CR_BURST;
	if (NewState){
		ADCx->ADCR |= ADC_CR_BURST;
	}
}

/********************************************************************
 * @brief Enable/Disable ADC channel number
 * @param[in] ADCx
 * @param[in] Channel :channel number
 * @param[in] NewState :Enable / Disable
 ********************************************************************/
void ADC_ChannelCmd(LPC_ADC_TypeDef *ADCx, uint8_t Channel, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        ADCx->ADCR |= ADC_CR_CH_SEL(Channel);
    }
    else
    {
        ADCx->ADCR &= ~ADC_CR_CH_SEL(Channel);
    }
}

/********************************************************************
 * @brief adc interrupt configuration 
 * @param[in] ADCx
 * @param[in] IntType: type of interrupt
                       ADC_ADINTEN0 = 0,
                       ADC_ADINTEN1,
                       ADC_ADINTEN2,
                       ADC_ADINTEN3,
                       ADC_ADINTEN4,
                       ADC_ADINTEN5,
                       ADC_ADINTEN6,
                       ADC_ADINTEN7,
                       ADC_ADGINTEN
 * @NewState :
                       SET
                       RESET
 * @return  none
 ********************************************************************/
void ADC_IntConfig(LPC_ADC_TypeDef *ADCx, ADC_TYPE_INT_OPT IntType, FunctionalState NewState)
{
    ADCx->ADINTEN &= ~ADC_INTEN_CH(IntType);

    if (NewState)
    {
        ADCx->ADINTEN |= ADC_INTEN_CH(IntType);
    }
}

/*********************************************************************
 * @brief Get adc result
 * @param[in] ADCx 
 * @param[in] Channel: channel number
 * @return    Data conversion
 ********************************************************************/
uint16_t ADC_ChannelGetData(LPC_ADC_TypeDef *ADCx, uint8_t channel)
{
    uint32_t adc_result;

    adc_result = *(uint32_t *) ((&ADCx->ADDR0) + channel);
	return ADC_DR_RESULT(adc_result);
}

/*********************************************************************
 * @brief get adc channel status from adc data register
 * @param[in] ADCx
 * @param[in] Channel: channel number
 * @param[in] StatusType :
                             0: Burst status
                             1: Done  status
 * @return    SET / RESET 
 *********************************************************************/
FlagStatus ADC_ChannelGetStatus(LPC_ADC_TypeDef *ADCx, uint8_t Channel, uint32_t StatusType)
{
    uint32_t tmp;

    tmp = *(uint32_t *)((&ADCx->ADDR0) + Channel);
    if (!StatusType)
    {
        tmp &= ADC_DR_OVERRUN;
    }
    else
    {
        tmp &= ADC_DR_DONE;
    }

    if (tmp)
        return SET;
    else 
        return RESET;
}

/************************************************************
 * @brief Get adc channel status from ad data register
 * @param[in] ADCx
 * @param[in] StatusType :
                         0: Burst status
                         1: Done  status
 * @return     SET/RESET
 ************************************************************/
uint32_t ADC_GlobalGetStatus(LPC_ADC_TypeDef *ADCx)
{
    uint32_t tmp;

    //tmp = ADCx->ADGDR;
    tmp = ADCx->ADSTAT;
    return (tmp & 0x000000ff);
//     if (StatusType)
//     {
//         tmp &= ADC_DR_DONE;    
//     }
//     else
//     {
//         tmp &= ADC_DR_OVERRUN;
//     }

//     if (tmp)
//     {
//         return SET;
//     }
//     else
//     {
//         return RESET;
//     }
}

#endif
