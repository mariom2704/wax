#include "stdafx.h"
#include "resource.h"
#include "wax.h"

#include "DBAuswahl.h"
#include "DBInfo.h"
#include "Auftragstest.h"


#include "..\waxint\WaxServer.h"
#include "..\waxui\SpanxStrangrohrDialog.h"
#include "..\waxui\SpanxVerteilerDialog.h"


#include "lizenz.rc.h"
#include "..\waxint\BefehlGesamtStatistkNeuSchreiben.H"
#include "..\mxparam\Passwd.h"
#include "NeedText.h"
#include "..\waxint\ServerDbPa.h"
#include "ImportSAPToWAX.h"



		#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_BENUTZEREINSTELLUNGEN, onBenutzereinstellungen) 
	ON_COMMAND(IDM_PARAMETER_DIALOG, on_systemparameter) 
	ON_COMMAND(ID_DATABASE, onDatabase) 
	ON_UPDATE_COMMAND_UI(ID_OPEN_DATABASE, onUpdateLoadServerDatabase) 
	ON_COMMAND(ID_OPEN_DATABASE, onLoadServerDatabase) 
	ON_COMMAND(ID_DATABASE_INFO, onDatabaseInfo)
	ON_COMMAND(ID_DATABASE_UPDATESTATISTIK, onDatabaseUpdateStatistik)
	ON_COMMAND(ID_OPEN_NEWWA, onOpenNewWa)
	ON_COMMAND(ID_OPEN_NEW_PA, onOpenNewPa)
	ON_UPDATE_COMMAND_UI(ID_OPEN_NEWWA, onUpdateLoadServerDatabase) 
	ON_COMMAND(ID_OPEN_MYWA, onOpenMyWa)
	ON_COMMAND(ID_UPDATE_ARTIKELSTAMM, onUpdateArtikelstamm)
	ON_UPDATE_COMMAND_UI(ID_OPEN_MYWA, onUpdateLoadServerDatabase) 	
	ON_COMMAND(ID_MONITOR, onMonitor)
	ON_COMMAND(ID_OPEN_DATABASE_PSPELEMENT, onOpenPspelement)
	ON_COMMAND(IDC_OPEN_AUFTRAEGE, onOpenAuftraege)
	ON_COMMAND(ID_OPEN_DB_PA, onOpenPreisanfrage)
	ON_UPDATE_COMMAND_UI(ID_OPEN_DATABASE_PSPELEMENT, onUpdateLoadServerDatabase) 
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_TIMER()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_SPANXANFRAGE_STRANGROHR, on_spanx_strangrohr)
	ON_MESSAGE(ID_SPANXANFRAGE_STRANGROHR, on_spanx_strangrohr)
	ON_COMMAND(ID_SPANXANFRAGE_VERTEILER, on_spanx_verteiler)
	ON_MESSAGE(ID_SPANXANFRAGE_VERTEILER, on_spanx_verteiler)
	ON_COMMAND(ID_AUFTRAGSTEST, on_auftragstest)
	ON_UPDATE_COMMAND_UI(ID_AUFTRAGSTEST, onUpdateLoadServerDatabase) 
	ON_COMMAND(ID_INITBARS, init_bars_and_menues)
	ON_COMMAND(SWITCH_AV, switch_av)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnDynamicTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnDynamicTipText)
	ON_WM_MENUSELECT()
END_MESSAGE_MAP()



CMainFrame::CMainFrame()
{}




CMainFrame::~CMainFrame()
{}



