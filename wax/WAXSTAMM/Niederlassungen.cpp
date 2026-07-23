#include "stdafx.h"

#include <mxodbc\mxodbc.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Niederlassungen::Niederlassungen()
: StammDat("niederlassungen", "bezeichnung like '%'", "nlnummer")
{
	
}

void Niederlassungen::load(const string& db)
{
	// Tabelle initialisieren
	MXTable nls(get_database(db), get_tbl());
		
	// Tabellenfelder definieren
	DECLARE_COL(Int, nlnummer, nls);
	DECLARE_COL(String, bezeichnung, nls);
	DECLARE_COL(Int, region, nls);

	nls.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!nls.isEof())
	{
		data* thedata = new Niederlassungen::data;
		thedata->nlnummer	 = nlnummer.data;
		thedata->bezeichnung = *bezeichnung.data;
		thedata->region		 = region.data;
		lst.AddTail(thedata);
		nls.moveNext();
	}
}

 int Niederlassungen::data::get_nlnummer() const
{
	return nlnummer;
}

const CString& Niederlassungen::data::get_bezeichnung() const
{
	return bezeichnung;
}


int Niederlassungen::data::get_nRegion() const
{
	return region;
}


Niederlassungen::data::data() : nlnummer(0) 
{

}



