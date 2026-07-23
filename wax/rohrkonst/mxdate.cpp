#include <stdafx.h>


		#include "mxdate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MXDate::MXDate() : COleDateTime()
{
}
	

MXDate::MXDate(const MXDate& other)
{
	copy(other);
}


MXDate::MXDate(const COleDateTime& other)
{
	// @todo: Alle Bezüge auf COleDateTime entfernen
	SetStatus(other.GetStatus());
	m_dt = other.m_dt;
}

const MXDate MXDate::get_current_date()
{
	COleDateTime cur = GetCurrentTime();
	MXDate dt;
	dt.SetStatus(cur.GetStatus());
	dt.m_dt = cur.m_dt;
	return dt;
}


CString MXDate::Format(DWORD dwFlags, LCID lcid) const
{
	if (is_valid())
		return COleDateTime::Format(dwFlags, lcid);
	return "";
}

CString MXDate::Format(LPCTSTR lpszFormat) const
{
	if (is_valid())
		return COleDateTime::Format(lpszFormat);
	return "";
}

CString MXDate::Format(UINT nFormatID) const
{
	if (is_valid())
		return COleDateTime::Format(nFormatID);
	return "";
}

bool MXDate::is_valid() const
{
	/**
	Prüft das Datum auf status == valid und m_dt > 0.
	Der Formatbefehlt führt bei m_dt = Null zum Absturz (seit VS2005)
	*/
	if (GetStatus() != valid || m_dt <= 1)
		return false;
	return true;
}

void MXDate::copy(const MXDate& other)
{
	SetStatus(other.GetStatus());
	SetDateTime(other.GetYear(), other.GetMonth(),other.GetDay(), other.GetHour(), other.GetMinute(), other.GetSecond());
}



bool MXDate::is_newer(const MXDate& other) const
{
	/**
	Liefert true, wenn dieses Datum neuer ist als other

	Datumsangaben werden vom Wax in der Datenbank als Int abgespeichert, mit den letzten 
	vier Nachkommastellen. Ein Vergleich kann falsche Ergebnisse liefern, wenn ein Datum 
	aus der Datenbank erzeugt wurde. 
	*/
	double this_date = (int)(m_dt*10000);
	double other_date = (int)(other.m_dt*10000);
	return this_date > other_date;
}


int MXDate::convDateToInt() const
{
	/**
	In COleDateTime ist m_dt ein Double, dessen Ganzzahlwert die Tage seit dem 1.1.1900 anzeigen.
	Der Nachkommaanteil gibt die Zeit von 0 bis 24 Uhr an. Dadurch ergibt sich eine Genauigkeit bis
	in den Millisekundenbereich. 

	Als Integer wird hier der Nachkommaanteil bis zur 5. Stelle mitgenommen. Dadurch sinkt die Genauigkeit auf
	24*3600/99999 als0 0,86 Sek. Das ist aber fast immer ausreichend.
	*/
	return (int)(m_dt*10000.0);	
}

int MXDate::toNornamlizedInt(bool bis) const
{
	/**
	Gibt den Integerwert für das Datum (*10000) zurück, um einen Bug im Mysql 
	bzw. ODBC Treiber zu umschiffen. Über bis kann der Uhrzeitanteil auf 
	23,59,59 gesetzt werden, damit alle WAs auch erkannt werden. bis gleich 
	false initialisiert die Uhrzeit auf 0,0,0.
	*/
	MXDate tmp(*this);
	if (bis)
		tmp.SetDateTime(GetYear(), GetMonth(), GetDay(),23,59,59);
	else
		tmp.SetDateTime(GetYear(), GetMonth(), GetDay(),0, 0, 0);
	return tmp.convDateToInt();
}

