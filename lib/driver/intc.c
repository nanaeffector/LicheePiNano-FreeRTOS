// author : Nana's Effector.
// date   : 2021/10/04.

#include <stdint.h>
#include <stdio.h>
#include "busreset.h"
#include "reg_intc.h"
#include "intc.h"

typedef void (*irq_handler)(void);

// ---------------------------------------------------------
// ----- 関数プロトタイプ. ---------------------------------
// ---------------------------------------------------------
// F1C100s Pripheral Interrupt Handler.
void NMI_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void UART_0_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void UART_1_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void UART_2_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void OWA_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void CIR_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void TWI_0_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void TWI_1_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void TWI_2_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void SPI_0_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void SPI_1_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void Timer_0_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void Timer_1_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void Timer_2_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void Watchdog_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void RSB_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void DMA_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void TouchPanel_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void AudioCodec_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void KEYADC_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void SDC_0_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void SDC_1_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void USB_OTG_Handler					( void ) __attribute__ ((weak, alias("irq_dummy")));
void TVD_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void TVE_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void TCON_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void DE_FE_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void DE_BE_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void CSI_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void DE_Interlacer_Handler				( void ) __attribute__ ((weak, alias("irq_dummy")));
void VE_Handler							( void ) __attribute__ ((weak, alias("irq_dummy")));
void DAUDIO_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void PIOD_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void PIOE_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));
void PIOF_Handler						( void ) __attribute__ ((weak, alias("irq_dummy")));


// dummy.
void irq_dummy(void){
	// nop.
	asm("NOP");
}

// ---------------------------------------------------------
// ----- Peripheral Interrupt Vector. ----------------------
// ---------------------------------------------------------
volatile irq_handler m_irq_handler[41]={
	NMI_Handler,
	UART_0_Handler,
	UART_1_Handler,
	UART_2_Handler,
	irq_dummy,						// reserve.
	OWA_Handler,
	CIR_Handler,
	TWI_0_Handler,
	TWI_1_Handler,
	TWI_2_Handler,
	SPI_0_Handler,
	SPI_1_Handler,
	irq_dummy,						// reserve.
	Timer_0_Handler,
	Timer_1_Handler,
	Timer_2_Handler,
	Watchdog_Handler,
	RSB_Handler,
	DMA_Handler,
	irq_dummy,						// reserve.
	TouchPanel_Handler,
	AudioCodec_Handler,
	KEYADC_Handler,
	SDC_0_Handler,
	SDC_1_Handler,
	irq_dummy,						// reserve.
	USB_OTG_Handler,
	TVD_Handler,
	TVE_Handler,
	TCON_Handler,
	DE_FE_Handler,
	DE_BE_Handler,
	CSI_Handler,
	DE_Interlacer_Handler,
	VE_Handler,
	DAUDIO_Handler,
	irq_dummy,						// reserve.
	irq_dummy,						// reserve.
	PIOD_Handler,
	PIOE_Handler,
	PIOF_Handler,
};

void intc_init(void){
	// 周辺割り込みをすべてOFF
	INTC.enable0 = 0;
	INTC.enable1 = 0;
	
	// オフセットを0に設定.
	INTC.baseAddr.word = 0;
}

void intc_enable_interrupt(_e_intc_irq_vector vect){
	if (vect < 32){
		INTC.enable0 |= (1 << vect);
	}
	else{
		INTC.enable1 |= (1 << (vect-32));
	}
}

void intc_disable_interrupt(_e_intc_irq_vector vect){
	if (vect < 32){
		INTC.enable0 &= ~(1 << vect);
	}
	else{
		INTC.enable1 &= ~(1 << (vect-32));
	}
}

void intc_pending_clear(_e_intc_irq_vector vect){
	if (vect < 32){
		INTC.pend0 &= ~(1 << vect);
	}
	else{
		INTC.pend1 &= ~(1 << (vect-32));
	}
}

void intc_set_priority(_e_intc_irq_vector vect, _e_intc_irq_prior prior){
	uint32_t val;
	uint32_t num = vect % 16;
	
	if(vect < 16){
		val = INTC.priority0;
		val &= ~(0x3 << (num*2));
		val |= (prior << (num*2));
		INTC.priority0 = val;
	}
	else if(vect < 32){
		val = INTC.priority1;
		val &= ~(0x3 << (num*2));
		val |= (prior << (num*2));
		INTC.priority1 = val;
	}
	else if(vect < 48){
		val = INTC.priority2;
		val &= ~(0x3 << (num*2));
		val |= (prior << (num*2));
		INTC.priority2 = val;
	}
	else{
		val = INTC.priority3;
		val &= ~(0x3 << (num*2));
		val |= (prior << (num*2));
		INTC.priority3 = val;
	}
}

void _irqHandler(void){
    uint32_t irq_src = INTC.vector.bit.INTC_VECTOR_ADDR;
	
	if(m_irq_handler[irq_src] != 0){
		m_irq_handler[irq_src]();
	}
	INTC.vector.bit.INTC_VECTOR_ADDR = 0x3FFFFFFF;
}