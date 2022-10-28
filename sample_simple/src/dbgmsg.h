// author : Nana's Effector.
// date   : 2022/07/20

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#ifndef __DBGMSG_H__
#define __DBGMSG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uart.h"

#define DBGMSG_BUF_SIZE		(0x4000)
#define CharOutput(C)	uart_putc(UART_0, C)

void dbgMsgPrint(char *str, uint32_t isIsr);
void dbgMsgPrintln(char *str, uint32_t isIsr);
void dbgMsgTaskInit(void);
void vDbgMsgTask(void *vParameters);

#ifdef __cplusplus
}
#endif

#endif /* __DBGMSG_H__ */
