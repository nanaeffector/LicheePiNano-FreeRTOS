// author : Nana's Effector.
// date   : 2021/09/22

#include <stdint.h>

#ifndef __REG_INTC_H__
#define __REG_INTC_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve			:2;
			uint32_t INTC_VECTOR_ADDR	:30;
		}bit;
	};
}INTC_VECTOR_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve			:2;
			uint32_t INTC_BASE_ADDR		:30;
		}bit;
	};
}INTC_BASE_ADDR_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t NMI_SRC_TYPE		:2;
			uint32_t reserve			:30;
		}bit;
	};
}NMI_INT_CTRL_REG;

typedef struct{
	INTC_VECTOR_REG		vector;		// 0x00
	INTC_BASE_ADDR_REG	baseAddr;	// 0x04
	uint32_t			reserve0;	// 0x08
	NMI_INT_CTRL_REG	nmiCtrl;	// 0x0C
	uint32_t			pend0;		// 0x10
	uint32_t			pend1;		// 0x14
	uint32_t			reserve1;	// 0x18
	uint32_t			reserve2;	// 0x1C
	uint32_t			enable0;	// 0x20
	uint32_t			enable1;	// 0x24
	uint32_t			reserve3;	// 0x28
	uint32_t			reserve4;	// 0x2C
	uint32_t			mask0;		// 0x30
	uint32_t			mask1;		// 0x34
	uint32_t			reserve5;	// 0x38
	uint32_t			reserve6;	// 0x3C
	uint32_t			resp0;		// 0x40		// response0.
	uint32_t			resp1;		// 0x44		// response1.
	uint32_t			reserve7;	// 0x48
	uint32_t			reserve8;	// 0x4C
	uint32_t			ff0;		// 0x50		// fast forcing0.
	uint32_t			ff1;		// 0x54		// fast forcing1.
	uint32_t			reserve9;	// 0x58
	uint32_t			reserve10;	// 0x5C
	uint32_t			priority0;	// 0x60
	uint32_t			priority1;	// 0x64
	uint32_t			priority2;	// 0x68
	uint32_t			priority3;	// 0x6C
}_st_intc;

#define F1C100S_INTC_BASE			(0x01C20400)
#define INTC (*(volatile _st_intc*)F1C100S_INTC_BASE)

#ifdef __cplusplus
}
#endif

#endif /* __REG_PIO_H__ */