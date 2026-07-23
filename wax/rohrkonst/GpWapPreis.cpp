#include "stdafx.h"

#include "MaterialListe.h"
#include "Material.h"
#include "Arbeitsplan.h"
#include "Werkauftrag.h"
#include "GpWapPreis.h"




GpWapPreis::GpWapPreis(void)
{
}

GpWapPreis::~GpWapPreis(void)
{
}


//Material Herstelkostenzuschlag
double GpWapPreis::getHkMaterialkostenSatz()
{
	Preisfaktoren pf;
	return pf.getPreisfaktor(Preisfaktoren::HKMATERIALKOSTEN) / 100.0;
}

//Material Grenzkostenzuschlag
double GpWapPreis::getGkMaterialkostenSatz()
{
	Preisfaktoren pf;
	return pf.getPreisfaktor(Preisfaktoren::GKMATREIALKOSTEN) / 100.0;
}

//Vollkosten Zuschlag für Verwaltungskosten
double GpWapPreis::getVkVerwaltungskostenSatz()
{
	Preisfaktoren pf;
	return pf.getPreisfaktor(Preisfaktoren::VKVERWALTUNGSKOSTEN) / 100.0;
}

//Grenzkosten Zuschlag für Verwaltungskosten
double GpWapPreis::getGkVerwaltungskostenSatz()
{
	Preisfaktoren pf;
	return pf.getPreisfaktor(Preisfaktoren::GKVERWALTUNGSKOSTEN) / 100.0;
}

/**
Zuschlag auf die Materialpreise
*/
bool GpWapPreis::calcMaterialkosten(GpWapPreis::PREISART part, const MaterialListe &matliste, double& gesFestPreis, double& gesSapPreis, double& gesPulverpreis, double& zuschlagProzent, double& zuschlag)
{

	switch (part)
	{
		case GpWapPreis::Vollkosten:
			zuschlagProzent = getHkMaterialkostenSatz();
			break;
		case GpWapPreis::Grenzkosten:
			zuschlagProzent = getGkMaterialkostenSatz();
			break;
		default:
			return false;
	} 

	//Gesamtpreis berechnen
	gesSapPreis = 0;
	gesFestPreis = 0;
	gesPulverpreis = 0;
	for (POSITION pos = matliste.get_matlist().GetHeadPosition(); pos;)
	{
		Material *m = (Material*)matliste.get_matlist().GetNext(pos);

		//Stammdaten aktualisieren
		m->initStammdaten();

		if (m->get_mass1() > 0)
		{
			gesSapPreis += (double)m->get_mass1()/1000.0 * m->get_sapPreis();
			gesFestPreis += (double)m->get_mass1()/1000.0 * m->get_festPreis();

			if (m->get_pulverpreis() > 0)
				gesPulverpreis += (double)m->get_mass1()/1000.0 * m->get_pulverpreis();
		}
		else
		{
			gesSapPreis += (double)m->get_anzahl() * m->get_sapPreis();
			gesFestPreis += (double)m->get_anzahl() * m->get_festPreis();

			if (m->get_pulverpreis() > 0)
				gesPulverpreis += (double)m->get_anzahl() * m->get_pulverpreis();
		}
	}

	//Materialkostenzuschlag berechnen
	zuschlag = gesSapPreis * zuschlagProzent;

	return true;
}


bool GpWapPreis::calcFertigungskosten( vector<Werkauftrag*>&  auftraege, GpWapPreis::PREISART part, const Beschichtung &beschichtung, int maxlaenge, double gesGewicht, int wanr,
									  Arbeitsplan &ap, double& gesPreis, double& gesPreisMitzuschlag, double& gesZeit)
{
	Kostenstellen k;

	gesPreisMitzuschlag = 0;
	gesPreis = 0;
	gesZeit = 0;


	///Afos fuer Schweisser zusammenfassen 
	ap.mergeSchweissafos();
	
	///Mindermenge setzen fuer Arbeitsplan
	if(auftraege.size() == 1)
	{
		if(auftraege[0]->get_GUID().IsEmpty() == TRUE)
		{
			ap.setMindermenge();
		}
	}
	else
	{
		if(auftraege[1]->get_GUID().IsEmpty() == TRUE)
		{
			ap.setMindermenge();
		}
	}
	
	
	for (POSITION pos = ap.get_arbeitsfolgen().GetHeadPosition(); pos; )
	{
		Arbeitsfolge *folge = (Arbeitsfolge*)(ap.get_arbeitsfolgen().GetNext(pos));
	
		// Soll Afo zu SAP rüber
		if (!folge->get_sap() || folge->get_anz() <= 0)
			continue;
		
		//Afotext überarbeiten
		ap.updateSpecialAF(folge, beschichtung, gesGewicht, maxlaenge, wanr);

		//Gesamtzeit
		gesZeit += folge->get_zeit()+folge->get_ruestzeit();


		double std = 0;
		double zuschlag = 0;
		switch (part)
		{
			case GpWapPreis::Vollkosten:
				std = k.getTarif(folge->getKostst(folge->get_arbpl()));
				zuschlag = k.getZuschlag(folge->getKostst(folge->get_arbpl()));
				break;
			case GpWapPreis::Grenzkosten:
				std = k.getGrenztarif(folge->getKostst(folge->get_arbpl()));
				zuschlag = k.getGrenzZuschlag(folge->getKostst(folge->get_arbpl()));
				break;
			default:
				return false;
		}

		//Wenn keine Fremdleistung dann Preis berechnen
		if (folge->get_typ() == 0)
		{
			//Preis neu berechnen (Zeit nicht mehr mit Anzahl multiplizieren)
			folge->set_preis(std * (folge->get_zeit() + folge->get_ruestzeit()));

			folge->set_zuschlag(zuschlag);

			folge->set_ppstd(std);
		}		


		//Gesamtpreis
		gesPreis += folge->get_preis();		
		gesPreisMitzuschlag += folge->get_preis() * (1 + folge->get_zuschlag() / 100.0);
		
	}

	return true;
}


bool GpWapPreis::calcSelbstkosten(GpWapPreis::PREISART part, double gesMatKosten, double gesFertkosten, double gesFremd, double &zuschlag, double &zuschlagProzent, double &herstellkosten, double &selbstkosten)
{
	zuschlagProzent = 0;
	switch (part)
	{
		case GpWapPreis::Vollkosten:
			zuschlagProzent = getVkVerwaltungskostenSatz();
			break;
		case GpWapPreis::Grenzkosten:
			zuschlagProzent = getGkVerwaltungskostenSatz();
			break;
		default:
			return false;
	}

	herstellkosten = gesMatKosten + gesFertkosten + gesFremd;

	zuschlag = herstellkosten * zuschlagProzent;

	selbstkosten = herstellkosten + zuschlag;

	return true;
}