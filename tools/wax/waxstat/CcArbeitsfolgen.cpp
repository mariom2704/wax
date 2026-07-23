#include <QtCore>
#include ".\CcArbeitsfolgen.h"
#include "DbCcWaxDaten.h"

CcArbeitsfolgen::CcArbeitsfolgen(void)
{
}

CcArbeitsfolgen::~CcArbeitsfolgen(void)
{
}




bool CcArbeitsfolgen::readFromDb(QList<EnArbeitsfolge> &listArbeitsFolgen)
{
	DbCcWaxDaten ccDbArbeitsfolgen;

	DbArbeitsfolgen *pDbArbeitsfolgen = NULL;


	//Tabellen Objekt zum Lesen der Arbeitsfolgen holen
	pDbArbeitsfolgen = ccDbArbeitsfolgen.getDbArbeitsfolgen(getDatenbankParameter());
	if (!pDbArbeitsfolgen)
		return false;


	//Auswertungsbereich setzen
	pDbArbeitsfolgen->setStartDatum(getStartDatum());
	pDbArbeitsfolgen->setEndDatum(getEndDatum());


	//Arbeitsfolgen aus der DB lesen
	if (!pDbArbeitsfolgen->readFromDb(listArbeitsFolgen))
	{
		appendError(pDbArbeitsfolgen->getErrorList());
		return false;
	}


	return true;
}


bool CcArbeitsfolgen::writeAuswertung(const QList<EnArbeitsfolge> &listArbeitsFolgen)
{
	DbCcWaxDaten ccDbArbeitsfolgen;

	DbArbeitsfolgen *pDbArbeitsfolgen = NULL;


	//Tabellen Objekt für Arbeitsfolgen holen
	pDbArbeitsfolgen = ccDbArbeitsfolgen.getDbArbeitsfolgen(getDatenbankParameter());
	if (!pDbArbeitsfolgen)
		return false;


	//Arbeitsfolgen in die Db schreiben
	if (!pDbArbeitsfolgen->writeToDbAsMatrix(listArbeitsFolgen))
	{
		//Fehlerbehandlung 


		return false;
	}


	return true;
}
