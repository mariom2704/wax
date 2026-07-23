#include "stdafx.h"

#include <mxodbc\mxodbc.h>



PutzenZeiten::PutzenZeiten() : StammDat("putzen_zeiten", "", "")
{

}

void PutzenZeiten::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, elemid, tbl);
	DECLARE_COL(Int, dn, tbl);
	DECLARE_COL(Double, zeit, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new PutzenZeiten::data;
		thedata->elemid	= elemid.data;
		thedata->dn		= dn.data;
		thedata->zeit	= zeit.data;
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

PutzenZeiten::data::data() : zeit(0.0)
{

}

const int PutzenZeiten::data::get_elemid() const
{
	return elemid;
}

const int PutzenZeiten::data::get_dn() const
{
	return dn;
}

const double PutzenZeiten::data::get_zeit() const
{
	return zeit;
}

double PutzenZeiten::getZeit(int id, int dn) 
{
	// in DB Angaben in Minuten
	PutzenZeiten::data *pz = NULL;
	for (POSITION pzpos = getStammdaten().get_putzenzeiten().GetHeadPosition(); pzpos;)
	{
		pz = (PutzenZeiten::data *)getStammdaten().get_putzenzeiten().GetNext(pzpos);

		if (pz->get_elemid() == id && pz->get_dn() == dn)				 
			return pz->get_zeit()/60;
	}
	return 0;
}