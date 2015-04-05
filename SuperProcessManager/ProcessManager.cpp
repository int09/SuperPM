// ProcessManager.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProcessManager.h"
#include "ProcessManager.h"
#include "FileVersion.h"
#include "MainFrm.h"
#include "SuperProcessManagerDoc.h"
#include "SuperProcessManagerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessManager dialog

#define  MODULE_LIST_ID						65001
#define  THREAD_LIST_ID						65002
#define  HANDLE_LIST_ID						65003

#define  PROCESS_MANAGER_TIMER				0x100
#define  PROCESS_MANAGER_TIMER_UELAPSE		1000
#define  WM_PROCESS_LIST_RESIZING			WM_USER+0x200

enum
{
		PROCESS_NAME,
		PROCESS_ID,
		PROCESS_PARENT,
		PROCESS_USER,
		PROCESS_SESSIONID,
		PROCESS_CREATETIME,
		PROCESS_THREAD_COUNT,
		PROCESS_MEMEORY,
		PROCESS_VIRTUAL_MEMEORY,
		PROCESS_HANDLE_COUNT,
		PROCESS_CPU,
		PROCESS_PRIORITY,
		PROCESS_DESCRIBE,
		PROCESS_COMPANY,
		PROCESS_PATH,
		PROCESS_COMMAND,
};

static COLUMN_DATA g_ProcessCol[]=
{
	{PROCESS_NAME,TEXT("进程"),LVCFMT_LEFT,FALSE,120,TYPE_STRING},
	{PROCESS_ID,TEXT("进程ID"),LVCFMT_RIGHT,TRUE,60,TYPE_INTGER},
	{PROCESS_PARENT,TEXT("父进程ID"),LVCFMT_LEFT,TRUE,120,TYPE_INTGER},
	{PROCESS_USER,TEXT("用户名"),LVCFMT_LEFT,TRUE,110,TYPE_STRING},
	{PROCESS_SESSIONID,TEXT("会话ID"),LVCFMT_CENTER,TRUE,60,TYPE_INTGER},
	{PROCESS_CREATETIME,TEXT("启动时间"),LVCFMT_CENTER,TRUE,60,TYPE_DATE},
	{PROCESS_THREAD_COUNT,TEXT("线程数"),LVCFMT_CENTER,TRUE,55,TYPE_INTGER},
	{PROCESS_MEMEORY,TEXT("内存使用"),LVCFMT_LEFT,TRUE,60,TYPE_INTGER},
	{PROCESS_VIRTUAL_MEMEORY,TEXT("虚拟内存使用"),LVCFMT_LEFT,TRUE,60,TYPE_INTGER},
	{PROCESS_HANDLE_COUNT,TEXT("句柄数"),LVCFMT_CENTER,TRUE,60,TYPE_INTGER},
	{PROCESS_CPU,TEXT("CPU"),LVCFMT_LEFT,TRUE,50,TYPE_INTGER},
	{PROCESS_PRIORITY,TEXT("优先级"),LVCFMT_LEFT,TRUE,100,TYPE_STRING},
	{PROCESS_DESCRIBE,TEXT("描述"),LVCFMT_LEFT,TRUE,50,TYPE_STRING},
	{PROCESS_COMPANY,TEXT("公司"),LVCFMT_LEFT,TRUE,50,TYPE_STRING},
	{PROCESS_PATH,TEXT("映像路径"),LVCFMT_LEFT,TRUE,120,TYPE_STRING},
	{PROCESS_COMMAND,TEXT("命令行"),LVCFMT_LEFT,TRUE,120,TYPE_STRING},
};

enum
{
		MODULE_NAME,
		MODULE_BASE,
		MODULE_SIZE,
		MODULE_PATH,
		MODULE_COMPANY,
		MODULE_DESCRIBE,
		MODULE_VERSION,
		MODULE_CRETION_TIME
};


static COLUMN_DATA g_ModuleCol[]=
{
	{MODULE_NAME,TEXT("映像名"),LVCFMT_LEFT,FALSE,120,TYPE_STRING},
	{MODULE_BASE,TEXT("映像基址"),LVCFMT_CENTER,TRUE,80,TYPE_STRING},
	{MODULE_SIZE,TEXT("映像大小"),LVCFMT_RIGHT,TRUE,80,TYPE_INTGER},
	{MODULE_PATH,TEXT("映像路径"),LVCFMT_LEFT,FALSE,300,TYPE_STRING},
	{MODULE_COMPANY,TEXT("描述"),LVCFMT_LEFT,FALSE,180,TYPE_STRING},
	{MODULE_DESCRIBE,TEXT("公司"),LVCFMT_LEFT,FALSE,180,TYPE_STRING},
	{MODULE_VERSION,TEXT("版本"),LVCFMT_LEFT,TRUE,80,TYPE_STRING},
	{MODULE_CRETION_TIME,TEXT("创建时间"),LVCFMT_LEFT,TRUE,80,TYPE_DATE}
};

enum
{
		THREAD_ID,
		THREAD_ENTRY_ADDRESS,
		THREAD_MODULE_PATH,
		THREAD_PRORITY,
		THREAD_DESKTOP,
		THREAD_CURRENT_STATE,
		THREAD_SWITCH_COUNT,
		THREAD_COMPANY,
		THREAD_DESCRIBE,
		THREAD_VERSION,
		THREAD_CRETION_TIME
};

static COLUMN_DATA g_ThreadCol[]=
{
	{THREAD_ID,TEXT("线程ID"),LVCFMT_LEFT,FALSE,160,TYPE_INTGER},
	{THREAD_ENTRY_ADDRESS,TEXT("入口"),LVCFMT_CENTER,TRUE,80,TYPE_STRING},
	{THREAD_PRORITY,TEXT("优先级"),LVCFMT_LEFT,FALSE,80,TYPE_STRING},
	{THREAD_DESKTOP,TEXT("所属桌面"),LVCFMT_RIGHT,TRUE,80,TYPE_STRING},
	{THREAD_CURRENT_STATE,TEXT("状态"),LVCFMT_CENTER,TRUE,80,TYPE_STRING},
	{THREAD_SWITCH_COUNT,TEXT("切换次数"),LVCFMT_RIGHT,TRUE,80,TYPE_INTGER},
	{THREAD_MODULE_PATH,TEXT("所在模块"),LVCFMT_LEFT,FALSE,300,TYPE_STRING},
	{THREAD_COMPANY,TEXT("描述"),LVCFMT_LEFT,FALSE,120,TYPE_STRING},
	{THREAD_DESCRIBE,TEXT("公司"),LVCFMT_LEFT,FALSE,120,TYPE_STRING},
	{THREAD_VERSION,TEXT("版本"),LVCFMT_LEFT,FALSE,80,TYPE_STRING},
	{THREAD_CRETION_TIME,TEXT("创建时间"),LVCFMT_LEFT,FALSE,120,TYPE_DATE}
};

