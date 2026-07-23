#pragma once

#include <mxodbc/datecol.h>

class DateColMfc : public DateCol
{
public:
	DateColMfc(const CString& field) : DateCol(field)
	{
	}

	//Laden der Daten
	virtual void load(void* pFX)
	{
		RFX_Date((CFieldExchange *)pFX, *fieldname, dt);
	}

	COleDateTime getdate() const
	{
		return dt;
	}

	void setdate(COleDateTime& date)
	{
		dt = date;
	}

private:
	COleDateTime dt;
};