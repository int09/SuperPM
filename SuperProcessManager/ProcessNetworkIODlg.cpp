// ProcessNetworkIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProcessManager.h"
#include "ProcessNetworkIODlg.h"
#include "DataManager.h"
#include "MainFrm.h"
#include "HookManager.h"
#include "MonitorNetworkIoSettingDlg.h"
#include "MonitorFileDlg.h"
#include "ProcessTool/ProcessTool.h"
#include "CMListCtrlFilterDlg.h"

#include <shlwapi.h>
#include <strsafe.H>





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessNetworkIODlg dialog

extern CDataManager	theDatamgr;
extern CSuperProcessManagerApp theApp;
extern CHookManager			theHookManager;
extern DWORD   g_dwNetworkIoSettingFlags;

#define WM_DATA_LIST_RESIZING		WM_USER+0x100
#define WM_READ_NETWORK_IO_DATA		WM_USER+0x101



static enum {CN_ID,CN_API,CN_PROTOCOL,CN_IP,CN_DNS,CN_PORT,CN_PROCESS_FILENAME,CN_PROCESS_ID,CN_THREAD_ID,CN_DATA,
CN_DATA_SIZE,CN_TIME,CN_RESULT,CN_LASTERROR,CN_PROCESS_PATH,CN_IODATA_ADDRESS};


static COLUMN_DATA g_DataListCol[]=
{
	{CN_ID,TEXT("ID"),LVCFMT_LEFT,FALSE,100,TYPE_INTGER},
	{CN_API,TEXT("API"),LVCFMT_LEFT,FALSE,100,TYPE_STRING},
	{CN_PROTOCOL,TEXT("协议"),LVCFMT_LEFT,TRUE,70,TYPE_STRING},
	{CN_IP,TEXT("IP"),LVCFMT_LEFT,TRUE,100,TYPE_STRING},
	{CN_DNS,TEXT("域名"),LVCFMT_LEFT,TRUE,100,TYPE_STRING},
	{CN_PORT,TEXT("Port"),LVCFMT_LEFT,TRUE,50,TYPE_INTGER},
	{CN_PROCESS_FILENAME,TEXT("进程名"),LVCFMT_LEFT,TRUE,100,TYPE_STRING},
	{CN_PROCESS_ID,TEXT("进程ID"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{CN_THREAD_ID,TEXT("线程ID/入口"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{CN_DATA,TEXT("数据"),LVCFMT_LEFT,TRUE,120,TYPE_STRING},
	{CN_DATA_SIZE,TEXT("大小"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{CN_TIME,TEXT("接收时间"),LVCFMT_LEFT,TRUE,130,TYPE_DATE},
	{CN_RESULT,TEXT("返回值"),LVCFMT_LEFT,TRUE,70,TYPE_INTGER},
	{CN_LASTERROR,TEXT("WSALastError"),LVCFMT_LEFT,TRUE,200,TYPE_STRING},
	{CN_PROCESS_PATH,TEXT("进程路径"),LVCFMT_LEFT,TRUE,150,TYPE_STRING},
	{CN_IODATA_ADDRESS,TEXT("数据地址"),LVCFMT_LEFT,TRUE,80,TYPE_INTGER},
};





CProcessNetworkIODlg::CProcessNetworkIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessNetworkIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessNetworkIODlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDataListButtomCoor=0;
	m_nDataListCurrentSel = -1;
}




void CProcessNetworkIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessNetworkIODlg)
	DDX_Control(pDX, IDC_DATA_LIST, m_DataList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessNetworkIODlg, CDialog)
	//{{AFX_MSG_MAP(CProcessNetworkIODlg)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_DATA_LIST, OnRclickDataList)
	ON_COMMAND(IDM_DELETE_CURRENT, OnDeleteCurrent)
	ON_COMMAND(IDM_DELETE_SELECED, OnDeleteSeleced)
	ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDM_SELECT_REVERSE_ALL, OnSelectReverseAll)
	ON_COMMAND(IDM_CANCEL_ALL, OnCancelAll)
	ON_NOTIFY(NM_CLICK, IDC_DATA_LIST, OnClickDataList)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_OPEN_DNS, OnOpenDns)
	ON_COMMAND(IDM_OPEN_IP, OnOpenIp)
	ON_COMMAND(IDM_GOOGLE_SEARCH_DNS, OnGoogleSearchDns)
	ON_COMMAND(IDM_GOOGLE_SEARCH_IP, OnGoogleSearchIp)
	ON_COMMAND(IDM_BAIDU_SEARCH_DNS, OnBaiduSearchDns)
	ON_COMMAND(IDM_BAIDU_SEARCH_IP, OnBaiduSearchIp)
	ON_COMMAND(IDM_POSITIONING_FILE, OnPositioningFile)
	ON_COMMAND(IDM_GOOGLE_SEARCH, OnGoogleSearch)
	ON_COMMAND(IDM_BAIDU_SEARCH, OnBaiduSearch)
	ON_COMMAND(IDM_ATTRIBUTE_FILE, OnAttributeFile)
	ON_COMMAND(IDM_ONLINE_VIRUS_SCAN, OnOnlineVirusScan)
	ON_COMMAND(IDM_SETTING, OnSetting)
	ON_COMMAND(IDM_DELETE_INVALID_DATA, OnDeleteInvalidData)
	ON_COMMAND(IDM_MONITOR_FILE, OnMonitorFile)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, IDC_DATA_LIST, OnDblclkDataList)
	ON_COMMAND(IDM_TERMENATER_PROCESS, OnTermenaterProcess)
	ON_COMMAND(IDM_SUSPEND_PROCESS, OnSuspendProcess)
	ON_COMMAND(IDM_RESUME_PROCESS, OnResumeProcess)
	ON_COMMAND(IDM_FILTER, OnFilter)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DATA_LIST_RESIZING,OnDataListResizeing)
	ON_MESSAGE(WM_READ_NETWORK_IO_DATA,OnReadNetworkIoData)
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_DATA_LIST, OnDrawColorForDataList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessNetworkIODlg message handlers

