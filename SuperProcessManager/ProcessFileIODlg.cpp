// ProcessFileIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProcessManager.h"
#include "ProcessFileIODlg.h"
#include "DataManager.h"
#include "MainFrm.h"
#include "HookManager.h"
#include "MonitorNetworkIoSettingDlg.h"
#include "MonitorFileDlg.h"
#include "ProcessTool/ProcessTool.h"
#include "CMListCtrlFilterDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessFileIODlg dialog
extern CDataManager				theDatamgr;
extern CSuperProcessManagerApp	theApp;
extern CHookManager				theHookManager;

#define WM_READ_FILE_IO_DATA		WM_USER+0x101


CProcessFileIODlg::CProcessFileIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessFileIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessFileIODlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcessFileIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessFileIODlg)
	DDX_Control(pDX, IDC_LIST, m_DataList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessFileIODlg, CDialog)
	//{{AFX_MSG_MAP(CProcessFileIODlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_READ_FILE_IO_DATA,OnReadFileIoData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessFileIODlg message handlers


static enum 
{
	FILEIO_ID,
	FILEIO_API,
	FILEIO_PARAM,
	FILEIO_DESCRIBE,
	FILEIO_PROCESS_FILENAME,
	FILEIO_PROCESS_ID,
	FILEIO_THREAD_ID,
	FILEIO_RESULT,
	FILEIO_TIME,
	FILEIO_LASTERROR,
	FILEIO_PROCESS_PATH
};



static COLUMN_DATA g_DataListCol[]=
{
	{FILEIO_ID,TEXT("ID"),LVCFMT_LEFT,FALSE,100,TYPE_INTGER},
	{FILEIO_API,TEXT("API"),LVCFMT_LEFT,FALSE,100,TYPE_STRING},
	{FILEIO_PARAM,TEXT("参数"),LVCFMT_LEFT,FALSE,100,TYPE_STRING},
	{FILEIO_DESCRIBE,TEXT("说明"),LVCFMT_LEFT,FALSE,200,TYPE_STRING},
	{FILEIO_PROCESS_FILENAME,TEXT("进程名"),LVCFMT_LEFT,TRUE,100,TYPE_STRING},
	{FILEIO_PROCESS_ID,TEXT("进程ID"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{FILEIO_THREAD_ID,TEXT("线程ID/入口"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{FILEIO_RESULT,TEXT("返回值"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{FILEIO_TIME,TEXT("时间"),LVCFMT_LEFT,TRUE,130,TYPE_DATE},
	{FILEIO_LASTERROR,TEXT("LastError"),LVCFMT_LEFT,TRUE,200,TYPE_STRING},
	{FILEIO_PROCESS_PATH,TEXT("进程路径"),LVCFMT_LEFT,TRUE,150,TYPE_STRING},
};


BOOL CProcessFileIODlg::InitDataList()
{
	m_DataList.SetExtendedStyle(m_DataList.GetExtendedStyle()|
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|
		LVS_EX_CHECKBOXES|
		LVS_EX_DOUBLEBUFFER|
		LVS_EX_HEADERDRAGDROP);
	
	m_DataIamge.Create(GetSystemMetrics(SM_CXSMICON),
					   GetSystemMetrics(SM_CYSMICON),
					   ILC_COLOR8|TRUE,200,300);
	m_DataList.SetImageList(&m_DataIamge,LVSIL_SMALL);
	
	m_DataList.InsertColumn(&g_DataListCol[0],sizeof(g_DataListCol)/sizeof(COLUMN_DATA));
	return TRUE;
}

BOOL CProcessFileIODlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitDataList();
	

	//theDatamgr.GetReserveInfo()->hNetworkIoDlg = m_hWnd;
	//theDatamgr.GetReserveInfo()->uNetworkIoDlgMsg = WM_READ_FILE_IO_DATA;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CProcessFileIODlg::ProcessIsMonitoring(DWORD dwPID)
{

	return FALSE;
}


BOOL CProcessFileIODlg::MonitorTargetProcess(DWORD dwPID)
{

	return TRUE;
}


void CProcessFileIODlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_DataList.GetSafeHwnd())
		m_DataList.MoveWindow(0,0,cx,cy);
}

LRESULT CProcessFileIODlg::OnReadFileIoData(WPARAM wParam,LPARAM lParam)
{


	return TRUE;
}

