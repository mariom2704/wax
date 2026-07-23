#include "stdafx.h"

#include "resource.h"

#include "mxparameter.h"
#include "regparameter.h"
#include "iniparameter.h"
#include "dbparameter.h"

namespace
{
std::string escape_sql(const std::string& value)
{
	std::string escaped;
	escaped.reserve(value.size());
	for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
	{
		escaped.push_back(*it);
		if (*it == '\'')
			escaped.push_back('\'');
	}
	return escaped;
}

std::string query_scalar(CDatabase* db, const CString& sql)
{
	if (!db || !db->IsOpen())
		return std::string();

	CRecordset rs(db);
	CString value;
	try
	{
		rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if (!rs.IsEOF())
			rs.GetFieldValue((short)0, value);
		rs.Close();
	}
	catch (CDBException* e)
	{
		e->Delete();
	}
	return std::string(CT2A(value));
}

void execute_sql(CDatabase* db, const CString& sql)
{
	if (!db || !db->IsOpen())
		return;
	try
	{
		db->ExecuteSQL(sql);
	}
	catch (CDBException* e)
	{
		e->Delete();
	}
}
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
    
#define KEY_BEARBEITER		"KEY_BEARBEITER"
#define KEY_PRINT_WORD      "PRINT_WORD"
#define KEY_APSAD_ROHRE		"APSAD_ROHRE"
#define KEY_GF_DN2ZOLL      "GF_DN_2_ZOLL"
#define KEY_ENABLE_PREISVERGLEICH "ENABLE_PREISVERGLEICH"
#define KEY_REL_ABST_AENDERN "REL_ABST_AENDERN"
#define KEY_GF_DWFHOEHE     "GF_DWFHOEHE"
#define KEY_SHOWFREIGABEPOS "SHOW_NO_FREIGABE_POS"
#define KEY_EMPFAENGER      "EMAIL_ADRESS"
#define KEY_DISPLAYNAME     "EMAIL_DISPLAYNAME"
#define KEY_BETREFF         "EMAIL_SUBJECT"
#define KEY_CHECKFREIGABE   "AUTO_FREIGABE"
#define KEY_MBTERMIN        "AV_INIT_MBTERMIN"
#define KEY_DIR_GROHR_FREE  "DIR_GROHR_FREE"
#define KEY_DIR_GROHR_ACTIVE "DIR_GROHR_ACTIVE"
#define KEY_DIR_GROHR_STOPPED "DIR_GROHR_STOPPED"
#define KEY_DIR_GROHR_FINISHED "DIR_GROHR_FINISHED"
#define KEY_DIR_GROHR_ARCHIVE "DIR_GROHR_ARCHIVE"
#define KEY_DIR_SAEGENUT_FREE "DIR_SAEGENUT_FREE"
#define KEY_DIR_SAEGENUT_ACTIVE "DIR_SAEGENUT_ACTIVE"
#define KEY_DIR_SAEGENUT_STOPPED "DIR_SAEGENUT_STOPPED"
#define KEY_DIR_SAEGENUT_FINISHED "DIR_SAEGENUT_FINISHED"
#define KEY_DIR_SAEGENUT_ARCHIVE "DIR_SAEGENUT_ARCHIVE"
#define KEY_DIR_VERTEILER_FREE "DIR_VERTEILER_FREE"
#define KEY_DIR_VERTEILER_FREE_USB "DIR_VERTEILER_FREE_USB"
#define KEY_DIR_VERTEILER1_ACTIVE "DIR_VERTEILER1_ACTIVE"
#define KEY_DIR_VERTEILER1_STOPPED "DIR_VERTEILER1_STOPPED"
#define KEY_DIR_VERTEILER1_FINISHED "DIR_VERTEILER1_FINISHED"
#define KEY_DIR_VERTEILER2_ACTIVE "DIR_VERTEILER2_ACTIVE"
#define KEY_DIR_VERTEILER2_STOPPED "DIR_VERTEILER2_STOPPED"
#define KEY_DIR_VERTEILER2_FINISHED "DIR_VERTEILER2_FINISHED"
#define KEY_DIR_VERTEILER_ARCHIVE "DIR_VERTEILER_ARCHIVE"
#define KEY_DIR_STRANG_FREE "DIR_STRANG_FREE"
#define KEY_DIR_STRANG1_ACTIVE "DIR_STRANG1_ACTIVE"
#define KEY_DIR_STRANG1_STOPPED "DIR_STRANG1_STOPPED"
#define KEY_DIR_STRANG1_FINISHED "DIR_STRANG1_FINISHED"
#define KEY_DIR_STRANG2_ACTIVE "DIR_STRANG2_ACTIVE"
#define KEY_DIR_STRANG2_STOPPED "DIR_STRANG2_STOPPED"
#define KEY_DIR_STRANG2_FINISHED "DIR_STRANG2_FINISHED"
#define KEY_DIR_STRANG3_ACTIVE "DIR_STRANG3_ACTIVE"
#define KEY_DIR_STRANG3_STOPPED "DIR_STRANG3_STOPPED"
#define KEY_DIR_STRANG3_FINISHED "DIR_STRANG3_FINISHED"
#define KEY_DIR_STRANG4_ACTIVE "DIR_STRANG4_ACTIVE"
#define KEY_DIR_STRANG4_STOPPED "DIR_STRANG4_STOPPED"
#define KEY_DIR_STRANG4_FINISHED "DIR_STRANG4_FINISHED"
#define KEY_DIR_STRANG_ARCHIVE "DIR_STRANG_ARCHIVE"
#define KEY_DIR_STATISTIK   "DIR_STATISTIK"
#define KEY_DIR_WAXTOSAP    "DIR_WAXTOSAP"
#define KEY_DIR_SAPTOWAX    "DIR_SAPTOWAX"
#define KEY_DIR_SAPTOWAXWITTENBERGE    "DIR_SAPTOWAXWITTENBERGE"
#define KEY_DIR_WAXAPAX     "DIR_WAXAPAX"
#define KEY_DIR_WAXAPAX_OLD "DIR_WAXAPAX_OLD"
#define KEY_DIR_WAXOLD      "DIR_WAXOLD"
#define KEY_BESCHRIFTUNGLAENGE "GF_BESCHRIFTUNG_ROHRLAENGE"
#define KEY_FONTGROESSE     "GF_FONTGROESSE"
//#define KEY_VIEWERVORSCHAU  "DWF_VIEWER_WAXGL"
#define KEY_GRAFIKFORMAT  "GRAFIKFORMAT"
#define KEY_VIEWERDRUCK     "DWF_VIEWER"
#define KEY_PROJEKTE        "DIR_WAXPRJ"
#define KEY_KOMP            "DIR_WAXKOMP"
#define KEY_CHECKSAP        "SAP"
#define KEY_STAMMDB         "STAMMDB"
#define KEY_SAPSOAPSERVER   "SAPSOAPSERVER"
#define KEY_SERVERDB        "SERVERDB"
#define KEY_SAMMELAUFTRAG_MAXPOS "SAMMELAUFTRAG_MAXPOS"
#define KEY_AV              "RESER4T"
#define KEY_DEBUG_NL        "DEBUG_NL"
#define KEY_LIEFERFRIST     "LIEFERFRIST"
#define KEY_KENNUNG         "IB_NL_KENNUNG"
#define KEY_EXCEL           "DIR_EXCEL_AUFTRAGSTABELLE"
#define KEY_DN2ZOLL         "GROHR_DN_2_ZOLL"
#define KEY_WINKELAUSRICHTUNG "GROHR_WINKEL_AUSRICHTUNG"
#define KEY_ZMASSLISTE      "GROHR_LISTE_ZMASSABZUG"
#define KEY_ZMASSGRAPHIK    "GROHR_GRAPHIK_ZMASSABZUG"
#define KEY_TEXTFORMAT      "KURZTEXT_FORM"
#define KEY_TRENNZEICHEN    "GROHR_NENNWEITE_TRENNZEICHEN"
#define KEY_BEMASSUNGSLINIE "GF_BEMASSUNGSLINIE"
#define KEY_TRENNLINIE      "GF_GRAFIK_TRENNLINIE"
#define KEY_RAHMEN          "GF_DRUCK_MIT_RAHMEN"
#define KEY_MB_TERMIN		"MB_TERMIN"
#define KEY_IB_NL_KENNUNG   "IB_NL_KENNUNG"



/// Gesch�tzter Defaultkonstruktor
MxParameter::MxParameter()
{
	ASSERT(false);
}

///Konstruktor
MxParameter::MxParameter(const string& userdb)
{
	dummy_para = new RegParameter(0, "", "", Parameter::UNDEFINED,false);
	db_name = userdb;
	user = get_username();
	db = new CDatabase;
}

///Destruktor, nicht �ffentlich
MxParameter::~MxParameter()
{
	clear_list();
	if (db->IsOpen())
		db->Close();
	delete db;
	delete dummy_para;
}

///Loescht die Objekte und die Pointer aus der Liste
void MxParameter::clear_list()
{
	ParameterMap::iterator it;
	for (it = pmap.begin(); it != pmap.end(); it++)
		delete it->second;
	pmap.clear();
}


///Fuellen der Liste mit Parametern aus der Registry und der Userdatenbank
void MxParameter::load()
{
#define INSERT_REGPARA(_key,_ordner,_entry,_default) \
	{ RegParameter *regp = new RegParameter(_key, _ordner, _entry, _default); \
	pmap.insert(make_pair(_key, regp)); \
	CString tmp; tmp.LoadString(IDC_PARA_##_key); \
	regp->set_bemerkung((LPCSTR)tmp); }\

#define INSERT_PARA(_key, _b, _c, _d) { \
	CString tmp; tmp.LoadString(IDC_PARA_##_key); \
	insert_para(_key, _b, _c, _d)->set_bemerkung((LPCSTR)tmp); }\

#define INSERT_PARAX(_key, _keyx, _b, _c, _d) { \
	CString tmp; tmp.LoadString(IDC_PARA_##_key); \
	insert_para(_keyx, _b, _c, _d)->set_bemerkung((LPCSTR)tmp); }\

	if (!db->IsOpen())
	{
		db->OpenEx(db_name.c_str(), CDatabase::openReadWrite | CDatabase::noOdbcDialog);
	}

	string sql = "UPDATE Systemparameter SET Alle = 0 WHERE Name like 'IB_NL_KENNUNG'";
	execute_sql(db, sql.c_str());
	
	sql = "UPDATE Systemparameter SET Alle = 1 WHERE Name like 'SAPSOAPSERVER'";
	execute_sql(db, sql.c_str());


	clear_list();
	
	CString path;

	TCHAR pathpure[_MAX_PATH];
	GetModuleFileName(0, pathpure, _MAX_PATH);
	CString strPath = pathpure;
	int iWhere = strPath.ReverseFind('\\');
	path = strPath.Mid(0, iWhere);

	
	string def_path = (LPCSTR)path;
	INSERT_REGPARA(LIEFERFRIST, "Project", "Lieferfrist", 14)

    INSERT_REGPARA(LANGUAGE, "Common", "Language", LANGUAGE_DEUTSCH)
	INSERT_REGPARA(AUTOSAVE, "Common", "Autosave", 10)

#ifdef _DEBUG
	INSERT_REGPARA(BAUMFARBE, "Ansicht", "Treecolor", RGB(255, 255, 255)) 
#else
	INSERT_REGPARA(BAUMFARBE, "Ansicht", "Treecolor", RGB(68, 120, 175))
#endif
	INSERT_REGPARA(GRAPHIKFENSTER, "Ansicht", "ShowOnlineGrafic", 1)
	INSERT_REGPARA(SHOWFREIGABEPOS, "Ansicht", "ShowFreigabepos", 0)
	INSERT_REGPARA(ICONGROESSE, "Ansicht", "Icongroesse", 1)
	INSERT_REGPARA(SYMBOLLEISTE, "Ansicht", "Symbolleiste", 1)
	INSERT_REGPARA(STATUSZEILE, "Ansicht", "Toolbar", 1)

	INSERT_REGPARA(FONTGROESSE, "Graphic", "Fontgroesse", 10)
	INSERT_REGPARA(BEMASSUNGSLINIE, "Graphic", "Bemassungslinie", 0)
	INSERT_REGPARA(TRENNLINIE, "Graphic", "Trennlinie", 1)
	INSERT_REGPARA(RAHMEN, "Graphic", "Rahmen", 0)
	INSERT_REGPARA(BESCHRIFTUNGLAENGE, "Graphic", "Beschriftunglaenge", 0)

	INSERT_REGPARA(DN2ZOLL, "GEWINDEROHR", "DN2Zoll", 1)
	INSERT_REGPARA(WINKELAUSRICHTUNG, "GEWINDEROHR", "WinkelAusrichtung", 0) 
	INSERT_REGPARA(TEXTFORMAT, "GEWINDEROHR", "Kurztext", 1)
	INSERT_REGPARA(TRENNZEICHEN, "GEWINDEROHR", "Trennzeichen", "x")
	INSERT_REGPARA(ZMASSLISTE, "GEWINDEROHR", "ZMassListe", 1)
	INSERT_REGPARA(ZMASSGRAPHIK, "GEWINDEROHR", "ZMassGraphik", 1) //, true 

	INSERT_REGPARA(AUTOFREIGABE, "FREIGABE", "AutoFreigabe", 0)
	INSERT_REGPARA(SAFEFREIGABE, "FREIGABE", "SafeFreigabe", 1)
	INSERT_REGPARA(CHECKSTATUS, "FREIGABE", "CheckStatus", 0)
#ifdef __WAX_FUER_DRITTE__
	INSERT_REGPARA(CHECKSAP, "FREIGABE", "CheckSap", 0)
#else
	INSERT_REGPARA(CHECKSAP, "FREIGABE", "CheckSap", 1)
#endif

	// Datenbanken
	if (!get_userlst().empty())
		INSERT_PARA(BEARBEITER, KEY_BEARBEITER, get_userlst().begin()->c_str(), false);
#ifdef _DEBUG
	INSERT_PARA(SAPSOAPSERVER, KEY_SAPSOAPSERVER, "http://MMXVI200.minimaxsap.int:8000/sap/bc/srt/rfc/sap/zplm_mcc_cad_create_matpl/015/service/binding", false); // Test SOAP Server
#else						
	INSERT_PARA(SAPSOAPSERVER, KEY_SAPSOAPSERVER, "http://MMXVP980.minimaxsap.int:8101/sap/bc/srt/rfc/sap/zplm_mcc_cad_create_matpl/015/service/binding", false); // Produktivserver SOAP Server
#endif
	INSERT_PARA(STAMMDB, KEY_STAMMDB, def_path + "\\waxstamm.dat", false);
	INSERT_PARA(SERVERDB, KEY_SERVERDB, "odwax2->waxserver", false);
	INSERT_PARA(DIR_DOC, "DIR_DOC", "doc", false);
	INSERT_PARA(EXCEL, KEY_EXCEL, def_path + "\\excel\\", false);
	INSERT_PARA(PROJEKTE, KEY_PROJEKTE, def_path + "\\projekte\\", false);
	INSERT_PARA(KOMP, KEY_KOMP, def_path + "\\komp\\", false);

	// diverse
#ifdef __WAX_FUER_DRITTE__
	INSERT_PARA(IB_NL_KENNUNG, KEY_IB_NL_KENNUNG, "284123456", false);
#else
	INSERT_PARA(IB_NL_KENNUNG, KEY_IB_NL_KENNUNG, "0", false);
#endif
	INSERT_PARA(AV, KEY_AV, "0", true);
	INSERT_PARA(DEBUGNL, KEY_DEBUG_NL, "0", true);
	INSERT_PARA(MAXPOSPERSA, KEY_SAMMELAUFTRAG_MAXPOS, "30", false);
	//INSERT_PARA(VIEWERVORSCHAU, KEY_VIEWERVORSCHAU, "adview", false);
	INSERT_PARA(SAP_MAT_TO_WAX_EXE, "SAP_MAT_TO_WAX_EXE", "SapMatToWax.exe", true);
	INSERT_PARA(SAP_MAT_TO_WAX_DAT, "SAP_MAT_TO_WAX_DAT", "", true);

	INSERT_PARA(GRAFIKFORMAT, KEY_GRAFIKFORMAT, "SVG", false);
	if (WaxSys::para_str(GRAFIKFORMAT) == "DWF")
		WaxSys::save_string("SVG", GRAFIKFORMAT);  //Wenn DWF eingestellt ist, automatisch umschalten
		
	INSERT_PARA(VIEWERDRUCK, KEY_VIEWERDRUCK, "SVG", false);
	if (WaxSys::para_str(VIEWERDRUCK) != "SVG") //Wenn DWF eingestellt ist, automatisch umschalten
		WaxSys::save_string("SVG", VIEWERDRUCK);
	

	INSERT_PARA(GF_DN2ZOLL, KEY_GF_DN2ZOLL, "0", false);
	INSERT_PARA(GF_DWFHOEHE, KEY_GF_DWFHOEHE, "4", false);
	INSERT_PARA(REL_ABST_AENDERN, KEY_REL_ABST_AENDERN, "1", false);
	INSERT_PARA(PRINT_WORD, KEY_PRINT_WORD, "0", true);
	INSERT_PARA(APSAD_ROHRE, KEY_APSAD_ROHRE, "0", true);

	// Email Parameter
	#ifdef __WAX_FUER_DRITTE__
		INSERT_PARA(EMPFAENGER, KEY_EMPFAENGER, "wax3@minimax.de", false);
	#else
		INSERT_PARA(EMPFAENGER, KEY_EMPFAENGER, "wax@minimax.de", false);
	#endif
	
	CString tmp; tmp.LoadString(IDS_FERT_IN_BO);
    INSERT_PARA(DISPLAYNAME,KEY_DISPLAYNAME, (LPCSTR)tmp, false);  
	tmp.LoadString(IDS_AUFTRAG_ZUR_FREIGABE);
	INSERT_PARA(BETREFF, KEY_BETREFF, (LPCSTR)tmp, false);
		
	// AV Pfadparameter
	INSERT_PARA(DIR_GROHR_FREE, KEY_DIR_GROHR_FREE, "", true);
	INSERT_PARA(DIR_GROHR_ACTIVE, KEY_DIR_GROHR_ACTIVE, "", true);
	INSERT_PARA(DIR_GROHR_STOPPED,	KEY_DIR_GROHR_STOPPED, "", true);
	INSERT_PARA(DIR_GROHR_FINISHED, KEY_DIR_GROHR_FINISHED, "", true);
	INSERT_PARA(DIR_GROHR_ARCHIVE, KEY_DIR_GROHR_ARCHIVE, "", true);
	INSERT_PARA(DIR_SAEGENUT_FREE, KEY_DIR_SAEGENUT_FREE, "", true);
	INSERT_PARA(DIR_SAEGENUT_ACTIVE, KEY_DIR_SAEGENUT_ACTIVE, "", true);
	INSERT_PARA(DIR_SAEGENUT_STOPPED, KEY_DIR_SAEGENUT_STOPPED, "", true);
	INSERT_PARA(DIR_SAEGENUT_FINISHED,	KEY_DIR_SAEGENUT_FINISHED,"", true);
	INSERT_PARA(DIR_SAEGENUT_ARCHIVE, KEY_DIR_SAEGENUT_ARCHIVE, "", true);
	INSERT_PARA(DIR_VERTEILER_FREE, KEY_DIR_VERTEILER_FREE, "", true);
	INSERT_PARA(DIR_VERTEILER_FREE_USB, KEY_DIR_VERTEILER_FREE_USB, "", true);
	INSERT_PARA(DIR_VERTEILER1_ACTIVE,	KEY_DIR_VERTEILER1_ACTIVE, "", true);
	INSERT_PARA(DIR_VERTEILER1_STOPPED, KEY_DIR_VERTEILER1_STOPPED, "", true);
	INSERT_PARA(DIR_VERTEILER1_FINISHED, KEY_DIR_VERTEILER1_FINISHED, "", true);
	INSERT_PARA(DIR_VERTEILER2_ACTIVE, KEY_DIR_VERTEILER2_ACTIVE, "", true);
	INSERT_PARA(DIR_VERTEILER2_STOPPED, KEY_DIR_VERTEILER2_STOPPED, "", true);
	INSERT_PARA(DIR_VERTEILER2_FINISHED, KEY_DIR_VERTEILER2_FINISHED, "", true);
	INSERT_PARA(DIR_VERTEILER_ARCHIVE, KEY_DIR_VERTEILER_ARCHIVE, "", true);
	INSERT_PARA(DIR_STRANG_FREE, KEY_DIR_STRANG_FREE, "", true);
	INSERT_PARA(DIR_STRANG1_ACTIVE, KEY_DIR_STRANG1_ACTIVE, "", true);
	INSERT_PARA(DIR_STRANG1_STOPPED, KEY_DIR_STRANG1_STOPPED, "", true);
	INSERT_PARA(DIR_STRANG1_FINISHED, KEY_DIR_STRANG1_FINISHED, "", true);
	INSERT_PARA(DIR_STRANG2_ACTIVE, KEY_DIR_STRANG2_ACTIVE, "", true);
	INSERT_PARA(DIR_STRANG2_STOPPED, KEY_DIR_STRANG2_STOPPED, "", true);
	INSERT_PARA(DIR_STRANG2_FINISHED, KEY_DIR_STRANG2_FINISHED, "", true);
	INSERT_PARA(DIR_STRANG3_ACTIVE, KEY_DIR_STRANG3_ACTIVE, "", true);
	INSERT_PARA(DIR_STRANG3_STOPPED, KEY_DIR_STRANG3_STOPPED, "", true);
	INSERT_PARA(DIR_STRANG3_FINISHED, KEY_DIR_STRANG3_FINISHED, "", true);
	INSERT_PARA(DIR_STRANG4_ACTIVE, KEY_DIR_STRANG4_ACTIVE, "", true);
	INSERT_PARA(DIR_STRANG4_STOPPED, KEY_DIR_STRANG4_STOPPED, "", true);
	INSERT_PARA(DIR_STRANG4_FINISHED, KEY_DIR_STRANG4_FINISHED, "", true);
	INSERT_PARA(DIR_STRANG_ARCHIVE, KEY_DIR_STRANG_ARCHIVE, "", true);
	INSERT_PARA(DIR_STATISTIK, KEY_DIR_STATISTIK, "", true);
	INSERT_PARA(DIR_WAXTOSAP, KEY_DIR_WAXTOSAP, "", true);
	INSERT_PARA(DIR_SAPTOWAX, KEY_DIR_SAPTOWAX, "", true);
	INSERT_PARA(DIR_SAPTOWAXWITTENBERGE, KEY_DIR_SAPTOWAXWITTENBERGE, "", true);
	INSERT_PARA(DIR_WAXAPAX, KEY_DIR_WAXAPAX, "", true);
	INSERT_PARA(DIR_WAXAPAX_OLD, KEY_DIR_WAXAPAX_OLD, "", true);
	INSERT_PARA(DIR_WAXOLD, KEY_DIR_WAXOLD, "", true);
	INSERT_PARA(MBTERMIN, KEY_MB_TERMIN, "30", true) 

	
}

Parameter* MxParameter::insert_para(int key, const string& entry, const string& def_str, bool av) 
{
	/**
	L�dt einen Parameter aus der Inidatei, falls dieser dort nicht definiert ist,
	wird der Parameter aus der Userdatenbank geladen.
	*/
	IniParameter *ip = new IniParameter("wax", key, entry, def_str);
	ip->set_av(av);

	if (ip->is_loaded())
	{
		pmap.insert(make_pair(key, ip));
		return ip;
	}
	else
	{
		delete ip;
		DbParameter *dbpara = new DbParameter(db, user, key, entry, def_str);
		dbpara->set_av(av);
		TRY
		{
			dbpara->load();
			pmap.insert(make_pair(key, dbpara));
			return dbpara;
		}
		CATCH_ALL(e)
		{
			delete dbpara;
		}
		END_CATCH_ALL
	}
	return dummy_para; 	
}

Parameter* MxParameter::get_para(int key)
{
	ParameterMap::iterator it = pmap.find(key);
	if (it != pmap.end() && it->second)
		return it->second;
	ASSERT(FALSE);
	return dummy_para;
}

const Parameter* MxParameter::get_para_const(int key) const
{
	ParameterMap::const_iterator it = pmap.find(key);
	if (it != pmap.end() && it->second)
		return it->second;
	ASSERT(FALSE);
	return dummy_para;
}

/// Default-Wert fuer Parameter, liefert String
CString MxParameter::def_val_str(int key) const
{
	return get_para_const(key)->get_def_str().c_str();
}

/// Default-Wert fuer Parameter, liefert Integer
int MxParameter::def_val_int(int key) const
{
	return get_para_const(key)->get_def_int();
}

/// Einstiegsmethode zum Finden eines Parameters, liefert String
CString MxParameter::val_str(int key) const
{
	return get_para_const(key)->get_val_str().c_str();
}

/// Einstiegsmethode zum Finden eines Parameters, liefert Integer
int MxParameter::val_int(int key) const
{
	return get_para_const(key)->get_val_int();
}

const string MxParameter::get_tempdir(const string& appname) const
{
	/**
	Liefert das tempor�re Verzeichnis f�r die Applikation als <systemtempdir>\\appname.
	Falls das Verzeichnis nicht existiert, wird es erzeugt
	*/
	char path[_MAX_PATH];
	DWORD res = ::GetTempPath(_MAX_PATH, path); 
	if (!res || res > _MAX_PATH) 
		strcpy(path, "C:\\");
	string apppath(path + appname);
	CFileStatus rStatus;
	if (!CFile::GetStatus(apppath.c_str(), rStatus))
		if (!CreateDirectory(apppath.c_str(), NULL))
			return path;
	return apppath + "\\";
}

const string MxParameter::get_username() const
{
	string user;
	char t_user[255];
	DWORD length = 254;
	memset(t_user, 0, 254);
	if (::GetUserName(t_user, &length))
	{
		// damit gabs mal �rger unter NT. Testen!!
		t_user[length] = 0;
		user = t_user;
	}
	else
		user = "user";
	return user;
}

const vector<string> MxParameter::get_userlst()
{
	/**
	L�dt alle Eintr�ge aus der Tabelle Benutzer (ausser dem Defaultuser 'user')
	Der aktuell angemeldete Benutzer steht immer an der ersten Stelle.
	*/
#undef BEARBEITER // Hier ist nicht der Systemparameter gemeint, sondern das Tabellenfeld
	string cur_user = get_username();
	vector<string> userlst;
	if (!get_db() || !get_db()->IsOpen())
		return userlst;

	CRecordset rs(get_db());
	try
	{
		rs.Open(CRecordset::forwardOnly, "SELECT CODE, BEARBEITER FROM Benutzer", CRecordset::readOnly);
		while (!rs.IsEOF())
		{
			CString code;
			CString bearbeiter;
			rs.GetFieldValue((short)0, code);
			rs.GetFieldValue((short)1, bearbeiter);

			const std::string code_str = (LPCSTR)CT2A(code);
			const std::string bearbeiter_str = (LPCSTR)CT2A(bearbeiter);
			if (code_str != "user")
			{
				if (code_str != cur_user)
					userlst.push_back(bearbeiter_str);
				else
					userlst.insert(userlst.begin(), bearbeiter_str);
			}
			rs.MoveNext();
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		e->Delete();
	}
	return userlst;
}

void MxParameter::add_user(const string& login)
{
	/** 
	F�gt den Benutzereintrag in die Benutzertabelle ein und macht eine Kopie der Standardsystemparameter
	des Referenzusers 'user' 
	*/
	string newuser;
	newuser = "INSERT INTO Benutzer (CODE) VALUES ('" + login + "')";
	execute_sql(get_db(), newuser.c_str());

	CRecordset rs(get_db());
	const std::string escaped_login = escape_sql(login);
	try
	{
		rs.Open(CRecordset::forwardOnly, "SELECT Name, Wert, Alle FROM Systemparameter WHERE Benutzer = 'user'", CRecordset::readOnly);
		while (!rs.IsEOF())
		{
			CString name;
			CString wert;
			long alle = 0;
			rs.GetFieldValue((short)0, name);
			rs.GetFieldValue((short)1, wert);
			rs.GetFieldValue((short)2, alle);

			const std::string escaped_name = escape_sql((LPCSTR)CT2A(name));
			const std::string escaped_wert = escape_sql((LPCSTR)CT2A(wert));
			CString insert_sql;
			insert_sql.Format(
				"INSERT INTO Systemparameter (Name, Benutzer, Wert, Alle) VALUES ('%s', '%s', '%s', %d)",
				escaped_name.c_str(),
				escaped_login.c_str(),
				escaped_wert.c_str(),
				(int)alle);
			execute_sql(get_db(), insert_sql);
			rs.MoveNext();
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		e->Delete();
	}

}

bool MxParameter::is_new_user(const string& login)
{
	/**
	Pr�fung, ob der Login bereits in der Usertabelle eingetragen ist
	*/

/*
Testcode f�r Simulation von neuen Usern
nicht l�schen!
string xselect = "DELETE FROM Benutzer WHERE CODE = 'peter'";
execute_sql(get_db(), xselect.c_str());
xselect = "DELETE FROM Systemparameter WHERE Benutzer = 'peter'";
execute_sql(get_db(), xselect.c_str());
*/
	UNREFERENCED_PARAMETER(login);
	string select = "SELECT COUNT(*) FROM Benutzer WHERE CODE = '" + escape_sql(get_username()) + "'";
	string count = query_scalar(get_db(), select.c_str());
	if (atoi(count.c_str()) == 1)
		return false;
	else if (atoi(count.c_str()) == 0)
		return true;

	ASSERT(false);
	return true;
}

string MxParameter::get_passwd() const
{
	/**
	Liefert das Passwort aus der Tabelle Password, Eintrag SYSTEM 
	*/
	return query_scalar(db, "SELECT Password FROM Password WHERE Name = 'SYSTEM'");
}

string MxParameter::getBemerkung(int key) const
{
	return get_para_const(key)->getBemerkung();
}



std::string MxParameter::getEmail()
{
	string select = "SELECT EMAIL FROM Benutzer WHERE CODE = '" + get_username() + "'";
	return query_scalar(db, select.c_str());
}


std::string MxParameter::getEmailFromDefaultUser()
{
	string select = "SELECT Wert FROM Systemparameter WHERE Name = 'EMAIL_ADRESS'  AND Benutzer = 'user'";
	return query_scalar(db, select.c_str());
}

std::string MxParameter::getNLFromDefaultUser()
{
	string select = "SELECT Wert FROM Systemparameter WHERE Name = 'IB_NL_KENNUNG'  AND Benutzer = 'user'";
	return query_scalar(db, select.c_str());
}