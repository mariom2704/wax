#include ".\befehlarbeitsfolgenprodn.h"
#include "CcArbeitsfolgen.h"
#include "EnDatenbankParameter.h"

BefehlArbeitsfolgenProDn::BefehlArbeitsfolgenProDn(void)
{
}

BefehlArbeitsfolgenProDn::~BefehlArbeitsfolgenProDn(void)
{
}






/**
Startet den Befehl "Anzahl Arbeitsfolgen pro DN-Wert"

Liest die Arbeitsfolgen der Wax Positionen aus der MySql Datenbank und schreibt sie in eine 
Exceldatei als Matrix "Anzahl Arbeitsfolgen - DnWerte"
*/
bool BefehlArbeitsfolgenProDn::start()
{
	CcArbeitsfolgen arbeitsfolgenMySql,
					arbeitsfolgenExcel;

	EnDatenbankParameter dbParameterMysql,
						 dbParameterExcel;

	QList<EnArbeitsfolge> m_listArbeitsFolgen;



	//Parameter für MySql Db setzen
	if (!dbParameterMysql.initStandardMySqlParameter())
	{
		appendError(dbParameterMysql.getErrorList());
		appendErrorId("BefehlArbeitsfolgenProDn1"); //Die Arbeitsfolgen konnten nicht aus der Datenbank gelesen werden
		return false;
	}


	//Datenbankparameter übergeben
	arbeitsfolgenMySql.setDatenbankParameter(dbParameterMysql);

	//Auswertungsbereich übergeben
	arbeitsfolgenMySql.setStartDatum(getStartDatum());
	arbeitsfolgenMySql.setEndDatum(getEndDatum());

	//Arbeitsfolgen aus der Datenbank lesen
	if (!arbeitsfolgenMySql.readFromDb(m_listArbeitsFolgen))
	{
		appendError(arbeitsfolgenMySql.getErrorList());
		appendErrorId("BefehlArbeitsfolgenProDn1"); //Die Arbeitsfolgen konnten nicht aus der Datenbank gelesen werden
		return false;
	}


	//Datenbankparameter für Exceldatei setzen
	dbParameterExcel.setDatabaseName(getExceldatei());
	dbParameterExcel.setDbtyp(EnDatenbankParameter::EXCEL);

	arbeitsfolgenExcel.setDatenbankParameter(dbParameterExcel);


	//Arbeitsfolgen in eine Exceldatei schreiben
	if (!arbeitsfolgenExcel.writeAuswertung(m_listArbeitsFolgen))
	{
		appendError(arbeitsfolgenExcel.getErrorList());
		appendErrorId("BefehlArbeitsfolgenProDn2"); //Die Arbeitsfolgen konnten nicht in die Exceldatei geschrieben werden
		return false;
	}

	return true;
}