enum
{
		HANDLE_TYPE,
		HANDLE_TYPE_VALUE,
		HANDLE_NAME,
		HANDLE_VALUE,
		HANDLE_INHERIT,
		HANDLE_CLOSE,
//		HANDLE_ACCESS,
		HANDLE_COUNT,
//		HANDLE_SHARE_FLAGS
};

static COLUMN_DATA g_HandleCol[]=
{
	{HANDLE_TYPE,TEXT("句柄类型"),LVCFMT_LEFT,FALSE,100,TYPE_STRING},
	{HANDLE_TYPE_VALUE,TEXT("句柄类型"),LVCFMT_LEFT,TRUE,60,TYPE_INTGER},
	{HANDLE_NAME,TEXT("名称"),LVCFMT_LEFT,FALSE,500,TYPE_STRING},
	{HANDLE_VALUE,TEXT("句柄值"),LVCFMT_LEFT,FALSE,100,TYPE_STRING},
	{HANDLE_INHERIT,TEXT("可否继承"),LVCFMT_LEFT,TRUE,10,TYPE_STRING},
	{HANDLE_CLOSE,TEXT("可否关闭"),LVCFMT_LEFT,TRUE,10,TYPE_STRING},
//	{HANDLE_ACCESS,TEXT("访问"),LVCFMT_LEFT,TRUE,100,TYPE_STRING},
	{HANDLE_COUNT,TEXT("计数"),LVCFMT_CENTER,TRUE,100,TYPE_INTGER},
//	{HANDLE_SHARE_FLAGS,TEXT("共享标志"),LVCFMT_LEFT,80},
};




CProcessManager::CProcessManager(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nProcessListButtomCoor = 0;
	m_nSubList = SUB_LIST_MODULE;
	m_nCurrentSelectItem = -1;
	EnableDebugPrivilege(SE_CHANGE_NOTIFY_NAME);
	EnableDebugPrivilege(SE_SECURITY_NAME);
	EnableDebugPrivilege(SE_BACKUP_NAME);
	EnableDebugPrivilege(SE_DEBUG_NAME);
	EnableDebugPrivilege(SE_IMPERSONATE_NAME);
	EnableDebugPrivilege(SE_CREATE_GLOBAL_NAME);
}


void CProcessManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessManager)
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ProcessList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessManager, CDialog)
	//{{AFX_MSG_MAP(CProcessManager)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS_LIST, OnRclickProcessList)
	ON_NOTIFY(NM_CLICK, IDC_PROCESS_LIST, OnClickProcessList)
	ON_COMMAND(IDM_PROCESS_HANDLE, OnProcessHandle)
	ON_COMMAND(IDM_PROCESS_MODULE, OnProcessModule)
	ON_COMMAND(IDM_PROCESS_THREAD, OnProcessThread)
	ON_COMMAND(IDM_SUSPPED_PROCESS, OnSusppedProcess)
	ON_COMMAND(IDM_TERMINATOR_PROCESS_CUREENT, OnTerminatorProcessCureent)
	ON_COMMAND(IDM_TERMINATOR_PROCESS_CHECKBOX, OnTerminatorProcessCheckbox)
	ON_COMMAND(IDM_TERMINATOR_PROCESS_TREE, OnTerminatorProcessTree)
	ON_COMMAND(IDM_SET_PRIORITY_REALTIME, OnSetPriorityRealtime)
	ON_COMMAND(IDM_SET_PRIORITY_NORMAL, OnSetPriorityNormal)
	ON_COMMAND(IDM_SET_PRIORITY_IDEL, OnSetPriorityIdel)
	ON_COMMAND(IDM_SET_PRIORITY_HEIGHT, OnSetPriorityHeight)
	ON_COMMAND(IDM_SET_PRIORITY_BELOW_NORMAL, OnSetPriorityBelowNormal)
	ON_COMMAND(IDM_SET_PRIORITY_ABOVE_HEIGHT, OnSetPriorityAboveHeight)
	ON_COMMAND(IDM_DEBUG_PROCESS, OnDebugProcess)
	ON_COMMAND(IDM_ATTRIBUTE_FILE, OnAttributeFile)
	ON_COMMAND(IDM_POSITIONING_FILE, OnPositioningFile)
	ON_WM_CREATE()
	ON_COMMAND(IDM_BAIDU_SEARCH, OnBaiduSearch)
	ON_COMMAND(IDM_GOOGLE_SEARCH, OnGoogleSearch)
	ON_COMMAND(IDM_ONLINE_VIRUS_SCAN, OnOnlineVirusScan)
	ON_COMMAND(IDM_MONITOR_ALL, OnMonitorAll)
	ON_COMMAND(IDM_MONITOR_FILEIO, OnMonitorFileio)
	ON_COMMAND(IDM_MONITOR_NETWOKIO, OnMonitorNetwokio)
	ON_COMMAND(IDM_MONITOR_REGIO, OnMonitorRegio)
	ON_NOTIFY(NM_DBLCLK, IDC_PROCESS_LIST, OnDblclkProcessList)
	ON_COMMAND(IDM_INJECT_PROCESS, OnInjectProcess)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, MODULE_LIST_ID, OnClickModuleList)
	ON_NOTIFY(NM_CLICK, THREAD_LIST_ID, OnClickThreadList)
	ON_NOTIFY(NM_CLICK, HANDLE_LIST_ID, OnClickHandleList)
	ON_MESSAGE(WM_PROCESS_LIST_RESIZING,OnProcessListResizeing)
	
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_PROCESS_LIST,OnDrawColorForProcessList)
	ON_NOTIFY( NM_CUSTOMDRAW, MODULE_LIST_ID, OnDrawColorForModuleList)
	ON_NOTIFY( NM_CUSTOMDRAW, THREAD_LIST_ID, OnDrawColorForThreadList)
	ON_NOTIFY( NM_CUSTOMDRAW, HANDLE_LIST_ID, OnDrawColorForHandleList)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CProcessManager message handlers

BOOL CProcessManager::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_PerformanceTool.Create();
	
	// TODO: Add extra initialization here
	InitProcessList();
	ShowProcessInfo();

	InitModuleList();
	InitThreadList();
	InitHandleList();
	
	SwitchSubWindow();
	

	LRESULT lResult = FALSE;
	m_ProcessList.SetItemState(m_ProcessList.GetItemCount()-1,LVIS_SELECTED,LVIS_SELECTED);
	OnClickProcessList(NULL,&lResult);

	SetTimer(PROCESS_MANAGER_TIMER,PROCESS_MANAGER_TIMER_UELAPSE,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessManager::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	CRect SubCtrlRc;

	if(!m_nProcessListButtomCoor) 
	{
		if(m_ProcessList.GetSafeHwnd()) m_ProcessList.MoveWindow(0,0,cx,cy * 0.63);
	}
	else
	{
		if(m_ProcessList.GetSafeHwnd()) m_ProcessList.MoveWindow(0,0,cx,m_nProcessListButtomCoor);
	}

	SubCtrlRc.top = m_nProcessListButtomCoor?m_nProcessListButtomCoor:cy * 0.63;
	SubCtrlRc.top+=5;
	SubCtrlRc.bottom = cy;
	SubCtrlRc.left = 0;
	SubCtrlRc.right = cx;

	if(m_ModuleList.GetSafeHwnd()) m_ModuleList.MoveWindow(SubCtrlRc);
	if(m_ThreadList.GetSafeHwnd()) m_ThreadList.MoveWindow(SubCtrlRc);
	if(m_HandleList.GetSafeHwnd()) m_HandleList.MoveWindow(SubCtrlRc);
}

