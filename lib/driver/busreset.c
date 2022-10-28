// author : Nana's Effector.
// date   : 2021/10/04.

#include <stdint.h>
#include "busreset.h"
#include "io.h"

#define F1C100S_CCU_BASE		(0x01c20000)

#define BUS_SOFT_RST_REG0		(0x2C0)
#define BUS_SOFT_RST_REG1		(0x2C4)
#define BUS_SOFT_RST_REG2		(0x2D0)



static void setResetReg0(uint32_t unitBit, uint32_t isSet){
	uint32_t val;
	val = read32(F1C100S_CCU_BASE + BUS_SOFT_RST_REG0);
	val &= ~(1 << unitBit);
	if(isSet){
		val |= (1 << unitBit);
	}
	write32(F1C100S_CCU_BASE + BUS_SOFT_RST_REG0, val);
}

static void setResetReg1(uint32_t unitBit, uint32_t isSet){
	uint32_t val;
	val = read32(F1C100S_CCU_BASE + BUS_SOFT_RST_REG1);
	val &= ~(1 << unitBit);
	if(isSet){
		val |= (1 << unitBit);
	}
	write32(F1C100S_CCU_BASE + BUS_SOFT_RST_REG1, val);
}

static void setResetReg2(uint32_t unitBit, uint32_t isSet){
	uint32_t val;
	val = read32(F1C100S_CCU_BASE + BUS_SOFT_RST_REG2);
	val &= ~(1 << unitBit);
	if(isSet){
		val |= (1 << unitBit);
	}
	write32(F1C100S_CCU_BASE + BUS_SOFT_RST_REG2, val);
}

void assertReset(SoftResetUnit unit){
	uint32_t unitBit;
	
	if(unit >= RESET_GATE2){
		unitBit = unit - RESET_GATE2;
		setResetReg2(unitBit, 0);
	}
	else if(unit >= RESET_GATE1){
		unitBit = unit - RESET_GATE1;
		setResetReg1(unitBit, 0);
	}
	else{
		unitBit = unit;
		setResetReg0(unitBit, 0);
	}
}

void deassertReset(SoftResetUnit unit){
	uint32_t unitBit;
	
	if(unit >= RESET_GATE2){
		unitBit = unit - RESET_GATE2;
		setResetReg2(unitBit, 1);
	}
	else if(unit >= RESET_GATE1){
		unitBit = unit - RESET_GATE1;
		setResetReg1(unitBit, 1);
	}
	else{
		unitBit = unit;
		setResetReg0(unitBit, 1);
	}
}