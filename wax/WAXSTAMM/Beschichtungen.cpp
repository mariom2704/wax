#include "stdafx.h"
#include <mxodbc\mxodbc.h>



Beschichtungen::Beschichtungen() : StammDat("beschichtungen", "verfuegbar", "indx")
{
}

void Beschichtungen::load(const string& db)
{
	// Tabelle initialisieren
	MXTable besch(get_database(db), get_tbl());
	
	// Tabellenfelder definieren
	DECLARE_COL_LANGUAGE_WAX(String, bezeichnung, besch);
	DECLARE_COL(Int, indx,		besch);
	DECLARE_COL(Int, serial,	besch);
	DECLARE_COL(Int, colour,	besch);
	DECLARE_COL(Int, ral,		besch);
	DECLARE_COL(Int, afo1,		besch);
	DECLARE_COL(Int, afo2,		besch);
	DECLARE_COL(Int, afo3,		besch);
	DECLARE_COL(Int, afo4,		besch);
	DECLARE_COL(Int, afo5,		besch);

	
	besch.load(get_select(), get_order());


	lst.RemoveAll();
	
	while (!besch.isEof())
	{
		data* thedata = new Beschichtungen::data;
		thedata->index = indx.data;
		thedata->bezeichnung = *bezeichnung.data;
		thedata->serial = serial.data;
		thedata->colour = colour.data;
		thedata->ral = ral.data;
		thedata->afo1 = afo1.data;
		thedata->afo2 = afo2.data;
		thedata->afo3 = afo3.data;
		thedata->afo4 = afo4.data;
		thedata->afo5 = afo5.data;

		lst.AddTail(thedata);
		besch.moveNext();
	}
}

CString Beschichtungen::getBezeichnung(int indx)
{
	for (POSITION apos = getStammdaten().get_beschichtungen().GetHeadPosition(); apos;)
	{
		Beschichtungen::data *b = (Beschichtungen::data *)getStammdaten().get_beschichtungen().GetNext(apos);
		if (b->get_index() == indx)
			return b->get_bezeichnung();
	}
	return "";
}


Beschichtungen::data::data() : ral(0), index(0), colour(0), serial(0)
{
}

 int Beschichtungen::data::get_colour() const
{
	return colour;
}

 int Beschichtungen::data::get_serial() const
{
	return serial;
}

const CString& Beschichtungen::data::get_bezeichnung() const
{
	return bezeichnung;
}

 int Beschichtungen::data::get_index() const
{
	return index;
}

 int Beschichtungen::data::get_ral() const
{
	return ral;
}

const int Beschichtungen::data::get_afo1() const
{
	return afo1;
}

const int Beschichtungen::data::get_afo2() const
{
	return afo2;
}

const int Beschichtungen::data::get_afo3() const
{
	return afo3;
}

const int Beschichtungen::data::get_afo4() const
{
	return afo4;
}

const int Beschichtungen::data::get_afo5() const
{
	return afo5;
}

