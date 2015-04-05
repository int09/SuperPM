#if !defined(AFX_PUBLICTEXTBARDLG_H__E8EE9900_F50D_4B32_B869_92ABB519D9C4__INCLUDED_)
#define AFX_PUBLICTEXTBARDLG_H__E8EE9900_F50D_4B32_B869_92ABB519D9C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PublicTextBarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPublicTextBarDlg dialog

class CPublicTextBarDlg : public CDialogBar
{
// Construction
public:
	CPublicTextBarDlg();   // standard constructor
	BOOL DeleteAllString();
	void InsertString(LPCTSTR szFormat,...);
	void SetCurSel(INT nIndex) {m_ComboBox.SetCurSel(nIndex);}
	BOOL GetCurrentText(CString & szText);
// Dialog Data
	//{{AFX_DATA(CPublicTextBarDlg)
	enum { IDD = IDD_TEXTBAR_DLG };
	CComboBox	m_ComboBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPublicTextBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPublicTextBarDlg)
	//}}AFX_MSG
	afx_msg LRESULT OnInitDialog(WPARAM , LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUBLICTEXTBARDLG_H__E8EE9900_F50D_4B32_B869_92ABB519D9C4__INCLUDED_)
