#include "stdafx.h"
#include "resource.h"


#include "Preis.h"
#include "Werkauftrag.h"
#include "Material.h"
#include "Arbeitsplan.h"
#include "Arbeitsfolge.h"
#include "Rohrleitung.h"
#include "Element.h"
#include "Gewinderohr.h"
#include "Verteiler.h"
#include "Strangrohr.h"
#include "Handelslaenge.h"
#include "Stationsverteiler.h" 
#include "SammelWerkauftrag.h"
#include "zinkkosten.h"




#include <mxutil/ddx_extension.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


// Sollten die Gemeinkosten und der Aufschlag fuer jedes Element unterschiedlich sein, 
// so ist fuer jedes Element die Faktoren aus den Stammdaten zu holen
// und mit dem Wap zu verrechnen. 
// Es ist zu ueberlegen, ob Material noch die Attribute "preisGemeinkosten"
// und "preisAufschlag" bekommt, und ob diese schon den Additionspreis 
// (inkl. multipliziert mit Anzahl?) oder nur den Faktor als Preis enthalten
// (in diesem Fall sollte der Faktor in der Tabelle 0.16 statt 1.16 sein).
// Sollen alle Berechnungen hier und nicht teilweise auch in der Klasse Preisausgabe 
// von waxprj durchgefuehrt werden, so ist dies erforderlich.  
// Analog dazu das gleiche mit dem Aufschlag bei Arbeitsfolgen.

// Das Material wird je nach Art anders kalkuliert. Bei Halbmaterial wird 
// auf die HerstellKosten (HK), im SAP der S-Preis, nur der allgemeine Aufschlag 
// (zur Zeit 10%) aufgerechnet. Bei Rohmaterial und Hilfs- sowie Betriebsstoffen
// werden auf den gleitenden Durchschnittspreis aus SAP (GLD), in SAP der V-Preis,
// nur die Materialgemeinkosten (zur Zeit 16,4%) addiert. Der Preis, der beim 
// jeweiligen Material nicht vorkommt, muss dort auf Null gesetzt werden!

//Standardkonstruktor
Preis::Preis(bool vk3)
{
	sonderkosten = 0;
	gesamtpreisVK3 = 0;
	vk3flag = vk3;
	nrVerzinkerei = DEFAULT_VERZINKEREI;
}	


//Destruktor
Preis::~Preis()
{
	delete_sokolist(true);
}

void Preis::copy(const Preis& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	vk3berechnung.copy(other.get_vk3berechnung());

	sonderkosten = other.sonderkosten;

	delete_sokolist(true);
	const CObList& solist = other.get_sokolist();
	for (POSITION pos = solist.GetHeadPosition(); pos != NULL;)	
	{
		const Sonderkosten *sonderkosten = dynamic_cast<const Sonderkosten*>(solist.GetNext(pos));
		XASSERT(sonderkosten);
		if (sonderkosten)
		{
			Sonderkosten* soko = new Sonderkosten;
			soko->copy(*sonderkosten);
			sokolist.AddTail(soko);
		}
	}
}

//Speichern und Laden der Preisdaten
void Preis::serialize(BinaryArchive & ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	double ddummy = 0;
	int idummy = 0;

	if (ar.is_storing())
	{
		ar << sonderkosten; 
	}
	else
	{
		ar >> sonderkosten;
		
		//gesamtpreis; 
		if (version < 46)
			ar >> ddummy;
		
		if (version > 19 && version < 37)
			ar >> ddummy;
		if (version > 33 && version < 37)
			ar >> idummy;
	}
	serialize_soko(ar);

	if (!ar.is_storing() && version < 46)
	{
		//preisarbeit.serialize(ar);
		ar >> ddummy >> ddummy >> ddummy >> ddummy;
		
		//preismaterial.serialize(ar);
		ar >> ddummy >> ddummy >> ddummy;

		//mat.serialize(ar);
		ar >> ddummy >> ddummy >> ddummy;

		//stk.serialize(ar);
		ar >> ddummy >> ddummy >> ddummy;		
	}


	if (!ar.is_storing())
	{
		if (version > 19 && version < 37)
			serialize_bonus(ar);
	}
	vk3berechnung.serialize(ar);
}

