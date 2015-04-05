#if !defined(AFX_CMLISTCTRLFILTERDLG_H__616372B2_CEF3_4602_BB85_C1F8FA456350__INCLUDED_)
#define AFX_CMLISTCTRLFILTERDLG_H__616372B2_CEF3_4602_BB85_C1F8FA456350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMListCtrlFilterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMListCtrlFilterDlg dialog

#include "MSortListCtrl.h"

class CCMListCtrlFilterDlg : public CDialog
{
// Construction
public:
	DWORD StartFilter(INT nColumn,INT nType,INT nRule,LPCTSTR szParam,INT nOperate);
	CCMListCtrlFilterDlg(CWnd* pParent = NULL);   // standard constructor
	CMSortListCtrl * m_lpCMListCtrl;
	LPCOLUMN_DATA	 m_lpColData;
	DWORD			 m_dwColDataSize;
	void ShowRule();
// Dialog Data
	//{{AFX_DATA(CCMListCtrlFilterDlg)
	enum { IDD = IDD_LISTCTRL_FILTER_DLG };
	CStatic	m_Result;
	CComboBox	m_Rule;
	CComboBox	m_Column;
	CString	m_szParameter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMListCtrlFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMListCtrlFilterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboColumn();
	afx_msg void OnOperateSelect();
	afx_msg void OnOperateDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLISTCTRLFILTERDLG_H__616372B2_CEF3_4602_BB85_C1F8FA456350__INCLUDED_)
