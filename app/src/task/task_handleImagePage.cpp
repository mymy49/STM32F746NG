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
#include <../bmp/folder.h>
#include <../bmp/picture.h>
#include <../font/Noto_Sans_CJK_HK_14.h>

#include <yss/Directory.h>
#include <yss/Fat32.h>
#include <yss/File.h>
#include <stdio.h>
#include <string.h>

class FileExplorer : public Object
{
public :
	FileExplorer(void)
	{
		setSize(480, 272);
		mLastDirCnt = mLastFileCnt = 0;
	}

	virtual ~FileExplorer(void)
	{

	}
	
	void refresh(void)
	{
		mEditLocker.lock();
		paint();
		update();
		mEditLocker.unlock();
	}

private :
	uint32_t mLastFileCnt, mLastDirCnt;

	virtual void paint(void)
	{
		uint32_t len;
		char *str = new char[256];
		char *name = new char[256];
		Position_t posIcon = {10, 10}, posName = {35, 12};

		if(sdmmc.isConnected())
		{
			Fat32 fat32(sdmmc);
			Directory dir(fat32);

			dir.initialize();
		
			uint32_t fileCnt = dir.getFileCount(), dirCnt = dir.getDirectoryCount();
			uint32_t drawCnt = 0;

			mFrameBuffer->setBackgroundColor(0x30, 0xFF, 0x30);
			mFrameBuffer->clear();

			mFrameBuffer->setBrushColor(0x30, 0x80, 0x30);
			mFrameBuffer->fillRect({5, 5}, Size_t{470, 262});

			mFrameBuffer->setFontColor(0xFF, 0xFF, 0xFF);
			mFrameBuffer->setFont(Font_Noto_Sans_CJK_HK_14);
			
			for(uint32_t i=0;i<dirCnt;i++)
			{
				if(drawCnt >= 10)
					break;

				dir.getDirectoryName(i, name, 256);

				sprintf(str, "%s", name);
				mFrameBuffer->drawBitmap(posIcon, folder);
				mFrameBuffer->drawString(posName, str);

				posName.y += 24;
				posIcon.y += 24;
				drawCnt++;
			}

			for(uint32_t i=0;i<fileCnt;i++)
			{
				if(drawCnt >= 10)
					break;

				dir.getFileName(i, name, 256);
				len = strlen(name);

				if(	(name[len-4] == '.' && name[len-3] == 'b' && name[len-2] == 'm' && name[len-1] == 'p') ||
					(name[len-4] == '.' && name[len-3] == 'B' && name[len-2] == 'M' && name[len-1] == 'P'))
				{
					sprintf(str, "%s", name);
					mFrameBuffer->drawBitmap(posIcon, picture);
					mFrameBuffer->drawString(posName, str);

					posName.y += 24;
					posIcon.y += 24;
					drawCnt++;
				}
			}
			Object::update();

		}
		else
		{
			mFrameBuffer->clear();
			Object::update();
		}

		delete str;
		delete name;
	}
};

namespace Task
{
	void handler_xBt(void);

	static Frame *gFrame;
	static FileExplorer *gFileExplorer;

	void thread_handleImagePage(void)
	{
		bool mLastDetectFlag = false, mLastConnectFlag = false, flag;
		
		while(1)
		{
			flag = sdmmc.isDetected();
			if(mLastDetectFlag != flag)
			{
				mLastDetectFlag = flag;

				if(flag && !sdmmc.isConnected())
				{
					sdmmc.connect();
					gFileExplorer->refresh();
				}
				else if(!flag && sdmmc.isConnected())
				{
					sdmmc.disconnect();
					gFileExplorer->refresh();
				}
			}
			thread::yield();
		}
	}

	error handleImagePage(FunctionQueue *obj)
	{
		(void)obj;

		lock();	// unlock()을 만날 때까지 외부에서 이 함수를 강제 종료 시키지 못한다.
		clearTask();	// 이전에 등록된 쓰레드 등을 전부 제거한다.

		gFrame = new Frame;
		gFileExplorer = new FileExplorer;
		ImageButton *xBt = new ImageButton(&xButton);

		xBt->setPushEventHandler(handler_xBt);
		xBt->setPosition(480 - 35 - 10, 10);
		
		gFrame->add(gFileExplorer);
		gFrame->add(xBt);

		setFrame(gFrame);

		addThread(thread_handleImagePage, 1024);	// thread_handleMainPage() 함수를 스케줄러에 등록한다.
													// addThread() 함수를 통해 등록된 쓰레드는 clearTask() 함수 호출시 종료 된다.

		unlock();	// 외부에서 강제로 종료가 가능하다.

		return error::ERROR_NONE;
	}
}


