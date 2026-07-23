#include "stdafx.h"

#include "wax.h"
#include "resource.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "LeftView.h"
#include "waxdoc.h"
#include "WaxDocManager.h"
#include "aboutdlg.h"

#include <ODBCINST.H>

#include <mxodbc\mxodbc.h>
#include <mxcontrols/Splash.h>

#include "DBAuswahl.h"
#include "DBPrompt.h"


#include "TabCtrlView.h"
#include "lizenz.rc.h"

#include <mxbase/localesaver.h>
#include <mxcontrols/localizedlg.h>
#include "WaxBrowser.h"



#ifdef __WAX_FUER_DRITTE__
#include "lizenzeingabe.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CWaxApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, on_about)
	ON_COMMAND(ID_VERSIONSGESCHICHTE, on_versionsgeschichte)
	ON_COMMAND(ID_DOKU, on_doku)
	ON_COMMAND(ID_DATABASE_SELECT, on_database_select)
	ON_COMMAND(ID_FILE_OPEN, on_file_open)
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, on_file_open)
	// Standard-Druckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(IDC_LIZENZCHECK, on_lizenz)
	ON_COMMAND(IDC_LIZENZCHECK2, on_lizenz_test)
	ON_COMMAND(ID_LANGUAGE, on_language)
END_MESSAGE_MAP()

CWaxApp theApp;

bool CWaxApp::m_Posting_WM_COMMAND_IDTESTTAB_OnActivate = true;

CWaxApp::CWaxApp()
{
	BCMenu::SetIconSize(20, 18);
	//BCMenu::SetMenuDrawMode(BCMENU_DRAWMODE_XP);

	
	// Toolstips dynamisch laden
	NoDynamicTips = FALSE;

}

CWaxApp::~CWaxApp()
{
	WaxSys::unload();
}



BOOL CWaxApp::InitInstance()
{
	/**
	Anwendung initialisieren
	Folgende Initialisierungen werden hier vorgenommen:

	  1. Splash Screen wird angezeigt und die Stammdaten geladen (Fortschrittsanzeige im Splash Screen)

	  2. MFC Standardinitialisierungen:
	  AfxEnableControlContainer() - Anzeige von Ole Controls ermöglichen. 
	  Enable3dControls() - 3D Look 
	  init_doctemplates()> - Dokumentverknüpfungen setzen
	  CMainFrame::DragAcceptFiles() - Drag and Drop ermöglichen  
	  EnableShellOpen() - Verknüpfung mit Dateiendung initialisieren. Doppelklick auf .Wax Dateien startet 
	  immer Wax+. Man braucht kein Setup dafür
	  RegisterShellFileTypes() - "adds registration entries for shell commands Print and Print To, allowing a user to print files directly from the shell, or by dragging the file to a printer object. It also adds a DefaultIcon key"
	  ParseCommandLine() - Befehlszeile parsen, um zu prüfen auf Standard-Umgebungsbefehle DDE, Datei offen
	  ProcessShellCommand() - Verteilung der in der Befehlszeile angegebenen Befehle
  
	  3. Sprache wird gesetzt (Registry Key:HKEY_CURRENT_USER/Minimax/Wax+/Common/Language)
	  Dieser Wert kann sein "Deutsch" : Deutsche Sprache wird gesetzt, jeder andere Wert: Englische Sprache
	  wird gesetzt.


	Es sollte darauf geachtet werden, dass alle Ini-Werte unter diesem Registry Key abgelegt werden. 
	Ausnahme sind alle Werte, die die Systemparameter überschreiben, da hier der Benutzer selber editieren 
	kann. Ziel ist es, alle Initialisierungen von Spanx+ und Wax+ unter dem Registry Key "Minimax" abzulegen.

	Die Dateien Wax.ini und Spanx.ini sollen nur noch dazu dienen, Systemparameter lokal zu überschreiben.

	*/
	SetRegistryKey(_T("Minimax"));

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(cmdInfo.m_strFileName.Find(_T(".wax")) == -1 )
	{
		cmdInfo.m_strFileName = m_lpCmdLine;
	}
	if(!cmdInfo.m_strFileName.GetLength())
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}
	AfxEnableControlContainer();
	LoadStdProfileSettings(10);  // Standard INI-Dateioptionen laden (einschließlich MRU)
		
	init_language();

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	init_doctemplates();
	
	// Splash Screen initialisieren: Bitmap, Anwendungsname, Versionsstring 
	// und default string für Fortschrittanzeige
	
	CSplashWnd::set_bmp(IDB_SPLASH);
	CSplashWnd::set_title(IDS_APPNAME);
	CSplashWnd::set_version(IDR_MAINFRAME);
	CSplashWnd::set_startmsg(IDS_OPEN_DB);
	

	if (!test_db())
		return false;

	bool result = true;
	if (!WaxSys::hotkey_pressed(VK_CONTROL, VK_SHIFT))
	{
		if (!init_stammdaten())
			result = false;
	}
	else
	{
		DBConnection con;
		DBAuswahl db;
		con.set_userdbentry(db.get_pfad());
	
		CSplashWnd::EnableSplashScreen(TRUE);
		CSplashWnd::ShowSplashScreen(NULL);
		WaxSys::load(con.get_userdb());
	}

