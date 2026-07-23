#include "stdafx.h"

#include <mxodbc\mxodbc.h>



DNzuAussendurchmesser::DNzuAussendurchmesser() : StammDat("dn_zu_aussendurchmesser", "", "dn")
{
}

void DNzuAussendurchmesser::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, dn, tbl);
	DECLARE_COL(Double, aussendurchmesser, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new DNzuAussendurchmesser::data;
		thedata->dn = dn.data;
		thedata->aussendurchmesser = aussendurchmesser.data;

		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

int DNzuAussendurchmesser::data::get_dn() const
{
	return dn;
}

double DNzuAussendurchmesser::data::get_aussendurchmesser() const
{
	return aussendurchmesser;
}

DNzuAussendurchmesser::data::data()
{
}



int DNzuAussendurchmesser::getAussendurchmesserToInt(int dn)
{

	double zahl = getAussendurchmesser(dn);

	//Runden
	return (int)(zahl<0 ? zahl-0.5 : zahl+0.5); //klein, fein aber dirty
}


int DNzuAussendurchmesser::getRadiusToInt(int dn)
{

	double zahl = getAussendurchmesser(dn);
	zahl /= 2.0;

	//Runden
	return (int)(zahl<0 ? zahl-0.5 : zahl+0.5); //klein, fein aber dirty
}

double DNzuAussendurchmesser::getAussendurchmesser(int dn)
{
	DNzuAussendurchmesser::data *g = NULL;
	for (POSITION pos = getStammdaten().get_dnzuaussendurchmesser().GetHeadPosition(); pos;)
	{
		g = (DNzuAussendurchmesser::data *)getStammdaten().get_dnzuaussendurchmesser().GetNext(pos);
		if (g->get_dn() == dn) 
			return g->get_aussendurchmesser();
	}
	return 0.0;
}

