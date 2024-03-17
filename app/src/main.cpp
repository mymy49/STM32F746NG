/*
	Copyright 2024. 홍윤기 all right reserved.

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

#include <yss.h>
#include <bsp.h>
#include "memory.h"
#include "task.h"

int main(void)
{
	bool mLastDetectFlag = false, flag;

	// 운영체체 초기화
	initializeYss();
	
	// 보드 초기화
	initializeBoard();

	// 설정 저장용 메모리 초기화
	memory::initialize();

	// Function Queue 처리 시작
	fq.start();

	fq.add(Task::displayLogo);		// 로고 출력
	fq.add(Task::handleMainPage);	// 메인 페이지 처리

	while(1)
	{
		flag = sdmmc.isDetected();

		if(mLastDetectFlag != flag)
		{
			mLastDetectFlag = flag;

			if(flag && !sdmmc.isConnected())
				sdmmc.connect();
			else if(!flag && sdmmc.isConnected())
				sdmmc.disconnect();
		}
		thread::yield();
	}
}

