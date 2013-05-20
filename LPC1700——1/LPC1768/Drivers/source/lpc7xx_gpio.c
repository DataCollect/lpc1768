/**********************************************
 * @file  lpc17xx_gpio.c 
 * @brief  all functions support for gpio
 * @version 1.0
 * @data   2010.5.28
 * @autor
 **********************************************/

#include "lpc17xx_gpio.h"
#include "lpc17xx_cfg.h"

#ifdef _GPIO

static LPC_GPIO_TypeDef *GPIO_GetPointer(uint8_t portNum);
static GPIO_HalfWord_TypeDef *FIO_HalfWordGetPointer(uint8_t portNum);
static GPIO_Byte_TypeDef *FIO_ByteGetPointer(uint8_t portNum);

/*****************************************************
 * @brief get pointer to gpio 
 * @param[in] portNum portNum value, 0 to 4
 * @return pointer to gpio peripheral
 *****************************************************/
static LPC_GPIO_TypeDef *GPIO_GetPointer(uint8_t portNum)
{
    LPC_GPIO_TypeDef *pGPIO = NULL;

    switch (portNum)
    {
    case 0:
        pGPIO = LPC_GPIO0;
        break;
    case 1:
        pGPIO = LPC_GPIO1;
        break;
    case 2:
        pGPIO = LPC_GPIO2;
        break;
    case 3:
        pGPIO = LPC_GPIO3;
        break;
    case 4:
        pGPIO = LPC_GPIO4;
        break;
    default:
        break;
    }

    return pGPIO;

}

/********************************************************
 * @brief get pointer to fio peripheral  in halfword
 * @prarm[in] portNum portNum value, 0 to 4
 * @return pointer to fio peripheral
 ********************************************************/
static GPIO_HalfWord_TypeDef *FIO_HalfWordGetPointer(uint8_t portNum)
{
    GPIO_HalfWord_TypeDef *pFIO = NULL;

    switch (portNum)
    {
    case 0:
        pFIO = GPIO0_HalfWord;
        break;
    case 1:
        pFIO = GPIO1_HalfWord;
        break;
    case 2:
        pFIO = GPIO2_HalfWord;
        break;
    case 3:
        pFIO = GPIO3_HalfWord;
        break;
    case 4:
        pFIO = GPIO4_HalfWord;
        break;
    default:
        break;
    }

    return pFIO;

}

/********************************************************
 * @brief pointer to peripheral in byte
 * @param[in] portNum portNum value
 * @return pointer to fio peripheral
 ********************************************************/
static GPIO_Byte_TypeDef *FIO_ByteGetPointer(uint8_t portNum)
{
    GPIO_Byte_TypeDef *pFIO = NULL;

    switch (portNum)
    {
    case 0:
        pFIO = GPIO0_Byte;
        break;
    case 1:
        pFIO = GPIO1_Byte;
        break;
    case 2:
        pFIO = GPIO2_Byte;
        break;
    case 3:
        pFIO = GPIO3_Byte;
        break;
    case 4:
        pFIO = GPIO4_Byte;
        break;
    default:
        break;
    }

    return pFIO;
}

/**********************************************************************
 * @brief set direction for gpio port
 * @param[in] portNum  : port number value
 * @param[in] bitValue : Value that contains all bits to set direction 
 * @param[in] dir      : direction  value 
                         0: input
                         1: output
 * @return none
 **********************************************************************/
void GPIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir)
{
    LPC_GPIO_TypeDef *pGPIO = GPIO_GetPointer(portNum);

    if (pGPIO != NULL)
    {
        if (dir)
        {
            pGPIO->FIODIR |= bitValue;
        }
        else
        {
            pGPIO->FIODIR &= ~ bitValue;
        }
    }
}

/**************************************************
 * @brief set value for bits that have output direction on gpio port
 * @param[in] portNum  : port number value 
 * @param[in] bitValue : value that contains all bits on gpio to set
 * @return none
 **************************************************/
void GPIO_SetValue(uint8_t portNum, uint32_t bitValue)
{
    LPC_GPIO_TypeDef *pGPIO = GPIO_GetPointer(portNum);

    if (pGPIO != NULL)
    {
        pGPIO->FIOSET = bitValue;
    }
}

/****************************************************************
 * @brief clear value for bits that have output direction on gpio port
 * @param[in] portNum : port number value,
 * @param[in] bitValue: Value that contains all bits on gpio to clear
 * @return none
 ****************************************************************/
void GPIO_ClearValue(uint8_t portNum, uint32_t bitValue)
{
    LPC_GPIO_TypeDef *pGPIO = GPIO_GetPointer(portNum);

    if (pGPIO != NULL)
    {
        pGPIO->FIOCLR = bitValue;
    }
}

/****************************************************************
 * @brief  read current state on port pin that have input direction of gpio
 * @param[in] portNum : port number to read value
 * @return none
 *
 ****************************************************************/
