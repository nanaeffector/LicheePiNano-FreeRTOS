#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "clk.h"
#include "timer.h"
#include "pio.h"
#include "intc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "dbgmsg.h"
#include "audioCodecTask.h"
#include "audioFxTask.h"

// 関数プロトタイプ.
static void showBootMsg(void);
static void settingPeripheral(void);
void vTestTask(void *vParameters);

// グローバル変数.
xTaskHandle xTestTask;

void main(void){
	// 周辺設定. 
	settingPeripheral();
	
	// Boot1 起動メッセージ. 
	showBootMsg();
	
	// task作成. create task.
	xTaskCreate( vTestTask, "TestTask", 192, NULL, 2, &xTestTask);
	dbgMsgTaskInit();
	createAudioCodecTask();
	createAudioFxTask();
	
	// taskスケジューリング開始.
	vTaskStartScheduler();
	
	// ここにはこない.
	while(1);
}

static void showBootMsg(void){
	char buf[128];
	
	// 起動メッセージ.
	uart_putstr(UART_0, "Boot1 Finished.\r\nHello, World!!\r\n");
	
	// CPUクロック表示.
	sprintf(buf, "Freq. CPU:%ld, PERIPH:%ld, AHB:%ld, APB:%ld\r\n", getCPUClock(), getPeriphClock(), getAHBClock(), getAPBClock());
	uart_putstr(UART_0, buf);
}

static void timer0_callback(void){
	// uart output.
	dbgMsgPrintln("TMR0 Interrupt.", pdTRUE);
}

static void settingPeripheral(void){
	_st_PinItem* item;
	
	// LED.
	item = pio_getPinItemPointer(PE3);
	pio_pinSetting(item->port, item->num, PIO_PULL_NONE, PIO_CFG_OUTPUT, PIO_STRONG_LV3);
	pio_write(item->port, item->num, 1);
	
	// UART.
	uart_init(UART_0, PE1_UART0_TX, PE0_UART0_RX, PIN_NOT_USE, PIN_NOT_USE);
	uart_set_baud(UART_0, 115200);
	uart_set_format(UART_0, UART_DATA_8BIT | UART_STOP_1BIT | UART_PARITY_NONE);
	
	// TIMER0.
	_st_TimerConfig cfg = {TimerMode_Continuous, 500000, timer0_callback};	// 0.5sec.
	timer_init(TIMER_0, cfg);
	
	// 割り込みコントローラ設定.
	intc_set_priority(IRQ_TIMER0, IRQ_PRI_LV0);		// 優先度をlowestに設定.
	intc_enable_interrupt(IRQ_TIMER0);
	
	timer_start(TIMER_0);
}

void vTestTask(void *vParameters){
	
	while(1){
		dbgMsgPrintln("testTask", pdFALSE);
		vTaskDelay(500 / portTICK_RATE_MS);	// 500msec delay.
	}
}
