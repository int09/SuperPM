#if !defined(AFX_RUNLOGDLG_H__54F6D2AB_F0AF_4A0F_8EAD_FD67C6A9A17F__INCLUDED_)
#define AFX_RUNLOGDLG_H__54F6D2AB_F0AF_4A0F_8EAD_FD67C6A9A17F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunlogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunlogDlg dialog

class CRunlogDlg : public CDialog
{
// Construction
public:
	CRunlogDlg(CWnd* pParent = NULL);   // standard constructor
	LRESULT OnOutputLogString(WPARAM wParam,LPARAM lParam);
// Dialog Data
	//{{AFX_DATA(CRunlogDlg)
	enum { IDD = IDD_RUNLOG_DLG };
	CListBox	m_TextList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunlogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunlogDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNLOGDLG_H__54F6D2AB_F0AF_4A0F_8EAD_FD67C6A9A17F__INCLUDED_)
