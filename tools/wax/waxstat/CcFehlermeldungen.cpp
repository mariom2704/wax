#include ".\ccfehlermeldungen.h"
#include "DbCcFehlermeldungen.h"

CcFehlermeldungen::CcFehlermeldungen(void)
{
}

CcFehlermeldungen::~CcFehlermeldungen(void)
{
}

/**
Ließt zu der FehlerId den Fehlertext aus der Db
*/
bool CcFehlermeldungen::readFromDb(EnError &error)
{
	DbCcFehlermeldungen ccDbFehlermeldungen;

	DbFehlermeldungen *pDbFehlermeldungen = NULL;


	//Tabellen Objekt zum Lesen der Fehlermeldungen holen
	pDbFehlermeldungen = ccDbFehlermeldungen.getDbFehlermeldungen(getDatenbankParameter());
	if (!pDbFehlermeldungen)
		return false;


	//Arbeitsfolgen aus der DB lesen
	if (!pDbFehlermeldungen->readFromDb(error))
	{
		appendError(pDbFehlermeldungen->getErrorList());
		appendErrorDescription("Der Text zu der FehlerID konnte nicht aus der Datenbank gelesen werden!");
		return false;
	}


	return true;
}
