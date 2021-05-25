
#pragma once

// RegDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegDataDlg dialog

class CRegDataDlg : public CDialog
{
// Construction
public:
	void EnableKey();
	void EnablePath();
	CRegDataDlg(CWnd* pParent = NULL);   // standard constructor
	bool EPath;
	bool EKey;
	bool isOK;
	bool isDWORD;             //ÊÇ·ñÎªDWORDÖµ
// Dialog Data
	//{{AFX_DATA(CRegDataDlg)
	enum { IDD = IDD_REGDATA_DLG };
	CString	m_path;
	CString	m_key;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegDataDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeKey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

