// author : Nana's Effector.
// date   : 2021/09/22

#ifndef __RESET_H__
#define __RESET_H__

#ifdef __cplusplus
extern "C" {
#endif

#define RESET_GATE0		(32*0)
#define RESET_GATE1		(32*1)
#define RESET_GATE2		(32*2)
	
typedef enum{
	// RST0.
	SoftResetUnit_DMA			= ( 6 + RESET_GATE0),
	SoftResetUnit_SD0			= ( 8 + RESET_GATE0),
	SoftResetUnit_SD1			= ( 9 + RESET_GATE0),
	SoftResetUnit_SDRAM			= (14 + RESET_GATE0),
	SoftResetUnit_SPI0			= (20 + RESET_GATE0),
	SoftResetUnit_SPI1			= (21 + RESET_GATE0),
	SoftResetUnit_USBOTG		= (24 + RESET_GATE0),
	// RST1.
	SoftResetUnit_VE			= ( 0 + RESET_GATE1),
	SoftResetUnit_LCD			= ( 4 + RESET_GATE1),
	SoftResetUnit_DEINTERLACE	= ( 5 + RESET_GATE1),
	SoftResetUnit_CSI			= ( 8 + RESET_GATE1),
	SoftResetUnit_TVD			= ( 9 + RESET_GATE1),
	SoftResetUnit_TVE			= (10 + RESET_GATE1),
	SoftResetUnit_DEBE			= (12 + RESET_GATE1),
	SoftResetUnit_DEFE			= (14 + RESET_GATE1),
	// RST2.
	SoftResetUnit_ACODEC		= ( 0 + RESET_GATE2),
	SoftResetUnit_OWA			= ( 1 + RESET_GATE2),
	SoftResetUnit_CIR			= ( 2 + RESET_GATE2),
	SoftResetUnit_RSB			= ( 3 + RESET_GATE2),
	SoftResetUnit_DAUDIO		= (12 + RESET_GATE2),
	SoftResetUnit_TWI0			= (16 + RESET_GATE2),
	SoftResetUnit_TWI1			= (17 + RESET_GATE2),
	SoftResetUnit_TWI2			= (18 + RESET_GATE2),
	SoftResetUnit_UART0			= (20 + RESET_GATE2),
	SoftResetUnit_UART1			= (21 + RESET_GATE2),
	SoftResetUnit_UART2			= (22 + RESET_GATE2),
} SoftResetUnit;

void assertReset(SoftResetUnit unit);
void deassertReset(SoftResetUnit unit);

#ifdef __cplusplus
}
#endif

#endif /* __RESET_H__ */