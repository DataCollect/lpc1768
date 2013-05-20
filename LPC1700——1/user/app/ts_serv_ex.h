#ifndef __TS_SERV_EX_H__
#define __TS_SERV_EX_H__

#include "lpc_types.h"

extern void ts_send_back(uint8_t *buff, uint8_t len);
extern void ts_send_byte(uint8_t dat); //touch screen
typedef void (*TS_COM_CMD_FUNC)(void);
typedef void (*TS_COM_DATA_FUNC)(uint8_t *);
typedef void (*TS_SERV_FUNC)(uint32_t);
//extern void back_light_ctl(uint8_t *buf);
extern void  ts_send_byte_str(uint8_t *pstr, uint32_t len);
extern void  ts_send_byte_str_remain(void);
//extern void ts_init(void);
extern void   ts_serv_ex_init(void);
extern const  TS_COM_CMD_FUNC  ts_com_cmd_serv[]; //TS_COM_SERV_FUNC
extern const  TS_COM_DATA_FUNC ts_com_data_serv[]; //TS_COM_SERV_FUNC

extern void dis_tst(void *);
extern void ts_serv_register(TS_SERV_FUNC serv);

#endif
