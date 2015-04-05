// RunlogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "superprocessmanager.h"
#include "RunlogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunlogDlg dialog


CRunlogDlg::CRunlogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunlogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunlogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRunlogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunlogDlg)
	DDX_Control(pDX, IDC_LOG_LIST, m_TextList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunlogDlg, CDialog)
	//{{AFX_MSG_MAP(CRunlogDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunlogDlg message handlers

void CRunlogDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_TextList.GetSafeHwnd())
		m_TextList.MoveWindow(0,0,cx,cy);
}


LRESULT CRunlogDlg::OnOutputLogString(WPARAM wParam,LPARAM lParam)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString szText;
	szText.Format(TEXT("%02u:%02u:%02u>>"),st.wHour,st.wMinute,st.wSecond);
	szText+=(LPCTSTR)wParam;
	m_TextList.InsertString(m_TextList.GetCount(),szText);
	return TRUE;
}
