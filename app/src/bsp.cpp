/*
	Copyright 2023. 홍윤기 all right reserved.

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <bsp.h>
#include <yss.h>

#include <mod/rgb_tft_lcd/RK043FN48H.h>
#include <mod/sdram/MT48LC4M32B2B5_6A.h>

void initializeLcd(void);

RK043FN48H lcd;

void initializeBoard(void)
{
	// TFT LCD 초기화
	initializeLcd();
}

void initializeLcd(void)
{
	using namespace define::gpio::altfunc;
	
	Gpio::AltFunc lcdPort[28] =
	{
		{GPIOJ, 6, PJ6_LCD_R7},
		{GPIOJ, 5, PJ5_LCD_R6},
		{GPIOJ, 4, PJ4_LCD_R5},
		{GPIOJ, 3, PJ3_LCD_R4},
		{GPIOJ, 2, PJ2_LCD_R3},
		{GPIOJ, 1, PJ1_LCD_R2},
		{GPIOJ, 0, PJ0_LCD_R1},
		{GPIOI, 15, PI15_LCD_R0},

		{GPIOK, 2, PK2_LCD_G7},
		{GPIOK, 1, PK1_LCD_G6},
		{GPIOK, 0, PK0_LCD_G5},
		{GPIOJ, 11, PJ11_LCD_G4},
		{GPIOJ, 10, PJ10_LCD_G3},
		{GPIOJ, 9, PJ9_LCD_G2},
		{GPIOJ, 8, PJ8_LCD_G1},
		{GPIOJ, 7, PJ7_LCD_G0},

		{GPIOK, 6, PK6_LCD_B7},
		{GPIOK, 5, PK5_LCD_B6},
		{GPIOK, 4, PK4_LCD_B5},
		{GPIOG, 12, PG12_LCD_B4},
		{GPIOJ, 15, PJ15_LCD_B3},
		{GPIOJ, 14, PJ14_LCD_B2},
		{GPIOJ, 13, PJ13_LCD_B1},
		{GPIOE, 4, PE4_LCD_B0},

		{GPIOI, 9, PI9_LCD_VSYNC},
		{GPIOI, 10, PI10_LCD_HSYNC},
		{GPIOK, 7, PK7_LCD_DE},
		{GPIOI, 14, PI14_LCD_CLK}
	};

	using namespace define::gpio;
	gpioA.setPackageAsAltFunc(lcdPort, 28, ospeed::FAST, otype::PUSH_PULL);

	// LCD DISP 핀 활성화
	gpioI.setAsOutput(12);
	gpioI.setOutput(12, true);

	ltdc.enableClock();
	ltdc.initialize(lcd.getSpecification());
	ltdc.enableInterrupt();
}

// OS에서 자동으로 호출함
void initializeSdram(void)
{
	using namespace define::gpio::altfunc;

	Gpio::AltFunc sdramPort[38]{
		{GPIOF, 0, PF0_FMC_A0},
		{GPIOF, 1, PF1_FMC_A1},
		{GPIOF, 2, PF2_FMC_A2},
		{GPIOF, 3, PF3_FMC_A3},
		{GPIOF, 4, PF4_FMC_A4},
		{GPIOF, 5, PF5_FMC_A5},
		{GPIOF, 12, PF12_FMC_A6},
		{GPIOF, 13, PF13_FMC_A7},
		{GPIOF, 14, PF14_FMC_A8},
		{GPIOF, 15, PF15_FMC_A9},
		{GPIOG, 0, PG0_FMC_A10},
		{GPIOG, 1, PG1_FMC_A11},
		{GPIOG, 4, PG4_FMC_BA0},
		{GPIOG, 5, PG5_FMC_BA1},
		{GPIOD, 14, PD14_FMC_D0},
		{GPIOD, 15, PD15_FMC_D1},
		{GPIOD, 0, PD0_FMC_D2},
		{GPIOD, 1, PD1_FMC_D3},
		{GPIOE, 7, PE7_FMC_D4},
		{GPIOE, 8, PE8_FMC_D5},
		{GPIOE, 9, PE9_FMC_D6},
		{GPIOE, 10, PE10_FMC_D7},
		{GPIOE, 11, PE11_FMC_D8},
		{GPIOE, 12, PE12_FMC_D9},
		{GPIOE, 13, PE13_FMC_D10},
		{GPIOE, 14, PE14_FMC_D11},
		{GPIOE, 15, PE15_FMC_D12},
		{GPIOD, 8, PD8_FMC_D13},
		{GPIOD, 9, PD9_FMC_D14},
		{GPIOD, 10, PD10_FMC_D15},
		{GPIOE, 0, PE0_FMC_NBL0},
		{GPIOE, 1, PE1_FMC_NBL1},
		{GPIOG, 8, PG8_FMC_SDCLK},
		{GPIOH, 5, PH5_FMC_SDNWE},
		{GPIOF, 11, PF11_FMC_SDNRAS},
		{GPIOG, 15, PG15_FMC_SDNCAS},
		{GPIOC, 3, PC3_FMC_SDCKE0},
		{GPIOH, 3, PH3_FMC_SDNE0}
	};

	gpioA.setPackageAsAltFunc(sdramPort, 38, define::gpio::ospeed::FAST, define::gpio::otype::PUSH_PULL);

	clock.enableSdram();
	sdram.initialize(define::sdram::bank::BANK1, MT48LC4M32B2B5_6A, 180000000);
}

