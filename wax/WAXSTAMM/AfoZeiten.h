#pragma once

#include "StammDat.h"

class WAXSTAMMDLL AfoZeiten : 
public StammDat  
{
public:
	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		data();

		const int get_dnvl_von() const;

		const int get_dnvl_bis() const;

		const int get_afoid() const;

		const int get_dn() const;

		const int get_elemid() const;

		const int get_laenge_bis() const;

		const int get_laenge_von() const;

		const int get_winkel_bis() const;

		const int get_winkel_von() const;

		const double get_zeit() const;

		const double get_zinkzuschlag() const;

		const int get_element_anzahl_von() const
		{
			return ElementAnzahlVon;
		}

		const int get_element_anzahl_bis() const
		{
			return ElementAnzahlBis;
		}

		const int get_element_anzahl_Id() const
		{
			return	ElementAnzahlId;
		}

		const double get_element_anzahl_DnVon() const
		{
			return	ElementAnzahlDnVon;
		}

		const double get_element_anzahl_DnBis() const
		{
			return	ElementAnzahlDnBis;
		}

		const int get_produktTyp() const
		{
			return	produkttyp;
		}

		const int get_Fertigungsstaette() const
		{
			return fertigungsstaette;
		}

		const bool get_makeAnlage()const
		{
			return  Anlage > 0 ? true : false;
		}

		const CString get_Arbeitsplatz()const
		{
			return  arbeitsplatz;
		}

	private:

		int fertigungsstaette;

		CString arbeitsplatz;

		int produkttyp;

		int dnvl_von;

		int dnvl_bis;

		int afoid;

		int elemid;

		int dn;
		
		int laenge_von;

		int laenge_bis;

		int	ElementAnzahlVon;

		int	ElementAnzahlBis;

		int	ElementAnzahlId;

		double	ElementAnzahlDnVon;

		double	ElementAnzahlDnBis;

		BOOL Anlage;


		int winkel_von;

		int winkel_bis;

		double zeit;

		double zinkzuschlag;

		

	friend class AfoZeiten;
	};


	AfoZeiten();

	virtual void load(const std::string& db);
};