/*
	if(!WaxSys::isAV())
	{
#ifndef _DEBUG
		checkNewVersion("waxgl.dll");
		checkNewVersion("waxstamm.dat");
#endif
	}
*/
	//Lizenz testen
	on_lizenz_test();
	//AfxGetMainWnd()->PostMessage(WM_COMMAND, IDC_LIZENZCHECK2);

	if (!result)
		return false;


	init_logo();
	
	
	// Öffnen per DragDrop aktivieren
	m_pMainWnd->DragAcceptFiles();

	// DDE-Execute-Open aktivieren
	EnableShellOpen();
	RegisterShellFileTypes();

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (WaxSys::isAV())
	{
		CSplashWnd::EnableSplashScreen(FALSE);
	}
	ProcessShellCommand(cmdInfo);

	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_INITBARS);
	WaxSys::edit_user(true);
	AfxGetMainWnd()->BringWindowToTop();

	return TRUE;
}



void CWaxApp::checkNewVersion(CString strDetect)
{
		TCHAR path[_MAX_PATH];
		GetModuleFileName(0, path, _MAX_PATH);
		CString strMeMySelf = path;
		int iWhere = strMeMySelf.ReverseFind('\\');
		strMeMySelf = strMeMySelf.Mid(0, iWhere + 1);
		#ifdef _DEBUG
			if(strDetect.Compare(_T("waxgl.dll")) == 0)
			{
				strMeMySelf += _T("waxgld.dll");		
			}
			else
			{
				strMeMySelf += strDetect;
			}
		#else
			strMeMySelf += strDetect;
		#endif
				
		LPCTSTR lpName = _T("INSTALLSERVER");
		GetEnvironmentVariable(  lpName,  path, _MAX_PATH);
		CString strWaxNetz = CString(_T("\\\\")) + path;
		strWaxNetz += _T("\\vol1\\install\\NOVELL\\zen\\TECH\\Ins_WAXplus_Lokal\\") + strDetect;
		
		CString temp = strMeMySelf.Mid(0,3);
		UINT  iLoacalDrive = GetDriveType(temp);
		if(DRIVE_FIXED == iLoacalDrive)
		{			
			CFileStatus statusLocal;
			if (CFile::GetStatus(strMeMySelf, statusLocal))
			{
				int tDayCreatedMeMySelf = statusLocal.m_mtime.GetDay();
				int iMonth				= statusLocal.m_mtime.GetMonth();

				CFileStatus statusNetz;
				if (CFile::GetStatus (strWaxNetz, statusNetz))
				{
					int tDayCreatedNetz = statusNetz.m_mtime.GetDay();
					int iMonthNetz		= statusLocal.m_mtime.GetMonth();
					if((tDayCreatedMeMySelf != tDayCreatedNetz && iMonth == iMonthNetz) || (tDayCreatedMeMySelf == tDayCreatedNetz && iMonth != iMonthNetz))
					{
						AfxMessageBox(getStammdaten().getText("WAX_NEW_VERSION").c_str());
					}
				}
			}
		}
}



