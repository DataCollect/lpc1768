/********************************************************************
 * @file lpc17xx_rtc.c
 * @brief  contains all functions about rtc
 * @version 1.0
 * @data 2010.6.4
 * @author
 *******************************************************************/
#include "lpc17xx_rtc.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_cfg.h"

#ifdef _RTC

/******************************************************
 * @brief intializes the RTC 
 * @param[in] RTCx
 * @return    none
 *****************************************************/
void RTC_Init(LPC_RTC_TypeDef *RTCx)
{
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCRTC, ENABLE);

    //clear all register to be default
    RTCx->ILR = 0x00;
    RTCx->CCR = 0x00;
    RTCx->CIIR = 0x00;
    RTCx->AMR  = 0xff;
    RTCx->CALIBRATION = 0x00;
}

/******************************************************
 * @breif close the RTC
 * @param[in] RTCx
 * @return    none
 ******************************************************/
void RTC_DeInit(LPC_RTC_TypeDef *RTCx)
{
    RTCx->CCR = 0x00; 
    
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCRTC, DISABLE);   
}

/******************************************************
 * @brief reset clock tick counter in RTC 
 * @param[in] RTCx
 * @return    none
 ******************************************************/
void RTC_ResetClockTickCounter(LPC_RTC_TypeDef *RTCx)
{
    RTCx->CCR |= RTC_CCR_CTCRST;
    RTCx->CCR &= (~RTC_CCR_CTCRST) & RTC_CCR_BITMASK ;
}

/******************************************************
 * @brief  start/stop rtc
 * @param[in] RTCx 
 * @param[in] NewState :
                        ENABLE
                        DISABLE
 * @return    none
 ******************************************************/
void RTC_Cmd(LPC_RTC_TypeDef *RTCx, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        RTCx->CCR |= RTC_CCR_CLKEN;
    }
    else
    {
        RTCx->CCR &= (~RTC_CCR_CLKEN) & RTC_CCR_BITMASK;
    }
}

/***********************************************************
 * @brief enable / disable counter increment interrupt 
 * @param[in] CntIncrIntType: the counter increment interrupt 
                                    RTC_TIMETYPE_SECOND = 0,
                                    RTC_TIMETYPE_MINUTE,
                                    RTC_TIMETYPE_HOUR,
                                    RTC_TIMETYPE_DAYOFWEEK,
                                    RTC_TIMETYPE_DAYOFMONTH,
                                    RTC_TIMETYPE_DAYOFYEAR,
                                    RTC_TIMETYPE_MONTH,
                                    RTC_TIMETYPE_YEAR                              
 *
 *
 *
 ***********************************************************/
void RTC_CntIncrIntConfig(LPC_RTC_TypeDef *RTCx, uint32_t CntIncrIntType, \
								                  FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        switch (CntIncrIntType)
        {
        case RTC_TIMETYPE_SECOND:
            RTCx->CIIR |= RTC_CIIR_IMSEC; 
            break;
        case RTC_TIMETYPE_MINUTE:
			RTCx->CIIR |= RTC_CIIR_IMMIN;
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->CIIR |= RTC_CIIR_IMHOUR;
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->CIIR |= RTC_CIIR_IMDOW;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->CIIR |= RTC_CIIR_IMDOM;
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->CIIR |= RTC_CIIR_IMDOY;
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->CIIR |= RTC_CIIR_IMMON;
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->CIIR |= RTC_CIIR_IMYEAR;
			break;
        }
    }
    else
    {
        switch (CntIncrIntType)
        {
        case RTC_TIMETYPE_SECOND:
			RTCx->CIIR &= (~RTC_CIIR_IMSEC) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->CIIR &= (~RTC_CIIR_IMMIN) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->CIIR &= (~RTC_CIIR_IMHOUR) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->CIIR &= (~RTC_CIIR_IMDOW) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->CIIR &= (~RTC_CIIR_IMDOM) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->CIIR &= (~RTC_CIIR_IMDOY) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->CIIR &= (~RTC_CIIR_IMMON) & RTC_CIIR_BITMASK;
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->CIIR &= (~RTC_CIIR_IMYEAR) & RTC_CIIR_BITMASK;
			break;
        }    
    }
}

