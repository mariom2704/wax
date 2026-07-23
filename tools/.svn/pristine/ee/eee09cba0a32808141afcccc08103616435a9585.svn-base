#pragma once
#include "ccbase.h"
#include <QtCore>

/**
Basis Control Class für Auswertungen von Listen
*/
class CcBaseListen :
	public CcBase
{
public:
	CcBaseListen(void);
	virtual ~CcBaseListen(void);

	///Beginn der Auswertung setzen
	void setStartDatum(QDate date);
	
	///Enddatum der Auswertung setzen
	void setEndDatum(QDate date);

	///Beginn der Auswertung holen
	QDate getStartDatum();

	//Ende der Auswertung holen
	QDate getEndDatum();

private:
	QDate m_startDate,
		  m_endDate;

};
