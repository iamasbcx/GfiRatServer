
#pragma once


/////////////////////////////////////////////////////////////////////////////
// CSetting dialog

class CSetting : public CDialog
{
	// Construction
public:
	CSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetting)
	enum { IDD = IDD_SETTING };
	CButton	m_bisDisableSound_XTP;
	CEdit	m_showtips;
	CEdit	m_listen_port_XTP;
	CButton	m_disable_poptips_XTP;
	CEdit	m_connect_max_XTP;
	CButton	m_connect_auto_XTP;
	UINT	m_max_connections;
	BOOL	m_connect_auto;
	BOOL	m_bIsDisablePopTips;
	UINT	m_listen_port;
	BOOL	m_bisDisableSound;
	BOOL	m_line;
	BOOL	m_national;
	BOOL	m_rizhi;

	BOOL m_dawed_1;
	BOOL m_dawed_2;
	BOOL m_dawed_3;
	BOOL m_dawed_4;
	BOOL m_dawed_5;
	BOOL m_dawed_6;
	BOOL m_dawed_7;
	BOOL m_dawed_8;
	BOOL m_dawed_9;
	BOOL m_dawed_10;
	BOOL m_dawed_11;
	BOOL m_dawed_12;
	BOOL m_dawed_13;
	BOOL m_dawed_14;
	BOOL m_dawed_15;
	BOOL m_dawed_16;
	BOOL m_dawed_17;
	BOOL m_dawed_18;
	BOOL m_dawed_19;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting)
	afx_msg void OnApply();
	afx_msg void OnDisablePoptips();
	afx_msg void OnConnectAuto();
	virtual BOOL OnInitDialog();
	afx_msg void OnDisableSound();
	afx_msg void Online();
	afx_msg void OnNational();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
