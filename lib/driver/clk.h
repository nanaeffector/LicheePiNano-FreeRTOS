#ifndef __CLK_H__
#define __CLK_H__

#include "stdint.h"

#define CLKGATE0		(32*0)
#define CLKGATE1		(32*1)
#define CLKGATE2		(32*2)

typedef enum{
	ClkGateUnit_AUDIO_CODEC		= ( 0 + CLKGATE2),
	ClkGateUnit_DAUDIO 			= (12 + CLKGATE2),
	ClkGateUnit_TWI0			= (16 + CLKGATE2),
	ClkGateUnit_TWI1			= (17 + CLKGATE2),
	ClkGateUnit_TWI2			= (18 + CLKGATE2),
	ClkGateUnit_UART0			= (20 + CLKGATE2),
	ClkGateUnit_UART1			= (21 + CLKGATE2),
	ClkGateUnit_UART2			= (22 + CLKGATE2),
} ClkGateUnit;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t PLL_M			:2;	// N:0-3
			uint32_t reserve1		:2;
			uint32_t PLL_K			:2;	// N:0-3
			uint32_t reserve2		:2;
			uint32_t PLL_N			:5;	// N:0-31.
			uint32_t reserve3		:3;
			uint32_t PLL_DIV_P		:2;	// 0:/1. 1:/2. 2:/4. 3:/8
			uint32_t reserve4		:10;
			uint32_t LOCK			:1;	// readonly. 0:unlock. 1:lock.
			uint32_t reserve5		:2;
			uint32_t PLL_ENABLE		:1;	// 0:disable. 1:enable.
		}bit;
	};
}PLL_CPU_CTRL_REG;

