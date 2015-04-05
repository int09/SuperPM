// SuperProcessManagerView.cpp : implementation of the CSuperProcessManagerView class
//

#include "stdafx.h"
#include "SuperProcessManager.h"

#include "SuperProcessManagerDoc.h"
#include "SuperProcessManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerView

#define			TAB_ID			65000


IMPLEMENT_DYNCREATE(CSuperProcessManagerView, CView)

BEGIN_MESSAGE_MAP(CSuperProcessManagerView, CView)
	//{{AFX_MSG_MAP(CSuperProcessManagerView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, TAB_ID, OnSelchangeTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerView construction/destruction

CSuperProcessManagerView::CSuperProcessManagerView()
{
	// TODO: add construction code here

}

CSuperProcessManagerView::~CSuperProcessManagerView()
{
}

BOOL CSuperProcessManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerView drawing

void CSuperProcessManagerView::OnDraw(CDC* pDC)
{
	CSuperProcessManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerView diagnostics

#ifdef _DEBUG
void CSuperProcessManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CSuperProcessManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSuperProcessManagerDoc* CSuperProcessManagerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSuperProcessManagerDoc)));
	return (CSuperProcessManagerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerView message handlers



typedef struct _TAB_COL
{
	INT nCol;
	LPCTSTR lpszText;
	CWnd * pDlg;
}TAB_COL,*LPTAB_COL;






int CSuperProcessManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	m_TabCtrl.Create(WS_VISIBLE|WS_CHILD|TCS_FIXEDWIDTH|TCS_BOTTOM,CRect(100,100,100,100),this,TAB_ID);

	m_TabFont.CreateFont(
		12,                       
		0,                       
		0,                        
		0,                       
		FW_NORMAL,              
		FALSE,                 
		FALSE,             
		0,               
		ANSI_CHARSET,        
		OUT_DEFAULT_PRECIS,    
		CLIP_DEFAULT_PRECIS,    
		DEFAULT_QUALITY,       
		DEFAULT_PITCH | FF_SWISS,
		TEXT("宋体")
		);
	m_TabCtrl.SetFont(&m_TabFont);
	
	m_ProcessManager.Create(IDD_PROCESS_MANAGER_DLG,&m_TabCtrl);
	m_ProcessNetworkIo.Create(IDD_PROCESS_NETWORKIO_DLG,&m_TabCtrl);
	m_ProcessRegIo.Create(IDD_PROCESS_REGIO_DLG,&m_TabCtrl);
	m_ProcesFileIo.Create(IDD_PROCESS_FILEIO_DLG,&m_TabCtrl);
	m_Runlog.Create(IDD_RUNLOG_DLG,&m_TabCtrl);

	TAB_COL TabCol[]=
	{
		{TAB_PROCESS_MANAGER,TEXT("进程管理"),&m_ProcessManager},
		{TAB_PROCESS_NETWORK_IO,TEXT("进程网络流量"),&m_ProcessNetworkIo},
		{TAB_PROCESS_FILE_IO,TEXT("进程文件读写"),&m_ProcesFileIo},
		{TAB_PROCESS_REG_IO,TEXT("进程注册表读写"),&m_ProcessRegIo},
		{TAB_RUN_LOG,TEXT("运行记录"),&m_Runlog}
	};

	for(INT i=0;i<sizeof(TabCol)/sizeof(TAB_COL);i++)
		m_TabCtrl.InsertItem(TCIF_TEXT|TCIF_PARAM,TabCol[i].nCol,TabCol[i].lpszText,NULL,(LPARAM)TabCol[i].pDlg);

	m_ProcessManager.ShowWindow(SW_SHOW);
	m_TabCtrl.SetCurSel(0);
	return 0;
}

void CSuperProcessManagerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	TCITEM TcItem;
	CRect rc;
	// TODO: Add your message handler code here
	if(m_TabCtrl.GetSafeHwnd())
	{
		m_TabCtrl.MoveWindow(0,0,cx,cy);
		INT nCount = m_TabCtrl.GetItemCount();
		
		for(INT i=0;i<nCount;i++)
		{
			ZeroMemory(&TcItem,sizeof(TCITEM));
			TcItem.mask = TCIF_PARAM;
			m_TabCtrl.GetItem(i,&TcItem);
			
			m_TabCtrl.GetItemRect(i,&rc);
			
			if(((CWnd *)TcItem.lParam)->GetSafeHwnd())
				((CWnd *)TcItem.lParam)->MoveWindow(0,0,cx-3,cy-28);
		}
	}
}

void CSuperProcessManagerView::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT nCount = m_TabCtrl.GetItemCount();
	INT nSel = m_TabCtrl.GetCurSel();

	TCITEM TcItem;
	for(INT i=0;i<nCount;i++)
	{
		ZeroMemory(&TcItem,sizeof(TCITEM));
		TcItem.mask = TCIF_PARAM;
		m_TabCtrl.GetItem(i,&TcItem);
		((CWnd *)TcItem.lParam)->ShowWindow(i == nSel?SW_SHOW:SW_HIDE);
	}
}

void CSuperProcessManagerView::SwitchWindow(INT uWindow)
{
	INT nCount = m_TabCtrl.GetItemCount();
	m_TabCtrl.SetCurSel(uWindow);
	INT nSel = uWindow;
	
	TCITEM TcItem;
	for(INT i=0;i<nCount;i++)
	{
		ZeroMemory(&TcItem,sizeof(TCITEM));
		TcItem.mask = TCIF_PARAM;
		m_TabCtrl.GetItem(i,&TcItem);
		((CWnd *)TcItem.lParam)->ShowWindow(i == nSel?SW_SHOW:SW_HIDE);
	}
}

void CSuperProcessManagerView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	INT nCount = m_TabCtrl.GetItemCount();
	TCITEM TcItem;
	for(INT i=0;i<nCount;i++)
	{
		ZeroMemory(&TcItem,sizeof(TCITEM));
		TcItem.mask = TCIF_PARAM;
		m_TabCtrl.GetItem(i,&TcItem);
		::SendMessage(((CWnd *)TcItem.lParam)->m_hWnd,WM_CLOSE,NULL,NULL);
	}
}
