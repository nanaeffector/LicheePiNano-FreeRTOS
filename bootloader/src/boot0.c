// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>
#include <stdio.h>
#include "clk.h"
#include "dram.h"
#include "copyself.h"


// ---------------------------------------------------------
// ----- セクション名定義. ---------------------------------
// ---------------------------------------------------------
// data.
extern uint32_t __sidata;
extern uint32_t __sdata;
extern uint32_t __edata;
// bss.
extern uint32_t __sbss;
extern uint32_t __ebss;
// image start.
extern uint32_t __image_start;

typedef void (*funcMain)(void);
#define CALL_MAIN()		((funcMain)&__image_start)()

// ---------------------------------------------------------
// ----- リセットハンドラ. ---------------------------------
// ---------------------------------------------------------
void boot0_main(void){
	uint32_t *pDest;
	
	// clockの設定.
	clock_init();
	
	// dramの設定.
	sys_dram_init();
	
	// dramに転送.
	sys_copyself();
	
	// data領域の初期化.
	// BROMからロードされるので不要.
	
	// bss領域の初期化.
	for (pDest = &__sbss; pDest < &__ebss; pDest++){
		*pDest = 0x00;
	}
	
	// main側 handler呼び出し.
	//_boot1_main();
	CALL_MAIN();
	
	while(1);
}

