
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg dialog

class CKeyBoardDlg : public CDialog
{
// Construction
public:
	void OnReceiveComplete();
	CKeyBoardDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyBoardDlg)
	enum { IDD = IDD_KEYBOARD };
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyBoardDlg)
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetLog();
	afx_msg void OnSaveLog();
	afx_msg void OnCleanLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//	bool SaveRecord();
	void UpdateTitle();
//	void ResizeEdit();
	void AddKeyBoardData();
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CString m_IPAddress;
	bool m_bIsOfflineRecord;
	void SendException();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
