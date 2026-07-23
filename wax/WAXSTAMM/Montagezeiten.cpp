#include "stdafx.h"
#include <mxodbc\mxodbc.h>
#include "resource.h"

#include "Montagezeiten.h"



Montagezeiten::Montagezeiten() : StammDat("montagezeiten", "", "rohrtyp,hoehe,dn")
{
	
}

Montagezeiten::~Montagezeiten()
{
	
}

void Montagezeiten::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, dn, tbl);
	DECLARE_COL(Int, rohrtyp, tbl);
	DECLARE_COL(Int, hoehe, tbl);
	DECLARE_COL(Int, aktion, tbl);
	DECLARE_COL(Double, zeit, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new Montagezeiten::data;
		thedata->dn	= dn.data;
		thedata->rohrtyp = rohrtyp.data;
		thedata->hoehe = hoehe.data;
		thedata->zeit = zeit.data;
		thedata->aktion = aktion.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}



Montagezeiten::data::data()
{
	zeit = 0.0;
	hoehe = 0;
	rohrtyp = 0;
	dn = 0;
}

Montagezeiten::data::~data()
{
	
}

const int Montagezeiten::data::get_dn() const
{
	return dn;
}

void Montagezeiten::data::set_dn(int value)
{
	dn = value;
}

const int Montagezeiten::data::get_rohrtyp() const
{
	return rohrtyp;
}

void Montagezeiten::data::set_rohrtyp(int value)
{
	rohrtyp = value;
}

const int Montagezeiten::data::get_hoehe() const
{
	return hoehe;
}

void Montagezeiten::data::set_hoehe(int value)
{
	hoehe = value;
}

const double Montagezeiten::data::get_zeit() const
{
	return zeit;
}

void Montagezeiten::data::set_zeit(double value)
{
	zeit = value;
}


const int Montagezeiten::data::get_aktion() const
{
	return aktion;
}

void Montagezeiten::data::set_aktion(int value)
{
	aktion = value;
}
