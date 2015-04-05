// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SuperProcessManager.h"
#include "MainFrm.h"
#include <strsafe.h>
#include "MonitorNetworkIoSettingDlg.h"
#include "MonitorFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame


HWND g_hMainFrmWnd = NULL;

#define WM_NC_MSG			WM_USER+0x100


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(IDM_PUBLIC_TEXT_BAR, OnUpdatePublicTextBar)
	ON_COMMAND(IDM_PUBLIC_TEXT_BAR, OnPublicTextBar)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_TOPMOST, OnTopmost)
	ON_COMMAND(IDM_SHOW_WINDOW, OnShowWindow)
	ON_COMMAND(IDM_SETTING, OnSetting)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_UPDATE, OnUpdate)
	ON_COMMAND(IDM_MONITOR_NETWORKIO_SETTING, OnMonitorNetworkioSetting)
	ON_COMMAND(IDM_MONITOR_FILE, OnMonitorFile)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_COMBO_ADDRESS, OnUpdateComboAddress)
	ON_MESSAGE(WM_NC_MSG,OnNcMsg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	IDS_HANDLE_NUM,
	IDS_PROCESS_NUM,
	IDS_THREAD_NUM,
	IDS_CPU_USAGE,
	IDS_PAGEFILE_USAGE,
	IDS_CPU_NUM
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{

}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	
	m_PublicTextBar.Create(this,IDD_TEXTBAR_DLG,WS_CHILD | WS_VISIBLE | CBRS_TOP |CBRS_GRIPPER|CBRS_SIZE_DYNAMIC,
		IDC_PUBLICTEXTBAR);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	m_PublicTextBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_PublicTextBar);
	
	RecalcLayout();
	CRect rect;
	m_wndToolBar.GetWindowRect(rect);
	rect.OffsetRect(1, 0);//偏移一个位置
	DockControlBar(&m_PublicTextBar, AFX_IDW_DOCKBAR_TOP,rect);///也停靠在底部


	ZeroMemory(&m_nc,sizeof(NOTIFYICONDATA));

	m_nc.cbSize=sizeof(NOTIFYICONDATA);
	m_nc.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nc.hWnd=m_hWnd;
	CString szTitle;
	szTitle.LoadString(AFX_IDS_APP_TITLE);
	StringCbCopy(m_nc.szTip,sizeof(m_nc.szTip),szTitle);
	m_nc.uCallbackMessage=WM_NC_MSG;
	m_nc.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&m_nc);

	m_NotifyIconMenu.LoadMenu(IDR_NOTIFYICON_MENU);

	return 0;
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style &=~FWS_ADDTOTITLE;  
    //cs.lpszName = TEXT("超级进程管理器 V1.0 R3版本"); 
	//cs.lpszClass = TEXT("321");

	WNDCLASS wndclass;
	::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndclass);
	wndclass.lpszClassName = TEXT("超级进程管理器 2012-11-30 V1.0");
	wndclass.hIcon = LoadIcon(AfxGetInstanceHandle(), (LPCTSTR)IDR_MAINFRAME);
	AfxRegisterClass(&wndclass);
	cs.lpszClass = wndclass.lpszClassName;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::SetStatusBarText(DWORD nIndex,LPCTSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	TCHAR szParamText[4096]={NULL};
	StringCbVPrintf(szParamText,sizeof(szParamText),szFormat,vagrc);
	va_end(vagrc);
	m_wndStatusBar.SetPaneText(nIndex,szParamText);
}

void CMainFrame::OnUpdateComboAddress(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdatePublicTextBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_PublicTextBar.IsWindowVisible());
}

void CMainFrame::OnPublicTextBar() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_PublicTextBar,m_PublicTextBar.IsWindowVisible()?FALSE:TRUE,FALSE);
	RecalcLayout();
}

void CMainFrame::OnEditCopy() 
{
	// TODO: Add your command handler code here

	::OpenClipboard(m_hWnd);

	CString szText;
	m_PublicTextBar.GetCurrentText(szText);

	INT nSize = szText.GetLength()*sizeof(TCHAR)+sizeof(TCHAR);

	HGLOBAL hGlobal= GlobalAlloc(GMEM_MOVEABLE,nSize);
	//在堆上分配可移动的内存块，程序返回一个内存句柄
	TCHAR * szGlobal;
	szGlobal=(TCHAR*)GlobalLock(hGlobal);
	StringCbCopy((TCHAR*)szGlobal,nSize,(LPCTSTR)szText);
	

	GlobalUnlock(hGlobal);
	::SetClipboardData(CF_UNICODETEXT,(HANDLE)hGlobal);
	::CloseClipboard();
}


void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CString szText;
	pCmdUI->Enable(m_PublicTextBar.GetCurrentText(szText));
}


LRESULT CMainFrame::OnNcMsg(WPARAM wParam,LPARAM lParam)
{

	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:
		{
			ShowWindow(SW_RESTORE); 
			::SetForegroundWindow(m_hWnd);
		}break;
	case WM_RBUTTONDOWN:
		{
			CMenu *pSub = m_NotifyIconMenu.GetSubMenu(0);
			POINT point;
			GetCursorPos(&point);
			
			pSub->CheckMenuItem(IDM_SHOW_WINDOW,MF_BYCOMMAND | IsWindowVisible()?MF_CHECKED:MF_UNCHECKED);
			pSub->CheckMenuItem(IDM_TOPMOST,MF_BYCOMMAND | (GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)?MF_CHECKED:MF_UNCHECKED);
			pSub->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTBUTTON,point.x,point.y,this);
		}break;

	}

	return TRUE;
}




void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
}



void CMainFrame::OnTopmost() 
{
	// TODO: Add your command handler code here
	if(GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
	{
		::SetWindowPos(m_hWnd,HWND_NOTOPMOST,NULL,NULL,NULL,NULL,SWP_NOMOVE|SWP_NOSIZE);
	}else
	{
		::SetWindowPos(m_hWnd,HWND_TOPMOST,NULL,NULL,NULL,NULL,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnShowWindow() 
{
	// TODO: Add your command handler code here
	if(IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_RESTORE); 
		::SetForegroundWindow(m_hWnd);
	}
}

void CMainFrame::OnSetting() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnExit() 
{
	// TODO: Add your command handler code here
	Shell_NotifyIcon(NIM_DELETE,&m_nc);
	CFrameWnd::OnClose();
}

void CMainFrame::OnAbout() 
{
	// TODO: Add your command handler code here
	((CSuperProcessManagerApp *)AfxGetApp())->OnAppAbout();
}

void CMainFrame::OnUpdate() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnMonitorNetworkioSetting() 
{
	// TODO: Add your command handler code here
	CMonitorNetworkIoSettingDlg MonitorNetworkIoSettingDlg;
	MonitorNetworkIoSettingDlg.DoModal();
}

void CMainFrame::OnMonitorFile() 
{
	// TODO: Add your command handler code here
	CMonitorFileDlg MonitorFileDlg;
	MonitorFileDlg.DoModal();
}
