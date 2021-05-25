#pragma once


/////////////////////////////////////////////////////////////////////////////
// CSerManager dialog


class CSerManager : public CDialog
{
	// Construction
public:
	CSerManager(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext* pContext = NULL);   // standard constructor
	void OnReceiveComplete();
	// Dialog Data
		//{{AFX_DATA(CSerManager)
	enum { IDD = IDD_SERMANA };
	CListCtrl	m_list_server;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerManager)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
private:
	void AdjustList();

	void ShowServiceList();

	void GetServiceList();

	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CImageList     m_ImgList;
	// Generated message map functions
	//{{AFX_MSG(CSerManager)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuRef();
	afx_msg void OnMenuStart();
	afx_msg void OnMenuStop();
	afx_msg void OnMenuDel();
	afx_msg void OnMenuCreate();
	afx_msg void OnMenuStartAuto();
	afx_msg void OnMenuStartManager();
	afx_msg void OnMenuStartDisable();
	afx_msg void OnDblclkListServer(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};