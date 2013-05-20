/***********************************************************
 * @file  lpc17xx_gpio.h
 * @brief GPIO of lpc17xx_gpio
 * @version 1.0
 * @data 2011.5.27
 * @author
 ***********************************************************/
#ifndef __LPC17XX_GPIO_H__
#define __LPC17XX_GPIO_H__

#include"lpc17xx.h"
#include"lpc_types.h"


/* @brief GPIO input/ouput   */
typedef enum
{
    GPIO_DIR_INPUT = 0,
    GPIO_DIR_OUTPUT = 1    
}GPIO_DIR_Status;

/* @breif gpio output_level	*/
typedef enum
{
	GPIO_L_LEVEL = 0,
	GPIO_H_LEVEL = 1
}GPIO_LEVEL_Status;
/* @brief GPIO_CFG_Type */
typedef struct
{
	uint8_t Portnum;    /* 0 ~ 4 */
    uint8_t Pinnum;     /* 0 ~ 31 */
    uint8_t Pindir;     /* 0 ~ 1  */
    uint8_t Pinlevel;    /* pull-up, tri-state, pull-down */
}GPIO_CFG_Type;
/***********************************************
 * @brief gpio port byte type definition
 ***********************************************/
typedef struct
{
    __IO uint8_t FIODIR[4];
        uint32_t RESERVED0[3];
    __IO uint8_t FIOMASK[4];
    __IO uint8_t FIOPIN[4];
    __IO uint8_t FIOSET[4];
    __IO uint8_t FIOCLR[4];
}GPIO_Byte_TypeDef;

/***********************************************
 * @brief gpio port half-word type definition
 ***********************************************/
typedef struct
{
    __IO uint16_t FIODIRL;
    __IO uint16_t FIODIRU;
         uint32_t RESEVED0[3];
    __IO uint16_t FIOMASKL;
    __IO uint16_t FIOMASKU;
    __IO uint16_t FIOPINL;
    __IO uint16_t FIOPINU;
    __IO uint16_t FIOSETL;
    __IO uint16_t FIOSETU;
    __IO uint16_t FIOCLRL;
    __IO uint16_t FIOCLRU;
}GPIO_HalfWord_TypeDef; 


#define GPIO0_Byte ((GPIO_Byte_TypeDef *)(LPC_GPIO0_BASE))
#define GPIO1_Byte ((GPIO_Byte_TypeDef *)(LPC_GPIO1_BASE))
#define GPIO2_Byte ((GPIO_Byte_TypeDef *)(LPC_GPIO2_BASE))
#define GPIO3_Byte ((GPIO_Byte_TypeDef *)(LPC_GPIO3_BASE))
#define GPIO4_Byte ((GPIO_Byte_TypeDef *)(LPC_GPIO4_BASE))

#define GPIO0_HalfWord ((GPIO_HalfWord_TypeDef *)(LPC_GPIO0_BASE))
#define GPIO1_HalfWord ((GPIO_HalfWord_TypeDef *)(LPC_GPIO1_BASE))
#define GPIO2_HalfWord ((GPIO_HalfWord_TypeDef *)(LPC_GPIO2_BASE))
#define GPIO3_HalfWord ((GPIO_HalfWord_TypeDef *)(LPC_GPIO3_BASE))
#define GPIO4_HalfWord ((GPIO_HalfWord_TypeDef *)(LPC_GPIO4_BASE))

/* @brief GPIO op         */
void GPIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir);
void GPIO_SetValue(uint8_t portNum, uint32_t bitValue);
void GPIO_ClearValue(uint8_t portNum, uint32_t bitValue);
uint32_t GPIO_ReadValue(uint8_t portNum);

/*****************************************************************
 * @brief cfg the gpio
 * @param[in] GPIO_CFG_Type : dir and  output
 * @return    none
 *****************************************************************/
extern void GPIO_ConfigDir(GPIO_CFG_Type *gpio_cfg);
/*****************************************************************
 * @brief cfg the output
 * @param[in] GPIO_CFG_Type output
 * @return none
 *****************************************************************/
extern void GPIO_ConfigOutput(GPIO_CFG_Type *gpio_cfg);
/*****************************************************************
 * @brief cfg the read: input level
 * @param[in] GPIO_CFG_Type
 * @return GPIO_LEVEL_Status :GPIO_H_LEVEL OR GPIO_L_LEVEL
 *****************************************************************/
extern GPIO_LEVEL_Status GPIO_ReadInput(GPIO_CFG_Type *gpio_cfg);


/* @brief FIO (byte-accessible) op */
void FIO_ByteSetDir(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t dir);
void FIO_ByteSetMask(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t maskValue);
void FIO_ByteSetValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue);
void FIO_ByteClearValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue);
uint8_t FIO_ByteReadValue(uint8_t portNum, uint8_t byteNum);

/* @brief FIO (halfword-accessible) op */
void FIO_HalfWordSetDir(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t dir);
void FIO_HalfWordSetMask(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t maskValue);
void FIO_HalfWordSetValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue);
void FIO_HalfWordClearValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue);
uint16_t FIO_HalfWordReadValue(uint8_t portNum, uint8_t halfwordNum);

/* @brief FIO (word-accessible) op */
void FIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir);
void FIO_SetValue(uint8_t portNum, uint32_t bitValue);
void FIO_ClearValue(uint8_t portNum, uint32_t bitValue);
void FIO_SetMask(uint8_t portNum, uint32_t bitValue, uint8_t maskValue);
uint32_t FIO_ReadValue(uint8_t portNum);

#endif


