// Settings.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "Setting.h"
#include "GfiRatServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CGfiRatServerDlg* g_pFrame;
extern CIOCPServer* m_iocpServer;
BOOL dawed_1, dawed_2, dawed_3, dawed_4, dawed_5, dawed_6, dawed_7, dawed_8, dawed_9, dawed_10, dawed_11, dawed_12, dawed_13, dawed_14, dawed_15, dawed_16, dawed_17, dawed_18, dawed_19;
/////////////////////////////////////////////////////////////////////////////
// CSetting dialog


CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetting)
	m_max_connections = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnection", 8000);
	m_connect_auto = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnectionAuto", 0);
	m_bIsDisablePopTips = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "PopTips", false);
	m_listen_port = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 81);
	m_bisDisableSound = FALSE;
	m_line = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "gridlines", false);//网格线
	m_national = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "national", false);//国旗
	m_rizhi = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "rizhi", false);//日志



	dawed_1 = m_dawed_1 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width01", TRUE);
	dawed_2 = m_dawed_2 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width02", TRUE);
	dawed_3 = m_dawed_3 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width03", TRUE);
	dawed_4 = m_dawed_4 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width04", TRUE);
	dawed_5 = m_dawed_5 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width05", TRUE);
	dawed_6 = m_dawed_6 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width06", TRUE);
	dawed_7 = m_dawed_7 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width07", TRUE);
	dawed_8 = m_dawed_8 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width08", TRUE);
	dawed_9 = m_dawed_9 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width09", TRUE);
	dawed_10 = m_dawed_10 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width10", TRUE);
	dawed_11 = m_dawed_11 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width11", TRUE);
	dawed_12 = m_dawed_12 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width12", TRUE);
	dawed_13 = m_dawed_13 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width13", TRUE);
	dawed_14 = m_dawed_14 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width14", TRUE);
	dawed_15 = m_dawed_15 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width15", TRUE);
	dawed_16 = m_dawed_16 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width16", TRUE);
	dawed_17 = m_dawed_17 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width17", TRUE);
	dawed_18 = m_dawed_18 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width18", TRUE);
	dawed_19 = m_dawed_19 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Settings", "Width19", TRUE);
	//}}AFX_DATA_INIT
}


void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting)
	DDX_Control(pDX, IDC_DISABLE_SOUND, m_bisDisableSound_XTP);
//	DDX_Control(pDX, IDC_SHOWTIPS, m_showtips);
	DDX_Control(pDX, IDC_LISTEN_PORT, m_listen_port_XTP);
	DDX_Control(pDX, IDC_DISABLE_POPTIPS, m_disable_poptips_XTP);
	DDX_Control(pDX, IDC_CONNECT_MAX, m_connect_max_XTP);
	DDX_Control(pDX, IDC_CONNECT_AUTO, m_connect_auto_XTP);
	DDX_Text(pDX, IDC_CONNECT_MAX, m_max_connections);
	DDV_MinMaxUInt(pDX, m_max_connections, 1, 10000);
	DDX_Check(pDX, IDC_CONNECT_AUTO, m_connect_auto);
	DDX_Check(pDX, IDC_DISABLE_POPTIPS, m_bIsDisablePopTips);
	DDX_Text(pDX, IDC_LISTEN_PORT, m_listen_port);
	DDV_MinMaxUInt(pDX, m_listen_port, 1, 65535);
	DDX_Check(pDX, IDC_DISABLE_SOUND, m_bisDisableSound);
	DDX_Check(pDX, IDC_LINE, m_line);
	DDX_Check(pDX, IDC_NATIONAL, m_national);
	DDX_Check(pDX, IDC_CHECK1, m_rizhi);

	DDX_Check(pDX, IDC_DAWED_1, m_dawed_1);
	DDX_Check(pDX, IDC_DAWED_2, m_dawed_2);
	DDX_Check(pDX, IDC_DAWED_3, m_dawed_3);
	DDX_Check(pDX, IDC_DAWED_4, m_dawed_4);
	DDX_Check(pDX, IDC_DAWED_5, m_dawed_5);
	DDX_Check(pDX, IDC_DAWED_6, m_dawed_6);
	DDX_Check(pDX, IDC_DAWED_7, m_dawed_7);
	DDX_Check(pDX, IDC_DAWED_8, m_dawed_8);
	DDX_Check(pDX, IDC_DAWED_9, m_dawed_9);
	DDX_Check(pDX, IDC_DAWED_10, m_dawed_10);
	DDX_Check(pDX, IDC_DAWED_11, m_dawed_11);
	DDX_Check(pDX, IDC_DAWED_12, m_dawed_12);
	DDX_Check(pDX, IDC_DAWED_13, m_dawed_13);
	DDX_Check(pDX, IDC_DAWED_14, m_dawed_14);
	DDX_Check(pDX, IDC_DAWED_15, m_dawed_15);
	DDX_Check(pDX, IDC_DAWED_16, m_dawed_16);
	DDX_Check(pDX, IDC_DAWED_17, m_dawed_17);
	DDX_Check(pDX, IDC_DAWED_18, m_dawed_18);
	DDX_Check(pDX, IDC_DAWED_19, m_dawed_19);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting, CDialog)
	//{{AFX_MSG_MAP(CSetting)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DISABLE_POPTIPS, OnDisablePoptips)
	ON_BN_CLICKED(IDC_CONNECT_AUTO, OnConnectAuto)
	ON_BN_CLICKED(IDC_DISABLE_SOUND, OnDisableSound)
	ON_BN_CLICKED(IDC_LINE, Online)
	ON_BN_CLICKED(IDC_NATIONAL, OnNational)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting message handlers

