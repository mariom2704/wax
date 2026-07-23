#include "stdafx.h"
#include <mxodbc\mxodbc.h>

Verzinkung::Verzinkung() : StammDat("Verzinkung", "", "")
{
}


void Verzinkung::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, nr, tbl); // Laufende Nummer als Kennung
	DECLARE_COL(String,	name, tbl); // Name der Verzinkerei: OTD oder OTN
	DECLARE_COL(Double, euro_pro_kg, tbl); // Kosten für Verzinkung von einem Kg Rohr
	DECLARE_COL(Double, gewichtszuschlag, tbl);
	DECLARE_COL(Double, zinkpreisausgleich, tbl); // Aufschlag auf Kosten Zink/Kg
	DECLARE_COL(Double, stdsatz, tbl); // Stundensatz in Euro pro Stunde
	DECLARE_COL(Double, vkzuschlag, tbl); // Aufschlag auf VK
	DECLARE_COL(Double, mgk, tbl); // MGK

	tbl.load(get_select(), get_order());
	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new Verzinkung::data;
		thedata->nr = nr.data;
		thedata->name = *name.data;
		thedata->euro_pro_kg = euro_pro_kg.data;
		thedata->gewichtszuschlag = gewichtszuschlag.data;
		thedata->zinkpreisausgleich = zinkpreisausgleich.data;
		thedata->stdsatz = stdsatz.data;
		thedata->vkzuschlag = vkzuschlag.data;
		thedata->mgk = mgk.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

const Verzinkung::data* Verzinkung::query(int nr) const
{
	for (POSITION pos = lst.GetHeadPosition(); pos;)
	{
		const Verzinkung::data *z= (Verzinkung::data*)lst.GetNext(pos);
		if (z->get_nr() == nr)
			return z;
	}
	return NULL;
}

Verzinkung::data::data() : nr(0), euro_pro_kg(0.0), gewichtszuschlag(0.0), zinkpreisausgleich(0.0), stdsatz(0.0), vkzuschlag(0.0), mgk(0.0)
{
}


int Verzinkung::data::get_nr() const
{
	return nr;
}

CString Verzinkung::data::get_name() const
{
	return name;
}

double Verzinkung::data::get_euro_pro_kg() const
{
	return euro_pro_kg;
}

double Verzinkung::data::get_gewichtszuschlag() const
{
	return gewichtszuschlag;
}

double Verzinkung::data::get_zinkpreisausgleich() const
{
	return zinkpreisausgleich;
}
double Verzinkung::data::get_stdsatz() const
{
	return stdsatz;
}

double Verzinkung::data::get_vkzuschlag() const
{
	return vkzuschlag;
}

double Verzinkung::data::get_mgk() const
{
	return mgk;
}
