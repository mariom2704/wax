#include "stdafx.h"

#include <mxodbc\mxodbc.h>


#include "Preisfaktoren.h"

Preisfaktoren::Preisfaktoren() : StammDat("preisfaktoren", "", "pindex")
{

}

void Preisfaktoren::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int,	pindex,	 tbl);
	DECLARE_COL(Double, prozent, tbl);
	DECLARE_COL(String, text, 	 tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new Preisfaktoren::data;
		thedata->index   = pindex.data;
		thedata->prozent = prozent.data;
		thedata->text	 = *text.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

Preisfaktoren::data::data() : prozent(0.0), text("")
{

}

const int Preisfaktoren::data::get_index() const
{
	return index;
}

const double Preisfaktoren::data::get_prozent() const
{
	return prozent;
}

const CString& Preisfaktoren::data::get_text() const
{
	return text;
}

double Preisfaktoren::getPreisfaktor(int index)
{
	double faktor = 0;
	Preisfaktoren::data *pf = NULL;
	for (POSITION pfpos = getStammdaten().get_preisfaktoren().GetHeadPosition(); pfpos;)
	{
		pf = (Preisfaktoren::data *)getStammdaten().get_preisfaktoren().GetNext(pfpos);
		if (pf->get_index() == index)
		{
			faktor = pf->get_prozent();
			break;
		}
	}
	return faktor;
}
