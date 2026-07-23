#pragma once
#include "StammDat.h"

class WAXSTAMMDLL DummyMaterialnummern : public StammDat  
{
public:

	DummyMaterialnummern();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		const bool get_verzinkt() const;

		const CString& get_klasse() const;

		data();

		const int get_nummer() const;

		const CString& get_iPPEClassName() const;

		const CString& get_iPPEKurzText() const;

		const CString& get_verwendung() const;

		const CString& get_bezeichnung() const;

		const int get_lieferzeit() const;

		const CString& get_segment() const;

	private:
		bool verzinkt;

	/// Gültigkeit für Objektklasse im Wax
		CString klasse;

	// Mepping zur iPPEClasse
		CString iPPEClassenName;

		CString iPPEKurzText;

	/// Hinweis zur Verwendung
		CString verwendung;

	/// Niederlassungsbezeichnung
		CString bezeichnung;

	/// Ib/Nl Nummer
		int nummer;

		int lieferzeit;

		CString segment;

	friend class DummyMaterialnummern;
	};
};
