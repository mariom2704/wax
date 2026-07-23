#include "stdafx.h"
#include "resource.h"


#include "PropPageUser.h"
#include <mxodbc\mxodbc.h>

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


	MXTable user((DWORD)parameter->get_db(), "Benutzer");
	DECLARE_COL(String, CODE, user);

	user.load();

	while (!user.isEof())
	{
		if (*CODE.data != "user")
			userlst.AddString(*CODE.data);
		user.moveNext();
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
	// Löschen des Benutzers aus der Benutzertabelle
	sql.Format("DELETE FROM Benutzer WHERE CODE = '%s'", u);
	parameter->get_db()->ExecSQL(sql);

	// Löschen aller Parameter aus der Systemparametertabelle
	sql.Format("DELETE FROM Systemparameter WHERE Benutzer = '%s'", u);
	parameter->get_db()->ExecSQL(sql);

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
	sql.Format("SELECT BEARBEITER FROM Benutzer WHERE CODE = '%s'", u);
	benutzername = parameter->get_db()->ExecSQL(sql);
	sql.Format("SELECT DURCHWAHL FROM Benutzer WHERE CODE = '%s'", u);
	durchwahl = parameter->get_db()->ExecSQL(sql);
	sql.Format("SELECT EMAIL FROM Benutzer WHERE CODE = '%s'", u);
	email = parameter->get_db()->ExecSQL(sql);
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
	where = "WHERE CODE = '" + (string)(LPCSTR)u + "'";
	sql = "UPDATE Benutzer SET BEARBEITER = '" + (string)(LPCSTR)benutzername + "' " + where;
	parameter->get_db()->ExecSQL(sql.c_str());
	sql = "UPDATE Benutzer SET DURCHWAHL = '" + (string)(LPCSTR)durchwahl + "' " + where;
	parameter->get_db()->ExecSQL(sql.c_str());
	sql = "UPDATE Benutzer SET EMAIL = '" + (string)(LPCSTR)email + "' " + where;
	parameter->get_db()->ExecSQL(sql.c_str());
	return true;
}