//Lädt und speichert die Liste mit Sonderkosten
void Preis::serialize_soko(BinaryArchive& ar)
{
	if (ar.is_storing())
	{
		for (POSITION pos = sokolist.GetHeadPosition(); pos;)
		{
			Preis::Sonderkosten* so = (Preis::Sonderkosten*)sokolist.GetNext(pos);
			ar << (int)1;
			so->serialize(ar);
		}
		ar << (int)0;
	}
	else
	{
		delete_sokolist(true);
		int dummy;
		ar >> dummy;
		while (dummy)
		{
			Preis::Sonderkosten* so = new Preis::Sonderkosten;
			so->serialize(ar); 
			sokolist.AddTail(so);
			ar >> dummy;
		}
	}
}

double Preis::getValueGerundet(double value)
{
	double valueGerundet = 0.0;
	CString str;
	str.Format("%.2f", value);
	valueGerundet = atof(str);
	return (valueGerundet);
}

//Loeschen der Liste mit Sonderkosten
void Preis::delete_sokolist(bool deleteFixedSoko)
{
	CObList tmpSokolist;

	// Loescht die Objekte und die Pointer aus der Sokolist
	for (POSITION pos = sokolist.GetHeadPosition(); pos != NULL;)
	{
		Preis::Sonderkosten *soko = (Preis::Sonderkosten*)(sokolist.GetNext(pos));
		if (!soko)
			continue;

		if (!deleteFixedSoko && soko->get_isfix())
			tmpSokolist.AddTail(soko);
		else
			delete soko;
	}
	sokolist.RemoveAll();

	for (POSITION pos = tmpSokolist.GetHeadPosition(); pos != NULL;)
	{
		sokolist.AddTail(tmpSokolist.GetNext(pos));
	}
}

//Werkauftrag setzen
void Preis::set_werkauftrag(Werkauftrag* value)
{
	werkauftrag = value;

	vk3berechnung.set_werkauftrag(value);

}

//alle Preise berechnen
void Preis::calc_all()
{		
	// VK-Preise
	calc_gesamtpreisVK3();
}



void Preis::calc_gesamtpreisVK3()
{
	vk3berechnung.calc_preis();

	calc_sonderkosten();

	gesamtpreisVK3 = vk3berechnung.get_preis() + sonderkosten;
}

//Berechnung
void Preis::calc_sonderkosten()
{
	
	sonderkosten = 0;
	
	Gewinderohr* pGewinderohr = dynamic_cast<Gewinderohr*>(werkauftrag);
	if(pGewinderohr)
	{
		double dPreis = vk3berechnung.get_preis();
		if(pGewinderohr->get_beschichtung().isVerzinkt() && (pGewinderohr->get_typ() == Rohrleitung::WASSER) && !pGewinderohr->get_sichtbereich())
		{
			CString ZwanzigProzentRabatt = getStammdaten().getText("IDS_GEWINDE_ZWANZIG_PROZENT").c_str();
			int iCounter = 0;
			for (POSITION pos = sokolist.GetHeadPosition();pos;)
			{			
				Sonderkosten* pSonderkost = ((Sonderkosten*)sokolist.GetNext(pos));
				if(pSonderkost->get_text().Compare(ZwanzigProzentRabatt) == 0)
				{
					sokolist.RemoveAt(sokolist.FindIndex(iCounter));
				}
				iCounter++;
			}

			Sonderkosten* pSoko = new Sonderkosten();
			pSoko->set_text(ZwanzigProzentRabatt);
			pSoko->set_preis(-dPreis * 0.20);
			sokolist.AddTail(pSoko);
			
		}
	}
	for (POSITION pos = sokolist.GetHeadPosition(); pos;)
	{
		Preis::Sonderkosten* so = (Preis::Sonderkosten*)sokolist.GetNext(pos);
		XASSERT(so);
		if (!so)
			break;

		sonderkosten += getValueGerundet(so->get_preis());
	}
	
}


//get sonderkosten
double Preis::get_sonderkosten()
{
	return sonderkosten;
}


//set sonderkosten
void Preis::set_sonderkosten(double value)
{
	sonderkosten = value;
}

//Liste mit Sonderkostenobjekten
CObList&  Preis::get_sokolist()
{
	return sokolist;
}

const CObList&  Preis::get_sokolist() const
{
	return sokolist;
}