BOOL CWaxApp::OnDDECommand(LPTSTR lpszCommand)
{
	AfxMessageBox("DDE");
	if (m_pDocManager != NULL)
		return m_pDocManager->OnDDECommand(lpszCommand);
	else
		return FALSE;
}


void CWaxApp::on_versionsgeschichte()
{
	CString ver; ver = getStammdaten().getText("IDS_VERSIONSGESCHICHTE").c_str();
	CString docdir = WaxSys::para_str(DIR_DOC);
	CString verfile = ".\\" + docdir + "\\" + ver;

	if ((DWORD)ShellExecute(NULL, "open", verfile, NULL, NULL, SW_SHOWNORMAL) < 31)
	{
		CString str; 
		str.Format(getStammdaten().getText("IDS_ERROR_VERSIONHISTORYNOTFOUND").c_str(), ver);
		AfxMessageBox(str, MB_ICONERROR);
	}
}


bool CWaxApp::test_db()
{
	/**
	Prüfung wird nur ausgeführt, wenn keine Datenbankauswahl eingestellt wurde. 
	Bei Datenbankauswahl wird erst nach erfolgter Auswahl geprüft.
	*/

	DBAuswahl db;

	if(db.ShouldApplicationfinishUserdbNotFound())
	{
		return false;
	}
	// Es wurde eingestellt, dass beim Start eine Datenbank ausgewählt werden soll
	if (db.get_prompt())
		return true; 
	
	// Test auf Connection zur Userdb
	DBConnection con;
	con.set_userdbentry(db.get_pfad());
	
	MXTable test(con.get_userdb(true), "Systemparameter");
	if (test.getCount() == -1)
	{
		if (db.DoModal() != IDOK)
			return false;
	}

	return true;
}


void CWaxApp::on_doku()
{
	CString doc;
#ifdef __WAX_FUER_DRITTE__
	doc = getStammdaten().getText("IDS_DOCNAME3").c_str();
#else
	doc = getStammdaten().getText("IDS_DOCNAME").c_str();
#endif
	CString docdir = WaxSys::para_str(DIR_DOC);
	CString docfile = ".\\" + docdir + "\\" + doc;

	if ((DWORD)ShellExecute(NULL, "open", docfile, NULL, NULL, SW_SHOWNORMAL) < 31)
	{
		CString str; 
		str.Format(getStammdaten().getText("IDS_ERROR_DOCNOTFOUND").c_str(), doc);
		AfxMessageBox(str, MB_ICONERROR);
	}
}

int CWaxApp::ExitInstance() 
{
	/**
	Programm beenden
	Systemparamter werden entladen durch <mf _WaxSys::UnLoadSysParm>
	*/
	delete CSplashWnd::splashWnd;
	return CWinApp::ExitInstance();
}



BOOL CWaxApp::PreTranslateMessage(MSG* pMsg)
{
	/**
	Vorverarbeitung von Messages
	Wird gebraucht, um das Splash Window bei Tastendruck/Mausklick auszublenden
	Kann evtl. entfallen, wenn das Laden der Stammdaten am Anfang eh ne Zeit braucht
	*/

		// Tabumschaltung, funktioniert nur, wenn der Focus auf der Tabbezeichnung liegt

#ifdef _DEBUG  //Zum besseren debuggen der Applikation, damit die WM_COMMAND Message nicht immer beim Activieren erzeugt wird
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE )
	{
		 m_Posting_WM_COMMAND_IDTESTTAB_OnActivate = false;
	}
