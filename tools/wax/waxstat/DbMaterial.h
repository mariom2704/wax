#pragma once
#include "DbBaseListen.h"
#include "EnMaterial.h"

/**
Basisklasse für den Datenbankzugriff um Materiallisten kumuliert über Wax-Positionen zu erhalten
*/
class DbMaterial :
	public DbBaseListen
{
public:
	DbMaterial(void);
	virtual ~DbMaterial(void);

	///Werte aus Db lesen
	virtual bool readFromDb(QList<EnMaterial> &liste);

	///Werte in Db (Über)Schreiben
	virtual bool writeToDb(const QList<EnMaterial> &liste);

	///Werte in Db Updaten
	virtual bool updateInDb(const QList<EnMaterial> &liste);

};
