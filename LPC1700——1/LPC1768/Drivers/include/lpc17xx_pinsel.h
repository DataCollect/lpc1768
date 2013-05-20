/***************************************************************
 * @file   lpc17xx_pinsel.h
 * @brief  contains all macro definitions and function prototypes
 *         support for pin connect blocks
 * @version 0.0
 * @data    2011.5.23
 * @author  
 ***************************************************************/
#ifndef __LPC17XX_PINSEL_H__
#define __LPC17XX_PINSEL_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/********************public macros*******************************/

/*****************************************************************
 * @brief macros for port selection
 *****************************************************************/
#define PINSEL_PORT_0 ((0))  /* PORT 0 */
#define PINSEL_PORT_1 ((1))  /* PORT 1 */
#define PINSEL_PORT_2 ((2))  /* PORT 2 */
#define PINSEL_PORT_3 ((3))  /* PORT 3 */
#define PINSEL_PORT_4 ((4))  /* PORT 4 */

/*****************************************************************
 * @macros define  for pin function selection 
 *****************************************************************/
#define PINSEL_FUNC_0  ((0)) /* default function */
#define PINSEL_FUNC_1  ((1)) /* first alternate function */
#define PINSEL_FUNC_2  ((2)) /* second alternate function */
#define PINSEL_FUNC_3  ((3)) /* third alternate function */

/*****************************************************************
 * @macros define for pin number of port
 *****************************************************************/
#define PINSEL_PIN_0   ((0))    /* PIN 0 */
#define PINSEL_PIN_1   ((1))    /* PIN 1 */
#define PINSEL_PIN_2   ((2))    /* PIN 2 */
#define PINSEL_PIN_3   ((3))    /* PIN 3 */
#define PINSEL_PIN_4   ((4))    /* PIN 4 */
#define PINSEL_PIN_5   ((5)) 	/* Pin 5 */
#define PINSEL_PIN_6   ((6)) 	/* Pin 6 */
#define PINSEL_PIN_7   ((7)) 	/* Pin 7 */
#define PINSEL_PIN_8   ((8)) 	/* Pin 8 */
#define PINSEL_PIN_9   ((9)) 	/* Pin 9 */
#define PINSEL_PIN_10  ((10)) 	/* Pin 10 */
#define PINSEL_PIN_11  ((11)) 	/* Pin 11 */
#define PINSEL_PIN_12  ((12)) 	/* Pin 12 */
#define PINSEL_PIN_13  ((13)) 	/* Pin 13 */
#define PINSEL_PIN_14  ((14)) 	/* Pin 14 */
#define PINSEL_PIN_15  ((15)) 	/* Pin 15 */
#define PINSEL_PIN_16  ((16)) 	/* Pin 16 */
#define PINSEL_PIN_17  ((17)) 	/* Pin 17 */
#define PINSEL_PIN_18  ((18)) 	/* Pin 18 */
#define PINSEL_PIN_19  ((19)) 	/* Pin 19 */
#define PINSEL_PIN_20  ((20)) 	/* Pin 20 */
#define PINSEL_PIN_21  ((21)) 	/* Pin 21 */
#define PINSEL_PIN_22  ((22)) 	/* Pin 22 */
#define PINSEL_PIN_23  ((23)) 	/* Pin 23 */
#define PINSEL_PIN_24  ((24)) 	/* Pin 24 */
#define PINSEL_PIN_25  ((25)) 	/* Pin 25 */
#define PINSEL_PIN_26  ((26)) 	/* Pin 26 */
#define PINSEL_PIN_27  ((27)) 	/* Pin 27 */
#define PINSEL_PIN_28  ((28)) 	/* Pin 28 */
#define PINSEL_PIN_29  ((29)) 	/* Pin 29 */
#define PINSEL_PIN_30  ((30)) 	/* Pin 30 */
#define PINSEL_PIN_31  ((31)) 	/* Pin 31 */

/**********************************************************
 * @macros define for pin mode
 **********************************************************/
#define PINSEL_PINMODE_PULLUP   ((0)) /* internal pull-up resistor */
#define PINSEL_PINMODE_TRISTATE ((2)) /* neither pull-up or pull-down */
#define PINSEL_PINMODE_PULLDOWN ((3)) /* internal pull-down */

/**********************************************************
 * @macros define for pin open drain mode(normal/open drain) 
 **********************************************************/
#define PINSEL_PINMODE_NORMAL    ((0)) /* NORMAL MODE */
#define PINSEL_PINMODE_OPENDRAIN ((1)) /* OPENDRAIN MODE */

/***********************************************************
 * @breif pin configuration structure
 ***********************************************************/
typedef struct
{
    uint8_t Portnum;    /* 0 ~ 4 */
    uint8_t Pinnum;     /* 0 ~ 31 */
    uint8_t Funcnum;    /* 0 ~ 3  */
    uint8_t Pinmode;    /* pull-up, tri-state, pull-down */
    uint8_t OpenDrain;
}PINSEL_CFG_Type;

/**********************************************************
 * @brief public function
 **********************************************************/
extern void PINSEL_ConfigPin(PINSEL_CFG_Type *PinCfg);

#endif
