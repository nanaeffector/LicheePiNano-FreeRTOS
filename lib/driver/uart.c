// author : Nana's Effector.
// date   : 2021/09/22

#include "io.h"
#include "clk.h"
#include "pio.h"
#include "busreset.h"
#include "uart.h"
#include "reg_uart.h"


static _st_PinItem* _Tx[3];
static _st_PinItem* _Rx[3];
static _st_PinItem* _Cts[3];
static _st_PinItem* _Rts[3];

void uart_init(int unit, int pinTx, int pinRx, int pinCts, int pinRts){
	// IO占有.
	// TX pin設定.
	_Tx[unit] = pio_getPinItemPointer(pinTx);
	pio_pinSetting(_Tx[unit]->port, _Tx[unit]->num, PIO_PULL_NONE, _Tx[unit]->cfg, PIO_STRONG_LV3);
	// RX pin設定.
	_Rx[unit] = pio_getPinItemPointer(pinRx);
	pio_pinSetting(_Rx[unit]->port, _Rx[unit]->num, PIO_PULL_UP, _Rx[unit]->cfg, PIO_STRONG_LV3);
	if(pinCts != PIN_NOT_USE){
		// CTS pin設定.
		_Cts[unit] = pio_getPinItemPointer(pinCts);
		pio_pinSetting(_Cts[unit]->port, _Cts[unit]->num, PIO_PULL_NONE, _Cts[unit]->cfg, PIO_STRONG_LV3);
	}
	if(pinRts != PIN_NOT_USE){
		// RTS pin設定.
		_Rts[unit] = pio_getPinItemPointer(pinCts);
		pio_pinSetting(_Rts[unit]->port, _Rts[unit]->num, PIO_PULL_NONE, _Rts[unit]->cfg, PIO_STRONG_LV3);
	}
	
	if(unit == UART_0){
		// クロックゲートを開ける.
		openClockGate(ClkGateUnit_UART0);
		// uart0 reset -> deassert.
		deassertReset(SoftResetUnit_UART0);
		
		// 各種初期化.
		regUART0.ADDR_04.IER.WORD = 0x00;	// Interrupt Enable Reg. 初期化.
		regUART0.ADDR_08.FCR.WORD = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET;	// FIFO enable and FIFO reset.
		regUART0.MCR.WORD = 0x00;			// Modem Control Reg. 初期化.
	}
	else if(unit == UART_1){
		// クロックゲートを開ける.
		openClockGate(ClkGateUnit_UART1);
		// uart0 reset -> deassert.
		deassertReset(SoftResetUnit_UART1);
		
		// 各種初期化.
		regUART1.ADDR_04.IER.WORD = 0x00;	// Interrupt Enable Reg. 初期化.
		regUART1.ADDR_08.FCR.WORD = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET;	// FIFO enable and FIFO reset.
		regUART1.MCR.WORD = 0x00;			// Modem Control Reg. 初期化.
	}
	else{
		// クロックゲートを開ける.
		openClockGate(ClkGateUnit_UART2);
		// uart0 reset -> deassert.
		deassertReset(SoftResetUnit_UART2);
		// 各種初期化.
		regUART2.ADDR_04.IER.WORD = 0x00;	// Interrupt Enable Reg. 初期化.
		regUART2.ADDR_08.FCR.WORD = FIFO_ENABLE | RX_FIFO_RESET | TX_FIFO_RESET;	// FIFO enable and FIFO reset.
		regUART2.MCR.WORD = 0x00;			// Modem Control Reg. 初期化.
	}
}

void uart_set_baud(int unit, uint32_t baud){
	uint32_t val;
	
	if(unit == UART_0){
		// Divisor Latch Regへのアクセスを有効化
		regUART0.LCR.BIT.DLAB = DLAB_ACCESS_DIV;

		// ボーレート設定. APB_CLK / (16 * 2byte). reg_val = APB_CLK / baud_rate / 16.
		val = BAUD_GET_DIV(getAPBClock(), baud);
		setBaud(regUART0, val);

		// Divisor Latch Regへのアクセスを無効化
		regUART0.LCR.BIT.DLAB = DLAB_ACCESS_BUF;
	}
	else if(unit == UART_1){
		// Divisor Latch Regへのアクセスを有効化
		regUART1.LCR.BIT.DLAB = DLAB_ACCESS_DIV;

		// ボーレート設定. APB_CLK / (16 * 2byte). reg_val = APB_CLK / baud_rate / 16.
		val = BAUD_GET_DIV(getAPBClock(), baud);
		setBaud(regUART1, val);

		// Divisor Latch Regへのアクセスを無効化
		regUART1.LCR.BIT.DLAB = DLAB_ACCESS_BUF;
	}
	else{
		// Divisor Latch Regへのアクセスを有効化
		regUART2.LCR.BIT.DLAB = DLAB_ACCESS_DIV;

		// ボーレート設定. APB_CLK / (16 * 2byte). reg_val = APB_CLK / baud_rate / 16.
		val = BAUD_GET_DIV(getAPBClock(), baud);
		setBaud(regUART2, val);

		// Divisor Latch Regへのアクセスを無効化
		regUART2.LCR.BIT.DLAB = DLAB_ACCESS_BUF;
	}
	// clock wait.
	// TODO.
}

