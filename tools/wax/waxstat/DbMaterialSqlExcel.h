#pragma once
#include "dbmaterialsql.h"
#include "EnMaterial.h"

/**
Zugriff über Sql auf ein Exceldokument für Material pro WAX Position
*/
class DbMaterialSqlExcel :
	public DbMaterialSql
{
public:
	DbMaterialSqlExcel(void);
	virtual ~DbMaterialSqlExcel(void);

	///Tabelle erstellen
	//virtual bool createTable();


	///Werte in Db (Über)Schreiben
	virtual bool writeToDb(const QList<EnMaterial> &liste);

};
