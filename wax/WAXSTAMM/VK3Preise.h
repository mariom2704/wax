#pragma once

#include "StammDat.h"

class WAXSTAMMDLL VK3Preise : public StammDat
{
public:
	VK3Preise();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : public CObject
	{
		friend class VK3Preise;
	public:
		const std::string& get_bezeichnung() const;

		const int get_elemidcounter() const
		{
			return counter;
		};

		const int Get_FertigungsstaetteVk() const
		{
			return Fertigungsstaette;
		}
		
		const int get_elemid() const;

		const int get_dn() const;

		const int get_dnvl() const;

		const int get_rohr_laenge_von() const;

		const int get_rohr_laenge_bis() const;

		const int get_element_anzahl_von() const;

		const int get_element_anzahl_bis() const;

		const int get_element_laenge_von() const;

		const int get_element_laenge_bis() const;
	
		const BOOL get_element_anzahl_id() const
		{
			return	elementanzahlid;
		}


		const int get_PreisWertigkeit()const
		{
			return PreisWertigkeit;
		}

		const double get_element_anzahl_dnVon() const
		{
			return elementanzahldnVon;
		}

		const double get_element_anzahl_dnBis() const
		{
			return elementanzahldnBis;
		}

		const BOOL get_Is_Anlage() const
		{
			return  Anlage;
		}

		const double get_preis() const;

		const double get_preis_verzinkt() const;

	private:
		std::string bezeichnung;

		int counter;

		int Fertigungsstaette;

		int elemid;

		int dn;

		int dnvl;

		int rohrlaengevon;
		
		int rohrlaengebis;
		
		int elementanzahlvon;
		
		int elementanzahlbis;

		int	elementanzahlid;

		double elementanzahldnVon;

		double elementanzahldnBis;

		int elementlaengevon;

		int elementlaengebis;

		BOOL Anlage;

		double preis;

		double preis_verzinkt;

		int PreisWertigkeit;
	};

	

	const VK3Preise::data* query(int iFertigungsstätte, int id, int dn, int anschlussDN, int laengeZuschnitt, int AnzahlElemente, int ElementAnzahld, double ElementAnzahldn, int ElementLaenge, BOOL Anlage) const;
};
