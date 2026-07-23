#include "stdafx.h"
#include <mxodbc\mxodbc.h>



Soko::Soko() : StammDat("sonderkosten", "", "")
{
}

void Soko::load(const string& db)
{
	// Tabelle initialisieren
	MXTable sokos(get_database(db), get_tbl());
	
	// Tabellenfelder definieren
	DECLARE_COL(String, bezeichnung, sokos);
	DECLARE_COL(Double, preis, sokos);
	
	// Tabelle öffnen und Daten laden
	sokos.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!sokos.isEof())
	{
		data* thedata = new Soko::data;
		thedata->bezeichnung = *bezeichnung.data;
		thedata->preis = preis.data;

		lst.AddTail(thedata);
		sokos.moveNext();
	}
}




const CString& Soko::data::get_bezeichnung() const
{
	return bezeichnung;
}

void Soko::data::set_bezeichnung(const CString& value)
{
	bezeichnung = value;
}

const double Soko::data::get_preis() const
{
	return preis;
}

void Soko::data::set_preis(double value)
{
	preis = value;
}