void CMainFrame::OnTimer(UINT_PTR timerid)
{
	KillTimer(timerid);
	if(!WaxSys::isAV())
	{
		//AfxMessageBox(_T("Es können mit dieser Programmversion\nnur noch Projekte geöffnet\ndiese aber nicht mehr freigegeben werden!!") , MB_OK | MB_ICONINFORMATION);
	}
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	/**
	Initialisierung des Fensters 
	Diese Methode wird von der MFC aufgerufen, wenn das Fenster erzeugt wird. 
	Hier wird das Toolbarobject toolbar und das Statusbarobjekt statusbar  
	erzeugt. Das Docking der Toolbar wird erlaubt und das Splash Window angezeigt
	*/
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	const UINT toolbarid =
#ifdef __WAX_FUER_DRITTE__ 	
		IDR_MAINFRAME_3;
#else
		IDR_MAINFRAME;
#endif

	if (!toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!toolbar.LoadToolBar(toolbarid))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	// Zweite Toolbar mit AV Funktionalität
	UINT style = WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | WS_VISIBLE;
	


#ifndef __WAX_FUER_DRITTE__ 	
	if (!toolbarAV.CreateEx(this, TBSTYLE_FLAT, style) ||
		!toolbarAV.LoadToolBar(IDR_AVFUNKTIONEN))	
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
#endif

	const UINT indicators[] =
	{
		ID_SEPARATOR,           // Statusleistenanzeige
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
	};

	if (!statusbar.Create(this) ||
		!statusbar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Statusleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	toolbar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&toolbar);

	// Code snippet von http://codeguru.earthweb.com/toolbar/demo_toolbar_d.shtml
	CRect rect;
	DWORD dw;
	UINT n;
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);

	toolbar.GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=toolbar.GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	

// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);


	rect.OffsetRect(1,0);

#ifndef __WAX_FUER_DRITTE__ 
	toolbarAV.EnableDocking(CBRS_ALIGN_ANY);
		// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(&toolbarAV,n,&rect);
#endif

	spanx_server.start(this);

	
	SetTimer(1, 1000*4, NULL);
	

//	AfxGetMainWnd()->PostMessage(WM_COMMAND, IDC_LIZENZCHECK2);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	/**
	Initialisierung des Fensters 
	Diese Methode wird von der MFC aufgerufen, bevor das Fenster erzeugt wird. 
	An dieser Stelle kann der Fensterstil vor der Erstellung beeinflusst werden.
	*/
	unsigned char *rect = 0;
	unsigned int size = sizeof(RECT);
	AfxGetApp()->GetProfileBinary("Appearance", "MainWindow Position", &rect, &size);
	RECT *r = (RECT*)rect;
	if (r)
	{
		cs.cx = r->right - r->left;
		cs.cy = r->bottom - r->top;
		cs.x = r->left;
		cs.y = r->top;
		delete r;
	}

	CString state = AfxGetApp()->GetProfileString("Appearance", "Main Window State", "SHOWNORMAL");
	
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	if (state == "SHOWMAXIMIZED")
		cs.style |= WS_MAXIMIZE;
	
	return TRUE;
}


//Zeigt einen Dialog mit den User-Einstellungen an
void CMainFrame::onBenutzereinstellungen() 
{
	/**
	Benutzereinstellungen aufrufen
	*/
	WaxSys::edit_user(false);	
}

void CMainFrame::on_systemparameter() 
{
	/**
	Ruft den Einstellungsdialog für die Systemparameter auf. Bei einer
	Änderung der Sprache wird das Hauptmenü neu geladen
	*/
	LCID sprache_vor = ::GetThreadLocale();
	WaxSys::edit_property(this, true, 0);
	LCID sprache_nach = ::GetThreadLocale();

	PostMessage(WM_COMMAND, ID_INITBARS);

	if (sprache_vor != sprache_nach)
	{
		// Systemparameter und Stammdaten neu laden
		((CWaxApp*)AfxGetApp())->init_stammdaten();

		//Texte für Statuszeile löschen
		statuszeilentext.clear();

		//Menüs neu setzen
		modify_menu(menu_default, IDR_MAINFRAME);
		modifyChildMenu();

	}
}


