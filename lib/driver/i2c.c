// author : Nana's Effector.
// date   : 2021/10/04.

#include "io.h"
#include "clk.h"
#include "pio.h"
#include "i2c.h"
#include "busreset.h"
#include <math.h>
#include "systimer.h"
#include <stdio.h>

#define TWI_ADDR_REG			(0x00)		// Slave Addr reg. スレーブモード時のアドレスを設定.
#define TWI_ADDR_MASK_SLA		(0xFE)		// Slave Addr.
#define TWI_ADDR_MASK_GCE		(0x01)		// General call enable.

#define TWI_XADDR_REG			(0x04)		// Slave Extend addr reg. スレーブモード時の拡張アドレスを設定.
#define TWI_XADDR_MASK_SLAX		(0xFF)		// extend slave address.

#define TWI_DATA_REG			(0x08)		// Data Transmitting or Received reg. データ送信、受信を行うレジスタ.

#define TWI_CNTR_REG			(0x0C)		// Control reg.
#define TWI_CNTR_INTEN_MASK		(0x80)		// Interrupt Enable.
#define TWI_CNTR_INTEN			(0x80)		// 割り込み有効.
#define TWI_CNTR_INTDIS			(0x00)		// 割り込み無効.
#define TWI_CNTR_BUSEN_MASK		(0x40)		// Bus Enable.
#define TWI_CNTR_BUSEN			(0x40)		// バス有効.
#define TWI_CNTR_BUSDIS			(0x00)		// バス無効.
#define TWI_CNTR_M_STA_MASK		(0x20)		// Master Mode Start.
#define TWI_CNTR_M_STA			(0x20)		// マスターモード開始.
#define TWI_CNTR_M_STP_MASK		(0x10)		// マスターモード終了.
#define TWI_CNTR_M_STP			(0x10)		// マスターモード終了.
#define TWI_CNTR_INTFL_MASK		(0x08)		// Interrupt Flag.
#define TWI_CNTR_A_ACK_MASK		(0x04)		// Assert Ack.

#define TWI_STAT_REG			(0x10)		// Status reg.
#define TWI_STAT_STA_MASK		(0xFF)		// Status Code.

#define TWI_CCR_REG				(0x14)		// Clock reg.
#define TWI_CCR_CLK_M_MASK		(0x78)		// CLK_M. 4bit.
#define TWI_CCR_CLK_N_MASK		(0x07)		// CLK_N. 3bit. F0 = APB Clock / 2^N. clk = F0 / (10*(M+1)).
#define TWI_CCR_CLK_M_bit		(3)
#define TWI_CCR_CLK_N_bit		(0)

#define TWI_SRST_REG			(0x18)		// Soft Reset reg.
#define TWI_SRST_SOFT_RST_MASK	(0x01)		// Soft Reset.
#define TWI_SRST_SOFT_RST		(0x01)		// 1でリセット. 0になったら完了.

#define TWI_EFR_REG				(0x1C)		// Enhance Feature reg.
#define TWI_EFR_DBN_MASK		(0x03)		// Data Byte number follow read command control.
#define TWI_EFR_DBN_0B			(0x00)		// no data byte to be wrote after read command.
#define TWI_EFR_DBN_1B			(0x01)		// only 1 byte data to be wrote after read command.
#define TWI_EFR_DBN_2B			(0x02)		// 2 bytes data can be wrote after read command.
#define TWI_EFR_DBN_3B			(0x03)		// 3 bytes data can be wrote after read command.

#define TWI_LCR_REG				(0x20)		// Line Control reg.
#define TWI_LCR_SCL_STATE_H		(0x20)		// SCL_STATE_HIGH.
#define TWI_LCR_SCL_STATE_L		(0x00)		// SCL_STATE_LOW.
#define TWI_LCR_SDA_STATE_H		(0x10)		// SDA_STATE_HIGH.
#define TWI_LCR_SDA_STATE_L		(0x00)		// SDA_STATE_LOW.
#define TWI_LCR_SCL_CTL_H		(0x08)		// SDA_STATE_HIGH.
#define TWI_LCR_SCL_CTL_L		(0x00)		// SDA_STATE_LOW.

