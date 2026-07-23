// SplitterWindowTutorialDoc.cpp : implementation of the CSplitterWindowTutorialDoc class
//

#include "stdafx.h"
#include "SplitterWindowTutorial.h"

#include "SplitterWindowTutorialDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialDoc

IMPLEMENT_DYNCREATE(CSplitterWindowTutorialDoc, CDocument)

BEGIN_MESSAGE_MAP(CSplitterWindowTutorialDoc, CDocument)
	//{{AFX_MSG_MAP(CSplitterWindowTutorialDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialDoc construction/destruction

CSplitterWindowTutorialDoc::CSplitterWindowTutorialDoc()
{
	// TODO: add one-time construction code here

}

CSplitterWindowTutorialDoc::~CSplitterWindowTutorialDoc()
{
}

BOOL CSplitterWindowTutorialDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialDoc serialization

void CSplitterWindowTutorialDoc::Serialize(CArchive& ar)
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
// CSplitterWindowTutorialDoc diagnostics

#ifdef _DEBUG
void CSplitterWindowTutorialDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSplitterWindowTutorialDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialDoc commands
