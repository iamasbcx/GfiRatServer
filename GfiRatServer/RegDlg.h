

#pragma once

// RegDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegDlg dialog
enum MYKEY
{
	MHKEY_CLASSES_ROOT,
	MHKEY_CURRENT_USER,
	MHKEY_LOCAL_MACHINE,
	MHKEY_USERS,
	MHKEY_CURRENT_CONFIG
};

enum KEYVALUE
{
	MREG_SZ,
	MREG_DWORD,
	MREG_QWORD,
	MREG_BINARY,
	MREG_EXPAND_SZ,
	MREG_MULTI_SZ
};

struct REGMSG
{
	int count;         //名字个数
	DWORD size;        //名字大小
	DWORD valsize;     //值大小	
};

class CRegDlg : public CDialog
{
	// Construction
public:
	BYTE getType(int index);
	DWORD atod(char* ch);
	CString GetFullPath(HTREEITEM hCurrent);
	void EnableCursor(bool b);
	void TestOK();
	char getFatherPath(CString& FullPath);
	void addKey(char* buf);
	void addPath(char* tmp);
	void OnReceiveComplete();
	void CreatStatusBar();
	CRegDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext* pContext);   // standard constructor
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	// Dialog Data
		//{{AFX_DATA(CRegDlg)
	enum { IDD = IDD_REG_DIALOG };
	CListCtrl	m_list;
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRegtDel();
	afx_msg void OnRegtCreat();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReglEdit();
	afx_msg void OnReglDelkey();
	afx_msg void OnReglStr();
	afx_msg void OnReglDword();
	afx_msg void OnReglExstr();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	HICON       m_hIcon;
	HTREEITEM	m_hRoot;
	HTREEITEM	HKLM;
	HTREEITEM	HKCR;
	HTREEITEM	HKCU;
	HTREEITEM	HKUS;
	HTREEITEM	HKCC;
	CImageList	m_ImageList_tree;
	CImageList	m_HeadIcon;

	HTREEITEM	SelectNode;
	CString		FullPath;
	int         size[3];
private:
	int how;       //执行了什么操作 1，删除项  2，新建项   3，删除键  4， 新建项  5，编辑项               
	int index;
	BYTE  type;
	CString Path;
	CString Key;
	CString Value;
	bool isEnable;    //控件是否可用
	bool isEdit;      //是否正在编辑
	CStatusBar  m_wndStatusBar;
};

