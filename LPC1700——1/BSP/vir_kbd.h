#ifndef __VIR_KBD_H__
#define __VIR_KBD_H__


#include "in_key.h"

typedef void (*KBD_SERV_FUNC)(uint32_t);
extern void vir_kbd_init(void);
extern void kbd_serv_register(KBD_SERV_FUNC serv);
#endif
