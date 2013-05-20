#include "adc.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

/* @brief key_in */
#define  UP_PRESSURE_PORT                       (PINSEL_PORT_0)           //ad0.2
#define  UP_PRESSURE_FUNC                       (PINSEL_FUNC_1)
#define  UP_PRESSURE_PINN                       (PINSEL_PIN_23)
#define  UP_PRESSURE_PINMODE                    (PINSEL_PINMODE_TRISTATE)
#define  UP_PRESSURE_PINMODE_OPENDRAIN          (PINSEL_PINMODE_NORMAL)

#define  BURST_PRESSURE_PORT                    (PINSEL_PORT_0)           //ad0.1
#define  BURST_PRESSURE_FUNC                    (PINSEL_FUNC_1)
#define  BURST_PRESSURE_PINN                    (PINSEL_PIN_24)
#define  BURST_PRESSURE_PINMODE                 (PINSEL_PINMODE_TRISTATE)
#define  BURST_PRESSURE_PINMODE_OPENDRAIN       (PINSEL_PINMODE_NORMAL)

#define  BURST_HEIGHT_PORT                      (PINSEL_PORT_0)           //ad0.3
#define  BURST_HEIGHT_FUNC                      (PINSEL_FUNC_1)
#define  BURST_HEIGHT_PINN                      (PINSEL_PIN_26)
#define  BURST_HEIGHT_PINMODE                   (PINSEL_PINMODE_TRISTATE)
#define  BURST_HEIGHT_PINMODE_OPENDRAIN         (PINSEL_PINMODE_NORMAL)

static void sensor_adc_init(void)
{
    PINSEL_CFG_Type PinCfg;
    
    PinCfg.Funcnum = UP_PRESSURE_FUNC;
    PinCfg.Portnum = UP_PRESSURE_PORT;
    PinCfg.Pinnum  = UP_PRESSURE_PINN;
    PinCfg.Pinmode = UP_PRESSURE_PINMODE;
    PinCfg.OpenDrain = UP_PRESSURE_PINMODE_OPENDRAIN;

    PINSEL_ConfigPin(&PinCfg);   
    
    PinCfg.Pinnum = BURST_PRESSURE_PINN;
    PINSEL_ConfigPin(&PinCfg); 
    
    PinCfg.Pinnum = BURST_HEIGHT_PINN;
    PINSEL_ConfigPin(&PinCfg); 
    
    ADC_Init(LPC_ADC, 200000u);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_3, ENABLE);
    
    //ADC_IntConfig(LPC_ADC,_ADC_INT_EN,ENABLE);
    //ADC_IntConfig(LPC_ADC, ADC_ADINTEN1, ENABLE);
    //ADC_IntConfig(LPC_ADC, ADC_ADINTEN2, ENABLE);
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN3, ENABLE);
    ADC_IntConfig(LPC_ADC, ADC_ADGINTEN, DISABLE);
    
    NVIC_SetPriority(ADC_IRQn, 0x02);
    NVIC_EnableIRQ(ADC_IRQn);
    //ADC_BurstCmd(LPC_ADC,ENABLE)    
    
}

void adc_start(void)
{
    //NVIC_EnableIRQ(ADC_IRQn);
    ADC_BurstCmd(LPC_ADC,ENABLE);    
}

SENSOR_SERV adc_over_serv;

void adc_register(SENSOR_SERV serv)
{
    sensor_adc_init();  
    adc_over_serv = serv;    
}

uint32_t cnt = 0;
#define ADC_DATA_BUFF  4u  //16u  //6u
uint16_t adc_val[3][ADC_DATA_BUFF];

#include "os.h"
void ADC_IRQHandler(void)
{   
    //uint32_t tmp;
    
    //OSIntEnter();
    
//     tmp = ADC_GlobalGetStatus(LPC_ADC);
//     if (tmp & 0x02)
//     {
//         adc_val[0][cnt] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_1); 
//     }
//     
//     if (tmp & 0x04) 
//     {
//         adc_val[1][cnt] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_2);
//     }
//     
//     if (tmp & 0x08)
//     {
//         adc_val[2][cnt] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_3);
//         if (++cnt == ADC_DATA_BUFF)//if (++cnt == ADC_DATA_BUFF - 5)
//         {
//             ADC_BurstCmd(LPC_ADC,DISABLE); 
//             //NVIC_DisableIRQ(ADC_IRQn);
//             cnt = 0;
//             
//             //OSIntEnter();
//             //adc_over_serv(adc_val[1][1], 4);
//             adc_over_serv((void *)&adc_val[0][0], sizeof(adc_val));
//             //OSIntExit();
//         }
//     } 
    //ADC_BurstCmd(LPC_ADC,DISABLE);
    adc_val[0][cnt] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_1);
    adc_val[1][cnt] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
    adc_val[2][cnt] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_3);
    
    if (++cnt == ADC_DATA_BUFF)
    {
        OSIntEnter();
        
        ADC_BurstCmd(LPC_ADC,DISABLE); 
        adc_over_serv((void *)&adc_val[0][0], sizeof(adc_val));
        //NVIC_DisableIRQ(ADC_IRQn);
        cnt = 0;
        
        OSIntExit();  
    }
        
    //OSIntExit();    
}
