/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "pio.h"

#ifdef __cplusplus
 extern "C" {
#endif"

#include <stdint.h>

const _st_PinItem m_pinList[] = {
// PA.
	{PIO_PORT_A, 0, 0},		// PA0
	{PIO_PORT_A, 1, 0},		// PA1
	{PIO_PORT_A, 2, 0},		// PA2
	{PIO_PORT_A, 3, 0},		// PA3
	
	{PIO_PORT_A, 0, 2},		// PA0_TPX1
	{PIO_PORT_A, 1, 2},		// PA1_TPX2
	{PIO_PORT_A, 2, 2},		// PA2_TPY1
	{PIO_PORT_A, 3, 2},		// PA3_TPY2
	
	{PIO_PORT_A, 2, 3},		// PA2_PWM0.
	{PIO_PORT_A, 3, 3},		// PA3_IR_RX.
	
	{PIO_PORT_A, 0, 4},		// PA0_DA_BCLK
	{PIO_PORT_A, 1, 4},		// PA1_DA_LRCK
	{PIO_PORT_A, 2, 4},		// PA2_DA_IN
	{PIO_PORT_A, 3, 4},		// PA3_DA_OUT
	
	{PIO_PORT_A, 0, 5},		// PA0_UART1_RTS
	{PIO_PORT_A, 1, 5},		// PA1_UART1_CTS
	{PIO_PORT_A, 2, 5},		// PA2_UART1_RX
	{PIO_PORT_A, 3, 5},		// PA3_UART1_TX
	
	{PIO_PORT_A, 0, 6},		// PA0_SPI1_CS
	{PIO_PORT_A, 1, 6},		// PA1_SPI1_MOSI
	{PIO_PORT_A, 2, 6},		// PA2_SPI1_CLK
	{PIO_PORT_A, 3, 6},		// PA3_SPI1_MISO
// PB.
	{PIO_PORT_B, 3, 0},		// PB3
	
	{PIO_PORT_B, 3, 2},		// PB3_DDR_REF_D
	
	{PIO_PORT_B, 3, 3},		// PB3_IR_RX
// PC.
	{PIO_PORT_C, 0, 0},		// PC0
	{PIO_PORT_C, 1, 0},		// PC1
	{PIO_PORT_C, 2, 0},		// PC2
	{PIO_PORT_C, 3, 0},		// PC3
	
	{PIO_PORT_C, 0, 2},		// PC0_SPI0_CLK
	{PIO_PORT_C, 1, 2},		// PC1_SPI0_CS
	{PIO_PORT_C, 2, 2},		// PC2_SPI0_MISO
	{PIO_PORT_C, 3, 2},		// PC3_SPI0_MOSI
	
	{PIO_PORT_C, 0, 3},		// PC0_SDC1_CLK
	{PIO_PORT_C, 1, 3},		// PC1_SDC1_CMD
	{PIO_PORT_C, 2, 3},		// PC2_SDC1_D0
	{PIO_PORT_C, 3, 3},		// PC3_UART0_TX
// PD.
	{PIO_PORT_D, 0, 0},		// PD0
	{PIO_PORT_D, 1, 0},		// PD1
	{PIO_PORT_D, 2, 0},		// PD2
	{PIO_PORT_D, 3, 0},		// PD3
	{PIO_PORT_D, 4, 0},		// PD4
	{PIO_PORT_D, 5, 0},		// PD5
	{PIO_PORT_D, 6, 0},		// PD6
	{PIO_PORT_D, 7, 0},		// PD7
	{PIO_PORT_D, 8, 0},		// PD8
	{PIO_PORT_D, 9, 0},		// PD9
	{PIO_PORT_D, 10, 0},	// PD10
	{PIO_PORT_D, 11, 0},	// PD11
	{PIO_PORT_D, 12, 0},	// PD12
	{PIO_PORT_D, 13, 0},	// PD13
	{PIO_PORT_D, 14, 0},	// PD14
	{PIO_PORT_D, 15, 0},	// PD15
	{PIO_PORT_D, 16, 0},	// PD16
	{PIO_PORT_D, 17, 0},	// PD17
	{PIO_PORT_D, 18, 0},	// PD18
	{PIO_PORT_D, 19, 0},	// PD19
	{PIO_PORT_D, 20, 0},	// PD20
	{PIO_PORT_D, 21, 0},	// PD21
	
	{PIO_PORT_D, 0, 2},		// PD0_LCD_D2
	{PIO_PORT_D, 1, 2},		// PD1_LCD_D3
	{PIO_PORT_D, 2, 2},		// PD2_LCD_D4
	{PIO_PORT_D, 3, 2},		// PD3_LCD_D5
	{PIO_PORT_D, 4, 2},		// PD4_LCD_D6
	{PIO_PORT_D, 5, 2},		// PD5_LCD_D7
	{PIO_PORT_D, 6, 2},		// PD6_LCD_D10
	{PIO_PORT_D, 7, 2},		// PD7_LCD_D11
	{PIO_PORT_D, 8, 2},		// PD8_LCD_D12
	{PIO_PORT_D, 9, 2},		// PD9_LCD_D13
	{PIO_PORT_D, 10, 2},	// PD10_LCD_D14
	{PIO_PORT_D, 11, 2},	// PD11_LCD_D15
	{PIO_PORT_D, 12, 2},	// PD12_LCD_D18
	{PIO_PORT_D, 13, 2},	// PD13_LCD_D19
	{PIO_PORT_D, 14, 2},	// PD14_LCD_D20
	{PIO_PORT_D, 15, 2},	// PD15_LCD_D21
	{PIO_PORT_D, 16, 2},	// PD16_LCD_D22
	{PIO_PORT_D, 17, 2},	// PD17_LCD_D23
	{PIO_PORT_D, 18, 2},	// PD18_LCD_CLK
	{PIO_PORT_D, 19, 2},	// PD19_LCD_DE
	{PIO_PORT_D, 20, 2},	// PD20_LCD_HSYNC
	{PIO_PORT_D, 21, 2},	// PD21_LCD_VSYNC
	
	{PIO_PORT_D, 0, 3},		// PD0_TWI0_SDA
	{PIO_PORT_D, 12, 3},	// PD12_TWI0_SCK
	
	{PIO_PORT_D, 5, 3},		// PD5_TWI1_SCK
	{PIO_PORT_D, 6, 3},		// PD6_TWI1_SDA
	
	{PIO_PORT_D, 1, 3},		// PD1_UART1_RTS
	{PIO_PORT_D, 2, 3},		// PD2_UART1_CTS
	{PIO_PORT_D, 3, 3},		// PD3_UART1_RX
	{PIO_PORT_D, 4, 3},		// PD4_UART1_TX
	
	{PIO_PORT_D, 7, 3},		// PD7_DA_MCLK
	{PIO_PORT_D, 8, 3},		// PD8_DA_BCLK
	{PIO_PORT_D, 9, 3},		// PD9_DA_LRCLK
	{PIO_PORT_D, 10, 3},	// PD10_DA_IN
	{PIO_PORT_D, 11, 3},	// PD11_DA_OUT
	
	{PIO_PORT_D, 13, 3},	// PD13_UART2_TX
	{PIO_PORT_D, 14, 3},	// PD14_UART2_RX
	{PIO_PORT_D, 15, 3},	// PD15_UART2_RTS
	{PIO_PORT_D, 16, 3},	// PD16_UART2_CTS
	
	{PIO_PORT_D, 17, 3},	// PD17_OWA_OUT
	
	{PIO_PORT_D, 18, 3},	// PD18_SPI0_CS
	{PIO_PORT_D, 19, 3},	// PD19_SPI0_MOSI
	{PIO_PORT_D, 20, 3},	// PD20_SPI0_CLK
	{PIO_PORT_D, 21, 3},	// PD21_SPI0_MISO
	
	{PIO_PORT_D, 0, 4},		// PD0_RSB_SDA
	{PIO_PORT_D, 12, 4},	// PD12_RSB_SCK
	
	{PIO_PORT_D, 15, 4},	// PD15_TWI2_SCK
	{PIO_PORT_D, 16, 4},	// PD16_TWI2_SDA
	
	{PIO_PORT_D, 0, 6},		// PD0_EINTD0
	{PIO_PORT_D, 1, 6},		// PD1_EINTD1
	{PIO_PORT_D, 2, 6},		// PD2_EINTD2
	{PIO_PORT_D, 3, 6},		// PD3_EINTD3
	{PIO_PORT_D, 4, 6},		// PD4_EINTD4
	{PIO_PORT_D, 5, 6},		// PD5_EINTD5
	{PIO_PORT_D, 6, 6},		// PD6_EINTD6
	{PIO_PORT_D, 7, 6},		// PD7_EINTD7
	{PIO_PORT_D, 8, 6},		// PD8_EINTD8
	{PIO_PORT_D, 9, 6},		// PD9_EINTD9
	{PIO_PORT_D, 10, 6},	// PD10_EINTD10
	{PIO_PORT_D, 11, 6},	// PD11_EINTD11
	{PIO_PORT_D, 12, 6},	// PD12_EINTD12
	{PIO_PORT_D, 13, 6},	// PD13_EINTD13
	{PIO_PORT_D, 14, 6},	// PD14_EINTD14
	{PIO_PORT_D, 15, 6},	// PD15_EINTD15
	{PIO_PORT_D, 16, 6},	// PD16_EINTD16
	{PIO_PORT_D, 17, 6},	// PD17_EINTD17
	{PIO_PORT_D, 18, 6},	// PD18_EINTD18
	{PIO_PORT_D, 19, 6},	// PD19_EINTD19
	{PIO_PORT_D, 20, 6},	// PD20_EINTD20
	{PIO_PORT_D, 21, 6},	// PD21_EINTD21
// PE.
	{PIO_PORT_E, 0, 0},		// PE0
	{PIO_PORT_E, 1, 0},		// PE1
	{PIO_PORT_E, 2, 0},		// PE2
	{PIO_PORT_E, 3, 0},		// PE3
	{PIO_PORT_E, 4, 0},		// PE4
	{PIO_PORT_E, 5, 0},		// PE5
	{PIO_PORT_E, 6, 0},		// PE6
	{PIO_PORT_E, 7, 0},		// PE7
	{PIO_PORT_E, 8, 0},		// PE8
	{PIO_PORT_E, 9, 0},		// PE9
	{PIO_PORT_E, 10, 0},	// PE10
	{PIO_PORT_E, 11, 0},	// PE11
	{PIO_PORT_E, 12, 0},	// PE12
	
	{PIO_PORT_E, 0, 2},		// PE0_CSI_HSYNC
	{PIO_PORT_E, 1, 2},		// PE1_CSI_VSYNC
	{PIO_PORT_E, 2, 2},		// PE2_CSI_PCLK
	{PIO_PORT_E, 3, 2},		// PE3_CSI_D0
	{PIO_PORT_E, 4, 2},		// PE4_CSI_D1
	{PIO_PORT_E, 5, 2},		// PE5_CSI_D2
	{PIO_PORT_E, 6, 2},		// PE6_CSI_D3
	{PIO_PORT_E, 7, 2},		// PE7_CSI_D4
	{PIO_PORT_E, 8, 2},		// PE8_CSI_D5
	{PIO_PORT_E, 9, 2},		// PE9_CSI_D6
	{PIO_PORT_E, 10, 2},	// PE10_CSI_D7
	
	{PIO_PORT_E, 11, 2},	// PE11_CLK_OUT
	
	{PIO_PORT_E, 12, 2},	// PE12_DA_MCLK
	
	{PIO_PORT_E, 0, 3},		// PE0_LCD_D0
	{PIO_PORT_E, 1, 3},		// PE1_LCD_D1
	{PIO_PORT_E, 2, 3},		// PE2_LCD_D8
	{PIO_PORT_E, 3, 3},		// PE3_LCD_D9
	{PIO_PORT_E, 4, 3},		// PE4_LCD_D16
	{PIO_PORT_E, 5, 3},		// PE5_LCD_D17
	
	{PIO_PORT_E, 6, 3},		// PE6_PWM1
	
	{PIO_PORT_E, 7, 3},		// PE7_UART2_TX
	{PIO_PORT_E, 8, 3},		// PE8_UART2_RX
	{PIO_PORT_E, 9, 3},		// PE9_UART2_RTS
	{PIO_PORT_E, 10, 3},	// PE10_UART2_CTS
	
	{PIO_PORT_E, 11, 3},	// PE11_TWI0_SCK
	{PIO_PORT_E, 12, 3},	// PE12_TWI0_SDA
	
	{PIO_PORT_E, 0, 4},		// PE0_TWI2_SCK
	{PIO_PORT_E, 1, 4},		// PE1_TWI2_SDA
	
	{PIO_PORT_E, 2, 4},		// PE2_CLK_OUT
	
	{PIO_PORT_E, 3, 4},		// PE3_DA_BCLK
	{PIO_PORT_E, 4, 4},		// PE4_DA_LRCK
	{PIO_PORT_E, 5, 4},		// PE5_DA_IN
	{PIO_PORT_E, 6, 4},		// PE6_DA_OUT
	
	{PIO_PORT_E, 7, 4},		// PE7_SPI1_CS
	{PIO_PORT_E, 8, 4},		// PE8_SPI1_MOSI
	{PIO_PORT_E, 9, 4},		// PE9_SPI1_CLK
	{PIO_PORT_E, 10, 4},	// PE10_SPI1_MISO
	
	{PIO_PORT_E, 11, 4},	// PE11_IR_RX
	
	{PIO_PORT_E, 12, 4},	// PE12_PWM0
	
	{PIO_PORT_E, 0, 5},		// PE0_UART0_RX
	{PIO_PORT_E, 1, 5},		// PE1_UART0_TX
	
	{PIO_PORT_E, 3, 5},		// PE3_RSB_SCK
	{PIO_PORT_E, 4, 5},		// PE4_RSB_SDA
	
	{PIO_PORT_E, 6, 5},		// PE6_OWA_OUT
	
	{PIO_PORT_E, 0, 6},		// PE0_EINTE0
	{PIO_PORT_E, 1, 6},		// PE1_EINTE1
	{PIO_PORT_E, 2, 6},		// PE2_EINTE2
	{PIO_PORT_E, 3, 6},		// PE3_EINTE3
	{PIO_PORT_E, 4, 6},		// PE4_EINTE4
	{PIO_PORT_E, 5, 6},		// PE5_EINTE5
	{PIO_PORT_E, 6, 6},		// PE6_EINTE6
	{PIO_PORT_E, 7, 6},		// PE7_EINTE7
	{PIO_PORT_E, 8, 6},		// PE8_EINTE8
	{PIO_PORT_E, 9, 6},		// PE9_EINTE9
	{PIO_PORT_E, 10, 6},	// PE10_EINTE10
	{PIO_PORT_E, 11, 6},	// PE11_EINTE11
	{PIO_PORT_E, 12, 6},	// PE12_EINTE12
// PF.
	{PIO_PORT_F, 0, 0},		// PF0
	{PIO_PORT_F, 1, 0},		// PF1
	{PIO_PORT_F, 2, 0},		// PF2
	{PIO_PORT_F, 3, 0},		// PF3
	{PIO_PORT_F, 4, 0},		// PF4
	{PIO_PORT_F, 4, 0},		// PF5
	
	{PIO_PORT_F, 0, 2},		// PF0_SDC0_D1
	{PIO_PORT_F, 1, 2},		// PF1_SDC0_D0
	{PIO_PORT_F, 2, 2},		// PF2_SDC0_CLK
	{PIO_PORT_F, 3, 2},		// PF3_SDC0_CMD
	{PIO_PORT_F, 4, 2},		// PF4_SDC0_D3
	{PIO_PORT_F, 5, 2},		// PF5_SDC0_D2
	
	{PIO_PORT_F, 2, 3},		// PF2_UART0_RX
	{PIO_PORT_F, 4, 3},		// PF4_UART0_TX
	
	{PIO_PORT_F, 0, 3},		// PF0_DBG_MS
	{PIO_PORT_F, 1, 3},		// PF1_DBG_DI
	{PIO_PORT_F, 3, 3},		// PF3_DBG_DO
	{PIO_PORT_F, 5, 3},		// PF5_DBG_CK
	
	{PIO_PORT_F, 0, 4},		// PF0_IR_RX
	
	{PIO_PORT_F, 5, 4},		// PF5_PWM1
	
	{PIO_PORT_F, 0, 6},		// PF0_EINTF0
	{PIO_PORT_F, 1, 6},		// PF1_EINTF1
	{PIO_PORT_F, 2, 6},		// PF2_EINTF2
	{PIO_PORT_F, 3, 6},		// PF3_EINTF3
	{PIO_PORT_F, 4, 6},		// PF4_EINTF4
	{PIO_PORT_F, 5, 6},		// PF5_EINTF5
};

void pinMode(uint32_t ulPin, uint32_t ulMode){
	uint32_t port = m_pinList[ulPin].port;
	uint32_t num = m_pinList[ulPin].num;
	uint32_t pull = ((ulMode == INPUT_PULLUP) ? 1 : (ulMode == INPUT_PULLDOWN) ? 2 : 0);
	uint32_t cfg;
	
	if(ulMode == PIN_DISABLE){
		cfg = 7;	// disable.
		pull = 0;
	}
	else if(m_pinList[ulPin].cfg == 0){
		if(ulMode == OUTPUT){
			cfg = 1;	// output.
		}
		else{
			cfg = 0;	// input.
		}
	}
	else{
		cfg = m_pinList[ulPin].cfg;	// function.
	}
	
	pio_pinSetting(port, num, pull, cfg, 0x3);
}

void digitalWrite( uint32_t ulPin, uint32_t ulVal ){
	uint32_t port = m_pinList[ulPin].port;
	uint32_t num = m_pinList[ulPin].num;
	
	pio_write(port, num, ulVal);
}

int digitalRead( uint32_t ulPin ){
	uint32_t port = m_pinList[ulPin].port;
	uint32_t num = m_pinList[ulPin].num;
	
	pio_read(port, num);
}


#ifdef __cplusplus
}
#endif

d