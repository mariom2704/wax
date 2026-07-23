#include "stdafx.h"
#include <mxodbc\mxodbc.h>


zulassung::zulassung() : StammDat("zulassung", "", "")
{
}

void zulassung::load(const string& db)
{
	MXTable zulassung(get_database(db), get_tbl());
	
	DECLARE_COL(String, typ, zulassung);
	DECLARE_COL(Long,	code, zulassung);
	
	zulassung.load(get_select(), get_order());

	lst.RemoveAll();
	while (!zulassung.isEof())
	{
		data* thedata = new zulassung::data;
		thedata->typ = *typ.data;
		thedata->code = code.data;
				
		lst.AddTail(thedata);
		zulassung.moveNext();
	}
}

const int zulassung::data::get_code() const
{
	return code;
}

const CString& zulassung::data::get_typ() const
{
	return typ;
}
