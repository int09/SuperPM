// SuperProcessManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SuperProcessManager.h"

#include "MainFrm.h"
#include "SuperProcessManagerDoc.h"
#include "SuperProcessManagerView.h"
#include "Common/DataManager.h"
#include "HookManager.h"

#include <shlwapi.h>
#include <strsafe.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerApp

BEGIN_MESSAGE_MAP(CSuperProcessManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CSuperProcessManagerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerApp construction

/////////////////////////////////////////////////////////////////////////////
// The one and only CSuperProcessManagerApp object

CSuperProcessManagerApp theApp;
CDataManager			theDatamgr;
CHookManager			theHookManager;

CSuperProcessManagerApp::CSuperProcessManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	GetModuleFileName(NULL,szDllPath,sizeof(szDllPath));
	PathRemoveFileSpec(szDllPath);
	StringCbCat(szDllPath,sizeof(szDllPath),TEXT("\\CMHook.dll"));

	m_hMutex = CreateMutex(NULL,FALSE,TEXT("超级进程管理器"));
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		MessageBox(NULL,TEXT("抱歉，本软件同时只能运行一个"),TEXT("提示"),MB_OK|MB_ICONINFORMATION);
		HWND hWnd=::FindWindow(TEXT("超级进程管理器 2012-11-30 V1.0"),NULL);
		ShowWindow(hWnd,SW_RESTORE);
		SetForegroundWindow(hWnd);
		ExitProcess(0);
	}
}




/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerApp initialization

BOOL CSuperProcessManagerApp::InitInstance()
{
	if(!theDatamgr.Create())
	{
		CFormatMessage FormatMessage;
		CString szText;
		szText.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n如果你不懂错误含义，请联系作者。"),FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(NULL,szText,TEXT("超级进程管理器初始化失败！"),MB_ICONERROR|MB_OK);
		ExitProcess(0);
	}

	if(!theHookManager.Create(szDllPath))
	{
		CFormatMessage FormatMessage;
		CString szMsg;
		szMsg.Format(TEXT("加载核心DLL失败！程序无法继续运行！\r\n错误代码:[%d]\r\n错误原因:%s\r\n"),FormatMessage.GetLastError(),
			FormatMessage.szErrorText);
		MessageBox(NULL,szMsg,TEXT("严重错误！"),MB_OK|MB_ICONERROR);
		ExitProcess(0);
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(APP_KEY);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSuperProcessManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSuperProcessManagerView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#include "UICommon/CreditStatic.h"


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CCreditStatic	m_MySay;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HBITMAP m_hBgBitMap;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_MYSAY, m_MySay);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSuperProcessManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_MySay.SetCredits(TEXT(
		"无尽的技术，无尽的追求\t||BY:Coding狂人。\r|| \
		仅以此程序，纪念我多年孤独努力Coding的日子。|| \
		本程序采用VC6.0(UNICODE)编写。|| \
		编写该程序，算是对我学习C,C++,ASM,MFC,STL的一个交代吧。|| \
		本程序完全免费，您可以自由复制，传播，修改。|| \
		在编写本程序里日子，几乎耗尽了作者这几月里的所有时间(白天 & 黑夜)。||\
		在此，真心的对我老婆以及刚刚出生的儿子说句:|| \
		对不起！||\
		我没有好好的陪你们。小家伙，快点长大吧。||\
		老婆，儿子，我会让你们过上好日子的，我爱你们。\r||\
		最后，祝大家使用愉快。谢谢你们。2012-11-30||\
		邪恶的飘过一个广告╮(￣▽￣)╭||\
		承接软件编写，定制，QQ:1159863174。\r|||"),
		'|');

	m_MySay.SetBkImage(IDB_ABOUT_BG);
	m_MySay.SetTransparent();
	m_MySay.SetSpeed(1);
	m_MySay.StartScrolling();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