//Automatische Erzeugung einiger Sonderkosten
void Preis::calcAutoSoko(bool deleteFixedSoko)
{
	sokonr = 0;
	// Erstmal Liste loeschen
	delete_sokolist(deleteFixedSoko);
	
	
	COIIVerteiler* rohr = dynamic_cast<COIIVerteiler*>(werkauftrag);
	if(rohr)
	{
		return;
	}
	
	
	// Soko fuers Verzinken
	calcSokoVerzinken();

	// Soko fuer Sonderfarbe
	calcSokoSonderfarbe();

	// Soko fuer Rohrstutzen pulvern
	calcSokoRohrstutzen();

}


//Soko fuers Verzinken
void Preis::calcSokoVerzinken()
{
	/**
	Verzinkungskosten werden als Soko eingefügt
	QA: Da bei der neuen Verzinkungsberechnung die Elementliste 
	benötigt wird, muss vor dem Berechnen makerohrleitung aufgerufen
	werden. Es sollte aber sichergestellt sein, dass dies schon vorher 
	passiert.
	*/
	if (!werkauftrag->get_beschichtung().isVerzinkt())
		return;

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(werkauftrag);
	
	// Gewinderohre sind verzinkt oder bei Verteiler extra in AV ausgewaehlt
	if (!rohr || grohr || rohr->get_rohrQuali() == Rohrleitung::RQ2440VZIN ||rohr->get_rohrQuali() == Rohrleitung::RQEN10217VERZ)
		return;

	rohr->createModell();

	sokonr++;
	Preis::Sonderkosten *so = new Preis::Sonderkosten;
	ZinkKosten z(getStammdaten().get_verzinkung().query(rohr->getBeschichtung().get_verzinkerei()), *rohr);
	so->set_preis(z.GesamtKosten());

	CString txt;
	txt = getStammdaten().getText("IDS_SOKO2").c_str(); 

	so->set_text(txt + (CString)" : " + (CString)z.get_name().c_str());
	so->set_nr(sokonr);

	sokolist.AddTail(so); 
}



//Soko fuer Rohrstutzen pulvern
void Preis::calcSokoRohrstutzen()
{
	int vk3StutzenPulvern150 = 1002;
	int vk3StutzenPulvern300 = 1003;

	// kann eigentlich entfallen, da die Abganglänge in der Zeitentabelle berücksichtigt ist 
	if (werkauftrag->get_beschichtung().isGrundiert() || werkauftrag->get_beschichtung().isVerzinkt())
		return;
	
	Rohrleitung *rohr = dynamic_cast<Rohrleitung *>(werkauftrag);
	if (!rohr)
		return;	
	int maxlaenge = 0;
	for (POSITION epos = rohr->get_elementList().GetHeadPosition(); epos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(rohr->get_elementList().GetNext(epos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr())
		{
			for (POSITION pos = ele->get_uelementList().GetHeadPosition(); pos != NULL;)	
			{
				Element *uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(pos));
				XASSERT(uele);
				if (!uele)
					break;

				if (uele->get_id() == ROHR)
				{
					if (maxlaenge < uele->get_laenge())
						maxlaenge = uele->get_laenge();
				}
			}
		}
	}
	if (maxlaenge > 150)
	{
		sokonr++;
		Preis::Sonderkosten *so = new Preis::Sonderkosten;
		string txt; //txt = getStammdaten().getText("IDS_SOKO4); 

		txt = getStammdaten().getText("IDS_SOKO4");


		const VK3Preise& vk3 = getStammdaten().get_vk3preise();
		const VK3Preise::data *v;

		so->set_text(txt.c_str());
		so->set_nr(sokonr);
		if (maxlaenge > 300)
			v = vk3.query(Werkauftrag::OLDESLOE, vk3StutzenPulvern300, 0, 0, 0, 0, 0, 0, 0, 0);			
		else 
			v = vk3.query(Werkauftrag::OLDESLOE, vk3StutzenPulvern150, 0, 0, 0, 0, 0, 0, 0, 0);
			
		so->set_preis(rohr->get_anzahl() * v->get_preis()); 
		sokolist.AddTail(so); 
	}
}

