// SuperProcessManagerView.h : interface of the CSuperProcessManagerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPERPROCESSMANAGERVIEW_H__376F2603_46AC_4F83_8F67_F7219B17F188__INCLUDED_)
#define AFX_SUPERPROCESSMANAGERVIEW_H__376F2603_46AC_4F83_8F67_F7219B17F188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ProcessManager.h"
#include "ProcessFileIODlg.h"
#include "ProcessNetworkIODlg.h"
#include "ProcessRegIODlg.h"
#include "RunlogDlg.h"


enum
{
		TAB_PROCESS_MANAGER,
		TAB_PROCESS_NETWORK_IO,
		TAB_PROCESS_FILE_IO,
		TAB_PROCESS_REG_IO,
		TAB_RUN_LOG
};

class CSuperProcessManagerView : public CView
{
protected: // create from serialization only
	CSuperProcessManagerView();
	DECLARE_DYNCREATE(CSuperProcessManagerView)

// Attributes
public:
	CSuperProcessManagerDoc* GetDocument();

	void SwitchWindow(INT uWindow);

	BOOL ProcessNetworkIoIsMonitoring(DWORD dwPID){ return m_ProcessNetworkIo.ProcessIsMonitoring(dwPID);}
	BOOL ProcessFileIoIsMonitoring(DWORD dwPID){ return m_ProcesFileIo.ProcessIsMonitoring(dwPID);}
	BOOL ProcessRegIoIsMonitoring(DWORD dwPID){ return m_ProcessRegIo.ProcessIsMonitoring(dwPID);}
	

	BOOL MonitorProcessNetworkIo(DWORD dwPID){ return m_ProcessNetworkIo.MonitorTargetProcess(dwPID);}
	BOOL MonitorProcessNetworkIo(LPPROCESS_INFORMATION lpProcessInfo){
		return m_ProcessNetworkIo.MonitorTargetProcess(lpProcessInfo);}
	
	
	BOOL UnMonitorProcessNetworkIo(DWORD dwPID){ return m_ProcessNetworkIo.UnMonitorTargetProcess(dwPID);}
	
	BOOL MonitorProcessFileIo(DWORD dwPID){ return m_ProcesFileIo.MonitorTargetProcess(dwPID);}
	BOOL MonitorProcessRegIo(DWORD dwPID){ return m_ProcessRegIo.MonitorTargetProcess(dwPID);}

// Operations
public:
	CRunlogDlg		  m_Runlog;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProcessManagerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSuperProcessManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSuperProcessManagerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_TabCtrl;
	CFont m_TabFont;

	CProcessManager m_ProcessManager;
	CProcessFileIODlg m_ProcesFileIo;
	CProcessNetworkIODlg m_ProcessNetworkIo;
	CProcessRegIODlg  m_ProcessRegIo;
};

#ifndef _DEBUG  // debug version in SuperProcessManagerView.cpp
inline CSuperProcessManagerDoc* CSuperProcessManagerView::GetDocument()
   { return (CSuperProcessManagerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERPROCESSMANAGERVIEW_H__376F2603_46AC_4F83_8F67_F7219B17F188__INCLUDED_)
