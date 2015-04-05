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

	m_hMutex = CreateMutex(NULL,FALSE,TEXT("�������̹�����"));
	if(ERROR_ALREADY_EXISTS == GetLastError())
	{
		MessageBox(NULL,TEXT("��Ǹ�������ͬʱֻ������һ��"),TEXT("��ʾ"),MB_OK|MB_ICONINFORMATION);
		HWND hWnd=::FindWindow(TEXT("�������̹����� 2012-11-30 V1.0"),NULL);
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
		szText.Format(TEXT("�������:[%d]\r\n����ԭ��:%s\r\n����㲻�������壬����ϵ���ߡ�"),FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(NULL,szText,TEXT("�������̹�������ʼ��ʧ�ܣ�"),MB_ICONERROR|MB_OK);
		ExitProcess(0);
	}

	if(!theHookManager.Create(szDllPath))
	{
		CFormatMessage FormatMessage;
		CString szMsg;
		szMsg.Format(TEXT("���غ���DLLʧ�ܣ������޷��������У�\r\n�������:[%d]\r\n����ԭ��:%s\r\n"),FormatMessage.GetLastError(),
			FormatMessage.szErrorText);
		MessageBox(NULL,szMsg,TEXT("���ش���"),MB_OK|MB_ICONERROR);
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
		"�޾��ļ������޾���׷��\t||BY:Coding���ˡ�\r|| \
		���Դ˳��򣬼����Ҷ���¶�Ŭ��Coding�����ӡ�|| \
		���������VC6.0(UNICODE)��д��|| \
		��д�ó������Ƕ���ѧϰC,C++,ASM,MFC,STL��һ�������ɡ�|| \
		��������ȫ��ѣ����������ɸ��ƣ��������޸ġ�|| \
		�ڱ�д�����������ӣ������ľ��������⼸���������ʱ��(���� & ��ҹ)��||\
		�ڴˣ����ĵĶ��������Լ��ոճ����Ķ���˵��:|| \
		�Բ���||\
		��û�кúõ������ǡ�С�һ��㳤��ɡ�||\
		���ţ����ӣ��һ������ǹ��Ϻ����ӵģ��Ұ����ǡ�\r||\
		���ף���ʹ����졣лл���ǡ�2012-11-30||\
		а���Ʈ��һ�����r(������)�q||\
		�н������д�����ƣ�QQ:1159863174��\r|||"),
		'|');

	m_MySay.SetBkImage(IDB_ABOUT_BG);
	m_MySay.SetTransparent();
	m_MySay.SetSpeed(1);
	m_MySay.StartScrolling();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
