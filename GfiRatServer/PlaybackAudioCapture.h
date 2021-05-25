#ifndef __PLAYBACK_AUDIO_CAPTURE_H__
#define __PLAYBACK_AUDIO_CAPTURE_H__

#include <winsock2.h>
#include "IOCPServer.h"
#include "macros.h"
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//#include "ClassRegister.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <process.h>
#include <avrt.h>
#include<iostream>
#include<fstream>
#define AUDIO_CAPTURE_CLASS _T("audio_cpature_message_class")
using namespace std;
#include <mmreg.h>
#pragma comment(lib, "Avrt.lib")



class CPlaybackCaptureImpl
{
public:
	CPlaybackCaptureImpl();
	~CPlaybackCaptureImpl();

	BOOL Initialize(CIOCPServer* ClientObject, ClientContext* m_ContextObject);
	VOID Destroy();

	BOOL Start();
	VOID Stop();

	BOOL IsInited() const;
	BOOL IsCapturing() const;

	//	IPlaybackCaptureEvent* GetEventHandler() const { return m_pEventHandler; }
	VOID OnThreadEnd();

public:
	IMMDevice* GetDefaultDevice();

public:
	HWND m_hWndMessage;
	HANDLE m_hEventStarted;
	HANDLE m_hEventStop;
	IMMDevice* m_pDevice;

	HANDLE m_hThreadCapture;

	//static CClassRegister m_sClassRegister;
	BOOL m_bInited;

	//IPlaybackCaptureEvent* m_pEventHandler;



	CIOCPServer* CIOCPServerobjext;
	ClientContext* ClientContextobject;
};



#endif //__PLAYBACK_AUDIO_CAPTURE_H__