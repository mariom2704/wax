
// xHMTLtestsuiteView.cpp: Implementierung der Klasse CxHMTLtestsuiteView
//

#include "stdafx.h"
#include "xHMTLtestsuite.h"

#include "xHMTLtestsuiteDoc.h"
#include "xHMTLtestsuiteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CxHMTLtestsuiteView

IMPLEMENT_DYNCREATE(CxHMTLtestsuiteView, CHtmlView)

BEGIN_MESSAGE_MAP(CxHMTLtestsuiteView, CHtmlView)
	// Standarddruckbefehle
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
END_MESSAGE_MAP()

// CxHMTLtestsuiteView-Erstellung/Zerstörung

CxHMTLtestsuiteView::CxHMTLtestsuiteView()
{
	// TODO: Hier Code zur Konstruktion einfügen

}

CxHMTLtestsuiteView::~CxHMTLtestsuiteView()
{
}

BOOL CxHMTLtestsuiteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CHtmlView::PreCreateWindow(cs);
}


void CxHMTLtestsuiteView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("D:\\work\\wax\\JScriptHtmlTemplates\\test.html"),NULL,NULL);
}


// CxHMTLtestsuiteView drucken


void CxHMTLtestsuiteView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}


void CxHMTLtestsuiteView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CxHMTLtestsuiteView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CxHMTLtestsuiteView-Diagnose

#ifdef _DEBUG
void CxHMTLtestsuiteView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CxHMTLtestsuiteView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CxHMTLtestsuiteDoc* CxHMTLtestsuiteView::GetDocument() const // Nichtdebugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxHMTLtestsuiteDoc)));
	return (CxHMTLtestsuiteDoc*)m_pDocument;
}
#endif //_DEBUG


// CxHMTLtestsuiteView-Meldungshandler
