#ifndef __LPC17XX_UART_H__
#define __LPC17XX_UART_H__

#include "lpc17xx.h"
#include "lpc_types.h"

/* UART receiver buffer register */
#define  UART_RBR_MASKBIT  ((uint8_t)0xff)

/* UART transmit hoiding register */
#define  UART_THR_MASKBIT  ((uint8_t)0xff)

/* UART divisor latch register */
#define UART_LOAD_DLL(div)  ((div) & 0xff)
#define UART_LOAD_DLM(div)  (((div) >>8) & 0xff)

/* UART interrupt enable register */
#define UART_IER_RBRINT_EN  ((uint32_t)(1 << 0)) /* RBR interrupt enable */
#define UART_IER_THREINT_EN ((uint32_t)(1 << 1)) /* THR interrupt enable */
#define UART_IER_RLSINT_EN  ((uint32_t)(1 << 2)) /* rx line status interrupt enable */
#define UART_IER_ABEOINT_EN	((uint32_t)(1 << 8)) /* end of auto-baud interrupt enable */
#define UART_IER_ABTOINT_EN	((uint32_t)(1 << 9)) /* auto-baud time-out interrupt enable */

/* UART interrupt identification register */
#define UART_IIR_INTSTAT_PEND ((uint32_t)(1 << 0)) /* interrupt status, 0: pending 1:no pending */
#define UART_IIR_INTID_RLS    ((uint32_t)(3 << 1)) /* interrupt identification : receive line status */
#define UART_IIR_INTID_RDA    ((uint32_t)(2 << 1)) /* interrupt identification : receive data available */
#define UART_IIR_INTID_CTI    ((uint32_t)(6 << 1)) /* interrupt identification : CHARACTER time-out indicator */
#define UART_IIR_INTID_THRE   ((uint32_t)(1 << 1)) /* interrupt identification : THRE interrupt */
#define UART_IIR_FIFO_EN      ((uint32_t)(3 << 6)) /* fifo */
#define UART_IIR_ABEO_INT	  ((uint32_t)(1 << 8)) /* End of auto-baud interrupt */
#define UART_IIR_ABTO_INT	  ((uint32_t)(1 << 9)) /* Auto-baud time-out interrupt */
#define UART_IIR_INTID_MASK	   ((uint32_t)(7<<1))	/*!<Interrupt identification: Interrupt ID mask */

/* UART FIFO control register */
#define UART_FCR_FIFO_EN      ((uint32_t)(1 << 0))  /* uart fifo enable */
#define UART_FCR_RX_RESET     ((uint32_t)(1 << 1))  /* RX fifo RESET */
#define UART_FCR_TX_RESET     ((uint32_t)(1 << 2))  /* tX fifo RESET */
#define UART_FCR_TRG_LEV0     ((uint32_t)(0))       /* RX TRIGGER LEVEL 0 : 1 char */
#define UART_FCR_TRG_LEV1     ((uint32_t)(1 << 6))  /* RX TRIGGER LEVEL 1 : 4*/
#define UART_FCR_TRG_LEV2     ((uint32_t)(2 << 6))  /* RX TRIGGER LEVEL 2 : 8*/
#define UART_FCR_TRG_LEV3     ((uint32_t)(3 << 6))  /* RX TRIGGER LEVEL 3 : 14*/

/* UART LINE CONTROL REGISTER */
#define UART_LCR_WLEN5         ((uint8_t)(0))       /* 5-bit character length */
#define UART_LCR_WLEN6         ((uint8_t)(1 << 0))  /* 5-bit character length */
#define UART_LCR_WLEN7         ((uint8_t)(2 << 0))  /* 5-bit character length */
#define UART_LCR_WLEN8         ((uint8_t)(3 << 0))  /* 5-bit character length */

#define UART_LCR_STOPBIT_1     ((uint8_t)(0))       /* uart one stop bit select*/
#define UART_LCR_STOPBIT_2     ((uint8_t)(1 << 2))  /* uart two stop bit select*/

#define UART_LCR_PARITY_EN     ((uint8_t)(1 << 3))  /* uart parity enable */
#define UART_LCR_PARITY_ODD    ((uint8_t)0)         /*  */
#define UART_LCR_PARITY_EVEN   ((uint8_t)(1 << 4))  /*  */
#define UART_LCR_PARITY_F_1    ((uint8_t)(2 << 4))  /*  */
#define UART_LCR_PARITY_F_2    ((uint8_t)(3 << 4))  /*  */

#define UART_LCR_DLAB_EN       ((uint8_t)(1 << 7))  /* ENABLE access to divisor latches */

/* Uart line status register */
#define UART_LSR_RDR           ((uint8_t)(1 << 0)) /* 0: the fifo is empty 1: the fifo is not empty */
#define UART_LSR_OE            ((uint8_t)(1 << 1)) /* overrun error */
#define UART_LSR_PE            ((uint8_t)(1 << 2)) /* PARITY error */
#define UART_LSR_FE            ((uint8_t)(1 << 3)) /* frming error */
#define UART_LSR_BI            ((uint8_t)(1 << 4)) /* Breaking interrupt */
#define UART_LSR_THRE          ((uint8_t)(1 << 5)) /* TRANSmit holding register empty */
#define UART_LSR_TEMT          ((uint8_t)(1 << 6)) /* temt is set when both thr and tsr are empty */
#define UART_LSR_RXFE          ((uint8_t)(1 << 7)) /* ERROR IN RX FIFO */

