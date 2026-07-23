#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "mxparameter.h"
#include <vector>
#include <string>

/**
Ein Zugriff auf die Systemeinstellungen kann nur über das globale Objekt WaxSys 
erfolgen. Zu diesem Zweck ist die Klasse nicht als MXPARAMDLL deklariert. 
Es existiert nur ein Objekt dieser Klasse (WaxSys)
*/
class WaxSys
{
private:
	//Konstruktor, nicht öffentlich
	WaxSys();
	
	//Destruktor, nicht öffentlich
	~WaxSys();

	/// Liste mit Tempfiles
	static std::vector<std::string> tmp_file_lst;
public:
	/// Entladen der Systemparameter
	static MXPARAMDLL void unload();
		
	/// Eigenschaften-Dialog
	static MXPARAMDLL void edit_property(CWnd* pWndParent, bool check, int defaultpage);

	/// Liefert den Namen des angemeldeten Benutzers
	static MXPARAMDLL std::string get_username();

	//Liefert den Connectstd::string für die Userdatenbank
	static MXPARAMDLL std::string get_userdb();

	//Stammdatenbank testen und Datenbankname zurückliefern
	static MXPARAMDLL CString getStammdb();

	static MXPARAMDLL CString verifyLocationOfStammDB(); 

	//Stammdatenbank testen und Datenbankname zurückliefern
	static MXPARAMDLL CString getStammdb(void * connection);

	static MXPARAMDLL CString getSAPSoapServer();

	//Serverdatenbank testen und Datenbankname zurückliefern
	static MXPARAMDLL CString getServerdb();

	static MXPARAMDLL std::string  getEmail();
	static MXPARAMDLL std::string  getEmailfromDefaultUser();
	static MXPARAMDLL std::string  getNLFromDefaultUser();

	//Rechte im Wax
	enum RECHTE
	{
		Niederlassung = 0,
		Av = 1,
		AvAdmin = 2,

		Administrator = 10
	};

	//Abfrage User AV Rechte hat
	static MXPARAMDLL bool isAV();

	//Abfrage ob User AV Admin Rechte hat
	static MXPARAMDLL bool isAvAdmin();

	//Abfrage ob User Administrator Rechte hat
	static MXPARAMDLL bool isAdministrator();



	//Sprache
	static MXPARAMDLL std::string getLanguage();

	static MXPARAMDLL std::string getStandardLanguage();

	/// Schaltet zwischen NL/AV Version um
	static MXPARAMDLL void switch_av();

	/// Keyboardabfrage, ob beim Aufruf dieser Methode die übergebene Tastenkombination gedrückt ist
	static MXPARAMDLL bool hotkey_pressed(UINT key1, UINT key2 = 0);

	/// Laden der Systemeinstellungen
	static MXPARAMDLL void load(const char* dbname);

	/// Benutzereinstellungsdialog anzeigen
	static MXPARAMDLL void edit_user(bool detect_new_user);

	/// Abfrage des Tempverzeichnisses
	static MXPARAMDLL const CString GetTempDir() ;

	/// Abfrage eines temporären Dateinamens
	static MXPARAMDLL std::string GetTempFilename(const std::string& ext);

	static MXPARAMDLL void add_tmpfile(const char* fname);
	
	/// Loeschen der temporaeren Dateien
	static MXPARAMDLL void remove_tmpfiles();

	/// Integer Systemparameter
	static MXPARAMDLL CString para_str(int parameter);

	static MXPARAMDLL std::string para_cstr(int parameter);

	/// std::string Systemparameter
	static MXPARAMDLL int para_int(int parameter);

	static MXPARAMDLL void save_string(const char* ext, int parameter);

	/// Initialisiert eine Combobox mit Usereinträgen
	static MXPARAMDLL void user_list(CComboBox *bx);

	///Paßwort aus Db
	static MXPARAMDLL std::string getPwd();

	/// Parameter 
	enum PARAMETER 
	{  
		LANGUAGE,	/// Allgemein - Sprache
		EMPFAENGER, /// Email - Empfängeradresse
		DISPLAYNAME, /// Email - Empfängername
		BETREFF,/// Email - Betreff
		TRENNZEICHEN, /// Gewinderohr - Trennzeichen
		VIEWERVORSCHAU, /// Graphik - Viewervorschau
		VIEWERDRUCK, /// Graphik - Viewerdruck
		IB_NL_KENNUNG, /// Projekt - Kennung
		LIEFERFRIST, /// Projekt - Lieferfrist
		EXCEL, /// Projekt - Excel
		
