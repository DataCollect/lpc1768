/****************************************************
 * @file        lpc17xx_pinsel.c
 * @brief all functions support for pin connect block 
 * @version   0.0
 * @data      2010.5.25
 * @author 
 ****************************************************/

#include "lpc17xx_pinsel.h"

static void set_PinFunc(uint8_t portnum, uint8_t pinnum, uint8_t funcnum);
static void set_PinResistorMode(uint8_t portnum, uint8_t pinnum, uint8_t modenum);
static void set_PinOpenDrainMode(uint8_t portnum, uint8_t pinnum, uint8_t modenum);

/********************************************************
  * @brief setup the pin selection function
  * @param[in]  portnum :
  *              PINSEL_PORT_0	: Port 0
  * 			 PINSEL_PORT_1	: Port 1
  * 			 PINSEL_PORT_2	: Port 2
  * 		     PINSEL_PORT_3	: Port 3

  * @param[in] pinnum:
                - PINSEL_PIN_0 : Pin 0
				- PINSEL_PIN_1 : Pin 1
				- PINSEL_PIN_2 : Pin 2
				- PINSEL_PIN_3 : Pin 3
				- PINSEL_PIN_4 : Pin 4
				- PINSEL_PIN_5 : Pin 5
				- PINSEL_PIN_6 : Pin 6
				- PINSEL_PIN_7 : Pin 7
				- PINSEL_PIN_8 : Pin 8
				- PINSEL_PIN_9 : Pin 9
				- PINSEL_PIN_10 : Pin 10
				- PINSEL_PIN_11 : Pin 11
				- PINSEL_PIN_12 : Pin 12
				- PINSEL_PIN_13 : Pin 13
				- PINSEL_PIN_14 : Pin 14
				- PINSEL_PIN_15 : Pin 15
				- PINSEL_PIN_16 : Pin 16
				- PINSEL_PIN_17 : Pin 17
				- PINSEL_PIN_18 : Pin 18
				- PINSEL_PIN_19 : Pin 19
				- PINSEL_PIN_20 : Pin 20
				- PINSEL_PIN_21 : Pin 21
				- PINSEL_PIN_22 : Pin 22
				- PINSEL_PIN_23 : Pin 23
				- PINSEL_PIN_24 : Pin 24
				- PINSEL_PIN_25 : Pin 25
				- PINSEL_PIN_26 : Pin 26
				- PINSEL_PIN_27 : Pin 27
				- PINSEL_PIN_28 : Pin 28
				- PINSEL_PIN_29 : Pin 29
				- PINSEL_PIN_30 : Pin 30
				- PINSEL_PIN_31 : Pin 31

  * @param[in]  funcnum
  *				- PINSEL_FUNC_0 : default function
  *				- PINSEL_FUNC_1 : first alternate function
  *				- PINSEL_FUNC_2 : second alternate function
  *				- PINSEL_FUNC_3 : third alternate function
  * @return none
********************************************************/
static void set_PinFunc(uint8_t portnum, uint8_t pinnum, uint8_t funcnum)
{
    uint32_t pinnum_t = pinnum;
    uint32_t pinselreg_index = 2 * portnum;
    uint32_t *pPinCon = (uint32_t *)(&LPC_PINCON->PINSEL0);

    if (pinnum_t > 16)
    {
        pinnum_t -= 16;
        pinselreg_index++; 
    }

    *(uint32_t *)(pPinCon + pinselreg_index) &= ~(0x03UL << (pinnum_t * 2)) ;
    *(uint32_t *)(pPinCon + pinselreg_index) |= ((uint32_t)funcnum << (pinnum_t * 2));
}

/***********************************************************************************
 * @brief setup resistor mode for each pin
 * @param[in]	portnum PORT number,
 * 				- PINSEL_PORT_0	: Port 0
 * 				- PINSEL_PORT_1	: Port 1
 * 				- PINSEL_PORT_2	: Port 2
 * 				- PINSEL_PORT_3	: Port 3
 * @param[in] pinnum 
                - PINSEL_PIN_0 : Pin 0
				- PINSEL_PIN_1 : Pin 1
				- PINSEL_PIN_2 : Pin 2
				- PINSEL_PIN_3 : Pin 3
				- PINSEL_PIN_4 : Pin 4
				- PINSEL_PIN_5 : Pin 5
				- PINSEL_PIN_6 : Pin 6
				- PINSEL_PIN_7 : Pin 7
				- PINSEL_PIN_8 : Pin 8
				- PINSEL_PIN_9 : Pin 9
				- PINSEL_PIN_10 : Pin 10
				- PINSEL_PIN_11 : Pin 11
				- PINSEL_PIN_12 : Pin 12
				- PINSEL_PIN_13 : Pin 13
				- PINSEL_PIN_14 : Pin 14
				- PINSEL_PIN_15 : Pin 15
				- PINSEL_PIN_16 : Pin 16
				- PINSEL_PIN_17 : Pin 17
				- PINSEL_PIN_18 : Pin 18
				- PINSEL_PIN_19 : Pin 19
				- PINSEL_PIN_20 : Pin 20
				- PINSEL_PIN_21 : Pin 21
				- PINSEL_PIN_22 : Pin 22
				- PINSEL_PIN_23 : Pin 23
				- PINSEL_PIN_24 : Pin 24
				- PINSEL_PIN_25 : Pin 25
				- PINSEL_PIN_26 : Pin 26
				- PINSEL_PIN_27 : Pin 27
				- PINSEL_PIN_28 : Pin 28
				- PINSEL_PIN_29 : Pin 29
				- PINSEL_PIN_30 : Pin 30
				- PINSEL_PIN_31 : Pin 31

 * @param[in] modenum
                - PINSEL_PINMODE_PULLUP	: Internal pull-up resistor
				- PINSEL_PINMODE_TRISTATE : Tri-state
				- PINSEL_PINMODE_PULLDOWN : Internal pull-down resistor
 *
 * @return none
 ***********************************************************************************/