uint32_t GPIO_ReadValue(uint8_t portNum)
{
    LPC_GPIO_TypeDef *pGPIO = GPIO_GetPointer(portNum);

    if (pGPIO != NULL)
    {
        return pGPIO->FIOPIN;
    }

    return 0;
}

/*****************************************************************
 * @brief cfg the gpio
 * @param[in] GPIO_CFG_Type : dir and  output
 * @return    none
 *****************************************************************/
void GPIO_ConfigDir(GPIO_CFG_Type *gpio_cfg)
{
    GPIO_SetDir(gpio_cfg->Portnum, (1UL << gpio_cfg->Pinnum), gpio_cfg->Pindir);
}

/*****************************************************************
 * @brief cfg the output
 * @param[in] GPIO_CFG_Type output
 * @return none
 *****************************************************************/
void GPIO_ConfigOutput(GPIO_CFG_Type *gpio_cfg)
{
	if (gpio_cfg->Pindir == GPIO_DIR_OUTPUT)
	{
		if (gpio_cfg->Pinlevel == GPIO_H_LEVEL)
			GPIO_SetValue(gpio_cfg->Portnum, (1UL << gpio_cfg->Pinnum));
		else
			GPIO_ClearValue(gpio_cfg->Portnum, (1UL << gpio_cfg->Pinnum)); 
	}	
}

/*****************************************************************
 * @brief cfg the read: input level
 * @param[in] GPIO_CFG_Type
 * @return GPIO_LEVEL_Status
 *****************************************************************/
GPIO_LEVEL_Status GPIO_ReadInput(GPIO_CFG_Type *gpio_cfg)
{
//	if (gpio_cfg->Pindir == GPIO_DIR_INPUT)
//	{
		if ((GPIO_ReadValue(gpio_cfg->Portnum) & (1UL << gpio_cfg->Pinnum)) != 0)
			return GPIO_H_LEVEL;
		else
			return GPIO_L_LEVEL; 
//	}		
}
 
/*****************************************************************
 * @brief the same with GPIO_SetDir()
 *****************************************************************/
void FIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir)
{
    GPIO_SetDir(portNum, bitValue, dir);
}

/****************************************************************
 * @brief the same with GPIO_SetValue
 ****************************************************************/
void FIO_SetValue(uint8_t portNum, uint32_t bitValue)
{
    GPIO_SetValue(portNum, bitValue);
}

/****************************************************************
 * @brief the same with GPIO_ClearValue
 ****************************************************************/
void FIO_ClearValue(uint8_t portNum, uint32_t bitValue)
{
    GPIO_ClearValue(portNum,  bitValue);
}

/****************************************************************
 * @brief the same with GPIO_ReadValue
 ****************************************************************/
uint32_t  FIO_ReadValue(uint8_t portNum)
{
    return (GPIO_ReadValue(portNum));    
}

/****************************************************************
 * @brief set mask value for bits in FIO port
 * @param[in] portNum  : port number
 * @param[in] bitValue : value that contains all bits in to set
 * @param[in] maskValue: mask value contains state value for each bit
                          0: not mask
                          1: mask
 * @return none
 ****************************************************************/
void FIO_SetMask(uint8_t portNum, uint32_t bitValue, uint8_t maskValue)
{
    LPC_GPIO_TypeDef *pFIO = GPIO_GetPointer(portNum);

    if (pFIO != NULL)
    {
        if (maskValue)
        {
            pFIO->FIOMASK |= bitValue;
        }
        else
        {
            pFIO->FIOMASK &= ~bitValue;
        }
    }
}

/***************************************************************************
 * @brief  set direction for FIO port in halfword accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] halfwordNum : 0(lower) to 1(upper)
 * @param[in] bitValue    : value that contains all bits in to set direction 
 * @param[in] dir         : direction value
                            0: input
                            1: output
 * @return    none
 ***************************************************************************/
void FIO_HalfWordSetDir(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t dir)
{
    GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (dir)
        {
            if (halfwordNum)
            {
                pFIO->FIODIRU |= bitValue;    
            }
            else
            {
                pFIO->FIODIRL |= bitValue;    
            }
        }
        else
        {
            if (halfwordNum)
            {
                pFIO->FIODIRU &= ~bitValue;    
            }
            else
            {
                pFIO->FIODIRL &= ~bitValue;    
            }
        }
    }
}

/***************************************************************************
 * @brief  set mask value for bits in FIO port in halfword accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] halfwordNum : 0(lower) to 1(upper)
 * @param[in] bitValue    : value that contains all bits in to set direction 
 * @param[in] maskValue   : mask  value contains state value for each bit
                            0: not mask
                            1: mask
 * @return    none
 ***************************************************************************/
