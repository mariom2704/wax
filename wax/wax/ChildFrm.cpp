#include "stdafx.h"
#include "resource.h"



#include "..\waxprj\drucklistenverwaltung.h"
#include "..\WaxPrj\GpFreigabe.h"
#include "..\waxprj\WpPreisanfrage.h"
#include "..\waxprj\GpMail.h"

#include "LeftView.h"
#include "WaxView.h"
#include "WaxDoc.h"
#include "TabCtrlView.h"
#include "MainFrm.h"
#include "JScriptViewTemplate.h"
#include "WaxHtmlView.h"

			
			
				#include "ChildFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_MESSAGE(WM_NAVIGATE_ANHANG, OnNavigateAnhang)
	ON_COMMAND(ID_VIEW_GRAFIK, OnViewGrafik)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAFIK, OnUpdateViewGrafik)
	ON_COMMAND(ID_DRUCKVORSCHAU, OnDruckvorschau)
	ON_COMMAND(WM_UPDATEVIEW, OnUpdateView)
	ON_UPDATE_COMMAND_UI(ID_DRUCKVORSCHAU, OnUpdateDruckvorschau)
	ON_COMMAND(ID_SAP, OnSap)
	ON_COMMAND(ID_EXCEL, OnExcel)
	ON_COMMAND(ID_NOGRAFIK, OnNografik)
	ON_UPDATE_COMMAND_UI(ID_NOGRAFIK, OnUpdateNografik)
	ON_COMMAND(ID_PREISE, OnPreise)
	ON_COMMAND(ID_FREIGABE, onFreigabe)
	ON_COMMAND(ID_PREISANFRAGE_FREIGEBEN, onPreisanfrageFreigabe)
	ON_COMMAND(ID_FREIGABE_ANGEBOT, onFreigabeAngebot)
	ON_COMMAND(ID_ANGEBOT_ANNEHMEN, onFreigabeAngebotsPreis)
	ON_UPDATE_COMMAND_UI(ID_FREIGABE, onUpdateFreigabe)
	ON_COMMAND(ID_SHOWEXPLORE, onShowexplore)
	ON_UPDATE_COMMAND_UI(ID_SHOWEXPLORE, onUpdateShowexplore)
	ON_COMMAND(ID_NEWELEMENT, onNewelement)
	ON_UPDATE_COMMAND_UI(ID_NEWELEMENT, onUpdateNewelement)
	ON_COMMAND(ID_ENDDRUCKVORSCHAU, onEnddruckvorschau)
	ON_WM_TIMER()
	ON_COMMAND(ID_EMAIL_BESTELLER, onEmailBesteller)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, onFilePrintDirect) 
	ON_COMMAND(ID_FILE_PRINT, onFilePrint) 
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, onUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_PRINTPREVIEW, onUpdateFilePrint)
	ON_COMMAND(ID_PRINTPREVIEW, onExternPrintpreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()



CChildFrame::CChildFrame() : m_OnlineGraficInStaticLowerSplitter(true), wndWidth(300)
{
	/**
	CChildFrame Konstruktion
	Es werden folgende Initialisierungen vorgenommen: "Grafik anzeigen" wird aus der Registry
	gelesen. Menüpunkt dazu ist "Ansicht"\"Grafik anzeigen"
	*/
	
	m_bGrafikView = false;
	m_bDruckVorschau = false;
	m_rViewSize = NULL;
	m_bGrafikView = AfxGetApp()->GetProfileInt("Appearance", "ShowGrafic", 0) ? true : false;
	m_OnlineGraficInStaticLowerSplitter = AfxGetApp()->GetProfileInt("Appearance", "ShowOnlineGrafic", 0) ? true : false;
	wndWidth = AfxGetApp()->GetProfileInt("Appearance", "BaumBreite", 300);
	if (wndWidth < 0)
		wndWidth = 300;
	
	unsigned char *rect = 0;
	unsigned int size = sizeof(RECT);
	AfxGetApp()->GetProfileBinary("Appearance", "View Size Grafic Window", &rect, &size);
	m_rViewSize = (RECT*)rect;
	if (!m_rViewSize)
	{
		m_rViewSize = new RECT;
		memset(m_rViewSize, 0, sizeof(RECT));
	}
	
	if (!m_rViewSize->bottom || m_rViewSize->right >= 2400 || m_rViewSize->bottom >= 2400)
	{
		memset(m_rViewSize, 0, sizeof(RECT));
		m_rViewSize->bottom = 100;
		m_rViewSize->right = 900;
	}
	m_Cmd = WaxDruck::DrucklistenVerwaltung::DRUCKVORSCHAU_ABBRECHEN;
}



LRESULT CChildFrame::OnNavigateAnhang(WPARAM iShowApaxAnhangAndWindow, LPARAM lParam)
{
	
	WaxPart* data = GetCentralDoc()->get_active_object();
	XASSERT(data);

	if( dynamic_cast<ApaxWerkauftrag*>(data->get_werkauftrag()))
	{
		if( FALSE == (BOOL)iShowApaxAnhangAndWindow)
		{
			m_OnlineGraficInStaticLowerSplitter = true;			
			SendMessage(WM_COMMAND, ID_NOGRAFIK);
			m_OnlineGraficInStaticLowerSplitter = true;	
			return 0;
		}
	}
	
	if (data)
	{
		data->createGraphic();
		if(data->get_grafic_file().length())
		{
			BOOL bOnlineGraficInStaticLowerSplitter = m_OnlineGraficInStaticLowerSplitter;
			
			if(bOnlineGraficInStaticLowerSplitter && !GetSplitterWnd()->GetIsChildSplitterActive())
			{
				m_OnlineGraficInStaticLowerSplitter = false;			
				SendMessage(WM_COMMAND, ID_NOGRAFIK);
			}
			if(!dynamic_cast<ApaxWerkauftrag*>(data->get_werkauftrag()))
			{
				GetSplitterWnd()->Navigate(data->get_grafic_file().c_str(), WaxSplitterWnd::eWAXJSCRIPTVIEW, TRUE);
			}
		}
		
	}
	
	return 0;

}


void CChildFrame::onUpdateFilePrint(CCmdUI* pCmdUI)
{
	CHtmlJScriptCoreView* pView(NULL);
	if(!GetSplitterWnd()->GetIsChildSplitterActive())
	{	
		pView =	dynamic_cast<CHtmlJScriptCoreView*>(GetSplitterWnd()->GetPane(0,1)); //Could be old HtmlView or JScriptview for call print
	}
	
	if(pView)
	{
		pView->onUpdatePrintpreview(pCmdUI);
	}
	else
	{
		pCmdUI->Enable(false);
	}
	
}

void CChildFrame::onExternPrintpreview()
{
	
	CHtmlJScriptCoreView* pView(NULL);
	if(!GetSplitterWnd()->GetIsChildSplitterActive())
	{	
		pView =	dynamic_cast<CHtmlJScriptCoreView*>(GetSplitterWnd()->GetPane(0,1)); //Could be old HtmlView or JScriptview for call print
	}																				 // so it is cast to the Base of both => 'CHtmlJScriptCoreView'							
	
	if(pView)
	{
		pView->onPrintpreview();
	}
}






void CChildFrame::onFilePrint()
{
		CHtmlJScriptCoreView* pView = NULL;
		if(!GetSplitterWnd()->GetIsChildSplitterActive())
		{	
			pView =	dynamic_cast<CHtmlJScriptCoreView*>(GetSplitterWnd()->GetPane(0,1));
		}

		if(!pView)
		{	
			SendMessage(WM_COMMAND, ID_DRUCKVORSCHAU);
			return;
		}

		pView->PostMessage(WM_COMMAND, ID_FILE_PRINT);
		return;

}






void CChildFrame::onFilePrintDirect()
{
	
	WaxPart* pt =  m_pCurrentDoc->get_active_object();
	XASSERT(pt);
	if (!pt)
		return;

	CHtmlJScriptCoreView* pView = NULL;
	
	bool printword = (WaxSys::para_int(PRINT_WORD) == 0) ? false : true;		
	if(!GetSplitterWnd()->GetIsChildSplitterActive())
	{	
		pView =	dynamic_cast<CHtmlJScriptCoreView*>(GetSplitterWnd()->GetPane(0,1)); //Could be old HtmlView or JScriptview for call print
	}

	if(!pView)
	{	
		SendMessage(WM_COMMAND, ID_DRUCKVORSCHAU);
		return;
	}

	if (printword)
	{
		pView->printWord();
	}
	else
	{
		pView->print();
	}	
}






//Virtueller Destruktor
CChildFrame::~CChildFrame()
{
	/**
	CChildFrame Destruktion
	Der Destruktor speichert den momentanen Zustand von "Grafik anzeigen"
	*/
	int dx, dxmin;
	m_wndSplitter.GetColumnInfo( 0, dx, dxmin);
	
	AfxGetApp()->WriteProfileInt("Appearance", "ShowGrafic", (int)m_bGrafikView);
	AfxGetApp()->WriteProfileBinary("Appearance", "View Size Grafic Window", (unsigned char*)m_rViewSize, sizeof(RECT));
	AfxGetApp()->WriteProfileInt("Appearance", "ShowOnlineGrafic", (int)m_OnlineGraficInStaticLowerSplitter);
	AfxGetApp()->WriteProfileInt("Appearance", "BaumBreite", dx > 0 ? dx : 300);
	
	TRACE("Destruktor: Viewsize auf %i/%i gesetzt\n", m_rViewSize->right, m_rViewSize->bottom);
	
	delete m_rViewSize;
}



BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT lpcs /**lpcs*/,
	CCreateContext* pContext)
{
	/**
	Client Window mit Splitterwindows erstellen
	Diese Methode wird von den MFC bei jedem Erstellen eines MDI Childwindows aufgerufen.
	Hier wird diesmal nur eine Grundinitialisierung vorgenommen. Das eigentlich Erstellen aller Views
	passiert in <mf .OnUpdateView>. 
	Rückgabe TRUE wenn Erstellung des Clients (inkl. der Splitterwindows) geklappt hat.
	*/
	UNREFERENCED_PARAMETER(lpcs);
	m_CreateContext = *pContext;
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(wndWidth, 600), &m_CreateContext) )
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	
	CLeftView* pView = (CLeftView*) m_wndSplitter.GetLeftView();
	pView->SetMainSplitter(this);

	if(!m_wndSplitter.AddSwitchableStatic( pContext, 0, 1))
	{
		return FALSE;
	}
	
	{
		CWaxView* pView = (CWaxView*)m_wndSplitter.AddSwitchableView(WaxSplitterWnd::eWAXGENERICVIEW,	RUNTIME_CLASS(CWaxView), pContext);
		pView->SetMainSplitter(this);
	}

	{
		CTabCtrlView* pView = (CTabCtrlView*)m_wndSplitter.AddSwitchableView(WaxSplitterWnd::eWAXTABVIEW, RUNTIME_CLASS(CTabCtrlView), pContext);
		pView->SetMainSplitter(this);
	}

	{
		WaxHtmlView* pView = (WaxHtmlView*)m_wndSplitter.AddSwitchableView(WaxSplitterWnd::eWAXHTMLVIEW, RUNTIME_CLASS(WaxHtmlView), pContext);
		pView->SetMainSplitter(this);
	}

	{
		CJScriptViewTemplate* pView = (CJScriptViewTemplate*)m_wndSplitter.AddSwitchableView(WaxSplitterWnd::eWAXJSCRIPTVIEW, RUNTIME_CLASS(CJScriptViewTemplate), pContext);
		pView->SetMainSplitter(this);
	}
	m_wndSplitter.LoadJScriptTemplate();

	setTimer();
	return TRUE;
}