LRESULT CProcessManager::OnProcessListResizeing(WPARAM wParam,LPARAM lParam)
{
	m_nProcessListButtomCoor = HIWORD(lParam);
	CRect SubCtrlRc;
	GetClientRect(&SubCtrlRc);
	SubCtrlRc.top = m_nProcessListButtomCoor + 9;
	SubCtrlRc.left = 0;
	if(m_ModuleList.GetSafeHwnd()) m_ModuleList.MoveWindow(SubCtrlRc);
	if(m_ThreadList.GetSafeHwnd()) m_ThreadList.MoveWindow(SubCtrlRc);
	if(m_HandleList.GetSafeHwnd()) m_HandleList.MoveWindow(SubCtrlRc);
	return 0;
}


int CProcessManager::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ProcessList.SetResizingInfo(50,GetSystemMetrics(SM_CYSCREEN),m_hWnd,WM_PROCESS_LIST_RESIZING);
	return 0;
}


BOOL CProcessManager::InitProcessList()
{

	if(!m_ProcessTool.Create())
		return FALSE;
	
	m_ProcessList.SetExtendedStyle(m_ProcessList.GetExtendedStyle()|
												LVS_EX_FULLROWSELECT|
												LVS_EX_GRIDLINES|
												LVS_EX_CHECKBOXES|
												LVS_EX_SUBITEMIMAGES|
												LVS_EX_DOUBLEBUFFER|
												LVS_EX_HEADERDRAGDROP);
	
	m_ProcessList.InsertColumn(&g_ProcessCol[0],sizeof(g_ProcessCol)/sizeof(COLUMN_DATA));

	m_ProcessListImg.Create(GetSystemMetrics(SM_CXSMICON),
							GetSystemMetrics(SM_CYSMICON),
							ILC_COLOR8|TRUE,200,300);

	m_ProcessList.SetImageList(&m_ProcessListImg,LVSIL_SMALL);

	m_ProcessMenu.LoadMenu(IDR_PROCESS_LIST_MENU);
	return TRUE;
}


BOOL CProcessManager::InitModuleList()
{
	m_ModuleList.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0), this,MODULE_LIST_ID);

	m_ModuleList.SetExtendedStyle(m_ModuleList.GetExtendedStyle()|
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|
		LVS_EX_CHECKBOXES|
		LVS_EX_SUBITEMIMAGES|
		LVS_EX_DOUBLEBUFFER|
		LVS_EX_HEADERDRAGDROP);
	
	m_ModuleList.InsertColumn(&g_ModuleCol[0],sizeof(g_ModuleCol)/sizeof(COLUMN_DATA));
	
	m_ImageModule.Create(GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		ILC_COLOR8|TRUE,200,300);
	
	m_ModuleList.SetImageList(&m_ImageModule,LVSIL_SMALL);

	return TRUE;
}

BOOL CProcessManager::InitThreadList()
{
	if(!m_ThreadTool.Create())
		return FALSE;

	m_ThreadList.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0), this,THREAD_LIST_ID);
	
	m_ThreadList.SetExtendedStyle(m_ThreadList.GetExtendedStyle()|
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|
		LVS_EX_CHECKBOXES|
		LVS_EX_SUBITEMIMAGES|
		LVS_EX_DOUBLEBUFFER|
		LVS_EX_HEADERDRAGDROP);
	
	m_ThreadList.InsertColumn(&g_ThreadCol[0],sizeof(g_ThreadCol)/sizeof(COLUMN_DATA));
	
	m_ImageThread.Create(GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		ILC_COLOR8|TRUE,200,300);
	
	m_ThreadList.SetImageList(&m_ImageThread,LVSIL_SMALL);
	
	return TRUE;
}

BOOL CProcessManager::InitHandleList()
{	
	if(!m_HandleTool.Create())
		return FALSE;

	m_HandleList.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,CRect(0,0,0,0), this,HANDLE_LIST_ID);
	
	m_HandleList.SetExtendedStyle(m_HandleList.GetExtendedStyle()|
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|
		LVS_EX_CHECKBOXES|
		LVS_EX_DOUBLEBUFFER|
		LVS_EX_HEADERDRAGDROP);
	
	m_HandleList.InsertColumn(&g_HandleCol[0],sizeof(g_HandleCol)/sizeof(COLUMN_DATA));
	return TRUE;
}


void CProcessManager::OnDrawColorForProcessList(NMHDR *pNmHdr, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNmHdr );
    *pResult = CDRF_DODEFAULT;
	
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		int  nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		pLVCD->clrTextBk = (nItem % 2)?RGB(228,232,239):RGB(255,255,255);

		PROCESS_SUSPEND::const_iterator iter;
		iter = m_ProcessSuspendInfo.find(m_ProcessList.GetItemData(nItem));
		
		if(iter != m_ProcessSuspendInfo.end())
			pLVCD->clrText= RGB(128,128,128);


        *pResult = CDRF_DODEFAULT;
	}
}

void CProcessManager::OnDrawColorForModuleList(NMHDR *pNmHdr, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNmHdr );
    *pResult = CDRF_DODEFAULT;
	
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		int  nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		pLVCD->clrTextBk = (nItem % 2)?RGB(228,232,239):RGB(255,255,255);;
        *pResult = CDRF_DODEFAULT;
	}
}

void CProcessManager::OnDrawColorForThreadList(NMHDR *pNmHdr, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNmHdr );
    *pResult = CDRF_DODEFAULT;
	
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		int  nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		pLVCD->clrTextBk = (nItem % 2)?RGB(228,232,239):RGB(255,255,255);;
        *pResult = CDRF_DODEFAULT;
	}
}

void CProcessManager::OnDrawColorForHandleList(NMHDR *pNmHdr, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNmHdr );
    *pResult = CDRF_DODEFAULT;
	
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		int  nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		pLVCD->clrTextBk = (nItem % 2)?RGB(228,232,239):RGB(255,255,255);;
        *pResult = CDRF_DODEFAULT;
	}
}


