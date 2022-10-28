// author : Nana's Effector.
// date   : 2021/09/22

#ifndef __INTC_H__
#define __INTC_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    IRQ_NMI     = 0,
    IRQ_UART0   = 1,
    IRQ_UART1   = 2,
    IRQ_UART2   = 3,

    IRQ_OWA     = 5,
    IRQ_CIR     = 6,
    IRQ_I2C0    = 7,
    IRQ_I2C1    = 8,
    IRQ_I2C2    = 9,
    IRQ_SPI0    = 10,
    IRQ_SPI1    = 11,

    IRQ_TIMER0  = 13,
    IRQ_TIMER1  = 14,
    IRQ_TIMER2  = 15,
    IRQ_WDOG    = 16,
    IRQ_RSB     = 17,
    IRQ_DMA     = 18,

    IRQ_TP      = 20,
    IRQ_AUDIO   = 21,
    IRQ_KEYADC  = 22,
    IRQ_MMC0    = 23,
    IRQ_MMC1    = 24,

    IRQ_USBOTG  = 26,
    IRQ_TVD     = 27,
    IRQ_TVE     = 28,
    IRQ_TCON    = 29,
    IRQ_DEFE    = 30,
    IRQ_DEBE    = 31,
    IRQ_CSI     = 32,
    IRQ_DEITLA  = 33,
    IRQ_VE      = 34,
    IRQ_I2S     = 35,

    IRQ_GPIOD   = 38,
    IRQ_GPIOE   = 39,
    IRQ_GPIOF   = 40,
} _e_intc_irq_vector;

typedef enum
{
	IRQ_PRI_LV0 = 0x0,		// level 0. lowest.
	IRQ_PRI_LV1 = 0x1,		// level 1.
	IRQ_PRI_LV2 = 0x2,		// level 2.
	IRQ_PRI_LV3 = 0x3,		// level 3. highest.
} _e_intc_irq_prior;

void intc_init(void);
void intc_enable_interrupt(_e_intc_irq_vector vect);
void intc_disable_interrupt(_e_intc_irq_vector vect);
void intc_set_priority(_e_intc_irq_vector vect, _e_intc_irq_prior prior);
void _irqHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __INTC_H__ */