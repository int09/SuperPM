#if !defined(AFX_MONITORFILEDLG_H__204C0995_FB17_4F12_9FD0_335AAB905C39__INCLUDED_)
#define AFX_MONITORFILEDLG_H__204C0995_FB17_4F12_9FD0_335AAB905C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonitorFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonitorFileDlg dialog

class CMonitorFileDlg : public CDialog
{
// Construction
public:
	void UpdateParameter(LPCTSTR szFilePath);
	CMonitorFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonitorFileDlg)
	enum { IDD = IDD_MONITOR_FILE };
	CString	m_szFilePath;
	CString	m_szWorkDir;
	CString	m_szDesktop;
	CString	m_szCommanLine;
	BOOL	m_bMonitorFileIo;
	BOOL	m_bMonitorNetworkIo;
	BOOL	m_bMonitorRegIo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonitorFileDlg)
	afx_msg void OnBrowserFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowserDir();
	afx_msg void OnMonitorTagert();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORFILEDLG_H__204C0995_FB17_4F12_9FD0_335AAB905C39__INCLUDED_)