		PROJEKTE, /// Projekt - Projekte
		KOMP, /// Projekt - Komp
		AUTOSAVE, /// Allgemein - Autosave
		BAUMFARBE, /// Ansicht - Baumfarbe
		GRAPHIKFENSTER, /// Ansicht - Graphikfenster
		SHOWFREIGABEPOS, /// Ansicht - Showfreigabepos
		ICONGROESSE, /// Ansicht - Icongroesse
		SYMBOLLEISTE,
		STATUSZEILE,
		ZMASSLISTE, /// Gewinderohr - ZmassListe
		
		ZMASSGRAPHIK, /// Gewinderohr - ZmassGraphik
		TEXTFORMAT, /// Gewinderohr - Textformat
		DN2ZOLL, /// Gewinderohr DN2Zoll 
		WINKELAUSRICHTUNG,	/// Gewinderohr - WinkelAusrichtung
		FONTGROESSE, /// Graphik - Fontgroesse
		BEMASSUNGSLINIE, /// Graphik - Bemassungslinie
		TRENNLINIE, /// Graphik - Trennlinie
		RAHMEN,/// Graphik - Rahmen
		BESCHRIFTUNGLAENGE,/// Graphik - Beschriftunglaenge
		AUTOFREIGABE,/// Freigabe - CheckFreigabe
		
		SAFEFREIGABE,/// Freigabe - SaveFreigabe
		CHECKSTATUS,/// Freigabe - CheckStatus
		CHECKSAP, /// Freigabe - CheckSap
		STAMMDB, /// System - Stamm DB
		SERVERDB, /// System - Server DB
		MAXPOSPERSA,/// System - Max Pos Per SA
		AV,/// System - AV
		DEBUGNL,/// System - Debug NL
		BEARBEITER,/// Bearbeiter 
		GF_DN2ZOLL, /// Graphik - DN2Zoll (Verteiler etc.)
		
		GF_DWFHOEHE,
		REL_ABST_AENDERN,
		MBTERMIN,
        DIR_GROHR_FREE,
		DIR_GROHR_ACTIVE,
		DIR_GROHR_STOPPED,
		DIR_GROHR_FINISHED,
        DIR_GROHR_ARCHIVE,
		DIR_SAEGENUT_FREE,
		DIR_SAEGENUT_ACTIVE,
		
		DIR_SAEGENUT_STOPPED,
		DIR_SAEGENUT_FINISHED,
        DIR_SAEGENUT_ARCHIVE,
		DIR_VERTEILER_FREE,
		DIR_VERTEILER_FREE_USB,
		DIR_VERTEILER1_ACTIVE,
		DIR_VERTEILER1_STOPPED,
		DIR_VERTEILER1_FINISHED,
		DIR_VERTEILER2_ACTIVE,
		DIR_VERTEILER2_STOPPED,
		DIR_VERTEILER2_FINISHED,
        DIR_VERTEILER_ARCHIVE,
		DIR_STRANG_FREE,
		DIR_STRANG1_ACTIVE,
		DIR_STRANG1_STOPPED,
		DIR_STRANG1_FINISHED,
		DIR_STRANG2_ACTIVE,
		DIR_STRANG2_STOPPED,
		DIR_STRANG2_FINISHED,
		DIR_STRANG3_ACTIVE,
		DIR_STRANG3_STOPPED,
		DIR_STRANG3_FINISHED,
		DIR_STRANG4_ACTIVE,
		DIR_STRANG4_STOPPED,
		DIR_STRANG4_FINISHED,
        DIR_STRANG_ARCHIVE,
		DIR_STATISTIK,
		DIR_WAXTOSAP,
		DIR_SAPTOWAX,
		DIR_SAPTOWAXWITTENBERGE,
		DIR_WAXAPAX,
		DIR_WAXAPAX_OLD,
		DIR_WAXOLD,
		PRINT_WORD,/// Ausdruck erfolgt ueber WINWORD
		DIR_DOC,
		APSAD_ROHRE,
		GRAFIKFORMAT,
		SAP_MAT_TO_WAX_EXE,
		SAP_MAT_TO_WAX_DAT,
		SAPSOAPSERVER,
	};	

	

	
};

