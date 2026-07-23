#pragma once

#include "StammDat.h"

/// Dateninhalt der Tabelle Sondermaterial
class WAXSTAMMDLL Sondermaterial : public StammDat
{
public:
	public:
	virtual void load(const std::string& db);

	Sondermaterial();

	class WAXSTAMMDLL data : public CObject
	{
	public:
		const long get_artikelnr() const;

		const CString& get_bez() const;

		const CString& get_kurztext() const;

		const long get_dn() const;

		const long get_dnvl() const;

		const bool get_einkauf() const;

		const long get_elemid() const;


		const double get_gewicht() const;

		const bool get_halbzeug() const;

		const long get_mass1() const;

		const long get_mass2() const;

		const double get_preis() const;

		const bool get_co2tikko() const;

		data();

	private:
		/// Bauelementespezifisches Mass
		long mass1;

		/// Bauelementespezifisches Mass
		long mass2;

		/// SAP Artikelnummer
		long artikelnr;

		/// Nennweite
		long dn;

		/// Rohrnennweite
		long dnvl;

		/// Gewicht pro Einheit
		double gewicht;

		/// Preis
		double preis;

		/// zusaetzlicher Text zur Standardbezeichnung aus Tabelle Bauelementegruppen
		CString bez;

		/// Standardbezeichnung aus Tabelle Bauelementegruppen plus zusaetzlicher Text
		CString kurztext;

		/// Flag für die spätere Preisberechnung: Halbzeuge bekommen einen anderen 
	/// Aufschlag als Rohteile
		bool halbzeug;

		/// Einkaufmaterial
		bool einkauf;


		/// Elemente ID aus Tabelle
		long elemid;

		bool co2tikko;

	friend class Sondermaterial;
	};
};

