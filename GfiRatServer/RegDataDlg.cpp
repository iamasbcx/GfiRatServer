// RegDataDlg.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "RegDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegDataDlg dialog


CRegDataDlg::CRegDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegDataDlg::IDD, pParent)

{
	//{{AFX_DATA_INIT(CRegDataDlg)
	m_path = _T("");
	m_key = _T("");
	//}}AFX_DATA_INIT
	EPath=false;
	EKey=false;
	isOK=false;
    isDWORD=false;
}


void CRegDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegDataDlg)
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_KEY, m_key);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegDataDlg, CDialog)
	//{{AFX_MSG_MAP(CRegDataDlg)
	ON_EN_CHANGE(IDC_KEY, OnChangeKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegDataDlg message handlers

BOOL CRegDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(EPath==true){
		EnablePath();
	}
	if(EKey==true){
		EnableKey();
	}
     UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 路径变灰
void CRegDataDlg::EnablePath()
{
	GetDlgItem(IDC_PATH)->EnableWindow(false);
	UpdateData(false);
}

// key变灰
void CRegDataDlg::EnableKey()
{
	GetDlgItem(IDC_KEY)->EnableWindow(false);
	UpdateData(false);
}

void CRegDataDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	CString restr="Data can not empty!";
	if(m_path==""&&!EPath){
		MessageBox(restr);
		isOK=false;
		return;
	}
	if(m_key==""&&!EKey){
		MessageBox(restr);
		isOK=false;
		return;
	}
	isOK=true;
	CDialog::OnOK();
}

void CRegDataDlg::OnChangeKey() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!isDWORD) return;
	UpdateData(true);
	int length=m_key.GetLength();
	
	for(int i=0;i<length;i++){
		char ch=m_key.GetAt(i);
		if(ch>57||ch<48){
			m_key.Delete(i);
			UpdateData(false);
		}
	}
	length=m_key.GetLength();
	if(length>9){                               //数据不能超范围
		m_key.Delete(length-1);
		UpdateData(false);
	}
}
