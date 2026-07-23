#include ".\befehlmaterialkumuliert.h"
#include "CcMaterial.h"

BefehlMaterialKumuliert::BefehlMaterialKumuliert(void)
{
}

BefehlMaterialKumuliert::~BefehlMaterialKumuliert(void)
{
}




/**
Startet den Befehl "Material der Wax-Positionen kumulieren"

Das Material der Wax Positionen wird aus der MySql Datenbank gelesen 
und in eine Exceldatei geschrieben.

Es kann zur Auswertung ein Zeitraum berücksichtigt werden. (Über setStartDatum(..) und setEndDatum(..))
*/
bool BefehlMaterialKumuliert::start()
{
	CcMaterial materialMySql,
			   materialExcel;

	EnDatenbankParameter dbParameterMysql,
						 dbParameterExcel;

	QList<EnMaterial> m_listMaterial;




	//Parameter für MySql Db setzen
	if (!dbParameterMysql.initStandardMySqlParameter())
	{
		appendError(dbParameterMysql.getErrorList());
		appendErrorId("BefehlArbeitsfolgenProDn1"); //Die Arbeitsfolgen konnten nicht aus der Datenbank gelesen werden
		return false;
	}


	//Datenbankparameter übergeben
	materialMySql.setDatenbankParameter(dbParameterMysql);

	//Auswertungsbereich übergeben
	materialMySql.setStartDatum(getStartDatum());
	materialMySql.setEndDatum(getEndDatum());

	//Material aus der Datenbank lesen
	if (!materialMySql.readFromDb(m_listMaterial))
	{
		//Fehlerauswertung

		return false;
	}


	//Datenbankparameter für Exceldatei setzen
	dbParameterExcel.setDatabaseName(getExceldatei());
	dbParameterExcel.setDbtyp(EnDatenbankParameter::EXCEL);

	materialExcel.setDatenbankParameter(dbParameterExcel);


	//Material in eine Exceldatei schreiben
	if (!materialExcel.writeAuswertung(m_listMaterial))
	{
		//Fehlerauswertung

		return false;
	}

	return true;
}


