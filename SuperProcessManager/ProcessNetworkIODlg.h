#if !defined(AFX_PROCESSNETWORKIODLG_H__F0798352_6F97_4247_9716_E89D61E5F273__INCLUDED_)
#define AFX_PROCESSNETWORKIODLG_H__F0798352_6F97_4247_9716_E89D61E5F273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessNetworkIODlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessNetworkIODlg dialog
#if _MSC_VER ==1200 
#pragma warning(disable:4786)
#endif
#include <string>
#include "Common/MSortListCtrl.h"

#include "ProcessTool/ThreadTool.h"

typedef map<DWORD,INT> ICONDATA;
typedef pair<DWORD,INT>  ICONDATAPAIR;
typedef map<string,string> DNSRECORDDATA;
typedef pair<string,string> DNSRECORDDATAPAIR;

class CProcessNetworkIODlg : public CDialog
{
// Construction
public:
	void ShowNetworkIoData();
	void InsertNormalData(LPNETWORK_API_DATA lpHead,LPBYTE lpContext);
	BOOL MonitorTargetProcess(DWORD dwPID);
	BOOL MonitorTargetProcess(LPPROCESS_INFORMATION lpProcessInfo);

	BOOL UnMonitorTargetProcess(DWORD dwPID);
	
	BOOL ProcessIsMonitoring(DWORD dwPID);
	BOOL InitDataList();
	INT GetIconIndex(DWORD dwPID,LPCTSTR szProcessPath);
	void InsertStirngToBar(CListCtrl * pList,LPCOLUMN_DATA pData,DWORD dwSize,INT nCurSel);
	CProcessNetworkIODlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessNetworkIODlg)
	enum { IDD = IDD_PROCESS_NETWORKIO_DLG };
	CMSortListCtrl	m_DataList;
	CImageList		m_DataIamge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessNetworkIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessNetworkIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRclickDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteCurrent();
	afx_msg void OnDeleteSeleced();
	afx_msg void OnSelectAll();
	afx_msg void OnSelectReverseAll();
	afx_msg void OnCancelAll();
	afx_msg void OnClickDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnOpenDns();
	afx_msg void OnOpenIp();
	afx_msg void OnGoogleSearchDns();
	afx_msg void OnGoogleSearchIp();
	afx_msg void OnBaiduSearchDns();
	afx_msg void OnBaiduSearchIp();
	afx_msg void OnPositioningFile();
	afx_msg void OnGoogleSearch();
	afx_msg void OnBaiduSearch();
	afx_msg void OnAttributeFile();
	afx_msg void OnOnlineVirusScan();
	afx_msg void OnSetting();
	afx_msg void OnDeleteInvalidData();
	afx_msg void OnMonitorFile();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDblclkDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTermenaterProcess();
	afx_msg void OnSuspendProcess();
	afx_msg void OnResumeProcess();
	afx_msg void OnFilter();
	//}}AFX_MSG
	afx_msg LRESULT OnDataListResizeing(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnReadNetworkIoData(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDrawColorForDataList(NMHDR *pNmHdr, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
private:
	INT m_nDataListButtomCoor;
	ICONDATA m_IconData;
	DNSRECORDDATA m_DnsRecordData;
	
	CThreadTool m_ThreadTool;

	CFont * m_pEditFont;
	CMenu m_DataListMenu;
	INT m_nDataListCurrentSel;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSNETWORKIODLG_H__F0798352_6F97_4247_9716_E89D61E5F273__INCLUDED_)
