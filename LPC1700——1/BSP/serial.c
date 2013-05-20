#include "serial.h"
#include "lpc17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lib_mem.h"

#define  UART0_TXD_PORT                 (PINSEL_PORT_0)
#define  UART0_TXD_FUNC                 (PINSEL_FUNC_1)
#define  UART0_TXD_PINN                 (PINSEL_PIN_2)
#define  UART0_TXD_PINMODE              (PINSEL_PINMODE_PULLUP)
#define  UART0_TXD_PINMODE_OPENDRAIN    (PINSEL_PINMODE_NORMAL)

#define  UART0_RXD_PORT                  (PINSEL_PORT_0)
#define  UART0_RXD_FUNC                  (PINSEL_FUNC_1)
#define  UART0_RXD_PINN                  (PINSEL_PIN_3)
#define  UART0_RXD_PINMODE               (PINSEL_PINMODE_PULLUP)
#define  UART0_RXD_PINMODE_OPENDRAIN     (PINSEL_PINMODE_NORMAL)

void serial_init(void)
{
    PINSEL_CFG_Type PinCfg;
    UART_CFG_Type   UartCfg;
    UART_FIFO_CFG_Type UartFifoCfg;
    
    PinCfg.Funcnum = UART0_TXD_FUNC;
    PinCfg.Portnum = UART0_TXD_PORT;
    PinCfg.Pinnum  = UART0_TXD_PINN;
    PinCfg.Pinmode = UART0_TXD_PINMODE;
    PinCfg.OpenDrain = UART0_TXD_PINMODE_OPENDRAIN;
     
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum  = UART0_RXD_PINN;
    PINSEL_ConfigPin(&PinCfg);         //

    UartCfg.Baud_rate = 115200;
    UartCfg.Databits = UART_DATABIT_8;
    UartCfg.Parity = UART_PARITY_NONE;
    UartCfg.Stopbits = UART_STOPBIT_1;
    
    UART_Init(LPC_UART0, &UartCfg);
    
    UartFifoCfg.FIFO_DMAMode = DISABLE;
    UartFifoCfg.FIFO_ResetRxBuf = ENABLE;
    UartFifoCfg.FIFO_ResetTxBuf = ENABLE;
    UartFifoCfg.FIFO_Level = UART_FIFO_TRGLEV2; //8
    UART_FIFOConfig(LPC_UART0, &UartFifoCfg);
    
    UART_IntConfig(LPC_UART0, UART_INTCFG_RBR, ENABLE);
    //UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, ENABLE);
    UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, DISABLE);
    UART_TxCmd(LPC_UART0, ENABLE);
    
    //Mem_Set(ts_recv, 0, TS_RECV_BUFF_LEN);
    //ts_recv_nbr = 0;
    
    NVIC_SetPriority(UART0_IRQn, 0x01);
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART0_IRQn);
}


UART_IRQ_SERV  recv_serv;
UART_SEND_SERV send_serv;

void serial_irq_serv_register(UART_IRQ_SERV recv, UART_SEND_SERV send)
{
    recv_serv = recv;
    send_serv = send;
}

uint8_t  ts_recv[TS_RECV_BUFF_LEN];
uint32_t ts_recv_nbr;

#include "os.h"
void UART0_IRQHandler (void)
{
    uint32_t intsrc, tmp;

    OSIntEnter();
    
	intsrc = UART_GetIntId(LPC_UART0);
	tmp = intsrc & UART_IIR_INTID_MASK;
    
    switch (tmp)
    {
        case UART_IIR_INTID_RDA:
        case UART_IIR_INTID_CTI:
            recv_serv(ts_recv, &ts_recv_nbr);        
            break;
        case UART_IIR_INTID_THRE:
            send_serv();
            break;
        default:
            break;
            
    }
    
    OSIntExit();
}
