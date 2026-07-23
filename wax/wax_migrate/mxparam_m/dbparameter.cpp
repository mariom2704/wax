#include "stdafx.h"
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
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/// Konstruktor
DbParameter::DbParameter(CDatabase *userdb, const string& cur_user, int parakey, const string& regentry, const string& def_str) : Parameter(parakey)
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
	G�ltigkeit ist USER oder LOCATION
	*/
	return USER | LOCATION;
}

void DbParameter::load()
{
	/**
	L�dt den Wert aus den Systemparametern. 
	
	todo: Hier k�nnte noch abgepr�ft werden, 
	ob der Parameter, wenn es ihn nicht gibt, von User USER geladen werden kann.
	Dann br�uchte man neue Parameter nur f�r den User USER in den Systemparametern 
	eintragen und h�tte automatisch alle User versorgt.
	*/
	if (!db->IsOpen())
		return;
	const std::string escaped_entry = escape_sql(entry);
	const std::string escaped_user = escape_sql(user);
	CString sql;
	sql.Format("SELECT Wert, Alle FROM Systemparameter WHERE Name = '%s' AND Benutzer = '%s'", escaped_entry.c_str(), escaped_user.c_str());

	CRecordset rs(db);
	try
	{
		rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
		if (!rs.IsEOF())
		{
			CString wert;
			long alle = 0;
			rs.GetFieldValue((short)0, wert);
			rs.GetFieldValue((short)1, alle);
			val_str = (LPCSTR)CT2A(wert);
			nmspace = alle == 1 ? LOCATION : USER;
		}
		rs.Close();
	}
	catch (CDBException* e)
	{
		e->Delete();
	}
}


void DbParameter::save()
{
	/**
	Speichern des Parameters in den Systemparametern
	to do: while Schleife beim Speichern entfernen
	*/
	if (!db || !db->IsOpen())
		return;

	const std::string escaped_entry = escape_sql(entry);
	const std::string escaped_user = escape_sql(user);
	const std::string escaped_value = escape_sql(val_str);
	const int alle = (nmspace == LOCATION) ? 1 : 0;

	CString count_sql;
	count_sql.Format("SELECT COUNT(*) FROM Systemparameter WHERE Name = '%s' AND Benutzer = '%s'", escaped_entry.c_str(), escaped_user.c_str());
	const std::string count_text = query_scalar(db, count_sql);
	const int count = atoi(count_text.c_str());

	CString sql;
	if (count == 0)
	{
		sql.Format(
			"INSERT INTO Systemparameter (Name, Benutzer, Wert, Alle) VALUES ('%s', '%s', '%s', %d)",
			escaped_entry.c_str(),
			escaped_user.c_str(),
			escaped_value.c_str(),
			alle);
	}
	else
	{
		sql.Format(
			"UPDATE Systemparameter SET Wert = '%s', Alle = %d WHERE Name = '%s' AND Benutzer = '%s'",
			escaped_value.c_str(),
			alle,
			escaped_entry.c_str(),
			escaped_user.c_str());
	}

	try
	{
		db->ExecuteSQL(sql);
	}
	catch (CDBException* e)
	{
		e->Delete();
	}
}


bool DbParameter::is_readonly() const
{
	return false;
}

