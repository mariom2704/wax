#pragma once

#include "StammDat.h"

/**
Dateninhalt der Tabelle CSpanxPreiseArtikelstamm
*/
class WAXSTAMMDLL CSpanxPreiseArtikelstamm : public StammDat  
  
{
public:
	/// Konstruktor
	CSpanxPreiseArtikelstamm();

	/// Laden der Daten aus der Stammdatenbank, Tabelle CSpanxPreiseArtikelstamm
	virtual void load(const std::string& db);

	/// Datenfelder der Tabelle CSpanxPreiseArtikelstamm
class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		data();

		const long get_artikelnr() const;

		const long get_artikelgruppenID() const;
		
		const CString get_ArtikelstammKurztext() const;

		const CString get_Unterartikelgruppe2Kurztext() const;

		const long get_Region() const;

		const double get_Classid() const;	

		const double get_Preis() const;


	private:

	

		long Artikelnummer;

		long artikelgruppe;
	
		double Preis;

		CString ArtikelstammKurztext;

		CString Unterartikelgruppe2Kurztext;

		long Classid;

		long Region;

		friend class CSpanxPreiseArtikelstamm;
	};

};