static void set_PinResistorMode(uint8_t portnum, uint8_t pinnum, uint8_t modenum)
{
    uint32_t pinnum_t = pinnum;
    uint32_t pinmodereg_index = 2 * portnum;
    uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINMODE0;
    
    if (pinnum_t >= 16)
    {
        pinnum_t -= 16;
        pinmodereg_index++;
    } 

    *(uint32_t *)(pPinCon + pinmodereg_index) &= (~(0x03UL << (pinnum_t * 2)));
    *(uint32_t *)(pPinCon + pinmodereg_index) |= ((uint32_t)(modenum << (pinnum_t * 2)));
}

/*******************************************************************
 * @brief setup open drain mode for each pin
 * @param[in] portnum 
 * 				- PINSEL_PORT_0	: Port 0
 * 				- PINSEL_PORT_1	: Port 1
 * 				- PINSEL_PORT_2	: Port 2
 * 				- PINSEL_PORT_3	: Port 3
 * @param[in]	pinnum	Pin number,
                - PINSEL_PIN_0 : Pin 0
				- PINSEL_PIN_1 : Pin 1
				- PINSEL_PIN_2 : Pin 2
				- PINSEL_PIN_3 : Pin 3
				- PINSEL_PIN_4 : Pin 4
				- PINSEL_PIN_5 : Pin 5
				- PINSEL_PIN_6 : Pin 6
				- PINSEL_PIN_7 : Pin 7
				- PINSEL_PIN_8 : Pin 8
				- PINSEL_PIN_9 : Pin 9
				- PINSEL_PIN_10 : Pin 10
				- PINSEL_PIN_11 : Pin 11
				- PINSEL_PIN_12 : Pin 12
				- PINSEL_PIN_13 : Pin 13
				- PINSEL_PIN_14 : Pin 14
				- PINSEL_PIN_15 : Pin 15
				- PINSEL_PIN_16 : Pin 16
				- PINSEL_PIN_17 : Pin 17
				- PINSEL_PIN_18 : Pin 18
				- PINSEL_PIN_19 : Pin 19
				- PINSEL_PIN_20 : Pin 20
				- PINSEL_PIN_21 : Pin 21
				- PINSEL_PIN_22 : Pin 22
				- PINSEL_PIN_23 : Pin 23
				- PINSEL_PIN_24 : Pin 24
				- PINSEL_PIN_25 : Pin 25
				- PINSEL_PIN_26 : Pin 26
				- PINSEL_PIN_27 : Pin 27
				- PINSEL_PIN_28 : Pin 28
				- PINSEL_PIN_29 : Pin 29
				- PINSEL_PIN_30 : Pin 30
				- PINSEL_PIN_31 : Pin 31

 * @param[in]	modenum  Open drain mode number,
 * 				- PINSEL_PINMODE_NORMAL : Pin is in the normal (not open drain) mode
 * 				- PINSEL_PINMODE_OPENDRAIN : Pin is in the open drain mode
 *
 * @return 		none
 *******************************************************************/
static void set_PinOpenDrainMode(uint8_t portnum, uint8_t pinnum, uint8_t modenum)
{
    uint32_t *pPinCon = (uint32_t *)&LPC_PINCON->PINMODE_OD0;
    
    if (modenum == PINSEL_PINMODE_OPENDRAIN)
    {
        *(uint32_t *)(pPinCon + portnum) |= (0x01UL << pinnum);
    }
    else
    {
        *(uint32_t *)(pPinCon + portnum) &= ~(0x01UL << pinnum);
    }   
}


/*****************************************************************
 * @brief    config pin 
 * @param[in] PinCfg pointer to a PINSEL_CFG_Type structure
 * @return none
 *****************************************************************/
void PINSEL_ConfigPin(PINSEL_CFG_Type *PinCfg)
{
    set_PinFunc(PinCfg->Portnum, PinCfg->Pinnum, PinCfg->Funcnum);
    set_PinResistorMode(PinCfg->Portnum, PinCfg->Pinnum, PinCfg->Pinmode);
    set_PinOpenDrainMode(PinCfg->Portnum, PinCfg->Pinnum, PinCfg->OpenDrain);
}