BOOL CProcessManager::ProcessListDeleteItem(INT nItem)
{
	DWORD dwData = m_ProcessList.GetItemData(nItem);
	m_ProcessSuspendInfo.erase(dwData);
	m_ProcessList.DeleteItem(nItem);
	return TRUE;
}


BOOL CProcessManager::ShowProcessInfo()
{
	SHFILEINFO shfi;
	static PSYSTEM_PROCESS_INFORMATION Offset,lpTempOffset;
	static PROCESS_EXTEND_INFO ProExtendInfo; 
	if(!m_ProcessTool.CreateSnapshotProcess() || !m_ProcessTool.ProcessFirst(&Offset))
		return FALSE;

	//采集所有进程CPU时间总和
	lpTempOffset = Offset;
	m_ProcessTool.SetCurrentTotalProcessCpuTime(Offset);

	do 
	{
		//更新进程
		if(PROCESS_NEW == m_ProcessTool.CheckProcess(Offset))
		{
			m_ProcessTool.GetExtendInfo(Offset,&ProExtendInfo);
			INT nItem = m_ProcessList.GetItemCount();
			SHGetFileInfo(ProExtendInfo.szPath,FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(shfi),SHGFI_SMALLICON|SHGFI_ICON |SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME);
			m_ProcessList.InsertItem(nItem,NULL,m_ProcessListImg.Add(shfi.hIcon));
			m_ProcessList.SetItemData(nItem,Offset->UniqueProcessId);
			m_ProcessList.SetItemText(nItem,PROCESS_NAME,Offset->ImageName.Buffer);
			m_ProcessList.SetItemInt(nItem,PROCESS_ID,Offset->UniqueProcessId);
			m_ProcessList.SetItemInt(nItem,PROCESS_SESSIONID,Offset->SessionId);
			m_ProcessList.SetItemInt(nItem,PROCESS_THREAD_COUNT,Offset->NumberOfThreads);
			m_ProcessList.SetItemFormatText(nItem,PROCESS_MEMEORY,TEXT("%d K"),Offset->WorkingSetSize/1024);
			m_ProcessList.SetItemText(nItem,PROCESS_CREATETIME,ProExtendInfo.szTime);
			m_ProcessList.SetItemFormatText(nItem,PROCESS_VIRTUAL_MEMEORY,TEXT("%d K"),ProExtendInfo.dwVirtualMemeorySize/1024);
			m_ProcessList.SetItemInt(nItem,PROCESS_HANDLE_COUNT,Offset->HandleCount);
			m_ProcessList.SetItemText(nItem,PROCESS_PATH,ProExtendInfo.szPath);
			m_ProcessList.SetItemText(nItem,PROCESS_USER,ProExtendInfo.szProcessUserName);
			m_ProcessList.SetItemFormatText(nItem,PROCESS_PARENT,_tcslen(ProExtendInfo.szParentFileName)?TEXT("%d(%s)"):TEXT("%d"),Offset->InheritedFromUniqueProcessId,ProExtendInfo.szParentFileName);
			m_ProcessList.SetItemText(nItem,PROCESS_PRIORITY,ProExtendInfo.lpszProcessPrority);
			m_ProcessList.SetItemText(nItem,PROCESS_COMMAND,ProExtendInfo.szCommandLine);
			m_ProcessList.SetItemText(nItem,PROCESS_CPU,TEXT("0"));
			CFileVersion FileVersion;
			if(_tcslen(ProExtendInfo.szPath) && FileVersion.Open(ProExtendInfo.szPath))
			{
				m_ProcessList.SetItemText(nItem,PROCESS_COMPANY,FileVersion.GetCompanyName());
				m_ProcessList.SetItemText(nItem,PROCESS_DESCRIBE,FileVersion.GetFileDescription());
			}
		}else //进程存在，更新下相关的数据
		{
			LVFINDINFO stFindInfo;
			stFindInfo.flags = LVFI_PARAM;
			stFindInfo.lParam = Offset->UniqueProcessId;
			INT nItem = m_ProcessList.FindItem(&stFindInfo);

			//毫无疑问先更新CPU
			m_ProcessList.SetItemFormatText(nItem,PROCESS_CPU,TEXT("%.2lf"),m_ProcessTool.GetSingleProcessCpuTime(Offset));

			m_ProcessTool.GetExtendInfo(Offset,&ProExtendInfo,EXTEND_PARENT_NAME | EXTEND_VM_SIZE |EXTEND_PRORITY);
			
			m_ProcessList.SetItemText(nItem,PROCESS_PRIORITY,ProExtendInfo.lpszProcessPrority);
			m_ProcessList.SetItemFormatText(nItem,PROCESS_PARENT,_tcslen(ProExtendInfo.szParentFileName)?TEXT("%d(%s)"):TEXT("%d"),Offset->InheritedFromUniqueProcessId,ProExtendInfo.szParentFileName);
			m_ProcessList.SetItemInt(nItem,PROCESS_THREAD_COUNT,Offset->NumberOfThreads);
			m_ProcessList.SetItemFormatText(nItem,PROCESS_MEMEORY,TEXT("%d K"),Offset->WorkingSetSize/1024);
			m_ProcessList.SetItemFormatText(nItem,PROCESS_VIRTUAL_MEMEORY,TEXT("%d K"),ProExtendInfo.dwVirtualMemeorySize/1024);
			m_ProcessList.SetItemInt(nItem,PROCESS_HANDLE_COUNT,Offset->HandleCount);
		}
	} while (m_ProcessTool.ProcessNext(&Offset));
	
	m_ProcessTool.SetLastTotalProcessCpuTime();

	//采集进程的CPU时间
	m_ProcessTool.SetSingleProcessCpuTime(lpTempOffset);

	//删除不存在的进程
	DWORD dwPID = 0;
	DWORD dwItemCount = m_ProcessList.GetItemCount();
	for(INT i=0;i<dwItemCount;i++)
	{
		dwPID = m_ProcessList.GetItemData(i);
		if(m_ProcessTool.ProcessExited(dwPID))
		{
			ProcessListDeleteItem(i);
			m_ProcessTool.DeleteProcessTime(dwPID);
			dwItemCount--;
		}
	}

	return TRUE;
}



