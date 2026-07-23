#include "stdafx.h"



#include "Bauelemente.h"

#include <mxodbc\mxodbc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Konstruktor
Bauelemente::Bauelemente() : StammDat("", "", "")
{}

//Laden der Daten aus der Stammdatenbank, Tabelle Bauelemente
void Bauelemente::load(const string& db)
{
	double toleranz = 0.000001;

// Tabelle initialisieren
	MXTable bauele(get_database(db), "tmpTable"); //join aus bauelemente und Artikelstamm
	
	DECLARE_COL(Long, elemid, bauele);
	DECLARE_COL(Long, dn, bauele);
	DECLARE_COL(Long, dnvl, bauele);
	DECLARE_COL(Long, artikelnr, bauele);
	DECLARE_COL(Long, mass1, bauele);
	DECLARE_COL(Long, mass2, bauele);
	DECLARE_COL(Bool, halbzeug, bauele);
	DECLARE_COL(Bool, einkauf, bauele);
	DECLARE_COL(Bool, co2tikko, bauele);
	DECLARE_COL(Double, SapPreis, bauele);
	DECLARE_COL(Double, FestPreis, bauele);
	DECLARE_COL(Double, Gewicht, bauele);

	string sql = "SELECT b.elemid, b.dn, b.dnvl, b.artikelnr, b.mass1, b.mass2, b.halbzeug, b.einkauf, b.co2tikko, a.SapPreis, a.FestPreis, a.Gewicht ";
	sql +=	     "FROM bauelemente b LEFT JOIN artikelstamm  a ON b.artikelnr = a.Artikelnummer ";
	sql +=		 "order by b.elemid, b.dn, b.mass1";

	bauele.set_sql(sql);

	bauele.load();

	lst.RemoveAll();
	while (!bauele.isEof())
	{
		data* thedata = new Bauelemente::data;
		thedata->dn = dn.data;
		thedata->dnvl = dnvl.data;
		thedata->artikelnr = artikelnr.data;

		if (FestPreis.data < toleranz)
		{
			thedata->festPreis = SapPreis.data;
			if (thedata->festPreis < toleranz)
				thedata->festPreis = 0;
		}
		else
			thedata->festPreis = FestPreis.data;

		thedata->sapPreis = SapPreis.data;

		thedata->gewicht = Gewicht.data;
		if (thedata->gewicht < toleranz)
			thedata->gewicht = 0;

		thedata->mass1 = mass1.data;
		thedata->mass2 = mass2.data;
		thedata->elemid = elemid.data;
		thedata->halbzeug = halbzeug.data ? true : false;
		thedata->einkauf = einkauf.data ? true : false;
		thedata->co2tikko = co2tikko.data ? true : false;
		lst.AddTail(thedata);

		bauele.moveNext();
	}
}
#if 0
const Bauelemente::data* Bauelemente::query(int artikelnr, int id) const
{
	for (POSITION pos = lst.GetHeadPosition(); pos;)
	{
		const Bauelemente::data* b= (Bauelemente::data*)lst.GetNext(pos);
		if (b->get_artikelnr() == artikelnr)
		{
			if (!id || (id == b->get_elemid()))
				return b;
		}
	}
	return NULL;
}
#endif

Bauelemente::data::data() : mass2(0), mass1(0), gewicht(0.0), festPreis(0.0), sapPreis(0.0), artikelnr(0), dn(0), halbzeug(false), elemid(0), dnvl(0), einkauf(false), co2tikko(false)
{
}

const long Bauelemente::data::get_artikelnr() const
{
	return artikelnr;
}

const long Bauelemente::data::get_dn() const
{
	return dn;
}

const long Bauelemente::data::get_dnvl() const
{
	return dnvl;
}

const long Bauelemente::data::get_elemid() const
{
	return elemid;
}

const double Bauelemente::data::get_gewicht() const
{
	return gewicht;
}

const long Bauelemente::data::get_mass1() const
{
	return mass1;
}

const long Bauelemente::data::get_mass2() const
{
	return mass2;
}

const double Bauelemente::data::get_festPreis() const
{
	return festPreis;
}

/**
GLD aus Sap
*/
const double Bauelemente::data::get_sapPreis() const
{
	return sapPreis;
}



const bool Bauelemente::data::get_halbzeug() const
{
	return halbzeug;
}

const bool Bauelemente::data::get_einkauf() const
{
	return einkauf;
}

const bool Bauelemente::data::get_co2tikko() const
{
	return co2tikko;
}