/* UART FRACTIONAL DIVIDER REGISTER  */
#define UART_FDR_DIVADDVAL(n)  ((uint32_t)(n & 0x0f))  /* baud-rate generation pre-scaler divisor value */
#define UART_FDR_MULVAL(n)     ((uint32_t)((n << 4) & 0xf0)) /* baud-rate pre-scaller value */

/* UART Tx Enable register */
#define UART_TER_TXEN			((uint8_t)(1<<7)) /*  Transmit enable bit */
#define UART_TER_BITMASK		((uint8_t)(0x80)) /* UART Transmit Enable Register bit mask */

/* uart databit type definitions */
typedef enum
{
    UART_DATABIT_5 = 0,   /* uart 5 bit data mode*/
    UART_DATABIT_6 ,      /* uart 6 bit data mode*/
    UART_DATABIT_7 ,      /* uart 7 bit data mode*/
    UART_DATABIT_8        /* uart 8 bit data mode*/
}UART_DATABIT_Type;

/* uart stop bit type definitions */
typedef enum
{
    UART_STOPBIT_1 = 0,
    UART_STOPBIT_2 
}UART_STOPBIT_Type;

/* uart parity type definitions */
typedef enum
{
    UART_PARITY_NONE = 0,
    UART_PARITY_ODD,
    UART_PARITY_EVEN,
    UART_PARITY_SP_1,
    UART_PARITY_SP_2
}UART_PARITY_Type;

/* uart fifo level type definitions */
typedef enum
{
    UART_FIFO_TRGLEV0 = 0,
    UART_FIFO_TRGLEV1,
    UART_FIFO_TRGLEV2,
    UART_FIFO_TRGLEV3 
}UART_FIFO_LEVEL_Type;

/* uart interrupt type definitions */
typedef enum
{
    UART_INTCFG_RBR = 0,
    UART_INTCFG_THRE,
    UART_INTCFG_RLS,
    UART_INTCFG_MS,
    UART_INTCFG_CTS,
    UART_INTCFG_ABEO,
    UART_INTCFG_ABTO
}UART_INT_Type; 

/* uart line status type definitions */
typedef enum
{
    UART_LINESTAT_RDR	= UART_LSR_RDR,			/* Line status register: Receive data ready */
	UART_LINESTAT_OE	= UART_LSR_OE,			/* Line status register: Overrun error */
	UART_LINESTAT_PE	= UART_LSR_PE,			/* Line status register: Parity error */
	UART_LINESTAT_FE	= UART_LSR_FE,			/* Line status register: Framing error */
	UART_LINESTAT_BI	= UART_LSR_BI,			/* Line status register: Break interrupt */
	UART_LINESTAT_THRE	= UART_LSR_THRE,		/* Line status register: Transmit holding register empty */
	UART_LINESTAT_TEMT	= UART_LSR_TEMT,		/* Line status register: Transmitter empty */
	UART_LINESTAT_RXFE	= UART_LSR_RXFE			/* Error in RX FIFO */
}UART_LS_Type;

/* uart configuration structure definition */
typedef struct
{
    uint32_t Baud_rate;
    UART_PARITY_Type Parity;
    UART_DATABIT_Type Databits;
    UART_STOPBIT_Type Stopbits;
}UART_CFG_Type;

/* uart fifo configuration structure definition */
typedef struct
{
    FunctionalState FIFO_ResetRxBuf;
    FunctionalState FIFO_ResetTxBuf;
    FunctionalState FIFO_DMAMode;
    UART_FIFO_LEVEL_Type FIFO_Level;
}UART_FIFO_CFG_Type;


/* uart init functions */
void UART_Init(LPC_UART_TypeDef *UARTx, UART_CFG_Type *UART_ConfigStruct);
void UART_DeInit(LPC_UART_TypeDef *UARTx);
void UART_ConfigStructInit(UART_CFG_Type *UART_InitStruct);

/* uart fifo functions */
void UART_FIFOConfig(LPC_UART_TypeDef *UARTx, UART_FIFO_CFG_Type *FIFOCfg);
void UART_FIFOConfigStructInit(UART_FIFO_CFG_Type *UART_FIFOInitStruct);

/* uart get information functions */
uint32_t UART_GetIntId(LPC_UART_TypeDef *UARTx);
uint8_t  UART_GetLineStatus(LPC_UART_TypeDef *UARTx);

/* uart send/receive functions */
void UART_SendByte(LPC_UART_TypeDef *UARTx, uint8_t Data);
uint8_t UART_ReceiveByte(LPC_UART_TypeDef *UARTx);
uint32_t  UART_Send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf, \
                    uint32_t buflen);
uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, \
                 uint32_t buflen);

void UART_IntConfig(LPC_UART_TypeDef *UARTx, UART_INT_Type UARTIntCfg, \
				FunctionalState NewState);

FlagStatus UART_CheckBusy(LPC_UART_TypeDef *UARTx);
void UART_TxCmd(LPC_UART_TypeDef *UARTx, FunctionalState NewState);
#endif
