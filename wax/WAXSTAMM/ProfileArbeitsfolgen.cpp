#include "stdafx.h"
#include <mxodbc\mxodbc.h>


ProfileArbeitsfolgen::ProfileArbeitsfolgen() : StammDat("profile_arbeitsfolgen", "", "")
{
}

void ProfileArbeitsfolgen::load(const string& db)
{
	MXTable profilearbeitsfolgen(get_database(db), get_tbl());
	
	DECLARE_COL(String, typ, profilearbeitsfolgen);
	DECLARE_COL(Long,	code, profilearbeitsfolgen);
	
	profilearbeitsfolgen.load(get_select(), get_order());

	lst.RemoveAll();
	while (!profilearbeitsfolgen.isEof())
	{
		data* thedata = new ProfileArbeitsfolgen::data;
		thedata->typ = *typ.data;
		thedata->code = code.data;
				
		lst.AddTail(thedata);
		profilearbeitsfolgen.moveNext();
	}
}

const int ProfileArbeitsfolgen::data::get_code() const
{
	return code;
}

const CString& ProfileArbeitsfolgen::data::get_typ() const
{
	return typ;
}
