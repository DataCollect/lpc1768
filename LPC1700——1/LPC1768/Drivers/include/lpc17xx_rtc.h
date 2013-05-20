#ifndef __LPC17XX_RTC_H__
#define __LPC17XX_RTC_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/* ILR: interrupt location register definitions */
#define RTC_ILR_BITMASK  ((0x00000003))
#define RTC_ILR_RTCCIF   ((1 << 0)) /* counter increment interrupt */
#define RTC_ILR_RTCALF   ((1 << 1)) /* alarm registers generated an interrupt */

/* ccr: clock control register */
#define RTC_CCR_BITMASK  ((0x00000013))
#define RTC_CCR_CLKEN    (((1 << 0))) /* the time counters are enabled 
                                         or they are disabled so that they may be initialized*/
#define RTC_CCR_CTCRST   ((1 << 1))   /* ctc reset:*/
#define RTC_CCALEN       ((1 << 4))   /* calibration counter enable */

/* CIIR: counter increment interrupt register */
#define RTC_CIIR_IMSEC   ((1 << 0))   /* an increment of the second value generates an interrupt*/
#define RTC_CIIR_IMMIN   ((1 << 1))
#define RTC_CIIR_IMHOUR  ((1 << 2))
#define RTC_CIIR_IMDOM   ((1 << 3))
#define RTC_CIIR_IMDOW	 ((1 << 4))
#define RTC_CIIR_IMDOY	 ((1 << 5))
#define RTC_CIIR_IMMON	 ((1 << 6))
#define RTC_CIIR_IMYEAR	 ((1 << 7))
#define RTC_CIIR_BITMASK ((0xFF)) 

/* alarm mask register */ 
#define RTC_AMR_AMRSEC    ((1 << 0))
#define RTC_AMR_AMRMIN    ((1 << 1))
#define RTC_AMR_AMRHOUR	  ((1 << 2))
#define RTC_AMR_AMRDOM	  ((1 << 3))
#define RTC_AMR_AMRDOW	  ((1 << 4))
#define RTC_AMR_AMRDOY	  ((1 << 5))
#define RTC_AMR_AMRMON	  ((1 << 6))
#define RTC_AMR_AMRYEAR	  ((1 << 7))
#define RTC_AMR_BITMASK   ((0xFF))

/* rtc auxiliary control register */
#define RTC_AUX_RTC_OSCF      ((1 << 4))/* rtc oscillator fail detect flag */
/* rtc auxiliary enable register */
#define RTC_AUXEN_RTC_OSCFEN  ((1 << 4))/* oscillator fail detect interrupt */

/* consolidate time register 0 */
#define RTC_CTIME0_SECONDS_MASK		((0x3F))
#define RTC_CTIME0_MINUTES_MASK		((0x3F00))
#define RTC_CTIME0_HOURS_MASK		((0x1F0000))
#define RTC_CTIME0_DOW_MASK			((0x7000000))

/* consolidate time register 1 */
#define RTC_CTIME1_DOM_MASK			((0x1F))
#define RTC_CTIME1_MONTH_MASK		((0xF00))
#define RTC_CTIME1_YEAR_MASK		((0xFFF0000))

/* consolidate time register 2 */
#define RTC_CTIME2_DOY_MASK         ((0xFFF))

/* Time counter group */
#define RTC_SEC_MASK   (0x0000003F)
#define RTC_MIN_MASK   (0x0000003F)
#define RTC_HOUR_MASK  (0x0000001F)
#define RTC_DOM_MASK   (0x0000001F)
#define RTC_DOW_MASK   (0x00000007)
#define RTC_DOY_MASK   (0x000001FF)
#define RTC_MONTH_MASK (0x0000000F)
#define RTC_YEAR_MASK  (0x00000FFF)

