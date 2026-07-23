#include ".\dbfehlermeldungensql.h"
#include "QSqlQuery.h"


DbFehlermeldungenSql::DbFehlermeldungenSql(void)
{
}

DbFehlermeldungenSql::~DbFehlermeldungenSql(void)
{
}

/**
Zuweisen der Datenbankparameter:
DBTyp, Datenbankname
*/
void DbFehlermeldungenSql::setDatenbankParameter(const EnDatenbankParameter &datenbankParameter)
{
	m_datenbankParameter = datenbankParameter;

	//Datenbankparameter zuweisen
 	m_sqlDb.setDatenbankParameter(m_datenbankParameter);

}


bool DbFehlermeldungenSql::readFromDb(EnError &error)
{
	QSqlQuery query;

	QString sql = "SELECT `Bezeichnung` from `Fehler` where `FehlerId`='%1'";


	//ID in Sql Statement einfügen
	sql = sql.arg(error.getErrorId());

	//Sql Statement ausführen
	if (!m_sqlDb.executeSql(sql,query))
	{
		appendError(m_sqlDb.getErrorList());
		m_sqlDb.closeDb();
		return false;
	}

	//Datensatz lesen
	if (query.first()) 
	{
		error.setErrorDescription(query.value(0).toString());
    }
	else
	{
		appendErrorDescription("Zu der FehlerID wurde kein Eintrag in der Datenbank gefunden!");
		m_sqlDb.closeDb();
		return false;
	}


	m_sqlDb.closeDb();

	return true;
}
