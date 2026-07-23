#pragma once
#include <QtSql>
#include "MxBase.h"
#include "EnDatenbankParameter.h"


/**
Basisklasse für die Control Classes für das Modul Datenbanken.
Die Control Class Datenbankschicht sind die Schnittstellen zur Anwendungsschicht
*/
class DbCcBase : public MxBase
{
public:
	DbCcBase(void);
	virtual ~DbCcBase(void);




private:

};
