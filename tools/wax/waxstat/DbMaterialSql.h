#pragma once
#include "dbmaterial.h"
#include "DbSqlBase.h"


/**
Zugriff auf die Datenbank über SQL für Material 
*/
class DbMaterialSql :
	public DbMaterial
{
public:
	DbMaterialSql(void);
	virtual ~DbMaterialSql(void);

	const QString getSelectSqlStatement()const;

	///Datenbank Parameter setzen
	virtual void setDatenbankParameter(const EnDatenbankParameter &datenbankParameter);

	///Werte aus Db lesen
	virtual bool readFromDb(QList<EnMaterial> &liste);

	///Werte in Db (Über)Schreiben
	virtual bool writeToDb(const QList<EnMaterial> &liste);

	///Werte in Db Updaten
	virtual bool updateInDb(const QList<EnMaterial> &liste);

protected:
	DbSqlBase m_sqlDb;

};
