#include "stdafx.h"

#include <mxodbc\mxodbc.h>



PulvernZeiten::PulvernZeiten() : StammDat("pulvern_zeiten", "", "")
{

}

void PulvernZeiten::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int,	abglaenge_von,	tbl);
	DECLARE_COL(Int,	abglaenge_bis,	tbl);
	DECLARE_COL(Int,	laenge_von,		tbl);
	DECLARE_COL(Int,	laenge_bis,		tbl);
	DECLARE_COL(Int,	dn,				tbl);
	DECLARE_COL(Double, zeit,			tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new PulvernZeiten::data;
		thedata->abglaenge_von	= abglaenge_von.data;
		thedata->abglaenge_bis	= abglaenge_bis.data;
		thedata->laenge_von		= laenge_von.data;
		thedata->laenge_bis		= laenge_bis.data;
		thedata->dn				= dn.data;
		thedata->zeit			= zeit.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

PulvernZeiten::data::data() : zeit(0.0)
{

}

const int PulvernZeiten::data::get_abglaenge_bis() const
{
	return abglaenge_bis;
}

const int PulvernZeiten::data::get_abglaenge_von() const
{
	return abglaenge_von;
}

const int PulvernZeiten::data::get_dn() const
{
	return dn;
}

const int PulvernZeiten::data::get_laenge_bis() const
{
	return laenge_bis;
}

const int PulvernZeiten::data::get_laenge_von() const
{
	return laenge_von;
}

const double PulvernZeiten::data::get_zeit() const
{
	return zeit;
}

