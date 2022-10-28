// author : Nana's Effector.
// date   : 2021/09/22

#include "io.h"
#include "clk.h"

#define F1C100S_CCU_BASE		(0x01c20000)

#define CCU_PLL_CPU_CTRL		(0x000)
#define CCU_PLL_AUDIO_CTRL		(0x008)
#define CCU_PLL_VIDEO_CTRL		(0x010)
#define CCU_PLL_VE_CTRL			(0x018)
#define CCU_PLL_DDR_CTRL		(0x020)
#define CCU_PLL_PERIPH_CTRL		(0x028)
#define CCU_CPU_CFG				(0x050)
#define CCU_AHB_APB_CFG			(0x054)

#define CCU_BUS_CLK_GATE0		(0x060)
#define CCU_BUS_CLK_GATE1		(0x064)
#define CCU_BUS_CLK_GATE2		(0x068)

#define CCU_SDMMC0_CLK			(0x088)
#define CCU_SDMMC1_CLK			(0x08c)
#define CCU_DAUDIO_CLK			(0x0b0)
#define CCU_SPDIF_CLK			(0x0b4)
#define CCU_I2S_CLK				(0x0b8)
#define CCU_USBPHY_CFG			(0x0cc)
#define CCU_DRAM_CLK_GATE		(0x100)
#define CCU_DEBE_CLK			(0x104)
#define CCU_DEFE_CLK			(0x10c)
#define CCU_LCD_CLK				(0x118)
#define CCU_DEINTERLACE_CLK		(0x11c)
#define CCU_TVE_CLK				(0x120)
#define CCU_TVD_CLK				(0x124)
#define CCU_CSI_CLK				(0x134)
#define CCU_VE_CLK				(0x13c)
#define CCU_ADDA_CLK			(0x140)
#define CCU_AVS_CLK				(0x144)

#define CCU_PLL_STABLE_TIME0	(0x200)
#define CCU_PLL_STABLE_TIME1	(0x204)
#define CCU_PLL_CPU_BIAS		(0x220)
#define CCU_PLL_AUDIO_BIAS		(0x224)
#define CCU_PLL_VIDEO_BIAS		(0x228)
#define CCU_PLL_VE_BIAS			(0x22c)
#define CCU_PLL_DDR0_BIAS		(0x230)
#define CCU_PLL_PERIPH_BIAS		(0x234)
#define CCU_PLL_CPU_TUN			(0x250)
#define CCU_PLL_DDR_TUN			(0x260)
#define CCU_PLL_AUDIO_PAT		(0x284)
#define CCU_PLL_VIDEO_PAT		(0x288)
#define CCU_PLL_DDR0_PAT		(0x290)

#define CCU_BUS_SOFT_RST0		(0x2c0)
#define CCU_BUS_SOFT_RST1		(0x2c4)
#define CCU_BUS_SOFT_RST3		(0x2d0)

#define CLK_CPU_MAX				(720000000)
#define CLK_24MHZ				(24000000)
#define CLK_12MHZ				(12000000)
#define CLK_32KHZ				(32000)

#define CLK_CPU_SRC_LOSC		(0x0)
#define CLK_CPU_SRC_24M			(0x1)
#define CLK_CPU_SRC_PLL			(0x2)


static inline void sdelay(int loops)
{
	__asm__ __volatile__ ("1:\n" "subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0"(loops));
}

static void clock_set_pll_cpu(uint32_t clk)
{
	uint32_t n, k, m, p;
	uint32_t div = 0;

	if(clk > CLK_CPU_MAX){
		clk = CLK_CPU_MAX;
	}

	if((clk % CLK_24MHZ) == 0)
	{
		div = clk / CLK_24MHZ;
		n = div - 1;
		k = 0;
		m = 0;
		p = 0;
	}
	else if((clk % CLK_12MHZ) == 0)
	{
		m = 1;
		div = clk / CLK_12MHZ;
		if((div % 3) == 0)
			k = 2;
		else if((div % 4) == 0)
			k = 3;
		else
			k = 1;
		n = (div / (k + 1)) - 1;
		p = 0;
	}
	else
	{
		div = clk / CLK_24MHZ;
		n = div - 1;
		k = 0;
		m = 0;
		p = 0;
	}
	
	// CPU Clock Source = 24MHz.
	CCU.CPU_CLK_SRC.bit.CPU_CLK_SRC_SEL = CLK_CPU_SRC_24M;
	sdelay(100);
	
	// PLL_CPU= (24MHz*N*K)/(P*M) =408MHz.
	// set pll setting.
	CCU.PLL_CPU_CTRL.bit.PLL_M = m;
	CCU.PLL_CPU_CTRL.bit.PLL_K = k;
	CCU.PLL_CPU_CTRL.bit.PLL_N = n;
	CCU.PLL_CPU_CTRL.bit.PLL_DIV_P = p;
	CCU.PLL_CPU_CTRL.bit.PLL_ENABLE = 1;
	
	// wait pll lock.
	{
		uint32_t time = 0xfff;
		while(time && !CCU.PLL_CPU_CTRL.bit.LOCK){
			time--;
		}
	}
	
	// CPU Clock Source = PLL.
	CCU.CPU_CLK_SRC.bit.CPU_CLK_SRC_SEL = CLK_CPU_SRC_PLL;
	sdelay(100);
}

