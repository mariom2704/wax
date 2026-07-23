#include "stdafx.h"
#include "resource.h"
#include "property.h"
#include "mxparameter.h"
#include "parameter.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



static MxParameter *mxparam = NULL;
vector<string> WaxSys::tmp_file_lst;

//Konstruktor, nicht �ffentlich
WaxSys::WaxSys()
{
}

//Destruktor, nicht �ffentlich
WaxSys::~WaxSys()
{
}

MXPARAMDLL void WaxSys::unload()
{
	delete mxparam;
	mxparam  = NULL;
	remove_tmpfiles();
}

//Laden der Systemeinstellungen
MXPARAMDLL void WaxSys::load(const char* dbname)
{
	delete mxparam;
	mxparam = new MxParameter(dbname);
	mxparam->load();
}


//Benutzereinstellungsdialog anzeigen
MXPARAMDLL void WaxSys::edit_user(bool detect_new_user)
{
	/**
	�ffnet die Benutzerverwaltung zur Eingabe der pers�nlichen Daten. Ist detect_new_user
	= true, wird die Benutzerverwaltung nur angezeigt, wenn der User neu ist.
	*/
	TRY
	{
		if (detect_new_user)
		{
			if (mxparam->is_new_user(get_username()))
			{
				mxparam->add_user(get_username());
				mxparam->load();
			}
			else 
				return;
		}
		Sleep(5000);
		edit_property(NULL, false, Property::USER);
	}
	CATCH(CDBException, e)
	{
		AfxMessageBox(IDS_DBERROR, MB_ICONERROR | MB_OK);
		PostQuitMessage(-1);
	}
	END_CATCH
}



//Abfrage des Tempverzeichnisses
MXPARAMDLL const CString WaxSys::GetTempDir() 
{
	if (mxparam)
		return mxparam->get_tempdir("wax").c_str();
	return "c:\\";
}