void CChildFrame::OnViewGrafik() 
{
	/**
	Menüpunkt "Grafik anzeigen"
	Hier wird m_bGrafikView getoggelt und OnUpdateView aufgerufen, wo die neue Ansicht 
	erstellt wird
	*/
	m_bGrafikView = !m_bGrafikView;
	OnUpdateView();	
}


void CChildFrame::OnUpdateViewGrafik(CCmdUI* pCmdUI) 
{
	/**
	Setzt den Haken am Menüpunkt "Grafik anzeigen"
	*/
	pCmdUI->SetCheck(m_bGrafikView ? 1 : 0);
	pCmdUI->Enable(m_bDruckVorschau ? FALSE : TRUE);
}



//View updaten
void CChildFrame::OnUpdateView()
{
	onUpdateView();
}



bool CChildFrame::onUpdateView()
{
		
	WaxPart *pt = getActiveWaxPart();

	if (!pt || !updateAllTabs())
	{
		return false;
	}

	m_wndSplitter.UpdateData(TRUE);
	
	bool bDruckvorschau = false;
	if(dynamic_cast<PrjGliederung*>(pt) || dynamic_cast<PrjLeitkarteData*>(pt))
	{
		bDruckvorschau = false;
	}
	else
	{
		bDruckvorschau = m_bDruckVorschau;
	}

	if(bDruckvorschau)
	{
		if(ShowDrawingsWithHeaderForPrinting(pt))
		{
			return true;
		}
	}

	if(m_bGrafikView)
	{
		if(ShowDrawingOnly(pt))
		{
			return true;
		}
	}

	ShowDialogforWaxDataInput(pt);

	return true;
}