// PLL_AUDIO(x8) = 24MHz * (N+1) + 2 / (M+1)
typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t PLL_M			:5;	// M:0-31.
			uint32_t reserve1		:3;
			uint32_t PLL_N			:7;	// N:0-127.
			uint32_t reserve2		:9;
			uint32_t PLL_SDM_EN		:1;	// 0:PLL_N is 7bit enable. 1:PLL_N is low 4bits are enable.
			uint32_t reserve3		:3;
			uint32_t LOCK			:1;	// readonly. 0:unlock. 1:lock.
			uint32_t reserve4		:2;
			uint32_t PLL_ENABLE		:1;	// 0:disable. 1:enable.
		}bit;
	};
}PLL_AUDIO_CTRL_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t PLL_M			:4;	// M:0-16.
			uint32_t reserve1		:4;
			uint32_t PLL_N			:7;	// N:0-127.
			uint32_t reserve2		:5;
			uint32_t PLL_SDM_EN		:1;	// 0:disable. 1:enable.
			uint32_t reserve3		:3;
			uint32_t PLL_MODE_SEL	:1;	// 0:Fractional. 1:Integer.
			uint32_t FRAC_CLK_OUT	:1;	// 0:PLLOUT=270MHz. 1:PLLOUT=297MHz.
			uint32_t reserve4		:2;
			uint32_t LOCK			:1;	// readonly. 0:unlock. 1:lock.
			uint32_t reserve5		:1;
			uint32_t MODE			:1;	// 0:manual. 1:automode.
			uint32_t PLL_ENABLE		:1;	// 0:disable. 1:enable.
		}bit;
	};
}PLL_VIDEO_CTRL_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t PLL_M			:4;	// M:0-16.
			uint32_t reserve1		:4;
			uint32_t PLL_N			:7;	// N:0-127.
			uint32_t reserve2		:9;
			uint32_t PLL_MODE_SEL	:1;	// 0:Fractional. 1:Integer.
			uint32_t FRAC_CLK_OUT	:1;	// 0:PLLOUT=270MHz. 1:PLLOUT=297MHz.
			uint32_t reserve3		:2;
			uint32_t LOCK			:1;	// readonly. 0:unlock. 1:lock.
			uint32_t reserve4		:2;
			uint32_t PLL_ENABLE		:1;	// 0:disable. 1:enable.
		}bit;
	};
}PLL_VE_CTRL_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t PLL_M			:2;	// M:0-3.
			uint32_t reserve1		:2;
			uint32_t PLL_K			:2;	// K:0-3.
			uint32_t reserve2		:2;
			uint32_t PLL_N			:5;	// N:0-31.
			uint32_t reserve3		:7;
			uint32_t CFG_UPDATE		:1;	// 0:no. 1:validating pll_ddr.
			uint32_t reserve4		:3;
			uint32_t SIGMA_DELTA_EN	:1;	// 0:disable. 1:enable.
			uint32_t reserve5		:3;
			uint32_t LOCK			:1;	// readonly. 0:unlock. 1:lock.
			uint32_t reserve6		:2;
			uint32_t PLL_ENABLE		:1;	// 0:disable. 1:enable.
		}bit;
	};
}PLL_DDR_CTRL_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t PLL_M				:2;	// M:0-3.
			uint32_t reserve1			:2;
			uint32_t PLL_K				:2;	// K:0-3.
			uint32_t reserve2			:2;
			uint32_t PLL_N				:5;	// N:0-31.
			uint32_t reserve3			:3;
			uint32_t PLL_24M_OUT_DIV	:2;	// 0:/1. 1:/2. 2:/3. 3:/4.
			uint32_t PLL_24M_OUT		:1;	// 0:disable. 1:enable.
			uint32_t reserve4			:9;
			uint32_t LOCK				:1;	// readonly. 0:unlock. 1:lock.
			uint32_t reserve5			:2;
			uint32_t PLL_ENABLE			:1;	// 0:disable. 1:enable.
		}bit;
	};
}PLL_PERIPH_CTRL_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve1			:16;
			uint32_t CPU_CLK_SRC_SEL	:2;	// 0:LOSC. 1:SC24M. 2-3:PLL_CPU.
			uint32_t reserve2			:14;
		}bit;
	};
}CPU_CLK_SRC_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve1			:4;
			uint32_t AHB_CLK_DIV_RATIO	:2;	// 0:/1. 1:/2. 2:/4. 3:/8.
			uint32_t AHB_PRE_DIV		:2;	// 0:/1. 1:/2. 2:/3. 3:/4.
			uint32_t APB_CLK_RATIO		:2;	// 0-1:/2. 2:/4. 3:/8.
			uint32_t reserve2			:2;
			uint32_t AHB_CLK_SRC		:2;	// 0:LOSC. 1:SC24M. 2:CPU_CLK. 3:PLL_PERIPH/AHB_PRE_DIV
			uint32_t reserve3			:2;
			uint32_t HCLKC_DIV			:2;	// 0:/1. 1:/2. 2:/3. 3:/4.
			uint32_t reserve4			:14;
		}bit;
	};
}AHB_APB_HCLKC_CFG_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve1			:6;
			uint32_t DMA				:1;
			uint32_t reserve2			:1;
			uint32_t SD0				:1;
			uint32_t SD1				:1;
			uint32_t reserve3			:4;
			uint32_t SDRAM				:1;
			uint32_t reserve4			:5;
			uint32_t SPI0				:1;
			uint32_t SPI1				:1;
			uint32_t reserve5			:2;
			uint32_t USB_OTG			:1;
			uint32_t reserve6			:7;
		}bit;
	};
}BUS_CLK_GATING_REG0;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t VE					:1;
			uint32_t reserve1			:3;
			uint32_t LCD				:1;
			uint32_t DE_INTERLACE		:1;
			uint32_t reserve2			:2;
			uint32_t CSI				:1;
			uint32_t TVD				:1;
			uint32_t TVE				:1;
			uint32_t reserve3			:1;
			uint32_t DEBE				:1;
			uint32_t reserve4			:1;
			uint32_t DEFE				:1;
			uint32_t reserve5			:17;
		}bit;
	};
}BUS_CLK_GATING_REG1;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t AUDIO_CODEC		:1;
			uint32_t OWA				:1;
			uint32_t CIR				:1;
			uint32_t RSB				:1;
			uint32_t reserve1			:8;
			uint32_t DAUDIO				:1;
			uint32_t reserve2			:3;
			uint32_t TWI0				:1;
			uint32_t TWI1				:1;
			uint32_t TWI2				:1;
			uint32_t reserve3			:1;
			uint32_t UART0				:1;
			uint32_t UART1				:1;
			uint32_t UART2				:1;
			uint32_t reserve4			:9;
		}bit;
	};
}BUS_CLK_GATING_REG2;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve1			:16;
			uint32_t CLK_SRC			:2;	// 0:PLL_AUDIO(x8). 1:PLL_AUDIO(x8)/2. 2::PLL_AUDIO(x8)/4. 3:PLL_AUDIO(x8)/8.
			uint32_t reserve2			:13;
			uint32_t SCLK_GATING		:1;	// 0:off. 1:on.
		}bit;
	};
}DAUDIO_CLK_REG;

typedef struct{
	union{
		uint32_t word;
		struct{
			uint32_t reserve			:31;
			uint32_t SCLK_GATING		:1;	// 0:off. 1:on.
		}bit;
	};
}AUDIO_CODEC_CLK_REG;

