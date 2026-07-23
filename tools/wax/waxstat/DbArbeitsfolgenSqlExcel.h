#pragma once
#include "dbarbeitsfolgensql.h"

/**
Zugriff über SQL auf eine Exceldatei für Arbeitsfolgen
*/
class DbArbeitsfolgenSqlExcel :
	public DbArbeitsfolgenSql
{
public:
	DbArbeitsfolgenSqlExcel(void);
	virtual ~DbArbeitsfolgenSqlExcel(void);


	///Werte in Db (Über)Schreiben
	virtual bool writeToDb(const QList<EnArbeitsfolge> &liste);

	///Werte als Matrix ablegen
	virtual bool writeToDbAsMatrix(const QList<EnArbeitsfolge> &liste);

};