void CProcessManager::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static double dCPUUsage=0,dPageFileTotalSize,dPageFileUsageSize;
	
	switch(nIDEvent)
	{
	case PROCESS_MANAGER_TIMER:
		ShowProcessInfo();
		((CMainFrame *)AfxGetApp()->GetMainWnd())->SetStatusBarText(STATUSBAR_HANDLE_NUM,
																	TEXT("句柄数: %d"),
																	m_ProcessTool.GetHandleCount());

		((CMainFrame *)AfxGetApp()->GetMainWnd())->SetStatusBarText(STATUSBAR_PROCESS_NUM,
																	TEXT("进程数: %d"),
																	m_ProcessTool.GetProcessCount());
		
		((CMainFrame *)AfxGetApp()->GetMainWnd())->SetStatusBarText(STATUSBAR_THREAD_NUM,
																	TEXT("线程数: %d"),
																	m_ProcessTool.GetThreadCount());

		((CMainFrame *)AfxGetApp()->GetMainWnd())->SetStatusBarText(STATUSBAR_CPU_NUM,
																	TEXT("CPU数量: %d"),
																	m_PerformanceTool.GetCPUNum());

		m_PerformanceTool.GetCPUUsage(dCPUUsage,NULL,NULL);
		((CMainFrame *)AfxGetApp()->GetMainWnd())->SetStatusBarText(STATUSBAR_CPU_UASGE,
																	TEXT("CPU 使用: %.2lf"),
																	dCPUUsage);

		m_PerformanceTool.GetPageFileUsag(dPageFileTotalSize,dPageFileUsageSize);
		((CMainFrame *)AfxGetApp()->GetMainWnd())->SetStatusBarText(STATUSBAR_PAGEFILE_USAGE,
																	TEXT("提交更改: %.2lfM / %.2lfM"),
																	dPageFileUsageSize,dPageFileTotalSize);


		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CProcessManager::OnDblclkProcessList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//获取PID
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	m_nCurrentSelectItem = m_ProcessList.GetNextSelectedItem(pos);
	m_ProcessList.SetCheck(m_nCurrentSelectItem,m_ProcessList.GetCheck(m_nCurrentSelectItem)?FALSE:TRUE);

	*pResult = 0;
}


void CProcessManager::OnRclickProcessList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//获取PID
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	m_nCurrentSelectItem = m_ProcessList.GetNextSelectedItem(pos);


	CMenu *pSub = m_ProcessMenu.GetSubMenu(0);
	
	switch(m_nSubList)
	{
	case SUB_LIST_MODULE:
		pSub->CheckMenuItem(IDM_PROCESS_MODULE,MF_BYCOMMAND | MF_CHECKED);
		pSub->CheckMenuItem(IDM_PROCESS_THREAD,MF_BYCOMMAND | MF_UNCHECKED);
		pSub->CheckMenuItem(IDM_PROCESS_HANDLE,MF_BYCOMMAND | MF_UNCHECKED);
		break;
	case SUB_LIST_THREAD:
		pSub->CheckMenuItem(IDM_PROCESS_MODULE,MF_BYCOMMAND | MF_UNCHECKED);
		pSub->CheckMenuItem(IDM_PROCESS_THREAD,MF_BYCOMMAND | MF_CHECKED);
		pSub->CheckMenuItem(IDM_PROCESS_HANDLE,MF_BYCOMMAND | MF_UNCHECKED);
		break;
	case SUB_LIST_HANDLE: 
		pSub->CheckMenuItem(IDM_PROCESS_MODULE,MF_BYCOMMAND | MF_UNCHECKED);
		pSub->CheckMenuItem(IDM_PROCESS_THREAD,MF_BYCOMMAND | MF_UNCHECKED);
		pSub->CheckMenuItem(IDM_PROCESS_HANDLE,MF_BYCOMMAND | MF_CHECKED);
		break;
	}


	PROCESS_SUSPEND::const_iterator iter;
	iter = m_ProcessSuspendInfo.find(m_ProcessList.GetItemData(m_nCurrentSelectItem));
	if(iter != m_ProcessSuspendInfo.end())
		pSub->ModifyMenu(IDM_SUSPPED_PROCESS,MF_BYCOMMAND|MF_STRING,IDM_SUSPPED_PROCESS,TEXT("恢复进程运行"));
	else
		pSub->ModifyMenu(IDM_SUSPPED_PROCESS,MF_BYCOMMAND|MF_STRING,IDM_SUSPPED_PROCESS,TEXT("暂停进程运行"));



	//设置进程优先级
	CString szProrityText = m_ProcessList.GetItemText(m_nCurrentSelectItem,PROCESS_PRIORITY);
	pSub->CheckMenuItem(IDM_SET_PRIORITY_REALTIME,
		MF_BYCOMMAND | (szProrityText==TEXT("实时"))?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_SET_PRIORITY_ABOVE_HEIGHT,
		MF_BYCOMMAND | (szProrityText == TEXT("高于标准"))?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_SET_PRIORITY_HEIGHT,
		MF_BYCOMMAND | (szProrityText == TEXT("高"))?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_SET_PRIORITY_NORMAL,
		MF_BYCOMMAND | (szProrityText == TEXT("标准"))?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_SET_PRIORITY_BELOW_NORMAL,
		MF_BYCOMMAND | (szProrityText == TEXT("低于标准"))?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_SET_PRIORITY_IDEL,
		MF_BYCOMMAND | (szProrityText == TEXT("低"))?MF_CHECKED:MF_UNCHECKED);


	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	CSuperProcessManagerView * pView;
	pView = (CSuperProcessManagerView* )(((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView());

	pSub->CheckMenuItem(IDM_MONITOR_NETWOKIO,pView->ProcessNetworkIoIsMonitoring(dwPID)?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_MONITOR_FILEIO,pView->ProcessFileIoIsMonitoring(dwPID)?MF_CHECKED:MF_UNCHECKED);
	pSub->CheckMenuItem(IDM_MONITOR_REGIO,pView->ProcessRegIoIsMonitoring(dwPID)?MF_CHECKED:MF_UNCHECKED);





	POINT point;
	GetCursorPos(&point);
	pSub->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTBUTTON,point.x,point.y,this);
	*pResult = 0;
}




void CProcessManager::InsertStirngToBar(CListCtrl * pList,LPCOLUMN_DATA pData,DWORD dwSize,INT nCurSel)
{
	//将相关信息显示到工具条
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	INT nCurSelItem = pList->GetNextSelectedItem(pos);

	CString szItemText;
	CMainFrame *pMainFrame = ((CMainFrame *)AfxGetApp()->GetMainWnd());
	
	if(pMainFrame == NULL)
		return;
	
	pMainFrame->m_PublicTextBar.DeleteAllString();

	for(INT i = 0;i < dwSize ;i++)
	{
		szItemText = pList->GetItemText(nCurSelItem,i);

		pMainFrame->m_PublicTextBar.InsertString(TEXT("%s:%s"),
			pData[i].szText,(LPCTSTR)szItemText);
	}
	pMainFrame->m_PublicTextBar.SetCurSel(nCurSel);
}


void CProcessManager::OnClickProcessList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	//获取PID
	POSITION pos = m_ProcessList.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	m_nCurrentSelectItem = m_ProcessList.GetNextSelectedItem(pos);

	InsertStirngToBar(&m_ProcessList,g_ProcessCol,sizeof(g_ProcessCol)/sizeof(COLUMN_DATA),PROCESS_PATH);

	switch(m_nSubList)
	{
	case SUB_LIST_MODULE:
		OnProcessModule();
		break;
	case SUB_LIST_THREAD:
		OnProcessThread();
		break;
	case SUB_LIST_HANDLE:
		OnProcessHandle();
		break;
	}
	*pResult = 0;
}


