#pragma once

#include <mxcontrols/bcmenu.h>

/**
MDI Main Window
*/
class CMainFrame : public CMDIFrameWnd  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CMainFrame)
public:
	/// Standardkonstruktor
	CMainFrame();

	/// Standarddestruktor
	virtual ~CMainFrame();

	void OnTimer(UINT_PTR timerid);

	/// Liefert den unteren linken Punkt des Toolbarbuttons mit der ID id zurück. Dieser Punkt dient als Koordinate für ein Contextmenü.
	CPoint getToolbarButton(UINT id);

	/// Kann von externen Klassen aufgerufen werden, um die kontextspezifischen Toolbars anzuzeigen
	void showAVToolbars(bool av = false);

	BOOL OnDynamicTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

protected:
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);

private:
	friend class CWaxApp;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	
	HMENU newMenu();
	HMENU newDefaultMenu();

	BCMenu menu;
	HMENU childMenu;

	BCMenu menu_default;

	/// Laden des Hauptmenüs
	void reload_menu();

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	/// Zeigt einen Dialog mit den User-Einstellungen an
	void onBenutzereinstellungen();
	
	/// Zeigt einen Dialog an um die Parameter zu editieren
	void on_systemparameter();
	
	/// Zeigt einen Dialog zur Einstellung der benutzten (User)Datenbank an
	void onDatabase();
	
	void onUpdateLoadServerDatabase(CCmdUI *pCmdUI);

	
	
	/// Öffnet die Serverdatenbank und erzeugt ein temporäres Projekt mit den freigegebenen Werkaufträgen
	void onLoadServerDatabase();
	
	/// Zeigt einen Dialog mit Infos zur Datenbankverbindung an.
	void onDatabaseInfo();


	//Statistik updaten
	void onDatabaseUpdateStatistik();

	void setMenueItem(BCMenu* menu, UINT nItemID, std::string dbId);

	void onOpenNewWa();

	void onOpenNewPa();
	
	void onOpenMyWa();

	void onOpenPreisanfrage();
	
	void onMonitor();
	
	void onOpenPspelement();

	void onOpenAuftraege();


	virtual void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
	LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu *pMenu);
	
	void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu);
	
	void on_auftragstest();
	
	/// Anzeigen oder Ausblenden der Toolbar und der Statusleiste, je nach Einstellung
	void init_bars_and_menues();

	/// Toolbar mit allgemeinen Funktionen
	CToolBar toolbar;

	/// Statuszeile
	CStatusBar statusbar;

	
	map<int, string> statuszeilentext;

#ifndef __WAX_FUER_DRITTE__ 
	/// Toolbar mit AV Funktionen
	CToolBar toolbarAV;
#endif

	/// Serverthread für Spanx+ Anfragen
	SpanxServer spanx_server;

	/// Preisermittlung für Strangrohr
	LRESULT on_spanx_strangrohr(WPARAM wparam, LPARAM lparam);

	/// Preisermittlung für Verteiler
	LRESULT on_spanx_verteiler(WPARAM wparam, LPARAM lparam);

	/// Preisermittlung für Strangrohr
	void on_spanx_strangrohr();

	/// Preisermittlung für Verteiler
	void on_spanx_verteiler();

	/// Menü anpassen
	void modify_menu(BCMenu& menu, UINT id);
	void modifyChildMenu();

	/// Switch in der Debugversion zwischen AV/NL Version über Ctrl+Shift+A
	void switch_av();

	void onUpdateArtikelstamm();

	Fortschrittanzeige progress;

	

};

