#include ".\ccmaterial.h"
#include "DbCcWaxDaten.h"

CcMaterial::CcMaterial(void)
{
}

CcMaterial::~CcMaterial(void)
{
}


/**
Über das Datenbankobjekt wird das kumulierte Material der Wax Positionen gelesen
*/
bool CcMaterial::readFromDb(QList<EnMaterial> &listMaterial)
{
	DbCcWaxDaten ccDbMaterial;

	DbMaterial *pDbMaterial = NULL;


	//Tabellen Objekt zum Lesen der Material holen
	pDbMaterial = ccDbMaterial.getDbMaterial(getDatenbankParameter());
	if (!pDbMaterial)
		return false;


	//Auswertungsbereich setzen
	pDbMaterial->setStartDatum(getStartDatum());
	pDbMaterial->setEndDatum(getEndDatum());


	//Material aus der DB lesen
	if (!pDbMaterial->readFromDb(listMaterial))
	{
		//Fehlerbehandlung 


		return false;
	}


	return true;
}


bool CcMaterial::writeAuswertung(const QList<EnMaterial> &listMaterial)
{
	DbCcWaxDaten ccDbMaterial;

	DbMaterial *pDbMaterial = NULL;


	//Tabellen Objekt für Material holen
	pDbMaterial = ccDbMaterial.getDbMaterial(getDatenbankParameter());
	if (!pDbMaterial)
		return false;


	//Material in die Db schreiben
	if (!pDbMaterial->writeToDb(listMaterial))
	{
		//Fehlerbehandlung 


		return false;
	}


	return true;
}
