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

#define MAX_LIST_COUNT		10

class BmpViewer :Object
{
public :
	BmpViewer(void)
	{

	}

	virtual ~BmpViewer(void)
	{

	}

private :
	virtual void paint(void)
	{

	}
};

class FileExplorer : public Object
{
public :
	FileExplorer(void)
	{
		mFat32 = new Fat32(sdmmc);
		mDir = new Directory(mFat32);

		setSize(480, 272);
		mLastDirCnt = mLastFileCnt = 0;
		mDisplayedFileCount = mDisplayedFolderCount = 0;
		mPage = 0;
		mLastConnectFlag = false;

		for(uint8_t i=0;i<MAX_LIST_COUNT;i++)
			mList[i] = 0;

		mFrameBuffer->setBackgroundColor(0x30, 0xFF, 0x30);
	}

	virtual ~FileExplorer(void)
	{
		delete mDir;
		delete mFat32;
	}
	
	void refresh(void)
	{
		bool flag = sdmmc.isConnected();
		mEditLocker.lock();

		if(flag && mLastConnectFlag == false)
		{
			mDir->initialize();
			mLastConnectFlag = true;
		}
		else if(!flag)
		{
			for(int32_t i=0;i<MAX_LIST_COUNT;i++)
				mList[i] = 0;

			mLastDirCnt = mLastFileCnt = 0;
			mDisplayedFileCount = mDisplayedFolderCount = 0;
			mPage = 0;
			mLastConnectFlag = false;
		}

		paint();
		update();
		mEditLocker.unlock();
	}

private :
	int32_t mLastFileCnt, mLastDirCnt, mList[10], mPage;
	uint8_t mDisplayedFileCount, mDisplayedFolderCount;
	const Position_t mListStartPos = {10, 10};
	const Size_t mListSize = {460, 24};
	bool mLastConnectFlag;
	Fat32 *mFat32;
	Directory *mDir;

	virtual void paint(void)
	{
		int32_t len;
		char *str = new char[256];
		char *name = new char[256];
		Position_t posIcon = mListStartPos, posName = {(int16_t)(mListStartPos.x + 25), (int16_t)(mListStartPos.y + 2)};

		if(sdmmc.isConnected())
		{
			int32_t fileCnt = mDir->getFileCount(), dirCnt = mDir->getDirectoryCount();
			int32_t drawCnt = 0;

			mFrameBuffer->clear();

			mFrameBuffer->setBrushColor(0x30, 0x80, 0x30);
			mFrameBuffer->fillRect({5, 5}, Size_t{470, 262});

			mFrameBuffer->setFontColor(0xFF, 0xFF, 0xFF);
			mFrameBuffer->setFont(Font_Noto_Sans_CJK_HK_14);
			
			for(int32_t i=mPage*MAX_LIST_COUNT;i<dirCnt;i++)
			{
				if(drawCnt >= MAX_LIST_COUNT)
					break;

				mDir->getDirectoryName(i, name, 256);

				sprintf(str, "%s", name);
				mFrameBuffer->drawBitmap(posIcon, folder);
				mFrameBuffer->drawString(posName, str);

				posName.y += 24;
				posIcon.y += 24;
				mList[drawCnt++] = i;
				mDisplayedFolderCount = drawCnt;
			}

			for(int32_t i=mPage*MAX_LIST_COUNT;i<fileCnt;i++)
			{
				if(drawCnt >= MAX_LIST_COUNT)
					break;

				mDir->getFileName(i, name, 256);
				len = strlen(name);

				if(	(name[len-4] == '.' && name[len-3] == 'b' && name[len-2] == 'm' && name[len-1] == 'p') ||
					(name[len-4] == '.' && name[len-3] == 'B' && name[len-2] == 'M' && name[len-1] == 'P'))
				{
					sprintf(str, "%s", name);
					mFrameBuffer->drawBitmap(posIcon, picture);
					mFrameBuffer->drawString(posName, str);

					posName.y += 24;
					posIcon.y += 24;
					mList[drawCnt++] = i;
					mDisplayedFileCount = drawCnt - mDisplayedFolderCount;
				}
			}
			Object::update();

			for(int32_t i=drawCnt;i<MAX_LIST_COUNT;i++)
			{
				mList[drawCnt++] = 0;
			}
		}
		else
		{
			mFrameBuffer->clear();
			Object::update();
		}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmismatched-new-delete"
		delete str;
		delete name;
#pragma GCC diagnostic pop
	}

	virtual Object *handlerPush(Position_t pos)
	{
		int32_t index;

		pos.x -= mListStartPos.x;
		pos.y -= mListStartPos.y;
		index = pos.y / mListSize.height;
		
		if(pos.x < mListSize.width && index < MAX_LIST_COUNT)
		{
			if(index < mDisplayedFolderCount)
			{
				mDir->enterDirectory(mList[index]);
				mPage = 0;
				refresh();
			}
			else if(index < mDisplayedFolderCount + mDisplayedFileCount)
			{
				setVisible(false);
				refresh();
			}
		}

		return this;
	}
};

namespace Task
{
	void handler_xBt(void);

	static Frame *gFrame;
	static FileExplorer *gFileExplorer;

	void thread_handleImagePage(void)
	{
		bool mLastConnectFlag = false, flag;
		
		gFileExplorer->refresh();

		while(1)
		{
			flag = sdmmc.isConnected();

			if(mLastConnectFlag != flag)
			{
				mLastConnectFlag = flag;
				gFileExplorer->refresh();
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


