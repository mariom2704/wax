#include ".\befehlbasezeitraum.h"

BefehlBaseZeitraum::BefehlBaseZeitraum(void)
{
}

BefehlBaseZeitraum::~BefehlBaseZeitraum(void)
{
}


///Beginn der Auswertung
void BefehlBaseZeitraum::setStartDatum(QDate date)
{
	m_startDate = date;
}
	
///Enddatum der Auswertung
void BefehlBaseZeitraum::setEndDatum(QDate date)
{
	m_endDate = date;
}

///Beginn der Auswertung holen
QDate BefehlBaseZeitraum::getStartDatum()
{
	return m_startDate;
}

//Ende der Auswertung holen
QDate BefehlBaseZeitraum::getEndDatum()
{
	return m_endDate;
}



void BefehlBaseZeitraum::setExceldatei(const QString &datei)
{
	m_datei = datei;
}

QString BefehlBaseZeitraum::getExceldatei()
{
	return m_datei;
}
