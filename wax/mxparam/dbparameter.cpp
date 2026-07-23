#include "stdafx.h"
#include "dbparameter.h"
#include <mxodbc\mxodbc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// Konstruktor
DbParameter::DbParameter(MXDatabase *userdb, const string& cur_user, int parakey, const string& regentry, const string& def_str) : Parameter(parakey)
{
	/**
	Definiert einen Parameter in der Userdatenbank. 
	*/
	entry = regentry;
	def_val_str = def_str;
	db = userdb;
	user = cur_user;
	nmspace = INVALID;
}


DbParameter::~DbParameter()
{
}

int DbParameter::valid_namespaces() const
{
	/**
	Gültigkeit ist USER oder LOCATION
	*/
	return USER | LOCATION;
}

void DbParameter::load()
{
	/**
	Lädt den Wert aus den Systemparametern. 
	
	todo: Hier könnte noch abgeprüft werden, 
	ob der Parameter, wenn es ihn nicht gibt, von User USER geladen werden kann.
	Dann bräuchte man neue Parameter nur für den User USER in den Systemparametern 
	eintragen und hätte automatisch alle User versorgt.
	*/
	if (!db->IsOpen())
		return;
	MXTable tab((DWORD)db, "Systemparameter");
	DECLARE_COL(String, Name, tab);
	DECLARE_COL(String, Benutzer, tab);
	DECLARE_COL(String, Wert, tab);
	DECLARE_COL(Bool, Alle, tab);
	string select = "Name = '" + entry + "' AND Benutzer = '" + user + "'";
	tab.load(select.c_str(), 0, true);
	if (tab.get_count())
	{
        val_str = (LPCSTR)*Wert.data;
	}
	nmspace = Alle.data == 1 ? LOCATION : USER;
}


void DbParameter::save()
{
	/**
	Speichern des Parameters in den Systemparametern
	to do: while Schleife beim Speichern entfernen
	*/
	MXTable tab((DWORD)db, "Systemparameter");
	DECLARE_COL(String, Name, tab);
	DECLARE_COL(String, Benutzer, tab);
	DECLARE_COL(String, Wert, tab);
	DECLARE_COL(Bool, Alle, tab);
	string select;
	if (get_valid_namespace() == LOCATION)
		select = "Name = '" + entry + "'";
	else 
		select = "Name = '" + entry + "' AND Benutzer = '" + user + "'";
	tab.load(select.c_str(), 0, false);

	if (tab.isEof()) // Parameter ist nicht in der Datenbank
	{
		tab.addNew();
		*Name.data = get_entry().c_str();
		*Benutzer.data = user.c_str();
		*Wert.data = val_str.c_str();
		Alle.data = nmspace == LOCATION ? 1 : 0;
		tab.update();
	
	}
	else
	{
		while (!tab.isEof()) // Parameter sollte nur einmal vorkommen, Schleife ist obsolet
		{
			tab.edit();
			*Wert.data = val_str.c_str();
			Alle.data = nmspace == LOCATION ? 1 : 0;
			tab.update();
			tab.moveNext();
		}
	}
}


bool DbParameter::is_readonly() const
{
	return false;
}

