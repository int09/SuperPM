// MonitorFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "superprocessmanager.h"
#include "MonitorFileDlg.h"
#include "MainFrm.h"
#include "SuperProcessManagerDoc.h"
#include "SuperProcessManagerView.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorFileDlg dialog


CMonitorFileDlg::CMonitorFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitorFileDlg)
	m_szFilePath = _T("");
	m_szWorkDir = _T("");
	m_szDesktop = _T("");
	m_szCommanLine = _T("");
	m_bMonitorFileIo = TRUE;
	m_bMonitorNetworkIo = TRUE;
	m_bMonitorRegIo = TRUE;
	//}}AFX_DATA_INIT

	m_szDesktop=TEXT("default");
}


void CMonitorFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorFileDlg)
	DDX_Text(pDX, IDC_FILE_PATH, m_szFilePath);
	DDV_MaxChars(pDX, m_szFilePath, 256);
	DDX_Text(pDX, IDC_WORK_DIRECTORY, m_szWorkDir);
	DDV_MaxChars(pDX, m_szWorkDir, 256);
	DDX_Text(pDX, IDC_DESKTOP, m_szDesktop);
	DDX_Text(pDX, IDC_COMMAN_LINE, m_szCommanLine);
	DDX_Check(pDX, IDC_MONITOR_FILEIO, m_bMonitorFileIo);
	DDX_Check(pDX, IDC_MONITOR_NETWORKIO, m_bMonitorNetworkIo);
	DDX_Check(pDX, IDC_MONITOR_REGIO, m_bMonitorRegIo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonitorFileDlg, CDialog)
	//{{AFX_MSG_MAP(CMonitorFileDlg)
	ON_BN_CLICKED(IDC_BROWSER_FILE, OnBrowserFile)
	ON_BN_CLICKED(IDC_BROWSER_DIR, OnBrowserDir)
	ON_BN_CLICKED(IDC_MONITOR_TAGERT, OnMonitorTagert)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorFileDlg message handlers


void CMonitorFileDlg::OnBrowserFile() 
{
	// TODO: Add your control notification handler code here
	TCHAR  szFilePath[MAX_PATH]={NULL};

	OPENFILENAME OpenInfo;
	ZeroMemory(&OpenInfo,sizeof(OPENFILENAME));
	OpenInfo.lStructSize = sizeof(OPENFILENAME);
	OpenInfo.hwndOwner = m_hWnd;
	OpenInfo.lpstrFile = szFilePath;
	OpenInfo.nMaxFile = sizeof(szFilePath);
	OpenInfo.lpstrFilter = TEXT("可执行文件(.exe)\0*.exe\0所有文件\0*.*");
	OpenInfo.lpstrTitle=TEXT("请选择要监视的文件");
	OpenInfo.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

	if(GetOpenFileName(&OpenInfo))
	{
		UpdateData(TRUE);
		UpdateParameter(szFilePath);
		UpdateData(FALSE);
	}
}





BOOL CMonitorFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton *)GetDlgItem(IDC_SHOW_RUN))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_SWITCH_NETWORKMONITRO))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMonitorFileDlg::OnBrowserDir() 
{
	// TODO: Add your control notification handler code here
	TCHAR szDir[MAX_PATH]={NULL};
	BROWSEINFO BrowseInfo;
	LPITEMIDLIST pIDList;

	ZeroMemory(&BrowseInfo,sizeof(BROWSEINFO));
	BrowseInfo.hwndOwner = m_hWnd;
	BrowseInfo.pszDisplayName = szDir;
	BrowseInfo.lpszTitle = TEXT("请选择工作目录");
	
	if((pIDList = SHBrowseForFolder(&BrowseInfo)))
	{
		UpdateData(TRUE);
		SHGetPathFromIDList(pIDList, szDir);
		m_szWorkDir = szDir;
		UpdateData(FALSE);
	}
}



