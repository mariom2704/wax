#pragma once
#include <QtCore>


/**
Umrechnung des Wax-Datum in QDate Format und Umgekehrt
*/
class EnWaxDate
{
public:
	EnWaxDate(void);
	virtual ~EnWaxDate(void);

	///Datum in Wax-Format setzen
	void setWaxDate(long date);

	///Datum in QDate Format setzen
	void setDate(const QDate &date);


	///Datum in Wax Format zurückgeben
	long getWaxDate();

	///Datum in Qdate Format zurückgeben
	QDate getDate();


private:
	long m_waxDate;
};
