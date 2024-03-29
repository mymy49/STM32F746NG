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

#include <mod/logic/SN74LV595A.h>
#include <std_ext/malloc.h>

#if !(defined(YSS_DRV_SPI_UNSUPPORTED) || defined(YSS_DRV_GPIO_UNSUPPORTED))

static const Spi::specification_t gConfig =
	{
		define::spi::mode::MODE0,
		40000000,
		define::spi::bit::BIT8
	};

SN74LV595A::SN74LV595A(void)
{
	reset();
}

void SN74LV595A::reset(void)
{
	mOe.port = 0;
	mRclk.port = 0;
	mSrclr.port = 0;
	mPeri = 0;
}

bool SN74LV595A::initialize(Config config)
{
	mPeri = &config.spi;
	mOe = config.OE;
	mRclk = config.RCLK;
	mSrclr = config.SRCLR;

	if(mOe.port)
		mOe.port->setOutput(mOe.pin, false);

	if(mRclk.port)
		mRclk.port->setOutput(mRclk.pin, false);

	if (mSrclr.port)
	{
		mSrclr.port->setOutput(mSrclr.pin, false);
		mSrclr.port->setOutput(mSrclr.pin, true);
	}

	return true;
}

void SN74LV595A::set(uint8_t data)
{
	mPeri->lock();
	mPeri->setSpecification(gConfig);
	mPeri->enable(true);
	mPeri->send(data);
	if(mRclk.port)
	{
		mRclk.port->setOutput(mRclk.pin, false);
		mRclk.port->setOutput(mRclk.pin, true);
	}
	mPeri->enable(false);
	mPeri->unlock();
}

void SN74LV595A::set(uint8_t *data, uint8_t size)
{
	mPeri->lock();
	mPeri->setSpecification(gConfig);
	mPeri->enable(true);
	mPeri->send((int8_t *)data, size);
	if(mRclk.port)
	{
		mRclk.port->setOutput(mRclk.pin, false);
		mRclk.port->setOutput(mRclk.pin, true);
	}
	mPeri->enable(false);
	mPeri->unlock();
}

void SN74LV595A::setOutputEn(bool en)
{
	if (mRclk.port)
		mRclk.port->setOutput(mRclk.pin, !en);
}

#endif