typedef struct{
	PLL_CPU_CTRL_REG		PLL_CPU_CTRL;		// 0x000
	uint32_t				reserve1;			// 0x004
	PLL_AUDIO_CTRL_REG		PLL_AUDIO_CTRL;		// 0x008
	uint32_t				reserve2;			// 0x00C
	PLL_VIDEO_CTRL_REG		PLL_VIDEO_CTRL;		// 0x010
	uint32_t				reserve3;			// 0x014
	PLL_VE_CTRL_REG			PLL_VE_CTRL;		// 0x018
	uint32_t				reserve4;			// 0x01C
	PLL_DDR_CTRL_REG		PLL_DDR_CTRL;		// 0x020
	uint32_t				reserve5;			// 0x024
	PLL_PERIPH_CTRL_REG		PLL_PERIPH_CTRL;	// 0x028
	uint32_t				reserve6[9];		// 0x02C-0x04C
	CPU_CLK_SRC_REG			CPU_CLK_SRC;		// 0x050
	AHB_APB_HCLKC_CFG_REG	AHB_APB_HCLKC_CFG;	// 0x054
	uint32_t				reserve7[2];		// 0x058-0x05C
	BUS_CLK_GATING_REG0		BUS_CLK_GATING0;	// 0x060
	BUS_CLK_GATING_REG1		BUS_CLK_GATING1;	// 0x064
	BUS_CLK_GATING_REG2		BUS_CLK_GATING2;	// 0x068
	uint32_t				reserve8[7];		// 0x06C-0x084
	uint32_t				SDMMC0_CLK;			// 0x088
	uint32_t				SDMMC1_CLK;			// 0x08C
	uint32_t				reserve8a[8];		// 0x090-0x0AC
	DAUDIO_CLK_REG			DAUDIO_CLK;			// 0x0B0
	uint32_t				OWA_CLK;			// 0x0B4
	uint32_t				CIR_CLK;			// 0x0B8
	uint32_t				reserve9[4];		// 0x0BC-0x0C8
	uint32_t				USBPHY_CLK;			// 0x0CC
	uint32_t				reserve10[12];		// 0x0D0-0x0FC
	uint32_t				DRAM_GATING;		// 0x100
	uint32_t				BE_CLK;				// 0x104
	uint32_t				reserve11;			// 0x108
	uint32_t				FE_CLK;				// 0x10C
	uint32_t				reserve12[2];		// 0x110-0x114
	uint32_t				TCON_CLK;			// 0x118
	uint32_t				DI_CLK;				// 0x11C
	uint32_t				TVE_CLK;			// 0x120
	uint32_t				TVD_CLK;			// 0x124
	uint32_t				reserve13[3];		// 0x128-0x130
	uint32_t				CSI_CLK;			// 0x134
	uint32_t				reserve14;			// 0x138
	uint32_t				VE_CLK;				// 0x13C
	AUDIO_CODEC_CLK_REG		AUDIO_CODEC_CLK;	// 0x140
	uint32_t				AVS_CLK;			// 0x144
	uint32_t				reserve15[46];		// 0x148-0x1FC
	uint32_t				PLL_STABLE_TIME0;	// 0x200
	uint32_t				PLL_STABLE_TIME1;	// 0x204
	uint32_t				reserve16[6];		// 0x208-0x21C
	uint32_t				PLL_CPU_BIAS;		// 0x220
	uint32_t				PLL_AUDIO_BIAS;		// 0x224
	uint32_t				PLL_VIDEO_BIAS;		// 0x228
	uint32_t				PLL_VE_BIAS;		// 0x22C
	uint32_t				PLL_DDR_BIAS;		// 0x230
	uint32_t				PLL_PERIPH_BIAS;	// 0x234
	uint32_t				reserve17[6];		// 0x238-0x24C
	uint32_t				PLL_CPU_TUN;		// 0x250
	uint32_t				reserve18[3];		// 0x254-0x25C
	uint32_t				PLL_DDR_TUN;		// 0x260
	uint32_t				reserve19[8];		// 0x264-0x280
	uint32_t				PLL_AUDIO_PAT_CTRL;	// 0x284
	uint32_t				PLL_VIDEO_PAT_CTRL;	// 0x288
	uint32_t				reserve20;			// 0x28C
	uint32_t				PLL_DDR_PAT_CTRL;	// 0x290
	uint32_t				reserve21[11];		// 0x294-0x2BC
	uint32_t				BUS_SOFT_RST0;		// 0x2C0
	uint32_t				BUS_SOFT_RST1;		// 0x2C4
	uint32_t				reserve22[2];		// 0x2C8-0x2CC
	uint32_t				BUS_SOFT_RST2;		// 0x2D0
}_st_CCU;


#define F1C100S_CCU_BASE		(0x01c20000)

#define CCU		(*(volatile _st_CCU*)F1C100S_CCU_BASE)

typedef enum{
	PLL_AUDIO_22_528MHZ = 0,
	PLL_AUDIO_24_576MHZ,
}ePLL_AUDIO_CLK;

void clock_init(void);
void clock_set_pll_audio(ePLL_AUDIO_CLK clk);
void clock_set_pll_video();


void openClockGate(ClkGateUnit unit);
void closeClockGate(ClkGateUnit unit);

uint32_t getCPUClock(void);
uint32_t getPeriphClock(void);
uint32_t getAHBClock(void);
uint32_t getAPBClock(void);

#endif /* __CLK_H__ */