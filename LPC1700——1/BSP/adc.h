#ifndef __ADC_H__
#define __ADC_H__

#include "lpc_types.h"

typedef void (*SENSOR_SERV)(void * , uint32_t );//void (*SENSOR_SERV)(void *, uint32_t );

extern void adc_start(void);
extern void adc_register(SENSOR_SERV serv);

#endif 
