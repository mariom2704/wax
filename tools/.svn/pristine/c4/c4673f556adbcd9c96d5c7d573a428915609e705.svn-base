#pragma once
#include "dbbase.h"
#include "EnWaxDate.h"


/**
Basisklasse für Entity Klassen der Persistenzschicht um auf die Datenbank bzw. Dateien zuzugreifen
Speziel für Listen, die über einen Zeitraum definiert werden sollen
*/
class DbBaseListen :
	public DbBase
{
public:
	DbBaseListen(void);
	virtual ~DbBaseListen(void);


	///Beginn der Auswertung
	void setStartDatum(QDate date);
	
	///Enddatum der Auswertung
	void setEndDatum(QDate date);

	const EnWaxDate getStartDatum()const; 
	const EnWaxDate getEndDatum()const;


private:
	EnWaxDate	m_dateStart,
				m_dateEnde;
};
