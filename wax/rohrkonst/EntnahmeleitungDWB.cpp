#include "stdafx.h"
#include "resource.h"

#include "EntnahmeleitungDWB.h"
#include "Element.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//Destruktor
EntnahmeleitungDWB::~EntnahmeleitungDWB()
{

}

//Konstruktor
EntnahmeleitungDWB::EntnahmeleitungDWB() : entleerungRechts(true), entleerung(LEER), laenge2(MINLAENGE_DN100_L2_TYPB), entnahmeleitung_typ(TYP_B)
{
	set_zeugnis(true);
	set_anzahl(1);
	set_dnvl(100);
	set_rohrQuali(RQ2458DICK);
	set_entnahmeleitung_typ(TYP_A);
	set_laenge1(500);
	set_laenge2(500);

	CString str; 
	str = getStammdaten().getText("IDS_STDTEXT_ENTNAHME1").c_str();
	str += getStammdaten().getText("IDS_STDTEXT_ENTNAHME2").c_str();
	str += getStammdaten().getText("IDS_STDTEXT_ENTNAHME3").c_str();
	set_stdbemerkung(str);
	
	get_leitkarte().set_sdMaterialnummer(400035);
	stdbezeichnung = getStammdaten().getText("IDS_ENTNAHME").c_str();
	get_leitkarte().set_vk3(false);
}

//Speichern und Laden der Rohrleitung. Achtung:
void EntnahmeleitungDWB::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	Werkauftrag::serialize(ar);

	if (ar.is_storing())
	{
		ar << get_dnvl();
		ar << get_laenge();
		ar << laenge2;
		ar << entnahmeleitung_typ;
		ar << entleerung;
		ar << laenge1;
	}
	else
	{
		int tmp;
		ar >> tmp; set_dnvl(tmp);
		ar >> tmp; set_laenge(tmp);
		ar >> laenge2;
		ar >> entnahmeleitung_typ;
		ar >> entleerung;
		if (version > 14)
			ar >> laenge1;
	}
	serialize_bool(entleerungRechts, ar);
}

//Hier wird automatisch der Arbeitsplan generiert
void EntnahmeleitungDWB::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	Verteiler::createAutoArbeit();

	///Umstempeln
	Arbeitsfolge *umstempeln = new Arbeitsfolge(1135);
	umstempeln->set_anz(get_anzahl());
	get_arbeit().addAfo(umstempeln);

	///Werkstoffliste
	Arbeitsfolge *werkstoffliste = new Arbeitsfolge(1161);
	werkstoffliste->set_anz(get_anzahl());
	werkstoffliste->set_nr(80); // Vor Vorrichten
	get_arbeit().addAfo( werkstoffliste);
		
	///Druckprobe
	Arbeitsfolge *druckprobe = new Arbeitsfolge(1075);
	druckprobe->set_anz(get_anzahl());
	druckprobe->set_nr(100); // Nach Schweissen aber vor Pulvern
	get_arbeit().addAfo(druckprobe);
	
	get_arbeit().sortAfos(get_arbeit().get_arbeitsfolgen());

	///Afo16/17 Zeitlohn mit Zuschlag 
	for (POSITION pos = get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge* afo = dynamic_cast<Arbeitsfolge*>(get_arbeit().get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if (!afo)
			return;

		///Zeitfaktoren aus SOMZ-Programm uebernommen
		if (afo->get_id() == 16)
		{
			if (get_entnahmeleitung_typ() == TYP_A)
				afo->set_zeit((1.5-afo->get_ruestzeit())*get_anzahl());
			else
				afo->set_zeit((2.75-afo->get_ruestzeit())*get_anzahl());
			///Sondertext
			afo->appendText("IDS_ZEITLOHN");
		}
		else if (afo->get_id() == 17)
		{
			if (get_entnahmeleitung_typ() == TYP_A)
				afo->set_zeit((3.5-afo->get_ruestzeit())*get_anzahl());
			else
				afo->set_zeit((5.0-afo->get_ruestzeit())*get_anzahl());

			///Sondertext
			afo->appendText("IDS_SCHWEISSGUETE");
			afo->appendText("IDS_ZEITLOHN");
		}
		else if (afo->get_id() == 27)
		{
			afo->appendText("IDS_FLANSCH_NICHT_PULVERN");
		}
	}

}