void CMainFrame::modify_menu(BCMenu& menu, UINT id)
{
	HMENU men = GetMenu()->m_hMenu;
	menu.Attach(men);

	if (id == IDR_MAINFRAME)
	{
		//Alle Menüs erstmal löschen
		while (menu.DeleteMenu(0, MF_BYPOSITION));
		
		//Menüs neu aufbauen

		//Menü: Projekt
		
		BCMenu* popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_PROJEKT").c_str());
#ifndef __WAX_FUER_DRITTE__ 
		if (WaxSys::isAV())
		{

			setMenueItem(popUp, ID_OPEN_NEWWA, "ID_OPEN_NEWWA");
			setMenueItem(popUp, ID_OPEN_MYWA, "ID_OPEN_MYWA");
			setMenueItem(popUp, ID_OPEN_DATABASE, "ID_OPEN_DATABASE");
			setMenueItem(popUp, ID_OPEN_DATABASE_PSPELEMENT, "ID_OPEN_DATABASE_PSPELEMENT");
			setMenueItem(popUp, ID_OPEN_DB_PA, "ID_OPEN_DB_PA");
			setMenueItem(popUp, ID_AUFTRAGSTEST, "ID_AUFTRAGSTEST");
			popUp->AppendMenu(MF_SEPARATOR);
		}
		else
		{
			setMenueItem(popUp, IDC_OPEN_AUFTRAEGE, "ID_OPEN_DATABASE_PSP_WANR_SD_ELEMENT");
			popUp->AppendMenu(MF_SEPARATOR);
		}
#endif	
		setMenueItem(popUp, ID_FILE_NEW,"ID_FILE_NEW");
		setMenueItem(popUp, ID_FILE_OPEN,"ID_FILE_OPEN");
		setMenueItem(popUp, ID_FILE_PRINT_SETUP,"ID_FILE_PRINT_SETUP");
		popUp->AppendMenu(MF_SEPARATOR);
		popUp->AppendMenu(MF_BYCOMMAND, ID_FILE_MRU_FILE1, "");
		popUp->AppendMenu(MF_SEPARATOR);
		setMenueItem(popUp, ID_APP_EXIT, "ID_APP_EXIT");

		//Menü: Systemverwaltung
		popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_SYSTEMVERWALTUNG").c_str());
		setMenueItem(popUp, IDM_PARAMETER_DIALOG, "IDM_PARAMETER_DIALOG");

		//UNtermenü: Datenbanken
		BCMenu* popUp2 = popUp->AppendODPopupMenu(getStammdaten().getText("ID_DATENBANK_POPUP").c_str());
		setMenueItem(popUp2, ID_DATABASE, "ID_DATABASE");
		setMenueItem(popUp2, ID_DATABASE_SELECT, "ID_DATABASE_SELECT");
		setMenueItem(popUp2, ID_DATABASE_INFO, "ID_DATABASE_INFO");

#ifdef __WAX_FUER_DRITTE__ 	
		popUp->AppendMenu(MF_SEPARATOR);
		setMenueItem(popUp, IDC_LIZENZCHECK, "IDC_LIZENZ");
#else	
		if (WaxSys::isAV())
		{	
			popUp->AppendMenu(MF_SEPARATOR);
			setMenueItem(popUp, ID_UPDATE_ARTIKELSTAMM, "ID_UPDATE_ARTIKELSTAMM");

			//Untermenü: Admintools (nicht mehr notwendig)
			//popUp2 = popUp->AppendODPopupMenu(getStammdaten().getText("ID_ADMINTOOLS").c_str());
			//setMenueItem(popUp2, ID_DATABASE, "ID_DATABASE_UPDATESTATISTIK");
		}

		//Untermenü: Spanx Rohreingabe
		popUp->AppendMenu(MF_SEPARATOR);
		popUp2 = popUp->AppendODPopupMenu(getStammdaten().getText("ID_SPANX_POPUP").c_str());
		setMenueItem(popUp2, ID_SPANXANFRAGE_STRANGROHR, "ID_SPANXANFRAGE_STRANGROHR");
		setMenueItem(popUp2, ID_SPANXANFRAGE_VERTEILER, "ID_SPANXANFRAGE_VERTEILER");
#endif	

		//Menü: Ansicht
		popUp =  menu.AppendODPopupMenu(getStammdaten().getText("ID_ANSICHT").c_str());
		setMenueItem(popUp, ID_VIEW_TOOLBAR, "ID_VIEW_TOOLBAR");
		setMenueItem(popUp, ID_VIEW_STATUS_BAR,	"ID_VIEW_STATUS_BAR");

		//Menü: Hilfe
		popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_HILFE").c_str());
		setMenueItem(popUp, ID_APP_ABOUT, "ID_APP_ABOUT");
		setMenueItem(popUp, ID_VERSIONSGESCHICHTE, "ID_VERSIONSGESCHICHTE");
		setMenueItem(popUp, ID_DOKU, "ID_DOKU");
	
	}
	menu.Detach();
}