void CProcessManager::OnClickModuleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	InsertStirngToBar(&m_ModuleList,g_ModuleCol,sizeof(g_ModuleCol)/sizeof(COLUMN_DATA),MODULE_PATH);
}

void CProcessManager::OnClickThreadList(NMHDR* pNMHDR, LRESULT* pResult)
{
	InsertStirngToBar(&m_ThreadList,g_ThreadCol,sizeof(g_ThreadCol)/sizeof(COLUMN_DATA),THREAD_MODULE_PATH);
}

void CProcessManager::OnClickHandleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	InsertStirngToBar(&m_HandleList,g_HandleCol,sizeof(g_HandleCol)/sizeof(COLUMN_DATA),HANDLE_NAME);
}




void CProcessManager::SwitchSubWindow()
{
	switch(m_nSubList)
	{
		case SUB_LIST_MODULE: 
			m_ModuleList.ShowWindow(SW_SHOW);
			m_ThreadList.ShowWindow(SW_HIDE);
			m_HandleList.ShowWindow(SW_HIDE);
			break;

		case SUB_LIST_THREAD:
			m_ThreadList.ShowWindow(SW_SHOW);
			m_ModuleList.ShowWindow(SW_HIDE);
			m_HandleList.ShowWindow(SW_HIDE);
			break;
		case SUB_LIST_HANDLE:
			m_HandleList.ShowWindow(SW_SHOW);
			m_ThreadList.ShowWindow(SW_HIDE);
			m_ModuleList.ShowWindow(SW_HIDE);
			break;
	}
}



BOOL CProcessManager::OnSubModule(DWORD dwPID)
{
	SHFILEINFO shfi;
	INT nItem = -1;
	MODULEENTRY32 me={sizeof(MODULEENTRY32)};

	if(!m_ModuleTool.CreateSnapshotThread(dwPID))
		return FALSE;

	if(!m_ModuleTool.Module32First(&me))
		return FALSE;

	do 
	{
		nItem = m_ModuleList.GetItemCount();
		SHGetFileInfo(me.szExePath,FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(shfi),SHGFI_SMALLICON|SHGFI_ICON |SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME);
		m_ModuleList.InsertItem(nItem,NULL,m_ImageModule.Add(shfi.hIcon));
		m_ModuleList.SetItemData(nItem,(DWORD)me.modBaseAddr);
		m_ModuleList.SetItemText(nItem,MODULE_NAME,me.szModule);
		m_ModuleList.SetItemText(nItem,MODULE_PATH,me.szExePath);
		m_ModuleList.SetItemFormatText(nItem,MODULE_SIZE,TEXT("%d K"),me.modBaseSize/1024);
		m_ModuleList.SetItemFormatText(nItem,MODULE_BASE,TEXT("%08X"),(DWORD)me.modBaseAddr);
		m_ModuleList.SetItemText(nItem,MODULE_CRETION_TIME,GetFileCreationTime(CString(me.szExePath)));

		CFileVersion FileVersion;
		if(_tcslen(me.szExePath) && FileVersion.Open(me.szExePath))
		{
			m_ModuleList.SetItemText(nItem,MODULE_COMPANY,FileVersion.GetCompanyName());
			m_ModuleList.SetItemText(nItem,MODULE_DESCRIBE,FileVersion.GetFileDescription());
			m_ModuleList.SetItemText(nItem,MODULE_VERSION,FileVersion.GetFileVersion());
		}
	} while (m_ModuleTool.Module32Next(&me));
	return TRUE;
}

BOOL CProcessManager::OnSubHandle(DWORD dwPID)
{
	INT nItem = -1;
	PSYSTEM_HANDLE_INFORMATION lpHandleInfo=NULL;
	EXTENDHANDLEINFO ExtendHandleInfo;

	if(!m_HandleTool.CreateSnapshotHandle(dwPID))
		return FALSE;

	if(!m_HandleTool.Handle32First(&lpHandleInfo))
		return FALSE;

	do
	{
		if(m_HandleTool.GetHandleExtendInfo(lpHandleInfo,&ExtendHandleInfo))
		{
			nItem= m_HandleList.GetItemCount();
			m_HandleList.InsertItem(nItem,NULL,NULL);
			m_HandleList.SetItemData(nItem,(DWORD)ExtendHandleInfo.hHandle);
			m_HandleList.SetItemText(nItem,HANDLE_TYPE,ExtendHandleInfo.szOjectType);
			m_HandleList.SetItemInt(nItem,HANDLE_TYPE_VALUE,ExtendHandleInfo.uTypeVaule);
			m_HandleList.SetItemText(nItem,HANDLE_NAME,ExtendHandleInfo.szOjectName);
			m_HandleList.SetItemFormatText(nItem,HANDLE_VALUE,TEXT("0x%08X"),ExtendHandleInfo.hHandle);
			m_HandleList.SetItemInt(nItem,HANDLE_COUNT,ExtendHandleInfo.AcCount);
			m_HandleList.SetItemText(nItem,HANDLE_INHERIT,ExtendHandleInfo.bInherit?TEXT("yes"):TEXT("no"));
			m_HandleList.SetItemText(nItem,HANDLE_CLOSE,ExtendHandleInfo.bClose?TEXT("no"):TEXT("yes"));
		}
	}while(m_HandleTool.Handle32Next(&lpHandleInfo));
	
	return TRUE;
}

void CProcessManager::OnProcessHandle() 
{
	// TODO: Add your command handler code here
	m_HandleList.DeleteAllItems();
	m_nSubList = SUB_LIST_HANDLE;
	SwitchSubWindow();
	
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	OnSubHandle(dwPID);
}

void CProcessManager::OnProcessModule() 
{
	// TODO: Add your command handler code here
	m_ModuleList.DeleteAllItems();
	while(m_ImageModule.Remove(0));
	m_nSubList = SUB_LIST_MODULE;
	SwitchSubWindow();
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	OnSubModule(dwPID);
}



