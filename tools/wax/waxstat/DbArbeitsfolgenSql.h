#pragma once
#include "DbArbeitsfolgen.h"
#include "EnDatenbankParameter.h"
#include "DbSqlBase.h"

/**
Zugriff über SQL auf die Datenbank für Arbeitsfolgen
*/
class DbArbeitsfolgenSql :
	public DbArbeitsfolgen
{
public:
	DbArbeitsfolgenSql(void);
	virtual ~DbArbeitsfolgenSql(void);

	const QString getSelectSqlStatement()const;

	///Datenbank Parameter setzen
	virtual void setDatenbankParameter(const EnDatenbankParameter &datenbankParameter);

	///Werte aus Db lesen
	virtual bool readFromDb(QList<EnArbeitsfolge> &liste);

	///Werte in Db (Über)Schreiben
	virtual bool writeToDb(const QList<EnArbeitsfolge> &liste);

	///Werte als Matrix ablegen
	virtual bool writeToDbAsMatrix(const QList<EnArbeitsfolge> &liste);

	///Werte in Db Updaten
	virtual bool updateInDb(const QList<EnArbeitsfolge> &liste);

protected:
	DbSqlBase m_sqlDb;

};
