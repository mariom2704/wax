#pragma once
#include "DbBaseListen.h"
#include "EnArbeitsfolge.h"

/**
Basisklasse für den Zugriff auf die Datenbank für Arbeitsfolgen
*/
class DbArbeitsfolgen :
	public DbBaseListen
{
public:
	DbArbeitsfolgen(void);
	virtual ~DbArbeitsfolgen(void);

	///Werte aus Db lesen
	virtual bool readFromDb(QList<EnArbeitsfolge> &liste);

	///Werte in Db (Über)Schreiben
	virtual bool writeToDb(const QList<EnArbeitsfolge> &liste);

	///Werte in Db Updaten
	virtual bool updateInDb(const QList<EnArbeitsfolge> &liste);

	
	///Werte als Matrix ablegen
	virtual bool writeToDbAsMatrix(const QList<EnArbeitsfolge> &liste);


};