BOOL CProcessNetworkIODlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ThreadTool.Create();

	theDatamgr.GetReserveInfo()->hNetworkIoDlg = m_hWnd;
	theDatamgr.GetReserveInfo()->uNetworkIoDlgMsg = WM_READ_NETWORK_IO_DATA;

	InitDataList();

	HFONT hFont = (HFONT)::GetStockObject(SYSTEM_FIXED_FONT);
    m_pEditFont = CFont::FromHandle(hFont);	
	((CEdit *)GetDlgItem(IDC_HEX_TEXT))->SetFont(m_pEditFont);
	((CEdit *)GetDlgItem(IDC_ASCII_TEXT))->SetFont(m_pEditFont);
	m_DataListMenu.LoadMenu(IDR_NETWORK_IO_DATA_LIST_MENU);


	//加载通用的菜单
	CMenu CommonMenu;
	CommonMenu.LoadMenu(IDR_PROCESS_LIST_MENU);
	CMenu *pCommonMenu = CommonMenu.GetSubMenu(0);
	UINT nCount = pCommonMenu->GetMenuItemCount();
	BOOL bFind = FALSE;
	CString szMenuText;

	for(UINT i = 0;i<nCount;i++)
	{
		if(IDM_POSITIONING_FILE == pCommonMenu->GetMenuItemID(i))
		{
			bFind = TRUE;
			(m_DataListMenu.GetSubMenu(0))->AppendMenu(MF_SEPARATOR);
		}
		if(bFind)
		{
			pCommonMenu->GetMenuString(i,szMenuText,MF_BYPOSITION);
			(m_DataListMenu.GetSubMenu(0))->AppendMenu(MF_STRING,pCommonMenu->GetMenuItemID(i),szMenuText);
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CProcessNetworkIODlg::InitDataList()
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


void CProcessNetworkIODlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(!m_nDataListButtomCoor)
	{	
		if(m_DataList.GetSafeHwnd()) m_DataList.MoveWindow(0,0,cx,cy * 0.6);
	}
	else
	{
		if(m_DataList.GetSafeHwnd()) m_DataList.MoveWindow(0,0,cx,m_nDataListButtomCoor);
	}


	CRect HexEditRc,AsciiEditRc;
	AsciiEditRc.top = m_nDataListButtomCoor?(m_nDataListButtomCoor+5):(cy * 0.6 + 5);
	AsciiEditRc.left = 0;
	AsciiEditRc.right = cx/2;
	AsciiEditRc.bottom = cy;
	if(((CEdit *)GetDlgItem(IDC_ASCII_TEXT))->GetSafeHwnd())
		((CEdit *)GetDlgItem(IDC_ASCII_TEXT))->MoveWindow(&AsciiEditRc);


	HexEditRc.top = m_nDataListButtomCoor?(m_nDataListButtomCoor+5):(cy * 0.6 + 5);
	HexEditRc.left = AsciiEditRc.right+5;
	HexEditRc.right = cx;
	HexEditRc.bottom = cy;
	if(((CEdit *)GetDlgItem(IDC_HEX_TEXT))->GetSafeHwnd())
		((CEdit *)GetDlgItem(IDC_HEX_TEXT))->MoveWindow(&HexEditRc);

}

LRESULT CProcessNetworkIODlg::OnDataListResizeing(WPARAM wParam,LPARAM lParam)
{
	m_nDataListButtomCoor = HIWORD(lParam);

	CRect ClientRc;
	GetClientRect(&ClientRc);

	CRect HexEditRc,AsciiEditRc;
	AsciiEditRc.top = m_nDataListButtomCoor + 9;
	AsciiEditRc.left = 0;
	AsciiEditRc.right = ClientRc.right/2;
	AsciiEditRc.bottom = ClientRc.bottom;
	if(((CEdit *)GetDlgItem(IDC_ASCII_TEXT))->GetSafeHwnd())
		((CEdit *)GetDlgItem(IDC_ASCII_TEXT))->MoveWindow(&AsciiEditRc);

	HexEditRc.top = m_nDataListButtomCoor + 9;
	HexEditRc.left = AsciiEditRc.right+5;
	HexEditRc.right = ClientRc.right;
	HexEditRc.bottom = ClientRc.bottom;

	if(((CEdit *)GetDlgItem(IDC_HEX_TEXT))->GetSafeHwnd())
		((CEdit *)GetDlgItem(IDC_HEX_TEXT))->MoveWindow(&HexEditRc);

	return TRUE;
}


int CProcessNetworkIODlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_DataList.SetResizingInfo(50,GetSystemMetrics(SM_CYSCREEN),m_hWnd,WM_DATA_LIST_RESIZING);
	return 0;
}


