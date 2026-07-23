#include ".\dbbaselisten.h"

DbBaseListen::DbBaseListen(void)
{
}

DbBaseListen::~DbBaseListen(void)
{
}


///Beginn der Auswertung
void DbBaseListen::setStartDatum(QDate date)
{
	m_dateStart.setDate(date);
}
	
///Enddatum der Auswertung
void DbBaseListen::setEndDatum(QDate date)
{
	m_dateEnde.setDate(date);
}

const EnWaxDate DbBaseListen::getStartDatum()const
{
	return m_dateStart;
}

const EnWaxDate DbBaseListen::getEndDatum()const
{
	return m_dateEnde;
}
