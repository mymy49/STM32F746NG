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

#include <task.h>
#include <yss.h>
#include <bsp.h>
#include <task.h>
#include <../bmp/infoBackground.h>
#include <../bmp/xButton.h>
#include <../font/Noto_Sans_CJK_HK_14.h>

class XButton : public Button
{
public :
	XButton(void)
	{
		setSize(xButton.width, xButton.height);
		mFrameBuffer->setBackgroundColor(0xFF, 0xFF, 0xFF, 0x00);
		mFrameBuffer->clear();
		mFrameBuffer->drawBitmap({0, 0}, xButton);
	}

	virtual ~XButton(void)
	{
		
	}

private :
	virtual void paint(void)
	{

	}
};

class Info : public Object
{
public :
	Info(void)
	{
		Position_t textPos = {70, 70};
		int16_t lineOffset = 30;
		Font font(Font_Noto_Sans_CJK_HK_14);

		// 객체 크기 설정
		setSize(infoBackground.width, infoBackground.height);
		
		// 배경 이미지 그리기
		mFrameBuffer->drawBitmap({0, 0}, infoBackground);
		
		// 본문 쓰기
		font.setSpaceWidth(6);
		mFrameBuffer->setFont(font);
		mFrameBuffer->setFontColor(0x00, 0x00, 0x00);

		mFrameBuffer->drawString(textPos, "본 소프트웨어는 이순신 OS를 위한 예제 프로젝트입니다.");
		textPos.y += lineOffset;
		mFrameBuffer->drawString(textPos, "전자 앨범과 탁상 시계 기능을 구현 할 예정입니다.");
		textPos.y += lineOffset;
		mFrameBuffer->drawString(textPos, "이순신 OS에서 권장하는 프로그래밍 패턴을");
		textPos.y += lineOffset;
		mFrameBuffer->drawString(textPos, "제공하기 위해 진행합니다.");
		textPos.y += lineOffset;

		

	}

	virtual ~Info(void)
	{
		
	}

private :
	virtual void paint(void)
	{

	}
};

namespace Task
{
	static Frame *gFrame;

	void thread_handleInfoPage(void)
	{
		while(1)
		{
			thread::yield();
		}
	}

	void handler_xBt(void)
	{
		fq.lock();
		fq.add(handleMainPage);
		fq.unlock();
	}

	error handleInfoPage(FunctionQueue *obj)
	{
		(void)obj;

		lock();	// unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();	// 이전에 등록된 쓰레드 등을 전부 제거한다.

		gFrame = new Frame;
		Info *info = new Info;
		XButton *xBt = new XButton;

		xBt->setPushEventHandler(handler_xBt);
		xBt->setPosition(480 - 35 - 10, 10);

		gFrame->add(info);
		gFrame->add(xBt);

		setFrame(gFrame);

		addThread(thread_handleInfoPage, 512);	// thread_handleMainPage() 함수를 스케줄러에 등록한다.
												// addThread() 함수를 통해 등록된 쓰레드는 clearTask() 함수 호출시 종료 된다.

		unlock();	// 외부에서 강제로 종료가 가능하다.

		return error::ERROR_NONE;
	}
}


