/*****************************************************************
 * @file   lpc17xx_uart.c
 * @brief  contains all functions about uart
 * @version 1.0
 * @data 2010.6.1
 * @author
 *****************************************************************/
#include "lpc17xx_uart.h"
#include "lpc17xx_cfg.h"
#include "lpc17xx_clkpwr.h"

#ifdef _UART0

static Status uart_set_divisors(LPC_UART_TypeDef *UARTx, uint32_t baudrate);

/*****************************************************************
 * @brief determines best dividers to get a target clock rate
 * @param[in]  UARTx    : pointer to selected uart peripheral 
 * @param[in]  baudrate : desired uart baud rate
 * @return     success or error
 *****************************************************************/
static Status uart_set_divisors(LPC_UART_TypeDef *UARTx, uint32_t baudrate)
{
    Status errorStatus = ERROR;

    uint32_t uClk;
    uint32_t d, m, bestd, bestm, tmp;
    uint64_t best_divisor, divisor;
    uint32_t current_error, best_error;
    uint32_t recalcbaud;

    if (UARTx == LPC_UART0)
    {
        uClk = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_UART0);
    }

    /*
        baudrate = uClk / ((16*best_divisor)*(mulFracDiv/(mulFracDiv + dividerAddFracDiv)) 
    */
    

    ///////////////////
    best_error = 0xFFFFFFFF;
    bestd = 0;
    bestm = 0;
    best_divisor = 0;
    
    for (m = 1; m <= 15; m++)
    {
        for (d = 0; d < m; d++)
        {
            divisor = ((uint64_t)uClk << 28) * m / (baudrate * (m + d));
            current_error = divisor & 0xFFFFFFFF;

            tmp = divisor >> 32;

            if (current_error > ((uint32_t)1 << 31))
            {
                current_error = -current_error;
                tmp++;
            }

            if (tmp<1 || tmp>65536)
                continue;

            if (current_error < best_error)
            {
                best_error = current_error;
                best_divisor = tmp;
                bestd = d;
                bestm = m;
                if (best_error == 0)
                    break;
            }
        }

        if (best_error == 0)
                break;
    }

    if (best_divisor == 0)
        return ERROR;

    recalcbaud = (uClk >> 4) * bestm / (best_divisor * (bestm + bestd));

    if (baudrate > recalcbaud)
        best_error = baudrate - recalcbaud;
    else
        best_error = recalcbaud - baudrate;

    best_error = best_error * 100 / baudrate;

    if (best_error < 3)
    {
        UARTx->LCR |= UART_LCR_DLAB_EN;
//         UARTx->DLM  = UART_LOAD_DLM(best_divisor);
//         UARTx->DLL  = UART_LOAD_DLL(best_divisor);
        UARTx->DLM  = 0; //UART_LOAD_DLM(best_divisor);
        UARTx->DLL  = 13;//UART_LOAD_DLL(best_divisor);
        UARTx->LCR &= (~UART_LCR_DLAB_EN);

//         UARTx->FDR  = (UART_FDR_DIVADDVAL(bestm) \
//                       | UART_FDR_MULVAL(bestd)); 
        errorStatus = SUCCESS;

    }
    
    return errorStatus;
}

/******************************************************
 * @brief  initializes the uartx peripheral 
 * @param[in]  UART_ConfigStruct :
               Pointer to a UART_CFG_Type structure that 
               contains the configuration information for the
               specified UART peripheral
 * @return none
 ******************************************************/
void UART_Init(LPC_UART_TypeDef *UARTx, UART_CFG_Type *UART_ConfigStruct)
{
    uint32_t tmp;

    if (UARTx == LPC_UART0)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART0, ENABLE);
    }

    UARTx->FCR  = (UART_FCR_FIFO_EN | UART_FCR_RX_RESET | UART_FCR_TX_RESET);
    UARTx->FCR  = 0;

    /* to check the rx fifo is  empty*/
    while (UARTx->LSR & UART_LSR_RDR) 
    {
        tmp = UARTx->RBR;
    }

    /* to check the thr is empty*/
    UARTx->TER = UART_TER_TXEN;
    while ( !(UARTx->LSR & UART_LSR_THRE));
    UARTx->TER = 0;

    UARTx->IER = 0;  /* disble interrupt */
    UARTx->LCR = 0;  /* set lcr to default value */
    UARTx->ACR = 0;  /* set acr to defualt value */
    //tmp = UARTx->LSR;

    uart_set_divisors(UARTx, UART_ConfigStruct->Baud_rate);

    tmp = (UARTx->LCR & UART_LCR_DLAB_EN);

    switch (UART_ConfigStruct->Databits)
    {
    case UART_DATABIT_5:
        tmp |= UART_LCR_WLEN5;
        break;
    case UART_DATABIT_6:
        tmp |= UART_LCR_WLEN6;
        break;
    case UART_DATABIT_7:
        tmp |= UART_LCR_WLEN7;
        break;
    case UART_DATABIT_8:
        tmp |= UART_LCR_WLEN8;
        break; 
    }

    if (UART_ConfigStruct->Parity != UART_PARITY_NONE)
    {
        tmp |= UART_LCR_PARITY_EN;
    }

    switch (UART_ConfigStruct->Stopbits)
    {
    case UART_STOPBIT_2:
        tmp |= UART_LCR_STOPBIT_2;
        break;
    case  UART_STOPBIT_1:
    default:
        break;
    }

    UARTx->LCR = (uint8_t)(tmp);
}

