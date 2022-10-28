// author : Nana's Effector.
// date   : 2022/07/20

#include <stdint.h>
#include <stdio.h>

#include "dbgmsg.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static char dbgMsgBuffer[DBGMSG_BUF_SIZE];
static uint32_t dbgMsgBufferRDPoint;
static uint32_t dbgMsgBufferWRPoint;

xTaskHandle xDbgMsgTask;
SemaphoreHandle_t xSemaphoreBinary;

extern BaseType_t xHigherPriorityTaskWoken;
static void dbgMsgWrite(char *str, uint32_t isIsr){
    if(isIsr){
		xSemaphoreTakeFromISR(xSemaphoreBinary, &xHigherPriorityTaskWoken);
	}
	else{
		xSemaphoreTake(xSemaphoreBinary, portMAX_DELAY);
	}

	// バッファへ書き出し.
	while(*str != 0){
		dbgMsgBuffer[dbgMsgBufferWRPoint] = *str;
		dbgMsgBufferWRPoint++;
		if(dbgMsgBufferWRPoint >= DBGMSG_BUF_SIZE){
			dbgMsgBufferWRPoint = 0;
		}
		str++;
	}
	
	if(isIsr){
		xSemaphoreGiveFromISR(xSemaphoreBinary, &xHigherPriorityTaskWoken);
	}
	else{
    	xSemaphoreGive(xSemaphoreBinary);
	}
}

void dbgMsgPrint(char *str, uint32_t isIsr){
	char buf[32];
	TickType_t tick;

	if(str == 0){
		return;
	}

	if(isIsr){
		tick = xTaskGetTickCountFromISR();
	}
	else{
		tick = xTaskGetTickCount();
	}
	
	// タイムスタンプ書き出し.
	sprintf(buf, "%ld:", tick);
	dbgMsgWrite(buf, isIsr);

	// メッセージ書き出し.
	dbgMsgWrite(str, isIsr);
}

void dbgMsgPrintln(char *str, uint32_t isIsr){
	char buf[32];
	TickType_t tick;

	if(str == 0){
		return;
	}
	
	if(isIsr){
		tick = xTaskGetTickCountFromISR();
	}
	else{
		tick = xTaskGetTickCount();
	}
	
	// タイムスタンプ書き出し.
	sprintf(buf, "%ld:", tick);
	dbgMsgWrite(buf, isIsr);

	// メッセージ書き出し.
	dbgMsgWrite(str, isIsr);

	// 改行書き出し.
	dbgMsgWrite("\r\n", isIsr);
}

static void dbgMsgOutput(void){
	uint32_t wrPoint = dbgMsgBufferWRPoint;
	while(dbgMsgBufferRDPoint != wrPoint){
		CharOutput(dbgMsgBuffer[dbgMsgBufferRDPoint]);
		dbgMsgBufferRDPoint++;
		if(dbgMsgBufferRDPoint >= DBGMSG_BUF_SIZE){
			dbgMsgBufferRDPoint = 0;
		}
	}
}

void dbgMsgTaskInit(void){
	xTaskCreate( vDbgMsgTask, "DbgMsgTask", 192, NULL, 1, &xDbgMsgTask);

	xSemaphoreBinary = xSemaphoreCreateBinary();
    //xSemaphoreGive(xSemaphoreBinary);
}

void vDbgMsgTask(void *vParameters){
	while(1){
		// タスク停止.
		vTaskDelay(100);

		// 溜まっているメッセージを出力.
		dbgMsgOutput();
	}
}