/*********************************************************************
 * @brief Enable/disable alarm interrupt 
 * @param[in] RTCx
 * @param[in] AlarmTimeTyepe :    alarm time interrupt type
                                    RTC_TIMETYPE_SECOND = 0,
                                    RTC_TIMETYPE_MINUTE,
                                    RTC_TIMETYPE_HOUR,
                                    RTC_TIMETYPE_DAYOFWEEK,
                                    RTC_TIMETYPE_DAYOFMONTH,
                                    RTC_TIMETYPE_DAYOFYEAR,
                                    RTC_TIMETYPE_MONTH,
                                    RTC_TIMETYPE_YEAR                              
 * @param[in] NewState :     new state of interrupt ,
                                    ENABLE
                                    DISABLE
 * @return    none
 ********************************************************************/
void RTC_AlarmIntConfig(LPC_RTC_TypeDef *RTCx, uint32_t AlarmTimeType, \
								               FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        switch (AlarmTimeType)
		{
		case RTC_TIMETYPE_SECOND:
			RTCx->AMR |= (RTC_AMR_AMRSEC);
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->AMR |= (RTC_AMR_AMRMIN);
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->AMR |= (RTC_AMR_AMRHOUR);
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->AMR |= (RTC_AMR_AMRDOW);
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->AMR |= (RTC_AMR_AMRDOM);
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->AMR |= (RTC_AMR_AMRDOY);
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->AMR |= (RTC_AMR_AMRMON);
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->AMR |= (RTC_AMR_AMRYEAR);
			break;
		}
    }
    else
    {
        switch (AlarmTimeType)
		{
		case RTC_TIMETYPE_SECOND:
			RTCx->AMR &= (~RTC_AMR_AMRSEC) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_MINUTE:
			RTCx->AMR &= (~RTC_AMR_AMRMIN) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_HOUR:
			RTCx->AMR &= (~RTC_AMR_AMRHOUR) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFWEEK:
			RTCx->AMR &= (~RTC_AMR_AMRDOW) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFMONTH:
			RTCx->AMR &= (~RTC_AMR_AMRDOM) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_DAYOFYEAR:
			RTCx->AMR &= (~RTC_AMR_AMRDOY) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_MONTH:
			RTCx->AMR &= (~RTC_AMR_AMRMON) & RTC_AMR_BITMASK;
			break;
		case RTC_TIMETYPE_YEAR:
			RTCx->AMR &= (~RTC_AMR_AMRYEAR) & RTC_AMR_BITMASK;
			break;
		}
    }
}

/*************************************************************
 * @brief set current time value
 * @param[in] RTCx
 * @param[in] TimeType :
                                    RTC_TIMETYPE_SECOND = 0,
                                    RTC_TIMETYPE_MINUTE,
                                    RTC_TIMETYPE_HOUR,
                                    RTC_TIMETYPE_DAYOFWEEK,
                                    RTC_TIMETYPE_DAYOFMONTH,
                                    RTC_TIMETYPE_DAYOFYEAR,
                                    RTC_TIMETYPE_MONTH,
                                    RTC_TIMETYPE_YEAR  
 * @param[in]  TimeValue  : Tie value to set
 * @return     none
 ************************************************************/
