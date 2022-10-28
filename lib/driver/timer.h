// author : Nana's Effector.
// date   : 2021/09/22

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif


#define TIMER_0				(0)
#define TIMER_1				(1)
#define TIMER_2				(2)

typedef void (*TimerCallback)(void);

typedef struct{
	uint32_t TMR0		:1;
	uint32_t TMR1		:1;
	uint32_t TMR2		:1;
	uint32_t reserve	:29;
}_st_irq_reg;

typedef struct{
	uint32_t Enable		:1;
	uint32_t Reload		:1;
	uint32_t ClkSrc		:2;
	uint32_t Prescale	:3;
	uint32_t Mode		:1;
	uint32_t reserve	:24;
}_st_ctrl_reg;

typedef struct{
	_st_ctrl_reg Control;
	uint32_t Interval;
	uint32_t Current;
	uint32_t reserve;
}_st_timer_reg;

typedef struct{
	_st_irq_reg		IRQ_EN;
	_st_irq_reg		IRQ_STA;
	uint32_t		reserve1;
	uint32_t		reserve2;
	_st_timer_reg	TMR[3];
}_st_timer;
	
typedef enum{
	TimerMode_Continuous = 0,
	TimerMode_Single = 1,
}_e_TimerMode;

typedef struct{
	_e_TimerMode isSingleMode;	// 0:contiuous. 1:single.
	uint32_t period;			// usec.
	TimerCallback pfCallback;	// callback.
}_st_TimerConfig;
	
#define F1C100S_TIMER_BASE		(0x01c20C00)
#define TIMER				(*(volatile _st_timer *)F1C100S_TIMER_BASE)

void timer_init(uint32_t unit, _st_TimerConfig cfg);
void timer_start(uint32_t unit);
void timer_stop(uint32_t unit, uint8_t isReload);
void timer_exit(uint32_t unit);
uint32_t timer_getCurrentValue(uint32_t unit);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */