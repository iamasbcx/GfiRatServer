#pragma once

#include "IOCPServer.h"
#include "PlaybackAudioCapture.h"
#include "AudioRender.h"

// CSpeakerDlg �Ի���

class CSpeakerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpeakerDlg)

public:
	CSpeakerDlg(CWnd* pParent = NULL, CIOCPServer* IOCPServer = NULL, ClientContext* ContextObject = NULL);   // ��׼���캯��
	virtual ~CSpeakerDlg();
	ClientContext* m_ContextObject;
	CIOCPServer* m_iocpServer;
	HICON          m_hIcon;
	CString       m_strIPAddress;
	DWORD         m_nTotalRecvBytes;
	BOOL          m_bIsWorking;
	BOOL		  m_bThreadRun;
	HANDLE        m_hWorkThread;
	CAudioRenderImpl SetSpeakerDate;
	CPlaybackCaptureImpl GetSpeakerDate;

	static DWORD  CSpeakerDlg::WorkThread(LPVOID lParam);

	void CSpeakerDlg::OnReceiveComplete(void);
	// �Ի�������
	enum {
		IDD = IDD_SPEAKER
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSend; // �Ƿ��ͱ���������Զ��
	BOOL m_brec; // �Ƿ��������������
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	void OnReceive();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
};