//Soko fuer Sonderfarbe
void Preis::calcSokoSonderfarbe()
{
	// Sonderfarbenkosten betragen die ermittelte Arbeitszeit*1/10 des Stundenlohns 
	int nr = 0;
	Arbeitsfolge *afo27 = NULL;
	if (werkauftrag->get_beschichtung().get_ral() <= 0)
		return;
	if (werkauftrag->get_beschichtung().isSonderfarbe()) 
	{
		CObList& arbeitsfolgen = werkauftrag->get_arbeit().get_arbeitsfolgen();
		for (POSITION pos = arbeitsfolgen.GetHeadPosition(); pos;)
		{
			afo27 = dynamic_cast<Arbeitsfolge *>(arbeitsfolgen.GetNext(pos));
			XASSERT(afo27);
			if (!afo27)
				continue;
			if (afo27->get_id() == 27)
				break;
			else
				afo27 = NULL;
		}
		
		if (afo27)
		{
			nr++;
			Preis::Sonderkosten *so = new Preis::Sonderkosten;
			CString txt, str;
			str = getStammdaten().getText("IDS_SOKO5").c_str();
			txt.Format(str, werkauftrag->get_beschichtung().get_ral()); 
			so->set_text(txt);
			so->set_nr(nr);
			so->set_preis(afo27->get_ppstd()/10.0 * afo27->get_zeit()); 
			sokolist.AddTail(so); 
		}
	}
}



//Erzeugt bei Änderungen die Rohrliste neu, erstellt die Afos und 
//Sonderkosten
void Preis::updatePreis()
{
	if (!WaxSys::isAV()) 
	{
		werkauftrag->createAutoArbeit();
		calcAutoSoko(true);
	}	
	calc_all();
}

void Preis::set_sokonr(int value)
{
	sokonr = value;
}

VK3Berechnung& Preis::get_vk3berechnung() 
{
	return vk3berechnung;
}


double Preis::get_gesamtpreisVK3() 
{
	if (!gesamtpreisVK3)
		calc_gesamtpreisVK3();
	return gesamtpreisVK3;
}

void Preis::set_gesamtpreisVK3(double value)
{
	gesamtpreisVK3 = value;
}

//Destruktor
Preis::Sonderkosten::~Sonderkosten()
{

}

//Standardkonstruktor
Preis::Sonderkosten::Sonderkosten()
{
	preis = 0.0;
	text = "";
	nr = 0;  
	isfix = 0;
}


//Standardkonstruktor
Preis::Sonderkosten::Sonderkosten(const Preis::Sonderkosten &other)
{
	copy(other);
}

//Kopieren der Sokoattribute
void Preis::Sonderkosten::copy(const Sonderkosten& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	nr		= other.nr;
	text	= other.text;
	preis	= other.preis;
	isfix   = other.isfix;
}

//Speichern und Laden der Sonderkostendaten
void Preis::Sonderkosten::serialize(BinaryArchive & ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	if (ar.is_storing())
	{
		ar << preis << text << nr << isfix; 
	}
	else
	{
		ar >> preis >> text >> nr; 
		
		if (version >= 53)
			ar >> isfix;
	}
}

bool Preis::Sonderkosten::get_isfix() const
{
	return isfix;
}
		
void Preis::Sonderkosten::set_isfix(bool value)
{
	isfix = value;
}

//get preis
double Preis::Sonderkosten::get_preis() const
{
	return preis;
}

//set preis
void Preis::Sonderkosten::set_preis(double value)
{
	preis = value;
}

//get text
const CString& Preis::Sonderkosten::get_text() const
{
	return text;
}

//set text
void Preis::Sonderkosten::set_text(const CString& value)
{
	text = value;
}

//get nr
const int Preis::Sonderkosten::get_nr() const
{
	return nr;
}

//set nr
void Preis::Sonderkosten::set_nr(int value)
{
	nr = value;
}


void Preis::serialize_bonus(BinaryArchive& ar)
{
	CString kommentar;
	int tage = 0;
	double prozentwertSoll = 0.0;
	double prozentwertIst = 0.0;
	bool takeSoll = false;
	double geldwert = 0.0;

	serialize_bool(takeSoll, ar);
	if (ar.is_storing())
	{
		ar << geldwert << kommentar << prozentwertIst << prozentwertSoll << tage; 
	}
	else
	{
		ar >> geldwert >> kommentar >> prozentwertIst >> prozentwertSoll >> tage; 
	}
}


bool Preis::get_vk3flag() const
{
	return vk3flag;
}

void Preis::set_vk3flag(bool vk3berechnung)
{
	vk3flag = vk3berechnung;
}
