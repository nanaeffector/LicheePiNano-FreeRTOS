// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>
#include "io.h"
#include "pio.h"
#include "reg_pio.h"

#ifdef __cplusplus
 extern "C" {
#endif


void pio_pinSetting(uint8_t port, uint8_t pin, uint8_t pull, uint8_t cfg, uint8_t drv){
	switch(pin){
		case 0:  PORT.PT[port].CFG.BIT.P0 = cfg; PORT.PT[port].DRV.BIT.P0 = drv; PORT.PT[port].PULL.BIT.P0 = pull; break;
		case 1:  PORT.PT[port].CFG.BIT.P1 = cfg; PORT.PT[port].DRV.BIT.P1 = drv; PORT.PT[port].PULL.BIT.P1 = pull; break;
		case 2:  PORT.PT[port].CFG.BIT.P2 = cfg; PORT.PT[port].DRV.BIT.P2 = drv; PORT.PT[port].PULL.BIT.P2 = pull; break;
		case 3:  PORT.PT[port].CFG.BIT.P3 = cfg; PORT.PT[port].DRV.BIT.P3 = drv; PORT.PT[port].PULL.BIT.P3 = pull; break;
		case 4:  PORT.PT[port].CFG.BIT.P4 = cfg; PORT.PT[port].DRV.BIT.P4 = drv; PORT.PT[port].PULL.BIT.P4 = pull; break;
		case 5:  PORT.PT[port].CFG.BIT.P5 = cfg; PORT.PT[port].DRV.BIT.P5 = drv; PORT.PT[port].PULL.BIT.P5 = pull; break;
		case 6:  PORT.PT[port].CFG.BIT.P6 = cfg; PORT.PT[port].DRV.BIT.P6 = drv; PORT.PT[port].PULL.BIT.P6 = pull; break;
		case 7:  PORT.PT[port].CFG.BIT.P7 = cfg; PORT.PT[port].DRV.BIT.P7 = drv; PORT.PT[port].PULL.BIT.P7 = pull; break;
		case 8:  PORT.PT[port].CFG.BIT.P8 = cfg; PORT.PT[port].DRV.BIT.P8 = drv; PORT.PT[port].PULL.BIT.P8 = pull; break;
		case 9:  PORT.PT[port].CFG.BIT.P9 = cfg; PORT.PT[port].DRV.BIT.P9 = drv; PORT.PT[port].PULL.BIT.P9 = pull; break;
		case 10: PORT.PT[port].CFG.BIT.P10 = cfg; PORT.PT[port].DRV.BIT.P10 = drv; PORT.PT[port].PULL.BIT.P10 = pull; break;
		case 11: PORT.PT[port].CFG.BIT.P11 = cfg; PORT.PT[port].DRV.BIT.P11 = drv; PORT.PT[port].PULL.BIT.P11 = pull; break;
		case 12: PORT.PT[port].CFG.BIT.P12 = cfg; PORT.PT[port].DRV.BIT.P12 = drv; PORT.PT[port].PULL.BIT.P12 = pull; break;
		case 13: PORT.PT[port].CFG.BIT.P13 = cfg; PORT.PT[port].DRV.BIT.P13 = drv; PORT.PT[port].PULL.BIT.P13 = pull; break;
		case 14: PORT.PT[port].CFG.BIT.P14 = cfg; PORT.PT[port].DRV.BIT.P14 = drv; PORT.PT[port].PULL.BIT.P14 = pull; break;
		case 15: PORT.PT[port].CFG.BIT.P15 = cfg; PORT.PT[port].DRV.BIT.P15 = drv; PORT.PT[port].PULL.BIT.P15 = pull; break;
		case 16: PORT.PT[port].CFG.BIT.P16 = cfg; PORT.PT[port].DRV.BIT.P16 = drv; PORT.PT[port].PULL.BIT.P16 = pull; break;
		case 17: PORT.PT[port].CFG.BIT.P17 = cfg; PORT.PT[port].DRV.BIT.P17 = drv; PORT.PT[port].PULL.BIT.P17 = pull; break;
		case 18: PORT.PT[port].CFG.BIT.P18 = cfg; PORT.PT[port].DRV.BIT.P18 = drv; PORT.PT[port].PULL.BIT.P18 = pull; break;
		case 19: PORT.PT[port].CFG.BIT.P19 = cfg; PORT.PT[port].DRV.BIT.P19 = drv; PORT.PT[port].PULL.BIT.P19 = pull; break;
		case 20: PORT.PT[port].CFG.BIT.P20 = cfg; PORT.PT[port].DRV.BIT.P20 = drv; PORT.PT[port].PULL.BIT.P20 = pull; break;
		case 21: PORT.PT[port].CFG.BIT.P21 = cfg; PORT.PT[port].DRV.BIT.P21 = drv; PORT.PT[port].PULL.BIT.P21 = pull; break;
		case 22: PORT.PT[port].CFG.BIT.P22 = cfg; PORT.PT[port].DRV.BIT.P22 = drv; PORT.PT[port].PULL.BIT.P22 = pull; break;
		case 23: PORT.PT[port].CFG.BIT.P23 = cfg; PORT.PT[port].DRV.BIT.P23 = drv; PORT.PT[port].PULL.BIT.P23 = pull; break;
		case 24: PORT.PT[port].CFG.BIT.P24 = cfg; PORT.PT[port].DRV.BIT.P24 = drv; PORT.PT[port].PULL.BIT.P24 = pull; break;
		case 25: PORT.PT[port].CFG.BIT.P25 = cfg; PORT.PT[port].DRV.BIT.P25 = drv; PORT.PT[port].PULL.BIT.P25 = pull; break;
		case 26: PORT.PT[port].CFG.BIT.P26 = cfg; PORT.PT[port].DRV.BIT.P26 = drv; PORT.PT[port].PULL.BIT.P26 = pull; break;
		case 27: PORT.PT[port].CFG.BIT.P27 = cfg; PORT.PT[port].DRV.BIT.P27 = drv; PORT.PT[port].PULL.BIT.P27 = pull; break;
		case 28: PORT.PT[port].CFG.BIT.P28 = cfg; PORT.PT[port].DRV.BIT.P28 = drv; PORT.PT[port].PULL.BIT.P28 = pull; break;
		case 29: PORT.PT[port].CFG.BIT.P29 = cfg; PORT.PT[port].DRV.BIT.P29 = drv; PORT.PT[port].PULL.BIT.P29 = pull; break;
		case 30: PORT.PT[port].CFG.BIT.P30 = cfg; PORT.PT[port].DRV.BIT.P30 = drv; PORT.PT[port].PULL.BIT.P30 = pull; break;
		case 31: PORT.PT[port].CFG.BIT.P31 = cfg; PORT.PT[port].DRV.BIT.P31 = drv; PORT.PT[port].PULL.BIT.P31 = pull; break;
		default: break;
	}
}

void pio_write(uint8_t port, uint8_t pin, uint8_t data){
	uint32_t val;
	
	// mask.
	val = PORT.PT[port].DATA.WORD & ~(1 << pin);
	
	// set.
	if(data != 0){
		val |= (1 << pin);
	}
	
	// write.
	PORT.PT[port].DATA.WORD = val;
}

int pio_read(uint8_t port, uint8_t pin){
	uint32_t val;
	
	// read.
	val = PORT.PT[port].DATA.WORD & (1 << pin);
	
	if(val != 0){
		return 1;
	}
	else{
		return 0;
	}
}

#ifdef __cplusplus
}
#endif