#endif

	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;


	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_KEYDOWN)
	{

			CChildFrame* pChildFrame =(CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
			if( pChildFrame && pChildFrame->GetSplitterWnd()->GetIsChildSplitterActive())
			{	
				
				CTabCtrlView* pTabView =	dynamic_cast<CTabCtrlView*>(pChildFrame->GetSplitterWnd()->GetWaxSplitterChildView()->GetPane(0,0)); //Could be old HtmlView or JScriptview for call print
			
				if(pTabView && pTabView->PreTranslateMessage(pMsg))
				{					
					return TRUE;// Eat the message, if something is going wrong => !TestTabs()
				}
			}// so TRUE for eaten
		
	}

	return CWinApp::PreTranslateMessage(pMsg);
}



//Document Templates initialisieren
void CWaxApp::init_doctemplates()
{
	/**
	Dokumenttemplates initialisieren
	Haupt-MDI-Rahmenfenster wird erzeugt
	*/

	if (m_pDocManager)
	{
		delete m_pDocManager;
		m_pDocManager = NULL;
	}
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_WAXTYPE,
		RUNTIME_CLASS(CWaxDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CLeftView));
	add_doctemplate(pDocTemplate);
	

	// Haupt-MDI-Rahmenfenster erzeugen
	if (!m_pMainWnd)
	{
		CMainFrame* pMainFrame = new CMainFrame;
		m_pMainWnd = pMainFrame;
		((CMainFrame*)m_pMainWnd)->LoadFrame(IDR_MAINFRAME);
	}
	else
	{
		CString strFullString, strTitle;
		if (strFullString.LoadString(IDR_MAINFRAME))
		{
			AfxExtractSubString(strTitle, strFullString, 0);
			((CMainFrame*)m_pMainWnd)->SetTitle(strTitle);
		}
	}
	pDocTemplate->m_hMenuShared = ((CMainFrame*)m_pMainWnd)->newMenu();
	((CMainFrame*)m_pMainWnd)->m_hMenuDefault = ((CMainFrame*)m_pMainWnd)->newDefaultMenu();

	((CMainFrame*)m_pMainWnd)->OnUpdateFrameMenu(((CMainFrame*)m_pMainWnd)->m_hMenuDefault);
	
}




void CWaxApp::init_logo()
{
	/**
	Datei minimax.jpg erzeugen
	Diese Datei wird bei Druckausgaben benötigt und muss im Temp Verzeichnis liegen
	*/
	CString logo = WaxSys::GetTempDir() + "minimax.jpg";
	HRSRC rclog = ::FindResource(NULL, (LPSTR)"LOGO", (LPSTR)"JPG");
	
	if (rclog)
	{
		DWORD size = ::SizeofResource(NULL, rclog);
		HGLOBAL hlog = ::LoadResource(NULL, rclog);
		char* bmp = (char*)::LockResource(hlog);
		if (bmp)
		{
			::DeleteFile(logo);
			TRY
			{
				CFile fbmp(logo, CFile::modeReadWrite | CFile::modeCreate);
				fbmp.Write(bmp, size);
				fbmp.Close();
			}
			CATCH(CFileException, e)
			{
				e->ReportError();
			}
			END_CATCH
		}
		else
		{
			TRACE("Logoresource nicht gefunden\n");
		}
	}
	else
	{
		TRACE("Kein Zugriff auf Resource\n");
	}
}



bool CWaxApp::init_stammdaten()
{
	/**
	Wird beim Programmstart aufgerufen. Hier wird geprüft, ob eine Datenbankauswahl stattfinden soll. Falls
	ja, wird der Dialog zur Datenbankauswahl angezeigt und die Systemparameter/Stammdaten geladen
	*/

	DBAuswahl db;

	// Es wurde eingestellt, dass beim Start eine Datenbank ausgewählt werden soll
	if (db.get_prompt())
	{
		on_database_select();
	}
	else
	{
		// Default ist erster Eintrag, i.A. Serverdatenbank
		CString connect = db.get_pfad();
		load_databases(connect);
	}
	if (!WaxSys::get_userdb().length())
		return false;
	return true;
}