#define RTC_SECOND_MAX		59   /*  Maximum value of second */
#define RTC_MINUTE_MAX		59   /*  Maximum value of minute */
#define RTC_HOUR_MAX		23   /*  Maximum value of hour */
#define RTC_MONTH_MIN		1    /*  Minimum value of month */
#define RTC_MONTH_MAX		12   /*  Maximum value of month */
#define RTC_DAYOFMONTH_MIN 	1    /*  Minimum value of day of month */
#define RTC_DAYOFMONTH_MAX 	31   /*  Maximum value of day of month */
#define RTC_DAYOFWEEK_MAX	6    /*  Maximum value of day of week */
#define RTC_DAYOFYEAR_MIN	1    /*  Minimum value of day of year */
#define RTC_DAYOFYEAR_MAX	366  /*  Maximum value of day of year */
#define RTC_YEAR_MAX		4095 /*  Maximum value of year */

/* Alarm register group */
#define RTC_ALSEC_MASK   (0x0000003F)
#define RTC_ALMIN_MASK   (0x0000003F)
#define RTC_ALHOUR_MASK  (0x0000001F)
#define RTC_ALDOM_MASK   (0x0000001F)
#define RTC_ALDOW_MASK   (0x00000007)
#define RTC_ALDOY_MASK   (0x000001FF)
#define RTC_ALMONTH_MASK (0x0000000F)
#define RTC_ALYEAR_MASK  (0x00000FFF) 


/* @brief time structure definitions for easy manipulate the data */
typedef struct
{
    uint32_t SEC;
    uint32_t MIN;
    uint32_t HOUR;
    uint32_t DOM;
    uint32_t DOW;
    uint32_t DOY;
    uint32_t MONTH;
    uint32_t YEAR;
}RTC_TIME_Type;

/* @brief rtc interrupt source */
typedef enum
{
    RTC_INT_COUNTER_INCREMENT = RTC_ILR_RTCCIF,
    RTC_INT_ALARM = RTC_ILR_RTCALF
}RTC_INT_OPT;

/* @brief RTC time type option */
typedef enum
{
    RTC_TIMETYPE_SECOND = 0,
    RTC_TIMETYPE_MINUTE,
    RTC_TIMETYPE_HOUR,
    RTC_TIMETYPE_DAYOFWEEK,
    RTC_TIMETYPE_DAYOFMONTH,
    RTC_TIMETYPE_DAYOFYEAR,
    RTC_TIMETYPE_MONTH,
    RTC_TIMETYPE_YEAR
}RTC_TIMETYPE_Num;

void RTC_Init(LPC_RTC_TypeDef *RTCx);
void RTC_DeInit(LPC_RTC_TypeDef *RTCx);
void RTC_ResetClockTickCounter(LPC_RTC_TypeDef *RTCx);
void RTC_Cmd(LPC_RTC_TypeDef *RTCx, FunctionalState NewState);
void RTC_CntIncrIntConfig(LPC_RTC_TypeDef *RTCx, uint32_t CntIncrIntType, \
                                                 FunctionalState NewState);
void RTC_AlarmIntConfig(LPC_RTC_TypeDef *RTCx, uint32_t AlamTimeType, \
                                               FunctionalState NewState);
void RTC_SetTime(LPC_RTC_TypeDef *RTCx, uint32_t TimeType, uint32_t TimeValue);
uint32_t RTC_GetTime(LPC_RTC_TypeDef *RTCx, uint32_t TimeType);
void RTC_SetFullTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);
void RTC_GetFullTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);

void RTC_SetAlarmTime(LPC_RTC_TypeDef *RTCx, uint32_t TimeType, uint32_t ALValue);
uint32_t RTC_GetAlarmTime(LPC_RTC_TypeDef *RTCx, uint32_t TimeType);
void RTC_SetFullAlarmTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);
void RTC_GetFullAlarmTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);

IntStatus RTC_GetIntPending(LPC_RTC_TypeDef *RTCx, uint32_t IntType);
void RTC_ClearIntPending(LPC_RTC_TypeDef *RTCx, uint32_t IntType);


#endif
