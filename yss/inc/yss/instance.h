////////////////////////////////////////////////////////////////////////////////////////
//
// 저작권 표기 License V3.3
//
// 본 소스 코드는 아래 사항에 동의할 경우에 사용 가능합니다.
// 아래 사항에 대해 동의하지 않거나 이해하지 못했을 경우 사용을 금합니다.
//
// 본 소스 코드를 :
//		- 사용하였다면 아래 사항을 모두 동의하는 것으로 자동 간주 합니다.
//		- 상업적 또는 비 상업적 이용이 가능합니다.
//		- 본 저작권 표시 주석을 제외한 코드의 내용을 임의로 수정하여 사용하는 것은 허용합니다.
//		- 사용자가 수정한 코드를 사용자의 고객사에게 상호간 전달은 허용합니다.
//		- 그러나 수정하여 다수에게 재배포하는 행위를 금지합니다. 
//		- 사용으로 인해 발생하는 모든 사고에 대해서 어떠한 법적 책임을 지지 않습니다.
//		- 어떤 형태의 기여든지, 그것은 기증으로 받아들입니다.
//
// 본 소스 코드는 프리웨어로 앞으로도 유료로 전환하지 않을 것입니다.
// 사용자 또는 부품의 제조사가 요구하는 업데이트가 있을 경우 후원금을 받아 
// 요구하는 사항을 업데이트 할 예정입니다.
//
// Home Page : http://cafe.naver.com/yssoperatingsystem
// Copyright 2024. 홍윤기 all right reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef YSS_INSTANCE__H_
#define YSS_INSTANCE__H_

#include <drv/peripheral.h>

#include <drv/Adc.h>
#include <drv/Can.h>
#include <drv/Capture.h>
#include <drv/Clock.h>
#include <drv/Crc32.h>
#include <drv/Dac.h>
#include <drv/Dma.h>
#include <drv/Dma2d.h>
#include <drv/Exti.h>
#include <drv/Flash.h>
#include <drv/Gpio.h>
#include <drv/I2c.h>
#include <drv/I2s.h>
#include <drv/Ltdc.h>
#include <drv/Nvic.h>
#include <drv/Pbus.h>
#include <drv/Pwm.h>
#include <drv/Qencoder.h>
#include <drv/Quadspi.h>
#include <drv/Rtc.h>
#include <drv/Sai.h>
#include <drv/Sdmmc.h>
#include <drv/Sdram.h>
#include <drv/Spi.h>
#include <drv/Timer.h>
#include <drv/Uart.h>
#include <drv/Usart.h>
#include <drv/Usbd.h>
#include <drv/Wdog.h>

// NVIC
#if defined(NVIC)
extern Nvic nvic;
#endif

#if defined(STM32F4)

#include <targets/st/instance_stm32f4.h>

#elif defined(STM32G4)

#include <targets/st/instance_stm32g4.h>

#elif defined(STM32F0)

#include <targets/st/instance_stm32f0.h>

#elif defined(STM32F1) || defined(GD32F1)

#include <targets/st/instance_stm32f1.h>

#elif defined(STM32F7)

#include <targets/st/instance_stm32f7.h>

#elif defined(EFM32PG22)

#include <targets/siliconlabs/instance_efm32pg22.h>

#elif defined(EFR32BG22)

#include <targets/siliconlabs/instance_efr32bg22.h>

#elif defined(W7500)

#include <targets/wiznet/instance_w7500x.h>

#elif defined(CPU_MIMXRT1011DAE5A)

#include <targets/nxp/instance_mimxrt1011.h>

#endif

#endif

