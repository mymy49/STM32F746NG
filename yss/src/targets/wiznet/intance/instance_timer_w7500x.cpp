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

#include <yss/instance.h>

#if defined(W7500)

#include <targets/wiznet/bitfield_w7500x.h>

#if TIMER0_ENABLE && defined(PWM0)
#if YSS_TIMER == RUNTIME_PWM0
#error "현재 PWM0는 yss OS의 Runtime에서 사용하고 있습니다. 일반적인 사용이 불가능합니다."
#elif (1 < TIMER0_ENABLE)
#error "W7500에서는 동인한 번호의 TIMER, PWM, CAPTURE의 중복 사용을 금지합니다."
// W7500에서 TIMER0, PWM0, CAPTURE0는 PWM0 장치 하나를 공유해서 사용합니다.
// 한 프로젝트에서 하나의 장치만 사용할 수 있습니다.
#endif

static void enableTimer0Clock(bool en)
{
	CRG->PWM0CLK_SSR = 1;
}

static void enableTimer0Interrup(bool en)
{
	PWM->IER |= 1 << 0;
	NVIC_EnableIRQ(PWM0_IRQn);
}

static uint32_t getTimer0ClockFrequency(void)
{
	uint32_t clk;

	switch(CRG->PWM0CLK_SSR & 0x3)
	{
	case 0 : // 비활성화
	default :
		return 0;
	
	case 1 : // MCLK
		clk = clock.getMclkFrequency();
		break;

	case 2 : // RCLK
		clk = clock.getRclkFrequency();
		break;
	
	case 3 : // OCLK
		clk = clock.getOclkFrequency();
		break;
	}

	return clk / (1 << (CRG->PWM0CLK_PVSR & 0x7));
}

static const Drv::setup_t gDrvTimer0Setup = 
{
	enableTimer0Clock,			//void (*clockFunc)(bool en);
	enableTimer0Interrup,		//void (*nvicFunc)(bool en);
	0,							//void (*resetFunc)(void);
	getTimer0ClockFrequency		//uint32_t (*getClockFunc)(void);
};


static const Timer::setup_t gTimer0Setup = 
{
	PWM0,						//YSS_TIMER_Dev *dev;
	0							//uint8_t index;
};

Timer timer0(gDrvTimer0Setup, gTimer0Setup);

extern "C"
{
void PWM0_IRQHandler(void)
{
	PWM0->ICR = PWM_CHn_ICR_OIC;
	timer0.isrUpdate();
}
}

#endif
#endif

