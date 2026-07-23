#include "stdafx.h"
#include "resource.h"



#include "Pumpenverteiler.h"
#include "Element.h"


#include <mxutil/ddx_extension.h>


using namespace MxErrorHandling;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//Destruktor
Pumpenverteiler::~Pumpenverteiler()
{
}

//Konstruktor
Pumpenverteiler::Pumpenverteiler()
{
	reduziert        = true;
	gesamthoehe      = 600;
	
	dnPumpDruckSt    = 80;
	winkelRohnippel  = 0;

	dnProbAnschl     = 100;
	probAnschl       = NUT;
	laengeProbAnschl = 160;
	abstandProbAnschl= 411;

	dnSprAnlAnschl   = 100;
	sprAnlAnschl     = FFL;

	schwMuffe        = true;
	winkelSchwMuffe  = 180;
	abstandSchwMuffe = 80;

	stdbezeichnung = getStammdaten().getText("IDS_PUMPENVERT").c_str();
}

//Speichern und Laden der Rohrleitung. Achtung:
void Pumpenverteiler::serialize(BinaryArchive& ar)
{
	Werkauftrag::serialize(ar);
	serialize_bool(schwMuffe, ar);
	if (ar.is_storing())
	{
		ar << gesamthoehe;
		ar << dnPumpDruckSt;
		ar << winkelRohnippel;
		ar << dnProbAnschl;
		ar << probAnschl;
		ar << laengeProbAnschl;
		ar << abstandProbAnschl;
		ar << dnSprAnlAnschl;
		ar << sprAnlAnschl;
		ar << winkelSchwMuffe;
		ar << abstandSchwMuffe;
	}
	else
	{
		ar >> gesamthoehe;
		ar >> dnPumpDruckSt;
		ar >> winkelRohnippel;
		ar >> dnProbAnschl;
		ar >> probAnschl;
		ar >> laengeProbAnschl;
		ar >> abstandProbAnschl;
		ar >> dnSprAnlAnschl;
		ar >> sprAnlAnschl;
		ar >> winkelSchwMuffe;
		ar >> abstandSchwMuffe;
	}
}

//Rohrkonstruktion auf korrekte Werte checken
CString Pumpenverteiler::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err;
	

	if (dnProbAnschl > dnSprAnlAnschl)
	{
		err = getStammdaten().getText("IDS_STRING2233").c_str();
		return err;
	}


	if (abstandSchwMuffe > dnPumpDruckSt*2.5 && dnPumpDruckSt != dnSprAnlAnschl)
	{
		err = getStammdaten().getText("IDS_ERROR67").c_str();
		return err;
	}
	
	if (abstandSchwMuffe >= gesamthoehe)
	{
		err = getStammdaten().getText("IDS_ERROR68").c_str();
		return err;
	}

	// min. 2.5*dn1 + redlaenge + 40 + 1/2d
	DNzuAussendurchmesser dntoad;
	int aussendurchmesser = dntoad.getAussendurchmesserToInt(dnProbAnschl);
	if (reduziert)
	{
		int abstand = 0;

		Element *red = new Element;
		red->initStammdaten(dnSprAnlAnschl, dnPumpDruckSt, KRED);
		red->set_laenge((int)red->get_mass1());
		red->set_abstAbs((int)(2.5 * (double)dnPumpDruckSt));
		
		
		abstand = red->get_abstAbs() + red->get_laenge() + 40 + aussendurchmesser/2;

		if (abstandProbAnschl < abstand)
		{
			err = getStammdaten().getText("IDS_PROBZUDICHTANRED").c_str();
			return err;
		}
		delete red;
	}

	if (abstandProbAnschl > gesamthoehe - 100 - aussendurchmesser/2)
	{
		err.Format(getStammdaten().getText("IDS_ERROR69").c_str(), 100+aussendurchmesser/2);
		return err;
	}

	aussendurchmesser = dntoad.getAussendurchmesserToInt(dnSprAnlAnschl);
	if (laengeProbAnschl < aussendurchmesser/2+100)
	{
        err = getStammdaten().getText("IDS_PROBZUKURZ").c_str();
		return err;
	}
	else if (laengeProbAnschl > aussendurchmesser/2+500)
	{
        err = getStammdaten().getText("IDS_PROBZULANG").c_str();
		return err;
	}

	int dn1dn3[][3] = 
	{
		50, 80, 0, 
		50, 100, 0,
		65, 80, 0,
		65, 100, 0,
		80, 100, 0,
		80, 125, 0,
		80, 150, 0,
		100, 150, 0,
		125, 150, 0,
		125, 200, 0,
		150, 200, 0,
		0, 0, 0
	};
	
	bool kombination_falsch = true;

	int dn1 = dnPumpDruckSt;
	int dn3 = dnSprAnlAnschl;

	if (dn1 == dn3)
		return err; // Keine Reduzierung
	
	CString msg; msg = getStammdaten().getText("IDS_ERROR70").c_str();
	int i;
	for (i = 0; dn1dn3[i][0]; i++)
	{
		if (dn1 == dn1dn3[i][0] && dn3 == dn1dn3[i][1])
		{
			kombination_falsch = false;
			break; // Kombination ok
		}
		if (dn1 == dn1dn3[i][0] || dn3 == dn1dn3[i][1] )
		{
			CString tmp;
			tmp.Format("DN1 = %i  DN3 = %i\n", dn1dn3[i][0], dn1dn3[i][1]);
			msg += tmp;
		}
	}
	if (kombination_falsch)
	{
		err = msg;
		return err;
	}
	else if (!dn1dn3[i][0]) // Keine gültiger Wert für dn1 in dn1dn3 Tabelle 
	{
		err = getStammdaten().getText("IDS_ERROR71").c_str();
		return err;
	}
	return "";
}