/****************************************************************
 * @brief close the power of uartx peripheral
 * @param[in]  UARTx
 * @return     none
 *
 *****************************************************************/
void UART_DeInit(LPC_UART_TypeDef *UARTx)
{
    if (UARTx == LPC_UART0)
    {
        CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART0, DISABLE);
    }
}

/*****************************************************************
 * @brief initializes UART_InitStruct member  
 * @param[in] UART_CFG_Type structure
 * @return    none
 *
 *****************************************************************/
void UART_ConfigStructInit(UART_CFG_Type *UART_InitStruct)
{
    UART_InitStruct->Baud_rate = 115200;
    UART_InitStruct->Databits = UART_DATABIT_8;
    UART_InitStruct->Parity = UART_PARITY_NONE;
    UART_InitStruct->Stopbits = UART_STOPBIT_1;
}

/*****************************************************************
 * @brief transmit a single data through uart peripheral
 * @param[in] UARTx
 * @param[in] Data : data to transmit
 * @return    none
 *****************************************************************/
void  UART_SendByte(LPC_UART_TypeDef *UARTx, uint8_t Data)
{
    UARTx->THR = Data;
}

/****************************************************************
 * @brief receive a single data from uart peripheral
 * @param[in] UARTx 
 * @return Data received
 ****************************************************************/
uint8_t UART_ReceiveByte(LPC_UART_TypeDef *UARTx)
{
    return (UARTx->RBR);
}

/****************************************************************
 * @brief send a block of data 
 * @param[in]: UARTx
 * @param[in]: txbuf : pointer to transmit buffer
 * @param[in]: buflen: length of transmit buffer length
 * @return   : number of bytes sent
 ****************************************************************/
uint32_t UART_send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t buflen)
{
    uint32_t  bToSend, bSent, fifo_cnt;
    uint8_t *pChar = txbuf;

    bToSend = buflen;
    bSent   = 0;

    while (bToSend)
    {
        if (!(UARTx->LSR & UART_LSR_THRE))
            break;
        fifo_cnt = 16;    /* tx fifo length */
        while (fifo_cnt && bToSend)
        {
            UART_SendByte(UARTx, (*pChar++));
            bToSend--;
            fifo_cnt--;
            bSent++;
        }
    }

    return bSent;
}

/************************************************************
 * @brief receive a block of data
 * @parm[in] UARTx
 * @param[in] buflen : length of received buffer
 * @param[out] rxbuf : pointer to received buffer
 * @return     number of bytes received 
 ************************************************************/
uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, \
                      uint32_t buflen)
{
    uint32_t bToReceive, bReceive;
    uint8_t *pChar = rxbuf;

    bToReceive = buflen;

    bReceive = 0;
    while (bToReceive)
    {
        if (!(UARTx->LSR & UART_LSR_RDR))
        {
            break;
        }
        else
        {
            (*pChar++) = UART_ReceiveByte(UARTx);
            bReceive++;
            bToReceive--;
        }
    }

    return bReceive;
}

/*********************************************************************
 * @brief enable or disable specified uart interrupt 
 * @param[in] UARTx
 * @param[in] UARTIntCfg: the interrupt flag
              UART_INTCFG_RBR
              UART_INTCFG_THRE,
              UART_INTCFG_RLS,
              UART_INTCFG_MS,
              UART_INTCFG_CTS,
              UART_INTCFG_ABEO,
              UART_INTCFG_ABTO

 * @PARAM[IN] NewState : new state of the uart interrupt type
 * return     None;
 **********************************************************************/
void UART_IntConfig(LPC_UART_TypeDef *UARTx, UART_INT_Type UARTIntCfg, FunctionalState NewState)
{
    uint32_t tmp = 0;

    switch (UARTIntCfg)
    {
    case UART_INTCFG_RBR:
		tmp = UART_IER_RBRINT_EN;
		break;
	case UART_INTCFG_THRE:
		tmp = UART_IER_THREINT_EN;
		break;
	case UART_INTCFG_RLS:
		tmp = UART_IER_RLSINT_EN;
		break;
	case UART_INTCFG_ABEO:
		tmp = UART_IER_ABEOINT_EN;
		break;
	case UART_INTCFG_ABTO:
		tmp = UART_IER_ABTOINT_EN;
		break; 
    }

    if (NewState == ENABLE)
    {
        UARTx->IER |= tmp;
    }
    else
    {
        UARTx->IER &= (~tmp);    
    }
}

