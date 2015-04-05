// SuperProcessManager.h : main header file for the SUPERPROCESSMANAGER application
//

#if !defined(AFX_SUPERPROCESSMANAGER_H__E28E1F4A_6A3B_4F3C_8059_2BAF75F90DF2__INCLUDED_)
#define AFX_SUPERPROCESSMANAGER_H__E28E1F4A_6A3B_4F3C_8059_2BAF75F90DF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerApp:
// See SuperProcessManager.cpp for the implementation of this class
//

class CSuperProcessManagerApp : public CWinApp
{
public:
	CSuperProcessManagerApp();
	TCHAR szDllPath[MAX_PATH];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProcessManagerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSuperProcessManagerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hMutex;		

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERPROCESSMANAGER_H__E28E1F4A_6A3B_4F3C_8059_2BAF75F90DF2__INCLUDED_)