void RTC_SetTime(LPC_RTC_TypeDef *RTCx, uint32_t TimeType, uint32_t TimeValue)
{
    switch (TimeType)
    {
    case RTC_TIMETYPE_SECOND:
        if (TimeValue <= RTC_SECOND_MAX)
            RTCx->SEC = TimeValue & RTC_SEC_MASK;
        break;

    case RTC_TIMETYPE_MINUTE:
        if (TimeValue <= RTC_MINUTE_MAX)
            RTCx->MIN = TimeValue & RTC_MIN_MASK;
        break;

    case RTC_TIMETYPE_HOUR:
        if (TimeValue <= RTC_HOUR_MAX)
            RTCx->HOUR = TimeValue & RTC_HOUR_MASK;
       
    case RTC_TIMETYPE_DAYOFWEEK:
		if (TimeValue<= RTC_DAYOFWEEK_MAX)
    		RTCx->DOW = TimeValue & RTC_DOW_MASK;
		break;

	case RTC_TIMETYPE_DAYOFMONTH:
		if ((TimeValue <= RTC_DAYOFMONTH_MAX) 
		   &&(TimeValue >= RTC_DAYOFMONTH_MIN))
		    RTCx->DOM = TimeValue & RTC_DOM_MASK;
		break;

	case RTC_TIMETYPE_DAYOFYEAR:
		if ((TimeValue <= RTC_DAYOFYEAR_MIN) 
		   && (TimeValue <= RTC_DAYOFYEAR_MAX))
            RTCx->DOY = TimeValue & RTC_DOY_MASK;
		break;

	case RTC_TIMETYPE_MONTH:
		if ((TimeValue <= RTC_MONTH_MIN)
		   &&(TimeValue <= RTC_MONTH_MAX))
		    RTCx->MONTH = TimeValue & RTC_MONTH_MASK;
		break;

	case RTC_TIMETYPE_YEAR:
		if (TimeValue <= RTC_YEAR_MAX)
		    RTCx->YEAR = TimeValue & RTC_YEAR_MASK;
		break;    
        
    }
}

/*********************************************************
 * @brief get current time 
 * @param[in] RTCx
 * @param[in] TimeType :
                                    RTC_TIMETYPE_SECOND ,
                                    RTC_TIMETYPE_MINUTE,
                                    RTC_TIMETYPE_HOUR,
                                    RTC_TIMETYPE_DAYOFWEEK,
                                    RTC_TIMETYPE_DAYOFMONTH,
                                    RTC_TIMETYPE_DAYOFYEAR,
                                    RTC_TIMETYPE_MONTH,
                                    RTC_TIMETYPE_YEAR 
 * @return value of the specified TimeType
 ********************************************************/
uint32_t RTC_Gettime(LPC_RTC_TypeDef *RTCx, uint32_t TimeType)
{
    switch (TimeType)
    {
    case RTC_TIMETYPE_SECOND:
		return (RTCx->SEC & RTC_SEC_MASK);
	case RTC_TIMETYPE_MINUTE:
		return (RTCx->MIN & RTC_MIN_MASK);
	case RTC_TIMETYPE_HOUR:
		return (RTCx->HOUR & RTC_HOUR_MASK);
	case RTC_TIMETYPE_DAYOFWEEK:
		return (RTCx->DOW & RTC_DOW_MASK);
	case RTC_TIMETYPE_DAYOFMONTH:
		return (RTCx->DOM & RTC_DOM_MASK);
	case RTC_TIMETYPE_DAYOFYEAR:
		return (RTCx->DOY & RTC_DOY_MASK);
	case RTC_TIMETYPE_MONTH:
		return (RTCx->MONTH & RTC_MONTH_MASK);
	case RTC_TIMETYPE_YEAR:
		return (RTCx->YEAR & RTC_YEAR_MASK);
	default:
		return (0);
    }
}

/**************************************************************
 * @brief set full time of RTC
 * @param[in]  RTCx
 * @param[in]  pFullTime : pointer to RTC_TIME_Type struct
 * @return     none
 *************************************************************/
void RTC_SetFullTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
   	RTCx->SEC = pFullTime->SEC & RTC_SEC_MASK;
    RTCx->MIN = pFullTime->MIN & RTC_MIN_MASK;
    RTCx->HOUR = pFullTime->HOUR & RTC_HOUR_MASK;

    if ((pFullTime->DOM <= RTC_DAYOFMONTH_MAX) 
	   &&(pFullTime->DOM >= RTC_DAYOFMONTH_MIN)) 
        RTCx->DOM = pFullTime->DOM & RTC_DOM_MASK;
	RTCx->DOW = pFullTime->DOW & RTC_DOW_MASK;
	RTCx->DOY = pFullTime->DOY & RTC_DOY_MASK;

	RTCx->MONTH = pFullTime->MONTH & RTC_MONTH_MASK;
	RTCx->YEAR = pFullTime->YEAR & RTC_YEAR_MASK;
}

