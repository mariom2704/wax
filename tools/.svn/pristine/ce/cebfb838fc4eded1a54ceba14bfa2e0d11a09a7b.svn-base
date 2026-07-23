#pragma once
#include "dbccbase.h"
#include "DbArbeitsfolgen.h"
#include "EnDatenbankParameter.h"
#include "DbMaterial.h"

/**
Control Class für die Persistenzschicht für Wax Daten. Dient als Schnittstelle zur Anwendungsschicht.

Liefert die Datenbankobjekte für den Zugriff auf die Wax Daten
*/
class DbCcWaxDaten :
	public DbCcBase
{
public:
	DbCcWaxDaten(void);
	virtual ~DbCcWaxDaten(void);

	///Gibt in Abhängigkeit des DB Typs ein Datenbankobjekt zum Lesen und Schreiben für Arbeitsfolgen auf die Db zurück
	DbArbeitsfolgen *getDbArbeitsfolgen(EnDatenbankParameter dbParam);

	///Gibt in Abhängigkeit des DB Typs ein Datenbankobjekt zum Lesen und Schreiben für Material auf die Db zurück
	DbMaterial *getDbMaterial(const EnDatenbankParameter &dbParam);

private:
	///Zeiger auf das Datenbankobjekt für das Material
	DbMaterial *m_pDbMaterial;

	///Zeiger auf das Datenbankobjekt für die Arbeitsfolgen
	DbArbeitsfolgen *m_pDbArbeitsfolgen;

};
