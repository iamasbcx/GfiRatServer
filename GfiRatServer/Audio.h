// Audio.h: interface for the CAudio class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <MMSYSTEM.H>
#include <MMReg.h>

#pragma comment(lib, "Winmm.lib")

class CAudio
{
public:
	CAudio();
	virtual ~CAudio();
	GSM610WAVEFORMAT m_GSMWavefmt;
	ULONG     m_ulBufferLength;
	LPWAVEHDR m_InAudioHeader[2];   //����ͷ
	LPBYTE    m_InAudioData[2];     //��������   ��������������
	HANDLE	  m_hEventWaveIn;
	HANDLE	  m_hStartRecord;    //�����¼�
	HWAVEIN   m_hWaveIn;         //�豸���	
	DWORD     m_nWaveInIndex;
	bool    m_hThreadCallBack;
	static DWORD WINAPI waveInCallBack(LPVOID lParam);   //���͵����ض�
	LPBYTE GetRecordBuffer(LPDWORD dwBufferSize);
	BOOL InitializeWaveIn();
	BOOL m_bIsWaveInUsed;

	HWAVEOUT m_hWaveOut;
	BOOL	m_bExit;
	BOOL     m_bIsWaveOutUsed;
	DWORD    m_nWaveOutIndex;
	LPWAVEHDR m_OutAudioHeader[2];   //����ͷ
	LPBYTE    m_OutAudioData[2];     //��������   ��������������
	BOOL PlayBuffer(LPBYTE szBuffer, DWORD dwBufferSize);
	BOOL InitializeWaveOut();
};
