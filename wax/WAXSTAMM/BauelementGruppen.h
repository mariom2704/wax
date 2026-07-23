#pragma once

#include "StammDat.h"

/**
Gruppierung der Bauelemente
z.B. Flach Flansch, Kehlmuffe mit ihren Eigenschaften
*/
class WAXSTAMMDLL BauelementGruppen : public StammDat  
{
public:
	//Konstruktor
	BauelementGruppen();
	
	virtual void load(const std::string& db);

	const CString getBezeichnung(int id) const;
	const CString getKurzbezeichnung(int id) const;
	
	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		data();

		 int get_elem_ID() const;

		const CString& get_bezeichnung() const;
		
		const CString& get_kurzbezeichnung() const;


		 bool get_kannDN2() const;

		 bool get_kannLaenge() const;

		 bool get_kannWinkel() const;

	private:
		friend class BauelementGruppen;

		int elem_ID;

		CString bezeichnung;

		CString kurzbezeichnung;

		bool kannDN2;

		bool kannLaenge;

		bool kannWinkel;

	};
};
