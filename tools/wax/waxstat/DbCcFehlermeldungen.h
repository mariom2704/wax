#pragma once
#include "ccbase.h"
#include "DbFehlermeldungen.h"


/**
Control Class für Fehlermeldungen. Verwaltet das Datenbankobjekt für den Datenbankzugriff
und dient als Schnittstelle zur Anwendungsschicht
*/
class DbCcFehlermeldungen :
	public CcBase
{
public:
	DbCcFehlermeldungen(void);
	virtual ~DbCcFehlermeldungen(void);


	///Gibt in Abhängigkeit des DB Typs ein Datenbankobjekt zum Lesen und Schreiben auf die Db zurück
	DbFehlermeldungen *getDbFehlermeldungen(EnDatenbankParameter dbParam);

private:
	///Zeiger auf das Datenbankobjekt für die Arbeitsfolgen
	DbFehlermeldungen *m_pDbFehlermeldungen;

};