LRESULT CProcessNetworkIODlg::OnReadNetworkIoData(WPARAM wParam,LPARAM lParam)
{
	static NETWORK_API_DATA Head;
	NETWORK_DNSDATA  DnsData;
	static LPBYTE lpData=NULL;

	switch(wParam)
	{
	case NETWORK_NORMAL_DATA:
		theDatamgr.GetHeadData(&Head,sizeof(NETWORK_API_DATA));
		theDatamgr.GetData(&lpData,sizeof(NETWORK_API_DATA));
		InsertNormalData(&Head,lpData);
		break;
	case NETWORK_DNS_DATA:
		{
			theDatamgr.GetHeadData(&DnsData,sizeof(NETWORK_DNSDATA));
			m_DnsRecordData.insert(DNSRECORDDATAPAIR(DnsData.szIP,DnsData.szDNS));
		}
		break;
	}
	return TRUE;
}


BOOL CProcessNetworkIODlg::ProcessIsMonitoring(DWORD dwPID)
{
	LPHOOK_RECORD lpRecord = theHookManager.SelectHookRecord(dwPID);
	if(lpRecord == NULL)
		return FALSE;
	return lpRecord->bNetworkIoApiHook;
}

BOOL CProcessNetworkIODlg::MonitorTargetProcess(LPPROCESS_INFORMATION lpProcessInfo)
{
	HANDLE hWaitThread = NULL;
	if(!InjectProcess(lpProcessInfo,theApp.szDllPath,hWaitThread))
		return FALSE;

	if(hWaitThread) 
	{
		WaitForSingleObject(hWaitThread,500);
		CloseHandle(hWaitThread);
	}

	if(!theHookManager.HookNetworkIo(lpProcessInfo->dwProcessId))
		return FALSE;
	
	LPHOOK_RECORD lpRecord = theHookManager.SelectHookRecord(lpProcessInfo->dwProcessId);
	if(lpRecord == NULL)
	{
		HOOK_RECORD Record;
		ZeroMemory(&Record,sizeof(HOOK_RECORD));
		Record.dwPID = lpProcessInfo->dwProcessId;
		Record.bNetworkIoApiHook = TRUE;
		theHookManager.InsertHookRecord(&Record);
	}else
	{
		lpRecord->bNetworkIoApiHook = TRUE;
		theHookManager.InsertHookRecord(lpRecord);
	}

	return TRUE;
}

BOOL CProcessNetworkIODlg::MonitorTargetProcess(DWORD dwPID)
{
	HANDLE hWaitThread = NULL;
	if(!InjectProcess(dwPID,theApp.szDllPath,hWaitThread))
		return FALSE;

	if(hWaitThread) 
	{
		WaitForSingleObject(hWaitThread,500);
		CloseHandle(hWaitThread);
	}
	if(!theHookManager.HookNetworkIo(dwPID))
		return FALSE;

	LPHOOK_RECORD lpRecord = theHookManager.SelectHookRecord(dwPID);
	if(lpRecord == NULL)
	{
		HOOK_RECORD Record;
		ZeroMemory(&Record,sizeof(HOOK_RECORD));
		Record.dwPID = dwPID;
		Record.bNetworkIoApiHook = TRUE;
		theHookManager.InsertHookRecord(&Record);
	}else
	{
		lpRecord->bNetworkIoApiHook = TRUE;
		theHookManager.InsertHookRecord(lpRecord);
	}
	return TRUE;
}

BOOL CProcessNetworkIODlg::UnMonitorTargetProcess(DWORD dwPID)
{
	LPHOOK_RECORD lpRecord = theHookManager.SelectHookRecord(dwPID);
	if(lpRecord == NULL)
		return FALSE;

	if(!theHookManager.UnHookNetworkIo(dwPID))
		return FALSE;
	
	return theHookManager.DeleteHookRecord(lpRecord);
}


INT CProcessNetworkIODlg::GetIconIndex(DWORD dwPID,LPCTSTR szProcessPath)
{
	int nIndex= -1;
	ICONDATA::iterator iter;
	iter = m_IconData.find(dwPID);

	if(iter != m_IconData.end())
		return iter->second;

	SHFILEINFO shfi;
	::SHGetFileInfo(szProcessPath,
		FILE_ATTRIBUTE_NORMAL,
		&shfi,sizeof(shfi),
		SHGFI_SMALLICON|SHGFI_ICON |SHGFI_USEFILEATTRIBUTES|SHGFI_TYPENAME);

	nIndex = m_DataIamge.Add(shfi.hIcon);
	m_IconData.insert(ICONDATAPAIR(dwPID,nIndex));
	return nIndex;
}