//Hier wird automatisch der Arbeitsplan generiert
void Pumpenverteiler::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	Verteiler::createAutoArbeit();
}

//Rohrkonstruktion erzeugen
void Pumpenverteiler::createModell()
{
	// Löschen aller Elemente aus der Liste
	elementList.deleteAllElemente();

	set_dnvl(dnPumpDruckSt);

	reduziert = dnPumpDruckSt != dnSprAnlAnschl;

	fill_elementArray();
	set_laenge(gesamthoehe);

	set_rohrAnfang(FFL );
	set_rohrEnde(sprAnlAnschl);
	Verteiler::createModell();
}

//Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
//Stationsverteiler oder Entnahmeleitungen
void Pumpenverteiler::fill_elementArray()
{
	elementArray.deleteAll();
	Element *probieranschluss = new Element;
	Element *rohrnippel = new Element;
	Element *muffe = new Element;
	Element *probanschlEnde = new Element;
	Element *reduzierung = new Element;
	reduziert = dnPumpDruckSt != dnSprAnlAnschl;
	try
	{
		if (schwMuffe)
		{
			muffe->initStammdaten(50, dnPumpDruckSt, MUFFE);
			//muffe->initStammdaten(50, dnSprAnlAnschl, MUFFE);
			muffe->set_laenge((int)rohrnippel->get_mass1());
			muffe->set_abstAbs(abstandSchwMuffe);
			muffe->set_abstRel(abstandSchwMuffe);
			muffe->set_winkel(winkelSchwMuffe );
		}

		rohrnippel->initStammdaten(15, dnPumpDruckSt, ROHRNIPPEL);
		rohrnippel->set_laenge((int)rohrnippel->get_mass1());
		rohrnippel->set_abstAbs(dnPumpDruckSt);
		rohrnippel->set_abstRel(dnPumpDruckSt);
		rohrnippel->set_winkel(winkelRohnippel);

		probieranschluss->initStammdaten(dnProbAnschl, dnSprAnlAnschl, ROHR);
		probieranschluss->set_laenge(laengeProbAnschl);
		probieranschluss->set_abstAbs(abstandProbAnschl);
		probieranschluss->set_abstRel(0);

		if (probAnschl != GLATT)
		{
			probanschlEnde->initStammdaten(dnProbAnschl, dnProbAnschl, probAnschl);
			probanschlEnde->set_abstAbs(abstandProbAnschl);
			probanschlEnde->set_abstRel(0);
		}

		if (reduziert)
		{
			reduzierung->initStammdaten(dnSprAnlAnschl, dnPumpDruckSt, KRED);
			reduzierung->set_laenge((int)reduzierung->get_mass1());
			reduzierung->set_abstAbs((int)(2.5 * (double)dnPumpDruckSt));
		}
	}
	catch (Element::Error e)
	{
		getErrorHandler().appendError(MxError(e.get_errorText(), MxError::Information));
	}
	
	if (schwMuffe)
	{
		if (muffe->get_abstAbs() < rohrnippel->get_abstAbs())
		{
			elementArray.Add(muffe);
			elementArray.Add(rohrnippel);
		}
		else
		{
			elementArray.Add(rohrnippel);
			elementArray.Add(muffe);
		}
	}
	else
	{
		delete muffe;
		elementArray.Add(rohrnippel);
	}

	if (reduziert)
		elementArray.Add(reduzierung);
	else
		delete reduzierung;
	elementArray.Add(probieranschluss);
	if (probAnschl != GLATT)
		elementArray.Add(probanschlEnde);
	else 
		delete probanschlEnde;
	
	// Rel. Abstaende neu berechnen
	elementArray.calcRelAbst();
}

