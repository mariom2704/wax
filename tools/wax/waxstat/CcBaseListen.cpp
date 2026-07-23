#include ".\ccbaselisten.h"

CcBaseListen::CcBaseListen(void)
{
}

CcBaseListen::~CcBaseListen(void)
{
}


///Beginn der Auswertung
void CcBaseListen::setStartDatum(QDate date)
{
	m_startDate = date;
}
	
///Enddatum der Auswertung
void CcBaseListen::setEndDatum(QDate date)
{
	m_endDate = date;
}

///Beginn der Auswertung holen
QDate CcBaseListen::getStartDatum()
{
	return m_startDate;
}

//Ende der Auswertung holen
QDate CcBaseListen::getEndDatum()
{
	return m_endDate;
}