void CProcessNetworkIODlg::InsertNormalData(LPNETWORK_API_DATA lpHead,LPBYTE lpContext)
{
	//过滤器
	if(!(lpHead->dwAPIFlags & g_dwNetworkIoSettingFlags))
		return;


	DWORD dwItem = m_DataList.GetItemCount();
	m_DataList.InsertItem(dwItem,NULL,GetIconIndex(lpHead->HeadData.dwPID,lpHead->HeadData.szModule));
	m_DataList.SetItemInt(dwItem,CN_ID,dwItem);
	m_DataList.SetItemData(dwItem,dwItem);

	m_DataList.SetItemTextA(dwItem,CN_API,lpHead->HeadData.szAPIName);
	m_DataList.SetItemFormatTextA(dwItem,CN_IP,"%s(%X)",lpHead->szIP,lpHead->Socket);

	switch(lpHead->nProtocol)
	{
	case NETWORK_PROTOCOL_TCP:
		m_DataList.SetItemFormatText(dwItem,CN_PROTOCOL,NETWORK_PROTOCOL_TCP_TEXT);
		break;
	case NETWORK_PROTOCOL_UDP:
		m_DataList.SetItemFormatText(dwItem,CN_PROTOCOL,NETWORK_PROTOCOL_UDP_TEXT);
		break;
	}
	
	//计算DSN
	DNSRECORDDATA::const_iterator iter;
	iter  = m_DnsRecordData.find(lpHead->szIP);
	if(iter != m_DnsRecordData.end())
		m_DataList.SetItemTextA(dwItem,CN_DNS,iter->second.c_str());


	m_DataList.SetItemInt(dwItem,CN_PORT,lpHead->dwPort);
	m_DataList.SetItemText(dwItem,CN_PROCESS_PATH,lpHead->HeadData.szModule);
	
	m_DataList.SetItemFormatText(dwItem,CN_PROCESS_ID,TEXT("%d %Xh"),lpHead->HeadData.dwPID,lpHead->HeadData.dwPID);
	
	m_DataList.SetItemFormatText(dwItem,CN_THREAD_ID,TEXT("%d %Xh %08X"),
		lpHead->HeadData.dwTID,lpHead->HeadData.dwTID,m_ThreadTool.GetThreadEntryAddress(lpHead->HeadData.dwTID));

	m_DataList.SetItemInt(dwItem,CN_DATA_SIZE,lpHead->dwDataSize);

	SYSTEMTIME st;
	GetLocalTime(&st);
	m_DataList.SetItemFormatText(dwItem,CN_TIME,TEXT("%04u-%02u-%02u %02u:%02u:%02u"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

	m_DataList.SetItemText(dwItem,CN_PROCESS_FILENAME,PathFindFileName(lpHead->HeadData.szModule));
	m_DataList.SetItemInt(dwItem,CN_RESULT,lpHead->nResult);

	CFormatMessage FormatMessage(lpHead->nLastError);
	m_DataList.SetItemText(dwItem,CN_LASTERROR,FormatMessage.szErrorText);


	if(lpHead->dwDataSize == 0 || lpHead->dwDataSize == -1 || lpHead->nResult == SOCKET_ERROR)
	{
		m_DataList.SetItemInt(dwItem,CN_IODATA_ADDRESS,(INT)0);
		return;
	}

	LPBYTE lpData = new BYTE[lpHead->dwDataSize + 2];
	if(lpData==NULL)
	{
		m_DataList.SetItemInt(dwItem,CN_IODATA_ADDRESS,(INT)0);
		return;
	}

	ZeroMemory(lpData,lpHead->dwDataSize+2);
	CopyMemory(lpData,lpContext,lpHead->dwDataSize);
	m_DataList.SetItemTextA(dwItem,CN_DATA,(LPCSTR)lpData);

	m_DataList.SetItemInt(dwItem,CN_IODATA_ADDRESS,(INT)lpData);
}


HBRUSH CProcessNetworkIODlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID()==IDC_HEX_TEXT)
		pDC->SetTextColor(RGB(0,0,255));
	
	if (pWnd->GetDlgCtrlID()==IDC_ASCII_TEXT)
		pDC->SetTextColor(RGB(0,0,128));

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CProcessNetworkIODlg::OnDrawColorForDataList(NMHDR *pNmHdr, LRESULT *pResult)
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
		COLORREF clrNewTextColor, clrNewBkColor;
		int  nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		CString szAPI = m_DataList.GetItemText(nItem,CN_API);
		CString szIoDataAddress = m_DataList.GetItemText(nItem,CN_IODATA_ADDRESS);


		INT  dwDataSize  = _ttoi((LPCTSTR)(m_DataList.GetItemText(nItem,CN_DATA_SIZE)));
		INT  nResult = _ttoi((LPCTSTR)(m_DataList.GetItemText(nItem,CN_RESULT)));

		//错误的记录 红色显示
		if(dwDataSize == 0 || dwDataSize == -1 ||  szIoDataAddress.IsEmpty() || nResult==SOCKET_ERROR){
			clrNewTextColor = RGB( 255,0,0);
		}
		else{

			//TCP 数据
			if(szAPI == "send" || szAPI == "WSASend" || szAPI == "recv" || szAPI == "WSARecv" )
				clrNewTextColor = RGB( 51,0,153);

			//UDP数据
			if(szAPI == "sendto" || szAPI == "WSASendTo" ||szAPI == "recvfrom" || szAPI == "WSARecvFrom" )
				clrNewTextColor = RGB(255,102,0);
		}

		clrNewBkColor = (nItem % 2)?RGB(228,232,239):RGB(255,255,255);
			
		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;
        *pResult = CDRF_DODEFAULT;
	}
}