BOOL CChildFrame::ShowDialogforWaxDataInput(WaxPart* pt)
{
	
	BOOL bGrafikclassAvailable = (BOOL) pt->getGraphicView();
	if(m_OnlineGraficInStaticLowerSplitter && bGrafikclassAvailable)// if available => Show Splitter => Drawing below the TabView
	{
		
		
			m_wndSplitter.SwitchStaticChild(WaxSplitterWnd::eWAXTABVIEW, WaxSplitterWnd::eWAXJSCRIPTVIEW);
			m_wndSplitter.SetInitializeTab(false);
			m_wndSplitter.UpdateData(FALSE);
			WaxPart* data = m_pCurrentDoc->get_active_object();
			XASSERT(data);
			if( data )
			{
				data->createGraphic();
				if(data->get_grafic_file().length())
				{
					m_wndSplitter.Navigate(data->get_grafic_file().c_str(), WaxSplitterWnd::eWAXJSCRIPTVIEW, TRUE);
				}
			}
	}
	else
	{
		// Show TabView only
		m_wndSplitter.SwitchView(WaxSplitterWnd::eWAXTABVIEW, 0, 1);
		m_wndSplitter.SetInitializeTab(false);
		m_wndSplitter.UpdateData(FALSE);
	}
	m_wndSplitter.RecalcLayout();

	return TRUE;
}


