// MsgBox.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "MsgBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW

#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgBox)
	m_Title = _T("！");
	m_Text = _T(" ");
	//}}AFX_DATA_INIT
}


void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgBox)
	DDX_Control(pDX, IDC_STATIC_ICON1, m_Icons1);
	DDX_Control(pDX, IDC_STATIC_ICON2, m_Icons2);
	DDX_Control(pDX, IDC_STATIC_ICON3, m_Icons3);
	DDX_Control(pDX, IDC_STATIC_ICON4, m_Icons4);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_Title);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgBox, CDialog)
	//{{AFX_MSG_MAP(CMsgBox)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio10)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBox message handlers

BOOL CMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_Icons1.SetIcon(LoadIcon(NULL, IDI_WARNING));
	m_Icons2.SetIcon(LoadIcon(NULL, IDI_ERROR));
	m_Icons3.SetIcon(LoadIcon(NULL, IDI_INFORMATION));
	m_Icons4.SetIcon(LoadIcon(NULL, IDI_QUESTION));
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO12))->SetCheck(TRUE);
	m_msgButton = MB_OK;
	m_msgIcon = MB_ICONWARNING;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgBox::OnShow()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MessageBox(m_Text.GetBuffer(0), m_Title.GetBuffer(0), m_msgIcon | m_msgButton);
}

void CMsgBox::OnRadio12()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_OK;
}

void CMsgBox::OnRadio11()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_CANCELTRYCONTINUE;
}

void CMsgBox::OnRadio10()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_HELP;
}

void CMsgBox::OnRadio7()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_YESNO;
}

void CMsgBox::OnRadio13()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_YESNOCANCEL;
}

void CMsgBox::OnRadio5()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_OKCANCEL;
}

void CMsgBox::OnRadio8()
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_ABORTRETRYIGNORE;
}

void CMsgBox::OnRadio1()
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONWARNING;
}

void CMsgBox::OnRadio2()
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONERROR;
}

void CMsgBox::OnRadio3()
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONINFORMATION;
}

void CMsgBox::OnRadio4()
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONQUESTION;
}

HBRUSH CMsgBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_TITLE | IDC_EDIT_TEXT) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(255, 0, 0);
		pDC->SetTextColor(clr);   //设置绿色的文本
		clr = RGB(0, 255, 0);
		pDC->SetBkColor(clr);     //设置黑色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}
