#include "stdafx.h"



#include <mxodbc\mxodbc.h>

//Konstruktor
Sondermaterial::Sondermaterial(): StammDat("sondermaterial", "", "elemid, dn")
{
}

//Laden der Daten aus der Stammdatenbank, Tabelle Sondermaterial
void Sondermaterial::load(const string& db)
{
// Tabelle initialisieren
	MXTable soma(get_database(db), get_tbl());
	
	DECLARE_COL(Long, elemid, soma);
	DECLARE_COL(String, bez, soma);
	DECLARE_COL_LANGUAGE_WAX(String, kurztext, soma);
	DECLARE_COL(Long, dn, soma);
	DECLARE_COL(Long, dnvl, soma);
	DECLARE_COL(Long, artikelnr, soma);
	DECLARE_COL(Double, preis, soma);
	DECLARE_COL(Double, gewicht, soma);
	DECLARE_COL(Long, mass1, soma);
	DECLARE_COL(Long, mass2, soma);
	DECLARE_COL(Bool, halbzeug, soma);
	DECLARE_COL(Bool, einkauf, soma);
	DECLARE_COL(Bool, co2tikko, soma);
	
	soma.load(get_select(), get_order());

	lst.RemoveAll();
	while (!soma.isEof())
	{
		data* thedata = new Sondermaterial::data;
		thedata->dn = dn.data;
		thedata->dnvl = dnvl.data;
		thedata->artikelnr = artikelnr.data;
		thedata->preis = preis.data;
		thedata->gewicht = gewicht.data;
		thedata->mass1 = mass1.data;
		thedata->mass2 = mass2.data;
		thedata->elemid = elemid.data;
		thedata->bez = *bez.data;
		thedata->kurztext = *kurztext.data;
		thedata->halbzeug = halbzeug.data ? true : false;
		thedata->einkauf = einkauf.data ? true : false;
		thedata->co2tikko = co2tikko.data ? true : false;
		lst.AddTail(thedata);
		soma.moveNext();
	}
}


Sondermaterial::data::data() : mass1(0), mass2(0), artikelnr(0), dn(0), dnvl(0), gewicht(0.0), preis(0.0), halbzeug(false), einkauf(false), elemid(0)
{
}

const long Sondermaterial::data::get_artikelnr() const
{
	return artikelnr;
}

const CString& Sondermaterial::data::get_bez() const
{
	return bez;
}

const CString& Sondermaterial::data::get_kurztext() const
{
	return kurztext;
}

const long Sondermaterial::data::get_dn() const
{
	return dn;
}

const long Sondermaterial::data::get_dnvl() const
{
	return dnvl;
}

const bool Sondermaterial::data::get_einkauf() const
{
	return einkauf;
}

const long Sondermaterial::data::get_elemid() const
{
	return elemid;
}


const double Sondermaterial::data::get_gewicht() const
{
	return gewicht;
}

const bool Sondermaterial::data::get_halbzeug() const
{
	return halbzeug;
}

const long Sondermaterial::data::get_mass1() const
{
	return mass1;
}

const long Sondermaterial::data::get_mass2() const
{
	return mass2;
}

const double Sondermaterial::data::get_preis() const
{
	return preis;
}

const bool Sondermaterial::data::get_co2tikko() const
{
	return co2tikko;
}

