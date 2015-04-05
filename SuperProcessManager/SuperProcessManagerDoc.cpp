// SuperProcessManagerDoc.cpp : implementation of the CSuperProcessManagerDoc class
//

#include "stdafx.h"
#include "SuperProcessManager.h"

#include "SuperProcessManagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerDoc

IMPLEMENT_DYNCREATE(CSuperProcessManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSuperProcessManagerDoc, CDocument)
	//{{AFX_MSG_MAP(CSuperProcessManagerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerDoc construction/destruction

CSuperProcessManagerDoc::CSuperProcessManagerDoc()
{
	// TODO: add one-time construction code here

}

CSuperProcessManagerDoc::~CSuperProcessManagerDoc()
{
}

BOOL CSuperProcessManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerDoc serialization

void CSuperProcessManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerDoc diagnostics

#ifdef _DEBUG
void CSuperProcessManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSuperProcessManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSuperProcessManagerDoc commands
