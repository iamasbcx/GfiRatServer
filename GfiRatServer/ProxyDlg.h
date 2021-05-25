#pragma once

// ProxyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProxyDlg dialog

class CProxyDlg : public CDialog
{
	// Construction
public:
	CProxyDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext* pContext = NULL);   // standard constructor
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	ClientContext* pContexts[10000];
	void OnReceiveComplete();
	void AddLog(TCHAR* lpText);
	LRESULT OnNotifyProc(WPARAM wParam, LPARAM lParam);
	// Dialog Data
		//{{AFX_DATA(CProxyDlg)
	enum { IDD = IDD_SOCKS };
	CEdit	m_edit;
	// NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProxyDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProxyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CIOCPLOCAL* m_iocpLocal;
};
