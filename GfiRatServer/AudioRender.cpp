#include "pch.h"
#include "AudioRender.h"

struct render_thread_data
{
	HANDLE hEventStarted;
	HANDLE hEventStop;
	CRITICAL_SECTION 临界锁;
	CBuffer*		动态内存;
	IMMDevice* pDevice;
	LPBYTE szBufferuser;
	DWORD dwBufferSizeuser;
};



CAudioRenderImpl::CAudioRenderImpl()
	:m_hThreadRender(NULL), m_bInited(FALSE), m_pDevice(NULL),
	 m_hEventStarted(NULL), m_hEventStop(NULL)
{

}

CAudioRenderImpl::~CAudioRenderImpl()
{
	if(m_bInited) Destroy();
}

IMMDevice* CAudioRenderImpl::GetDefaultDevice()
{
	IMMDevice* pDevice = NULL;
	IMMDeviceEnumerator *pMMDeviceEnumerator = NULL;
	HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, 
        __uuidof(IMMDeviceEnumerator),
        (void**)&pMMDeviceEnumerator
    );
	if(FAILED(hr)) return NULL;

    hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    pMMDeviceEnumerator->Release();
	return pDevice;
}

BOOL AdjustFormatTo16Bits2(WAVEFORMATEX* pwfx)
{
	BOOL bRet(FALSE);

	if (pwfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
	{
		pwfx->wFormatTag = WAVE_FORMAT_PCM;
		pwfx->wBitsPerSample = 16;
		pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
		pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

		bRet = TRUE;
	}
	else if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		PWAVEFORMATEXTENSIBLE pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
		if (IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, pEx->SubFormat))
		{
			pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
			pEx->Samples.wValidBitsPerSample = 16;
			pwfx->wBitsPerSample = 16;
			pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
			pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

			bRet = TRUE;
		}
	}

	return bRet;
}
UINT RenderAudio(CRITICAL_SECTION cs, CBuffer* dtncbuffer, IMMDevice* pDevice, HANDLE hEventStarted, HANDLE hEventStop, LPBYTE szBufferuser, DWORD dwBufferSizeuser)
{
	HRESULT hr;
	IAudioClient *pAudioClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	IAudioRenderClient *pAudioRenderClient = NULL;
	DWORD nTaskIndex = 0;
	HANDLE hTask = NULL;
	BOOL bStarted(FALSE);
	UINT32 nFrameBufferSize(0);
	HANDLE hEventRequestData = NULL;
	do 
	{/*
		ifstream outFile("students.dat", ios::in | ios::binary);
		outFile.seekg(0, std::ios::end);
		int len = outFile.tellg();
		if (len <= 0)
		{
			break;
		}*/

	//	outFile.seekg(0, std::ios::beg);
		hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
		if(FAILED(hr)) break;

		hr = pAudioClient->GetMixFormat(&pwfx);
		if (FAILED(hr)) break;

		SetEvent(hEventStarted);
		if (!AdjustFormatTo16Bits2(pwfx)) break;
		//NotifyWaveFormat(hWndMessage, pwfx);
		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 0, 0, pwfx, 0);
		if(FAILED(hr)) break;

		hr = pAudioClient->GetBufferSize(&nFrameBufferSize);
		if(FAILED(hr)) break;

		hr = pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pAudioRenderClient);
		if(FAILED(hr)) break;

		hTask = AvSetMmThreadCharacteristics((char*)"Playback", &nTaskIndex);
		if (NULL == hTask) break;

		hEventRequestData = CreateEvent(NULL, FALSE, FALSE, NULL);
		if(hEventRequestData == NULL) break;

		hr = pAudioClient->SetEventHandle(hEventRequestData);
		if(FAILED(hr)) break;

		hr = pAudioClient->Start();
		if(FAILED(hr)) break;

		//NotifyStatus(hWndMessage, RENDER_START, 0);
		bStarted = TRUE;

		HANDLE waitArray[2] = { hEventStop, hEventRequestData };
		DWORD dwWaitResult;
		BYTE *pData = NULL;
		UINT32 nFramesOfPadding(0);
		UINT nNeedDataLen(0);
		int bufferlen = 0,audiolen=0;
		
		while(TRUE)
		{
			dwWaitResult = WaitForMultipleObjects(sizeof(waitArray)/sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
			if(WAIT_OBJECT_0 == dwWaitResult) break;

			if (WAIT_OBJECT_0 + 1 != dwWaitResult)
			{
			//	NotifyStatus(hWndMessage, RENDER_ERROR);
				break;
			}
		
			hr = pAudioClient->GetCurrentPadding(&nFramesOfPadding);
			if(FAILED(hr)) break;

			if(nFrameBufferSize == nFramesOfPadding) continue;

			nNeedDataLen = nFrameBufferSize - nFramesOfPadding;
			hr = pAudioRenderClient->GetBuffer(nNeedDataLen, &pData);
			if(FAILED(hr))break;

			EnterCriticalSection(&cs);//进入临界区
			/////拿出数据
			
			bufferlen = dtncbuffer->GetBufferLen();
			audiolen = nNeedDataLen * pwfx->nBlockAlign;
			if(bufferlen > audiolen)
			dtncbuffer->Read(pData, nNeedDataLen * pwfx->nBlockAlign);
			LeaveCriticalSection(&cs);//离开临界区
		//	NotifyData(hWndMessage, pData, nNeedDataLen * pwfx->nBlockAlign);

			pAudioRenderClient->ReleaseBuffer(nNeedDataLen, 0);	
		}

	}while(FALSE);

	if(hEventRequestData != NULL)
	{
		CloseHandle(hEventRequestData);
		hEventRequestData = NULL;
	}

	if(hTask != NULL)
	{
		AvRevertMmThreadCharacteristics(hTask);
		hTask = NULL;
	}

	if(pAudioRenderClient != NULL)
	{
		pAudioRenderClient->Release();
		pAudioRenderClient = NULL;
	}

	if(pwfx != NULL)
	{
		CoTaskMemFree(pwfx);
		pwfx = NULL;
	}

	if(pAudioClient != NULL)
	{
		if(bStarted)
		{
			pAudioClient->Stop();
			//NotifyStatus(hWndMessage, RENDER_STOP);
		}

		pAudioClient->Release();
		pAudioClient = NULL;
	}

	return 0;
}