void CProcessNetworkIODlg::OnDblclkDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION Pos = m_DataList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		m_nDataListCurrentSel = m_DataList.GetNextSelectedItem(Pos);
		m_DataList.SetCheck(m_nDataListCurrentSel,m_DataList.GetCheck(m_nDataListCurrentSel)?FALSE:TRUE);
	}
	*pResult = 0;
}


void CProcessNetworkIODlg::OnRclickDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMenu *pSub = m_DataListMenu.GetSubMenu(0);
	POINT point;
	GetCursorPos(&point);
	

	POSITION Pos = m_DataList.GetFirstSelectedItemPosition();

	UINT nCount = pSub->GetMenuItemCount();
	for(UINT i =4;i<nCount;i++)
		pSub->EnableMenuItem(i,MF_BYPOSITION| ((Pos==NULL)?(MF_DISABLED|MF_GRAYED):MF_ENABLED));

	if (Pos) {
		m_nDataListCurrentSel = m_DataList.GetNextSelectedItem(Pos);
		
		CString szText;
		szText = m_DataList.GetItemText(m_nDataListCurrentSel,CN_IP);
		pSub->EnableMenuItem(IDM_OPEN_IP,MF_BYCOMMAND| (szText.IsEmpty()?(MF_DISABLED|MF_GRAYED):MF_ENABLED));
		pSub->EnableMenuItem(IDM_BAIDU_SEARCH_IP,MF_BYCOMMAND| (szText.IsEmpty()?(MF_DISABLED|MF_GRAYED):MF_ENABLED));
		pSub->EnableMenuItem(IDM_GOOGLE_SEARCH_IP,MF_BYCOMMAND| (szText.IsEmpty()?(MF_DISABLED|MF_GRAYED):MF_ENABLED));

		szText = m_DataList.GetItemText(m_nDataListCurrentSel,CN_DNS);
		pSub->EnableMenuItem(IDM_OPEN_DNS,MF_BYCOMMAND| (szText.IsEmpty()?(MF_DISABLED|MF_GRAYED):MF_ENABLED));
		pSub->EnableMenuItem(IDM_BAIDU_SEARCH_DNS,MF_BYCOMMAND| (szText.IsEmpty()?(MF_DISABLED|MF_GRAYED):MF_ENABLED));
		pSub->EnableMenuItem(IDM_GOOGLE_SEARCH_DNS,MF_BYCOMMAND| (szText.IsEmpty()?(MF_DISABLED|MF_GRAYED):MF_ENABLED));


	}else{

	}


	pSub->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTBUTTON,point.x,point.y,this);
	*pResult = 0;
}


void CProcessNetworkIODlg::OnDeleteCurrent() 
{
	// TODO: Add your command handler code here
	POSITION Pos = m_DataList.GetFirstSelectedItemPosition();
	if(!Pos) return;

	INT nItem = m_DataList.GetNextSelectedItem(Pos);
	
	CString szDataAddress = m_DataList.GetItemText(nItem,CN_IODATA_ADDRESS);
	LPBYTE lpData = NULL;

	m_DataList.DeleteItem(nItem);

	if(!szDataAddress.IsEmpty() && (lpData = (LPBYTE) _ttoi(szDataAddress)))
		delete [] lpData;
}



void CProcessNetworkIODlg::OnDeleteSeleced() 
{
	// TODO: Add your command handler code here
	CString szDataAddress;
	LPBYTE lpData = NULL;

	DWORD dwCount = m_DataList.GetItemCount();
	for (DWORD i=0;i<dwCount;i++)
	{
		if(m_DataList.GetCheck(i))
		{
			lpData = NULL;
			szDataAddress = m_DataList.GetItemText(i,CN_IODATA_ADDRESS);
			if(!szDataAddress.IsEmpty() && (lpData = (LPBYTE) _ttoi(szDataAddress)))
				delete [] lpData;

			if(m_DataList.DeleteItem(i))
			{
				dwCount = m_DataList.GetItemCount();
				i--;
			}
		}
	}
}