#define CLK_F0_10KHZ			(1500000)	// 10kHz時のF0はAPBClockを1.5MHzに分周.
#define CLK_F0_100KHZ			(12000000)	// 100kHz時のF0はAPBClockを12MHzに分周.
#define CLK_F0_400KHZ			(12000000)	// 400kHz時のF0はAPBClockを12MHzに分周.
#define CLK_F0_1MHZ				(24000000)	// 1MHz時のF0はAPBClockを12MHzに分周.

#define TX_DATA(ADDR)			((uint8_t)(ADDR << 1 & 0xFE))
#define RX_DATA(ADDR)			((uint8_t)((ADDR << 1 & 0xFE) | 1))

#define GET_BUF_INDEX(UNIT)		(UNIT == I2C_1 ? 1 : UNIT == I2C_2 ? 2 : 0)

#define I2C_TIMEOUT				(100)	// 1msec.

enum {
	I2C_STAT_BUS_ERROR	= 0x00,
	I2C_STAT_TX_START	= 0x08,
	I2C_STAT_TX_RSTART	= 0x10,
	I2C_STAT_TX_AW_ACK	= 0x18,
	I2C_STAT_TX_AW_NAK	= 0x20,
	I2C_STAT_TXD_ACK	= 0x28,
	I2C_STAT_TXD_NAK	= 0x30,
	I2C_STAT_LOST_ARB	= 0x38,
	I2C_STAT_TX_AR_ACK	= 0x40,
	I2C_STAT_TX_AR_NAK	= 0x48,
	I2C_STAT_RXD_ACK	= 0x50,
	I2C_STAT_RXD_NAK	= 0x58,
	I2C_STAT_IDLE		= 0xf8,
};

static void setMode(uint32_t unit, I2CConfig cfg);
static void setClock(uint32_t unit, uint32_t clk);
static void softReset(uint32_t unit);
static uint32_t waitStatus(uint32_t unit);
static uint32_t startCondition(uint32_t unit);
static uint32_t sendData(uint32_t unit, uint32_t data);
static void stopCondition(uint32_t unit);
static int write(uint32_t unit, uint8_t *pBuf, int len);

static uint8_t bufTx[3][16];
static uint32_t ptrTx[3];
//static uint8_t bufRx[3][16];
//static uint32_t ptrRx[3];

// モード設定.
static void setMode(uint32_t unit, I2CConfig cfg){
	uint32_t addr = unit;
	uint32_t val;
	
	if(cfg.mode == I2CMode_Slave){
		// slave mode.
		write32(addr + TWI_ADDR_REG, ((cfg.slaveAddr << 1) & 0xFE) | (cfg.enGeneralCall & 0x1));
		write32(addr + TWI_XADDR_REG, cfg.slaveExAddr);
		
		// setup interrupt.
		// TODO.
	}
	else{
		write32(addr + TWI_ADDR_REG, 0x00);
		write32(addr + TWI_XADDR_REG, 0x00);
	}
	
	// i2c-bus 有効化.
	val = read32(addr + TWI_CNTR_REG);
	val |= TWI_CNTR_BUSEN;
	write32(addr + TWI_CNTR_REG, val);
}


