// SerAttribute.cpp : implementation file
//

#include "pch.h"
#include "GfiRatServer.h"
#include "SerAttribute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerAttribute dialog


CSerAttribute::CSerAttribute(CWnd* pParent, CListCtrl* pService)
	: CDialog(CSerAttribute::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerAttribute)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_list_service = pService;
}


void CSerAttribute::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerAttribute)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSerAttribute, CDialog)
	//{{AFX_MSG_MAP(CSerAttribute)
	ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerAttribute message handlers

BOOL CSerAttribute::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int nItem = m_list_service->GetNextItem(-1, LVNI_SELECTED);
	CString strText = m_list_service->GetItemText(nItem, 0);
	strText += " µÄÊôÐÔ";
	SetWindowText(strText);

	char szText[1024];
	m_list_service->GetItemText(nItem, 2, szText, 1024);
	SetDlgItemText(IDC_EDT_SERVICENAME, szText);
	m_list_service->GetItemText(nItem, 0, szText, 1024);
	SetDlgItemText(IDC_EDT_DISPLAYNAME, szText);
	m_list_service->GetItemText(nItem, 1, szText, 1024);
	SetDlgItemText(IDC_EDT_DESCRIPTION, szText);
	m_list_service->GetItemText(nItem, 5, szText, 1024);
	SetDlgItemText(IDC_EDT_BINARYPATH, szText);
	m_list_service->GetItemText(nItem, 3, szText, 1024);
	SetDlgItemText(IDC_EDT_STARTTYPE, szText);
	m_list_service->GetItemText(nItem, 4, szText, 1024);
	if (strcmp(szText, "") == 0)
		SetDlgItemText(IDC_EDT_STATE, "Í£Ö¹");
	else
		SetDlgItemText(IDC_EDT_STATE, szText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSerAttribute::OnBtnPrev()
{
	// TODO: Add your control notification handler code here
	int nItem = m_list_service->GetNextItem(-1, LVNI_SELECTED);
	if (nItem == 0)
	{
		nItem = m_list_service->GetItemCount();
	}
	m_list_service->SetItemState(--nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_list_service->EnsureVisible(nItem, TRUE);
	OnInitDialog();
}

void CSerAttribute::OnBtnNext()
{
	// TODO: Add your control notification handler code here
	int nItem = m_list_service->GetNextItem(-1, LVNI_SELECTED);
	if (nItem == m_list_service->GetItemCount() - 1)
	{
		nItem = -1;
	}
	m_list_service->SetItemState(++nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_list_service->EnsureVisible(nItem, TRUE);
	OnInitDialog();
}

BOOL CSerAttribute::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_EDT_SERVICENAME)->m_hWnd)
			return TRUE;
		if (pMsg->hwnd == GetDlgItem(IDC_EDT_DISPLAYNAME)->m_hWnd)
			return TRUE;
		if (pMsg->hwnd == GetDlgItem(IDC_EDT_DESCRIPTION)->m_hWnd)
			return TRUE;
		if (pMsg->hwnd == GetDlgItem(IDC_EDT_BINARYPATH)->m_hWnd)
			return TRUE;
		if (pMsg->hwnd == GetDlgItem(IDC_EDT_STARTTYPE)->m_hWnd)
			return TRUE;
		if (pMsg->hwnd == GetDlgItem(IDC_EDT_STATE)->m_hWnd)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
