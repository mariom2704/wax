#pragma once
#include "StammDat.h"

/**
Tabelle Sonderkosten mit vordefinierten Sokos. Klasse heisst Soko, weil in 
Rohrkonst bereits eine Klasse Sonderkosten existiert.
*/
class WAXSTAMMDLL Soko : public StammDat  
{
public:	
	Soko();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		const CString& get_bezeichnung() const;

		void set_bezeichnung(const CString& value);

		const double get_preis() const;

		void set_preis(double value);

	private:
		friend class Soko;

		CString bezeichnung;

		double preis;

	};
};
