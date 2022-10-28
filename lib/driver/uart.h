#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#define UART_0			(0)
#define UART_1			(1)
#define UART_2			(2)

#define UART_DATA_MASK		(0x03)
#define UART_DATA_5BIT		(0x00)		// data is 5bit.
#define UART_DATA_6BIT		(0x01)		// data is 6bit.
#define UART_DATA_7BIT		(0x02)		// data is 7bit.
#define UART_DATA_8BIT		(0x03)		// data is 8bit.

#define UART_STOP_MASK		(0x30)
#define UART_STOP_1BIT		(0x00)		// stop bit is 1bit.
#define UART_STOP_1HALF_BIT	(0x10)		// stop bit is 1.5bit. only DLS reg is zero.
#define UART_STOP_2_BIT		(0x20)		// stop bit 2bit. only DLS reg is not zero.

#define UART_PARITY_MASK	(0xC0)
#define UART_PARITY_NONE	(0x00)		// none parity.
#define UART_PARITY_ODD		(0x40)		// odd parity.
#define UART_PARITY_EVEN	(0x80)		// even parity.


void uart_init(int unit, int pinTx, int pinRx, int pinCts, int pinRts);
void uart_set_baud(int unit, uint32_t baud);
void uart_set_format(int unit, uint32_t format);
void uart_exit(int unit);
void uart_putc(int unit, char c);
void uart_putstr(int unit, const char *str);
	
#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */