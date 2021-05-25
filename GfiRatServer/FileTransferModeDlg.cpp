// FileTransferModeDlg.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "FileTransferModeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileTransferModeDlg dialog


CFileTransferModeDlg::CFileTransferModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTransferModeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTransferModeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileTransferModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTransferModeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileTransferModeDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTransferModeDlg)

	ON_BN_CLICKED(IDC_OVERWRITE, &CFileTransferModeDlg::OnBnClickedOverwrite)
	ON_BN_CLICKED(IDC_OVERWRITE_ALL, &CFileTransferModeDlg::OnBnClickedOverwriteAll)
	ON_BN_CLICKED(IDC_ADDITION, &CFileTransferModeDlg::OnBnClickedAddition)
	ON_BN_CLICKED(IDC_ADDITION_ALL, &CFileTransferModeDlg::OnBnClickedAdditionAll)
	ON_BN_CLICKED(IDC_JUMP, &CFileTransferModeDlg::OnBnClickedJump)
	ON_BN_CLICKED(IDC_JUMP_ALL, &CFileTransferModeDlg::OnBnClickedJumpAll)
	ON_BN_CLICKED(IDC_CANCEL, &CFileTransferModeDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTransferModeDlg message handlers


void CFileTransferModeDlg::OnEndDialog(UINT id)
{
	// TODO: Add your control notification handler code here
	EndDialog(id);
}

BOOL CFileTransferModeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString	str;
	str.Format(_T("此文件夹已包含一个名为“%s”的文件"), m_strFileName);

	for (int i = 0; i < str.GetLength(); i += 120)
	{
		str.Insert(i, "\n");
		i += 1;
	}

	SetDlgItemText(IDC_TIPS, str);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}








void CFileTransferModeDlg::OnBnClickedOverwrite()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_OVERWRITE);
}


void CFileTransferModeDlg::OnBnClickedOverwriteAll()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_OVERWRITE_ALL);
}


void CFileTransferModeDlg::OnBnClickedAddition()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_ADDITION);
}


void CFileTransferModeDlg::OnBnClickedAdditionAll()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_ADDITION_ALL);
}


void CFileTransferModeDlg::OnBnClickedJump()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_JUMP);
}


void CFileTransferModeDlg::OnBnClickedJumpAll()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_JUMP_ALL);
}


void CFileTransferModeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_CANCEL);
}