void CProcessNetworkIODlg::OnDeleteInvalidData() 
{
	// TODO: Add your command handler code here
	DWORD dwCount = m_DataList.GetItemCount();
	TCHAR szResult[100]={NULL};
	TCHAR szAddress[100]={NULL};
	TCHAR szDataSize[100]={NULL};

	LPBYTE lpData = NULL;


	for (DWORD i=0;i<dwCount;i++)
	{
		szResult[0]=0x00;
		szAddress[0]=0x00;
		szDataSize[0]=0x00;

		lpData = NULL;

		m_DataList.GetItemText(i,CN_DATA_SIZE,szDataSize,sizeof(szDataSize));
		m_DataList.GetItemText(i,CN_RESULT,szResult,sizeof(szResult));
		
		if( (szResult[0] == '-' && szResult[1] == '1') || szDataSize[0]=='0' || szDataSize[0]=='-')
		{
			m_DataList.GetItemText(i,CN_IODATA_ADDRESS,szAddress,sizeof(szAddress));
			
			if( ( lpData = (LPBYTE)_ttoi(szAddress)))
				delete [] lpData;

			if(m_DataList.DeleteItem(i)) {
				dwCount = m_DataList.GetItemCount();
				i--;
			}
		}
	}
}




void CProcessNetworkIODlg::OnSelectAll() 
{
	// TODO: Add your command handler code here
	DWORD dwItem = m_DataList.GetItemCount();
	for (DWORD i=0;i<dwItem;i++)
		m_DataList.SetCheck(i);
}



void CProcessNetworkIODlg::OnSelectReverseAll() 
{
	// TODO: Add your command handler code here
	DWORD dwItem = m_DataList.GetItemCount();
	for (DWORD i=0;i<dwItem;i++)
		m_DataList.SetCheck(i,m_DataList.GetCheck(i)?FALSE:TRUE);
}


void CProcessNetworkIODlg::OnCancelAll() 
{
	// TODO: Add your command handler code here
	DWORD dwItem = m_DataList.GetItemCount();
	for (DWORD i=0;i<dwItem;i++)
		m_DataList.SetCheck(i,FALSE);
}

void CProcessNetworkIODlg::InsertStirngToBar(CListCtrl * pList,LPCOLUMN_DATA pData,DWORD dwSize,INT nCurSel)
{
	//将相关信息显示到工具条
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;
	INT nCurSelItem = pList->GetNextSelectedItem(pos);
	
	CString szItemText;
	((CMainFrame *)AfxGetApp()->GetMainWnd())->m_PublicTextBar.DeleteAllString();
	
	for(INT i = 0;i < dwSize ;i++)
	{
		szItemText = pList->GetItemText(nCurSelItem,i);
		
		((CMainFrame *)AfxGetApp()->GetMainWnd())->m_PublicTextBar.InsertString(TEXT("%s:%s"),
			pData[i].szText,(LPCTSTR)szItemText);
	}
	
	((CMainFrame *)AfxGetApp()->GetMainWnd())->m_PublicTextBar.SetCurSel(nCurSel);
}


void CProcessNetworkIODlg::OnClickDataList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION Pos = m_DataList.GetFirstSelectedItemPosition();
	if (Pos ==NULL)
		return;

	m_nDataListCurrentSel = m_DataList.GetNextSelectedItem(Pos);
	InsertStirngToBar(&m_DataList,g_DataListCol,sizeof(g_DataListCol)/sizeof(COLUMN_DATA),CN_LASTERROR);
	ShowNetworkIoData();
	*pResult = 0;
}