BOOL CProcessManager::OnSubThread(DWORD dwPID)
{
	INT nItem = -1;
	SHFILEINFO shfi;
	CString szPath;
	LPTHREADINFO lpThreadInfo = NULL;
	EXTENDTHREADINFO ExtendThreadInfo;
	if(!m_ThreadTool.CreateSnapshotThread(dwPID))
		return FALSE;
	if(!m_ThreadTool.Thread32First(&lpThreadInfo))
		return FALSE;
	
	do 
	{
		nItem = m_ThreadList.GetItemCount();
		
		if(m_ThreadTool.GetThreadExtendInfo(lpThreadInfo,&ExtendThreadInfo))
		{
			szPath = ExtendThreadInfo.szThreadModule;
			DosDeviceToFilePath(szPath);
			SHGetFileInfo(szPath,FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(shfi),SHGFI_SMALLICON|SHGFI_ICON |SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME);
			m_ThreadList.InsertItem(nItem,NULL,m_ImageThread.Add(shfi.hIcon));
			m_ThreadList.SetItemFormatText(nItem,THREAD_ID,TEXT("%d(%Xh)%s"),
				lpThreadInfo->ClientId.UniqueThread,
				lpThreadInfo->ClientId.UniqueThread,
				ExtendThreadInfo.bIsMainThread?TEXT("(主线程)"):TEXT(""));
			m_ThreadList.SetItemText(nItem,THREAD_MODULE_PATH,szPath);
			m_ThreadList.SetItemFormatText(nItem,THREAD_ENTRY_ADDRESS,TEXT("%08X"),ExtendThreadInfo.dwEntryAddress);
			m_ThreadList.SetItemText(nItem,THREAD_DESKTOP,ExtendThreadInfo.szThreadDesktop);
			m_ThreadList.SetItemText(nItem,THREAD_PRORITY,ExtendThreadInfo.lpszPrority);
			m_ThreadList.SetItemText(nItem,THREAD_CURRENT_STATE,ExtendThreadInfo.lpszThreadStatus);
			m_ThreadList.SetItemText(nItem,THREAD_CRETION_TIME,GetFileCreationTime(szPath));

			CFileVersion FileVersion;
			if(!szPath.IsEmpty()&&FileVersion.Open(szPath))
			{
				m_ThreadList.SetItemText(nItem,THREAD_COMPANY,FileVersion.GetCompanyName());
				m_ThreadList.SetItemText(nItem,THREAD_DESCRIBE,FileVersion.GetFileDescription());
				m_ThreadList.SetItemText(nItem,THREAD_VERSION,FileVersion.GetFileVersion());
			}
		}else
		{
			m_ThreadList.InsertItem(nItem,NULL,0);
		}

		m_ThreadList.SetItemData(nItem,lpThreadInfo->ClientId.UniqueThread);
		m_ThreadList.SetItemInt(nItem,THREAD_SWITCH_COUNT,lpThreadInfo->ContextSwitchCount);
		
	} while (m_ThreadTool.Thread32Next(&lpThreadInfo));

	return TRUE;
}


void CProcessManager::OnProcessThread() 
{
	// TODO: Add your command handler code here
	m_ThreadList.DeleteAllItems();
	while(m_ImageThread.Remove(0));

	m_ImageThread.Add(::LoadIcon(NULL,IDI_QUESTION));

	m_nSubList = SUB_LIST_THREAD;
	SwitchSubWindow();
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	OnSubThread(dwPID);
}

void CProcessManager::OnSusppedProcess() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	
	PROCESS_SUSPEND::iterator iter;
	iter = m_ProcessSuspendInfo.find(dwPID);
	
	if(iter == m_ProcessSuspendInfo.end())
	{
		if(m_ThreadTool.SusppedProcess(dwPID))
		{
			MessageBox(TEXT("暂停进程运行成功!"),TEXT("恭喜"),MB_OK|MB_ICONINFORMATION);
			m_ProcessSuspendInfo.insert(PROCESS_SUSPEND_PAIR(dwPID,TRUE));
		}
		else
		{
			CFormatMessage ErrorMsg;
			szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
			MessageBox(szText,TEXT("暂停进程运行失败!"),MB_OK|MB_ICONERROR);
		}
	}else
	{
		if(m_ThreadTool.ResumeProcess(dwPID))
		{
			MessageBox(TEXT("恢复进程运行成功!"),TEXT("恭喜"),MB_OK|MB_ICONINFORMATION);
			m_ProcessSuspendInfo.erase(iter);
		}
		else
		{
			CFormatMessage ErrorMsg;
			szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
			MessageBox(szText,TEXT("恢复进程运行失败!"),MB_OK|MB_ICONERROR);
		}
	}
}


void CProcessManager::OnTerminatorProcessCureent() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ThreadTool.SusppedProcess(dwPID))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("暂停进程运行失败!"),MB_OK|MB_ICONERROR);
	}
	if(!m_ProcessTool.TerminateProcess(dwPID))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("结束进程失败!"),MB_OK|MB_ICONERROR);
	}
}


void CProcessManager::OnTerminatorProcessCheckbox() 
{
	// TODO: Add your command handler code here
	DWORD dwCount = m_ProcessList.GetItemCount();
	CString szText,szTitle;
	DWORD dwPID;
	for(DWORD i = 0 ; i< dwCount; i++)
	{
		if(m_ProcessList.GetCheck(i))
		{
			dwPID = m_ProcessList.GetItemData(i);

			if(!m_ThreadTool.SusppedProcess(dwPID))
			{
				CFormatMessage ErrorMsg;
				szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
				szTitle.Format(TEXT("暂停进程ID[%d]运行失败!"),dwPID);
				MessageBox(szText,szTitle,MB_OK|MB_ICONERROR);
			}
			if(!m_ProcessTool.TerminateProcess(dwPID))
			{
				CFormatMessage ErrorMsg;
				szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
				szTitle.Format(TEXT("结束进程[%d]失败!"),dwPID);
				MessageBox(szText,szTitle,MB_OK|MB_ICONERROR);
			}
		}
	}
}


void CProcessManager::OnTerminatorProcessTree() 
{
	// TODO: Add your command handler code here
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	DWORD dwCount = m_ProcessList.GetItemCount();
	CString szParent,szText,szTitle;
	DWORD dwChildPID;
	for(DWORD i = 0 ; i< dwCount; i++)
	{
		szParent = m_ProcessList.GetItemText(i,PROCESS_PARENT);
		
		if(dwPID == _ttoi(szParent))
		{
			dwChildPID = m_ProcessList.GetItemData(i);
			
			if(!m_ThreadTool.SusppedProcess(dwChildPID))
			{
				CFormatMessage ErrorMsg;
				szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
				szTitle.Format(TEXT("暂停进程ID[%d]运行失败!"),dwChildPID);
				MessageBox(szText,szTitle,MB_OK|MB_ICONERROR);
			}
			if(!m_ProcessTool.TerminateProcess(dwChildPID))
			{
				CFormatMessage ErrorMsg;
				szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
				szTitle.Format(TEXT("结束进程[%d]失败!"),dwChildPID);
				MessageBox(szText,szTitle,MB_OK|MB_ICONERROR);
			}
		}
	}
	if(!m_ThreadTool.SusppedProcess(dwPID))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		szTitle.Format(TEXT("暂停进程ID[%d]运行失败!"),dwPID);
		MessageBox(szText,szTitle,MB_OK|MB_ICONERROR);
	}
	if(!m_ProcessTool.TerminateProcess(dwPID))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		szTitle.Format(TEXT("结束进程[%d]失败!"),dwPID);
		MessageBox(szText,szTitle,MB_OK|MB_ICONERROR);
	}
}


