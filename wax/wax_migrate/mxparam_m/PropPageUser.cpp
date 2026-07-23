#include "stdafx.h"
#include "resource.h"


#include "PropPageUser.h"

namespace
{
std::string escape_sql(const CString& value)
{
	const std::string input = (LPCSTR)CT2A(value);
	std::string escaped;
	escaped.reserve(input.size());
	for (std::string::const_iterator it = input.begin(); it != input.end(); ++it)
	{
		escaped.push_back(*it);
		if (*it == '\'')
			escaped.push_back('\'');
	}
	return escaped;
}

CString query_scalar(CDatabase* db, const CString& sql)
{
	CString value;
	if (!db || !db->IsOpen())
		return value;

	CRecordset rs(db);
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
	return value;
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


BEGIN_MESSAGE_MAP(PropPageUser, WaxPropPage)
	ON_BN_CLICKED(IDC_BENUTZERLOESCHEN,	on_loeschen)
	ON_CBN_SELCHANGE(IDC_PARA_LOGIN,	on_selchange)
	ON_CBN_EDITCHANGE(IDC_PARA_LOGIN,	on_update)
	ON_EN_UPDATE(IDC_PARA_DURCHWAHL,	on_update)
	ON_EN_UPDATE(IDC_PARA_EMAIL,		on_update)
	ON_EN_UPDATE(IDC_PARA_BENUTZERNAME, on_update)
END_MESSAGE_MAP()


/// Konstructor
PropPageUser::PropPageUser(MxParameter *para) : WaxPropPage(IDD_PARA_USER, para)
{
}

/// Destructor
PropPageUser::~PropPageUser()
{
}

/// Default-Werte setzen
void PropPageUser::set_default()
{
}


/// Dialog initialisieren
BOOL PropPageUser::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();


	setWindowsTextWithColon(IDC_STATIC_LOGIN, "IDC_STATIC_LOGIN");
	setWindowsTextWithColon(IDC_STATIC_NAME, "IDC_STATIC_NAME");
	setWindowsTextWithColon(IDC_STATIC_TEL, "IDC_STATIC_TEL");
	setWindowsTextWithColon(IDC_STATIC_EMAIL, "IDC_STATIC_EMAIL");
	setWindowsText(IDC_BENUTZERLOESCHEN, "IDC_BENUTZERLOESCHEN");


	CRecordset rs(parameter->get_db());
	try
	{
		rs.Open(CRecordset::forwardOnly, "SELECT CODE FROM Benutzer", CRecordset::readOnly);
		while (!rs.IsEOF())
		{
			CString code;
			rs.GetFieldValue((short)0, code);
			if (code.CompareNoCase("user") != 0)
				userlst.AddString(code);
			rs.MoveNext();
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		e->Delete();
	}
	login = WaxSys::get_username().c_str();
	userlst.SelectString(-1, login);
	on_selchange();

	GetDlgItem(IDC_PARA_LOGIN)->EnableWindow(passwd_ok ? true : false);
	GetDlgItem(IDC_BENUTZERLOESCHEN)->EnableWindow(passwd_ok ? true : false);

	UpdateData(FALSE);

	return TRUE;  
}

/// Datenaustausch
void PropPageUser::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARA_LOGIN, userlst);

	DDX_CBString(pDX, IDC_PARA_LOGIN, login);
	DDX_Text(pDX, IDC_PARA_BENUTZERNAME, benutzername);
	DDX_Text(pDX, IDC_PARA_DURCHWAHL, durchwahl);
	DDX_Text(pDX, IDC_PARA_EMAIL, email);
}

/// Benutzer loeschen
void PropPageUser::on_loeschen() 
{
	CString u;
	GetDlgItem(IDC_PARA_LOGIN)->GetWindowText(u);
	CString sql;
	// L�schen des Benutzers aus der Benutzertabelle
	sql.Format("DELETE FROM Benutzer WHERE CODE = '%s'", escape_sql(u).c_str());
	execute_sql(parameter->get_db(), sql);

	// L�schen aller Parameter aus der Systemparametertabelle
	sql.Format("DELETE FROM Systemparameter WHERE Benutzer = '%s'", escape_sql(u).c_str());
	execute_sql(parameter->get_db(), sql);

	userlst.DeleteString(userlst.FindString(-1, u));
	userlst.SetCurSel(0);
	on_selchange();
	UpdateData(FALSE);
}


/// Select Login - Datenaustausch
void PropPageUser::on_selchange() 
{
	CString u;
	GetDlgItem(IDC_PARA_LOGIN)->GetWindowText(u);
	CString sql;
	sql.Format("SELECT BEARBEITER FROM Benutzer WHERE CODE = '%s'", escape_sql(u).c_str());
	benutzername = query_scalar(parameter->get_db(), sql);
	sql.Format("SELECT DURCHWAHL FROM Benutzer WHERE CODE = '%s'", escape_sql(u).c_str());
	durchwahl = query_scalar(parameter->get_db(), sql);
	sql.Format("SELECT EMAIL FROM Benutzer WHERE CODE = '%s'", escape_sql(u).c_str());
	email = query_scalar(parameter->get_db(), sql);
	UpdateData(FALSE);
}


/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageUser::OnApply() 
{
	if (WaxPropPage::OnApply())
		return true;
	UpdateData();
	CString u;
	GetDlgItem(IDC_PARA_LOGIN)->GetWindowText(u);
	string sql;
	string where;
	where = "WHERE CODE = '" + escape_sql(u) + "'";
	sql = "UPDATE Benutzer SET BEARBEITER = '" + escape_sql(benutzername) + "' " + where;
	execute_sql(parameter->get_db(), sql.c_str());
	sql = "UPDATE Benutzer SET DURCHWAHL = '" + escape_sql(durchwahl) + "' " + where;
	execute_sql(parameter->get_db(), sql.c_str());
	sql = "UPDATE Benutzer SET EMAIL = '" + escape_sql(email) + "' " + where;
	execute_sql(parameter->get_db(), sql.c_str());
	return true;
}