// クロック設定.
static void setClock(uint32_t unit, uint32_t clk){
	uint32_t addr = unit;
	uint32_t val;
	uint32_t srcclk;
	uint32_t n, m;

	srcclk = getAPBClock();
	if(clk == I2C_CLK_LOW_10KHZ){
		n = log2(srcclk/CLK_F0_10KHZ);	// F0の目標クロックを基に分周比を計算.
		srcclk = srcclk / (pow(2, n));	// f0の実クロック計算.
		m = srcclk / I2C_CLK_LOW_10KHZ / 10 - 1;
	}
	else if(clk == I2C_CLK_STD_100KHZ){
		n = log2(srcclk/CLK_F0_100KHZ);	// F0の目標クロックを基に分周比を計算.
		srcclk = srcclk / (pow(2, n));	// 実クロック計算.
		m = srcclk / I2C_CLK_STD_100KHZ / 10 - 1;
	}
	else if(clk == I2C_CLK_FAST_400KHZ){
		n = log2(srcclk/CLK_F0_400KHZ);	// F0の目標クロックを基に分周比を計算.
		srcclk = srcclk / (pow(2, n));	// 実クロック計算.
		m = srcclk / I2C_CLK_FAST_400KHZ / 10 - 1;
	}
	else{
		// invalid.
		return;
	}
	
	val = ((m << TWI_CCR_CLK_M_bit) & TWI_CCR_CLK_M_MASK) | ((n << TWI_CCR_CLK_N_bit) & TWI_CCR_CLK_N_MASK);
	write32(addr + TWI_CCR_REG, val);
}

static void softReset(uint32_t unit){
	uint32_t addr = unit;
	uint32_t timeout = getSysTimer() + I2C_TIMEOUT;
	
	// soft reset実行.
	write32(addr + TWI_SRST_REG, TWI_SRST_SOFT_RST);
	while(read32(addr + TWI_SRST_REG) & TWI_SRST_SOFT_RST_MASK){
		if(timeout < getSysTimer()){
			break;
		}
	}
}

static uint32_t waitStatus(uint32_t unit){
	uint32_t addr = unit;
	uint32_t timeout = getSysTimer() + I2C_TIMEOUT;
	
	// INTFL待ち.
	while(!(read32(addr + TWI_CNTR_REG) & TWI_CNTR_INTFL_MASK)){
		if(timeout < getSysTimer()){
			return I2C_STAT_BUS_ERROR;
		}
	}
	
	// ステータス情報を返却.
	return read32(unit + TWI_STAT_REG);
}

static uint32_t startCondition(uint32_t unit){
	uint32_t addr = unit;
	uint32_t val;
	uint32_t status;
	uint32_t timeout = getSysTimer() + I2C_TIMEOUT;
	
	// マスターモード移行すると、start conditionが発行される.
	val = read32(addr + TWI_CNTR_REG);
	val |= TWI_CNTR_INTEN | TWI_CNTR_M_STA | TWI_CNTR_A_ACK_MASK;
	write32(addr + TWI_CNTR_REG, val);
	
	// モード移行待ち.
	while(read32(addr + TWI_CNTR_REG) & (TWI_CNTR_M_STA)){
		if(timeout < getSysTimer()){
			break;
		}
	}
	
	// ステータス情報を取得.
	status = waitStatus(unit);
	
	// startコンディションを送付していたら成功.
	return (status == I2C_STAT_TX_START) || (status == I2C_STAT_TX_RSTART);
}

static uint32_t sendData(uint32_t unit, uint32_t data){
	uint32_t addr = unit;
	uint32_t val;
	
	// データ書き込み.
	write32(addr + TWI_DATA_REG, data);

	// INTFLクリア.
	val = read32(addr + TWI_CNTR_REG) & ~TWI_CNTR_INTFL_MASK; 
	write32(addr + TWI_CNTR_REG, val);
	
	// ステータス情報を返却.
	return waitStatus(unit);
}

static void stopCondition(uint32_t unit){
	uint32_t addr = unit;
	uint32_t val;
	uint32_t timeout = getSysTimer() + I2C_TIMEOUT;
	
	// マスターモードを抜けると、ストップコンディションが発行される.
	val = read32(addr + TWI_CNTR_REG);
	val = (val | TWI_CNTR_M_STP) & ~(TWI_CNTR_INTEN | TWI_CNTR_INTFL_MASK);
	write32(addr + TWI_CNTR_REG, val);
	
	// モード移行待ち.
	while(read32(addr + TWI_CNTR_REG) & (TWI_CNTR_M_STP)){
		if(timeout < getSysTimer()){
			break;
		}
	}
}