void CProcessNetworkIODlg::ShowNetworkIoData()
{
	//先清空显示
	::SetDlgItemTextA(m_hWnd,IDC_HEX_TEXT,"");
	::SetDlgItemTextA(m_hWnd,IDC_ASCII_TEXT,"");

	//根本没有选中.
	if(m_nDataListCurrentSel == -1)
		return;


	DWORD dwItemCount = m_DataList.GetItemCount();
	if(dwItemCount == 0 || m_nDataListCurrentSel > dwItemCount)
		return;

	CString szDataAddress = m_DataList.GetItemText(m_nDataListCurrentSel,CN_IODATA_ADDRESS);
	if(szDataAddress.IsEmpty())
	{
		SetDlgItemText(IDC_ASCII_TEXT,TEXT("该数据是无效的，因为数据地址是空的。"));
		return;
	}

	LPBYTE lpData = (LPBYTE) _ttoi(szDataAddress);
	if((INT)lpData <=0)
	{
		SetDlgItemText(IDC_ASCII_TEXT,TEXT("该数据是无效的，因为数据地址小于等于0。"));
		return;
	}

	DWORD dwDataSize = _ttoi((LPCTSTR)(m_DataList.GetItemText(m_nDataListCurrentSel,CN_DATA_SIZE)));
	if(dwDataSize <=0)
	{
		SetDlgItemText(IDC_ASCII_TEXT,TEXT("该数据是无效的，因为数据大小小于等于0"));
		return;
	}

	CHAR * lpszHexText  = new CHAR[dwDataSize * 4];
	if(lpszHexText == NULL)
	{
		CFormatMessage FormatMessage;
		SetDlgItemText(IDC_ASCII_TEXT,FormatMessage.szErrorText);
		return;
	}


	CHAR * lpszAsciiText  = new CHAR[dwDataSize * 2];
	if(lpszAsciiText == NULL)
	{
		CFormatMessage FormatMessage;
		SetDlgItemText(IDC_ASCII_TEXT,FormatMessage.szErrorText);
		delete [] lpszHexText;
		return;
	}
	
	ZeroMemory(lpszHexText,dwDataSize * 4);
	ZeroMemory(lpszAsciiText,dwDataSize * 2);

	//格式化数据
	CHAR  szTempBuffer[10];

	for (DWORD i=0;i<dwDataSize;i++)
	{
		StringCbPrintfA(szTempBuffer,sizeof(szTempBuffer),"%02X ",(BYTE)lpData[i]);
		StringCbCatA(lpszHexText,dwDataSize * 4,szTempBuffer);

		if((BYTE)lpData[i])
			lpszAsciiText[i]=(CHAR)lpData[i];
		else
			lpszAsciiText[i]='.';
	}

	::SetDlgItemTextA(m_hWnd,IDC_HEX_TEXT,lpszHexText);
	::SetDlgItemTextA(m_hWnd,IDC_ASCII_TEXT,lpszAsciiText);
	delete [] lpszHexText;
	delete [] lpszAsciiText;
}


void CProcessNetworkIODlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CString szDataAddress;
	LPBYTE lpData = NULL;
	DWORD dwCount = m_DataList.GetItemCount();

	for (DWORD i=0;i<dwCount;i++)
	{
		lpData = NULL;
		szDataAddress = m_DataList.GetItemText(i,CN_IODATA_ADDRESS);
		if(!szDataAddress.IsEmpty() && (lpData = (LPBYTE) _ttoi(szDataAddress)))
			delete [] lpData;
	}
	CDialog::OnClose();
}

void CProcessNetworkIODlg::OnOpenDns() 
{
	// TODO: Add your command handler code here
	CString szDNS=m_DataList.GetItemText(m_nDataListCurrentSel,CN_DNS);
	CString szPort = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PORT);

	CString szURL;
	szURL = TEXT("http://");
	szURL+=szDNS;
	szURL+=":";
	szURL+=szPort;
	OpenUrl(szURL);
}

void CProcessNetworkIODlg::OnOpenIp() 
{
	// TODO: Add your command handler code here
	CString szIP=m_DataList.GetItemText(m_nDataListCurrentSel,CN_IP);
	INT nStart = szIP.Find(TEXT("("));
	szIP.Delete(nStart,szIP.GetLength() - nStart);
	CString szPort = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PORT);
	CString szURL;
	szURL = TEXT("http://");
	szURL+=szIP;
	szURL+=":";
	szURL+=szPort;
	OpenUrl(szURL);
}


void CProcessNetworkIODlg::OnGoogleSearchDns() 
{
	// TODO: Add your command handler code here
	CString szDNS=m_DataList.GetItemText(m_nDataListCurrentSel,CN_DNS);
	CString szPort = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PORT);

	CString szURL;
	szURL = TEXT("http://www.google.com/search?q=");
	szURL+=szDNS;
	szURL+=":";
	szURL+=szPort;
	OpenUrl(szURL);
}

void CProcessNetworkIODlg::OnGoogleSearchIp() 
{
	// TODO: Add your command handler code here
	CString szIP=m_DataList.GetItemText(m_nDataListCurrentSel,CN_IP);
	CString szPort = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PORT);
	INT nStart = szIP.Find(TEXT("("));
	szIP.Delete(nStart,szIP.GetLength() - nStart);


	CString szURL;
	szURL = TEXT("http://www.google.com/search?q=");
	szURL+=szIP;
	szURL+=":";
	szURL+=szPort;
	OpenUrl(szURL);
}

void CProcessNetworkIODlg::OnBaiduSearchDns() 
{
	// TODO: Add your command handler code here
	CString szDNS=m_DataList.GetItemText(m_nDataListCurrentSel,CN_DNS);
	CString szPort = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PORT);
	
	CString szURL;
	szURL = TEXT("http://www.baidu.com/s?wd=");
	szURL+=szDNS;
	szURL+=":";
	szURL+=szPort;
	OpenUrl(szURL);
}

void CProcessNetworkIODlg::OnBaiduSearchIp() 
{
	// TODO: Add your command handler code here
	CString szIP=m_DataList.GetItemText(m_nDataListCurrentSel,CN_IP);
	CString szPort = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PORT);
	INT nStart = szIP.Find(TEXT("("));
	szIP.Delete(nStart,szIP.GetLength() - nStart);


	CString szURL;
	szURL = TEXT("http://www.baidu.com/s?wd=");
	szURL+=szIP;
	szURL+=":";
	szURL+=szPort;
	OpenUrl(szURL);
}



