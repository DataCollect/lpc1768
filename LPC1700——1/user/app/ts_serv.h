#ifndef __TS_SERV_H__
#define __TS_SERV_H__

#include "lpc_types.h"

extern void ts_serv_init(void);
extern void ts_send_byte_str(uint8_t *pstr, uint32_t len);
extern void ts_send_dbyte_str(uint16_t *pstr, uint32_t len);

#endif
