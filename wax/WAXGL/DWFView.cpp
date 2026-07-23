#include "stdafx.h"


#include "dwfview.h"

#include <afxmt.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DWFView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(DWFView, CView)


//Dynamische Erstellung verwendet geschützten Konstruktor
DWFView::DWFView()
{
	waxGl = NULL;
}

DWFView::~DWFView()
{
	delete waxGl;
}

void DWFView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}



//DWF Viewercontrol erstellen
UINT DWFView::createDwfViewer(LPVOID waxgl)
{
	/*
	Anzeige von DWF File initalisieren
	Wenn dwfFilename gesetzt ist und noch kein ActiveX Control erzeugt wurde, 
	wird durch WaxGl::InitWaxGl ein ActiveX Control erzeugt.
	Ansonsten wird nur der Dateiname gesetzt, was ein Neuzeichnen zur Folge hat
*/

	DWFView *view = (DWFView*)waxgl;

	TRY
	{
		// Ein Erzeugen einer Ansicht ohne File ergibt eine Exception in der Ole-Initialisierung
		XASSERT(view->dwfFilename.GetLength());
		
		if (!view->waxGl)
		{
			view->waxGl = new WaxGl;
			view->waxGl->InitWaxGl(view);
		}
		view->waxGl->SetFileName(view->dwfFilename);
//		TRACE("Graphicdatei: %s\n", view->dwfFilename);
	}
	CATCH_ALL(e)
	{
#ifdef _DEBUG
		e->ReportError();
#endif
	}
	END_CATCH_ALL
	return 1;
}

//DWF Filename setzen. Damit wird das Control initialisiert. Nach der 
//Initialisierung wird die Datei angezeigt.
void DWFView::showFile(const CString &fname)
{
	/*
	Dateinamen setzen
	Beim ersten Aufruf wird <c WaxGl> waxGl initialisiert. Dann wird der Filename
	gesetzt über <mf WaxGl::SetFileName>
	*/
	if (!fname.GetLength())
		return;
	if (dwfFilename != fname)
		dwfFilename = fname;
	createDwfViewer((LPVOID)this);
}

void DWFView::DoDataExchange(CDataExchange* pDX) 
{
	/*
	Anzeige updaten
	Hier findet natürlich kein Datenaustausch statt, es wird nur <mf .ShowFile> aufgerufen
	*/
	CView::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate)
		showFile(dwfFilename);
}

void DWFView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNREFERENCED_PARAMETER(pSender);
	UNREFERENCED_PARAMETER(pHint);
	string* fname = (string*)lHint;
	if (fname && fname->length())
	{
		showFile(fname->c_str());
	}
}


void DWFView::OnSize(UINT nType, int cx, int cy)
{
	/*
	Grösse des Fensters ändern. Parameter werden an das ActiveX Control weitergegeben.
	*/
	CView::OnSize(nType, cx, cy);
	if (waxGl && (nType == SIZE_RESTORED))
	{
		waxGl->OnSize(cx, cy);
	}
}

BOOL DWFView::OnEraseBkgnd( CDC* pDC )
{
	/**
	Diese Methode tut so, als ob sie den Hintergrund löschen würde. 
	Wird die default Methode benutzt, wird der Hintergrund gelöscht, nachdem das 
	ActiveX Control gezeichnet hat! 
	*/

	if (waxGl && waxGl->axCtrl && ::IsWindow(((CWnd*)waxGl->axCtrl)->m_hWnd))
	{
		CWnd* ctrl = (CWnd*)waxGl->axCtrl;
		RECT r;
		ctrl->GetWindowRect(&r);

		if ((r.right - r.left) < 100 || (r.bottom - r.top) < 100)
		{
			((CWnd*)ctrl)->SetRedraw(FALSE);
			((CWnd*)ctrl)->ShowWindow(SW_HIDE);
		}
		else
		{
			((CWnd*)ctrl)->ShowWindow(SW_SHOW);
			((CWnd*)ctrl)->SetRedraw(TRUE);
		}
		if (((CWnd*)ctrl)->IsWindowVisible())
			return TRUE;
	}
	CView::OnEraseBkgnd(pDC);

	if (waxGl && waxGl->axCtrl)
		pDC->TextOut(10,10, "Bereich zu klein");
	return true;
}