/********************************************************
 * @brief config fifo funtion 
 * @param[in] UARTx
 * @param[in] FIFOcfg :Pointer to a UART_FIFO_CFG_Type structure
 * @return    none
 ********************************************************/
void UART_FIFOConfig(LPC_UART_TypeDef *UARTx, UART_FIFO_CFG_Type *FIFOCfg)
{
    uint8_t tmp = 0;

    tmp |= UART_FCR_FIFO_EN;    /* enable fifo */
    switch (FIFOCfg->FIFO_Level)
    {
    case UART_FIFO_TRGLEV0:
        tmp |= UART_FCR_TRG_LEV0;
        break;
    case UART_FIFO_TRGLEV1:
        tmp |= UART_FCR_TRG_LEV1;
        break;
    case UART_FIFO_TRGLEV2:
        tmp |= UART_FCR_TRG_LEV2;
        break;
    case UART_FIFO_TRGLEV3:
    default:
        tmp |= UART_FCR_TRG_LEV3;
        break;
    }

    if (FIFOCfg->FIFO_ResetRxBuf == ENABLE)
    {
        tmp |= UART_FCR_RX_RESET;
    }
    if (FIFOCfg->FIFO_ResetTxBuf == ENABLE)
    {
        tmp |= UART_FCR_TX_RESET;
    }

    UARTx->FCR = tmp;
}

/************************************************************
 * @brief initialize UART_FIFOInitStruct member 
 * @param[in] UART_FIFOInitStruct Pointer to a UART_FIFO_CFG_TYPE 
              structure
 * @return    none
 ************************************************************/
void UART_FIFOConfigStructInit(UART_FIFO_CFG_Type *UART_FIFOInitStruct)
{
    UART_FIFOInitStruct->FIFO_DMAMode = DISABLE;
    UART_FIFOInitStruct->FIFO_ResetRxBuf = ENABLE;
    UART_FIFOInitStruct->FIFO_ResetTxBuf = ENABLE;
    UART_FIFOInitStruct->FIFO_Level = UART_FIFO_TRGLEV2;
}

/*********************************************************************//**
 * @brief		Check whether if UART is busy or not
 * @param[in]	UARTx	UART peripheral selected, should be:
 *  			- LPC_UART0: UART0 peripheral
 * 				- LPC_UART1: UART1 peripheral
 * 				- LPC_UART2: UART2 peripheral
 * 				- LPC_UART3: UART3 peripheral
 * @return		RESET if UART is not busy, otherwise return SET.
 **********************************************************************/
FlagStatus UART_CheckBusy(LPC_UART_TypeDef *UARTx)
{
	if (UARTx->LSR & UART_LSR_TEMT)
    {
		return RESET;
	} 
    else 
    {
		return SET;
	}
}

/************************************************************
 * @brief get current value of Line Status Register
 * @param[in] UARTx
 * @return    none
 *
 ************************************************************/
uint8_t UART_GetLineStatus(LPC_UART_TypeDef *UARTx)
{
    return (UARTx->LSR & 0xFF);
}

/************************************************************
 * @brief get interrupt  identification value
 * @param[in] UARTx
 * @return    current value of UART UIIR register 
 ************************************************************/
uint32_t UART_GetIntId(LPC_UART_TypeDef *UARTx)
{
    return (UARTx->IIR & 0x03cf);
}

/*********************************************************************//**
 * @brief		Enable/Disable transmission on UART TxD pin
 * @param[in]	UARTx	UART peripheral selected, should be:
 *   			- LPC_UART0: UART0 peripheral
 * 				- LPC_UART1: UART1 peripheral
 * 				- LPC_UART2: UART2 peripheral
 * 				- LPC_UART3: UART3 peripheral
 * @param[in]	NewState New State of Tx transmission function, should be:
 * 				- ENABLE: Enable this function
				- DISABLE: Disable this function
 * @return none
 **********************************************************************/
void UART_TxCmd(LPC_UART_TypeDef *UARTx, FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_UARTx(UARTx));
	//CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		if (((LPC_UART1_TypeDef *)UARTx) == LPC_UART1)
		{
			((LPC_UART1_TypeDef *)UARTx)->TER |= UART_TER_TXEN;
		}
		else
		{
			UARTx->TER |= UART_TER_TXEN;
		}
	}
	else
	{
		if (((LPC_UART1_TypeDef *)UARTx) == LPC_UART1)
		{
			((LPC_UART1_TypeDef *)UARTx)->TER &= (~UART_TER_TXEN) & UART_TER_BITMASK;
		}
		else
		{
			UARTx->TER &= (~UART_TER_TXEN) & UART_TER_BITMASK;
		}
	}
}

#endif
