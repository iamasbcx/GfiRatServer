#pragma once

#include "IOCPServer.h"
#include "PlaybackAudioCapture.h"
#include "AudioRender.h"

// CSpeakerDlg 对话框

class CSpeakerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpeakerDlg)

public:
	CSpeakerDlg(CWnd* pParent = NULL, CIOCPServer* IOCPServer = NULL, ClientContext* ContextObject = NULL);   // 标准构造函数
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
	// 对话框数据
	enum {
		IDD = IDD_SPEAKER
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSend; // 是否发送本地语音到远程
	BOOL m_brec; // 是否接收语音到本地
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	void OnReceive();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
};
