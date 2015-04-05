// MonitorNetworkIoSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "superprocessmanager.h"
#include "MonitorNetworkIoSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorNetworkIoSettingDlg dialog


DWORD   g_dwNetworkIoSettingFlags = HOOK_SEND_API | HOOK_RECV_API | HOOK_SENDTO_API| HOOK_RECVFROM_API| \
									HOOK_WSASEND_API|HOOK_WSARECV_API|HOOK_WSASENDTO_API|HOOK_WSARECVFROM_API;




CMonitorNetworkIoSettingDlg::CMonitorNetworkIoSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorNetworkIoSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitorNetworkIoSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonitorNetworkIoSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorNetworkIoSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonitorNetworkIoSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CMonitorNetworkIoSettingDlg)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorNetworkIoSettingDlg message handlers

BOOL CMonitorNetworkIoSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton *)GetDlgItem(IDC_HOOK_SEND))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_SEND_API );
	((CButton *)GetDlgItem(IDC_HOOK_RECV))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_RECV_API );

	((CButton *)GetDlgItem(IDC_HOOK_SENDTO))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_SENDTO_API );
	((CButton *)GetDlgItem(IDC_HOOK_RECVFROM))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_RECVFROM_API );

	((CButton *)GetDlgItem(IDC_HOOK_WSASEND))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_WSASEND_API );
	((CButton *)GetDlgItem(IDC_HOOK_WSARECV))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_WSARECV_API );

	((CButton *)GetDlgItem(IDC_HOOK_WSASENDTO))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_WSASENDTO_API );
	((CButton *)GetDlgItem(IDC_HOOK_WSARECVFROM))->SetCheck( g_dwNetworkIoSettingFlags & HOOK_WSARECVFROM_API );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonitorNetworkIoSettingDlg::OnDone() 
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE,NULL,NULL);
}

void CMonitorNetworkIoSettingDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	g_dwNetworkIoSettingFlags= 0;

	if(((CButton *)GetDlgItem(IDC_HOOK_SEND))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_SEND_API;

	if(((CButton *)GetDlgItem(IDC_HOOK_RECV))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_RECV_API;
	
	if(((CButton *)GetDlgItem(IDC_HOOK_SENDTO))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_SENDTO_API;

	if(((CButton *)GetDlgItem(IDC_HOOK_RECVFROM))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_RECVFROM_API;

	

	if(((CButton *)GetDlgItem(IDC_HOOK_WSASEND))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_WSASEND_API;

	if(((CButton *)GetDlgItem(IDC_HOOK_WSARECV))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_WSARECV_API;

	if(((CButton *)GetDlgItem(IDC_HOOK_WSASENDTO))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_WSASENDTO_API;

	if(((CButton *)GetDlgItem(IDC_HOOK_WSARECVFROM))->GetCheck())
		g_dwNetworkIoSettingFlags |= HOOK_WSARECVFROM_API;


	CDialog::OnClose();
}
