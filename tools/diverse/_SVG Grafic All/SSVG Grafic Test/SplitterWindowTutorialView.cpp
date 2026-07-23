// SplitterWindowTutorialView.cpp : implementation of the CSplitterWindowTutorialView class
//

#include "stdafx.h"
#include "SplitterWindowTutorial.h"

#include "SplitterWindowTutorialDoc.h"
#include "SplitterWindowTutorialView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialView

IMPLEMENT_DYNCREATE(CSplitterWindowTutorialView, CView)

BEGIN_MESSAGE_MAP(CSplitterWindowTutorialView, CView)
	//{{AFX_MSG_MAP(CSplitterWindowTutorialView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialView construction/destruction

CSplitterWindowTutorialView::CSplitterWindowTutorialView()
{
	// TODO: add construction code here

}

CSplitterWindowTutorialView::~CSplitterWindowTutorialView()
{
}

BOOL CSplitterWindowTutorialView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialView drawing

void CSplitterWindowTutorialView::OnDraw(CDC* )
{
	CSplitterWindowTutorialDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialView printing

BOOL CSplitterWindowTutorialView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSplitterWindowTutorialView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSplitterWindowTutorialView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialView diagnostics

#ifdef _DEBUG
void CSplitterWindowTutorialView::AssertValid() const
{
	CView::AssertValid();
}

void CSplitterWindowTutorialView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSplitterWindowTutorialDoc* CSplitterWindowTutorialView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSplitterWindowTutorialDoc)));
	return (CSplitterWindowTutorialDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialView message handlers
