#include "stdafx.h"


#include "Krantaetigkeit.h"

#include <mxodbc\mxodbc.h>

/// Konstruktor
Krantaetigkeit::Krantaetigkeit(): StammDat("krantaetigkeit", "", "")
{
}

void Krantaetigkeit::load(const string& db)
{
///  Tabelle initialisieren
	MXTable kran(get_database(db), get_tbl());
	
	DECLARE_COL(Long, ind, kran);
	DECLARE_COL(Double, wert, kran);
	
	kran.load(get_select(), get_order());

	lst.RemoveAll();
	while (!kran.isEof())
	{
		data* thedata = new Krantaetigkeit::data;
		thedata->index = ind.data;
		thedata->wert = wert.data;
		lst.AddTail(thedata);
		kran.moveNext();
	}
}

double Krantaetigkeit::getWert(int index)
{
	for (POSITION gpos = getStammdaten().get_krantaetigkeit().GetHeadPosition(); gpos;)
	{
		Krantaetigkeit::data *k = (Krantaetigkeit::data *)getStammdaten().get_krantaetigkeit().GetNext(gpos);
		if (k->get_index() == index) 
			return k->get_wert();
	}
	return 0.0;
}


Krantaetigkeit::data::data() : index(0), wert(0.0)
{

}

const int Krantaetigkeit::data::get_index() const
{
	return index;
}

const double Krantaetigkeit::data::get_wert() const
{
	return wert;
}
