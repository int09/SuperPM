#if !defined(AFX_MONITORNETWORKIOSETTINGDLG_H__EBB3CBDB_E1C1_410A_8B90_8B8D8A483A1B__INCLUDED_)
#define AFX_MONITORNETWORKIOSETTINGDLG_H__EBB3CBDB_E1C1_410A_8B90_8B8D8A483A1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonitorNetworkIoSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonitorNetworkIoSettingDlg dialog

class CMonitorNetworkIoSettingDlg : public CDialog
{
// Construction
public:
	CMonitorNetworkIoSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonitorNetworkIoSettingDlg)
	enum { IDD = IDD_MONITOR_NETWORKIO_SETTING_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorNetworkIoSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonitorNetworkIoSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDone();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORNETWORKIOSETTINGDLG_H__EBB3CBDB_E1C1_410A_8B90_8B8D8A483A1B__INCLUDED_)
