#include <stdint.h>
#include "timer.h"
#include "uart.h"

volatile uint32_t msysTimerUnit = 0;
volatile unsigned long msysTimerCount = 0;

static void sysTimerCallback(void){
	msysTimerCount++;
}

void initSysTimer(uint32_t timerUnit){
	_st_TimerConfig cfg = {TimerMode_Continuous, 1000, sysTimerCallback};

	msysTimerUnit = timerUnit;
	timer_init(timerUnit, cfg);
	
	timer_start(timerUnit);
}