void CSetting::OnApply()
{
	// TODO: Add your control notification handler code here
	int prev_port = m_listen_port;
	int prev_max_connections = m_max_connections;

	UpdateData(TRUE);

	if (prev_max_connections != m_max_connections)
	{
		if (m_connect_auto)
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, 8000);
		else
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, m_max_connections);
	}

	if (prev_port != m_listen_port)
		g_pFrame->Activate(m_listen_port, m_iocpServer->m_nMaxConnections);

	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "ListenPort", m_listen_port);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnection", m_max_connections);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);



	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width01", m_dawed_1);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width02", m_dawed_2);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width03", m_dawed_3);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width04", m_dawed_4);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width05", m_dawed_5);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width06", m_dawed_6);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width07", m_dawed_7);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width08", m_dawed_8);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width09", m_dawed_9);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width10", m_dawed_10);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width11", m_dawed_11);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width12", m_dawed_12);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width13", m_dawed_13);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width14", m_dawed_14);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width15", m_dawed_15);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width16", m_dawed_16);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width17", m_dawed_17);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width18", m_dawed_18);
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Width19", m_dawed_19);
	//SetDlgItemText(IDC_SHOWTIPS, "已保存设置");

	BOOL dawed_DATA = NULL;
	if ((dawed_1 != m_dawed_1) || (dawed_2 != m_dawed_2) || (dawed_3 != m_dawed_3) || (dawed_4 != m_dawed_4) || (dawed_5 != m_dawed_5) ||
		(dawed_6 != m_dawed_6) || (dawed_7 != m_dawed_7) || (dawed_8 != m_dawed_8) || (dawed_9 != m_dawed_9) || (dawed_10 != m_dawed_10) ||
		(dawed_11 != m_dawed_11) || (dawed_12 != m_dawed_12) || (dawed_13 != m_dawed_13) || (dawed_14 != m_dawed_14) || (dawed_15 != m_dawed_15)
		|| (dawed_16 != m_dawed_16) || (dawed_17 != m_dawed_17) || (dawed_18 != m_dawed_18) || (dawed_19 != m_dawed_19)
		)
	{
		dawed_DATA = TRUE;
	}


	if (MessageBox("系统参数已经更改，需重启软件生效...", " 温馨提示！", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		g_pFrame->m_bSetRestart = TRUE;
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

void CSetting::OnDisablePoptips()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "PopTips", m_bIsDisablePopTips);
	((CGfiRatServerApp*)AfxGetApp())->m_bIsDisablePopTips = m_bIsDisablePopTips;
	if (((CButton*)GetDlgItem(IDC_DISABLE_POPTIPS))->GetCheck() == TRUE)
	{
		//SetDlgItemText(IDC_SHOWTIPS, "已关闭上线气泡显示");
	}
	else
	{
		//SetDlgItemText(IDC_SHOWTIPS, "已开启上线气泡显示");
	}
}

void CSetting::OnConnectAuto()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(!m_connect_auto);
	m_connect_auto = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);
	if (((CButton*)GetDlgItem(IDC_CONNECT_AUTO))->GetCheck() == TRUE)
	{
		//SetDlgItemText(IDC_SHOWTIPS, "无限上线数量");
	}
	else
	{
		//SetDlgItemText(IDC_SHOWTIPS, "有限上线数量，请自定义");
	}
}

BOOL CSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if (m_connect_auto == TRUE)
		GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(TRUE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetting::OnDisableSound()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "Sound", m_bisDisableSound);
	((CGfiRatServerApp*)AfxGetApp())->m_bisDisableSound = m_bisDisableSound;
	if (((CButton*)GetDlgItem(IDC_DISABLE_SOUND))->GetCheck() == TRUE)
	{
		//SetDlgItemText(IDC_SHOWTIPS, "已关闭提示音");
	}
	else
	{
		//SetDlgItemText(IDC_SHOWTIPS, "已开启提示音");
	}
}
//新添加
void CSetting::Online()
{
	UpdateData();
	m_line = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "gridlines", m_line);
	//SetDlgItemText(IDC_SHOWTIPS, "信息提示:列表网格设置设置成功");
	if (MessageBox("系统参数已经更改，需重启软件生效...", " 温馨提示！", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		g_pFrame->m_bSetRestart = TRUE;
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}

void  CSetting::OnNational()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (MessageBox("系统参数已经更改，需重启软件生效...", " 温馨提示！", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "National", m_national);
	//	SetDlgItemText(IDC_SHOWTIPS, "信息提示:显示国旗图标设置成功");
		g_pFrame->m_bSetRestart = TRUE;
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}
void CSetting::OnCheck1()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_rizhi = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.SetInt("Settings", "rizhi", m_rizhi);
//	SetDlgItemText(IDC_SHOWTIPS, "信息提示:日志栏设置成功");
	if (MessageBox("系统参数已经更改，需重启软件生效...", " 温馨提示！", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		g_pFrame->m_bSetRestart = TRUE;
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}
