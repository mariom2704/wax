#include "stdafx.h"


#include ".\sapartikelstamm.h"

#include <mxodbc\mxodbc.h>

SapArtikelstamm::SapArtikelstamm() : StammDat("artikelstamm", "", "Artikelnummer")
{
}

SapArtikelstamm::~SapArtikelstamm()
{
}


//Laden der Daten aus der Stammdatenbank, Tabelle Bauelemente
void SapArtikelstamm::load(const string& db)
{
// Tabelle initialisieren
	MXTable tabArtikelstamm(get_database(db), get_tbl());
	
	DECLARE_COL(Long, Artikelnummer, tabArtikelstamm);
	DECLARE_COL(String, Einheit, tabArtikelstamm);
	DECLARE_COL(Double, SapPreis, tabArtikelstamm);
	DECLARE_COL(Double, Gewicht, tabArtikelstamm);
	DECLARE_COL_LANGUAGE_WAX_EXT(String, Bezeichnung, tabArtikelstamm);
	
	tabArtikelstamm.load(get_select(), get_order());

	lst.RemoveAll();
	while (!tabArtikelstamm.isEof())
	{
		data* thedata = new SapArtikelstamm::data;
		thedata->m_artikelnr = Artikelnummer.data;
		thedata->m_einheit   = *Einheit.data;
		thedata->bezeichnung   = *Bezeichnung.data;
		thedata->preis = SapPreis.data;
		thedata->gewicht = Gewicht.data;

		lst.AddTail(thedata);
		tabArtikelstamm.moveNext();
	}
}


SapArtikelstamm::data::data() : m_artikelnr(0)
{
}

const long SapArtikelstamm::data::get_artikelnr() const
{
	return m_artikelnr;
}

const CString& SapArtikelstamm::data::get_einheit() const
{
	return m_einheit;
}


const CString& SapArtikelstamm::data::get_bezeichnung() const
{
	return bezeichnung;
}


const double SapArtikelstamm::data::get_preis() const
{
	return preis;
}


const double SapArtikelstamm::data::get_gewicht() const
{
	return gewicht;
}