BOOL CChildFrame::ShowDrawingsWithHeaderForPrinting(WaxPart *pt)
{

	BOOL bGrafikclassAvailable = (BOOL) pt->getGraphicView();
	if( (bGrafikclassAvailable  || dynamic_cast<SammelAuftrag*>(pt)) && m_Cmd == WaxDruck::DrucklistenVerwaltung::PREISANFRAGE )
	{
		m_Cmd = WaxDruck::DrucklistenVerwaltung::GRAPHIKDRUCK;
	}
	
	
	
	if(m_Cmd == WaxDruck::DrucklistenVerwaltung::GRAPHIKDRUCK && (bGrafikclassAvailable  || dynamic_cast<SammelAuftrag*>(pt)))
	{
		m_wndSplitter.SwitchView(WaxSplitterWnd::eWAXJSCRIPTVIEW, 0, 1);
		CJScriptViewTemplate* pJScriptTemplate = (CJScriptViewTemplate*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW);
		CString strUrlOrGrafic, strHeader, strRows;
		pJScriptTemplate->createScriptHeaderAndGrafic(strHeader, strUrlOrGrafic, strRows);
		strHeader = "0" + strHeader;
		strRows = "0" + strRows;
		m_wndSplitter.Navigate(strUrlOrGrafic, WaxSplitterWnd::eWAXJSCRIPTVIEW, FALSE, strHeader, strRows);
	}
	else
	{
		m_wndSplitter.SwitchView(WaxSplitterWnd::eWAXHTMLVIEW, 0, 1);
		m_wndSplitter.SetInitializeTab(false);
		m_wndSplitter.UpdateData(FALSE);	
	}
	m_wndSplitter.RecalcLayout();
	return TRUE;
}



BOOL CChildFrame::ShowDrawingOnly(WaxPart* pt)
{
	BOOL bGrafikclass = (BOOL) pt->getGraphicView();

	if(bGrafikclass)// if available => Show Splitter => Drawing below the TabView
	{
		m_wndSplitter.SwitchView(WaxSplitterWnd::eWAXJSCRIPTVIEW, 0, 1);
		WaxPart *data = ((CWaxDoc*)GetActiveView()->GetDocument())->get_active_object();
		XASSERT(data);
		if (data)
		{
			data->createGraphic();
			if(data->get_grafic_file().length())
			{
				m_wndSplitter.Navigate(data->get_grafic_file().c_str(), WaxSplitterWnd::eWAXJSCRIPTVIEW, TRUE);
			}
		}
	}
	else
	{
		m_wndSplitter.SwitchView(WaxSplitterWnd::eWAXTABVIEW, 0, 1);
		m_wndSplitter.SetInitializeTab(false);
		m_wndSplitter.UpdateData(FALSE);	
	}
	m_wndSplitter.RecalcLayout();

	return TRUE;
}


void CChildFrame::ActivateFrame(int nCmdShow) 
{			
	OnUpdateView();

	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}




