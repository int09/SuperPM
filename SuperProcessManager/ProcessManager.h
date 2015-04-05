#if !defined(AFX_PROCESSMANAGER_H__29433EF3_1D27_4887_83EE_75157784BC01__INCLUDED_)
#define AFX_PROCESSMANAGER_H__29433EF3_1D27_4887_83EE_75157784BC01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MSortListCtrl.h"
// ProcessManager.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessManager dialog
#include "ProcessTool/ModuleToo.h"
#include "ProcessTool/ProcessTool.h"
#include "ProcessTool/ThreadTool.h"
#include "ProcessTool/HandleTool.h"
#include "ProcessTool/PerformanceTool.h"

typedef map<DWORD,BOOL> PROCESS_SUSPEND;
typedef pair<DWORD,BOOL> PROCESS_SUSPEND_PAIR;


enum {SUB_LIST_MODULE,SUB_LIST_THREAD,SUB_LIST_HANDLE};

class CProcessManager : public CDialog
{
// Construction
public:
	void SwitchSubWindow();
	BOOL OnSubThread(DWORD dwPID);
	BOOL OnSubModule(DWORD dwPID);
	BOOL OnSubHandle(DWORD dwPID);
	BOOL InitHandleList();
	BOOL InitThreadList();
	BOOL InitModuleList();
	BOOL InitProcessList();
	BOOL ShowProcessInfo();
	BOOL ProcessListDeleteItem(INT nItem);
	void InsertStirngToBar(CListCtrl * pList,LPCOLUMN_DATA pData,DWORD dwSize,INT nCurSel);
	CProcessManager(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessManager)
	enum { IDD = IDD_PROCESS_MANAGER_DLG };
	CMSortListCtrl	m_ProcessList;
	//}}AFX_DATA

	CImageList      m_ProcessListImg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessManager)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRclickProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProcessHandle();
	afx_msg void OnProcessModule();
	afx_msg void OnProcessThread();
	afx_msg void OnSusppedProcess();
	afx_msg void OnTerminatorProcessCureent();
	afx_msg void OnTerminatorProcessCheckbox();
	afx_msg void OnTerminatorProcessTree();
	afx_msg void OnSetPriorityRealtime();
	afx_msg void OnSetPriorityNormal();
	afx_msg void OnSetPriorityIdel();
	afx_msg void OnSetPriorityHeight();
	afx_msg void OnSetPriorityBelowNormal();
	afx_msg void OnSetPriorityAboveHeight();
	afx_msg void OnDebugProcess();
	afx_msg void OnAttributeFile();
	afx_msg void OnPositioningFile();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBaiduSearch();
	afx_msg void OnGoogleSearch();
	afx_msg void OnOnlineVirusScan();
	afx_msg void OnMonitorAll();
	afx_msg void OnMonitorFileio();
	afx_msg void OnMonitorNetwokio();
	afx_msg void OnMonitorRegio();
	afx_msg void OnDblclkProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInjectProcess();
	//}}AFX_MSG
	afx_msg LRESULT OnProcessListResizeing(WPARAM,LPARAM);
	afx_msg void OnClickModuleList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickThreadList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickHandleList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDrawColorForProcessList(NMHDR *pNmHdr, LRESULT *pResult);
	afx_msg void OnDrawColorForThreadList(NMHDR *pNmHdr, LRESULT *pResult);
	afx_msg void OnDrawColorForModuleList(NMHDR *pNmHdr, LRESULT *pResult);
	afx_msg void OnDrawColorForHandleList(NMHDR *pNmHdr, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	CProcessTool m_ProcessTool;
	CMenu	m_ProcessMenu;
	INT m_nProcessListButtomCoor;
	INT m_nCurrentSelectItem;
private:
	CMSortListCtrl m_ModuleList;
	CModuleTool   m_ModuleTool;
	CImageList m_ImageModule;
private:
	CMSortListCtrl m_ThreadList;
	CThreadTool m_ThreadTool;
	CImageList m_ImageThread;
private:
	CMSortListCtrl m_HandleList;
	CHandleTool m_HandleTool;
private:
	INT m_nSubList;
	CPerformanceTool m_PerformanceTool;
private:
	PROCESS_SUSPEND m_ProcessSuspendInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMANAGER_H__29433EF3_1D27_4887_83EE_75157784BC01__INCLUDED_)