//Rohrkonstruktion erzeugen
void EntnahmeleitungDWB::createModell()
{
	Rohrleitung::createModell();

	// Löschen aller Elemente aus der Liste
	elementList.deleteAllElemente();
	
	fill_elementArray();

	set_rohrAnfang(get_entnahmeleitung_typ() == TYP_A ? FFL : VFL16);
	set_rohrEnde(get_entnahmeleitung_typ() == TYP_B ? FFL : VFL16);
	
	if (get_laenge() > 0) 
	{
		// Laengen für Anfang/Ende/Verb. setzen 
		setAnfaEndeVerbLaenge();
	
		// Berechnen der Rohrstuecke
		CString err = setAutoRohrlaengen();
		if (err.GetLength())
			return;

		// Elementliste fuer Rohrkonstruktion generieren
		if (!makeRohre(elementList))
			return; // Schwerer Fehler
	
		// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen 
		rohreBeschriften(elementList);

		// Bei Rohrstuecken setzen ob mit od. ohne Rohrstutzen (fuers Pulvern)
		setRohrMitStut();
		calc_laengeProNr();		
	}
	delete_stueckliste();
	create_stueckliste();
}

//Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
//Stationsverteiler oder Entnahmeleitungen
void EntnahmeleitungDWB::fill_elementArray()
{
	Element *ele  = NULL;
	Element *ele2 = NULL;

	try
	{
		elementArray.deleteAll();
		ele2 = new Element;
		ele2->initStammdaten(get_dnvl(), get_dnvl(), ELEMENT_BOGEN);

		Element flachflansch, vflansch;
		flachflansch.initStammdaten(get_dnvl(), get_dnvl(), FFL);
		vflansch.initStammdaten(get_dnvl(), get_dnvl(), VFL16);
		int vflaenge = (int)vflansch.get_mass1();
		set_laenge(1000);
		
		if (get_entnahmeleitung_typ() == TYP_A)
		{
			// Gesamtlänge ist L1 + Länge Bogen + Flanschlänge am Ende
			set_laenge((int)(laenge1 + ele2->get_mass1() + vflaenge));

			ele2->set_abstAbs((int)(get_laenge() - ele2->get_mass1() - vflaenge));
			ele2->set_abstRel(ele2->get_abstAbs());
			ele2->set_winkel(0);
			ele2->set_laenge((int)ele2->get_mass1());
		}
		else
		{	
			// Typ B zwei Bogen, erster Bogen am Anfang
			ele = new Element; 
			ele->initStammdaten(get_dnvl(), get_dnvl(), ELEMENT_BOGEN);
			ele->set_abstAbs((int)(ele->get_mass1()+vflaenge));
			ele->set_abstRel(ele->get_abstAbs());
			ele->set_winkel(0);
			ele->set_laenge((int)ele->get_mass1());
			elementArray.Add(ele);

			// Gesamtlänge ist L1 + L2 + Länge Bogen1
			set_laenge((int)(laenge1 + laenge2 + ele2->get_mass1() + vflaenge));
		

			// Zweiter Bogen bei L1
			ele2->set_abstAbs(get_laenge() - laenge2);
			ele2->set_abstRel(ele2->get_abstAbs());
			ele2->set_winkel(0);
			ele2->set_laenge((int)ele2->get_mass1());
		}

		// Unterelemente für die Entleerung hinzufügen
		if (get_entleerung() != LEER)
		{
			Element *entleer = new Element;
			entleer->initStammdaten(50, 50, get_entleerung());

			entleer->set_laenge(get_entleerung() == ROHR ? 180 : (int)entleer->get_mass1());
			entleer->set_winkel(get_entleerungRechts() ? -90 : -180);
			entleer->set_abstAbs(0);
			entleer->set_abstRel(0);
			if (get_entleerungRechts() || get_entnahmeleitung_typ() == TYP_A)
				ele2->get_uelementList().AddTail(entleer);
			else
				ele->get_uelementList().AddTail(entleer);
		}
		
		elementArray.Add(ele2);

		// Rel. Abstaende neu berechnen
		elementArray.calcRelAbst();
	}
	catch (Element::Error e)
	{
		delete ele;
		delete ele2;
	}
}

