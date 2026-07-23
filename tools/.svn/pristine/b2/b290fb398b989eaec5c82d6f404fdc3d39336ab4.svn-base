#pragma once
#include "dbfehlermeldungen.h"
#include "DbSqlBase.h"

/**
Zugriff auf die Datenbank über SQL für Fehlermeldungen
*/
class DbFehlermeldungenSql :
	public DbFehlermeldungen
{
public:
	DbFehlermeldungenSql(void);
	virtual ~DbFehlermeldungenSql(void);

	///Datenbank Parameter setzen
	virtual void setDatenbankParameter(const EnDatenbankParameter &datenbankParameter);

	virtual bool readFromDb(EnError &error);

protected:
	DbSqlBase m_sqlDb;


};