/*********************************************************************
 * @brief get full time of rtc
 * @param[in]  RTCx
 * @param[in]  pFullTime: pointer to RTC_TIME_TYPE struct that will be
                          stored time in full
 * @return     none
 *********************************************************************/
void RTC_GetFullTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
    pFullTime->DOM = RTCx->DOM & RTC_DOM_MASK;
	pFullTime->DOW = RTCx->DOW & RTC_DOW_MASK;
	pFullTime->DOY = RTCx->DOY & RTC_DOY_MASK;
	pFullTime->HOUR = RTCx->HOUR & RTC_HOUR_MASK;
	pFullTime->MIN = RTCx->MIN & RTC_MIN_MASK;
	pFullTime->SEC = RTCx->SEC & RTC_SEC_MASK;
	pFullTime->MONTH = RTCx->MONTH & RTC_MONTH_MASK;
	pFullTime->YEAR = RTCx->YEAR & RTC_YEAR_MASK;    
}

/***********************************************************
 * @brief set full alarm time
 * @param[in] RTCx
 * @param[in] pFullTime : pointer to a RTC_TIME_TYPE  struct
                          that contains alarm time
 * @return    none
 ***********************************************************/
void RTC_SetFullAlarmTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
    RTCx->ALDOM = pFullTime->DOM & RTC_DOM_MASK;
	RTCx->ALDOW = pFullTime->DOW & RTC_DOW_MASK;
	RTCx->ALDOY = pFullTime->DOY & RTC_DOY_MASK;
	RTCx->ALHOUR = pFullTime->HOUR & RTC_HOUR_MASK;
	RTCx->ALMIN = pFullTime->MIN & RTC_MIN_MASK;
	RTCx->ALSEC = pFullTime->SEC & RTC_SEC_MASK;
	RTCx->ALMON = pFullTime->MONTH & RTC_MONTH_MASK;
	RTCx->ALYEAR = pFullTime->YEAR & RTC_YEAR_MASK;
}

/**************************************************************
 * @brief get full alarm time 
 * @param[in]  RTCx
 * @param[in]  pFullTime : pointer to RTC_TIME_TYPE struct that will be
                          stored alarm time in full
 * 2return     none
 **************************************************************/
void RTC_GetFullAlarmTime(LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime)
{
	pFullTime->DOM = RTCx->ALDOM & RTC_DOM_MASK;
	pFullTime->DOW = RTCx->ALDOW & RTC_DOW_MASK;
	pFullTime->DOY = RTCx->ALDOY & RTC_DOY_MASK;
	pFullTime->HOUR = RTCx->ALHOUR & RTC_HOUR_MASK;
	pFullTime->MIN = RTCx->ALMIN & RTC_MIN_MASK;
	pFullTime->SEC = RTCx->ALSEC & RTC_SEC_MASK;
	pFullTime->MONTH = RTCx->ALMON & RTC_MONTH_MASK;
	pFullTime->YEAR = RTCx->ALYEAR & RTC_YEAR_MASK;
}

/**********************************************************
 * @brief check wether if specified location interrupt in RTC
   is set or not
 * @param[in] RTCx 
 * @param[in] IntType: interrupt location type 
                     RTC_INT_COUNTER_INCREMENT 
                     RTC_INT_ALARM
 * @return    the State of specified location interrupt in RTC
              SET
              RESET
 **********************************************************/
IntStatus RTC_GetIntPending(LPC_RTC_TypeDef *RTCx, uint32_t IntType)
{
    return ((RTCx->ILR) ? SET : RESET);
}

/************************************************************
 * @brief clear specified location interrupt pending in RTC 
 * @param[in]  RTCx
 * @param[in]  IntType :interrupt location type 
                     RTC_INT_COUNTER_INCREMENT 
                     RTC_INT_ALARM
 * @return     none
 ***********************************************************/
void RTC_ClearIntPending(LPC_RTC_TypeDef *RTCx, uint32_t IntType)
{
    RTCx->ILR = IntType;
}


#endif

