#ifndef __VIR_LIMIT_CHECK_H__
#define __VIR_LIMIT_CHECK_H__

#define LIM_MOTOR_UP_BIT   ((uint32_t)1)
#define LIM_MOTOR_DOWN_BIT ((uint32_t)2)
#define LIM_PISTON_BIT     ((uint32_t)4)

typedef void (*LIM_SERV_FUNC)(uint32_t);
extern void vir_limit_check_init(void);
extern void lim_serv_register(LIM_SERV_FUNC serv);

#endif