void CChildFrame::OnDruckvorschau() 
{
	/***
	Druckvorschau aktivieren
	Es wird zwischen Normalansicht und Druckvorschau umgeschaltet
	Dabei ist es egal, ob Grafikansicht oder Eingabeansicht aktiviert ist. Es wird jetzt auf der
	rechten Seite ein <c WaxHtmlView> Objekt angezeigt. Dieses Objekt wird in <mf .OnUpdateView>
	erzeugt. Das HtmlView Objekt holt sich die anzuzeigende Html Datei.
	*/
	
	
	WaxPart *wa = getActiveWaxPart();
	if (!wa || !updateAllTabs())
		return;

	if (dynamic_cast<PrjGliederung*>(wa) || dynamic_cast<PrjLeitkarteData*>(wa))
		return;

	CMenu pop;
		
	WaxDruck::DrucklistenVerwaltung drucklst(wa);
	drucklst.init_menu();

	pop.CreatePopupMenu();

	for (vector<int>::const_iterator id = drucklst.get_menuitems().begin(); id != drucklst.get_menuitems().end(); id++)
	{
		if (*id == WaxDruck::DrucklistenVerwaltung::SEPARATOR)
			pop.AppendMenu(MF_SEPARATOR);
		else
			pop.AppendMenu(MF_STRING, *id,  drucklst.get_name(*id, true).c_str());
				
		if (m_bDruckVorschau && *id == drucklst.get_lastAusgabe())
			pop.CheckMenuItem(drucklst.get_currentAusgabe(), MF_BYCOMMAND | MF_CHECKED);
	}
	
	CPoint pt = ((CMainFrame*)AfxGetMainWnd())->getToolbarButton(ID_DRUCKVORSCHAU);
	UINT uCmd = pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, this); 
	pop.DestroyMenu();
	
	bool bMenuhasChecked = true;
	if(uCmd == 0)
	{
		uCmd = m_Cmd;
		bMenuhasChecked = false;
	}
	else
	{
		m_Cmd = uCmd;
	}
	
			
	if (m_Cmd == WaxDruck::DrucklistenVerwaltung::DRUCKVORSCHAU_ABBRECHEN)
	{
		m_bDruckVorschau = false;
			
	}
	else if (m_Cmd)
	{
		drucklst.set_currentAusgabe(m_Cmd);

		// ausgewaehlte Ausgabe merken
		drucklst.set_lastAusgabe(m_Cmd);
		m_bDruckVorschau = true;
	}

	if (bMenuhasChecked) // Bei 0 wurde kein Menüpunkt ausgewählt
	{
			OnUpdateView();
	}


}


void CChildFrame::OnUpdateDruckvorschau(CCmdUI* pCmdUI) 
{
	/**
	Menüpunkt Druckvorschau aktualisieren
	Setzt den Haken im Menü "Ansicht/Druckvorschau"
	*/
	pCmdUI->SetCheck(m_bDruckVorschau ? 1 : 0);
	pCmdUI->Enable();	
}



void CChildFrame::OnPreise()
{
	/**
	Menüpunkt Preis-Bereich
	Zeigt das Popupmenü mit den verschiedenen Preisanzeigen an
	*/
	UINT cmd;
	WaxPart *wp = getActiveWaxPart();
	if (!wp || !wp->get_werkauftrag() || !updateAllTabs())
		return;
	CMenu pop;

	Preise preise(wp->get_werkauftrag());
	
	if (!wp->get_subobjects().IsEmpty() && !wp->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{
		// Objekte der 1. Untergliederungsebene in Liste packen
		preise.set_walist(wp->get_subobjects());
	
		// Weitere Untergliederungen in Liste packen
		while (!preise.get_gliederunglist().IsEmpty())
		{
			preise.set_walist();
		}
	}
	else
	{
		if (!WaxSys::isAV())
		{
			SammelAuftrag* tst = dynamic_cast<SammelAuftrag*>(wp);
			if (tst)
				tst->onCreateAutoarbeit();
		}	
		preise.walist.AddTail(wp);
	}
	
	// Popupmenü anzeigen
	pop.CreatePopupMenu();
	preise.initAusgaben();
	for (int i = 0; i < preise.get_ausgaben().GetSize(); i++)
	{
		int id = preise.get_ausgaben().GetAt(i);
		preise.set_currentAusgabe(id);
		pop.AppendMenu(MF_STRING, id,  (LPCTSTR)preise.getName(id));
	}
	CPoint pt = ((CMainFrame*)AfxGetMainWnd())->getToolbarButton(ID_PREISE);
	
	cmd = pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, this); 
	pop.DestroyMenu();
	preise.makeAusgabe(cmd);
}

