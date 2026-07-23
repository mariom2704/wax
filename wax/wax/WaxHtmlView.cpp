#include "stdafx.h"
#include "resource.h"

#include "..\waxprj\drucklistenverwaltung.h"

#include "MehrfachdruckDlg.h"
#include "WaxDoc.h"
#include "ChildFrm.h"



		#include "WaxHtmlView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(WaxHtmlView, CHtmlJScriptCoreView)

BEGIN_MESSAGE_MAP(WaxHtmlView, CHtmlJScriptCoreView)
	ON_COMMAND_RANGE(WAXID_FIRST, WAXID_LAST, HandleMsgToLeftView)
	
	
	ON_COMMAND(ID_PRINT_DIRECT, onPrintDirect)
END_MESSAGE_MAP()

//Geschützter Konstruktor wird von dynamischer Erstellung verwendet
WaxHtmlView::WaxHtmlView()
{
	m_bInitialUpdate = false;
}

//Destruktor
WaxHtmlView::~WaxHtmlView()
{
	
}

//Datenaustausch und Initialisierung
void WaxHtmlView::DoDataExchange(CDataExchange* pDX)
{	
	CHtmlView::DoDataExchange(pDX);
	/*
	Datenaustausch
	Wird hier benutzt um die Methode <mf .OnInitialUpdate> aufzurufen. OnInitialUpdate wird leider
	nicht für Views aufgerufen, die in einem Splitterwindow eingebettet sind (MFC-Bug)
	*/
	if (!m_bInitialUpdate)
	{
		OnInitialUpdate();
	}
}


//Messages an LeftView weiter, damit z.B. das Drucken richtig 
//aufgerufen wird.
void WaxHtmlView::HandleMsgToLeftView(UINT cmd) 
{
	/*
	Message an Treeview weitersenden
	Alle Messages zwischen WAXID_FIRST und WAXID_LAST werden an die Treeview gesendet.
	*/
	m_pMainSplitterFrame->GetSplitterWnd()->GetLeftView()->SendMessage(WM_COMMAND, cmd); 
} 


void WaxHtmlView::OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	Refresh();
	CView::OnUpdate( pSender, 0, 0);
}






//OnInitialUpdate wird von der MFC leider nicht aufgerufen, wenn die View in 
//einem Splitterwindow erzeugt wird. Deshalb wird die Methode in 
//DoDataExchange aufgerufen, wenn initialUpdate false ist.
void WaxHtmlView::OnInitialUpdate() 
{
	//SetOffline(true);
	/*
	View initialisieren
	Diese Methode ruft <mf WaxPart::createHtmlFile> auf und lädt die erzeugte Htmldatei.
	*/
	m_bInitialUpdate = true;
	CString strURL = createHtmlFile();
	
	if(strURL.IsEmpty())
	{
		return;
	}
	
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
	Navigate(strURL, navNoHistory | navNoReadFromCache | navNoWriteToCache);
}


//Erzeugt die Druckausgabedatei(en)
CString WaxHtmlView::createHtmlFile(CString zielstring)
{
	
	WaxPart* pt = ((CWaxDoc*)GetDocument())->get_active_object();
	XASSERT(pt);
	if (!pt)
	{
		return _T("");
	}
	
	if (pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{
		if (!WaxSys::isAV())
			dynamic_cast<SammelAuftrag*>(pt)->onCreateAutoarbeit();
	}
	
	// Hier wird die ausgewählte Druckliste erzeugt, bzw. die Defaultliste
	WaxDruck::DrucklistenVerwaltung druckliste(pt);
	druckliste.set_ziel(zielstring);
	return  druckliste.makeAusgabe();
}



void WaxHtmlView::onPrintDirect()
{
	print(1);
}



//Erzeugt aus der filelist eine Html Gesamtdatei und liefert den Dateinamen zurück




