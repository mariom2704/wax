#include "stdafx.h"
#include <mxodbc\mxodbc.h>



ProfileSaegezeiten::ProfileSaegezeiten() : StammDat("profile_saegezeiten", "", "")
{
}

void ProfileSaegezeiten::load(const string& db)
{
	MXTable profilesaegezeiten(get_database(db), get_tbl());
	
	DECLARE_COL(Long,	code,		profilesaegezeiten);
	DECLARE_COL(String, typ,		profilesaegezeiten);
	DECLARE_COL(Long,	ende,		profilesaegezeiten);
	DECLARE_COL(Long,	laenge_von,	profilesaegezeiten);
	DECLARE_COL(Long,	laenge_bis,	profilesaegezeiten);
	DECLARE_COL(Double,	ruestzeit,	profilesaegezeiten);
	DECLARE_COL(Double,	zeit,		profilesaegezeiten);
	
	profilesaegezeiten.load(get_select(), get_order());

	lst.RemoveAll();
	while (!profilesaegezeiten.isEof())
	{
		data* thedata = new ProfileSaegezeiten::data;
		thedata->code = code.data;
		thedata->typ = *typ.data;
		thedata->ende = ende.data;
		thedata->laengeVon = laenge_von.data;
		thedata->laengeBis = laenge_bis.data;
		thedata->ruestzeit = ruestzeit.data;
		thedata->zeit = zeit.data;
				
		lst.AddTail(thedata);
		profilesaegezeiten.moveNext();
	}
}

const int ProfileSaegezeiten::data::get_code() const
{
	return code;
}

const int ProfileSaegezeiten::data::get_ende() const
{
	return ende;
}

const int ProfileSaegezeiten::data::get_laengeBis() const
{
	return laengeBis;
}

const int ProfileSaegezeiten::data::get_laengeVon() const
{
	return laengeVon;
}

const double ProfileSaegezeiten::data::get_zeit() const
{
	return zeit;
}

const double ProfileSaegezeiten::data::get_ruestzeit() const
{
	return ruestzeit;
}

const CString& ProfileSaegezeiten::data::get_typ() const
{
	return typ;
}

