// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>

#ifndef __REG_PIO_H__
#define __REG_PIO_H__

#ifdef __cplusplus
extern "C" {
#endif

// define Registers.
typedef union{
	struct{
		// CFG0.
		uint32_t P0		: 4;
		uint32_t P1		: 4;
		uint32_t P2		: 4;
		uint32_t P3		: 4;
		uint32_t P4		: 4;
		uint32_t P5		: 4;
		uint32_t P6		: 4;
		uint32_t P7		: 4;
		// CFG1.
		uint32_t P8		: 4;
		uint32_t P9		: 4;
		uint32_t P10	: 4;
		uint32_t P11	: 4;
		uint32_t P12	: 4;
		uint32_t P13	: 4;
		uint32_t P14	: 4;
		uint32_t P15	: 4;
		// CFG2.
		uint32_t P16	: 4;
		uint32_t P17	: 4;
		uint32_t P18	: 4;
		uint32_t P19	: 4;
		uint32_t P20	: 4;
		uint32_t P21	: 4;
		uint32_t P22	: 4;
		uint32_t P23	: 4;
		// CFG3.
		uint32_t P24	: 4;
		uint32_t P25	: 4;
		uint32_t P26	: 4;
		uint32_t P27	: 4;
		uint32_t P28	: 4;
		uint32_t P29	: 4;
		uint32_t P30	: 4;
		uint32_t P31	: 4;
	}BIT;
	uint32_t WORD[4];
}_st_PT_4BIT;

typedef union{
	struct{
		uint32_t P0		: 1;
		uint32_t P1		: 1;
		uint32_t P2		: 1;
		uint32_t P3		: 1;
		uint32_t P4		: 1;
		uint32_t P5		: 1;
		uint32_t P6		: 1;
		uint32_t P7		: 1;
		uint32_t P8		: 1;
		uint32_t P9		: 1;
		uint32_t P10	: 1;
		uint32_t P11	: 1;
		uint32_t P12	: 1;
		uint32_t P13	: 1;
		uint32_t P14	: 1;
		uint32_t P15	: 1;
		uint32_t P16	: 1;
		uint32_t P17	: 1;
		uint32_t P18	: 1;
		uint32_t P19	: 1;
		uint32_t P20	: 1;
		uint32_t P21	: 1;
		uint32_t P22	: 1;
		uint32_t P23	: 1;
		uint32_t P24	: 1;
		uint32_t P25	: 1;
		uint32_t P26	: 1;
		uint32_t P27	: 1;
		uint32_t P28	: 1;
		uint32_t P29	: 1;
		uint32_t P30	: 1;
		uint32_t P31	: 1;
	}BIT;
	uint32_t WORD;
}_st_PT_1BIT;

typedef union{
	struct{
		// PUL0.
		uint32_t P0		: 2;
		uint32_t P1		: 2;
		uint32_t P2		: 2;
		uint32_t P3		: 2;
		uint32_t P4		: 2;
		uint32_t P5		: 2;
		uint32_t P6		: 2;
		uint32_t P7		: 2;
		uint32_t P8		: 2;
		uint32_t P9		: 2;
		uint32_t P10	: 2;
		uint32_t P11	: 2;
		uint32_t P12	: 2;
		uint32_t P13	: 2;
		uint32_t P14	: 2;
		uint32_t P15	: 2;
		// PUL1.
		uint32_t P16	: 2;
		uint32_t P17	: 2;
		uint32_t P18	: 2;
		uint32_t P19	: 2;
		uint32_t P20	: 2;
		uint32_t P21	: 2;
		uint32_t P22	: 2;
		uint32_t P23	: 2;
		uint32_t P24	: 2;
		uint32_t P25	: 2;
		uint32_t P26	: 2;
		uint32_t P27	: 2;
		uint32_t P28	: 2;
		uint32_t P29	: 2;
		uint32_t P30	: 2;
		uint32_t P31	: 2;
	}BIT;
	uint32_t WORD[2];
}_st_PT_2BIT;

#define INT_CLK_32KHZ		(0)
#define INT_CLK_24MHZ		(1)
typedef union{
	struct{
		uint32_t INT_CLK_SEL		: 1;	// 0:32kHz(deb=250Hz～32kHz), 1:24MHz(deb=187.5kHz～24MHz).
		uint32_t reserve0			: 3;
		uint32_t DEB_CLK_PRE_SCALE	: 3;	// Debounce Clock Pre-Scale N. prescale value = CLK / 2^N.
		uint32_t reserve1			: 25;
	}BIT;
	uint32_t WORD;
}_st_EINT_DEB;

typedef struct{
	_st_PT_4BIT	CFG;		// 0:in, 1:out, 2-7:other-func.
	_st_PT_1BIT	DATA;		// in,out data.
	_st_PT_2BIT	DRV;		// drv strength.
	_st_PT_2BIT	PULL;		// pull-up, pull-down.
}_st_PortReg; // 36byte.

#define EINT_CFG_POS_EDGE		(0)
#define EINT_CFG_NEG_EDGE		(1)
#define EINT_CFG_HIGH_LV		(2)
#define EINT_CFG_LOW_LV			(3)
#define EINT_CFG_DOUBLE_EDGE	(4)
typedef struct{
	_st_PT_4BIT		CFG;		// interrupt config.
	_st_PT_1BIT		EN;			// interrupt enable.
	_st_PT_1BIT		STATUS;		// interrupt status.
	_st_EINT_DEB	DEB;		// intertupt debounce.
}_st_PortEINT;	// 

typedef struct{
	_st_PortReg		PT[6];
	uint32_t		reserve0[(0x200 - (sizeof(_st_PortReg)*6))/4];
	_st_PortEINT	PD_INT;
	_st_PortEINT	PE_INT;
	_st_PortEINT	PF_INT;
	uint32_t		reserve1[(0xC0 - (sizeof(_st_PortEINT)*3))/4];
	uint32_t		SDR_DRV;
	uint32_t		SDR_PULL;
}_st_Port;

#define F1C100S_PIO_BASE		(0x01C20800)
#define PORT				(*(volatile _st_Port *)F1C100S_PIO_BASE)


#ifdef __cplusplus
}
#endif

#endif /* __REG_PIO_H__ */