//Rohrkonstruktion auf korrekte Werte checken
CString EntnahmeleitungDWB::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err;

	int minlaenge = 0;
	CString l;
	
	int checklen1 = 0, checklen2 = 0;
	int gesamtlen = 0;

	// Zu prüfende Minimallängen festlegen
	if (entnahmeleitung_typ == TYP_A)
	{
		checklen1 = get_dnvl() == 100 ? MINLAENGE_DN100_L1_TYPA : MINLAENGE_DN150_L1_TYPA;
		gesamtlen = laenge1;
	}
	else if (entnahmeleitung_typ == TYP_B)
	{
		checklen1 = get_dnvl() == 100 ? MINLAENGE_DN100_L1_TYPB : MINLAENGE_DN150_L1_TYPB;
		checklen2 = get_dnvl() == 100 ? MINLAENGE_DN100_L2_TYPB : MINLAENGE_DN150_L2_TYPB;
		gesamtlen = laenge1 + laenge2;
	}
	else 
		XASSERT(FALSE);
	
	// Gesamtlänge prüfen
	if (gesamtlen > MAXLAENGE)
	{
		CString txt; txt = getStammdaten().getText("IDS_ERROR1a").c_str();
		err.Format(txt, MAXLAENGE);
			return err;
	}
	else
	{
		// Einzellängen prüfen
		if (laenge1 < checklen1)
		{
			minlaenge = checklen1;
			l = getStammdaten().getText("IDS_LAENGE1a").c_str();
			CString txt; txt = getStammdaten().getText("IDS_ERROR26").c_str();
			err.Format(txt, l, minlaenge);
			return err;
		}
		else if (checklen2 && laenge2 < checklen2)
		{
			minlaenge = checklen2;
			l = getStammdaten().getText("IDS_LAENGE2").c_str();
			CString txt; txt = getStammdaten().getText("IDS_ERROR26").c_str();
			err.Format(txt, l, minlaenge);
			return err;
		}
	}
	
	return Rohrleitung::checkPlausis();
}

const int EntnahmeleitungDWB::get_entleerung() const
{
	return entleerung;
}

void EntnahmeleitungDWB::set_entleerung(int value)
{
	entleerung = value;
}

const bool EntnahmeleitungDWB::get_entleerungRechts() const
{
	return entleerungRechts;
}

void EntnahmeleitungDWB::set_entleerungRechts(bool value)
{
	entleerungRechts = value;
}

const int EntnahmeleitungDWB::get_laenge2() const
{
	return laenge2;
}

void EntnahmeleitungDWB::set_laenge2(int value)
{
	laenge2 = value;
}

const int EntnahmeleitungDWB::get_laenge1() const
{
	return laenge1;
}

void EntnahmeleitungDWB::set_laenge1(int value)
{
	laenge1 = value;
}

int EntnahmeleitungDWB::get_entnahmeleitung_typ() const
{
	return entnahmeleitung_typ;
}

void EntnahmeleitungDWB::set_entnahmeleitung_typ(int value)
{
	entnahmeleitung_typ = value;
}

void EntnahmeleitungDWB::create_stueckliste()
{
	Rohrleitung::create_stueckliste();

	// Dickwandiges Rohr und Bogen
	// Spezielle Muffe und Rohrstutzen DN50 ist Rohrnippel
	for (POSITION pos = stueckliste.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(stueckliste.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			return;

		if (ele->get_id() == BOGVERZ)
			ele->set_id(ELEMENT_BOGEN);

		if (ele->get_id() == ELEMENT_BOGEN || ele->get_id() == ROHR_Q8 || ele->get_id() == MUFFE)
		{
			if (ele->get_id() == ROHR_Q8 && ele->get_dn() == 50)
			{
				ele->set_id(ROHRNIPPEL);
				ele->set_anzahl(get_anzahl());
			}
			ele->initStammdatenSoma(ele->get_dn(), ele->get_anschlussDN(), ele->get_id());
		}
	}
}