void uart_set_format(int unit, uint32_t format){
	uint8_t data;
	uint8_t stop;
	uint8_t parityEn;
	uint8_t parity;
	
	switch(format & UART_DATA_MASK){
		case UART_DATA_5BIT:
			data = DLS_DATA_5BIT;
			break;
		case UART_DATA_6BIT:
			data = DLS_DATA_6BIT;
			break;
		case UART_DATA_7BIT:
			data = DLS_DATA_7BIT;
			break;
		case UART_DATA_8BIT:
			data = DLS_DATA_8BIT;
			break;
		default:
			data = DLS_DATA_8BIT;
			break;
	}
	
	switch(format & UART_STOP_MASK){
		case UART_STOP_1BIT:
			stop = STOP_1BIT;
			break;
		case UART_STOP_1HALF_BIT:
			stop = STOP_1HALF_BIT;
			break;
		case UART_STOP_2_BIT:
			stop = STOP_2_BIT;
			break;
		default:
			stop = STOP_1BIT;
			break;
	}
	
	switch(format & UART_PARITY_MASK){
		case UART_PARITY_NONE:
			parityEn = 0;
			parity = 0;
			break;
		case UART_PARITY_ODD:
			parityEn = 1;
			parity = EPS_ODD_PARITY;
			break;
		case UART_PARITY_EVEN:
			parityEn = 1;
			parity = EPS_EVEN_PARITY;
			break;
		default:
			parityEn = 0;
			parity = 0;
			break;
	}
	
	if(unit == UART_0){
		regUART0.LCR.BIT.DLS = data;
		regUART0.LCR.BIT.STOP = stop;
		regUART0.LCR.BIT.PEN = parityEn;
		regUART0.LCR.BIT.EPS = parity;
	}
	else if(unit == UART_1){
		regUART1.LCR.BIT.DLS = data;
		regUART1.LCR.BIT.STOP = stop;
		regUART1.LCR.BIT.PEN = parityEn;
		regUART1.LCR.BIT.EPS = parity;
	}
	else{
		regUART2.LCR.BIT.DLS = data;
		regUART2.LCR.BIT.STOP = stop;
		regUART2.LCR.BIT.PEN = parityEn;
		regUART2.LCR.BIT.EPS = parity;
	}
}

void uart_exit(int unit){
	
	if(unit == UART_0){
		while(!uart_isTransmitterEmpty(regUART0));	// 送信シフトレジスタ空き待ち.
		
		// uart0 reset -> assert.
		assertReset(SoftResetUnit_UART0);
		// クロックゲートを閉じる.
		closeClockGate(ClkGateUnit_UART0);
	}
	else if(unit == UART_1){
		// uart1 reset -> assert.
		assertReset(SoftResetUnit_UART1);
		// クロックゲートを閉じる.
		closeClockGate(ClkGateUnit_UART1);
	}
	else{
		// uart2 reset -> assert.
		assertReset(SoftResetUnit_UART2);
		// クロックゲートを閉じる.
		closeClockGate(ClkGateUnit_UART2);
	}
	
	// IO開放.
	if(_Tx[unit]){
		// TX pin設定.
		pio_pinSetting(_Tx[unit]->port, _Tx[unit]->num, PIO_PULL_NONE, _Tx[unit]->cfg, PIO_STRONG_LV3);
	}
	if(_Rx[unit]){
		// RX pin設定.
		pio_pinSetting(_Rx[unit]->port, _Rx[unit]->num, PIO_PULL_UP, _Rx[unit]->cfg, PIO_STRONG_LV3);
	}
	if(_Cts[unit]){
		// CTS pin設定.
		pio_pinSetting(_Cts[unit]->port, _Cts[unit]->num, PIO_PULL_NONE, _Cts[unit]->cfg, PIO_STRONG_LV3);
	}
	if(_Rts[unit]){
		// RTS pin設定.
		pio_pinSetting(_Rts[unit]->port, _Rts[unit]->num, PIO_PULL_NONE, _Rts[unit]->cfg, PIO_STRONG_LV3);
	}
	
	_Tx[unit] = (void*)0;
	_Rx[unit] = (void*)0;
	_Cts[unit] = (void*)0;
	_Rts[unit] = (void*)0;
}

void uart_putc(int unit, char c){
	if(unit == UART_0){
		while(!uart_isTxBufferEmpty(regUART0));	// THRE待ち.（FIFO空きができるまで待ち).
		
		writeTHR(regUART0, c);
	}
	else if(unit == UART_1){
		while(!uart_isTxBufferEmpty(regUART1));	// THRE待ち.（FIFO空きができるまで待ち).
		
		writeTHR(regUART1, c);
	}
	else{
		while(!uart_isTxBufferEmpty(regUART2));	// THRE待ち.（FIFO空きができるまで待ち).
		
		writeTHR(regUART2, c);
	}
}


// UART0から文字列を出力.
void uart_putstr(int unit, const char *str){
	int i = 0;
	
	while(1){
		if(str[i] != 0){
			uart_putc(unit, str[i]);
			i++;
		}
		else{
			break;
		}
	}
}
