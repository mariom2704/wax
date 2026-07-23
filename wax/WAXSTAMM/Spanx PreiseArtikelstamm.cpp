#include "stdafx.h"



#include "Spanx PreiseArtikelstamm.h"

#include <mxodbc\mxodbc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Konstruktor
CSpanxPreiseArtikelstamm::CSpanxPreiseArtikelstamm() : StammDat("", "", "")
{}

//Laden der Daten aus der Stammdatenbank, Tabelle CSpanxPreiseArtikelstamm
void CSpanxPreiseArtikelstamm::load(const string& db)
{

// Tabelle initialisieren
	MXTable CSpanxPreiseArtikelstamm(get_database(db), ""); //join aus CSpanxPreiseArtikelstamm und Artikelstamm

	DECLARE_COL(Long, Artikelnummer, CSpanxPreiseArtikelstamm);
	DECLARE_COL(Long, artikelgruppe, CSpanxPreiseArtikelstamm);
	DECLARE_COL(Double, Preis, CSpanxPreiseArtikelstamm);
	DECLARE_COL(String, ArtikelstammKurztext, CSpanxPreiseArtikelstamm);
	DECLARE_COL(String, Unterartikelgruppe2Kurztext, CSpanxPreiseArtikelstamm);
	DECLARE_COL(Long, Classid, CSpanxPreiseArtikelstamm);
	DECLARE_COL(Long, Region, CSpanxPreiseArtikelstamm);
	
	
	string sql = "SELECT artikelnummer, artikelgruppe, Preis, Artikelstamm.Bezeichnung" +  WaxSys::getLanguage() + ", SpanxUnterartikelgruppe.Bezeichnung" +  WaxSys::getLanguage() + ", Classid, region \
				FROM (SpanxUnterartikelgruppe INNER JOIN Artikelstamm ON SpanxUnterartikelgruppe.Id = Artikelstamm.Artikelgruppe) INNER JOIN SpanxPreise ON Artikelstamm.Artikelnummer = SpanxPreise.Materialstamm_ArtNr";

	CSpanxPreiseArtikelstamm.set_sql(sql);
	CSpanxPreiseArtikelstamm.load();

	lst.RemoveAll();
	
	while (!CSpanxPreiseArtikelstamm.isEof())
	{

		data* thedata = new CSpanxPreiseArtikelstamm::data;
		thedata->Artikelnummer = Artikelnummer.data;	
		thedata->artikelgruppe =  artikelgruppe.data;
		thedata->ArtikelstammKurztext = *ArtikelstammKurztext.data;
		thedata->Unterartikelgruppe2Kurztext = *Unterartikelgruppe2Kurztext.data;
		thedata->Region = Region.data;
		thedata->Preis = Preis.data;
		thedata->Classid = Classid.data;

		lst.AddTail(thedata);
		CSpanxPreiseArtikelstamm.moveNext();
	}
	
}


CSpanxPreiseArtikelstamm::data::data() 
{}

const long CSpanxPreiseArtikelstamm::data::get_artikelgruppenID() const
{
	return artikelgruppe;
}


const long CSpanxPreiseArtikelstamm::data::get_artikelnr() const
{
	return Artikelnummer;
}


const CString CSpanxPreiseArtikelstamm::data::get_ArtikelstammKurztext() const
{
	return ArtikelstammKurztext;
}


const CString CSpanxPreiseArtikelstamm::data::get_Unterartikelgruppe2Kurztext() const
{
	return Unterartikelgruppe2Kurztext;
}


const long CSpanxPreiseArtikelstamm::data::get_Region() const
{
	return Region;
}


const double CSpanxPreiseArtikelstamm::data::get_Classid() const
{
	return Classid;
}		


const double CSpanxPreiseArtikelstamm::data::get_Preis() const
{
	return Preis;
}



