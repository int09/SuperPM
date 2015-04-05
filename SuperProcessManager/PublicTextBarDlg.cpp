// PublicTextBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "superprocessmanager.h"
#include "PublicTextBarDlg.h"
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPublicTextBarDlg dialog


CPublicTextBarDlg::CPublicTextBarDlg()
{
	//{{AFX_DATA_INIT(CPublicTextBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPublicTextBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPublicTextBarDlg)
	DDX_Control(pDX, IDC_COMBO_ADDRESS, m_ComboBox);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CPublicTextBarDlg, CDialogBar)
	//{{AFX_MSG_MAP(CPublicTextBarDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPublicTextBarDlg message handlers
LRESULT CPublicTextBarDlg::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam,lParam);

	UpdateData(FALSE);
	return bRet;
}

BOOL CPublicTextBarDlg::DeleteAllString()
{
	/*INT dwCount = m_ComboBox.GetCount();
	for(INT i =0;i<dwCount;i++)
		m_ComboBox.DeleteString(i);*/

	/*while(m_ComboBox.GetCount())
		m_ComboBox.DeleteString(0);*/

	m_ComboBox.ResetContent();

	return TRUE;
}

void CPublicTextBarDlg::InsertString(LPCTSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	TCHAR szParamText[4096]={NULL};
	StringCbVPrintf(szParamText,sizeof(szParamText),szFormat,vagrc);
	va_end(vagrc);
	m_ComboBox.AddString(szParamText);
}

BOOL CPublicTextBarDlg::GetCurrentText(CString & szText)
{
	if(CB_ERR  == m_ComboBox.GetCurSel())
		return FALSE;

	m_ComboBox.GetWindowText(szText);
	return TRUE;
}