static void clock_set_pll_peripheral(){
	// PLL_ENABLE, UNLOCK, PLL_24M_OUT_EN, N=0x18(24+1=25), K=1, M=1.
	CCU.PLL_PERIPH_CTRL.word = 0x80041800;
	
	// wait lock.
	{
		uint32_t time = 0xfff;
		while(time && !CCU.PLL_PERIPH_CTRL.bit.LOCK){
			time--;
		}
	}
}

void clock_set_pll_audio(ePLL_AUDIO_CLK clk){
	CCU.PLL_AUDIO_CTRL.bit.PLL_ENABLE = 0;
	// wait 100msec.
	sdelay(100);

	if(clk == PLL_AUDIO_22_528MHZ){
		// audio pll clock is 22.528MHz.
		CCU.PLL_AUDIO_CTRL.bit.PLL_SDM_EN = 0;	// PLL_N is 7bit enable.
		//CCU.PLL_AUDIO_CTRL.bit.PLL_SDM_EN = 1;	// PLL_N is low 4bits are enable.
		
		// TODO. PLLの設定が出来ていない. 24.576MHzで動作する.
		CCU.PLL_AUDIO_CTRL.bit.PLL_N = 0x55;	// N=85.
		CCU.PLL_AUDIO_CTRL.bit.PLL_M = 0x14;	// M=20.
	}
	else{
		// audio pll clock is 24.576MHz.
		CCU.PLL_AUDIO_CTRL.bit.PLL_SDM_EN = 0;	// PLL_N is 7bit enable.
		//CCU.PLL_AUDIO_CTRL.bit.PLL_SDM_EN = 1;	// PLL_N is low 4bits are enable.
		
		CCU.PLL_AUDIO_CTRL.bit.PLL_N = 0x55;	// N=85.
		CCU.PLL_AUDIO_CTRL.bit.PLL_M = 0x14;	// M=20.
	}
	
	CCU.PLL_AUDIO_CTRL.bit.PLL_ENABLE = 1;
	
	// wait lock.
	{
		uint32_t time = 0xfff;
		while(time && !CCU.PLL_AUDIO_CTRL.bit.LOCK){
			time--;
		}
	}
	
	// wait 100msec.
	sdelay(100);
/*
	CCU.DAUDIO_CLK.bit.CLK_SRC = 
	CCU.DAUDIO_CLK.bit.SCLK_GATING = 1;		// on
*/
}

void clock_set_pll_video(){
	uint32_t val;

	// PLL_VIDEO=(24MHz*66)/7=226MHz.
	// PLL_ENABLE, MANUAL, UNLOCK, FRAC_CLK=270MHz, INTEGER, PLL_SDM=Disable, N=0x41(65+1=66), M=7.
	CCU.PLL_VIDEO_CTRL.word = 0x81004107;
	
	// wait lock.
	{
		uint32_t time = 0xfff;
		while(time && !CCU.PLL_VIDEO_CTRL.bit.LOCK){
			time--;
		}
	}
	
	// wait 100msec.
	sdelay(100);
	
	// Gate open. DRAM for BE and DRAM for FE.
	val = CCU.DRAM_GATING;
	val |= (0x1 << 26) | (0x1 << 24);
	CCU.DRAM_GATING = val;
}

void clock_init(void)
{
	CCU.PLL_STABLE_TIME0 = 0x1ff;
	CCU.PLL_STABLE_TIME1 = 0x1ff;
	
	// PLL_CPU=408MHz.
	//clock_set_pll_cpu(408000000);
	clock_set_pll_cpu(720000000);
	
	// PLL_PERIPH=(24MHz*25*1)=600MHz.
	clock_set_pll_peripheral();
	
	// HCLKC=CPUCLK(408MHz). AHB_CLK=600MHz/3=200MHz. APB_CLK=200MHz/2=100MHz.
	CCU.AHB_APB_HCLKC_CFG.word = 0x00003180;		// HCLKC_DIV=/1, AHB_SRC=PLL_PERIPH/AHB_PRE_DIV, APB_DIV=/2, AHB_PRE_DIV=/3, AHB_DIV=/1.
	
	sdelay(100);
}

