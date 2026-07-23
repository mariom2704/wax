#if !defined _WAX_H_
#define _WAX_H_

#pragma once

/**
Applikationsklasse
Diese Klasse initialisiert die Anwendung, z.B. die Document Templates und die 
Verknüpfung mit der WAX Dateiendung
*/
class CWaxApp : public CWinApp  
{
	DECLARE_MESSAGE_MAP()
	friend class CMainFrame;
public:
	static bool m_Posting_WM_COMMAND_IDTESTTAB_OnActivate;
	

	/// Konstruktor
	CWaxApp();

		/// Destruktor
	~CWaxApp();

	BOOL NoDynamicTips;

	void checkNewVersion(CString strVersion);


	private:
		static string m_strPurePath; 

	

		
		
protected:

	/// Lädt die Userdatenbank/Systemparameter aus userdb und die Stammdaten aus den darin eingetragenen Stammdatenbank
	void load_databases(const CString& userdb);
	
	/// Document Templates initialisieren
	void init_doctemplates();

	
	/// Vorverarbeitung von Messages
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL InitInstance();

	virtual int ExitInstance();

	void on_about();
	
	void init_language();

	void on_language();

	void on_next_window();

	void on_versionsgeschichte();

	void on_doku();

	void on_lizenz_test();

	void on_lizenz();

	virtual BOOL OnDDECommand(LPTSTR lpszCommand);

	/// Leider ist die OnFileOpen Methode in CWinApp nicht virtuell.
	void on_file_open();

	/// Zeigt den Auswahldialog für die Userdatenbank an
	void on_database_select();
	
private:

	void add_doctemplate(CDocTemplate* pTemplate);

	/// Erstellt das Logo minimax.jpg im temporären Verzeichnis, das Logo wird für Druckausgaben gebraucht
	void init_logo();

	/// Lädt die Stammdaten
	bool init_stammdaten();

	///Testet in der Version für dritte den Lizenzschlüssel
	bool check_lizenz(bool force = false) const;

	/// Prüft die Standarduserdatenbank auf Vorhandensein
	bool test_db();
	
	
	
};


#endif 
