// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__03DB4418_5B60_4B80_AD82_A1FAA874F319__INCLUDED_)
#define AFX_MAINFRM_H__03DB4418_5B60_4B80_AD82_A1FAA874F319__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PublicTextBarDlg.h"

enum{STATUSBAR_HANDLE_NUM=1,STATUSBAR_PROCESS_NUM,STATUSBAR_THREAD_NUM,STATUSBAR_CPU_UASGE,STATUSBAR_PAGEFILE_USAGE,STATUSBAR_CPU_NUM};

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	void SetStatusBarText(DWORD nIndex,LPCTSTR szFormat,...);
// Attributes
public:
	CPublicTextBarDlg m_PublicTextBar;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdatePublicTextBar(CCmdUI* pCmdUI);
	afx_msg void OnPublicTextBar();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnTopmost();
	afx_msg void OnShowWindow();
	afx_msg void OnSetting();
	afx_msg void OnExit();
	afx_msg void OnAbout();
	afx_msg void OnUpdate();
	afx_msg void OnMonitorNetworkioSetting();
	afx_msg void OnMonitorFile();
	//}}AFX_MSG
	afx_msg LRESULT OnNcMsg(WPARAM,LPARAM);
	afx_msg void OnUpdateComboAddress(CCmdUI * pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	NOTIFYICONDATA m_nc;
	CMenu  m_NotifyIconMenu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__03DB4418_5B60_4B80_AD82_A1FAA874F319__INCLUDED_)