//Menüpunkt SAP
void CChildFrame::OnSap()
{
	/**
	Menüpunkt SAP-Bereich
	*/
	UINT cmd;
	WaxPart *wa = getActiveWaxPart();
	if (!wa || !updateAllTabs())
		return;;

	CMenu pop;

	Sap sap;
	if (!wa->get_subobjects().IsEmpty())
	{
		sap.set_werkauftrag(wa->get_werkauftrag());
		sap.set_walist(wa->get_subobjects());
	}
	else
		sap.set_werkauftrag(wa->get_werkauftrag());
	sap.initAusgaben();

	pop.CreatePopupMenu();

	for (int i = 0; i < sap.get_ausgaben().GetSize(); i++)
	{
		sap.set_currentAusgabe(sap.get_ausgaben().GetAt(i));
		pop.AppendMenu(MF_STRING, sap.get_ausgaben().GetAt(i),  (LPCTSTR)sap.getName());
	}
	
	CPoint pt = ((CMainFrame*)AfxGetMainWnd())->getToolbarButton(ID_SAP);
	
	cmd = pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, this); 

	// Nicht bei Gliederung und Projektleitkarte
	if ((wa->IsKindOf(RUNTIME_CLASS(PrjGliederung)) || wa->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData)) || wa->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)))
		&& (cmd == Sap::SAPTOWAX_FILE || cmd == Sap::SAPTOWAX_ALL))
		return;

	// Ist Werkauftrag unter Sammelauftrag muss dieser makiert sein
	if ((wa->get_parent() && wa->get_parent()->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		&& cmd == Sap::WAXTOSAP)
	{
		CString str; str = getStammdaten().getText("IDS_HINWEIS1a").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONERROR);
		return;
	}
	
	sap.set_currentAusgabe(cmd);
	pop.DestroyMenu();
	sap.makeAusgabe(cmd);

	if (cmd == Sap::WAXTOSAP && sap.get_createOK())
		PostMessage(WM_COMMAND, ID_FREIGABE);

	const PrjLeitkarteData *prjleitkarte = ((CWaxDoc*)GetActiveView()->GetDocument())->get_prjleitkarte();
	if (prjleitkarte)
	{
		(const_cast<PrjLeitkarteData *>(prjleitkarte))->get_werkauftrag()->get_leitkarte().copy(wa->get_leitkarte());
	}
	CTabCtrlView* pTab = (CTabCtrlView*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
	if (!pTab->UpdateData(FALSE))
		return;
}

//Menüpunkt Excel
void CChildFrame::OnExcel()
{
	/**
	Menüpunkt Excel-Schnittstellen
	*/
	UINT cmd;
	WaxPart *wa = getActiveWaxPart();
	if (!wa || !updateAllTabs())
		return;

	CMenu pop;

	Excel excel;

	
	if (WaxSys::isAV())
	{
		excel.setExcelTableOfficeVersion2010(false);
	}
	

	// Objekte der 1. Untergliederungsebene in Liste packen
	if (!wa->get_subobjects().IsEmpty())
	{
		excel.set_walist(wa->get_subobjects());
		// Weitere Untergliederungen und Sammelauftraege in Liste packen
		while (!excel.get_sammelauftraege().IsEmpty())
		{
			excel.set_walist();
		}
	}
	else
		excel.set_werkauftrag(wa->get_werkauftrag());
	
	excel.initAction();
	pop.CreatePopupMenu();

	for (int i = 0; i < excel.get_aktionen().GetSize(); i++)
	{
		int id = excel.get_aktionen().GetAt(i);
		pop.AppendMenu(MF_STRING, id,  (LPCTSTR)excel.getName(id));
	}
	
	CPoint pt = ((CMainFrame*)AfxGetMainWnd())->getToolbarButton(ID_EXCEL);
	cmd = pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, this); 

	if (cmd != Excel::PREISTEST && cmd != Excel::AUFTRAGSTABELLE && cmd != Excel::ZUSAMMENFASSUNG_WERKAUFTRAEGE)
	{
		// Fuer Maschine muss Werkauftrag markiert sein
		if (wa->IsKindOf(RUNTIME_CLASS(PrjGliederung)) || wa->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData)) || wa->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)))
		{
			CString str; str = getStammdaten().getText("IDS_HINWEIS2a").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);	
			return;
		}
		// Ist Werkauftrag unter Sammelauftrag muss dieser makiert sein
		if (wa->get_parent() && wa->get_parent()->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		{
			CString str; str = getStammdaten().getText("IDS_HINWEIS1a").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			return;
		}
	}
	excel.set_currentAction(cmd);
	pop.DestroyMenu();
	excel.makeAction(cmd);
}

//Schaltet Online Grafik an und aus
void CChildFrame::OnNografik()
{
	m_OnlineGraficInStaticLowerSplitter = !m_OnlineGraficInStaticLowerSplitter;
	OnUpdateView();
}

void CChildFrame::OnUpdateNografik(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_OnlineGraficInStaticLowerSplitter);
}

