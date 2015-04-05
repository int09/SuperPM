#if !defined(AFX_PROCESSREGIODLG_H__FDC1DFED_5C97_457A_9375_E75A23853A55__INCLUDED_)
#define AFX_PROCESSREGIODLG_H__FDC1DFED_5C97_457A_9375_E75A23853A55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessRegIODlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessRegIODlg dialog

class CProcessRegIODlg : public CDialog
{
// Construction
public:
	CProcessRegIODlg(CWnd* pParent = NULL);   // standard constructor
	BOOL ProcessIsMonitoring(DWORD dwPID);
	BOOL MonitorTargetProcess(DWORD dwPID);


// Dialog Data
	//{{AFX_DATA(CProcessRegIODlg)
	enum { IDD = IDD_PROCESS_REGIO_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessRegIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessRegIODlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSREGIODLG_H__FDC1DFED_5C97_457A_9375_E75A23853A55__INCLUDED_)
