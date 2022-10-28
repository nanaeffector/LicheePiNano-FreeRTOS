// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>

#ifndef __SYSTIMER_H__
#define __SYSTIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define getSysTimer()		(msysTimerCount)
#define getSysTimerUSec()	timer_getCurrentValue(msysTimerUnit)
extern volatile uint32_t	msysTimerUnit;
extern volatile uint32_t	msysTimerCount;

#ifdef __cplusplus
}
#endif

#endif /* __SYSTIMER_H__ */