void CMainFrame::setMenueItem(BCMenu* menu, UINT nItemID, std::string dbId)
{
	if (menu == NULL)
		return;

	

	std::string txt = getStammdaten().getText(dbId);

	std::string txtMenu,
				txtStatuszeile;
	
	int index = txt.find_first_of('\n');
	if (index >= 0)
	{
		txtStatuszeile = txt.substr(0, index);
		txtMenu = txt.substr(index + 1, txt.length() - index + 1);

		if (txtStatuszeile.length() > 0)
			statuszeilentext.insert(make_pair(nItemID, txtStatuszeile));
	}
	else
		txtMenu = txt;			

	menu->AppendMenu(MF_BYCOMMAND, nItemID,	txtMenu.c_str());
}


void CMainFrame::modifyChildMenu()
{
	menu.Attach(childMenu);

	//Alle Menüs erstmal löschen
	while (menu.DeleteMenu(0, MF_BYPOSITION));
	
	//Menüs neu aufbauen

	//Menü: Projekt
	BCMenu* popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_PROJEKT").c_str());
	setMenueItem(popUp, ID_FILE_NEW, "ID_FILE_NEW");
	setMenueItem(popUp, ID_FILE_OPEN, "ID_FILE_OPEN");
	setMenueItem(popUp, ID_FILE_CLOSE, "ID_FILE_CLOSE");
	setMenueItem(popUp, ID_FILE_SAVE, "ID_FILE_SAVE");
	setMenueItem(popUp, ID_FILE_SAVE_AS, "ID_FILE_SAVE_AS");
	
	popUp->AppendMenu(MF_SEPARATOR);	
	setMenueItem(popUp, ID_FILE_PRINT, "ID_FILE_PRINT");
	setMenueItem(popUp, ID_FILE_PRINT_SETUP, "ID_FILE_PRINT_SETUP");
	setMenueItem(popUp, ID_FILE_SEND_MAIL, "ID_FILE_SEND_MAIL");

	popUp->AppendMenu(MF_SEPARATOR);
	popUp->AppendMenu(MF_BYCOMMAND, ID_FILE_MRU_FILE1, "");

	popUp->AppendMenu(MF_SEPARATOR);
	setMenueItem(popUp, ID_APP_EXIT, "ID_APP_EXIT");


	//Menü: Bearbeiten
	popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_BEARBEITEN").c_str());
	setMenueItem(popUp, WAXID_CUT, "WAXID_CUT");
	setMenueItem(popUp, WAXID_COPY, "WAXID_COPY");
	setMenueItem(popUp, WAXID_PASTE, "WAXID_PASTE");
	setMenueItem(popUp, WAXID_DELETE, "WAXID_DELETE");
		
	popUp->AppendMenu(MF_SEPARATOR);
	setMenueItem(popUp, WAXID_UMBENENNEN, "WAXID_UMBENENNEN");

#ifndef __WAX_FUER_DRITTE__ 	
	setMenueItem(popUp, ID_FREIGABE, "ID_FREIGABE");
	setMenueItem(popUp, ID_STATUS_AKTUALISIEREN, "ID_STATUS_AKTUALISIEREN");
#endif


	//Menü: Ansicht
	popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_ANSICHT").c_str());
	setMenueItem(popUp, ID_VIEW_TOOLBAR, "ID_VIEW_TOOLBAR");
	setMenueItem(popUp, ID_VIEW_STATUS_BAR, "ID_VIEW_STATUS_BAR");

	popUp->AppendMenu(MF_SEPARATOR);
	setMenueItem(popUp, ID_VIEW_GRAFIK, "ID_VIEW_GRAFIK");
	setMenueItem(popUp, ID_DRUCKVORSCHAU, "ID_DRUCKVORSCHAU");
	setMenueItem(popUp, ID_NOGRAFIK, "ID_NOGRAFIK");
	popUp->AppendMenu(MF_SEPARATOR);
	setMenueItem(popUp, ID_VOLOVIEW, "ID_VOLOVIEW");


	//Menü: Ansicht
	popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_FENSTER").c_str());
	setMenueItem(popUp, ID_WINDOW_CASCADE, "ID_WINDOW_CASCADE");
	setMenueItem(popUp, ID_WINDOW_TILE_HORZ, "ID_WINDOW_TILE_HORZ");
	setMenueItem(popUp, ID_WINDOW_ARRANGE, "ID_WINDOW_ARRANGE");
	popUp->AppendMenu(MF_SEPARATOR);
	setMenueItem(popUp, ID_SHOWEXPLORE, "ID_SHOWEXPLORE");

	//Menü: Hilfe
	popUp = menu.AppendODPopupMenu(getStammdaten().getText("ID_HILFE").c_str());
	setMenueItem(popUp, ID_APP_ABOUT, "ID_APP_ABOUT");
	setMenueItem(popUp, ID_VERSIONSGESCHICHTE, "ID_VERSIONSGESCHICHTE");
	setMenueItem(popUp, ID_DOKU, "ID_DOKU");

	menu.Detach();
}