void CWaxApp::on_database_select()
{
	/**
	Zeigt den Auswahldialog für die Userdatenbank an
	*/
	DBPrompt p;

	if (p.DoModal() == IDOK)
	{
		CString connect = p.get_dbname();
		
		CFileStatus status;
		if( !connect.IsEmpty() )
		{	
			if( !CFile::GetStatus( connect, status ))
			{
				AfxMessageBox(getStammdaten().getText("IDS_WRONG_USER_DATABASE").c_str(), MB_OK);
			}
			else
			{
				load_databases(connect);
			}
		}
		else
		{
			AfxMessageBox(getStammdaten().getText("IDS_WRONG_USER_DATABASE").c_str(), MB_OK);
		}
	}
}

void CWaxApp::load_databases(const CString& userdb)
{
	/**
	Lädt die Userdatenbank/Systemparameter aus userdb und die Stammdaten aus darin 
	eingetragenen Stammdatenbank
	*/
	TRY
	{
		DBConnection con;
		con.set_userdbentry(userdb);
		
		WaxSys::load(con.get_userdb(true));
				
		DBConnection connect;
		// In den Systemparametern ist der Eintrag für die Stammdatenbank 
		MXTable teststamm(WaxSys::getStammdb(&connect), "niederlassungen");
		
		if (teststamm.getCount() == -1)
		{
			AfxMessageBox(getStammdaten().getText("IDS_WRONGDATABASE2").c_str(), MB_OK);
			AfxGetMainWnd()->ShowWindow(SW_SHOW);
			AfxGetMainWnd()->UpdateWindow();
		}
		else
		{	
			CSplashWnd::EnableSplashScreen(TRUE);
			CSplashWnd::ShowSplashScreen(NULL);
			getStammdaten().loadData((LPCSTR)WaxSys::getStammdb(&connect), CSplashWnd::splashWnd ? &CSplashWnd::splashWnd->progress : NULL);
		}
		
	}
	CATCH_ALL(e)
	{
		WaxSys::unload();
		PostQuitMessage(-1);
	}
	END_CATCH_ALL
}

void CWaxApp::on_file_open()
{
	/**
	Leider ist die OnFileOpen Methode in CWinApp nicht virtuell. 
	*/
	OpenDocumentFile("");
}



void CWaxApp::add_doctemplate(CDocTemplate* pTemplate)
{
	if (m_pDocManager == NULL)
		m_pDocManager = new WaxDocManager;
	
	CWinApp::AddDocTemplate(pTemplate);
}


void CWaxApp::on_lizenz()
{
	/**
	Zeigt den Dialog für die Lizensierung an
	*/
	check_lizenz(true);
}


void CWaxApp::on_lizenz_test()
{
	/**
	Prüft die Lizenz und zeigt ggf. den Dialog für die Lizensierung an
	*/
	check_lizenz(false);
}


bool CWaxApp::check_lizenz(bool force) const
{
	/**
	Lizenztestdialog wird nur angezeigt, wenn force = true ist oder
	die Lizenznummer falsch
	*/

#ifdef __WAX_FUER_DRITTE__
	
		CWnd *wnd = AfxGetMainWnd();
		Lizenzeingabe lic(wnd, this);
		if (!lic.is_valid() || force)
		{
			lic.DoModal();
		}
		if (!lic.is_valid())
		{
			PostQuitMessage(0);
		}
	
#else
	UNREFERENCED_PARAMETER(force);
#endif

	return true;
}


void CWaxApp::on_about()
{
	/**
	"Über Wax" Dialog anzeigen
	*/
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CWaxApp::on_language()
{
	LocalizeDlg dlg(IDD_LANGUAGE, "Wax");
	std::vector<MxBase::Locale> loc;
	loc.push_back(MxBase::Locale(MxBase::Locale::DEUTSCH));
	loc.push_back(MxBase::Locale(MxBase::Locale::ENGLISH));
	dlg.set_locales(loc);
	dlg.run();
}


void CWaxApp::init_language()
{
	using namespace MxBase;
	Locale loc = LocaleSaver().load("Wax");
	if (loc == Locale(Locale::UNDEFINED))
	{
		loc = Locale(Locale::DEUTSCH);
		//	on_language();
	}
	BaseDialog::set_locale(loc);
}