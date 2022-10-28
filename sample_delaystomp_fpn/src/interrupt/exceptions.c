// author : Nana's Effector.
// date   : 2021/09/22

// ---------------------------------------------------------
// ----- 関数プロトタイプ. ---------------------------------
// ---------------------------------------------------------
// ARM9 Excception Handler. 
void dummy(void);
void UndefinedInstructions_Handler		( void ) __attribute__ ((interrupt("UNDEF"), weak, alias("dummy")));
void SoftwareInterrupt_Handler			( void ) __attribute__ ((interrupt("SWI"), weak, alias("dummy")));
void PrefetchAbort_Handler				( void ) __attribute__ ((interrupt("ABORT"), weak, alias("dummy")));
void DataAbort_Handler					( void ) __attribute__ ((interrupt("ABORT"), weak, alias("dummy")));
void IRQ_Handler						( void ) __attribute__ ((interrupt("IRQ"), weak, alias("dummy")));
void FIQ_Handler						( void ) __attribute__ ((interrupt("FIQ"), weak, alias("dummy")));

// dummy.
void dummy(void){
	// nop.
	asm("NOP");
}