void CProcessManager::OnSetPriorityRealtime() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.SetPriorityClass(dwPID,REALTIME_PRIORITY_CLASS))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("设置进程优先级失败!"),MB_OK|MB_ICONERROR);
	}
}

void CProcessManager::OnSetPriorityNormal() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.SetPriorityClass(dwPID,NORMAL_PRIORITY_CLASS))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("设置进程优先级失败!"),MB_OK|MB_ICONERROR);
	}
}

void CProcessManager::OnSetPriorityIdel() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.SetPriorityClass(dwPID,IDLE_PRIORITY_CLASS))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("设置进程优先级失败!"),MB_OK|MB_ICONERROR);
	}
}

void CProcessManager::OnSetPriorityHeight() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.SetPriorityClass(dwPID,HIGH_PRIORITY_CLASS))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("设置进程优先级失败!"),MB_OK|MB_ICONERROR);
	}
}

void CProcessManager::OnSetPriorityBelowNormal() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.SetPriorityClass(dwPID,BELOW_NORMAL_PRIORITY_CLASS))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("设置进程优先级失败!"),MB_OK|MB_ICONERROR);
	}
}

void CProcessManager::OnSetPriorityAboveHeight() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.SetPriorityClass(dwPID,ABOVE_NORMAL_PRIORITY_CLASS))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("设置进程优先级失败!"),MB_OK|MB_ICONERROR);
	}
}

void CProcessManager::OnDebugProcess() 
{
	// TODO: Add your command handler code here
	CString szText;
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	if(!m_ProcessTool.DebugBreakProcess(dwPID))
	{
		CFormatMessage ErrorMsg;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),ErrorMsg.GetLastError(),ErrorMsg.szErrorText);
		MessageBox(szText,TEXT("调试进程失败!"),MB_OK|MB_ICONINFORMATION);
	}
}

void CProcessManager::OnAttributeFile() 
{
	// TODO: Add your command handler code here
	CString szPath = m_ProcessList.GetItemText(m_nCurrentSelectItem,PROCESS_PATH);
	SHELLEXECUTEINFO   sei; 
	sei.lpVerb   =   TEXT("properties"); 
	sei.lpFile   =   szPath.GetBuffer(0); 
	sei.lpDirectory   =   NULL; 
	sei.lpParameters   =   NULL; 
	sei.nShow   =   SW_SHOWNORMAL; 
	sei.fMask   =   SEE_MASK_INVOKEIDLIST; 
	sei.lpIDList   =   NULL; 
	sei.cbSize   =   sizeof(SHELLEXECUTEINFO); 
	ShellExecuteEx(&sei); 
}

void CProcessManager::OnPositioningFile() 
{
	// TODO: Add your command handler code here
	CString szPath = m_ProcessList.GetItemText(m_nCurrentSelectItem,PROCESS_PATH);
	CString szDwPath=TEXT("/select,");
	szDwPath+=szPath;
	ShellExecute(NULL,NULL,TEXT("explorer"),szDwPath.GetBuffer(0),NULL,SW_SHOW); 
}


void CProcessManager::OnBaiduSearch() 
{
	// TODO: Add your command handler code here
	CString szFileName=m_ProcessList.GetItemText(m_nCurrentSelectItem,PROCESS_NAME);
	CString szURL;
	szURL.Format(TEXT("http://www.baidu.com/s?wd=%s"),szFileName);
	OpenUrl(szURL);
}

void CProcessManager::OnGoogleSearch() 
{
	// TODO: Add your command handler code here
	CString szFileName=m_ProcessList.GetItemText(m_nCurrentSelectItem,PROCESS_NAME);
	CString szURL;
	szURL.Format(TEXT("http://www.google.com/search?q=%s"),szFileName);
	OpenUrl(szURL);
}

void CProcessManager::OnOnlineVirusScan() 
{
	// TODO: Add your command handler code here
	OpenUrl(TEXT("http://www.virscan.org/"));
}



void CProcessManager::OnMonitorAll() 
{
	// TODO: Add your command handler code here
	
}

void CProcessManager::OnMonitorFileio() 
{
	// TODO: Add your command handler code here
	
}


void CProcessManager::OnMonitorNetwokio() 
{
	// TODO: Add your command handler code here
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);
	CSuperProcessManagerView * pView;
	pView = (CSuperProcessManagerView* )(((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView());
	
	CString szError;

	//该进程还没有HOOK，故HOOK之
	if(!pView->ProcessNetworkIoIsMonitoring(dwPID))
	{
		if(!pView->MonitorProcessNetworkIo(dwPID))
		{
			CFormatMessage FormatMessage;
			szError.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),
				FormatMessage.GetLastError(),FormatMessage.szErrorText);
			MessageBox(szError,TEXT("监视进程网络数据失败!"),MB_OK|MB_ICONERROR);
			return;
		}
		pView->SwitchWindow(TAB_PROCESS_NETWORK_IO);
	}else
	{
		if(!pView->UnMonitorProcessNetworkIo(dwPID))
		{
			CFormatMessage FormatMessage;
			szError.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),
				FormatMessage.GetLastError(),FormatMessage.szErrorText);
			MessageBox(szError,TEXT("取消监视进程网络数据失败!"),MB_OK|MB_ICONERROR);
			return;
		}
	}
}

void CProcessManager::OnMonitorRegio() 
{
	// TODO: Add your command handler code here
	
}




void CProcessManager::OnInjectProcess() 
{
	// TODO: Add your command handler code here
	DWORD dwPID = m_ProcessList.GetItemData(m_nCurrentSelectItem);

	TCHAR  szFilePath[MAX_PATH]={NULL};
	OPENFILENAME OpenInfo;
	ZeroMemory(&OpenInfo,sizeof(OPENFILENAME));
	OpenInfo.lStructSize = sizeof(OPENFILENAME);
	OpenInfo.hwndOwner = m_hWnd;
	OpenInfo.lpstrFile = szFilePath;
	OpenInfo.nMaxFile = sizeof(szFilePath);
	OpenInfo.lpstrFilter = TEXT("动态链接库(.dll)\0*.dll\0所有文件\0*.*");
	OpenInfo.lpstrTitle=TEXT("请选择要注入的文件");
	OpenInfo.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
	
	if(GetOpenFileName(&OpenInfo))
	{
		HANDLE hWait = NULL;
		if(InjectProcess(dwPID,szFilePath,hWait))
		{
			MessageBox(TEXT("注入进程成功!"),TEXT("恭喜"),MB_OK|MB_ICONINFORMATION);
			CloseHandle(hWait);
			return;
		}
		CFormatMessage FormatMessage;
		CString szError;
		szError.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),
			FormatMessage.GetLastError(),FormatMessage.szErrorText);
			MessageBox(szError,TEXT("注入进程失败!"),MB_OK|MB_ICONERROR);
	}
}

