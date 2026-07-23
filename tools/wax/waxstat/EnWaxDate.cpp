#include ".\enwaxdate.h"

EnWaxDate::EnWaxDate(void)
{
	m_waxDate = 0;
}

EnWaxDate::~EnWaxDate(void)
{
}

///Datum in Wax-Format setzen
void EnWaxDate::setWaxDate(long date)
{
	m_waxDate = date;
}

long EnWaxDate::getWaxDate()
{
	return m_waxDate;
}

/**
Datum in QDate Format setzen.
Wird intern in das Wax-Datum umgerechnet
*/
void EnWaxDate::setDate(const QDate &date)
{
	long baseQDate,
		 diffDays;


	QDate oleDate;

	//BasisDatum COleDateTime
	oleDate.setYMD(1899,12,30);

	//Tage zum Basisdatum von COledateTime
	diffDays = oleDate.daysTo(date);

	//umrechnen in Wax-Datum
	m_waxDate = diffDays * 10000;

}

/**
Das Wax-Datumsformat in QDate zurückgeben
*/
QDate EnWaxDate::getDate()
{
	QDate date;

	//Wax Datum ins COleDateTime Basisdatum umrechnen
	long waxDate = m_waxDate / 10000;

	//auf diesem Datum basiert das COleDateTime Objekt. 
	date.setYMD(1899,12,30);

	//Tage anhängen
	date = date.addDays(waxDate);


	return date;
}