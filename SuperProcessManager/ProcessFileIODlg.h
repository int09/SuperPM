#if !defined(AFX_PROCESSFILEIODLG_H__B42314EE_C045_424F_B4F1_80DD7C462B45__INCLUDED_)
#define AFX_PROCESSFILEIODLG_H__B42314EE_C045_424F_B4F1_80DD7C462B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessFileIODlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessFileIODlg dialog
#include "MSortListCtrl.h"

class CProcessFileIODlg : public CDialog
{
// Construction
public:
	CProcessFileIODlg(CWnd* pParent = NULL);   // standard constructor
	BOOL ProcessIsMonitoring(DWORD dwPID);
	BOOL MonitorTargetProcess(DWORD dwPID);
	BOOL InitDataList();
// Dialog Data
	//{{AFX_DATA(CProcessFileIODlg)
	enum { IDD = IDD_PROCESS_FILEIO_DLG };
	CMSortListCtrl	m_DataList;
	CImageList		m_DataIamge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessFileIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessFileIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnReadFileIoData(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSFILEIODLG_H__B42314EE_C045_424F_B4F1_80DD7C462B45__INCLUDED_)
