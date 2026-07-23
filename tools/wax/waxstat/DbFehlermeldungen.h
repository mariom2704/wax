#pragma once
#include "dbbase.h"
#include "EnError.h"

/**
Basisklasse für den Zugriff auf die Datenbank für Fehlermeldungen
*/
class DbFehlermeldungen :
	public DbBase
{
public:
	DbFehlermeldungen(void);
	virtual ~DbFehlermeldungen(void);

	///Werte aus Db lesen
	virtual bool readFromDb(EnError &error);
};
