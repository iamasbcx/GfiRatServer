
#pragma once


/////////////////////////////////////////////////////////////////////////////
// CFileTransferModeDlg dialog


class CFileTransferModeDlg : public CDialog
{
	// Construction
public:
	CString m_strFileName;
	CFileTransferModeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileTransferModeDlg)
	enum { IDD = IDD_TRANSFERMODE_DLG };
	// NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileTransferModeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileTransferModeDlg)
	afx_msg	void OnEndDialog(UINT id);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnBnClickedOverwrite();
	afx_msg void OnBnClickedOverwriteAll();
	afx_msg void OnBnClickedAddition();
	afx_msg void OnBnClickedAdditionAll();
	afx_msg void OnBnClickedJump();
	afx_msg void OnBnClickedJumpAll();
	afx_msg void OnBnClickedCancel();
};
