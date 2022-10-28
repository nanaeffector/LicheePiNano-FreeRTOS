// author : Nana's Effector.
// date   : 2021/10/04.

#include <stdint.h>
#include "busreset.h"
#include "io.h"
#include "timer.h"
#include "uart.h"

#define AVS_CNT_CTL_REG		(0x80)		// AVS Counter Control Reg.
#define AVS_CNT0_REG		(0x84)		// AVS Counter 0 Reg.
#define AVS_CNT1_REG		(0x88)		// AVS Counter 1 Reg.
#define AVS_CNT_DIV_REG		(0x8C)		// AVS Counter Divisor Reg.

#define WDOG_IRQ_EN_REG		(0xA0)		// Watchdog IRQ Enable Reg.
#define WDOG_IRQ_STA_REG	(0xA4)		// Watchdog Status Reg.
#define WDOG_CTRL_REG		(0xB0)		// Watchdog Control Reg.
#define WDOG_CFG_REG		(0xB4)		// Watchdog Configuration Reg.
#define WDOG_MODE_REG		(0xB8)		// Watchdog Mode Reg.

#define CLK_SRC_1US		(24000000 / 1000000)

static TimerCallback mTimerCallback[3] = {(void*)0};

static void reload(uint32_t unit){
	// Timerのリロード.
	TIMER.TMR[unit].Control.Reload = 1;
	while(TIMER.TMR[unit].Control.Reload);
}

void timer_init(uint32_t unit, _st_TimerConfig cfg){
	// Timer設定.
	TIMER.TMR[unit].Interval = CLK_SRC_1US * cfg.period;
	TIMER.TMR[unit].Control.Enable = 0;					// 一旦無効.
	TIMER.TMR[unit].Control.Mode = cfg.isSingleMode;	// 0:連続. 1:シングル.
	TIMER.TMR[unit].Control.Prescale = 0;				// 1/1.
	TIMER.TMR[unit].Control.ClkSrc = 1;					// 24MHz.
	
	// カウンタリロード
	reload(unit);
	
	// コールバック設定.
	mTimerCallback[unit] = cfg.pfCallback;
	
	// 割り込みの有効化.
	if(unit == TIMER_0){
		TIMER.IRQ_STA.TMR0 = 1;
		TIMER.IRQ_EN.TMR0 = (cfg.pfCallback != (void*)0) ? 1 : 0;
	}
	else if(unit == TIMER_1){
		TIMER.IRQ_STA.TMR1 = 1;
		TIMER.IRQ_EN.TMR1 = (cfg.pfCallback != (void*)0) ? 1 : 0;
	}
	else{
		TIMER.IRQ_STA.TMR2 = 1;
		TIMER.IRQ_EN.TMR2 = (cfg.pfCallback != (void*)0) ? 1 : 0;
	}
}

void timer_start(uint32_t unit){
	// timer 有効.
	TIMER.TMR[unit].Control.Enable = 1;
}

void timer_stop(uint32_t unit, uint8_t isReload){
	// timer 無効.
	TIMER.TMR[unit].Control.Enable = 0;
	
	if(isReload){
		// カウンタリロード
		reload(unit);
	}
}

void timer_exit(uint32_t unit){
	timer_stop(unit, 0);
	
	// コールバック設定.
	mTimerCallback[unit] = (void*)0;
	
	// 割り込みの無効化.
	if(unit == TIMER_0){
		TIMER.IRQ_EN.TMR0 = 0;
	}
	else if(unit == TIMER_1){
		TIMER.IRQ_EN.TMR1 = 0;
	}
	else{
		TIMER.IRQ_EN.TMR2 = 0;
	}
}

inline uint32_t timer_getCurrentValue(uint32_t unit){
	return TIMER.TMR[unit].Current;
}

void Timer_0_Handler(void){
	if(mTimerCallback[0]){
		mTimerCallback[0]();
	}
	
	// flag clear.
	TIMER.IRQ_STA.TMR0 = 1;
}

void Timer_1_Handler(void){
	if(mTimerCallback[1]){
		mTimerCallback[1]();
	}
	
	// flag clear.
	TIMER.IRQ_STA.TMR1 = 1;
}

void Timer_2_Handler(void){
	if(mTimerCallback[2]){
		mTimerCallback[2]();
	}
	
	// flag clear.
	TIMER.IRQ_STA.TMR2 = 1;
}
