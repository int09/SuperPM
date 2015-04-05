// SuperProcessManagerDoc.h : interface of the CSuperProcessManagerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPERPROCESSMANAGERDOC_H__0B092D09_5F64_44AE_957A_C861A52D9CCD__INCLUDED_)
#define AFX_SUPERPROCESSMANAGERDOC_H__0B092D09_5F64_44AE_957A_C861A52D9CCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSuperProcessManagerDoc : public CDocument
{
protected: // create from serialization only
	CSuperProcessManagerDoc();
	DECLARE_DYNCREATE(CSuperProcessManagerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperProcessManagerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSuperProcessManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSuperProcessManagerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERPROCESSMANAGERDOC_H__0B092D09_5F64_44AE_957A_C861A52D9CCD__INCLUDED_)