void FIO_HalfWordSetMask(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t maskValue)
{
    GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (maskValue)
        {
            if (halfwordNum)
            {
                pFIO->FIOMASKU |= bitValue;    
            }
            else
            {
                pFIO->FIOMASKL |= bitValue;    
            }
        }
        else
        {
            if (halfwordNum)
            {
                pFIO->FIOMASKU &= ~bitValue;    
            }
            else
            {
                pFIO->FIOMASKL &= ~bitValue;    
            }
        }
    }
}

/*************************************************************
 * @brief set bits for fio port in halfword accessible style
 * @param[in] portNum : 0 to 4
 * @param[in] halfwordNum : 0(lower) 1(upper)
 * @param[in] bitValue : value that contains all bits in to set
 * @return    none
 *************************************************************/
void FIO_HalfWordSetValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue)
{
    GPIO_HalfWord_TypeDef * pFIO = FIO_HalfWordGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (halfwordNum)
        {
            pFIO->FIOSETU = bitValue;
        }
        else
        {
            pFIO->FIOSETL = bitValue;
        }
    }
  
}

/*************************************************************
 * @brief clear bits for fio port in halfword accessible style
 * @param[in] portNum : 0 to 4
 * @param[in] halfwordNum : 0(lower) 1(upper)
 * @param[in] bitValue : value that contains all bits in to set
 * @return    none
 *************************************************************/
void FIO_HalfWordClearValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue)
{
    GPIO_HalfWord_TypeDef * pFIO = FIO_HalfWordGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (halfwordNum)
        {
            pFIO->FIOCLRU = bitValue;
        }
        else
        {
            pFIO->FIOCLRL = bitValue;
        }
    }
  
}

/*************************************************************
 * @brief read current state on portpin that have input direction  of GPIO
    in halfword accessible style
 * @param[in] portNum : 0 to 4
 * @param[in] halfwordNum : 0(lower) 1(upper)
 * @return    current state
 *************************************************************/
uint16_t FIO_HalfWordReadValue(uint8_t portNum, uint8_t halfwordNum)
{
    GPIO_HalfWord_TypeDef * pFIO = FIO_HalfWordGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (halfwordNum)
        {
            return (pFIO->FIOPINU);
        }
        else
        {
            return (pFIO->FIOPINL);
        }
    }

    return (0);
}

/***************************************************************************
 * @brief  set direction for FIO port in byte accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] byteNum     : 0 to 3
 * @param[in] bitValue    : value that contains all bits in to set direction 
 * @param[in] dir         : direction value
                            0: input
                            1: output
 * @return    none
 ***************************************************************************/
void FIO_ByteSetDir(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t dir)
{
    GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (dir)
        {
            if (byteNum <= 3)
            {
                pFIO->FIODIR[byteNum] |= bitValue;    
            }
        }
        else
        {
            if (byteNum <= 3)
            {
                pFIO->FIODIR[byteNum] &= ~bitValue;    
            }
        }
    }
}

/***************************************************************************
 * @brief  set mask value for bits in FIO port in byte accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] byteNum     : 0 to 3
 * @param[in] bitValue    : value that contains all bits in to set direction 
 * @param[in] maskValue   : mask value contains state value for each bit
                            0: not mask
                            1: mask
 * @return    none
 ***************************************************************************/
void FIO_ByteSetMask(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t maskValue)
{
    GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (maskValue)
        {
            if (byteNum <= 3)
            {
                pFIO->FIOMASK[byteNum] |= bitValue;    
            }
        }
        else
        {
            if (byteNum <= 3)
            {
                pFIO->FIOMASK[byteNum] &= ~bitValue;    
            }
        }
    }
}

/***************************************************************************
 * @brief  set bits for FIO port in byte accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] byteNum     : 0 to 3
 * @param[in] bitValue    : value that contains all bits in to set direction 
 * @return    none
 ***************************************************************************/
void FIO_ByteSetValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue)
{
    GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (byteNum <= 3)
        {
            pFIO->FIODIR[byteNum] |= bitValue;    
        }
    }
}

/***************************************************************************
 * @brief  clear bits for FIO port in byte accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] byteNum     : 0 to 3
 * @param[in] bitValue    : value that contains all bits in to set direction 
 * @return    none
 ***************************************************************************/
void FIO_ByteClearValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue)
{
    GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (byteNum <= 3)
        {
            pFIO->FIOCLR[byteNum] |= bitValue;    
        }
    }
}

/***************************************************************************
 * @brief  Read Current state on port pin that have input direction of GPIO
           in byte accessible style
 * @param[in] portNum     : 0 to 4
 * @param[in] byteNum     : 0 to 3
 * @return    Current value of FIO port pin of specified byte part
 ***************************************************************************/
uint8_t FIO_ByteReadValue(uint8_t portNum, uint8_t byteNum)
{
    GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

    if (pFIO != NULL)
    {
        if (byteNum <= 3)
        {
            return(pFIO->FIOPIN[byteNum]);    
        }
    }

    return (0);
}

#endif  