static void setClockGate0(uint32_t unitBit, uint32_t isSet){
	uint32_t val;
	val = read32(F1C100S_CCU_BASE + CCU_BUS_CLK_GATE0);
	val &= ~(1 << unitBit);
	if(isSet){
		val |= (1 << unitBit);
	}
	write32(F1C100S_CCU_BASE + CCU_BUS_CLK_GATE0, val);
}

static void setClockGate1(uint32_t unitBit, uint32_t isSet){
	uint32_t val;
	val = read32(F1C100S_CCU_BASE + CCU_BUS_CLK_GATE1);
	val &= ~(1 << unitBit);
	if(isSet){
		val |= (1 << unitBit);
	}
	write32(F1C100S_CCU_BASE + CCU_BUS_CLK_GATE1, val);
}

static void setClockGate2(uint32_t unitBit, uint32_t isSet){
	uint32_t val;
	val = read32(F1C100S_CCU_BASE + CCU_BUS_CLK_GATE2);
	val &= ~(1 << unitBit);
	if(isSet){
		val |= (1 << unitBit);
	}
	write32(F1C100S_CCU_BASE + CCU_BUS_CLK_GATE2, val);
}

void openClockGate(ClkGateUnit unit){
	uint32_t unitBit;
	
	if(unit >= CLKGATE2){
		unitBit = unit - CLKGATE2;
		setClockGate2(unitBit, 1);
	}
	else if(unit >= CLKGATE1){
		unitBit = unit - CLKGATE1;
		setClockGate1(unitBit, 1);
	}
	else{
		unitBit = unit;
		setClockGate0(unitBit, 1);
	}
	sdelay(100);
}

void closeClockGate(ClkGateUnit unit){
	uint32_t unitBit;
	
	if(unit >= CLKGATE2){
		unitBit = unit - CLKGATE2;
		setClockGate2(unitBit, 0);
	}
	else if(unit >= CLKGATE1){
		unitBit = unit - CLKGATE1;
		setClockGate1(unitBit, 0);
	}
	else{
		unitBit = unit;
		setClockGate0(unitBit, 0);
	}
	sdelay(100);
}

uint32_t getCPUClock(void){
	uint32_t reg = read32(F1C100S_CCU_BASE + CCU_PLL_CPU_CTRL);
	uint32_t n = (reg >> 8) & 0x1F;
	uint32_t k = (reg >> 4) & 0x3;
	uint32_t m = (reg >> 0) & 0x3;
	uint32_t p = (reg >> 16) & 0x3;

	p = (1 << p);

	return (CLK_24MHZ * (n + 1) * (k + 1) / ((m + 1) * p));
}

uint32_t getPeriphClock(void){
	uint32_t reg = read32(F1C100S_CCU_BASE + CCU_PLL_PERIPH_CTRL);
	uint32_t mul = (reg >> 8) & 0x1F;
	uint32_t div = (reg >> 4) & 0x3;
	uint32_t clk = (CLK_24MHZ * (mul + 1) / (div + 1));

	return clk;
}

uint32_t getAHBClock(void){
	uint32_t reg = read32(F1C100S_CCU_BASE+CCU_AHB_APB_CFG);
	uint32_t src = (reg >> 12) & 0x3;
	uint32_t div = (reg >> 4) & 0x3;
	uint32_t prediv = (reg >> 6) & 0x3;
	uint32_t clk;

	div = (1 << div);

	switch (src)
	{
		case 0:
			clk = CLK_32KHZ / div;
			break;
		case 1:
			clk = CLK_24MHZ / div;
			break;
		case 2:
			clk = getCPUClock() / div;
			break;
		case 3:
			clk = getPeriphClock() / (prediv+1) / div;
			break;
		default:
			clk = 0;
			break;
	}
	
	return clk;
}

uint32_t getAPBClock(void){
	uint32_t div = (read32(F1C100S_CCU_BASE+CCU_AHB_APB_CFG) >> 8) & 0x3;
	uint32_t clk = getAHBClock();

	switch (div)
	{
		case 0:
		case 1:
			return clk / 2;
		case 2:
			return clk / 4;
		case 3:
			return clk / 8;
		default:
			clk = 0;
			break;
	}
	
	return clk;
}