void CProcessNetworkIODlg::OnPositioningFile() 
{
	// TODO: Add your command handler code here
	CString szPath = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_PATH);
	CString szDwPath=TEXT("/select,");
	szDwPath+=szPath;
	ShellExecute(NULL,NULL,TEXT("explorer"),szDwPath.GetBuffer(0),NULL,SW_SHOW); 
}

void CProcessNetworkIODlg::OnGoogleSearch() 
{
	// TODO: Add your command handler code here
	CString szPath = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_PATH);
	CString szURL;
	szURL.Format(TEXT("http://www.google.com/search?q=%s"),szPath);
	OpenUrl(szURL);
}

void CProcessNetworkIODlg::OnBaiduSearch() 
{
	// TODO: Add your command handler code here
	CString szPath = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_PATH);

	CString szURL;
	szURL.Format(TEXT("http://www.baidu.com/s?wd=%s"),szPath);
	OpenUrl(szURL);
}

void CProcessNetworkIODlg::OnAttributeFile() 
{
	// TODO: Add your command handler code here
	CString szPath = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_PATH);
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

void CProcessNetworkIODlg::OnOnlineVirusScan() 
{
	// TODO: Add your command handler code here
	OpenUrl(TEXT("http://www.virscan.org/"));
}

void CProcessNetworkIODlg::OnSetting() 
{
	// TODO: Add your command handler code here
	CMonitorNetworkIoSettingDlg MonitorNetworkIoSettingDlg;
	MonitorNetworkIoSettingDlg.DoModal();
}



void CProcessNetworkIODlg::OnMonitorFile() 
{
	// TODO: Add your command handler code here
	CMonitorFileDlg MonikerFileDlg;
	MonikerFileDlg.DoModal();
}

void CProcessNetworkIODlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	TCHAR szFilePath[MAX_PATH]={NULL};
	DragQueryFile(hDropInfo,0,szFilePath,MAX_PATH);
	DragFinish(hDropInfo);

	CMonitorFileDlg MonikerFileDlg;
	MonikerFileDlg.UpdateParameter(szFilePath);
	MonikerFileDlg.DoModal();
}


void CProcessNetworkIODlg::OnTermenaterProcess() 
{
	// TODO: Add your command handler code here
	CString szPID = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_ID);

	CProcessTool ProcessTool;
	if(ProcessTool.TerminateProcess(_ttoi(szPID)))
	{
		MessageBox(TEXT("结束进程成功！"),TEXT("提示"),MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		CFormatMessage FormatMessage;
		CString szText;
		szText.Format(TEXT("结束进程[%s]失败\r\n错误代码:[%d]\r\n错误原因:%s"),
			(LPCTSTR)szPID,FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(szText,TEXT("提示"),MB_OK|MB_ICONERROR);
	}

}


void CProcessNetworkIODlg::OnSuspendProcess() 
{
	// TODO: Add your command handler code here
	CString szPID = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_ID);
	CThreadTool ThreadTool;
	ThreadTool.Create();

	if(ThreadTool.SusppedProcess(_ttoi(szPID)))
	{
		MessageBox(TEXT("暂停进程运行成功！"),TEXT("提示"),MB_OK|MB_ICONINFORMATION);
	}else
	{
		CFormatMessage FormatMessage;
		CString szText;
		szText.Format(TEXT("暂停进程[%s]运行失败\r\n错误代码:[%d]\r\n错误原因:%s"),
			(LPCTSTR)szPID,FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(szText,TEXT("提示"),MB_OK|MB_ICONERROR);
	}

}

void CProcessNetworkIODlg::OnResumeProcess() 
{
	// TODO: Add your command handler code here
	CString szPID = m_DataList.GetItemText(m_nDataListCurrentSel,CN_PROCESS_ID);
	CThreadTool ThreadTool;
	ThreadTool.Create();

	if(ThreadTool.ResumeProcess(_ttoi(szPID)))
	{
		MessageBox(TEXT("恢复进程运行成功！"),TEXT("提示"),MB_OK|MB_ICONINFORMATION);
	}else
	{
		CFormatMessage FormatMessage;
		CString szText;
		szText.Format(TEXT("恢复进程[%s]运行失败\r\n错误代码:[%d]\r\n错误原因:%s"),
			(LPCTSTR)szPID,FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(szText,TEXT("提示"),MB_OK|MB_ICONERROR);
	}
}


void CProcessNetworkIODlg::OnFilter() 
{
	// TODO: Add your command handler code here
	CCMListCtrlFilterDlg CMListCtrlFilterDlg;
	CMListCtrlFilterDlg.m_lpCMListCtrl = &m_DataList;
	CMListCtrlFilterDlg.m_lpColData = g_DataListCol;
	CMListCtrlFilterDlg.m_dwColDataSize = sizeof(g_DataListCol)/sizeof(COLUMN_DATA);
	CMListCtrlFilterDlg.DoModal();
}