void CMonitorFileDlg::OnMonitorTagert() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_szFilePath.IsEmpty())
		return;

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.lpDesktop = m_szDesktop.GetBuffer(0);
	si.wShowWindow = ((CButton *)GetDlgItem(IDC_SHOW_RUN))->GetCheck()?SW_SHOW:SW_HIDE;
	


	PROCESS_INFORMATION pi;

	if(CreateProcess(m_szFilePath,m_szCommanLine.GetBuffer(0),
		NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE|CREATE_SUSPENDED,NULL,m_szWorkDir,&si,&pi))
	{
		
		CSuperProcessManagerView * pView;
		pView = (CSuperProcessManagerView* )(((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView());
		CString szError;
		
		if(m_bMonitorNetworkIo)
		{
			if(!pView->MonitorProcessNetworkIo(&pi))
			{
				CFormatMessage FormatMessage;
				szError.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),
					FormatMessage.GetLastError(),FormatMessage.szErrorText);
				MessageBox(szError,TEXT("监视进程网络数据失败!"),MB_OK|MB_ICONERROR);
			}
			if(((CButton *)GetDlgItem(IDC_SWITCH_NETWORKMONITRO))->GetCheck())
				pView->SwitchWindow(TAB_PROCESS_NETWORK_IO);
		}

		/*if(m_bMonitorFileIo)
		{
			if(!pView->MonitorProcessFileIo(&pi))
			{
				CFormatMessage FormatMessage;
				szError.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),
					FormatMessage.GetLastError(),FormatMessage.szErrorText);
				MessageBox(szError,TEXT("监视文件操作失败!"),MB_OK|MB_ICONERROR);
			}
			if(((CButton *)GetDlgItem(IDC_SWITCH_FILEMONITRO))->GetCheck())
				pView->SwitchWindow(TAB_PROCESS_FILE_IO);
		}

		if(m_bMonitorRegIo)
		{
			if(!pView->MonitorProcessRegIo(&pi))
			{
				CFormatMessage FormatMessage;
				szError.Format(TEXT("错误代码:[%d]\r\n错误原因:%s\r\n"),
					FormatMessage.GetLastError(),FormatMessage.szErrorText);
				MessageBox(szError,TEXT("监视进程注册表操作失败!"),MB_OK|MB_ICONERROR);
			}
			if(((CButton *)GetDlgItem(IDC_SWITCH_REGMONITRO))->GetCheck())
				pView->SwitchWindow(TAB_PROCESS_REG_IO);
		}*/

		ResumeThread(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}else
	{
		CFormatMessage FormatMessage;
		CString szText;
		szText.Format(TEXT("错误代码[%d]\r\n错误原因:%s"),
			FormatMessage.GetLastError(),FormatMessage.szErrorText);
		MessageBox(szText,TEXT("运行程序失败！"),MB_OK|MB_ICONERROR);
	}
	m_szDesktop.ReleaseBuffer();
	m_szCommanLine.ReleaseBuffer();
}


void CMonitorFileDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	TCHAR szFilePath[MAX_PATH]={NULL};
	DragQueryFile(hDropInfo,0,szFilePath,MAX_PATH);
	DragFinish(hDropInfo);
	UpdateData(TRUE);
	UpdateParameter(szFilePath);
	UpdateData(FALSE);
}


void CMonitorFileDlg::UpdateParameter(LPCTSTR szFilePath)
{
	m_szFilePath = szFilePath;

	if(m_szCommanLine.IsEmpty())
	{
		m_szCommanLine=TEXT("\"");
		m_szCommanLine+= m_szFilePath;
		m_szCommanLine+=TEXT("\"");
	}
	if(m_szWorkDir.IsEmpty())
	{
		m_szWorkDir = m_szFilePath;
		PathRemoveFileSpec(m_szWorkDir.GetBuffer(0));
		m_szWorkDir.ReleaseBuffer();
	}
}