void CMainFrame::reload_menu() 
{
	/**
	Laden des Hauptmenüs, kann nach einer Sprachänderung aufgerufen werden
	*/
	HMENU hmen = ::LoadMenu(NULL, MAKEINTRESOURCE(IDR_MAINFRAME));

	BCMenu menu;
	menu.Attach(hmen);
	
	// Hauptfenster muß manuell upgedated werden.
	if (AfxGetMainWnd())
	{
		HWND mainwnd = AfxGetMainWnd()->m_hWnd;
		if (::IsWindow(mainwnd))
		{
			::SetMenu(mainwnd, hmen);
			AfxGetMainWnd()->DrawMenuBar( );
		}
	}
	((CWaxApp*)AfxGetApp())->init_doctemplates();
}

BOOL CMainFrame::DestroyWindow() 
{
	/*
	Hauptfenster wird zerstört, Applikation wird beendet
	Hier wird der Fensteraufbau/grösse in der Registry abgespeichert.
	Keys sind "Appearance", "MainWindow Position" -> eine binär gespeicherte RECT Struktur
	"Appearance", "Main Window State" -> String mit SHOWMAXIMIZED oder SHOWNORMAL. Minimiert wird nicht
	abgespeichert, das gibt nur Ärger mit unbedarften Anwendern.  
	*/
	if (::IsWindow(m_hWnd))
	{
		RECT r;
		GetWindowRect(&r);
		// Fenster ist minimiert
		WINDOWPLACEMENT place;
		GetWindowPlacement(&place);
		if (place.showCmd != SW_SHOWNORMAL)
		{
			if (place.showCmd == SW_SHOWMAXIMIZED)
				AfxGetApp()->WriteProfileString("Appearance", "Main Window State", "SHOWMAXIMIZED");
		}
		else
		{
			AfxGetApp()->WriteProfileBinary("Appearance", "MainWindow Position", (unsigned char*)&r, sizeof(r));
			AfxGetApp()->WriteProfileString("Appearance", "Main Window State", "SHOWNORMAL");
		}
	}
	else
	{
		TRACE("Windowpos ist nicht mehr speicherbar!\n");
	}
	
	return CMDIFrameWnd::DestroyWindow();
}

