#include "stdafx.h"


#include <mxodbc\mxodbc.h>

AussendurchmesserWandstaerke::AussendurchmesserWandstaerke() : StammDat("aussendurchmesser_wandstaerke", "", "elemid")
{

}

void AussendurchmesserWandstaerke::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, elemid, tbl);
	DECLARE_COL(Int, dn, tbl);
	DECLARE_COL(Double, innendurchmesser, tbl);
	DECLARE_COL(Double, wandstaerke, tbl);
	//DECLARE_COL(Double, aussendurchmesser, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new AussendurchmesserWandstaerke::data;
		thedata->elemid = elemid.data;
		thedata->dn = dn.data;
		thedata->innendurchmesser	= innendurchmesser.data;
		thedata->wandstaerke		= wandstaerke.data;

		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

AussendurchmesserWandstaerke::data::data()
{

}

int AussendurchmesserWandstaerke::data::get_elemid() const
{
	return elemid;
}

int AussendurchmesserWandstaerke::data::get_dn() const
{
	return dn;
}

double AussendurchmesserWandstaerke::data::get_aussendurchmesser() const
{
	return aussendurchmesser;
}

double AussendurchmesserWandstaerke::data::get_wandstaerke() const
{
	return wandstaerke;
}

const double AussendurchmesserWandstaerke::data::get_innendurchmesser() const
{
	return innendurchmesser;
}

