// ProcessRegIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProcessManager.h"
#include "ProcessRegIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessRegIODlg dialog


CProcessRegIODlg::CProcessRegIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessRegIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessRegIODlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcessRegIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessRegIODlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessRegIODlg, CDialog)
	//{{AFX_MSG_MAP(CProcessRegIODlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessRegIODlg message handlers
BOOL CProcessRegIODlg::ProcessIsMonitoring(DWORD dwPID)
{
	
	return FALSE;
}


BOOL CProcessRegIODlg::MonitorTargetProcess(DWORD dwPID)
{

	return TRUE;
}