//Zeigt einen Dialog zur Einstellung der benutzten (User)Datenbank an
void CMainFrame::onDatabase() 
{
	/*
	Datenbankpfad eingeben
	Anzeige eines Dialogs mit dem Pfad zur Benutzerdatenbank. In der Benutzerdatenbank sind die Einträge 
	für die Stammdatenbank und die Serverdatenbank hinterlegt.
	*/
	DBAuswahl db;
	if (db.DoModal() == IDOK)
	{
		// Systemparameter und Stammdaten neu laden
		((CWaxApp*)AfxGetApp())->load_databases(db.get_pfad());
	}	
}


void CMainFrame::onUpdateLoadServerDatabase(CCmdUI *pCmdUI)
{
	if (!WaxSys::isAV())
		return;
	pCmdUI->Enable(true);
}




//Liefert den unteren linken Punkt des Toolbarbuttons mit der ID id zurück. 
//Dieser Punkt dient als Koordinate für ein Contextmenü.
CPoint CMainFrame::getToolbarButton(UINT id)
{
	CPoint ret;
	RECT Rect;

#ifdef __WAX_FUER_DRITTE__ 	
	CToolBar &bar = toolbar;
#else
	CToolBar &bar = toolbar.CommandToIndex(id) != -1 ? toolbar : toolbarAV;
#endif
	UINT nid = bar.CommandToIndex(id);
	bar.GetItemRect(nid, &Rect );
	bar.ClientToScreen(&Rect);
	ret.x = Rect.left;
	ret.y = Rect.bottom;
	return ret;
}



//Zeigt einen Dialog mit Infos zur Datenbankverbindung an.
void CMainFrame::onDatabaseInfo()
{
	DBInfo inf;
	inf.DoModal();
}


//Öffnet die Serverdatenbank und erzeugt ein temporäres Projekt mit 
//den freigegebenen Werkaufträgen
void CMainFrame::onLoadServerDatabase()
{
	ServerDBOpen dbo;
	if (dbo.DoModal() == IDOK)
		AfxGetApp()->OpenDocumentFile(dbo.get_fname());
}


void CMainFrame::onOpenNewWa()
{
	ServerDBNewWa dbo(Status::Freigegeben);
	//dbo.set_status();
	if (dbo.DoModal() == IDOK)
	{
		AfxGetApp()->OpenDocumentFile(dbo.get_fname());

		//Das Dokument, das die neuen WaxPart Objekte enthält schreibt die Statistik in die Datenbank
		AfxGetMainWnd()->PostMessage(WM_COMMAND,WAXID_UPDATESTATISTIK);

		//Das Dokument, das die neuen WaxPart Objekte enthält die KC Nummer zuweisen
		AfxGetMainWnd()->PostMessage(WM_COMMAND,WAXID_UPDATEKCNUMMER);

		// Für alle geladenen WAs die Afos schreiben
		AfxGetMainWnd()->PostMessage(WM_COMMAND,WAXID_UPDATEAFOS);
	}
}



/*
	Neue Preisanfragen öffnen
*/
void CMainFrame::onOpenNewPa()
{
	ServerDBNewWa dbo(Status::Preisanfrage);
	if (dbo.DoModal() == IDOK)
	{
		AfxGetApp()->OpenDocumentFile(dbo.get_fname());

		// Für alle geladenen WAs die Afos schreiben
		AfxGetMainWnd()->PostMessage(WM_COMMAND,WAXID_UPDATEAFOS);
	}
}


