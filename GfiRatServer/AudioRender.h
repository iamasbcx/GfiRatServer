#ifndef __AUDIO_RENDER_H__
#define __AUDIO_RENDER_H__

// Windows 头文件:
#include <windows.h>
#include "windowsx.h"
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "Buffer.h"
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <process.h>
#include <avrt.h>
#include<iostream>
#include<fstream>
using namespace std;
#define AUDIO_RENDER_CLASS _T("audio_render_message_class")
#include <mmreg.h>
#pragma comment(lib, "Avrt.lib")


class CAudioRenderImpl
{
public:
	CAudioRenderImpl();
	~CAudioRenderImpl();

	BOOL Initialize();
	VOID Destroy();

	BOOL Start();
	VOID Stop();

	BOOL IsInited() const;
	BOOL IsRendering() const;

	BOOL PlayBuffer(LPBYTE szBuffer, DWORD dwBufferSize);
	VOID OnThreadEnd();
	HANDLE m_hEventStarteduser;
	HANDLE m_hEventStarteduser2;

	CRITICAL_SECTION cs;// 临界区的声明
	CBuffer	m_WriteBuffer;
	 int time;
private:
	IMMDevice* GetDefaultDevice();

private:
	HWND m_hWndMessage;
	HANDLE m_hEventStarted;
	HANDLE m_hEventStop;
	IMMDevice* m_pDevice;

	HANDLE m_hThreadRender;


	BOOL m_bInited;

	LPBYTE szBufferuser;
	DWORD dwBufferSizeuser;

};


#endif //__AUDIO_RENDER_H__