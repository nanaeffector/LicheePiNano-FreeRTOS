// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>
#include "spiflash.h"
#include "timer.h"
#include "systimer.h"
#include "intc.h"
#include "pio.h"

#define USE_JTAG 1

// ---------------------------------------------------------
// ----- 関数プロトタイプ. ---------------------------------
// ---------------------------------------------------------
void initSysTimer(uint32_t timerUnit);
static void load_code(uint32_t src, void* dest, uint32_t size);
void main(void);

// ---------------------------------------------------------
// ----- セクション名定義. ---------------------------------
// --------------------------------------------------------- 
// bss.
extern uint32_t __sbss;
extern uint32_t __ebss;
// load image.
extern unsigned char __image_start;
extern unsigned char __image_end;
extern unsigned int __flash_load_addr;

// システム初期化関数.
void system_init(void){
	void * mem;
	uint32_t size;
	uint32_t flashaddr;
	uint32_t *pDest;
	_st_PinItem* item;

#ifdef USE_JTAG
	// JTAG設定.
	item = pio_getPinItemPointer(PF0_DBG_MS);
	pio_pinSetting(item->port, item->num, PIO_PULL_NONE, item->cfg, PIO_STRONG_LV3);
	item = pio_getPinItemPointer(PF1_DBG_DI);
	pio_pinSetting(item->port, item->num, PIO_PULL_NONE, item->cfg, PIO_STRONG_LV3);
	item = pio_getPinItemPointer(PF3_DBG_DO);
	pio_pinSetting(item->port, item->num, PIO_PULL_NONE, item->cfg, PIO_STRONG_LV3);
	item = pio_getPinItemPointer(PF5_DBG_CK);
	pio_pinSetting(item->port, item->num, PIO_PULL_NONE, item->cfg, PIO_STRONG_LV3);
#endif
	
	// SPIフラッシュからDRAMにプログラムコードを読み出す.
	mem = (void *)&__image_start;
	size = &__image_end - &__image_start;
	flashaddr = (uint32_t)&__flash_load_addr;
	load_code(flashaddr, mem, size);
	
	// bss領域の初期化.
	for (pDest = &__sbss; pDest < &__ebss; pDest++){
		*pDest = 0x00;
	}
	
	// 割り込みコントローラ初期化.
	intc_init();
	
	// call main.
	main();
}

// image 読み出し.
static void load_code(uint32_t src, void* dest, uint32_t size){
	sys_spi_flash_init();
	sys_spi_flash_read(src, dest, size);
	sys_spi_flash_exit();
}