//Abfrage eines tempor�ren Dateinamens
MXPARAMDLL string WaxSys::GetTempFilename(const string& ext)
{
	char tmpfile[_MAX_PATH];
	::GetTempFileName(GetTempDir(), "", 0, tmpfile);
	::DeleteFile(tmpfile);

	string filename = tmpfile;
	string::iterator it;
	for (it = filename.begin(); it != filename.end(); it++)
		*it = (char)tolower(*it);
	filename.replace(filename.find("tmp"), 3, ext);
	HANDLE htmp = ::CreateFile(filename.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	::CloseHandle(htmp);
	add_tmpfile(filename.c_str());	
	return filename;
}

MXPARAMDLL void WaxSys::add_tmpfile(const char* fname)
{
	char shortpath[MAX_PATH];
	::GetShortPathName(fname, (LPTSTR)shortpath, MAX_PATH);
  	tmp_file_lst.push_back(fname);
}

/// Loeschen der temporaeren Dateien
MXPARAMDLL void WaxSys::remove_tmpfiles()
{
	for (vector<string>::iterator it = tmp_file_lst.begin(); it != tmp_file_lst.end();)
	{
		TRACE("Tempor�re Datei %s wird gel�scht\n", it->c_str());
		if (::DeleteFile(it->c_str()))
		{
			it = tmp_file_lst.erase(it);
			continue;
		}
		else
		{
			TRACE("%s konnte nicht gel�scht werden: GetLastError=%i\n", it->c_str(), GetLastError());
			it++;
		}
	}
#ifdef _DEBUG
	if (tmp_file_lst.size())
		TRACE("Noch %i Dateien in der tmp_file_list\n", tmp_file_lst.size());
#endif
}


//Abfrage, ob Programm die AV Version ist
MXPARAMDLL bool WaxSys::isAV()
{
	if (mxparam)
	{
		if (mxparam->val_int(AV) >= WaxSys::Av )
			return true;
	}
	return false;
}


//Abfrage, ob User die AV-Superuser Rechte hat (Leiter AV)
MXPARAMDLL bool WaxSys::isAvAdmin()
{
	if (mxparam)
	{
		if (mxparam->val_int(AV) >= WaxSys::AvAdmin)
			return true;
	}
	return false;
}

//Abfrage, ob User die Administrator Rechte hat
MXPARAMDLL bool WaxSys::isAdministrator()
{
	if (mxparam)
	{
		if (mxparam->val_int(AV) == WaxSys::Administrator)
			return true;
	}
	return false;
}


MXPARAMDLL std::string WaxSys::getLanguage()
{
	std::string str;

	if (mxparam)
		str = mxparam->val_str(LANGUAGE).GetString();

	if (str.empty())
		str = getStandardLanguage();

	//Wegen Vereinheitlichung der Tabellennamen hier der Fix
	if (str == "English")
		str = LANGUAGE_ENGLISCH;  //Neue Bezeichnung ist "Englisch"
	
	return str;
}

MXPARAMDLL std::string WaxSys::getStandardLanguage()
{
	return LANGUAGE_DEUTSCH;
}


MXPARAMDLL string WaxSys::getPwd()
{
	if (!mxparam)
		return "";

	return mxparam->get_passwd();
}

//Serverdatenbank testen und Datenbankname zur�ckliefern
MXPARAMDLL CString WaxSys::getServerdb()
{
	DBConnection con;

	CString path(mxparam->val_str(SERVERDB));
			
	con.set_serverdbentry(path);
	return con.get_serverdb();
}


//Stammdatenbank testen und Datenbankname zur�ckliefern
MXPARAMDLL CString WaxSys::getStammdb()
{
	DBConnection con;
	CString stammdb = verifyLocationOfStammDB();
	con.set_stammdbentry(stammdb);
	return con.get_stammdb();
}


MXPARAMDLL CString WaxSys::verifyLocationOfStammDB()
{
	CString stammdb;
#ifdef _DEBUG
	stammdb = mxparam->val_str(STAMMDB);
#else
	TCHAR pathpure[_MAX_PATH];
	GetModuleFileName(0, pathpure, _MAX_PATH);
	CString strPath = pathpure;
	int iWhere = strPath.ReverseFind('\\');
	CString path = strPath.Mid(0, iWhere);
	stammdb = path + _T("\\waxstamm.mdb");
	CFileFind finder;
	
	if(finder.FindFile(stammdb, 0) == FALSE)
	{
		stammdb = path + _T("\\waxstamm.dat");
		if(finder.FindFile(stammdb, 0) == FALSE)
		{
			 stammdb = mxparam->val_str(STAMMDB);
		}
	}
#endif
	return stammdb;
}



//Stammdatenbank testen und Datenbankname zur�ckliefern
MXPARAMDLL CString WaxSys::getSAPSoapServer()
{	
	CString SAPSOAPServer(mxparam->val_str(SAPSOAPSERVER));
	return SAPSOAPServer;
}


//Stammdatenbank testen und Datenbankname zur�ckliefern
MXPARAMDLL CString WaxSys::getStammdb(void * connection)
{
	DBConnection *con = (DBConnection *)connection;

	CString stammdb = verifyLocationOfStammDB();

	con->set_stammdbentry(stammdb);
	return con->get_stammdb(true);
}


//Liefert den Connectstring f�r die Userdatenbank
MXPARAMDLL string WaxSys::get_userdb()
{
	if (mxparam)
		return (LPCSTR)mxparam->get_db()->GetConnect();	
	return "";
}

MXPARAMDLL string WaxSys::get_username()
{
	if (mxparam)
		return mxparam->get_username();
	return "unknown user";
}

MXPARAMDLL bool WaxSys::hotkey_pressed(UINT key1, UINT key2)
{
	/**
	Pr�ft max. zwei Tasten gleichzeitig. Wird nur key1 ben�tigt, muss f�r key2 Null �bergeben werden (default)
	*/
	const int pressed_flag = 0x8000;
	
	key1 = key1 ? GetAsyncKeyState(key1) & pressed_flag : pressed_flag;
	key2 = key2 ? GetAsyncKeyState(key2) & pressed_flag : pressed_flag;

	return key1 == pressed_flag && key2 == pressed_flag;
}

MXPARAMDLL void WaxSys::edit_property(CWnd* pWndParent, bool check, int defaultpage)
{
	Property prop(pWndParent, mxparam);
	if (check)
		if (!prop.check_password())
			return;
	prop.SetActivePage(defaultpage);
	prop.DoModal();
}

MXPARAMDLL int WaxSys::para_int(int parameter)
{
	return mxparam->val_int(parameter);
}


MXPARAMDLL void WaxSys::save_string(const char* ext, int parameter)
{
	mxparam->save_para(parameter, ext);
}


MXPARAMDLL CString WaxSys::para_str(int parameter)
{
	return mxparam->val_str(parameter);
}

MXPARAMDLL string WaxSys::para_cstr(int parameter)
{
	return (LPCSTR)mxparam->val_str(parameter);
}

MXPARAMDLL void WaxSys::user_list(CComboBox *bx)
{
	vector<string> userlst = mxparam->get_userlst();
	vector<string>::const_iterator it;

	for (it = userlst.begin(); it != userlst.end(); it++)
		bx->AddString(it->c_str());

	// Als erstes sollte der angemeldete User in der Liste stehen
	bx->SelectString(-1, userlst.begin()->c_str());
}

MXPARAMDLL void WaxSys::switch_av()
{
	Parameter* parm = mxparam->get_para(AV);
	parm->set_val(parm->get_val_int() ? 0 : 1);	
}


MXPARAMDLL std::string  WaxSys::getEmail()
{
	return mxparam->getEmail();
}

MXPARAMDLL std::string  WaxSys::getEmailfromDefaultUser()
{
	return mxparam->getEmailFromDefaultUser();
}



MXPARAMDLL std::string  WaxSys::getNLFromDefaultUser()
{
	return mxparam->getNLFromDefaultUser();
}