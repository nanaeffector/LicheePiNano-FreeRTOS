// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>

#ifndef __REG_UART_H__
#define __REG_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef union{
	struct{
		uint32_t RBR		: 8;	// Receiver buffer reg.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_RBR;

typedef union{
	struct{
		uint32_t THR		: 8;	// Transmit Holding reg.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_THR;

#define BAUD_GET_DIV(APBCLK, BAUD)	(APBCLK / BAUD / 16)
typedef union{
	struct{
		uint32_t DLL		: 8;	// Divisor Latch Low reg.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_DLL;

typedef union{
	struct{
		uint32_t DLH		: 8;	// Divisor Latch High reg.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_DLH;

typedef union{
	struct{
		uint32_t ERBFI		: 1;	// Enable Received Data Available Interrupt.
		uint32_t ETBEI		: 1;	// Enable Transmit Holding Register Empty(THRE) Interrupt.
		uint32_t ELSI		: 1;	// Enable Receiver Line Status Interrupt.
		uint32_t EDSSI		: 1;	// Enable Modem Status Interrupt.
		uint32_t 			: 3;
		uint32_t PTIME		: 1;	// Programmable THRE Interrupt Mode Enable.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_IER;

#define IID_NO_INT			(0x1)		// none.
#define IID_RECV_LINE		(0x6)		// highest. overrun/parity/framing erros or break. clear:read line status reg.
#define IID_RECV_DATA		(0x4)		// second. data available. clear: read recv reg or FIFO drops trigger level.
#define IID_RECV_TIMEOUT	(0xC)		// second. 直近の32bit期間(4文字分)に受信FIFOに受信がない、かつ受信FIFOに1文字以上ある場合. clear:read recv reg.
#define IID_THR_EMPTY		(0x2)		// third. THR reg empty or 送信FIFOがスレッショルド未満. clear: read iir reg or write thre reg or 送信FIFOがスレッショルドより上.
#define IID_MODEM_STATUS	(0x0)		// fourth. CTS/DSR/RI/DCD detect. flow controlが有効ならInterruptは発生しない. clear:read modem status reg.
#define IID_BUSY_DETECT		(0x7)		// fifth. UARTがbusyのときにLineControlRegに書き込もうとした場合. clear:read UART status reg.
#define FEFLAG_FIFO_ENALBE	(0x3)
#define FEFLAG_FIFO_DISABLE	(0x0)
typedef union{
	struct{
		uint32_t IID		: 4;	// Interrupt ID.
		uint32_t 			: 2;
		uint32_t FEFLAG		: 2;	// FIFOs Enable Indicate Flag. 00: FIFO Disable. 11: FIFO Enable.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_IIR;

#define FIFO_ENABLE			(0x1)
#define RX_FIFO_RESET		(0x2)
#define TX_FIFO_RESET		(0x4)
#define TFT_EMPTY			(0x0)	// empty.
#define TFT_2CHAR			(0x1)	// 2char.
#define TFT_FIFO_QUARTERS	(0x2)	// FIFOの1/4 (4char).
#define TFT_FIFO_HALF		(0x3)	// FIFOの1/2 (8char).
#define RT_1CHAR			(0x0)	// 1char.
#define RT_FIFO_QUARTERS	(0x1)	// FIFOの1/4 (4char).
#define RT_FIFO_HALF		(0x2)	// FIFOの1/2 (8char).
#define RT_FIFO_2LTF		(0x3)	// FIFO残り2文字 (14～15char).
typedef union{
	struct{
		uint32_t FIFOE		: 1;	// Enable FIFOs.
		uint32_t RFIFOR		: 1;	// Receive FIFO Reset.
		uint32_t XFIFOR		: 1;	// Transmit FIFO Reset.
		uint32_t DMAM		: 1;	// DMA mode. 0:mode0, 1:mode1.
		uint32_t TFT		: 2;	// TX Empty interrupt Trigger Threshold.
		uint32_t RT			: 2;	// RX Availabel interrupt Trigger Threshold.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_FCR;

#define DLS_DATA_5BIT		(0)		// data is 5bit.
#define DLS_DATA_6BIT		(1)		// data is 6bit.
#define DLS_DATA_7BIT		(2)		// data is 7bit.
#define DLS_DATA_8BIT		(3)		// data is 8bit.
#define STOP_1BIT			(0)		// stop bit is 1bit.
#define STOP_1HALF_BIT		(1)		// stop bit is 1.5bit. only DLS reg is zero.
#define STOP_2_BIT			(1)		// stop bit 2bit. only DLS reg is not zero.
#define EPS_ODD_PARITY		(0)		// odd parity.
#define EPS_EVEN_PARITY		(1)		// even parity.
#define DLAB_ACCESS_BUF		(0)		// access to RBR/THR reg.
#define DLAB_ACCESS_DIV		(1)		// access to DLL/DLH reg.
typedef union{
	struct{
		uint32_t DLS		: 2;	// Data length select.
		uint32_t STOP		: 1;	// stop bits select.
		uint32_t PEN		: 1;	// parity enable.
		uint32_t EPS		: 2;	// even parity select.
		uint32_t BC			: 1;	// break control transmitted to receiving device.
		uint32_t DLAB		: 1;	// divisor latch access bit. 0:access to RBR/THR reg. 1:access to DLL/DLH reg
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_LCR;

#define LOOPBACK_DISABLE	(0)
#define LOOPBACK_ENABLE		(1)
#define AUTOFLOW_DISABLE	(0)
#define AUTOFLOW_ENABLE		(1)
#define IRDA_MODE_DISABLE	(0)
#define IRDA_MODE_ENABLE	(1)
typedef union{
	struct{
		uint32_t DTR		: 1;	// DTR output. 0:LOW, 1:HIGH.
		uint32_t RTS		: 1;	// RTS output. 0:LOW, 1:HIGH.
		uint32_t			: 2;
		uint32_t LOOP		: 1;	// loopback mode(for test).
		uint32_t AFCE		: 1;	// auto flow control enable(with FIFO).
		uint32_t SIRE		: 1;	// SIR mode enable(IrDA mode).
		uint32_t 			: 25;
	}BIT;
	uint32_t WORD;
}_st_UART_MCR;

#define uart_isDataReady(UNIT)			(UNIT.LSR.BIT.DR != 0)
#define uart_isOverrunError(UNIT)		(UNIT.LSR.BIT.OE != 0)
#define uart_isParityError(UNIT)		(UNIT.LSR.BIT.PE != 0)
#define uart_isFramingError(UNIT)		(UNIT.LSR.BIT.FE != 0)
#define uart_isBreakInterrupt(UNIT)		(UNIT.LSR.BIT.BI != 0)
#define uart_isTxBufferEmpty(UNIT)		(UNIT.LSR.BIT.THRE != 0)
#define uart_isTransmitterEmpty(UNIT)	(UNIT.LSR.BIT.TEMT != 0)
#define uart_isFIFOError(UNIT)			(UNIT.LSR.BIT.FIFOERR != 0)
typedef union{
	struct{
		uint32_t DR			: 1;	// DR status. 0: no data. 1:data ready.
		uint32_t OE			: 1;	// overrun error. clear:read this reg.
		uint32_t PE			: 1;	// parity error. clear:read this reg.
		uint32_t FE			: 1;	// framing error. clear:read this reg.
		uint32_t BI			: 1;	// break interrupt. clear:read this reg.
		uint32_t THRE		: 1;	// TX holding register empty.	// 送信バッファレジスタに空きあり.
		uint32_t TEMT		: 1;	// Transmitter empty.			// 送信シフトレジスタが空.
		uint32_t FIFOERR	: 1;	// rx data error in FIFO.		// 受信FIFO中にPE/FE/BIが発生したデータあり. clear:read this reg.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_LSR;

typedef union{
	struct{
		uint32_t DCTS		: 1;	// 最後にMSRレジスタがリードされて以降、CTSピンが変化したかどうか.
		uint32_t DDSR		: 1;	// 最後にMSRレジスタがリードされて以降、DSRピンが変化したかどうか.
		uint32_t TERI		: 1;	// 最後にMSRレジスタがリードされて以降、RIピンが変化したかどうか.
		uint32_t DDCD		: 1;	// 最後にMSRレジスタがリードされて以降、DCDピンが変化したかどうか.
		uint32_t CTS		: 1;	// CTS state. 0:deassert. 1:assert.
		uint32_t DSR		: 1;	// DSR state. 0:deassert. 1:assert.
		uint32_t RI			: 1;	// RI state. 0:deassert. 1:assert.
		uint32_t DCD		: 1;	// DCD state. 0:deassert. 1:assert.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_MSR;

typedef union{
	struct{
		uint32_t SCRATCH	: 8;	// temporary storage space. 用途不明.
		uint32_t 			: 24;
	}BIT;
	uint32_t WORD;
}_st_UART_SCH;

typedef union{
	struct{
		uint32_t BUSY		: 1;	// uart is busy.
		uint32_t TFNF		: 1;	// transmit FIFO not full.
		uint32_t TFE		: 1;	// transmit FIFO empty.
		uint32_t RFNE		: 1;	// receive FIFO not empty.
		uint32_t RFF		: 1;	// receive FIFO full.
		uint32_t 			: 27;
	}BIT;
	uint32_t WORD;
}_st_UART_USR;

typedef union{
	struct{
		uint32_t TFL		: 7;	// transmit FIFO level status.
		uint32_t 			: 25;
	}BIT;
	uint32_t WORD;
}_st_UART_TFL;

typedef union{
	struct{
		uint32_t RFL		: 7;	// receive FIFO level status.
		uint32_t 			: 25;
	}BIT;
	uint32_t WORD;
}_st_UART_RFL;

typedef union{
	struct{
		uint32_t HALT_TX	: 1;	// transmit FIFO filled.
		uint32_t CHCFG_BUSY	: 1;	// UARTがBUSYの際にLCRレジスタの変更を行うことを許可.
		uint32_t CHCFG_SET	: 1;	// UARTがBUSYの際にLCRレジスタの変更を確定させる. 0->1、1->0を行うと確定.
		uint32_t 			: 1;
		uint32_t SIR_TX_INV	: 1;	// SIR transmit pulse polarity invert.
		uint32_t SIR_RX_INV	: 1;	// SIR receiver pulse polarity invert.
		uint32_t 			: 26;
	}BIT;
	uint32_t WORD;
}_st_UART_HALT;

#define writeTHR(UNIT, DATA)		(UNIT.ADDR_00.THR.BIT.THR = DATA)
#define readRBR(UNIT)				(UNIT.ADDR_00.RBR.BIT.THR)
#define setBaud(UNIT, REGVAL)		{UNIT.ADDR_00.DLL.BIT.DLL = REGVAL & 0xFF; UNIT.ADDR_04.DLH.BIT.DLH = (REGVAL >> 8) & 0xFF;}
typedef struct{
	union{
		_st_UART_RBR	RBR;				// only read access.
		_st_UART_THR	THR;				// only write access.
		_st_UART_DLL	DLL;
	}ADDR_00;
	union{
		_st_UART_IER	IER;
		_st_UART_DLH	DLH;
	}ADDR_04;
	union{
		_st_UART_IIR	IIR;				// only read access.
		_st_UART_FCR	FCR;				// only write access.
	}ADDR_08;
	_st_UART_LCR		LCR;
	_st_UART_MCR		MCR;
	_st_UART_LSR		LSR;
	_st_UART_MSR		MSR;
	_st_UART_SCH		SCH;
	
	uint32_t			reserve0[0x60/4];
	
	_st_UART_USR		USR;
	_st_UART_TFL		TFL;
	_st_UART_RFL		RFL;
	uint32_t			HSK;	// rserve??
	
	uint32_t			reserve1[0x1C/4];
	
	_st_UART_HALT		HALT;
	
	uint32_t			reserve2[0xC/4];
	
	uint32_t			DBG_DLL;
	uint32_t			DBG_;
}_st_UART;

#define F1C100S_UART0_BASE		(0x01c25000)
#define F1C100S_UART1_BASE		(0x01c25400)
#define F1C100S_UART2_BASE		(0x01c25800)

#define regUART0				(*(volatile _st_UART *)F1C100S_UART0_BASE)
#define regUART1				(*(volatile _st_UART *)F1C100S_UART1_BASE)
#define regUART2				(*(volatile _st_UART *)F1C100S_UART2_BASE)


#ifdef __cplusplus
}
#endif

#endif	// __REG_UART_H__