void Pumpenverteiler::create_stueckliste()
{
	Rohrleitung::create_stueckliste();

	// Muffe DN 15 mit Rohrnippel tauschen
	for (POSITION pos = stueckliste.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(stueckliste.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			return;

		if (ele->get_id() == MUFFE && ele->get_dn() == 15)
		{
			ele->set_id(ROHRNIPPEL);
			ele->initStammdatenSoma(ele->get_dn(), ele->get_anschlussDN(), ele->get_id());
		}
	}
}

///Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
CString Pumpenverteiler::checkRohrtrennstelle()
{
	// hier nicht, Elemente zu dicht zusammen ok, Sonderkonstruktion  
	return "";
}

const int Pumpenverteiler::get_abstandProbAnschl() const
{
	return abstandProbAnschl;
}

void Pumpenverteiler::set_abstandProbAnschl(int value)
{
	abstandProbAnschl = value;
}

const int Pumpenverteiler::get_abstandSchwMuffe() const
{
	return abstandSchwMuffe;
}

void Pumpenverteiler::set_abstandSchwMuffe(int value)
{
	abstandSchwMuffe = value;
}

const int Pumpenverteiler::get_dnProbAnschl() const
{
	return dnProbAnschl;
}

void Pumpenverteiler::set_dnProbAnschl(int value)
{
	dnProbAnschl = value;
}

const int Pumpenverteiler::get_dnPumpDruckSt() const
{
	return dnPumpDruckSt;
}

void Pumpenverteiler::set_dnPumpDruckSt(int value)
{
	dnPumpDruckSt = value;
}

const int Pumpenverteiler::get_dnSprAnlAnschl() const
{
	return dnSprAnlAnschl;
}

void Pumpenverteiler::set_dnSprAnlAnschl(int value)
{
	dnSprAnlAnschl = value;
}

const int Pumpenverteiler::get_gesamthoehe() const
{
	return gesamthoehe;
}

void Pumpenverteiler::set_gesamthoehe(int value)
{
	gesamthoehe = value;
}

const int Pumpenverteiler::get_laengeProbAnschl() const
{
	return laengeProbAnschl;
}

void Pumpenverteiler::set_laengeProbAnschl(int value)
{
	laengeProbAnschl = value;
}

const int Pumpenverteiler::get_probAnschl() const
{
	return probAnschl;
}

void Pumpenverteiler::set_probAnschl(int value)
{
	probAnschl = value;
}

const bool Pumpenverteiler::get_reduziert() const
{
	return reduziert;
}

void Pumpenverteiler::set_reduziert(bool value)
{
	reduziert = value;
}

const bool Pumpenverteiler::get_schwMuffe() const
{
	return schwMuffe;
}

void Pumpenverteiler::set_schwMuffe(bool value)
{
	schwMuffe = value;
}

const int Pumpenverteiler::get_sprAnlAnschl() const
{
	return sprAnlAnschl;
}

void Pumpenverteiler::set_sprAnlAnschl(int value)
{
	sprAnlAnschl = value;
}

const int Pumpenverteiler::get_winkelRohnippel() const
{
	return winkelRohnippel;
}

void Pumpenverteiler::set_winkelRohnippel(int value)
{
	winkelRohnippel = value;
}

const int Pumpenverteiler::get_winkelSchwMuffe() const
{
	return winkelSchwMuffe;
}

void Pumpenverteiler::set_winkelSchwMuffe(int value)
{
	winkelSchwMuffe = value;
}
