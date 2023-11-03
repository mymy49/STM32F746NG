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

#ifndef YSS_CONFIG__H_
#define YSS_CONFIG__H_

// ####################### 외부 크리스탈 클럭 주파수 설정 #######################
#define HSE_CLOCK_FREQ 25000000

// ####################### lmalloc 설정 #######################
// SDRAM을 이용한 동적할당 메모리의 사용 여부(true, false)
#define YSS_L_HEAP_USE			true

// SDRAM의 시작 주소 설정
// STM32F7 (Bank1 - 0x60000000, Bank2 - 0x70000000)
#define YSS_SDRAM_ADDR			0x60000000

// SDRAM의 총 메모리 용량 설정
#define YSS_L_HEAP_SIZE			(8 * 1024 * 1024)

// lmalloc의	기본 할당 단위
#define YSS_L_HEAP_CLUSTER_SIZE	(256)

// lmalloc의	최대 할당 개수
#define YSS_L_MAX_NUM_OF_MALLOC	1024

// ####################### 스케줄러 설정 #######################
// 내부 ms 를 만들 시계의 타이머 설정 (timer1 ~ timer14)
#define YSS_TIMER			timer6

// 쓰레드당 할당 받는 Systick Clock의 수
#define THREAD_GIVEN_CLOCK	20000

// 최대 등록 가능한 쓰레드의 수
#define MAX_THREAD			20

// 쓰레드의 스택을 0xAA 패턴으로 채우기 (true, false)
#define FILL_THREAD_STACK	false

// ####################### GUI 설정 #######################
// GUI library Enable (true, false)
#define USE_GUI							true

// Touch Event Enable (true, false)
#define USE_EVENT						true

// Stack Size of Touch Event handler (Byte)
#define TOUCH_EVENT_HANDLER_STACK_SIZE	4096

// Depth of Touch Event Memory ( 32 ~ 256 )
#define TOUCH_EVENT_MEMORY_DEPTH		128

// Frame Buffer of GUI Object (Rgb565, Rgb888, Argb1555)
#define YSS_GUI_FRAME_BUFFER			Argb1555

// Frame Buffer of GUI Object (Rgb565, Rgb888, Argb1555)
#define YSS_OUTPUT_FRAME_BUFFER			Rgb888

// ####################### KEY 설정 #######################
// 최대 KEY 생성 가능 갯수 설정 (0 ~ ), 0일 경우 기능 꺼짐
#define NUM_OF_YSS_KEY 0

// ###################### 주변 장치 활성화 ######################
// 활성화 시킬 장치에 대해 false -> true로 변경하여 활성화 한다.
//
// 주의 
// 1. TIMER, PWM은 실제 동일한 장치지만 OS 구조상 별도의 장치로 표현한다. 그러므로 동일한 번호의 PWM과 TIMER는 동시에 활성화 되지 못한다.

// ADC 활성화
#define ADC1_ENABLE			false
#define ADC2_ENABLE			false
#define ADC3_ENABLE			false

// CRC32 활성화
#define CRC32_ENABLE		false

// DAC 활성화
#define DAC1_ENABLE			false

// DMA2D 활성화
#define DMA2D_ENABLE		false

// I2C 활성화
#define I2C1_ENABLE			false
#define I2C2_ENABLE			false
#define I2C3_ENABLE			true
#define I2C4_ENABLE			false

// I2S 활성화
#define I2S1_ENABLE			false
#define I2S2_ENABLE			false
#define I2S3_ENABLE			false

// LTDC 활성화
#define LTDC_ENABLE			false

// PWM 활성화
#define PWM1_ENABLE			false
#define PWM2_ENABLE			false
#define PWM3_ENABLE			false
#define PWM4_ENABLE			false
#define PWM5_ENABLE			false
#define PWM8_ENABLE			false
#define PWM9_ENABLE			false
#define PWM10_ENABLE		false
#define PWM11_ENABLE		false
#define PWM12_ENABLE		false
#define PWM13_ENABLE		false
#define PWM14_ENABLE		false

// QENCODER 활성화
#define QENCODER1_ENABLE	false
#define QENCODER2_ENABLE	false
#define QENCODER3_ENABLE	false
#define QENCODER4_ENABLE	false
#define QENCODER5_ENABLE	false
#define QENCODER8_ENABLE	false

// SAI 활성화
#define SAI1_ENABLE			false
#define SAI2_ENABLE			false

// SDMMC 활성화
#define SDMMC_ENABLE		true

// SDRAM 활성화
#define SDRAM_ENABLE		true

// SPI 활성화
#define SPI1_ENABLE			false
#define SPI2_ENABLE			false
#define SPI3_ENABLE			false
#define SPI4_ENABLE			false
#define SPI5_ENABLE			false
#define SPI6_ENABLE			false

// TIMER 활성화
#define TIM1_ENABLE			false
#define TIM2_ENABLE			false
#define TIM3_ENABLE			false
#define TIM4_ENABLE			false
#define TIM5_ENABLE			false
#define TIM6_ENABLE			true
#define TIM7_ENABLE			false
#define TIM8_ENABLE			false
#define TIM9_ENABLE			false
#define TIM10_ENABLE		false
#define TIM11_ENABLE		false
#define TIM12_ENABLE		false
#define TIM13_ENABLE		false
#define TIM14_ENABLE		false

// UART 활성화
#define USART1_ENABLE		false
#define USART2_ENABLE		false
#define USART3_ENABLE		false
#define UART4_ENABLE		false
#define UART5_ENABLE		false
#define USART6_ENABLE		false

#endif


