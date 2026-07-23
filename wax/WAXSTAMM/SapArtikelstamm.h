#pragma once
#include "stammdat.h"

class WAXSTAMMDLL SapArtikelstamm : public StammDat
{
public:
	SapArtikelstamm();
	virtual ~SapArtikelstamm();

	/// Laden der Daten aus der Stammdatenbank, Tabelle Bauelemente
	virtual void load(const std::string& db);

	/// Datenfelder der Tabelle Bauelemente
	class WAXSTAMMDLL data : public CObject  
	{
	public:
		data();

		const long get_artikelnr() const;

		const CString& get_einheit() const;

		const CString& get_bezeichnung() const;

		const double get_preis() const;

		const double get_gewicht() const;

		friend class SapArtikelstamm;

	private:

		double gewicht;

		double preis;

		long	m_artikelnr;
		 
		CString m_einheit;

		CString bezeichnung;
	};
};
