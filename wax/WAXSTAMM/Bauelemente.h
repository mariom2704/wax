#pragma once

#include "StammDat.h"

/**
Dateninhalt der Tabelle Bauelemente
*/
class WAXSTAMMDLL Bauelemente : public StammDat  
  
{
public:
	/// Konstruktor
	Bauelemente();

	/// Laden der Daten aus der Stammdatenbank, Tabelle Bauelemente
	virtual void load(const std::string& db);

	/// Datenfelder der Tabelle Bauelemente
class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		data();

		const long get_artikelnr() const;

		const long get_dn() const;

		const long get_dnvl() const;

		const long get_elemid() const;

		const double get_gewicht() const;

		const long get_mass1() const;

		const long get_mass2() const;

		const double get_festPreis() const;

		const double get_sapPreis() const;

		const bool get_halbzeug() const;

		const bool get_einkauf() const;

		const bool get_co2tikko() const;

	private:
	/// Bauelementespezifisches Mass
		long mass2;

	/// Bauelementespezifisches Mass
		long mass1;

	/// Gewicht pro Einheit
		double gewicht;

	/// Preis
		double festPreis;

	/// SapPreis (GLD)
		double sapPreis;

	/// SAP Artikelnummer
		long artikelnr;

	/// Durchmesser Nennweite Abgang
		long dnvl;

	/// Durchmesser Nennweite
		long dn;

	/// Elemente ID aus Tabelle
		long elemid;


	/// Flag für die spätere Preisberechnung: Halbzeuge bekommen einen anderen 
	/// Aufschlag als Rohteile
		bool halbzeug;

		bool einkauf;

		bool co2tikko;
	
	friend class Bauelemente;
	};

	//const Bauelemente::data* query(int artikelnr, int id = 0) const;
};