//Wrapper Methode für die Freigabe. Die Funktion in der Toolbar wird nur 
//enabled, wenn auch ein Eintrag in der MessageMap vorhanden ist. Die 
//Freigabe wird allerdings vom WaxPart behandelt.
void CChildFrame::onFreigabe()
{
	if (!getActiveWaxPart() || !updateAllTabs())
		return;
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	XASSERT(doc);
	if (!doc)
		return;
	
	WaxPart* activeo = doc->get_active_object();
	XASSERT(activeo);
	if (!activeo)
		return;
	
	CTabCtrlView* pTab = (CTabCtrlView*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
	pTab->UpdateData();

	doc->doAutosave(true); // Speichern vor der Freigabe, damit alle Änderungen abgespeichert sind,
	// falls während der Freigabe abgebrochen wird
	activeo->on_freigeben();
	
	m_wndSplitter.GetLeftView()->GetTreeCtrl().SetFocus();
	m_wndSplitter.GetLeftView()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	init_crc();

	// Nach der Freigabe nochmals abspeichern
	doc->doAutosave(true);
}

void CChildFrame::onPreisanfrageFreigabe()
{
	if (!getActiveWaxPart() || !updateAllTabs())
		return;
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	XASSERT(doc);
	if (!doc)
		return;
	
	WaxPart* activeo = doc->get_active_object();
	XASSERT(activeo);
	if (!activeo)
		return;
	
	CTabCtrlView* pTab = (CTabCtrlView*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
	pTab->UpdateData();

	doc->doAutosave(true); // Speichern vor der Freigabe, damit alle Änderungen abgespeichert sind,
	// falls während der Freigabe abgebrochen wird
	//activeo->on_freigeben();

	GpFreigabe freigabe;
	freigabe.freigabePreisanfrage(activeo);

	
	m_wndSplitter.GetLeftView()->GetTreeCtrl().SetFocus();
	m_wndSplitter.GetLeftView()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	init_crc();

	// Nach der Freigabe nochmals abspeichern
	doc->doAutosave(true);
}

void CChildFrame::onFreigabeAngebot()
{
	if (!getActiveWaxPart() || !updateAllTabs())
		return;
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	XASSERT(doc);
	if (!doc)
		return;
	
	WaxPart* activeo = doc->get_active_object();
	XASSERT(activeo);
	if (!activeo)
		return;
	
	CTabCtrlView* pTab = (CTabCtrlView*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
	pTab->UpdateData();

	doc->doAutosave(true); // Speichern vor der Freigabe, damit alle Änderungen abgespeichert sind,
	// falls während der Freigabe abgebrochen wird
	//activeo->on_freigeben();

	GpFreigabe freigabe;
	if (!freigabe.freigabeAngebot(activeo))
		return;
	
	m_wndSplitter.GetLeftView()->GetTreeCtrl().SetFocus();
	m_wndSplitter.GetLeftView()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	init_crc();

	// Nach der Freigabe nochmals abspeichern
	doc->doAutosave(true);


	//1.Sammelauftrag wegen Emailadresse holen
	CString stmail;
	for (POSITION posG = activeo->get_subobjects().GetHeadPosition(); posG != NULL; )
	{
		WaxPart* waxpt = dynamic_cast<WaxPart*>(activeo->get_subobjects().GetNext(posG));
		if (!waxpt || waxpt->get_deleted())
			continue;

		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(waxpt);
		if (!sammel)
			continue;

		stmail = sammel->get_leitkarte().get_emailNl();
	}

	//Email an den Besteller
	CString betreff;
	betreff.Format(getStammdaten().getText("msg_angebot_erstellt").c_str(), activeo->get_leitkarte().get_preisanfrage().get_nummer());
	string null;
	GpMail mail;
	mail.sendMail(stmail.GetBuffer(), betreff.GetBuffer(), null, null);

}

void CChildFrame::init_crc()
{
	CTabCtrlView* pTab = (CTabCtrlView*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
	pTab->UpdateData();
	if (!pTab)
		return;
	WaxPartForm* cur = dynamic_cast<WaxPartForm*>(pTab->runtimeObjects.GetTail());
	if (cur)
		cur->initCrc();
}



void CChildFrame::onUpdateFreigabe(CCmdUI* pCmdUi)
{
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	XASSERT(doc);
	if (doc)
	{
		if(!WaxSys::isAV())
		{
			pCmdUi->Enable(false);
		}
		else
		{
			WaxPart* ltprobe = dynamic_cast<PrjLeitkarteData*>(doc->get_active_object());
			WaxPart* pt = doc->get_active_object();
			XASSERT(pt);
			pCmdUi->Enable(!ltprobe && pt && pt->get_plausiOk());
		}
	}
}



void CChildFrame::set_DruckVorschau(bool value)
{
	m_bDruckVorschau = value;
}

const bool CChildFrame::get_DruckVorschau() const
{
	return m_bDruckVorschau;
}

//Blendet das Projektfenster ein oder aus indem die Breite auf 0 bzw. letzten 
//Wert gesetzt wird
void CChildFrame::onShowexplore()
{
	CRect r;
	CWnd *wnd = m_wndSplitter.GetLeftView();
	wnd->GetWindowRect(&r);

	// Fenster auf Breite 0 setzen
	if (r.left-r.right)
	{
		wndWidth = (r.right-r.left) > 0 ? r.right-r.left : 300;
		m_wndSplitter.SetColumnInfo( 0, 0, 0);
	}
	else
	{// Fensterbreite wieder normalisieren
		m_wndSplitter.SetColumnInfo( 0, wndWidth, 10);
	}
	m_wndSplitter.RecalcLayout();
}



void CChildFrame::onUpdateShowexplore(CCmdUI* cmdui)
{
	CWnd *wnd = m_wndSplitter.GetLeftView();
	CRect r;
	wnd->GetWindowRect(&r);

	if (r.left-r.right)
		cmdui->SetCheck();
	else
		cmdui->SetCheck(0);
}

void CChildFrame::onNewelement()
{
	/**
	Neues Objekt nach Auswahl einfügen. 
	*/
	m_wndSplitter.GetLeftView()->PostMessage(WM_COMMAND, ID_NEWELEMENT);
}



void CChildFrame::onUpdateNewelement(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(true);
}



void CChildFrame::onEnddruckvorschau()
{
	m_bDruckVorschau = false;	
	OnUpdateView();
}



void CChildFrame::OnTimer(UINT_PTR timerid)
{
	UNREFERENCED_PARAMETER(timerid);
	KillTimer(0);
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	if (doc && doc->IsModified())
	{
		doc->doAutosave();
	}
	setTimer();
}

BOOL CChildFrame::DestroyWindow( )
{
	KillTimer(0);

	return CMDIChildWnd::DestroyWindow();
}

void CChildFrame::setTimer()
{
	/***
	Timer für Autosave initialisieren. Default ist zehn Minuten. Eingestellt wird die
	Zeit über den Parameter CurrentUser->Software->Minimax->Wax+->Save_options->autosave
	*/
	UINT timeout = AfxGetApp()->GetProfileInt("Common", "Autosave", 10);
	if (timeout > 0)
		SetTimer(0, 1000*60*timeout, NULL);
}

bool CChildFrame::updateAllTabs()
{
	if (!m_wndSplitter.UpdateData(TRUE))
		return false;
	return true;
}

WaxPart *CChildFrame::getActiveWaxPart()
{
	WaxPart *pt = NULL;
	TRY
	{
		CWaxDoc *activedoc = dynamic_cast<CWaxDoc*>(GetActiveView()->GetDocument());
		XASSERT(activedoc);
		if (activedoc)
			pt = activedoc->get_active_object();
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL
	return pt;
}




void CChildFrame::onFreigabeAngebotsPreis()
{
	if (!getActiveWaxPart() || !updateAllTabs())
		return;
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	XASSERT(doc);
	if (!doc)
		return;
	
	WaxPart* activeo = doc->get_active_object();
	XASSERT(activeo);
	if (!activeo)
		return;
	
	CTabCtrlView* pTab = (CTabCtrlView*)m_wndSplitter.GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
	pTab->UpdateData();

	doc->doAutosave(true); // Speichern vor der Freigabe, damit alle Änderungen abgespeichert sind,
	// falls während der Freigabe abgebrochen wird
	GpFreigabe freigabe;
	freigabe.freigabeAngebotspreis(activeo);
	
	m_wndSplitter.GetLeftView()->GetTreeCtrl().SetFocus();
	m_wndSplitter.GetLeftView()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	init_crc();

	// Nach der Freigabe nochmals abspeichern
	doc->doAutosave(true);
}


void CChildFrame::onEmailBesteller()
{
	if (!getActiveWaxPart() || !updateAllTabs())
		return;
	CWaxDoc* doc = (CWaxDoc*)GetActiveDocument();
	XASSERT(doc);
	if (!doc)
		return;
	
	WaxPart* activeo = doc->get_active_object();
	XASSERT(activeo);
	if (!activeo)
		return;

	CString stmail = activeo->get_leitkarte().get_emailNl();
	CString prjBez = activeo->get_leitkarte().get_prj_auftragsbez();
	string null;
	GpMail mail;
	mail.sendMail(stmail.GetBuffer(), prjBez.GetBuffer(), null, null);
}



void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	if(m_wndSplitter.GetWaxSplitterChildView())
	{
		if( dynamic_cast<CWaxSplitterChildView*>(m_wndSplitter.GetPane(0, 1)) && nType != SIZE_MINIMIZED )
		{
			CRect cr;
			m_wndSplitter.GetPane(0, 0)->GetWindowRect(cr);
			ScreenToClient(&cr);
			
			m_wndSplitter.SetColumnInfo( 0,  cr.right-3, 50);
			m_wndSplitter.RecalcLayout();

			m_wndSplitter.SetColumnInfo( 0,  cr.right-2, 50);
			m_wndSplitter.RecalcLayout();
		}	
	}
}
