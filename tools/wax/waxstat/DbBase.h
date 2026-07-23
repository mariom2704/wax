#pragma once
#include <QtSql>
#include "MxBase.h"
#include "EnDatenbankParameter.h"
#include "EnStringPair.h"


/**
Basisklasse für Entity Klassen der Persistenzschicht um auf die Datenbank bzw. Dateien zuzugreifen
*/
class DbBase : public MxBase
{
public:
	DbBase(void);
	virtual ~DbBase(void);

	//set Methoden
	virtual void setDatenbankParameter(const EnDatenbankParameter &datenbankParameter);
	
	

protected:
	EnDatenbankParameter m_datenbankParameter;

};