// return 0以下：何バイト目で失敗したのか、絶対値で示す. 1=成功.
static int write(uint32_t unit, uint8_t *pBuf, int len){
	int i = 0;
	uint32_t status;
	
	// Address + Write-bitを送信.
	status = sendData(unit, pBuf[i]);
	if(status != I2C_STAT_TX_AW_ACK){
		// Address+Write-bit送信後にACK受信していない場合、失敗.
		return -i;
	}
	
	// dataを送信.
	for(i=1; i<len; i++){
		status = sendData(unit, pBuf[i]);
		if(status != I2C_STAT_TXD_ACK){
			// 送信後にACK受信していない場合、失敗.
			return -i;
		}
	}
	
	// 成功.
	return 1;
}

void i2c_init(uint32_t unit, uint32_t clk, I2CConfig cfg){
	_st_PinItem* item;
	
	if(unit == I2C_0){
		// IO占有.
		item = pio_getPinItemPointer(PE11_TWI0_SCK);
		pio_pinSetting(item->port, item->num, PIO_PULL_UP, item->cfg, PIO_STRONG_LV3);
		item = pio_getPinItemPointer(PE12_TWI0_SDA);
		pio_pinSetting(item->port, item->num, PIO_PULL_UP, item->cfg, PIO_STRONG_LV3);
		
		// クロックゲートオープン.
		openClockGate(ClkGateUnit_TWI0);
		
		// twi0 reset -> deassert.
		deassertReset(SoftResetUnit_TWI0);
	}
	else if(unit == I2C_1){
		// TODO.
		return;
	}
	else if(unit == I2C_2){
		// TODO.
		return;
	}
	else{
		// invalid.
		return;
	}

	// クロック設定.
	setClock(unit, clk);
	
	// モード設定.
	setMode(unit, cfg);
	
	softReset(unit);
}

void i2c_startTransmission(uint32_t unit, uint8_t addr){
	uint32_t i = GET_BUF_INDEX(unit);
	
	bufTx[i][0] = TX_DATA(addr);
	ptrTx[i] = 1;
}

void i2c_write(uint32_t unit, uint8_t data){
	uint32_t i = GET_BUF_INDEX(unit);
	
	bufTx[i][ptrTx[i]] = data;
	ptrTx[i]++;
}

TransmissionStatus i2c_endTransmission(uint32_t unit, uint8_t stopBit){
	uint32_t i = GET_BUF_INDEX(unit);
	uint32_t status;
	
	// i2c開始.
	if(!startCondition(unit)){
		// スタートコンディション発行失敗.
		return StartConditionError;
	}
	
	status = write(unit, &bufTx[i][0], ptrTx[i]);
	
	if(!(status > 0)){
		// データ送信失敗.
		
		// ストップコンディション発行.
		stopCondition(unit);
		
		return WriteDataError;
	}
	else{
		// データ送信成功.
		if(stopBit){
			// i2c終了.ストップコンディション発行.
			stopCondition(unit);
		}
		
		return Success;
	}
	
}

TransmissionStatus i2c_probe(uint32_t unit, uint8_t addr){
	// 送信開始.
	i2c_startTransmission(unit, addr);
	
	// 送信完了.
	return i2c_endTransmission(unit, 1);
}

void i2c_exit(uint32_t unit){
	_st_PinItem* item;
	
	if(unit == I2C_0){
		// twi0 reset -> assert.
		assertReset(SoftResetUnit_TWI0);
		
		// クロックゲートを閉じる.
		closeClockGate(ClkGateUnit_TWI0);
		
		// IO開放.
		item = pio_getPinItemPointer(PE11_TWI0_SCK);
		pio_pinSetting(item->port, item->num, PIO_PULL_NONE, PIO_CFG_DISABLE, PIO_STRONG_LV3);
		item = pio_getPinItemPointer(PE12_TWI0_SDA);
		pio_pinSetting(item->port, item->num, PIO_PULL_NONE, PIO_CFG_DISABLE, PIO_STRONG_LV3);
	}
	else if(unit == I2C_1){
		// TODO.
		return;
	}
	else if(unit == I2C_2){
		// TODO.
		return;
	}
	else{
		// invalid.
		return;
	}
	
}
