#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "lpc_types.h"

#define  TS_RECV_BUFF_LEN  64u

typedef  void (*UART_IRQ_SERV)(void *, void *);
typedef  void (*UART_SEND_SERV)(void);
extern   void serial_irq_serv_register(UART_IRQ_SERV recv, UART_SEND_SERV send);
extern   void serial_init(void);
//extern   void uart0_send_byte(uint8_t dat); //touch screen;
//extern void ts_send_byte(uint8_t dat);
//extern void ts_send_str(uint8_t *pstr, uint32_t len);

#endif