void CMainFrame::onOpenMyWa()
{
	ServerDBMyWa mywa;
	if (mywa.DoModal() == IDOK)
		AfxGetApp()->OpenDocumentFile(mywa.get_fname());
}


void CMainFrame::onOpenAuftraege()
{
	AskDifferentWerkauftraege psp;
	if (psp.DoModal() == IDOK)
		AfxGetApp()->OpenDocumentFile(psp.get_fname());

}


void CMainFrame::onOpenPspelement()
{
	ServerDBPSPElement psp;
	if (psp.DoModal() == IDOK)
		AfxGetApp()->OpenDocumentFile(psp.get_fname());
}


void CMainFrame::onOpenPreisanfrage()
{
	ServerDbPa pa;
	if (pa.DoModal() == IDOK)
		AfxGetApp()->OpenDocumentFile(pa.get_fname());
}

void CMainFrame::onMonitor()
{
	/*
	Menüpunkt Monitor
	*/
	UINT cmd;
	CMenu pop;

	Monitor monitor;
	monitor.initAction();
	
	pop.CreatePopupMenu();
	for (int i = 0; i < monitor.get_aktionen().GetSize(); i++)
	{
		int id = monitor.get_aktionen().GetAt(i);
		pop.AppendMenu(MF_STRING, id, (LPCTSTR)monitor.getName(id));
	}
	
	CPoint pt = ((CMainFrame*)AfxGetMainWnd())->getToolbarButton(ID_MONITOR);
	cmd = pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, this); 
	monitor.set_currentAction(cmd);
	pop.DestroyMenu();

	if (cmd != 0)
		monitor.makeAction(cmd);
}


//Kann von externen Klassen aufgerufen werden, um die kontextspezifischen Toolbars anzuzeigen
void CMainFrame::showAVToolbars(bool av)
{
#ifndef __WAX_FUER_DRITTE__ 	
	toolbarAV.ShowWindow(av ? SW_SHOW : SW_HIDE);
#endif
	
	RecalcLayout(TRUE);
}



HMENU CMainFrame::newMenu()
{
	static UINT toolbars[] =
	{
#ifdef __WAX_FUER_DRITTE__ 	
		IDR_MAINFRAME_3,
#else
		IDR_MAINFRAME,
#endif
	};

	menu.LoadMenu(IDR_WAXTYPE);

	/*
#ifdef __WAX_FUER_DRITTE__ 	
	menu.DeleteMenu(ID_FREIGABE, MF_BYCOMMAND);
	menu.DeleteMenu(ID_STATUS_AKTUALISIEREN, MF_BYCOMMAND);
	menu.DeleteMenu(ID_VERSIONSGESCHICHTE, MF_BYCOMMAND);
#endif
	*/

	menu.LoadToolbars(toolbars, 1);
	childMenu = menu.Detach();
	return childMenu;
}


HMENU CMainFrame::newDefaultMenu()
{
	menu_default.LoadMenu(IDR_MAINFRAME);
#ifdef __WAX_FUER_DRITTE__ 	
	menu_default.LoadToolbar(IDR_MAINFRAME_3);
#else
	menu_default.LoadToolbar(IDR_MAINFRAME);
#endif
	return menu_default.Detach();
}

// Measure item implementation relies on unique control/menu IDs
void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	BOOL setflag=FALSE;
	if(lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		if (IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu = CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			if(menu.IsMenu(cmenu) || menu_default.IsMenu(cmenu))
			{
				menu.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}
	if (!setflag)
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);

}

LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu *pMenu)
{
	LRESULT lresult;
	if(menu.IsMenu(pMenu) || menu_default.IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	return lresult;
}


void CMainFrame::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if(!bSysMenu)
	{
		if(menu.IsMenu(pPopupMenu) || menu_default.IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}

void CMainFrame::on_spanx_strangrohr()
{
	on_spanx_strangrohr(0, 0);
}

LRESULT CMainFrame::on_spanx_strangrohr(WPARAM wparam, LPARAM lparam)
{
	/**
	Ruft den Strangrohrdialog auf. Parent vom Dialog ist der Desktop, damit nicht 
	das ganze Wax+ Fenster hochkommt. In der Dialogresource ist angeknipst, dass
	der Dialog in den Vordergrund gesetzt werden soll.
	*/
	UNREFERENCED_PARAMETER(wparam);
	string data;
	if (lparam)
		data = (char*)lparam;
	SpanxStrangrohrDialog dlg(GetActiveWindow(), data);
	if (dlg.DoModal() == IDOK)
	{
		spanx_server.write_return_pipe(dlg.xml_data().c_str());
	}
	else
	{
		spanx_server.write_return_pipe("CANCELLED");
	}
	return 0;
}


void CMainFrame::on_spanx_verteiler()
{
	on_spanx_verteiler(0, 0);
}


LRESULT CMainFrame::on_spanx_verteiler(WPARAM wparam, LPARAM lparam)
{
	/**
	Ruft den Verteilerdialog auf. 
	*/
	UNREFERENCED_PARAMETER(wparam);
	string data;
	if (lparam)
		data = (char*)lparam;
	SpanxVerteilerDialog dlg(GetActiveWindow(), data);
	if (dlg.DoModal() == IDOK)
	{
		spanx_server.write_return_pipe(dlg.xml_data().c_str());
	}
	else
	{
		spanx_server.write_return_pipe("CANCELLED");
	}
	return 0;
}



void CMainFrame::on_auftragstest()
{
	Auftragstest auftragstest(this);
	auftragstest.DoModal();
}

void CMainFrame::init_bars_and_menues()
{
	ShowControlBar(&toolbar, WaxSys::para_int(SYMBOLLEISTE), false);
	ShowControlBar(&statusbar, WaxSys::para_int(STATUSZEILE), false);
	modify_menu(menu_default, IDR_MAINFRAME);
	showAVToolbars(WaxSys::isAV());
	modifyChildMenu();
}



void CMainFrame::onDatabaseUpdateStatistik()
{
	
	Passwd passwd(this,WaxSys::getPwd());
	if (passwd.DoModal() != IDOK)
		return;

	if (!passwd.check_passwd())
		return;
	

	BefehlGesamtStatistkNeuSchreiben befehl;

	befehl.start();
	
}

void CMainFrame::switch_av()
{
#ifdef _DEBUG
	WaxSys::switch_av();
	string msg;
	msg = (string)"Umgeschaltet auf " + (string)(WaxSys::isAV() ? " AV Version" : " NL Version");
	statusbar.SetWindowText(msg.c_str());
#endif
}




// Add handler for TTN_NEEDTEXT(A/W)
BOOL CMainFrame::OnDynamicTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	// call one global single handler - see NeedText.cpp for details.
	return ::OnNeedText(id,pNMHDR,pResult, &statusbar);
}

/**
	Text für die Statusleiste holen
*/
void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(hSysMenu);

	string text;

	map <int, string> :: const_iterator iter;
    iter = statuszeilentext.find(nItemID);

	if (iter != statuszeilentext.end())
		text = iter->second;
	
	statusbar.SendMessage(SB_SETTEXT,0,(LPARAM)text.c_str());

	return;
}



void CMainFrame::onUpdateArtikelstamm()
{

	CImportSAPToWAX ImportSAPToWAX;
	ImportSAPToWAX.DoModal();

	if(ImportSAPToWAX.m_bImported == TRUE)
	{
		// Systemparameter und Stammdaten neu laden
		((CWaxApp*)AfxGetApp())->init_stammdaten();
	}
	statuszeilentext.clear();

	//Menüs neu setzen
	modify_menu(menu_default, IDR_MAINFRAME);
	modifyChildMenu();

}