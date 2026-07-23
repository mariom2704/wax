#pragma once
#include "befehlbase.h"
#include <QtCore>

/**
Basiskalsse für Befehle bei denen ein Zeitraum definiert wird
*/

class BefehlBaseZeitraum :
	public BefehlBase
{
public:
	BefehlBaseZeitraum(void);
	virtual ~BefehlBaseZeitraum(void);

	///Beginn der Auswertung
	void setStartDatum(QDate date);
	
	///Enddatum der Auswertung
	void setEndDatum(QDate date);

	///Beginn der Auswertung holen
	QDate getStartDatum();

	//Ende der Auswertung holen
	QDate getEndDatum();


	void setExceldatei(const QString &datei);

	QString getExceldatei();

private:
	QDate m_startDate,
		  m_endDate;

	QString m_datei;
};