UINT __stdcall RenderTheadProc(LPVOID param)
{
	CoInitialize(NULL);

	render_thread_data* pData = (render_thread_data*)param;
	CRITICAL_SECTION 临界锁 = pData->临界锁;
	CBuffer* 动态内存 = pData->动态内存;
	HANDLE hEventStop = pData->hEventStop;
	IMMDevice* pDevice = pData->pDevice;
	HANDLE hEventStarted = pData->hEventStarted;
	LPBYTE szBufferuser = pData->szBufferuser;
	DWORD dwBufferSizeuser = pData->dwBufferSizeuser;
	UINT nRet = RenderAudio(临界锁, 动态内存, pDevice, hEventStarted, hEventStop, szBufferuser, dwBufferSizeuser);

	CoUninitialize();

	return nRet;
}

BOOL CAudioRenderImpl::Initialize()
{


	InitializeCriticalSection(&cs);//初始化临界区

	if(m_bInited) return TRUE;

	//m_pEventHandler = pHandler;

	do
	{
		

		m_pDevice = GetDefaultDevice();
		if(m_pDevice == NULL) break;

		m_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
		if(m_hEventStop == NULL) break;

		m_hEventStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
		if(m_hEventStarted == NULL) break;

		m_bInited = TRUE;
	
	}while(FALSE);

	if(!m_bInited)
	{
		Destroy();
	}

	return m_bInited;
}

VOID CAudioRenderImpl::Destroy()
{
	if(m_hWndMessage != NULL
		&& ::IsWindow(m_hWndMessage))
	{
		DestroyWindow(m_hWndMessage);
	}

	m_hWndMessage = NULL;

	if(m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if(m_hEventStop != NULL)
	{
		CloseHandle(m_hEventStop);
		m_hEventStop = NULL;
	}

	if(m_hEventStarted != NULL)
	{
		CloseHandle(m_hEventStarted);
		m_hEventStarted = NULL;
	}

	m_bInited = FALSE;
}

BOOL CAudioRenderImpl::Start()
{
	if(!m_bInited) return FALSE;
	if(m_hThreadRender != NULL) return TRUE;

	render_thread_data data;
	data.hEventStop = m_hEventStop;
	data.临界锁 = cs;
	data.动态内存 = &m_WriteBuffer;
	data.pDevice = m_pDevice;
	data.hEventStarted = m_hEventStarted;
	data.szBufferuser = szBufferuser;
	data.dwBufferSizeuser = dwBufferSizeuser;
	m_hThreadRender = (HANDLE)_beginthreadex(NULL, 0, &RenderTheadProc, &data, 0, NULL);
	if(m_hThreadRender == NULL) return FALSE;

	HANDLE arWaits[2] = {m_hEventStarted, m_hThreadRender};
	DWORD dwWaitResult = WaitForMultipleObjects(sizeof(arWaits)/sizeof(arWaits[0]), arWaits, FALSE, INFINITE);
	if(dwWaitResult != WAIT_OBJECT_0)
	{
		Stop();
		return FALSE;
	}

	return TRUE;
}

VOID CAudioRenderImpl::OnThreadEnd()
{
	if(m_hThreadRender != NULL)
	{
		CloseHandle(m_hThreadRender);
		m_hThreadRender = NULL;
	}
}

VOID CAudioRenderImpl::Stop()
{
	if(!m_bInited) return;

	if(m_hEventStop != NULL
		&& m_hThreadRender != NULL)
	{
		SetEvent(m_hEventStop);
		OnThreadEnd();
	}
}

BOOL CAudioRenderImpl::IsInited() const
{
	return m_bInited;
}

BOOL CAudioRenderImpl::IsRendering() const
{
	return m_hThreadRender != NULL;
}


BOOL CAudioRenderImpl::PlayBuffer(LPBYTE szBuffer, DWORD dwBufferSize)
{
	EnterCriticalSection(&cs);//进入临界区
	////加入数据
	m_WriteBuffer.Write(szBuffer, dwBufferSize);
	time++;
	if (time>100)
	{
		m_WriteBuffer.ClearBuffer();
		time = 0;
	}
	LeaveCriticalSection(&cs);//离开临界区
	
	return true;
}


