#include "stdafx.h"
#include "resource.h"



#include "Element.h"
#include "Verteiler.h"
#include "Strangrohr.h"
#include "Gewinderohr.h"
#include "resource.h"
#include "..\WaxDef.h"
#include "typeinfo.h"
#include "Arbeitsfolge.h"
#include "GpConvertOldVersion.h"
#include "ZinkKosten.h"


	#include "Rohrleitung.h"


using namespace boost;
using namespace MxUtil;
using namespace MxErrorHandling;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 



///Standardkonstruktor
Rohrleitung::Rohrleitung() : avnachbearbeitung_per_zeichnung(false)
{
	dnvl        = 50;
	laenge      = 6000;
	set_posNr("1");
	nTeilRohre  = 0;
	
	reset_rohrlaengen_und_ids();
	schnittlaenge  = 6000;

	rohrKFarbe     = 0;
	rohrDruckstufe = 0;
	rohrQuali	   = DEFAULT_ROHRQUALI; // = RQ2458GSCH
	rohrVerb	   = SNAHT;
	rohrAnfang	   = NUT;
	rohrEnde	   = NUT;
	rohrEndeLaenge = 0;
	rohrAnfaLaenge = 0;
	rohrVerbLaenge = 0;
	get_leitkarte().set_sdMaterialnummer(400010);
	fertigungshinweis = "";	
	set_trockenanlage(false);
	set_Ueberlaenge(false);
	set_IsApaxBauteil(false);
	pi = 3.141592653;
	bSprinklerschellen = false;
}



///Destruktor
Rohrleitung::~Rohrleitung()
{
	elementArray.deleteAll();
	elementArraySap.deleteAll();
	avelementArray.deleteAll();
	stdelementArray.deleteAll();

	elementList.deleteAllElemente();
	elementListSap.deleteAllElemente();
	delete_stuecklisteSap();

	delete_stueckliste();

	passlaengenListe.deleteAllElemente();
	pulverlaengenListe.deleteAllElemente();
	vk3elementeListe.deleteAllElemente();
}

///Kopierfunktion fuer alle Attribute
///Achtung: UpToDate halten!
void Rohrleitung::copy(const Rohrleitung& other)
{
	Werkauftrag::copy(other);

	dnvl			= other.dnvl;			
	laenge			= other.laenge;			
	rohrKFarbe		= other.rohrKFarbe;		
	rohrDruckstufe	= other.rohrDruckstufe;	
	rohrQuali		= other.rohrQuali;		
	rohrVerb		= other.rohrVerb;		
	rohrAnfang		= other.rohrAnfang;		
	rohrEnde		= other.rohrEnde;	
	schnittlaenge   = other.schnittlaenge;
	trockenanlage   = other.trockenanlage;
	m_bUeberlaenge  = other.m_bUeberlaenge;
	m_IsApaxBauteil = other.m_IsApaxBauteil;
	bSprinklerschellen = other.bSprinklerschellen;

	avnachbearbeitung_per_zeichnung = other.avnachbearbeitung_per_zeichnung;
	fertigungshinweis = other.fertigungshinweis;

	// Elementarray kopieren
	elementArray.copy(other.get_elementArray());
	avelementArray.copy(other.get_avelementArray());
	stueckliste.copy(other.get_stueckliste());
	
	passlaengenListe.deleteAllElemente();
	
	///VK3Passlaenge kopieren
	const CObList& passlist = other.get_passlaengenListe();
	for (POSITION pos = passlist.GetHeadPosition(); pos != NULL;)	
	{
		const Element* const element = dynamic_cast<const Element* const>(passlist.GetNext(pos));
		XASSERT(element);
		if (element)
		{
			Element* ele = new Element;
			ele->copy(*element);
			passlaengenListe.AddTail(ele);
		}
	}

	pulverlaengenListe.deleteAllElemente();

	///VK3Pulvern kopieren

	const CObList& pulverlist = other.get_pulverlaengenListe();
	for (POSITION pos = pulverlist.GetHeadPosition(); pos != NULL;)	
	{
		const Element* const element = dynamic_cast<const Element* const>(pulverlist.GetNext(pos));
		XASSERT(element);
		if (element)
		{
			Element* ele = new Element;
			ele->copy(*element);
			pulverlaengenListe.AddTail(ele);
		}
	}
}

///Speichern und Laden der Rohrleitung. 
///Achtung:UpToDate halten!
void Rohrleitung::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	
	serialize_bool_if(version > 18, avnachbearbeitung_per_zeichnung, ar);

	if (!ar.is_storing() && version == 32)
	{
		bool verbAnfang, verbEnde; // gibs nicht mehr 
		serialize_bool(verbAnfang, ar);
		serialize_bool(verbEnde, ar);
	}
	
	Werkauftrag::serialize(ar);

	if (ar.is_storing())
	{
		ar << dnvl << laenge << nTeilRohre << rohrQuali << rohrVerb;
		ar << rohrAnfang << rohrEnde << rohrDruckstufe << rohrKFarbe;
		ar << fertigungshinweis << schnittlaenge;
	}	
	else
	{
		ar >> dnvl >> laenge >> nTeilRohre >> rohrQuali >> rohrVerb;
		ar >> rohrAnfang >> rohrEnde >> rohrDruckstufe >> rohrKFarbe;
		initDaten(get_anzahl(), dnvl, laenge, get_posNr(),  rohrQuali, rohrVerb);

		if (version > 19)
			ar >> fertigungshinweis;

		if (version > 39)
			ar >> schnittlaenge;

		if (version > 17 && version < 31)
		{
			delete_stueckliste();
			int dummy;
			ar >> dummy;
			while (dummy)
			{
				Element *ele = new Element;
				ele->serialize(ar);
				stueckliste.add(ele);
				ar >> dummy;
			}
		}
		if (version > 23 && version < 31)
		{
			vk3elementeListe.deleteAllElemente();
			int dummy;
			ar >> dummy;
			while (dummy)
			{
				Element *ele = new Element;
				ele->serialize(ar);
				vk3elementeListe.AddTail(ele);
				ar >> dummy;
			}
			
			passlaengenListe.deleteAllElemente();
			ar >> dummy;
			while (dummy)
			{
				Element *ele = new Element;
				ele->serialize(ar);
				passlaengenListe.AddTail(ele);
				ar >> dummy;
			}
			
			pulverlaengenListe.deleteAllElemente();
			ar >> dummy;
			while (dummy)
			{
				Element *ele = new Element;
				ele->serialize(ar);
				pulverlaengenListe.AddTail(ele);
				ar >> dummy;
			}
		}

		if (version <= 37)
		{
			//Alte FlanschId konvertieren
			rohrAnfang = GpConvertOldVersion::getNewFlanschId(rohrAnfang,dnvl);
			rohrEnde = GpConvertOldVersion::getNewFlanschId(rohrEnde,dnvl);
			rohrVerb = GpConvertOldVersion::getNewFlanschId(rohrVerb,dnvl);
		}
	}

	UINT information = 0;
	if (rohrAnfang == KB40 || rohrAnfang == KB50)
	{
		rohrAnfang = SCH;
		information = IDS_SPUELANSCHLUSS_ZU_SCHEIBE;
	}
	if (rohrEnde == KB40 || rohrEnde == KB50)
	{
		information = IDS_SPUELANSCHLUSS_ZU_SCHEIBE;
		rohrEnde = SCH;
	}
	if (information)
		getErrorHandler().appendErrorDescription(getStammdaten().getText("information"), MxError::Information);

	if(version > 55)
	{
		serialize_bool(m_bUeberlaenge, ar);
	}

	if(version > 56)
	{
		serialize_bool(m_IsApaxBauteil, ar);
	}
}

///Rohrquali auf neuen Standard aendern
void Rohrleitung::switchRohrquali()
{
	if (rohrQuali == RQ2440_2458GSCH && dnvl < 65)
		rohrQuali = RQ2440GSCH;
	else if (rohrQuali == RQ2440_2458GSCH && dnvl > 50 )
		rohrQuali = RQ2458GSCH;
}

///Initialisierung der Daten
void Rohrleitung::initDaten(int anzahl, int dnvl, int laenge, CString posNr, int rohrQuali, int rohrVerb)
{
	set_anzahl(anzahl);
	this->dnvl		= dnvl;
	this->laenge	= laenge;
	this->set_posNr(posNr);
	this->rohrQuali = rohrQuali;
    set_rohrVerb(rohrVerb);

	set_maxlaenge(laenge);

	// Rohrquali auf neuen Standard aendern
	switchRohrquali();

	// Nur in NL automatisch anpassen: bei APSAD-Rohren kein VK3
	if (!WaxSys::isAV())
	{
		if (rohrQuali == RQEN10217)
			get_leitkarte().set_vk3(false);
		else
			get_leitkarte().set_vk3(true);
	}
}




///Generieren der Stueckliste
void Rohrleitung::create_stueckliste()
{
///Hier werden die Elemente aus der Elementlist in die Stueckliste gepackt.

	///Temporaere Stueckliste generieren
	CObList templist;
	create_templist(templist, elementList);

	///Rohrqualitaet bei Rohrstutzen setzen
	setQualiRohrstutzen(templist);

	///Templiste in Stueckliste zusammenfassen
	
	putTemplistInStueckliste(templist, stueckliste);

	///Temporaere Objekte wieder loeschen
	for (POSITION xpos = templist.GetHeadPosition(); xpos; )
	{
		Element *x = dynamic_cast<Element*>(templist.GetNext(xpos));
		if (x)
			delete x;
	}
	templist.RemoveAll();

	///Ausschuss addieren 
	addRohrausschuss(stueckliste);
}

///Temporaere Stueckliste generieren
void Rohrleitung::create_templist(CObList& templist, CObList& elementList)
{

#define INSERT_ELEMENT(dn, dnvl, id, laenge) \
	Element *ele = new Element; \
	ele->initStammdaten(dn, dnvl, id); \
	ele->initDaten(0, 0, laenge, id, 0); \
	templist.AddTail(ele); 

#define INSERT_ELEMENT2(element, laenge) \
	Element *ele = new Element(*element); \
	ele->set_laenge(laenge); \
	templist.AddTail(ele); 

	int eleSLaenge = 0, flanschlaenge = 0;

	///Elementliste abarbeiten und in Templiste packen.
	///Hier werden die Sonderbehandlungen vorgenommen.
	if (!elementList.IsEmpty())
	{	
		for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
		{
			Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
			XASSERT(element);
			if (!element)
				break;

			if (element->isEleRohr() && element->get_laenge() <= 0)
				continue;
			
			if (element->get_id() == SNAHT    || element->get_id() == LEER || element->get_id() == GLATT 
			 || element->get_id() == SCHWEISS || element->get_id() == NUT  || element->get_id() == GEW)
				continue;
try {
			// Spuelanschluss sind eigentlich 2 Elemente:
			// Scheibe/Boden + Spans
			if (element->get_id() == KB40) 
			{
				if (element->get_anschlussDN() == 50)
					;
				else if (element->get_anschlussDN() < 200) 
				{
					INSERT_ELEMENT(dnvl, dnvl, SCH, 0)
				}
				else
				{
					INSERT_ELEMENT(dnvl, dnvl, BOD, 0)
				}
				INSERT_ELEMENT(40, dnvl, SPANS, 0)
			} 
			else if (element->get_id() == KB50) 
			{
				if (element->get_anschlussDN() < 200) 
				{
					INSERT_ELEMENT(dnvl, dnvl, SCH, 0)
				}
				else
				{
					INSERT_ELEMENT(dnvl, dnvl, BOD, 0)
				}
				INSERT_ELEMENT(50, dnvl, SPANS, 0)
			}
			else 	
			{
				INSERT_ELEMENT2(element, element->get_laenge());
			}
} 
catch (Element::Error e) 
{	
	getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	
} 
			for (POSITION upos = element->get_uelementList().GetHeadPosition(); upos != NULL;)
			{
				Element *uelement = dynamic_cast<Element*>(element->get_uelementList().GetNext(upos));
				XASSERT(uelement);
				if (!uelement)
					break;

				if (uelement->get_id() == BOHR24 || uelement->get_id() == BOHR50)
					continue;

				for (POSITION uupos = uelement->get_uelementList().GetHeadPosition(); uupos != NULL;)
				{
					Element *uuelement = dynamic_cast<Element*>(uelement->get_uelementList().GetNext(uupos));
					XASSERT(uuelement);
					if (!uuelement)
						break;

					if (uuelement->get_id() == NUT || uuelement->get_id() == GEW || uuelement->get_id() == GLATT)
					{
						uelement->set_urid(uuelement->get_id());
						continue;
					}

					INSERT_ELEMENT2(uuelement, uuelement->get_laenge());

					flanschlaenge = uuelement->get_laenge();
				}
				// erst tatsaechliche Rohrstutzenlaenge berechnen
				if (uelement->get_id() == ROHR || (uelement->get_id() == STDSTUTZEN && uelement->get_urid() == ROHR)) 
				{
					eleSLaenge = berechnungRohrstutzen(uelement->get_dn(), uelement->get_anschlussDN(), uelement->get_laenge(), flanschlaenge, uelement->get_makeAnlage());
				}
				INSERT_ELEMENT2(uelement, (uelement->get_id() == ROHR || (uelement->get_id() == STDSTUTZEN && uelement->get_urid() == ROHR))?eleSLaenge:uelement->get_laenge());
			}
		}
	} 
}

///Rohrqualitaet bei Rohrstutzen setzen
void Rohrleitung::setQualiRohrstutzen(CObList& templist)
{
	///Sollen standardmaessig in der selben Quali wie das Rohr ausgefuehrt werden
	for (POSITION pos = templist.GetHeadPosition(); pos;)	
	{
		Element *ele = dynamic_cast<Element*>(templist.GetNext(pos));
		XASSERT(ele);
		if (!ele) 
			return;

		if (ele->get_id() == ROHR)
		{
			ele->set_id(rohrQuali+ROHR_Q1-1);
			ele->initStammdaten(get_beschichtung().isVerzinkt());
		}
	}
}

///Templiste in Stueckliste zusammenfassen
void Rohrleitung::putTemplistInStueckliste(CObList& templist, CObList& stklist)
{
	///Elemente aus Templiste in Stueckliste packen
	///Hier wird geprueft ob Element schon in Liste,
	///dann wird addiert.
	for (POSITION pos = templist.GetHeadPosition(); pos != NULL;)	
	{
		Element *elem = dynamic_cast<Element*>(templist.GetNext(pos));
		XASSERT(elem);
		if (!elem)
			break;

		// Bezeichnung setzen wegen DN Angabe 
		elem->get_bezeichnung();
		
		// gleiches Element schon in Liste?
		POSITION p = stklist.GetHeadPosition();
		if (!stklist.IsEmpty()) 
		{
			for (p = stklist.GetHeadPosition(); p != NULL;)	
			{
				Element *element = dynamic_cast<Element*>(stueckliste.GetNext(p));
				XASSERT(element);
				if (!element)
					break;

				if (elem->get_artikelnr() == element->get_artikelnr()) 
				{
					if (elem->isEleRohr())
					{
						if (element->isEleRohr())
						{
							element->set_laenge(element->get_laenge()+elem->get_laenge());
							p--; 
							break;
						} 
					} 
					else if (elem->get_id() == ROHR || (elem->get_id() == STDSTUTZEN && elem->get_urid() == ROHR)) 
					{
						if (element->get_id() == ROHR || (elem->get_id() == STDSTUTZEN && elem->get_urid() == ROHR)) 
						{
							element->set_laenge(element->get_laenge()+elem->get_laenge());
							element->set_anzahl(element->get_anzahl()+elem->get_anzahl());
							p--; 
							break;
						}
					}
					else 
					{
						element->set_anzahl(element->get_anzahl()+elem->get_anzahl());
						p--; 
						break;
					}
				}
			}
		}
		if (p == NULL && elem->get_id() != LEER) 
		{
			// Element in Liste
			Element *e = new Element;
			e->copy(*elem);
			stklist.AddTail(e);
		}
	}

	// Alles multiplizieren mit Anzahl der Rohre
	POSITION spos = stklist.GetHeadPosition();
	if (!stklist.IsEmpty()) 
	{
		for (spos = stklist.GetHeadPosition(); spos != NULL;)
		{
			Element *element = dynamic_cast<Element*>(stklist.GetNext(spos));
			XASSERT(element);
			if (!element)
				break;

			if (element->get_id() == ROHR || element->isEleRohr() || (element->get_id() == STDSTUTZEN && element->get_urid() == ROHR) )
				element->set_laenge(element->get_laenge() * get_anzahl());
			else 
				element->set_anzahl(element->get_anzahl() * get_anzahl());
		}
	}
}

//Loescht die Objekte und die Pointer aus der Stueckliste
void Rohrleitung::delete_stueckliste()
{
	stueckliste.clear();
}

///Liefert true zurück, wenn die Materialliste und die Arbeitsliste leer ist. 
///Wird in Rohrleitung überschrieben, damit auch die Elementliste überprüft werden kann.
bool Rohrleitung::isEmpty() const
{
	return Werkauftrag::isEmpty() && elementList.IsEmpty();
}

///Je nach Bedingung wird die richtige ID geliefert
int Rohrleitung::correctElemID(int id, int dn) const
{
	int retTyp = id;

	switch (id) 
	{
	case SCH:
		if (rohrDruckstufe == 16 || dn > 150) 
			retTyp = BOD;
		break;
	
	case ELEMENT_BOGEN:
		if (dn > 50 && dn <= 300 && getBeschichtung().isVerzinkt())
			retTyp = BOGVERZ;
		break;

	default:
		retTyp = id;
	}
	return (retTyp);
}

///Liefert den String der Kappenfarbe zurück
CString Rohrleitung::getKappenfarbe(int farbe)
{
	string ids = "IDS_KEINE_KAPPENFARBE";
	switch (farbe)
	{
		case KEINE:		ids = "IDS_KEINE";			break;
		case ROT:		ids = "IDS_FARBE_ROT";		break;
		case BLAU:		ids = "IDS_FARBE_BLAU";		break;
		case GELB:		ids = "IDS_FARBE_GELB";		break;
		case GRUEN:		ids = "IDS_FARBE_GRUEN";	break;
		case WEISS:		ids = "IDS_FARBE_WEISS";	break;
		case SCHWARZ:	ids = "IDS_FARBE_SCHWARZ";	break;
		case GRAU:		ids = "IDS_FARBE_GRAU";		break;
		default: XASSERT(FALSE);
	}

	std::string str = getStammdaten().getText(ids);
	return str.c_str();
}

///Gesamtgewicht der Rohrleitung berechnen
double Rohrleitung::getGesGewicht(int teile) const 
{
	///teile: 0=alle, 1=Einkauf, 2=Lager, 3=ohne Preis 
	///Gesamtgewicht einer Rohrleitung ermitteln. 
	///Zur Berechnung des gesamten Verteilers muss noch mit
	///der Anzahl multipliziert werden. Das passiert jetzt
	///schon in der Stückliste, heisst, die Anzahl ist schon 
	///in den Elementen der Stueckliste gespeichert.
	
	///Ausschuss fuer Verteiler betraegt 10%, zu SAP und VK3
	///Fuer Gewicht muss man diesen wieder abziehen 
	Preisfaktoren pf;
	double faktor = pf.getPreisfaktor(Preisfaktoren::VERTEILERVERSCHNITT);
	faktor = 100/(100+faktor); 

	double gesGew = 0.0;
	
	// Alles multiplizieren mit Anzahl der Rohre
	for (POSITION spos = stueckliste.GetHeadPosition(); spos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(stueckliste.GetNext(spos));
		XASSERT(ele);
		if (!ele)
			break;

		if (teile == 1 &&  ele->get_einkauf()
		 || teile == 2 && !ele->get_einkauf()
		 || teile == 3 &&  ele->get_festPreis() == 0
		 || teile == 0)
		 if (ele->isEleRohr() || ele->get_id() == ROHR)
				gesGew += ele->getGewicht()*faktor;
			else
				gesGew += ele->getGewicht();
	}
	return gesGew;
}

///Gesamtoberflaeche der Rohrleitung berechnen
double Rohrleitung::getGesOberfl() 
{
	///Gesamtoberflaeche einer Rohrleitung ermitteln
	///Zur Berechnung des gesamten Verteilers muss noch mit	der Anzahl multipliziert werden.
	double gesObfl = 0.0;

	for (POSITION spos = stueckliste.GetHeadPosition(); spos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(stueckliste.GetNext(spos));
		XASSERT(element);
		if (!element)
			break;

		if (element->get_id() == ROHR || element->isEleRohr())
			gesObfl += element->get_oberfl() * (double)element->get_laenge()/1000;
		else 
			gesObfl += element->get_oberfl() * element->get_anzahl();
	}
	return gesObfl;
}



int Rohrleitung::berechnungRohrstutzen(int eleDn, int anschlussDN, int eleLaenge, int flanschLaenge, bool anlage)
{
	const int zuschlagKonstante  = 30;		//bei Handweißung (ab Dn 65 muß Rohr länger sein, damit Schablone für Kontur aufgesetzt werden kann)
	const int zuschlagSonderfall = 5;	    //bei Handschweißung

	const int dn[]     = {32, 40, 50, 65, 80, 100, 125, 0};   //Dn Stutzen
	const int laenge[] = {15, 15, 20, 45, 50,  65,  75, 0};   //Länge UK Stutzen - OK Grundrohr (Maschinenschweißung)

	int eleSLaenge = 0;
			
	DNzuAussendurchmesser dntoad;
	int aussendurchmesser = dntoad.getAussendurchmesserToInt(anschlussDN);
	
	AbzugRadius ar;
	int zuschlagStammdaten = ar.getAbzug(eleDn, anschlussDN);
	

	if (anlage)
	{
		//Stutzenlänge für Maschinenschweißung

		for (int i=0; dn[i]; i++)
		{
			if (dn[i] == eleDn)
			{
				eleSLaenge = laenge[i];
				break;
			}
		}
		eleSLaenge = getStammdaten().queryBrennzugabe.get_brennzugabe(anschlussDN, eleDn);
		TRACE("Stutzen DN %i, Verteiler DN %i -> Brennzugabe %i\n", anschlussDN, eleDn, eleSLaenge);

		if (eleSLaenge == 0)
			return 0;		//Fehler

		eleSLaenge = eleLaenge - flanschLaenge - (aussendurchmesser/2) + eleSLaenge;
	}
	else
	{
		//Stutzenlänge für Handschweißung

		if (eleDn == 50 && anschlussDN == 40) 
		{
			eleSLaenge = eleLaenge - flanschLaenge - (aussendurchmesser/2) + zuschlagSonderfall;
		} 
		else if (eleDn < 65) 
		{
			eleSLaenge = eleLaenge - flanschLaenge - (aussendurchmesser/2) + zuschlagStammdaten;
		} 
		else 
		{
			if (eleDn == anschlussDN)
			{
				eleSLaenge = eleLaenge - flanschLaenge + zuschlagKonstante;
			}
			else
			{
				eleSLaenge = eleLaenge - flanschLaenge;
			}
		}
	}

	if (eleSLaenge <= 0)
		return 0;			//Fehler


	return eleSLaenge;
}

///Rohrkonstruktion erzeugen
///Muss in abgeleiteten Klassen überschrieben werden
void Rohrleitung::createModell()
{
	if (get_laenge() > 0 && isVerteiler() && checkAnlage())
	{
		makeRohrleitungSap();
	}
}

///Laenge von Anfang/Ende/Verbindung setzen
void Rohrleitung::setAnfaEndeVerbLaenge()
{
	/**
	Todo: bool machen
	Muss in abgeleiteten Klassen überschrieben werden
	
	Achtung - Fehler: Endelaenge wird mit dnvl (Start-DN) berechnet. 
	Bei Verteiler muesste aber wegen moeglicher Reduzierung 
	die DN des letzten Rohrstuecks genommen werden.
	*/
	rohrAnfaLaenge = 0;
	rohrEndeLaenge = 0;
	rohrVerbLaenge = 0;
		
	Element ele;
	try 
	{ 
		// Laenge v. Verteileranfang   
		ele.initStammdaten(dnvl, dnvl, rohrAnfang);
		ele.initDaten(0, 0, 0, rohrAnfang, 0);		
		rohrAnfaLaenge = ele.get_laenge();

		// Laenge v. Verteilerende
		ele.initStammdaten(getLastRohrDN(), getLastRohrDN(), rohrEnde);
		ele.initDaten(0, 0, 0, rohrEnde, 0);
		rohrEndeLaenge = ele.get_laenge();

		// Laenge v. Rohrverbindung
		ele.initStammdaten(dnvl, dnvl, rohrVerb);
		ele.initDaten(0, 0, 0, rohrVerb, 0);
		rohrVerbLaenge = ele.get_laenge();
	}
	catch(Element::Error e) 
	{
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	}
}

///Automatische Aufteilung der Rohrlaenge
CString Rohrleitung::setAutoRohrlaengen()
{
	if (elementArray.GetSize() == 0 || !elementArray.has_reduzierung_bogen())
		return Rohrleitung::setRohrlaengen();

	reset_rohrlaengen_und_ids();

	//Automatische Berechnung der Rohrstuecke 
	CString err = "";
	//Rohrleitung	
	int rohrLaenge = get_laenge();
	int rohrDN     = get_dnvl(); 
	int nRohre	   = 0;

	bool trefferRed  = false;	
	bool trefferEle  = false;
	int summe1 = 0, summe2  = 0;
	int j=0, k=0; 
	int rest		   = 0;

	int schnittLaenge  = get_schnittlaenge();

	Element *ele, *elem;
	// erstmal Anzahl ohne Red/Bog
	rest = rohrLaenge % get_schnittlaenge();					
	nRohre = (int)(rohrLaenge / get_schnittlaenge());			
	if (rest > 0) 
		nRohre += 1;
		
	// Schnittlaenge ermitteln
	if (rohrLaenge < get_schnittlaenge()) 
		schnittLaenge = rohrLaenge;
	summe2 = schnittLaenge; 

	// Rohrstuecke incl. Red./Bog ermitteln
	while (summe2 <= rohrLaenge) 
	{
		// noch Elemente in Liste?
		while (k < elementArray.GetSize()) 
		{
			// Element aus Array
			ele = elementArray.get_element_at(k);
			XASSERT(ele);
			if (!ele)
				break;

			if (ele->get_abstAbs() > summe2) 
				break;
			// diese Elemente sind auch Rohrstuecke	
			if (ele->isEleRedBogEinz()) 
			{
				if ((ele->get_abstAbs() >= summe1) && (ele->get_abstAbs() <= summe2)) 
				{	
					rohrID[j] = ROHR_Q1; 
					// Bemassung bis Elementmitte
					if (ele->isEleBogen()) 
					{
						err = checkRohrlaenge(ele->get_abstAbs() - summe1 - ele->get_laenge(), ele);
						if (err.GetLength())
							return err;
						set_rohrLaengen(j, ele->get_abstAbs() - summe1 - ele->get_laenge());	
						summe1 += ele->get_abstAbs() - summe1 - ele->get_laenge();
					// Bemassung bis Elementanfang
					} 
					else 
					{
						err = checkRohrlaenge(ele->get_abstAbs() - summe1, ele);
						if (err.GetLength())
							return err;
						set_rohrLaengen(j, ele->get_abstAbs() - summe1);	
						summe1 += ele->get_abstAbs() - summe1;
					}
					j++;
					rohrID[j]       = ele->get_id(); 
					// bei Bogen muss man bei beiden angrenzenden Stuecken Laenge abziehen
					if (ele->isEleBogen()) 
					{
						set_rohrLaengen(j, 2*ele->get_laenge());			
						summe1         += 2*ele->get_laenge(); 
					} 
					else 
					{
						set_rohrLaengen(j, ele->get_laenge());			
						summe1         +=   ele->get_laenge(); 
					}
					trefferRed = true;
					k++;
				}
				break;
			} 
			else 
			{	
				int counter = 0;
				int summe1Merk = summe1;
				
				elem = elementArray.get_element_at(k-counter);
				XASSERT(elem);
				if (!elem)
					break;

				while (elem->get_abstAbs() == (summe2 - ((50+rohrDN)*counter))) 
				{
					rohrID[j]       = ROHR_Q1+100; 
					int len = (50+rohrDN)*(counter+1);
					set_rohrLaengen(j, schnittLaenge - len);	
					summe1          = summe1Merk + schnittLaenge - len;
					trefferEle = true;
					counter++;
					if (counter <= k)
					{
						elem = elementArray.get_element_at(k-counter);
						XASSERT(elem);
						if (!elem)
							break;
					}
				}
			}
			k++;
		} 

		if (!trefferRed && !trefferEle) 
		{ 
			rohrID[j] = ROHR_Q1+100; 
			set_rohrLaengen(j, schnittLaenge);							
			summe1 = summe2;  	
			summe2 = summe1 + schnittLaenge;
		} 
		else 
		{
			summe2 = summe1 + schnittLaenge;
		}

		trefferRed = false;
		trefferEle = false;
		j++;
	}
		
	// noch Elemente in Liste?
	while (k < elementArray.GetSize()) 
	{
		// Element aus Array
		ele = elementArray.get_element_at(k);
		
		if (ele->isEleRedBogEinz()) 
		{
			if (ele->get_abstAbs() >= summe1) 
			{	
				rohrID[j] = ROHR_Q1; 
				int len = 0, len2 = 0;
				
				if (ele->isEleBogen()) 
					len = ele->get_abstAbs() - summe1 - ele->get_laenge();
				else 
					len = ele->get_abstAbs() - summe1;

				if (len < 0)
				{
					err = checkRohrlaenge(get_rohrLaengen(j-1)+len, ele);
					if (err.GetLength())
						return err;
					set_rohrLaengen(j-1, get_rohrLaengen(j-1)+len);
					rohrID[j-1] = ROHR_Q1; 
				}	
				else
				{
					set_rohrLaengen(j, len);	
					j++;
				}
				summe1 += len;
				
				rohrID[j] = ele->get_id(); 
				if (ele->isEleBogen()) 
					len2 = 2*ele->get_laenge();			
				else 
					len2 = ele->get_laenge();			

				set_rohrLaengen(j, len2);	
				summe1 += len2;
				j++;
				trefferRed = true;
			}
		}
		k++;
	}
	rohrID[j]      = ROHR_Q1; 
	set_rohrLaengen(j, rohrLaenge - summe1);
	nRohre = j+1; 
	set_nRohre(nRohre);

	// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
	err = checkRohrtrennstelle();

	return err;
}


///Setzen der Rohrlaengen
/*
Änderung 13.10.06; Jacobs: unterschiedliche Schnittlängen werden berücksichtigt
*/
CString Rohrleitung::setRohrlaengen() 
{
	CString err; 
	
	int maxSchnittlaenge = 6000; //Maximallänge der Rohrstücke


	reset_rohrlaengen_und_ids();
	// Spezielle Saegebedingungen erfuellen:
	// 1. Ein Rohrstueck sollte min. 70mm ein
	// 2. Rohrstuecke sollten moeglichst > 1499 sein, 
	//    damit diese mit der Anlage gesaegt werden koennen
	// Da Laengen fuer Anfang und Ende in makeRohre() von den Rohrlaengen abgezogen werden,
	// muss hier immer die eingegebene Gesamtlaenge fuer die Berechnung der Laengen verwendet werden.
	// Und da das hintere Rohrstueck auch nach Abzug der Endelaenge min. 70 lang sein muss,
	// ist die Endelaenge auf das 2. Stueck zunaechst aufzuschlagen.
	setAnfaEndeVerbLaenge();
	int laengerohr = laenge - rohrEndeLaenge - rohrAnfaLaenge;
	if ((laengerohr > 6000 && laengerohr < 6071) || 
		(laengerohr > 7449 && laengerohr < 7501) &&
		get_schnittlaenge() == 6000)
	{
		if (laengerohr > 6000 && laengerohr < 6071) 
		{
			rohrLaengen[0] = laenge - 70 - rohrEndeLaenge; 
			rohrLaengen[1] = 70 + rohrEndeLaenge;
		}	
		else if (laengerohr > 7449 && laengerohr < 7501) 
		{
			rohrLaengen[0] = laenge - 1501 - rohrEndeLaenge;
			rohrLaengen[1] = 1501 + rohrEndeLaenge;
		}
		rohrID[0] = ROHR_Q1+100;
		rohrID[1] = ROHR_Q1;
		nTeilRohre = 2;

		// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
		if (laengerohr > 6000)
			err = checkRohrtrennstelle();

		return err;
	}


	//Prüfen ob Schnittlaenge zu lang
	if (get_schnittlaenge() > maxSchnittlaenge ||
		(laengerohr - get_schnittlaenge()) > maxSchnittlaenge)
	{
		err = getStammdaten().getText("IDS_SCHNITTLAENGEa").c_str(); 
		return err;
	}


	if (laengerohr > get_schnittlaenge())
	{
		//Rohr teilen, max. 2 Rohrstücke möglich

		rohrLaengen[0] = get_schnittlaenge();
		// hier die Gesamtlaenge, da Laenge der Enden in makeRohre() abgezogen wird
		rohrLaengen[1] = laenge - get_schnittlaenge();

		rohrID[0] = ROHR_Q1+100;
		rohrID[1] = ROHR_Q1;

		nTeilRohre = 2;

		// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
		err = checkRohrtrennstelle();
		
		return err;
	}
	

	rohrID[0] = ROHR_Q1;
	// hier die Gesamtlaenge rein, da Laenge der Enden in makeRohre() abgezogen wird
	rohrLaengen[0] = laenge;
	nTeilRohre = 1;

	return err;

/*
	// Laengen der einzelnen Rohrstuecke ermitteln
	// Anzahl mit Laenge ohne Enden berechnen
	rest = laengerohr % get_schnittlaenge();					
	nRohre = (int)(laengerohr/get_schnittlaenge());			
	if (rest > 0) 
		nRohre += 1;	
	
	if (nRohre == 1)
	{
		rohrID[0] = ROHR_Q1;
		if (rest == 0)
			rohrLaengen[0] = get_schnittlaenge();
		else 
			// hier die Gesamtlaenge rein, da Laenge der Enden in makeRohre() abgezogen wird
			rohrLaengen[0] = laenge;
	}
	else 
	{
		rohrID[0] = ROHR_Q1+100;
		rohrLaengen[0] = get_schnittlaenge();
		rohrID[1] = ROHR_Q1;
		// hier die Gesamtlaenge, da Laenge der Enden in makeRohre() abgezogen wird
		rest = laenge % get_schnittlaenge();					
		rohrLaengen[1] = (rest == 0) ? get_schnittlaenge() : rest;
	}

	// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
	if (laengerohr > 6000)
		err = checkRohrtrennstelle();

	return err;
*/
}

///Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
CString Rohrleitung::checkRohrtrennstelle()
{
	// Bei Bedarf wird Schweissnaht nach links verschoben. Bedeutet, 
	// dass das Rohr vor der Schweissnaht kuerzer wird, das dahinter
	// laenger.
	int absLaenge = 0;
	CString err;

	// get_nRohre()-1, weil auf Abstand am Ende anders geprueft wird
	for (int i=0; i<get_nRohre()-1; i++) 
	{
		int k = 0;
		absLaenge += get_rohrLaengen(i);
		while (k < elementArray.GetSize()) 
		{
			// Element aus Array
			Element* ele = elementArray.get_element_at(k);
			XASSERT(ele); 
			if (!ele)
				return err;

			if (!ele->isEleRedBogEinz()) 
			{
				// Aussendurchmesser holen
				DNzuAussendurchmesser dntoad;
				int eleAussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_dn());
						
				int diff = ele->get_abstAbs() - absLaenge;
				if (diff < 0) 
					diff *= (-1);

				// Abstand kleiner 1/2 Aussendurchmesser + 50mm 
				if (diff < eleAussendurchmesser/2+50) 
				{
					// Die Laenge kann nur bei wirkliche Rohren veraendert werden, nicht bei Red. od. Boegen  
					if ((rohrID[i]   != ROHR_Q1 && rohrID[i]   != ROHR_Q1+100)
					 || (rohrID[i+1] != ROHR_Q1 && rohrID[i+1] != ROHR_Q1+100))
					{
						/* 
						//Wozu diese Prüfung nur in der NL Version vonehmen? 
						if (!WaxSys::isAV())
						{
							err = getStammdaten().getText("IDS_ERROR73").c_str(); 	 //Es konnte keine gültige Rohrtrennstelle ermittelt werden!
							i = get_nRohre();
							break;
						}
						*/
						break;
					}
					else
					{
						set_rohrLaengen(i, get_rohrLaengen(i)-(eleAussendurchmesser/2+50));
						set_rohrLaengen(i+1, get_rohrLaengen(i+1)+(eleAussendurchmesser/2+50));
						
						// Rohr darf nicht kuerzer als 70mm und nicht laenger als 6000mm sein
						if (rohrLaengen[i] < 70 || rohrLaengen[i+1] > 6000)
						{
							//Es konnte keine gültige Rohrtrennstelle ermittelt werden!	 
							getErrorHandler().appendErrorDescription(getStammdaten().getText("IDS_ERROR73"), MxError::Warning, __FILE__, __LINE__);
							
							i = get_nRohre();
							break;
						}
					}
					absLaenge = 0;
					i = -1;
					break;
				}
			}
			k++;
		}
	}
	return err;
}

///Neues Element in Elementliste packen
void Rohrleitung::elementList_setElement(EnCObListModell& list, Element *ele, int dn, int andn, int id, int abs, int rel, int laenge, int winkel, int rlaenge, bool correctID, bool initStamm)
{
	if (ele == NULL)
		ele = new Element;
	list.AddTail(ele);
	if (correctID)
		ele->set_id(correctElemID(id, dn));
	else
		ele->set_id(id);
	ele->set_dn(dn);
	ele->set_anschlussDN(andn);
	if (initStamm)
		ele->initStammdaten(dn, andn, ele->get_id());
	ele->initDaten(abs, rel, laenge, ele->get_id(), winkel);
	ele->set_rohrLaenge(rlaenge);
}

///Neues Element in Elementliste packen
void Rohrleitung::elementList_setElement(EnCObListModell& list, Element *elem, int rlaenge, bool correctID)
{
	Element *ele = new Element(*elem);
	list.AddTail(ele);
	if (correctID)
		ele->set_id(correctElemID(ele->get_id(), ele->get_dn()));
	ele->initStammdaten(get_beschichtung().isVerzinkt());
	ele->set_rohrLaenge(rlaenge);
}


///Erstes Rohr in Elementliste setzen
bool Rohrleitung::elementList_setRohr(EnCObListModell& elementList, int& rohrDN, int& rohrid, int& laengeVomRohrstueck, Element* ele)
{
	///Elemente initialisiert?
	try 
	{ 
		elementList_setElement(elementList, ele, rohrDN, rohrDN, rohrid, 0, 0, laengeVomRohrstueck, 0, laengeVomRohrstueck);	
	}
	catch(Element::Error e ) 
	{
		elementList.deleteAllElemente();
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
		return false;
	}
	return true;
}

///Abgaenge in Elementliste setzen
void Rohrleitung::elementList_setAbgaenge(EnCObListModell& elementList, int& laengeVomRohrstueck, int& index, int& i, Element* elementRA, Element* ele)
{
	Element *eleprev = NULL;
	Element *elementeMerker = NULL;
	Element *abgeleMerker = NULL;

	///Elemente initialisiert?
	try 
	{ 
		while ( index < elementArray.GetSize()) 
		{
			Element* ex = elementArray.get_element_at(index); 
			if (!ex)
				continue;
			ele->copy(*ex);
		
			if (ele->get_abstAbs() < rohrLaengen[i])
			{ 
				Element *elemente = new Element(*ele);
				elemente->initStammdaten(get_beschichtung().isVerzinkt());
				
				if (index > 0) 
				{ 
					eleprev = elementArray.get_element_at(index-1);

					// Abschluss od. Abgang auf Abgang
					if ( ((ele->get_winkel() == eleprev->get_winkel())
							&& (ele->get_abstRel() == 0) 
							&& (!ele->get_verkettung())) 
					  || ((ele->get_abstRel() != 0) &&  ele->get_verkettung()) )
					{
						if (elementeMerker)
						{
							elementeMerker->get_uelementList().AddTail( elemente );
							// todo: bei Nut die Schnittlaenge von Rohrstutzen nehmen
							if (ele->get_id() == NUT)
							{
								elemente->set_rohrLaenge(eleprev->get_laenge());
							}
							else
							{
								elemente->set_rohrLaenge(laengeVomRohrstueck);
							}
						}
					}
					// Abgang auf Rohr
					else 
					{
						elementRA->get_uelementList().AddTail( elemente );
						elemente->set_rohrLaenge(laengeVomRohrstueck);
					}
				} 
				// Abgang auf Rohr
				else 
				{
					elementRA->get_uelementList().AddTail( elemente );
					elemente->set_rohrLaenge(laengeVomRohrstueck);
				}

				if (ele->get_abstRel() == 0 && (!eleprev || (ele->get_winkel() == eleprev->get_winkel())) && !ele->get_verkettung())
					abgeleMerker = elemente;
				else
					elementeMerker = elemente;

				index++;
			} 
			else 
				break;
		}
	}
	catch (Element::Error e) 
	{
		elementList.deleteAllElemente();
		
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	}
}

///Ende vom Rohrstueck in Elementliste setzen
void Rohrleitung::elementList_setEndeRohrstueck(EnCObListModell& elementList, int& rohrDN, int& rohrid, int& laengeVomRohrstueck, int& i, Element* elementRA, Element* ele, int summe1)
{
	///Elemente initialisiert?
	try 
	{ 
		Element e;
		e.set_id(rohrID[i-1]);
		
		///Red/Bog
		if (ele && (((ele->isEleRed())	 && (ele->get_abstAbs() == summe1))  
		         || ((ele->isEleBogen()) && (ele->get_abstAbs() == (summe1+rohrLaengen[i+1]/2))))) 
		{  
			elementList_setElement(elementList, ele, laengeVomRohrstueck, true);

			rohrDN = ele->get_dn(); 
			i++;
		} 

		///Einz
		else if (ele && ele->get_id() == EINZ && ele->get_abstAbs() == summe1)  
		{  

			if (ele->get_dn() < ele->get_anschlussDN())
			{
				elementList_setElement(elementList, ele, laengeVomRohrstueck);
		
				rohrDN = ele->get_dn(); 
				laengeVomRohrstueck++;
				elementRA->initDaten(0, 0, (laengeVomRohrstueck), rohrid, 0);
				elementRA->set_rohrLaenge(laengeVomRohrstueck);
			}
			else
				elementList_setElement(elementList, NULL, rohrDN, rohrDN, SCHWEISS, ele->get_abstAbs(), ele->get_abstRel(), 1, 0, laengeVomRohrstueck);	
			i++;
		} 
		// sonst Verb
		else 
		{
			// bei Rohr Ueberlaenge ist Verb. immer SCHWEISS (Schweissnaht Werk)
			if (rohrID[i] == ROHR_Q1+100) 
			{
				int laenge = 0;
				for (int l=0; l<=i; l++)
					laenge += rohrLaengen[l];

				// nach dem ersten Rohrstueck
				if (i==0)
					elementList_setElement(elementList, NULL, rohrDN, rohrDN, SCHWEISS, rohrLaengen[i]-rohrAnfaLaenge-1, rohrLaengen[i]-rohrAnfaLaenge-1, 1, 0, laengeVomRohrstueck);	
				// wenn vorheriges Stueck kein Rohr, dann auch keine Verblaenge abziehen
				else if (rohrID[i-1] != ROHR_Q1+100 && rohrID[i-1] != ROHR_Q1)
					elementList_setElement(elementList, NULL, rohrDN, rohrDN, SCHWEISS, laenge, laenge, 1, 0, laengeVomRohrstueck, false, false);	
				// sonst
				else
					elementList_setElement(elementList, NULL, rohrDN, rohrDN, SCHWEISS, laenge-rohrVerbLaenge-1, laenge-rohrVerbLaenge-1, 1, 0, laengeVomRohrstueck, false, false);	
			}
			// wenn vorheriges Stueck Ueberlaenge war
			else if (rohrID[i-1] == ROHR_Q1+100) 
			{
				elementList_setElement(elementList, NULL, rohrDN, rohrDN, rohrVerb, rohrLaengen[i]-rohrVerbLaenge, rohrLaengen[i]-rohrVerbLaenge, rohrVerbLaenge, 0, laengeVomRohrstueck, true);	
			}
			else if (e.isEleRedBog()) 
			{
				elementList_setElement(elementList, NULL, rohrDN, rohrDN, rohrVerb, rohrLaengen[i]-1-rohrVerbLaenge, rohrLaengen[i]-1-rohrVerbLaenge, rohrVerbLaenge, 0, laengeVomRohrstueck, true);	
			}
			// sonst Auswahl
			else 
			{
				if (i==0)
					elementList_setElement(elementList, NULL, rohrDN, rohrDN, rohrVerb, rohrLaengen[i]-rohrAnfaLaenge-rohrVerbLaenge, rohrLaengen[i]-rohrAnfaLaenge-rohrVerbLaenge, rohrVerbLaenge, 0, laengeVomRohrstueck);	
				else
					elementList_setElement(elementList, NULL, rohrDN, rohrDN, rohrVerb, rohrLaengen[i]-2*rohrVerbLaenge, rohrLaengen[i]-2*rohrVerbLaenge, rohrVerbLaenge, 0, laengeVomRohrstueck, true);	
			}
		}
		i++;
	}
	catch(Element::Error e ) 
	{
		elementList.deleteAllElemente();
		
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	}
}

///Anfang vom Rohrstueck in Elementliste setzen
void Rohrleitung::elementList_setAnfangRohrstueck(EnCObListModell& elementList, int& rohrDN, int& index, int& i, Element* elementVA, Element* ele, int redabst)
{
	///Elemente initialisiert?
	try 
	{ 
		// zu "(rohrLaengen[i]==0 || rohrLaengen[0]==0 && i==2)?LEER:SCHWEISS":
		// Bogen in der Mitte		- nach Bogen kommt SCHWEISS
		// Bogen am Anfang			- nach Bogen kommt LEER
		// Rohr nach Bogen Laenge 0 - nach Bogen kommt LEER
	
		////////// nach Red kommt SCHWEISS //////////
		if ((ele->get_id() == KRED || ele->get_id() == ERED) && (ele->get_abstAbs() == redabst)) 
		{ 
			elementList_setElement(elementList, elementVA, rohrDN, rohrDN, SCHWEISS, rohrLaengen[i-1]-rohrAnfaLaenge, rohrLaengen[i-1]-rohrAnfaLaenge, (rohrLaengen[i]==0)?0:1, 0, rohrLaengen[i]-rohrVerbLaenge-rohrVerbLaenge, false);	
			index++;
		}

		// nach Bog kommt SCHWEISS (mit Laenge=0, 
		// weil bei Bogen die Bogenlaenge inkl. Schweissnaht bei beiden Rohren schon abgezogen ist
		else if (ele->isEleBogen() && ele->get_abstAbs() == redabst) 
		{ 
			elementList.AddTail( elementVA );
			if (rohrLaengen[i]>0)
				elementVA->initStammdaten(rohrDN, rohrDN, SCHWEISS);
	
			elementVA->initDaten(rohrLaengen[i-1]-rohrAnfaLaenge, 
							     rohrLaengen[i-1]-rohrAnfaLaenge, 
								 0, SCHWEISS, 0);
			elementVA->set_laenge(0);
			elementVA->set_rohrLaenge(rohrLaengen[i]-rohrVerbLaenge-rohrVerbLaenge);
		
			index++;
		}
		else if (ele->get_id() == EINZ 	&& ele->get_abstAbs() == redabst) 
		{ 
			if (ele->get_dn() < ele->get_anschlussDN())
			{
				elementList.AddTail( elementVA );
				if (rohrLaengen[i]>0)
					elementVA->initStammdaten(rohrDN, rohrDN, SCHWEISS);
				elementVA->initDaten(rohrLaengen[i-1]-rohrAnfaLaenge, 
									 rohrLaengen[i-1]-rohrAnfaLaenge, 
									 (rohrLaengen[i]==0)?0:1, (rohrLaengen[i]==0 || rohrLaengen[0]==0 && i==2)?LEER:SCHWEISS, 0);
				elementVA->set_rohrLaenge(rohrLaengen[i]-rohrVerbLaenge-rohrVerbLaenge);
			}
			else
			{
				elementList.AddTail( elementVA );
				elementVA->initStammdaten(ele->get_dn(), ele->get_anschlussDN(), ele->get_id());
				elementVA->initDaten(ele->get_abstAbs(), ele->get_abstRel(),
									 ele->get_laenge(), ele->get_id(), ele->get_winkel());
				elementVA->set_rohrLaenge(rohrLaengen[i]-rohrVerbLaenge-rohrVerbLaenge);
				rohrDN = ele->get_dn(); 
			}
			index++;
		} 
		////////// sonst Verb. //////////
		else 
		{
			// das vorherige Rohr war Ueberlaenge / nach SCHWEISS kommt LEER
			if (rohrID[i-1] == ROHR_Q1+100) 
			{
				elementList.AddTail( elementVA );
				elementVA->initDaten(rohrLaengen[i-1]-rohrAnfaLaenge, 
									 rohrLaengen[i-1]-rohrAnfaLaenge, 
									 0, LEER, 0);
				elementVA->set_rohrLaenge(rohrLaengen[i]-rohrVerbLaenge-rohrVerbLaenge);
			} 
			// sonst Auswahl / nach SNAHT kommt GLATT
			else 
			{
				elementList.AddTail( elementVA );
				elementVA->initStammdaten(rohrDN, rohrDN, (rohrVerb == SNAHT) ? GLATT : correctElemID(rohrVerb, rohrDN));
				elementVA->initDaten(0, 
									 0, 
									 (rohrVerb == SNAHT)?0:rohrVerbLaenge, (rohrVerb == SNAHT) ? GLATT : correctElemID(rohrVerb, rohrDN), 0);
				elementVA->set_rohrLaenge(rohrLaengen[i]-2*rohrVerbLaenge);
			}
		}
	}
	catch(Element::Error e ) 
	{
		elementList.deleteAllElemente();
		
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	}
}

///Rohrende in Elementliste setzen
void Rohrleitung::elementList_setRohrende(EnCObListModell& elementList, int& rohrDN, int& laengeVomRohrstueck, int& i)
{
	///Elemente initialisiert?
	try 
	{ 
		Element *elementE = new Element;
		elementList.AddTail( elementE );
		elementE->initStammdaten(rohrDN, rohrDN, correctElemID(rohrEnde, rohrDN));
		elementE->set_rohrLaenge(laengeVomRohrstueck);
		if (i == 0) 
		{
			elementE->initDaten((rohrLaengen[i]-rohrEndeLaenge-rohrAnfaLaenge), 
								get_restabstand(),
								rohrEndeLaenge, correctElemID(rohrEnde, rohrDN), 0);
		}
		else if (rohrID[i-1] != ROHR_Q1) 
		{
			elementE->initDaten((rohrLaengen[i]-rohrEndeLaenge), 
								get_restabstand(),
								rohrEndeLaenge, correctElemID(rohrEnde, rohrDN), 0);
		}
		else 
		{
			elementE->initDaten((rohrLaengen[i]-rohrVerbLaenge-rohrEndeLaenge), 
								get_restabstand(),
								rohrEndeLaenge, correctElemID(rohrEnde, rohrDN), 0);
		}

		
	}
	catch(Element::Error e ) 
	{
		elementList.deleteAllElemente();
		
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	}
}

//Erstellen der Elementliste der Rohrleitung für n Rohre mit 
//Elementen auf dem Rohr
bool Rohrleitung::makeRohre(EnCObListModell& elementList)
{
	///Todo: Wegen durch Reduzierung veraenderte Nennweite kann sich die Rohrverb/ende aendern.
	///      Die jeweiliges Laenge muss immer wieder neu berechnet werden. 
	
	// Rohrleitung	
	int rohrDN     = get_dnvl(); // wegen Reduzierung
	int rohrid     = get_rohrQuali()+ROHR_Q1-1;
	//TRACE("Rohrquali = %i, rohrid = %i\n", get_rohrQuali(), rohrid);
	int nTeilRohreLocal = get_nRohre();
	// allgemein
	int i=0, j=0, index=0; 
	int summe1=0, summe2=0;
	int laengeVomRohrstueck = 0;
	int templaenge = 0;

	// Merker
	Element *elementeMerker = NULL;
	Element *abgeleMerker = NULL;
	Element *eleprev = NULL;
	Element *ele = NULL;

	///Elemente initialisiert?
	try 
	{ 
		///Laenge vom Rohrstueck bestimmen
		if (nTeilRohreLocal>1)
		{
			templaenge = rohrLaengen[i] - rohrAnfaLaenge;
			// wenn Rohrstueck Ueberlaenge od. Einziehung, wird 1mm fuer Schweissnaht abgezogen
			if (rohrID[i] == ROHR_Q1+100 || rohrID[i+1] == EINZ)
			{
				templaenge -= 1;
			}
			else if (rohrID[i+1] == ELEMENT_BOGEN || rohrID[i+1] == BOG2S || rohrID[i+1] == BOGVERZ || rohrID[i+1] == ERED  || rohrID[i+1] == KRED)
			{
				;
			}
			// sonst Verb.laenge abziehen
			else 
			{
				templaenge -= rohrVerbLaenge;
			}
			laengeVomRohrstueck = templaenge;
		}
		else
		{
			laengeVomRohrstueck = rohrLaengen[i] - rohrAnfaLaenge - rohrEndeLaenge;
		}
		
		///Rohranfang in Elementliste setzen
		elementList.newElement(dnvl, dnvl, correctElemID(rohrAnfang,dnvl), 0, 0, rohrAnfaLaenge, 0, laengeVomRohrstueck);
		
		///Erstes Rohr in Elementliste setzen
		Element *elementRA = NULL;
		elementRA = new Element;
		if (!elementList_setRohr(elementList, rohrDN, rohrid, laengeVomRohrstueck, elementRA))
			return false;

		ele = new Element;
		///Abgaenge in Elementliste setzen
		elementList_setAbgaenge(elementList, laengeVomRohrstueck, index, i, elementRA, ele);

		if (nTeilRohreLocal > 1)
		{
			///Ende vom Rohrstueck in Elementliste setzen
			summe1 = rohrLaengen[i];

			elementList_setEndeRohrstueck(elementList, rohrDN, rohrid, laengeVomRohrstueck, i, elementRA, ele, summe1);
					
			if(nTeilRohreLocal > 2)
			{
				////////// Mittelrohre //////////
				while( i < (nTeilRohreLocal-1) ) 
				{
					summe1 = summe2 = 0;
					for (j=0; j<=i; j++) summe1 += rohrLaengen[j];
					for (j=0; j<i;  j++) summe2 += rohrLaengen[j];

					///Anfang vom Rohrstueck in Elementliste setzen
					Element *elementVA = new Element;
					elementList_setAnfangRohrstueck(elementList, rohrDN, index, i, elementVA, ele, summe2-ele->get_laenge());
					

					///Laenge vom Rohrstueck bestimmen
					templaenge = rohrLaengen[i];	
					// vorheriges ist Bog od. Ueberlaenge
					if (rohrID[i-1] == ELEMENT_BOGEN || rohrID[i-1] == BOG2S || rohrID[i-1] == BOGVERZ || rohrID[i-1] == ROHR_Q1+100)
					{
						;	
					}
					// vorheriges ist Red
					else if (rohrID[i-1] == KRED || rohrID[i-1] == ERED)
					{
						templaenge -= 1;	
					}
					// vorheriges ist Einz
					else if (rohrID[i-1] == EINZ)
					{
						if (elementVA->get_id() != EINZ)
							templaenge -= 1;	
					}
					// sonst Verb
					else 
					{
						templaenge -= (rohrVerb == SNAHT)?0:rohrVerbLaenge;
					}

					// jetztiges ist Ueberlaenge
					if (rohrID[i] == ROHR_Q1+100)
					{
						templaenge -= 1;	
					}
					// naechstes ist Red
					else if (rohrID[i+1] == KRED || rohrID[i+1] == ERED || rohrID[i+1] == ELEMENT_BOGEN || rohrID[i+1] == BOG2S || rohrID[i+1] == BOGVERZ)
					{
						;	
					}
					// das naechste Rohr ist Einziehung
					else if (rohrID[i+1] == EINZ)	
					{
						templaenge -= 1;
					}		
					// sonst
					else 
					{
						templaenge -= rohrVerbLaenge;
					}
					laengeVomRohrstueck = (rohrLaengen[i]==0)?0:templaenge;


					///Rohr in Elementliste setzen
					Element *elementR = new Element;
					if (!elementList_setRohr(elementList, rohrDN, rohrid, laengeVomRohrstueck, elementR))
					{
						CString str; str = getStammdaten().getText("IDS_ERROR74").c_str(); 					
						throw Element::Error(str);
					}


					///Abgaenge in Elementliste setzen
					elementList_setAbgaenge(elementList, laengeVomRohrstueck, index, i, elementR, ele);


					////////// Elemente //////////
					while (index < elementArray.GetSize()) 
					{
						Element* ex = elementArray.get_element_at(index);
						if (!ex)
							continue;
						ele->copy(*ex);

						if ((ele->get_abstAbs() < summe1) && (ele->get_abstAbs() > summe2)) 
						{ 
							Element *elemente = new Element(*ele);
							elemente->initStammdaten(get_beschichtung().isVerzinkt());
							
							if (index > 0) 
							{ 
								eleprev = elementArray.get_element_at(index-1);
						
								// Abschluss od. Abgang auf Abgang
								if ( ((ele->get_winkel() == eleprev->get_winkel())
										&& (ele->get_abstRel() == 0) 
										&& (!ele->get_verkettung())) 
								  || ((ele->get_abstRel() != 0) &&  ele->get_verkettung()) )
								{
									elementeMerker->get_uelementList().AddTail( elemente );
									// todo: bei Nut die Schnittlaenge von Rohrstutzen nehmen
									if (ele->get_id() == NUT)
										elemente->set_rohrLaenge(eleprev->get_laenge());
									else
										elemente->set_rohrLaenge(laengeVomRohrstueck);
								}
								// Abgang auf Rohr
								else 
								{
									elementR->get_uelementList().AddTail( elemente );
									elemente->set_rohrLaenge(laengeVomRohrstueck);
								}
							// Abgang auf Rohr
							} 
							else 
							{
								elementR->get_uelementList().AddTail( elemente );
								elemente->set_rohrLaenge(laengeVomRohrstueck);
							}
							if (ele->get_abstRel() == 0 && (!eleprev || (ele->get_winkel() == eleprev->get_winkel())) && !ele->get_verkettung())
								abgeleMerker = elemente;
							else
								elementeMerker = elemente;
							index++;
						} 
						else break;
					}
					
					///Ende vom Rohrstueck in Elementliste setzen
					elementList_setEndeRohrstueck(elementList, rohrDN, rohrid, laengeVomRohrstueck, i, elementR, ele, summe1);
				}
			}

			int next = (i==1)?i+1:i;
			if (rohrLaengen[next]-rohrEndeLaenge > 0 || nTeilRohreLocal == 2)
			{
				// zu "(rohrLaengen[i]==0 || rohrLaengen[0]==0 && i==2)?LEER:SCHWEISS":
				// Bogen in der Mitte		- nach Bogen kommt SCHWEISS
				// Bogen am Anfang			- nach Bogen kommt LEER
				// Rohr nach Bogen Laenge 0 - nach Bogen kommt LEER
				 				
				templaenge = rohrLaengen[i]-((rohrLaengen[i]==0 || laengeVomRohrstueck==0 && i==2)?0:rohrVerbLaenge)-rohrEndeLaenge;

				if (laengeVomRohrstueck >= 0)
				{
										

						//TRACE("El Abs: %d\n",ele->get_abstAbs());
					    //TRACE("Berech: %d\n",summe1+rohrLaengen[i-1]/2);

					////////// nach Red kommt SCHWEISS//////////
					if ((ele->get_id() == KRED || ele->get_id() == ERED) && (ele->get_abstAbs() == summe1))  
					{  
						Element *elementE = new Element;
						elementList.AddTail( elementE );
						if (rohrLaengen[i]==0 || rohrLaengen[0]==0 && i==2)
						{
							elementE->initDaten(0, 0, 0, LEER, 0);
						}
						else
						{
							elementE->initStammdaten(rohrDN, rohrDN, SCHWEISS);
							elementE->initDaten(0, 0, 1, SCHWEISS, 0);
						}
						elementE->set_rohrLaenge(templaenge);
						index++;
						if (i==1) i++;
					}
					////////// nach Bog kommt SCHWEISS//////////
					else if (ele->isEleBogen() && (ele->get_abstAbs() == (summe1+rohrLaengen[i-1]/2))) 
					{  
						Element *elementE = new Element;
						elementList.AddTail( elementE );
						elementE->initStammdaten(rohrDN, rohrDN, SCHWEISS);
						elementE->initDaten(0, 0, 0, (rohrLaengen[i]==0 || rohrLaengen[0]==0 && i==2)?LEER:SCHWEISS, 0);
						elementE->set_laenge(0);
						elementE->set_rohrLaenge(templaenge);
						index++;
						if (i==1) 
							i++;
					}
					////////// nach Einz kommt LEER / neu SCHWEISS//////////
					else if (ele->get_id() == EINZ && ele->get_abstAbs() == summe1)  
					{  
						if (ele->get_dn() < ele->get_anschlussDN())
						{
							Element *elementE = new Element;
							elementList.AddTail( elementE );
							elementE->initStammdaten(rohrDN, rohrDN, SCHWEISS);
							elementE->initDaten(0, 0, 1, (rohrLaengen[i]==0 || rohrLaengen[0]==0 && i==2)?LEER:SCHWEISS, 0);
							elementE->set_rohrLaenge(templaenge);
						}
						else
						{
							Element *elementE = new Element;
							elementList.AddTail( elementE );
							elementE->initStammdaten(ele->get_dn(), ele->get_anschlussDN(), ele->get_id());
							elementE->initDaten(ele->get_abstAbs(), ele->get_abstRel(), ele->get_laenge(), ele->get_id(), ele->get_winkel());
							elementE->set_rohrLaenge(templaenge);
							rohrDN = ele->get_dn(); 
						}
						index++;
						if (i==1) 
							i++;
					} 
					////////// sonst Verb. //////////
					else 
					{
						if (rohrID[i-1] == ROHR_Q1+100) 
						{
							Element *elementE = new Element;
							elementList.AddTail( elementE );
							elementE->initDaten(0, 0, 0, LEER, 0);
							elementE->set_rohrLaenge(laengeVomRohrstueck);
						} 
						else 
						{
							Element *elementE = new Element;
							elementList.AddTail( elementE );
							elementE->initStammdaten(rohrDN, rohrDN, (rohrVerb == SNAHT)?GLATT:correctElemID(rohrVerb, rohrDN));
							elementE->initDaten(0, 0, ((rohrVerb == SNAHT)?0:rohrVerbLaenge), (rohrVerb == SNAHT)?GLATT:correctElemID(rohrVerb, rohrDN), 0);
							elementE->set_rohrLaenge(laengeVomRohrstueck);
						}
					}
				}

				////////// letztes Rohrstueck //////////
				templaenge = rohrLaengen[i]-rohrEndeLaenge;	
				// vorheriges Rohr ist Red 
				if (rohrID[i-1] == KRED || rohrID[i-1] == ERED)
				{
					templaenge -= (rohrLaengen[i]==0 /*|| laengeVomRohrstueck == 0 && i==2*/) ? 0:1;
				}			
				// vorheriges Rohr ist Bog 
				else if (rohrID[i-1] == ELEMENT_BOGEN || rohrID[i-1] == BOG2S || rohrID[i-1] == BOGVERZ)
				{
					;
				}			
				
				// vorheriges Rohrstueck ist Einziehung 
				else if (rohrID[i-1] == EINZ)
				{
					// vorheriges Rohr wird eingezogen
					if (ele->get_dn() < ele->get_anschlussDN())
					{
						templaenge -= 1;
					}
					// dieses Rohr wird eingezogen
					else
					{
						;
					}
				}
				
				// vorheriges Rohr ist Ueberlaenge 
				else if (rohrID[i-1] == ROHR_Q1+100)
				{
					;
				}			
				// vorheriges Rohr ist keine Ueberlaenge 
				else if (rohrID[i-1] == ROHR_Q1)
				{
					templaenge	-= (rohrVerb == SNAHT)?0:rohrVerbLaenge;
				}
				laengeVomRohrstueck	= templaenge;	

				
				///Letztes Rohr in Elementliste setzen
				Element *elementRE = new Element;
				if (!elementList_setRohr(elementList, rohrDN, rohrid, laengeVomRohrstueck, elementRE))
				{
					CString str; str = getStammdaten().getText("IDS_ERROR74").c_str(); 					
					throw Element::Error(str);
				}
	
				////////// Elemente //////////
				summe1 = 0;
				summe2 = 0;
				for (j=0; j< i; j++) summe1 += rohrLaengen[j];
				for (j=0; j<=i; j++) summe2 += rohrLaengen[j];
			
				while ( index < elementArray.GetSize() ) 
				{
					Element* ex = elementArray.get_element_at(index); 
					if (!ex)
						continue;
					ele->copy(*ex);

					if ( (ele->get_abstAbs() > rohrLaengen[0]) 
					  && (ele->get_abstAbs() < summe2) ) 
					{ 
						Element *elemente = new Element(*ele);
						elemente->initStammdaten(get_beschichtung().isVerzinkt());

						if (index > 0) 
						{
							eleprev = elementArray.get_element_at(index-1);
							
							// Abschluss od. Abgang auf Abgang
							if ( ((ele->get_winkel() == eleprev->get_winkel())
									&& (ele->get_abstRel() == 0) 
									&& (!ele->get_verkettung())) 
							  || ((ele->get_abstRel() != 0) &&  ele->get_verkettung()) )
							{
								elementeMerker->get_uelementList().AddTail( elemente );
								// todo: bei Nut die Schnittlaenge von Rohrstutzen nehmen
								if (ele->get_id() == NUT)
									elemente->set_rohrLaenge(eleprev->get_laenge());
								else
									elemente->set_rohrLaenge(laengeVomRohrstueck);
							}
							// Abgang auf Rohr
							else 
							{
								elementRE->get_uelementList().AddTail( elemente );
								elemente->set_rohrLaenge(laengeVomRohrstueck);
							}
						} 
						// Abgang auf Rohr
						else 
						{
							elementRE->get_uelementList().AddTail( elemente );
							elemente->set_rohrLaenge(laengeVomRohrstueck);
						}
						if (ele->get_abstRel() == 0 && (!eleprev || (ele->get_winkel() == eleprev->get_winkel())) && !ele->get_verkettung())
							abgeleMerker = elemente;
						else
							elementeMerker = elemente;
						index++;
					} 
					else break;
				}
			}
		}
		///Rohrende in Elementliste setzen
		elementList_setRohrende(elementList, rohrDN, laengeVomRohrstueck, i);
		delete ele;
		return true;
	}
	catch(Element::Error e) 
	{
		delete ele;
		elementList.deleteAllElemente();
		
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
		return false;
	}
}

///Hier wird automatisch der Arbeitsplan generiert
void Rohrleitung::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	createModell();

	///Wenn ap 32000, dann wird neu berechnet
	anzahlNennweiten.RemoveAll();

	///Standardarbeitsfolgen fuer Werkauftrag generieren
	Werkauftrag::createAutoArbeit();


	//bei Verzinkung Preis direkt zuordnen
	for (POSITION pos = get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos; )
	{
		
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge*>(get_arbeit().get_arbeitsfolgen().GetNext(pos));
		if (!afo)
			continue;

		
		if (afo->get_id() == 32)
		{
			ZinkKosten z(getStammdaten().get_verzinkung().query(this->get_beschichtung().get_verzinkerei()), *this);
			afo->set_preis(z.GesamtKosten() - z.GesamtVKZuschlag());			
		}
	
	}


}

///Fuegt einen neuen Radius ein
void Rohrleitung::add_radien(int radius)
{
	for (int i=0; i<radien.GetSize(); i++)
	{
		if (radien.GetAt(i) == radius)
			return;
	}
	radien.Add(radius);
}

///Ruestzeit fuers Klinken wird ueber Afo 8 (Ruesten) hinzugefuegt.
///Es muss bei jedem neu einzustellenden Radius umgeruestet werden,
///nicht nur bei jeder neuen Nennweite.
void Rohrleitung::addRuestzeiten()
{
	///Todo:AV nachfragen ob jetzt ok, dann Attribut ruesten entfernen	
	///Ruestzeiten zu addieren ist hier nicht noetig, da Ruestzeit bei der Preisberechnung zu Arbeitszeit addiert wird
	int i;
	for (i=0; i<4; i++)
		ruesten[i] = false;
	///Ruestzeit fuers Klinken in extra Afo 8
	radien.RemoveAll();
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr())
		{
			for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
			{
				Element *uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(upos));
				XASSERT(uele);
				if (!uele)
					break;
			
				if (uele->get_id() == ROHR)
				{
					int radius = 0;
					AbzugRadius ar;
					radius = ar.getRadius(uele->get_dn(), uele->get_anschlussDN());

					if (uele->get_dn() == 25 && radius > 0)
					{
						add_radien(radius);
						ruesten[0] = true;
					}
					else if (uele->get_dn() == 32 && radius > 0)
					{
						add_radien(radius);
						ruesten[1] = true;
					}
					else if (uele->get_dn() == 40 && radius > 0)
					{
						add_radien(radius);
						ruesten[2] = true;
					}
					else if (uele->get_dn() == 50 && radius > 0)
					{
						add_radien(radius);
						ruesten[3] = true;
					}
				}
			}
		}
	}

	int count = 0;
	for (i=0; i<4; i++)
	{
		if (ruesten[i]) 
			count++;
	}

	if (radien.GetSize() > 0)
		get_arbeit().addAfo_Ruesten(radien.GetSize());
}

///Rohr mit Stutzen
void Rohrleitung::setRohrMitStut()
{
	// Fuer Passlaenge und Pulvern
	int nr = 0, i = 0;
	int nrMitStut[256];
	int idMerk = 999, lastNrMitStut = 0;
	Element *eleMerk = NULL;
	// Elementliste nach Rohrstuecken absuchen
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;

		// Wenn Element ein Rohr ist, dann werden Rohrstutzen gesucht
		if (ele->isEleRohr()) 
		{
			// Wenn mit vorherigem Rohr verbunden, dann erstmal Status von diesem uebernehmen
			if (idMerk == LEER || idMerk == SCHWEISS)
				ele->set_rohrMitStut(eleMerk->get_rohrMitStut());
			else
				nr++;
			ele->set_rohrNr(nr);

			if (!ele->get_rohrMitStut())
			{
				CObList& uelementList = ele->get_uelementList();
				for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
				{
					Element *uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
					XASSERT(uele);
					if (!uele)
						break;

					if (uele->get_id() == ROHR)
					{
						ele->set_rohrMitStut(true);
						if (lastNrMitStut != nr)
						{
							// Bei Rohr mit	Stut die Rohrnr merkern
							nrMitStut[i] = nr;
							i++;
						}
						// Wenn mit vorherigem Rohr verbunden, hat vorheriges Rohr selben Status 
						if (idMerk == LEER || idMerk == SCHWEISS)
							eleMerk->set_rohrMitStut(true);

						break;
					}
				}
			}
		}
		eleMerk = ele;
		idMerk = ele->get_id();
	}
	
	// alle Rohrnr mit Stut durchgehen und Rohre mit gleicher Nr in Elementliste suchen und Status setzen
	for (int j=0; j<i; j++)
	{
		for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
		{
			Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
			XASSERT(ele); 
			if (!ele) 
				break;

			// Wenn Element ein Rohr ist, 
			if (ele->isEleRohr()) 
			{
				// dann Rohrnr mit Rohrnr mit Stut vergleichen und Status setzen
				if (nrMitStut[j] == ele->get_rohrNr())
					ele->set_rohrMitStut(true);
			}
		}
	}
}

//Berechnung der Laenge pro Nr.
void Rohrleitung::calc_laengeProNr()
{
	// Fuer Passlaenge und Pulvern
	int nr = 0, nrMerk = 0, abglaenge = 0;
	
	laengeProNr.RemoveAll();
	dnProNr.RemoveAll();
	stutProNr.RemoveAll();
	
	
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;

		// Wenn Element ein Rohr ist, 
		if (ele->isEleRohr()) 
		{
			// Unterelementliste nach Rohrstutzen suchen und groesste Laenge merken
			CObList& uelementList = ele->get_uelementList();
			for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
			{
				Element *uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
				XASSERT(uele); 
				if (!uele) 
					break;

				// Wenn Element ein Rohr ist, 
				if (uele->get_id() == ROHR) 
				{
					abglaenge = (abglaenge < uele->get_laenge()) ? uele->get_laenge() : abglaenge;
				}
			}
			
			nr = ele->get_rohrNr();
			// dann Laeng/Nr addieren, groesste DN und Stut/nichtStut setzen
			if (laengeProNr.GetSize() == 0)
			{
				laengeProNr.Add(ele->get_laenge());
				dnProNr.Add(ele->get_dn());
				stutProNr.Add(ele->get_rohrMitStut());
				
			}
			else 
			{
				if (nr == nrMerk)
				{
					laengeProNr.SetAt(laengeProNr.GetSize()-1, laengeProNr.GetAt(laengeProNr.GetSize()-1) + ele->get_laenge());
					if (dnProNr.GetAt(dnProNr.GetSize()-1) < ele->get_dn())
						dnProNr.SetAt(dnProNr.GetSize()-1, ele->get_dn());
				}
				else 
				{
					laengeProNr.Add(ele->get_laenge());
					dnProNr.Add(ele->get_dn());
					stutProNr.Add(ele->get_rohrMitStut());
				}
			}
			nrMerk = nr;
		}
	}
}

//Bei den Rohrstuecken links/rechts den Beschriftungstext setzen
void Rohrleitung::rohreBeschriften(CObList& elementList)
{
	// wanr
	CString str = "", txt = "";
	txt.Format("%06ld", get_status().get_wanr());
		for (int i=2; i<6; i++) 
			str += txt.GetAt(i);

	int count = 0, schweissnr = 0;
	CString rechts = "", links = "";
	// Elementliste abarbeiten und Rohrstuecke beschriften 
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRohr() && element->get_laenge() > 0) 
		{
			POSITION p = elementList.FindIndex(count-1); 
			if (!p)
				continue;
			Element *prev = dynamic_cast<Element*>(elementList.GetAt(p));
			XASSERT(prev);
			if (!prev)
				break;

			if (prev->get_id() != LEER && prev->get_id() != EINZ)
			{
				if (schweissnr == 0)
					links.Format("%s-%s.%d", str, get_posNr(), ++schweissnr);
				else
					links.Format("%s.%d", get_posNr(), ++schweissnr);
				element->set_beschriftungLinks(links);
				continue;
			}
			
			p = elementList.FindIndex(count+1); 
			Element *next = dynamic_cast<Element*>(elementList.GetAt(p));
			XASSERT(next);
			if (!next)
				break;

			if (next->get_id() != SCHWEISS && next->get_id() != KRED  && next->get_id() != ERED
			 && next->get_id() != EINZ     && next->get_id() != ELEMENT_BOGEN && next->get_id() != BOG2S
			 && next->get_id() != BOG2S)
			{
				if (schweissnr == 0)
					links.Format("%s-%s.%d", str, get_posNr(), ++schweissnr);
				else
					links.Format("%s.%d", get_posNr(), ++schweissnr);
				element->set_beschriftungLinks(links);
			}
		}
		count++;
	}
}

//get laengeProNr
const CArray<int,int>& Rohrleitung::get_laengeProNr() const
{
	return laengeProNr;
}

//get dnProNr
const CArray<int,int>& Rohrleitung::get_dnProNr() const
{
	return dnProNr;
}

//get stutProNr
const CArray<bool,bool>& Rohrleitung::get_stutProNr() const
{
	return stutProNr;
}

//get dnvl
const int Rohrleitung::get_dnvl() const
{
	return dnvl;
}

//set dnvl
 void Rohrleitung::set_dnvl(int value)
{
	dnvl = value;
}

//get laenge
const int Rohrleitung::get_laenge() const
{
	return laenge;
}

//set laenge
void Rohrleitung::set_laenge(int value)
{
	laenge = value;
	set_maxlaenge(laenge);
}

//get nRohre
const int Rohrleitung::get_nRohre() const
{
	return nTeilRohre;
}

//set nRohre
void Rohrleitung::set_nRohre(int value)
{
	nTeilRohre = value;
}

//get rohrLaengen
const int Rohrleitung::get_rohrLaengen(int index) const
{
	return rohrLaengen[index];
}

//set rohrLaengen
void Rohrleitung::set_rohrLaengen(int index, int value)
{
	rohrLaengen[index] = value;
}

//get rohrQuali
int Rohrleitung::get_rohrQuali() const
{
	return rohrQuali;
}

//set rohrQuali
void Rohrleitung::set_rohrQuali(int value)
{
	rohrQuali = value;
}

//get rohrVerb
const int Rohrleitung::get_rohrVerb() const
{
	return rohrVerb;
}

//set rohrVerb
void Rohrleitung::set_rohrVerb(int value)
{
	rohrVerb = correctElemID(value, dnvl);
}

//get elementListSap
EnCObListModell& Rohrleitung::get_elementListSap()
{
	return elementListSap;
}

//get elementList
const EnCObListModell& Rohrleitung::get_elementList() const
{
	return elementList;
}


EnCObListModell& Rohrleitung::get_elementList()
{
	return elementList;
}


//Rückgabe der Arrays


///Liefert ja nach Rohrleitungstyp das (AV)Element Array
EnContainerRohrElemente& Rohrleitung::getElementArray()
{
	return get_elementArray();
}

//get elementArray
EnContainerRohrElemente& Rohrleitung::get_elementArray()
{
	return elementArray;
}



//get stdelementArray
EnContainerRohrElemente& Rohrleitung::get_stdelementArray()
{
	return stdelementArray;
}






///Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
CString Rohrleitung::getTabname()
{
	return "element";
}



//get schnittlaenge
int Rohrleitung::get_schnittlaenge() 
{
	return schnittlaenge;
}

//set schnittlaenge
void Rohrleitung::set_schnittlaenge(int value)
{
	schnittlaenge = value;
}

//get rohrKFarbe
const int Rohrleitung::get_rohrKFarbe() const
{
	return rohrKFarbe;
}

//set rohrKFarbe
void Rohrleitung::set_rohrKFarbe(int value)
{
	rohrKFarbe = value;
}

//get rohrAnfang
const int Rohrleitung::get_rohrAnfang() const
{
	return rohrAnfang;
}

//set rohrAnfang
void Rohrleitung::set_rohrAnfang(int value)
{
	//rohrAnfang = correctElemID(value, dnvl);
	rohrAnfang = value;
}

//get rohrEnde
const int Rohrleitung::get_rohrEnde() const
{
	return rohrEnde;
}

//set rohrEnde
void Rohrleitung::set_rohrEnde(int value)
{
	//rohrEnde = correctElemID(value, getLastRohrDN());
	rohrEnde = value;
}

//get rohrDruckstufe
const int Rohrleitung::get_rohrDruckstufe() const
{
	return rohrDruckstufe;
}

//set rohrDruckstufe
void Rohrleitung::set_rohrDruckstufe(int value)
{
	rohrDruckstufe = value;
}


EnContainerRohrElemente& Rohrleitung::get_avelementArray() 
{
	return avelementArray;
}

const EnContainerRohrElemente& Rohrleitung::get_avelementArray() const
{
	return avelementArray;
}

const EnContainerRohrElemente& Rohrleitung::get_stdelementArray() const
{
	return stdelementArray;
}

const EnContainerRohrElemente& Rohrleitung::get_elementArray() const
{
	return elementArray;
}

// Liefert als default avelementArray, bei Verteiler allerdings elementArray
const EnContainerRohrElemente& Rohrleitung::getelemente() const
{
	return avelementArray;
}

//Errechnet die Pulverzeit für den Werkauftrag
double Rohrleitung::getPulverzeit() 
{
	double zeit = 0.0;
	int maxdn = getMaxRohrDN();
	int maxabglaenge = getMaxAbgLaenge();	

	if (get_beschichtung().isVerzinkt() || get_beschichtung().isGrundiert() || laenge == 0)
		return 0.0;
	
	int rohrlaenge = 0;
	if (laenge > 12000)
		rohrlaenge = 12000;
	else
		rohrlaenge = laenge;


	PulvernZeiten::data *pz = NULL;
	for (POSITION pzpos = getStammdaten().get_pulvernzeiten().GetHeadPosition(); pzpos;)
	{
		pz = (PulvernZeiten::data *)getStammdaten().get_pulvernzeiten().GetNext(pzpos);

		if (   (pz->get_dn()            == maxdn)				 
			&& (pz->get_laenge_von()    <= rohrlaenge)			
			&& (pz->get_laenge_bis()    >= rohrlaenge	|| pz->get_laenge_bis()    == 0)
			&& (pz->get_abglaenge_von() <= maxabglaenge)			
			&& (pz->get_abglaenge_bis() >= maxabglaenge || pz->get_abglaenge_bis() == 0))
		{
			zeit = (pz->get_zeit() * (double)get_anzahl());
			break;
		}
	}
	if (laenge > 12000)
		zeit *= nTeilRohre;

	XASSERT(zeit);
	if (!zeit)
		return 0;
	return zeit;
}


///Errechnet die Putzzeit für den Werkauftrag
double Rohrleitung::getPutzzeit() 
{
	if (!get_beschichtung().isVerzinkt())
		return 0.0;
#if 0
	Verzinkung zink(this);

	return zink.getPutzzeit();
#else
	return 0.0;
#endif
}

///Errechnet die Grundierzeit für den Werkauftrag
double Rohrleitung::getGrundierungszeit()  
{
	// Wenn Typ 10 ist, dann ist doppelt grundiert ausgewählt
	if (!get_beschichtung().isGrundiert())
		return 0.0;

	const int grundierungsafo = 21;
	double typ = 1.0;
	if (get_beschichtung().get_typ() == Beschichtung::DOPPELT_GRUND)
		typ = 2.0;

	AfoZeiten::data *az = NULL;
	for (POSITION azpos = getStammdaten().get_afozeiten().GetHeadPosition(); azpos;)
	{
		az = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(azpos);
	
		if (az->get_dn() == dnvl && az->get_afoid() == grundierungsafo)
		{
			// Achtung: Zeit/m! 
			return az->get_zeit() * get_anzahl() * get_laenge()/1000 * typ;
		}
	}
	XASSERT(FALSE);
	return 0.0;
}



//Fügt die AV Elemente je nach absolutem Abstand in das Elementarray ein.
void Rohrleitung::processAVElemente()
{
	// AVElemente je nach absoluten Abstand ins Array schieben
	EnContainerRohrElemente &a = get_elementArray();
	for(int i=0; i < avelementArray.GetSize(); i++)
	{
		Element *e = avelementArray.get_element_at(i);
		XASSERT(e);
		if (!e)
			break;
		
		// Element mit kleinerem absoluten Abstand suchen
		int j;
		for (j=0; j<a.GetSize(); j++) 	
		{
			Element *elem = a.get_element_at(j);
			XASSERT(elem);
			if (!elem)
				break;

			if (e->get_abstAbs() < elem->get_abstAbs())
				break;
		}

		if(e->get_id() != SPRINKLERSCHELLE_VIROTEC)
		{
			Element *ele = new Element(*e);
			
			// Wenn es ein Element mit kleinerem absoluten Abstand gibt: einfügen sonst anhängen
			if (j < a.GetSize())
				a.InsertAt(j, ele);
			else 
				a.Add(ele);
		}
	}
}

///Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. Stationsverteiler oder Entnahmeleitungen
void Rohrleitung::fill_elementArray()
{
}

const int Rohrleitung::get_restabstand() const
{
	return restabstand;
}

void Rohrleitung::set_restabstand(int value)
{
	/**
	Setzt den Restabstand, darf nur nach der Längenberechnung aufgerufen werden
	*/
	if (value) // setzen
		restabstand = value;
	else // berechnen
	{
		if (elementArray.GetSize() > 0)
		{
			Element* ele = elementArray.get_element_at(elementArray.GetSize()-1); 
			XASSERT(ele);
			if (!ele)
				return;
			restabstand = laenge - ele->get_abstAbs();
		}
		else
			restabstand = laenge;
	}
}

bool Rohrleitung::get_avnachbearbeitung() const
{
	return avnachbearbeitung_per_zeichnung || get_avelementArray().GetSize() > 0;
}

void Rohrleitung::set_avnachbearbeitung_per_zeichnung(bool value)
{
	avnachbearbeitung_per_zeichnung = value;
}

bool Rohrleitung::get_avnachbearbeitung_per_zeichnung() const
{
	return avnachbearbeitung_per_zeichnung;
}

///Ist ein Rohrstutzen in der Konstruktion mit/ohne Ende
const int Rohrleitung::isRohrstutzen(int drehfaktor) const
{
	/**
	Funktion wird benutzt, um den Abstand der Bemassung zu ermitteln.
	Drehfaktor ist die Richtung des Rohres. Speziell geht es hier also um die Rohrstutzen,
	die in Richtung Bemassungslinie gezeichnet und beschriftet werden.
	Returnwert: 0=nein, 1=ohne Ende, 2=mit Ende
	Da bei Rohrstutzen nun auch glatt als Ende eingegeben werden muss, 
	ist das Unterelement auf glatt zu prüfen. 
	*/
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr()) 
		{
			const CObList& uelementList = ele->get_uelementList();
			for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)
			{
				const Element *uele = dynamic_cast<const Element*>(uelementList.GetNext(upos));
				XASSERT(uele);
				if (!uele)
					break;
				
				// Winkel setzen
				int winkel = 0;
				if (uele->get_winkel() >= 0 && uele->get_winkel() <= 89)
					winkel = 0;
				else if (uele->get_winkel() >= 90 && uele->get_winkel() <= 179)
					winkel = 90;
				else if (uele->get_winkel() >= 180 && uele->get_winkel() <= 269)
					winkel = 180;
				else if (uele->get_winkel() >= 270 && uele->get_winkel() <= 359)
					winkel = 270;

				if ((uele->get_id() == STDSTUTZEN && uele->get_urid() == ROHR &&(winkel == 0   || winkel == 90)  && drehfaktor == 0) 
				 || (uele->get_id() == STDSTUTZEN && uele->get_urid() == ROHR &&(winkel == 180 || winkel == 270) && drehfaktor == 90) 
				 || (uele->get_id() == STDSTUTZEN && uele->get_urid() == ROHR &&(winkel == 180 || winkel == 270) && drehfaktor == 180) 
				 || (uele->get_id() == STDSTUTZEN && uele->get_urid() == ROHR &&(winkel == 0   || winkel == 90)  && drehfaktor == 270))
					return 2;

				if ((uele->get_id() == ROHR && (winkel == 0   || winkel == 90)  && drehfaktor == 0) 
				 || (uele->get_id() == ROHR && (winkel == 180 || winkel == 270) && drehfaktor == 90) 
				 || (uele->get_id() == ROHR && (winkel == 180 || winkel == 270) && drehfaktor == 180) 
				 || (uele->get_id() == ROHR && (winkel == 0   || winkel == 90)  && drehfaktor == 270))
				{
					if (uele->get_uelementList().IsEmpty())
						return 1;
					else
					{
						const Element* uuele = dynamic_cast<const Element*>(uele->get_uelementList().GetHead());
						if (!uuele || uuele->get_id() == GLATT)
							return 1;
						else 
							return 2;
					}
				}
			}
		} 
	} 
	return 0;
}

///Liefert die groesste Nennweite der Leitung
int Rohrleitung::getMaxRohrDN() const
{
	// todo: virtual und bei Gewinderohr ableiten
	int maxRohrDN = 0;
	bool firstele = true;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		// Fitting am Anfang?
		if (firstele)
		{
			firstele = false;
			if (ele->get_id() == GRFITTING)
				maxRohrDN = ele->get_anschlussDN();
		}

		if (ele->isEleRohr())
			if (maxRohrDN < ele->get_dn())
				maxRohrDN = ele->get_dn();
	}
	return maxRohrDN;
}

///Liefert die kleinste Nennweite der Leitung
int Rohrleitung::getMinRohrDN() const
{
	// todo: virtual und bei Gewinderohr ableiten
	int minRohrDN = 1000;
	bool firstele = true;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		// Fitting am Anfang?
		if (firstele)
		{
			firstele = false;
			if (ele->get_id() == GRFITTING)
				minRohrDN = ele->get_anschlussDN();
		}

		if (ele->isEleRohr())
			if (minRohrDN > ele->get_dn())
				minRohrDN = ele->get_dn();
	}
	return minRohrDN;
}

///Liefert die Nennweite am Ende der Leitung
int Rohrleitung::getLastRohrDN() const
{
	int lastRohrDN = dnvl;

	for (int i=0; i<elementArray.GetSize(); i++)
	{
		const Element* const ele = elementArray.get_element_at(i);
		if (!ele)
			break;
	
		if (ele->isEleRedEinz())
			lastRohrDN = ele->get_dn();
	}
	return lastRohrDN;
}

int Rohrleitung::getMaxAbgLaenge() const
{
	int maxAbgLaenge = 0;
	
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr())
		{
			for (POSITION pos = ele->get_uelementList().GetHeadPosition(); pos != NULL;)	
			{
				const Element *uele = dynamic_cast<const Element*>(ele->get_uelementList().GetNext(pos));
				XASSERT(uele);
				if (!uele)
					break;

				if (uele->get_id() == ROHR)
					maxAbgLaenge = (uele->get_laenge() > maxAbgLaenge) ? uele->get_laenge() : maxAbgLaenge;
			}
		}
	}
	return maxAbgLaenge;
}

 int Rohrleitung::setNewDrehfaktor(int drehfaktor, int winkel) const
{
	if (winkel == 90)
		return -90;
	if (winkel == 270)
		return -270;

	if (drehfaktor == 0) 
	{
		if		(winkel ==   0) drehfaktor =  90;
		else if (winkel == 180) drehfaktor = 270;
	
	}
	else if (drehfaktor ==  90) 
	{
		if		(winkel ==   0) drehfaktor = 180;
		else if (winkel == 180) drehfaktor =   0;
	
	} 
	else if (drehfaktor == 180) 
	{
		if		(winkel ==   0) drehfaktor = 270;
		else if (winkel == 180) drehfaktor =  90;
	
	}
	else if (drehfaktor == 270) 
	{
		if		(winkel ==   0) drehfaktor =   0;
		else if (winkel == 180) drehfaktor = 180;
	}
	return drehfaktor;
}

int Rohrleitung::getMaxXLaenge()
{
	int maxXLaenge1 = 0;
	int maxXLaenge2 = 0;
	int drehfaktor = 0;
	int boglaenge = 0;
	bool bog = false;
	bool firstbog = false;
	
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (drehfaktor == 0)
		{
			if (firstbog && ele->isEleBogen())
				maxXLaenge1 += ele->get_laenge();
			maxXLaenge1 += ele->get_laenge();
		}
		else if (drehfaktor == 180)
		{
			if (ele->isEleBogen())
				maxXLaenge2 += ele->get_laenge();
			maxXLaenge2 += ele->get_laenge();
		}

		if (ele->isEleBogen())
		{
			drehfaktor = setNewDrehfaktor(drehfaktor, ele->get_winkel());
			boglaenge = ele->get_laenge();
			bog = true;
			firstbog = true;
		}
	}

	if (bog && drehfaktor == 0)
		maxXLaenge1 += boglaenge;
	else if (bog && drehfaktor == 180)
		maxXLaenge2 += boglaenge;

	//CString err;
	//err.Format("X: %d \n-X: %d", maxXLaenge1, maxXLaenge2); 
	

	return (maxXLaenge1>maxXLaenge2)?maxXLaenge1:maxXLaenge2;
}

int Rohrleitung::getMaxYLaenge()
{
	int maxYLaenge1 = 0;
	int maxYLaenge2 = 0;
	int drehfaktor = 0;
	int boglaenge = 0;
	bool bog = false;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (drehfaktor == 90)
		{
			if (ele->isEleBogen())
				maxYLaenge1 += ele->get_laenge();
			maxYLaenge1 += ele->get_laenge();
		}
		else if (drehfaktor == 270)
		{
			if (ele->isEleBogen())
				maxYLaenge2 += ele->get_laenge();
			maxYLaenge2 += ele->get_laenge();
		}

		if (ele->isEleBogen())
		{
			drehfaktor = setNewDrehfaktor(drehfaktor, ele->get_winkel());
			boglaenge = ele->get_laenge();
			bog = true;
		}
	}

	if (bog && drehfaktor == 90)
		maxYLaenge1 += boglaenge;
	else if (bog && drehfaktor == 270)
		maxYLaenge2 += boglaenge;

	//CString err;
	//err.Format("Y: %d \n-Y: %d", maxYLaenge1, maxYLaenge2); 
	

	return (maxYLaenge1>maxYLaenge2)?maxYLaenge1:maxYLaenge2;
}

///Liefert die Anzahl der Rohtstuecke in der Rohrleitung
int Rohrleitung::getAnzahlRohrstuecke()
{
	int count = 0;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRohr()) 
			count++;
	}
	return count;
}

void Rohrleitung::addRohrausschuss(CObList& list)
{
	// Ausschuss fuer Verteilerrohre betraegt 10%, nur zu VK3
	Preisfaktoren pf;
	double faktor = 1+pf.getPreisfaktor(Preisfaktoren::VERTEILERVERSCHNITT)/100;

	for (POSITION pos = list.GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(list.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr() || ele->get_id() == ROHR)
		{
			double laenge = (double)ele->get_laenge() * faktor;
			ele->set_laenge((int)laenge);
		}
	}
}

const int Rohrleitung::get_rohrAnfaLaenge() const
{
	return rohrAnfaLaenge;
}

const int Rohrleitung::get_rohrEndeLaenge() const
{
	return rohrEndeLaenge;
}

const int Rohrleitung::get_rohrVerbLaenge() const
{
	return rohrVerbLaenge;
}

const CString& Rohrleitung::get_fertigungshinweis() const
{
	return fertigungshinweis;
}


std::vector<std::string> Rohrleitung::getFertigungshinweis() const
{
	/*
	Liefert den Fertigungshinweis zeilenweise 
	*/
	string txt = (LPCSTR)fertigungshinweis;
	vector<string> res;
	boost::algorithm::split(res, txt,  boost::algorithm::is_any_of("\r\n"));
	return res;
	
}

void Rohrleitung::set_fertigungshinweis(const CString& value)
{
	fertigungshinweis = value;
}

///Rohrstutzen auf Abschlusselement pruefen 
CString Rohrleitung::check_rohrstutzen()
{
	CString msg;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;
		
		if (ele->isEleRohr())
		{
			for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
			{
				Element *uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(upos));
				XASSERT(uele);
				if (!uele)
					break;

				if (uele->get_id() == ROHR)
				{
					if (uele->get_uelementList().IsEmpty())
					{
						CString str; 
						str = getStammdaten().getText("IDS_ERROR75").c_str(); 
						CString elemsg;
						elemsg.Format("%s (l=%i mm): %s", uele->get_kurztext(), uele->get_laenge(), str);
#ifdef _DEBUG
						CDumpContext dc;
						elementList.Dump(dc);
#endif
						msg += elemsg;
					}
				}
			}
		}
	}
	return msg;
}

/// Prüfen ob anstatt Feuerverzinkt auch Werksverzinkt möglich ist
CString Rohrleitung::check_werksVerzinkt() const
{
	//Wenn Beschichtung nicht verzinkt ist, keine weitere Prüfung vornehmen
	if (!getBeschichtung().isVerzinkt())
		return "";

	//Wenn schon RohrQuali Verzinkt, keine weitere Prüfung
	if (get_rohrQuali() == RQ2440VZIN || get_rohrQuali() == RQEN10217VERZ)
		return "";


	//Wenn Rohr geschweißt wird, keine weitere Prüfung vornehmen
	if (isSchweissen())
		return "";

	//Wenn Rohr länger 6000, dann wird auch geschweißt 
	if (get_laenge() > 6000)
		return "";
	
	if (get_laenge() == 6000)
	{
		return getStammdaten().getText("IDS_WERKSVERZ").c_str(); 
	}

	return "";
}


///Rohrkonstruktion auf korrekte Werte checken
CString Rohrleitung::checkPlausis()
{
	CString err, meldung;
	try
	{
		check_rohrQuali(get_dnvl());
		check_rohrAnfangEndeVerb();
		check_dnvl();
		check_posnr();
		check_elementArray();
		check_bogen_abstand_anfang_ende();
		check_biegung();
		checkHoeheBeschichten();
		err = check_rohrstutzen();
	}
	catch (CString exception)
	{
		err = exception;
	}
	catch (std::string exception)
	{
		err = exception.c_str();
	}
	catch (char *exception)
	{
		err = exception;
	}
	catch (Element::Error e)
	{
		err = e.get_errorText().c_str();
	}
	catch (std::exception const & e)
	{
		err = e.what();
	}

	return err;
}




void Rohrleitung::check_bogen_abstand_anfang_ende() const
{
	/**
	Prüft die Rohrleitungslänge gegen die Bogenlänge
	und den Abstand des Bogens zum Anfangselement
	*/
	if (!isBogen())
		return;
	int len = 0;
	int dnabschluss = dnvl;
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		const Element* const ele = elementArray.get_element_at(i);
		
		if (ele->isEleBogen())
		{
			// Nur beim ersten Bogen Gesamtlänge addieren, danach ist der rel. Abstand Bogenmitte zu Bogenmitte
			if (!i)
				len += ele->get_laenge(); 
			len += ele->get_abstRel();
		}
		dnabschluss = ele->get_anschlussDN();
	}
	
	// Länge kleinstes Rohrstück dazurechnen, wenn Ende != GLATT
	// Wenn GLATT, kann Bogen auch ganz am Ende sitzen
	Element elel;
	if (get_abschluss_element(elel))
		len += get_min_rohrlaenge(elel);
	
	if (len > this->get_laenge())
	{
		CString err;
		err.Format(getStammdaten().getText("IDS_ERROR_BOGENLAENGE").c_str(), get_laenge(), len);
		throw err;
	}
	
	// Abstand des ersten Bogens vom Rohranfang prüfen
	const Element* const ele = elementArray.get_element_at(0);
	if (!ele->isEleBogen())
		return;
	
	int mindest_abstand = get_min_rohrlaenge(get_anfang_element());

	if (ele->get_abstRel() - ele->get_laenge() < mindest_abstand)
	{
		CString err;
		err.Format(getStammdaten().getText("IDS_ERROR_BOGENLAENGE2").c_str(), ele->get_abstRel(), mindest_abstand + ele->get_laenge());
		throw err;
	}
}

int Rohrleitung::get_min_rohrlaenge(const Element& elem) const
{
	/**
	Liefert die Mindestrohrlänge für ein Rohrstück, das mit einem Element verbunden ist
	also z.B. Mindestabstand vom Rohranfang
	*/
	
	const int mindest_rohrlaenge = 50;

	const int id = elem.get_id();
	switch (id)
	{
	case GLATT:
		return 0;
	case NUT: 
	case GEW:
		return mindest_rohrlaenge;
	default:
		return elem.get_laenge();
	}
}

const Element Rohrleitung::get_anfang_element() const
{
	Element anfang;
	anfang.initStammdaten(dnvl, dnvl, get_rohrAnfang());
	return anfang;
}

const bool Rohrleitung::get_abschluss_element(Element &ele) const
{
	const Element* const letztes_ele = elementArray.get_element_at(elementArray.GetSize()-1);
	
	try
	{
		ele.initStammdaten(letztes_ele ? letztes_ele->get_dn() : dnvl, letztes_ele ? letztes_ele->get_dn() : dnvl, get_rohrEnde());
	}
	catch(Element::Error e)
	{
		return false;
	}
	
	
	return true;
}

CString Rohrleitung::checkHinweis() const
{
	//prüfen ob Werksverzinkt möglich ist. Wenn ja, nur eine Meldung bringen
	CString meldung;
	meldung = check_werksVerzinkt();
	return meldung;
}

///Absoluten Abstand zur Rohrlaenge checken
void Rohrleitung::check_absAbst_rohrlaenge(const Element* ele)
{
	// Fehlermeldungen für den Bereich +-50 vom Rohranfang/ende werden nur in der NL angezeigt, 
	// führen aber nicht zum Abbruch der Verarbeitung.
	
	bool is_av = WaxSys::isAV(); 
	DNzuAussendurchmesser dntoad;
	int eleAussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_dn());
	string err, errmessage;

	if (ele->isEleRedBogEinz())
	{
		// zum Ende
		if (rohrEnde == NUT || rohrEnde == GEW)
		{
			if (ele->get_abstAbs() > get_laenge() - ele->get_laenge() - 50)
				//Bei NUT od. GEWINDE am Ende darf absoluter Abstand nur Rohrlaenge-Elementlänge-50mm=%dmm sein!
				errmessage = str(format(getStammdaten().getText("IDS_ERROR76")) % (get_laenge() - ele->get_laenge() - 50));
		}
		else
		{
			if (ele->get_abstAbs() > get_laenge() - ele->get_laenge())
				//Absoluter Abstand darf nur Rohrlänge-Elementlänge=%dmm sein!
				errmessage = str(format(getStammdaten().getText("IDS_ERROR77")) % (get_laenge() - ele->get_laenge()));
		}
		// zum Anfang
		if (rohrAnfang == NUT || rohrAnfang == GEW)
		{
			if (ele->get_abstAbs() < 50)
				errmessage = getStammdaten().getText("IDS_ERROR78");
		}
	}
	// Keine Reduzierung, Bogen oder Einziehung
	else
	{
		// zum Ende
		if (ele->get_abstAbs() > (get_laenge() - (int)(eleAussendurchmesser/2) - 50)) 
			//Abstand zum Rohrende muss min. Elementdurchmesser(%dmm)/2+50mm=%dmm sein!
			errmessage = str(format(getStammdaten().getText("IDS_ERROR79")) % eleAussendurchmesser % ((int)(eleAussendurchmesser/2)+50) );

		// zum Anfang
		if (ele->get_abstAbs() < (int)(eleAussendurchmesser/2) + 50)
			//Abstand zum Rohranfang muss min. Elementdurchmesser(%dmm)/2+50mm=%dmm sein!
			errmessage = str(format(getStammdaten().getText("IDS_ERROR80")) % eleAussendurchmesser % ((int)(eleAussendurchmesser/2)+50) );
		
		// zum Ende
		if (ele->get_abstAbs() > (get_laenge() - (int)(eleAussendurchmesser/2))) 
			//Abstand zum Rohrende sollte min. Elementdurchmesser(%dmm)/2=%dmm sein!
			err = str(format(getStammdaten().getText("IDS_ERROR81")) % eleAussendurchmesser % ((int)(eleAussendurchmesser/2))); 

		// zum Anfang
        if (ele->get_abstAbs() < (int)(eleAussendurchmesser/2))
			//Abstand zum Rohranfang sollte min. Elementdurchmesser(%dmm)/2=%dmm sein!
			err = str(format(getStammdaten().getText("IDS_ERROR82")) % eleAussendurchmesser % ((int)(eleAussendurchmesser/2)));
	}

	if (err.length())
		throw (CString)err.c_str();

	if (errmessage.length() && !is_av)
		getErrorHandler().appendErrorDescription(errmessage, MxError::Information);
}

///Prueft die Nennweite fuer Scheibe
void Rohrleitung::check_AnfangEnde_ScheibeOk() const
{
	/***
	wird in Strangrohr überschrieben
	*/
	CString err, str;
	if (rohrAnfang == SCH || rohrEnde == SCH) 
	{
		if (dnvl < 25 || dnvl > 150) 
		{
			str = getStammdaten().getText("IDS_ERROR23").c_str();  //Bei Scheibe muss DN zwischen %d und %d sein!
			err.Format(str, 25, 150);
			throw err;
		}
		if (getBeschichtung().isVerzinkt() && dnvl < 65)
		{
			err = getStammdaten().getText("IDS_ERROR83").c_str();  //Bei verzinktem Rohr und DN < 65 ist keine Scheibe möglich.\nBitte Gewinde auswählen und Kappe vor Ort aufschrauben!
			throw err; 
		}
	} 
}

///Rohrqualitaet checken
void Rohrleitung::check_rohrQuali(int dnvl) const
{
	CString err, str;
	Element ele;
	ele.set_id(ROHR_Q1 + get_rohrQuali() - 1); 

	switch (rohrQuali)
	{
		case RQ2448NLOS_CO2_DICK:
		case RQ2448NLOS_MX1230200:
		case RQ2448NLOS_MX1230200_DN65_GEPULVERT_EXTRA_DICK:
		break;

		case RQ2458GSCH:
			if (getBeschichtung().isGrundiert() && (dnvl < 65 || getLastRohrDN() < 65))
			{
				str = getStammdaten().getText("IDS_ERROR56").c_str();  // und DN < 65 nicht grundiert erhältlich!
				throw (CString)(ele.get_bezeichnung() + str);
			}
			else if ( !m_bUeberlaenge &&(laenge > 9000 && (dnvl < 50 || getLastRohrDN() < 50))) 
			{
				str = getStammdaten().getText("IDS_ERROR57").c_str();  // und DN 32/40 und Länge > 9000 ungültig!
				throw (CString)(ele.get_bezeichnung() + str);
			}
			else if (dnvl < 32 || getLastRohrDN() < 32) 
			{
				str = getStammdaten().getText("IDS_ERROR58").c_str();  // < DN 32 ungültig!
				throw (CString)(ele.get_bezeichnung() + str);
			}
			break;
		
		
		case RQ2440_2458GSCH:
			if (dnvl < 25 || dnvl > 300) 
			{
				str = getStammdaten().getText("IDS_ERROR61").c_str();  // < DN25 & > DN300 ungültig!
				throw ele.get_bezeichnung() + str;
			}
			break;

		case RQ2448NLOS:
			if (dnvl < 32) 
			{
				str = getStammdaten().getText("IDS_ERROR62").c_str();  // < DN %d ungültig!
				err.Format(str, 32);
				throw ele.get_bezeichnung() + err;
			}
			break;
		case RQ2440NLOS:
			if (dnvl > 150) 
			{
				str = getStammdaten().getText("IDS_ERROR63").c_str();  // > DN %d ungültig!
				err.Format(str, 150);
				throw ele.get_bezeichnung() + err;
			}
			break;

		case RQ2458DICK:
			if (dnvl != 100 && dnvl != 150) 
			{
				str = getStammdaten().getText("IDS_ERROR86").c_str();  // nur DN 100/150 gültig!
				throw ele.get_bezeichnung() + str;
			}
			break;

		case RQ2440VZIN:
			if (dnvl < 25 || dnvl > 50) 
			{
				str = getStammdaten().getText("IDS_ERROR63").c_str();  // > DN %d ungültig!
				err.Format(str, 50);
				throw ele.get_bezeichnung() + err;
			}
			if (laenge > 6000) 
			{
				str = getStammdaten().getText("IDS_ERROR84").c_str();  // nur bis 6000mm!
				throw ele.get_bezeichnung() + str;
			}
			if (isSchweissen())
			{
				str = getStammdaten().getText("IDS_ERROR85").c_str();  // kann nicht geschweisst werden!
				throw ele.get_bezeichnung() + str;
			}
			break;
			
		case RQ2440GSCH:
			if (dnvl < 25 || dnvl > 50) 
			{
				str = getStammdaten().getText("IDS_ERROR64").c_str();  // nur DN 25-50 gültig!
				throw ele.get_bezeichnung() + str;
			}
			break;
		

		case RQEN10217:
			if (dnvl < 25 || dnvl > 250) 
			{
				str.Format(getStammdaten().getText("IDS_NENNWEITE_UNGUELTIG").c_str(), 25, 250); // Nennweite < DN %i & > DN %i ungültig!
				throw ele.get_bezeichnung() + str;
			}
			break;
		case RQEN10217VERZ:
			if (dnvl < 32 || dnvl > 200) 
			{
				str.Format(getStammdaten().getText("IDS_NENNWEITE_UNGUELTIG").c_str(), 32, 200); // Nennweite < DN %i & > DN %i ungültig!
				throw ele.get_bezeichnung() + str;
			}
			if (laenge > 6000) 
			{
				str = getStammdaten().getText("IDS_ERROR84").c_str();  // nur bis 6000mm!
				throw ele.get_bezeichnung() + str;
			}
			if (isSchweissen())
			{
				str = getStammdaten().getText("IDS_ERROR85").c_str();  // kann nicht geschweisst werden!
				throw ele.get_bezeichnung() + str;
			}
			break;

		// Jetzt sonstiges
		case LEER:
			//throw "Bitte Rohrqualität auswählen!";
			break;
		default:
			str = getStammdaten().getText("IDS_ERROR41").c_str();  //Unbekannte Rohrqualität
			XASSERT(err == str); // Ist false, Meldung wird also angezeigt
			throw str;
			break;
	} 
}

///Rohranfang, Rohrende und Rohrverbindung checken
void Rohrleitung::check_rohrAnfangEndeVerb() const
{
	//todo: getLastRohrDN() statt dnvl bei rohrende
	CString err, str;

	if (rohrVerb == LEER) 
	{ 
		err = getStammdaten().getText("IDS_ERROR87").c_str();  //Bitte Verbindung auswählen!
		throw err;
	}
	if (rohrAnfang == LEER || rohrEnde == LEER) 
	{ 
		err = getStammdaten().getText("IDS_ERROR88").c_str();  //Bitte Anfang und Ende auswählen!
		throw err;
	}

	if (rohrQuali == RQ2458GSCH && (rohrAnfang == GEW || rohrEnde == GEW)) 
	{
		err = getStammdaten().getText("IDS_ERROR89").c_str();  //Gewinde ist bei dieser Rohrqualität und Nennweite nicht möglich. Bitte andere Rohrqualität auswählen\noder als Element NUT wählen und über SAP ein Verbindungsstück Nut/Gewinde bestellen!
			throw err;
	} 

	check_AnfangEnde_ScheibeOk();
	
	struct
	{
		int verbArt;
		bool checkrohrverb;
		int min_dnvl;
		int max_dnvl;
	} verb_check[] = 
	{
		NUT,		true,	25,		400,
		GEW,		false,	15,		65,
		FFL,		true,	32,		400,
		VFL16,		true,	32,		400,
		VFL10,		true,	200,	400,
		AFL,		true,	40,		400,
		SFL,		true,	40,		400,
		BOD,		false,	40,		400,
		KB40,		false,	50,		200,
		KB50,		false,	65,		200,
		0,			false,	0,		0,
	};

	for (int i = 0; verb_check[i].verbArt; i++)
	{
		if (rohrAnfang == verb_check[i].verbArt || (verb_check[i].checkrohrverb && rohrVerb == verb_check[i].verbArt))
		{
			if (dnvl < verb_check[i].min_dnvl || dnvl > verb_check[i].max_dnvl)
			{
				if (verb_check[i].verbArt == VFL10)
					str = getStammdaten().getText("IDS_ERROR21").c_str();  //Bei PN10 und DN 32-150 ist der Flachflansch Standard.
				else
					str = getStammdaten().getText("IDS_ERROR90").c_str();  //"Bei %s muss DN zwischen %i und %i sein!

				err.Format(str, getStammdaten().getBaugruppenBezeichnung(verb_check[i].verbArt), verb_check[i].min_dnvl, verb_check[i].max_dnvl);
				throw err;
			}
		}
		if (rohrEnde == verb_check[i].verbArt)
		{
			if (getLastRohrDN() < verb_check[i].min_dnvl || getLastRohrDN() > verb_check[i].max_dnvl)
			{
				if (verb_check[i].verbArt == VFL10)
					str = getStammdaten().getText("IDS_ERROR21").c_str();  //Bei PN10 und DN 32-150 ist der Flachflansch Standard.
				else
					str = getStammdaten().getText("IDS_ERROR90").c_str();  //"Bei %s muss DN zwischen %i und %i sein!

				err.Format(str, getStammdaten().getBaugruppenBezeichnung(verb_check[i].verbArt), verb_check[i].min_dnvl, verb_check[i].max_dnvl);
				throw err;
			}
		}
	}
}

//Rohr DN checken
void Rohrleitung::check_dnvl() const
{
	CString err, str;
	if (dnvl !=  25 && dnvl !=  32 && dnvl !=  40 && dnvl !=  50
	 && dnvl !=  65 && dnvl !=  80 && dnvl != 100 && dnvl != 125 
	 && dnvl != 150 && dnvl != 200 && dnvl != 250 && dnvl != 300 && dnvl != 350 && dnvl != 400 
	 && dnvl !=  15 && dnvl !=  20) 
	{ 
		str = getStammdaten().getText("IDS_ERROR91").c_str();  //Rohrnennweite DN %d ungültig!
		err.Format(str, dnvl);
		throw err;
	}
}

void Rohrleitung::check_posnr() const
{
	CString err, str;
	if (get_posNr().GetLength() > 5)
	{
		str = getStammdaten().getText("IDS_POSLAENGE").c_str();  //Die Positionsnummer darf max. 5 Zeichnen enthalten
		throw str;
	}

	for (int i=0; i < get_posNr().GetLength(); i++)
	{
		char testc(get_posNr().GetAt(i));
		if (testc >= '0' && testc <= '9' 
		 //||	testc >= 'a' && testc <= 'z'   //Posnr darf wegen Sägenutanlage nur noch Gro0buchstaben enthalten
		 || testc >= 'A' && testc <= 'Z'
		 || testc == '.' || testc == '-')
		{
			continue;
		}
		else
		{
			str = getStammdaten().getText("IDS_ERROR92").c_str();  //Posnr. darf nur die Zeichen '0'-'9', 'A'-'Z', '.' und '-' enthalten. '%c' ist nicht erlaubt
			err.Format(str, testc);
			throw err;
		}
	}
}

/**
Prüft den Mindestabstand zwischen den Stutzen und Muffen

\param minAbst: Mindesabstand 
\param bAnlage:   bAnlage =true:  Abgang wird über Schweißrohrstraße gefertigt
				          =false: Abgang ist Handschweißung
\param bVonBauteilAussenkante =true: Die Mindesabstände beziehen sich auf Stutzenaußenkante
*/
string Rohrleitung::check_abstandZwischenStutzen(int minAbst, bool bAnlage, bool bVonBauteilAussenkante)
{
	string strx;

	DNzuAussendurchmesser dntoad;	
	int eleAussendurchmesser = 0;
	
	// Elementarray durchgehen	
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		const Element* const ele = elementArray.get_element_at(i);
		if (!ele)
			continue;

		//Wenn Abstände für Anlage geprüft werden, nur Objekte prüfen, die überAnlage laufen
		if (bAnlage && !ele->get_makeAnlage())
			continue;

		strx = str(format(getStammdaten().getText("IDS_FORMAT_ELE_ERROR")) % (LPCSTR)get_posNr() % (i+1));
		
		// Aussendurchmesser 
		eleAussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_dn());

		if (ele->isEleAbgang())
		{	
			Element *e = NULL;
			int index = i-1;
			bool gleicherWinkel = false, redbogen = false, gleicheMuffe = false; 
			// naehesten davorliegenden Abgang mit gleicher Winkelpos. oder Bogen/Red suchen
			// 90° versetzte gleich grosse Muffen sind Sonderfall -> Message
			while (index >= 0)
			{
				e = elementArray.get_element_at(index);
				if (!e)
					continue;

				if (e->isEleAbgang() && e->get_winkel() == ele->get_winkel())
				{
					gleicherWinkel = true; 
					break;
				} 
				else if (e->isEleAbgang() 
					  && (((e->get_winkel() == 90 || e->get_winkel() == 270) && (ele->get_winkel() ==  0 || ele->get_winkel() == 180))
					  ||  ((e->get_winkel() ==  0 || e->get_winkel() == 180) && (ele->get_winkel() == 90 || ele->get_winkel() == 270))))
				{
					if (ele->get_id() == MUFFE && e->get_id() == MUFFE && ele->get_dn() == ele->get_anschlussDN() && ele->get_dn() == e->get_dn())
						gleicheMuffe = true;
				}
				else if (e->isEleRedBog())
				{
					redbogen = true;
					break;
				}

				index--;
			}

			// Element gefunden
			if (e && gleicherWinkel)
			{
				if (bVonBauteilAussenkante)
				{
					int aussendurchmesser = dntoad.getAussendurchmesserToInt(e->get_dn());
					if ((ele->get_abstAbs() - e->get_abstAbs()) < ((int)(eleAussendurchmesser/2) + (int)(aussendurchmesser/2)+minAbst)) 
					{
						//Abstand sollte mindestens die Hälfte der beiden Aussendurchmesser + 50mm = %dmm sein!
						string txt = str(format(getStammdaten().getText("IDS_ERROR103A")) % minAbst % ((int)(eleAussendurchmesser/2)+(int)(aussendurchmesser/2)+minAbst) );
						return strx + txt;
					}
				}
				else
				{
					if ((ele->get_abstAbs() - e->get_abstAbs()) < minAbst) 
					{
						//Der Abstand zwischen den Abgängen sollte min. %dmm betragen.
						string txt = str(format(getStammdaten().getText("IDS_ERROR103B")) % minAbst);
						return strx + txt;
					}
				}
			}

			// Gleiche Muffe um 90° versetzt: geht, aber Schweissnaehte ueberlappen sich, 
			// da der Durchmesser der Muffe groesser ist als der des Rohres. So ist eine
			// Druckprobe erforderlich. Deswegen kein Fehler, aber Hinweis ausgeben.
			else if (e && gleicheMuffe)
			{
				if ((ele->get_abstAbs() - e->get_abstAbs()) < (eleAussendurchmesser + 5)) 
				{
					string txt = str(format(getStammdaten().getText("IDS_HINWEIS_GLEICHE_MUFFE")) % (eleAussendurchmesser+5));
					getErrorHandler().appendErrorDescription(strx + txt, MxError::Information);
				}
			}
		}		
	}
	return "";
}



/**
Prüfen ob Längen für Rohranfang und Rohrende eingehalten werden.
- Dazu erst das Anfangs- und Endelement auf dem Rohrsegment ermitteln
- dann Abstände zum Rohranfang und -ende berechnen. Dabei wird das Bauteil vor dem Rohr (Bogen, Redzuierung, ..)
  berücksichtigt. 

- Wenn proRohr=true, beziehen sich die Abstände auf die 6m Rohrstücke und nicht aufs Gesamtrohr. 
  (Die Rohre werden auf der Verteilerrohrstrasse erst nach der Maschinenschweissung zusammengeschweißt)

\param minAnfang1 Mindesabstand Außenkante Abgang zu Rohranfang
\param minEnde1   Mindesabstand Außenkante Abgang zu Rohrende

\param minAnfang2 Mindesabstand Außenkante Abgang zu Rohranfang für eine größere Dn Größe als bei minAnfang1 (0 wenn keine Überprüfung für zweiten Wert stattfinden soll)
\param dnAnfang2  <= Dn Wert auf dem sich der Mindesabstand bezieht (0 wenn keine Überprüfung für zweiten Wert stattfinden soll)
\param minEnde2   Mindesabstand Außenkante Abgang zu Rohrende für eine größere Dn Größe als bei minEnde1(0 wenn keine Überprüfung für zweiten Wert stattfinden soll)
\param dnAnfang2  <= Dn Wert auf dem sich der Mindesabstand bezieht (0 wenn keine Überprüfung für zweiten Wert stattfinden soll)

\param bAnlage   bAnlage =true:  Abgang wird über Schweißrohrstraße gefertigt
				         =false: Abgang ist Handschweißung
\param bVonBauteilAussenkante =true: Die Mindesabstände beziehen sich auf Stutzenaußenkannte
*/
string Rohrleitung::check_abstandRohranfangEnde(int minAnfang1,int minEnde1,
												 int minAnfang2,int dnAnfang2,
												 int minEnde2,int dnEnde2,
												 bool bAnlage, bool proRohr, bool bVonBauteilAussenkante)
{

	string		err;

	Element		*elPrevRohr		  = NULL, //Bauteil vor dem Rohr (z.B. Boden, Flansch, Bogen, ...)
		        *elPrevRohrMerken = NULL; 

	int			rohrLaenge = 0,
		        prevRohrLaenge = 0;

	CObList		listeAnbauteile;
				

	//Elementliste des Bauteils durchlaufen
	for (POSITION pos = get_elementList().GetHeadPosition(); pos != NULL;)	
	{
		

		Element* ele = dynamic_cast<Element*>(get_elementList().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;


		if (ele->isEleRohr())
		{
			//Rohrlänge speichern
			rohrLaenge += ele->get_laenge();
			
			//Wenn Rohr keine Elemente hat zum nächsten Element springen
			if (ele->get_uelementList().GetSize() <= 0)
				continue;


			//Unterelementlisten speichern
			listeAnbauteile.AddTail(&ele->get_uelementList());


			//Wenn es ein Rohr war, immer mit nächsten Element weitermachen. Damit bei einem Rohr mit Schweißnaht,
			//die anderen Elemente auch gespeichert werden. 
			if (pos && proRohr == false)
				continue;	
		}
		else //Wenn Element kein Rohr ist, evt. Bauteil vor Rohr zwischenspeichern
		{
			if (ele->get_id() == 0 || ele->get_id() == SCHWEISS)
				continue;

			//Bauteil vor dem Rohr zwischenspeichern
			if (elPrevRohr)
				elPrevRohrMerken = ele;
			else
				elPrevRohr = elPrevRohrMerken = ele;		
		}


		Element *elFirst = NULL, //1. Element auf Rohr(segment)
				*elLast  = NULL; //letztes Element auf Rohr(segment)

		//Unterelemente des Rohres durchlaufen
		for (POSITION upos = listeAnbauteile.GetHeadPosition(); upos != NULL;)	
		{
			Element* uele = dynamic_cast<Element*>(listeAnbauteile.GetNext(upos));
			XASSERT(uele);
			if (!uele)
				break;

			//nur Unterelemente prüfen, die entweder über die Schweißstraße laufen oder Handschweißung sind
			if (uele->get_makeAnlage() != bAnlage)
				continue;

			//1. Element auf Rohr(segment) speichern
			if (elFirst == NULL)
				elFirst = uele;

			//letztes Element auf Rohr(segemnt) speichern
			elLast = uele;
		}
		
		//Wenn Abgänge bAnlage ensprachen prüfen ob Abstände eingehalten werden
		if (elFirst && elLast)
		{

			//Abstände und Rohrlängen

			int abstandAbsElFirst = elFirst->get_abstAbs(),
				abstandAbsElLast  = elLast->get_abstAbs();

			if (proRohr)
			{
				abstandAbsElFirst -= prevRohrLaenge;
				abstandAbsElLast  -= prevRohrLaenge;
			}


			int laengeElVorRohr = 0,
				abstandAbsElVorRohr = 0;

			//Bauteil vor Rohr berücksichtigen
			if (elPrevRohr)
			{
				laengeElVorRohr     = elPrevRohr->get_laenge();  //genaue Rohrlänge
				abstandAbsElVorRohr = elPrevRohr->get_abstAbs(); //Absoluter Abstand zum Anfang der Position 
			}

			int min_abstand_ende   = minEnde1,
				min_abstand_anfang = minAnfang1;


			//Unterschiede bei Dn Größe beachten. 
			if (minAnfang2 > 0 && dnAnfang2 > 0)
			{
				if (elFirst->get_dn() >= dnAnfang2 || elLast->get_dn() >= dnAnfang2)
					min_abstand_anfang = minAnfang2;
			}

			if (minEnde2 > 0 && dnEnde2 > 0)
			{
				if (elFirst->get_dn() >= dnEnde2 || elLast->get_dn() >= dnEnde2)
					min_abstand_ende = minEnde2;
			}

			//Durchmesser der Abgänge beachten. (Mindesmaße beziehen sich auch Stutzenaußenkante)
			int stutzenRadiusFirst = 0,
				stutzenRadiusLast = 0;
			if (bVonBauteilAussenkante)
			{
				DNzuAussendurchmesser dntoad;
				stutzenRadiusFirst = int(dntoad.getAussendurchmesserToInt(elFirst->get_dn()) / 2.0);
				stutzenRadiusLast = int(dntoad.getAussendurchmesserToInt(elLast->get_dn()) / 2.0);
			}

			//genauen Abstand zu Rohranfang und Rohrende berechnen. (Bauteil vor dem Rohr wird dabei berücksichtigt)
			abstandAbsElFirst = abstandAbsElFirst - laengeElVorRohr - abstandAbsElVorRohr - stutzenRadiusFirst;
			abstandAbsElLast  = rohrLaenge - (abstandAbsElLast - abstandAbsElVorRohr - laengeElVorRohr + stutzenRadiusLast) ;

			//Anfangs und Endabstände prüfen 
			if (abstandAbsElFirst < min_abstand_anfang || abstandAbsElLast < min_abstand_ende)
			{
				//Prüfen ob Rohr gedreht Bedingungen erfüllt
				if (abstandAbsElFirst < min_abstand_ende   || abstandAbsElLast < min_abstand_anfang)
				{
					string errid = "IDS_ERROR144"; //Der Abstand von Mitte Abgang zum Rohranfang (min. %d mm) bzw. -ende (min. %d mm) ist zu klein!
					if (bVonBauteilAussenkante)
						errid = "IDS_ERROR141"; //Der Abstand von Außenkante Abgang zum Rohranfang (%d mm) bzw. -ende (%d mm) ist zu klein!
					err = str(format(getStammdaten().getText(errid)) % min_abstand_anfang % min_abstand_ende);
					return err;
				}
			}
		}

		//evt. zu berücksichtigendes Bauteil vor Rohr setzten.(z.B. Bogen, Reduzierung)
		elPrevRohr		 = elPrevRohrMerken;
		elPrevRohrMerken = NULL;

		//Rohrlänge zurücksetzen, da nächstes Rohrstück bearbeitet wird
	    prevRohrLaenge += rohrLaenge; 
		rohrLaenge = 0;
		listeAnbauteile.RemoveAll();
	}
	return err;
}



/**
Allgemeine Prüfungen für Elemente auf dem Rohr durchführen. 
Achtung: Prüfungen gelten für alle Rohre

Folgende Prüfungen erfolgen:
- Elementabstand zu Rohranfang u. -ende
- Abstand zwischen den Elementen
- Verkettung wird geprüft
- Länge Stutzen
- Winkel der Abgänge
- Abschlußelemente
*//*
Änderungen:
07.04.06, Jacobs: Bedingungen für die Prüfung Elementabstand zu Rohranfang u. -ende überarbeitet
*/
void Rohrleitung::check_elementArray()
{
	string err, strx, txt;
	bool is_av = WaxSys::isAV(); 

	/*
	Prüfen ob Längen für Rohranfang und Rohrende eingehalten werden. (Handschweißung). 
	Achtung: Die Prüfungen für die Abstände für die Schweißstraßen sollten vor diesem Funtionsaufruf stattfinden.
	- Die Mindesabstände beziehen sich auf Stutzenaußenkante. (Dies wird berücksichtig wenn bStutzenAussenKante = true ist).
	*/

	//Modell aus den Eingabedaten erzeugen (elementList wird neu gesetzt)
	createModell();


	err = check_abstandRohranfangEnde(A_MIN_ABSTAND_ANFANG,A_MIN_ABSTAND_ENDE,0,0,0,0,false, false, true);

	if (!err.empty())
	{
		if (is_av)
			getErrorHandler().appendErrorDescription(err, MxError::Information);
		else
			throw (CString)err.c_str();
	}

	err = check_abstandZwischenStutzen(A_MIN_ZWISCHEN_STUTZEN,0,true);

	if (!err.empty())
	{
		if (is_av)
			getErrorHandler().appendErrorDescription(err, MxError::Information);
			
		else
			throw (CString)err.c_str();
	}


	// bei Strang etc. Elementarray mit Standardelementen und AVElementen fuellen
	fill_elementArray();


	// Elementarray durchgehen	
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		txt = getStammdaten().getText("IDS_FORMAT_ELE_ERROR");
		strx = str(format(txt) % get_posNr() % (i+1));
		
		Element *ele = elementArray.get_element_at(i);
		// vorheriges & naechstes fuer diverse Abfragen 
		Element *prev = i > 0 ? elementArray.get_element_at(i-1) : NULL;
		Element *next = i < (elementArray.GetSize()-1) ? elementArray.get_element_at(i+1) : NULL;

		// Aussendurchmesser holen fuer diverse Abfragen zum Abstand
		DNzuAussendurchmesser dntoad;
		int anschlussAussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_anschlussDN());
	

// Relativen Abstand prüfen
		if (ele->get_abstRel() < 0)
		{
			err = getStammdaten().getText("IDS_ERROR93"); //Negativer Abstand nicht möglich!
			throw (CString)(strx + err).c_str();
		}

		if (ele->get_abstRel() == 0 && !ele->isEleRed())
		{
			
			if (!prev || ele->get_winkel() == prev->get_winkel() && !ele->isEleAbgEnde()
				      || ele->isEleBogen())
			{
				if(!prev)
				{
					err = getStammdaten().getText("IDS_FEHLER_GLEICHER_WINKEL_SELBE_POS"); //Relativer Abstand 0 nicht zulässig!
					throw (CString)(strx + err).c_str();
				}
				else
				{
					if(ele->get_id() != SPRINKLERSCHELLE_VIROTEC && prev->get_id() != SPRINKLERSCHELLE_VIROTEC)
					{
						err = getStammdaten().getText("IDS_FEHLER_GLEICHER_WINKEL_SELBE_POS"); //Relativer Abstand 0 nicht zulässig!
						throw (CString)(strx + err).c_str();
					}
				}

			}
		}
		// DN checken
		err = ele->check_dn();
		if (err.length() > 0)
			throw (CString)(strx + err).c_str();
		
// Verkettung bei erstem Element überprüfen
		if (!i && ele->get_verkettung() == 1)
		{
			err = getStammdaten().getText("IDS_ERROR95"); //Erstes Element kann nicht verkettet sein!
			throw (CString)(strx + err).c_str();
		}

// Quali mit neuer DN checken
		if (ele->isEleRedEinz())
		{
			check_rohrQuali(ele->get_dn());
		}

// Absoluten Abstand vom Anfang und Ende überprüfen
		check_absAbst_rohrlaenge(ele);
		
// Laenge vom Rohrstutzen checken
		if (ele->get_id() == ROHR) 
		{
			
			// min. Laenge Abgang
			const int zuschlag = 50; int laenge = ele->get_laenge();
			if (next && next->isEleAbgEnde())
				laenge -= next->get_laenge();
			if (laenge < (anschlussAussendurchmesser/2 + zuschlag))
			{
				string txt = getStammdaten().getText("IDS_ERROR96");  //Länge vom Rohrstutzen muss min. 1/2*Rohrdurchmesser+Länge Flansch+50mm=%dmm sein!
				if (next)
					err = str(format(txt) % (anschlussAussendurchmesser/2 + zuschlag + next->get_laenge()));
				else
					err = str(format(txt) % (anschlussAussendurchmesser/2 + zuschlag));
				throw (CString)(strx + err).c_str();
			}
		}

// Winkel checken
		if (ele->get_winkel() < 0 && ele->get_winkel() > 359)  
		{
			err = getStammdaten().getText("IDS_ERROR97"); //Winkel der Abgänge kleiner 0° und grösser 359° ungültig!
			throw (CString)(strx + err).c_str();
		}


		// Abgang fuer Abschlusselement checken
		if (ele->isEleAbgEnde()) 
		{
			if (i == 0) 
			{
				err = getStammdaten().getText("IDS_ERROR98"); //Abschlusselement nicht direkt in Rohrleitung einfügbar!
				throw (CString)(strx + err).c_str();
			} 
			else if (prev)
			{
				if (prev->isEleAbgEnde()) 
				{
					err = getStammdaten().getText("IDS_ERROR99"); //Vorheriges Element war schon ein Abschlusselement!
					throw (CString)(strx + err).c_str();
				} 
				else if (prev->get_id() != ROHR && prev->get_id() != STDSTUTZEN) 
				{
					err = getStammdaten().getText("IDS_ERROR100"); //Vorheriges Element muss bei Abschlusselement ein Rohrstutzen sein!
					throw (CString)(strx + err).c_str();
				}
				else if (ele->get_winkel() != prev->get_winkel())
				{
					err = getStammdaten().getText("IDS_ERROR101");  //Rohrstutzen muss gleichen Winkel haben wie Abschlusselement!
					throw (CString)(strx + err).c_str();
				}
				else if (ele->get_abstRel() != 0)
				{
					err = getStammdaten().getText("IDS_ERROR102"); //Der relative Abstand muss 0 sein!
					throw (CString)(strx + err).c_str();
				}
			}
		}

		if (ele->get_id() == ROHRNIPPEL) 
		{
			if (ele->get_dn() != 15)
			{
				err = getStammdaten().getText("IDS_ERROR_ROHRNIPPEL_DN"); //Bei Rohrnippel ist nur DN 15 zulässig!
				throw (CString)(strx + err).c_str();
			}
		}

	}
}

EnCObListVk3& Rohrleitung::get_pulverlaengenListe() 
{
	return pulverlaengenListe;
}


const EnCObListVk3& Rohrleitung::get_pulverlaengenListe() const 
{
	return pulverlaengenListe;
}

EnCObListVk3& Rohrleitung::get_passlaengenListe() 
{
	return passlaengenListe;
}

const EnCObListVk3& Rohrleitung::get_passlaengenListe() const
{
	return passlaengenListe;
}


///Erzeugt die VK3-Elemente aus der Elementliste
bool Rohrleitung::createVk3Liste()
{
	// erstmal alte loeschen
	vk3elementeListe.deleteAllElemente();


	// Liste mit Elementen durchgehen
	for (POSITION pos = elementList.GetHeadPosition(); pos;)
	{
		Element *e = dynamic_cast<Element *>(elementList.GetNext(pos));
		XASSERT(e);
		if (!e)
			break;

		if (e->get_id() == GLATT || e->get_id() == LEER)
			continue;

		//Bei Rohrquali RQ2440VZIN werden die Nuten nicht berechnet, da es Rohre mit Nuten sind
		//ToDo: besser wäre, ein Schalter in den Stammdaten
		if (e->get_id() == NUT && (get_rohrQuali() == RQ2440VZIN || get_rohrQuali() == RQEN10217VERZ))
			continue;

		if (e->isEleRohr() && e->get_laenge() <= 0)
			continue;

		//TRACE("Element ID: %d\n",e->get_id());

		Element* ele = new Element;
		ele->copy(*e);

		if (ele->isEleRohr())
		{
			// grundiert, 
			if (get_beschichtung().isGrundiert()) 
				ele->set_id(ele->get_id() + 600);		
		}


		vk3elementeListe.AddTail(ele);

		CObList& uelementList = ele->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			// Element in Liste
			Element *e = new Element;
			e->copy(*uelement);


			if (e->isKnick())
			{
				if (e->get_durchmesser() > 45)
					e->set_id(Element::KNICK90);
				else if (e->get_durchmesser() > 15)
					e->set_id(Element::KNICK45);
				else
					e->set_id(Element::KNICK15);
			}


			vk3elementeListe.AddTail(e);

			CObList& uuelementList = uelement->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)
			{
				Element *uuelement = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuelement);
				if (!uuelement)
					break;

				// Element in Liste
				Element *e = new Element;
				e->copy(*uuelement);
				vk3elementeListe.AddTail(e);
			}
		}
	}



	// gleiche Elemente zusammenfassen
	vk3elementeListe.kumulieren();
	//Menge hochsetzen
	vk3elementeListe.setMenge(get_anzahl());

	// zuletzt noch Preise setzen
	vk3elementeListe.setVk3Preise(get_beschichtung().isVerzinkt(), 0, 0, 0);


#ifdef _DEBUG
//afxDump << get_vk3elementeListe();
#endif
	return true;
}




void Rohrleitung::createPasslaengenListe()
{
	// erstmal alte loeschen
	passlaengenListe.deleteAllElemente();

	Element *e = NULL;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		e = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(e);
		if (!e)
			return;

		if (e->isEleRohr())
			break;
	}

	// Arrays mit Rohrinfos  
	for (int i=0; i<laengeProNr.GetSize(); i++)
	{
		if (laengeProNr.GetAt(i) >= 500 && laengeProNr.GetAt(i) != 6000)
		{
			Element* ele = new Element;
			ele->set_anzahl(get_anzahl());
			ele->set_dn(dnProNr.GetAt(i));
			ele->set_anschlussDN(dnProNr.GetAt(i));
			ele->set_id(e->get_id()+500);

			passlaengenListe.AddTail(ele);
		}
	}

	// zuletzt noch Preise setzen
	passlaengenListe.setVk3Preise(get_beschichtung().isVerzinkt(), 0, 0, 0);

	
}

/**
Vk3 Elemente für Pulvern. beim Pulvern werden erst die einzelnen Rohrlängen addiert und am Ende
auf volle Meter gerundet.
*/
void Rohrleitung::createPulverlaengenListe()
{
	
	// erstmal alte loeschen
	pulverlaengenListe.deleteAllElemente();
	
	if (!get_beschichtung().isGepulvert())
		return;

	// Arrays mit Rohrinfos  
	for (int i=0; i<laengeProNr.GetSize(); i++)
	{
		Element* ele = new Element;

		

		ele->set_laenge(laengeProNr.GetAt(i)*get_anzahl());

		ele->set_dn(dnProNr.GetAt(i));
		ele->set_anschlussDN(dnProNr.GetAt(i));
		if (stutProNr.GetAt(i))
			ele->set_id(701);
		else
			ele->set_id(700);

		pulverlaengenListe.AddTail(ele);
	}

	// zuletzt noch Preise setzen
	pulverlaengenListe.setVk3Preise(false, 0, 0, 0);
}



bool Rohrleitung::hasRohr2Quali() const
{
	if (rohrQuali != RQ2440_2458GSCH)
		return false;

	bool dn50 = false;
	bool dn65 = false;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->get_id() == ROHR_Q1)
		{
			if (ele->get_dn() <= 50)
				dn50 = true;
			else
				dn65 = true;
		}
		if (dn50 && dn65)
			return true;
	}
	return false;
}

void Rohrleitung::sortVk3Listen(CObList &liste)
{
	POSITION pos, nextpos; 
   
	for (int i=0; i < liste.GetCount()-1; i++)	
	{
		pos = liste.FindIndex(i);
		Element *first = dynamic_cast<Element *>(liste.GetAt(pos));
		nextpos = liste.FindIndex(i+1);
		Element *second = dynamic_cast<Element *>(liste.GetAt(nextpos));

		XASSERT(first && second);
		if (!first || !second)
			return;
		if (first->get_id() > second->get_id() || (first->get_id() == second->get_id() && first->get_dn() > second->get_dn()))
		{	
			// Elemente werden vertauscht
			liste.SetAt(nextpos, first);
			liste.SetAt(pos, second);
			i = -1; // Wieder von vorn beginnen
		}
	}
}

///Prueft ob Abgaenge da sind, die geschweisst werden muessen
const bool Rohrleitung::isSchweissen() const
{
	for (int i=0; i<get_stdelementArray().GetSize(); i++)
	{
		const Element* const ele = get_stdelementArray().get_element_at(i);
		if (!ele)
			break;

		if (ele->get_id() != SPRINKLERSCHELLE_VIROTEC && ele->get_id() != BOHR24 && ele->get_id() != BOHR50) 
			return true;
	}
	for (int i=0; i<get_elementArray().GetSize(); i++)
	{
		const Element* const ele = get_elementArray().get_element_at(i);
		if (!ele)
			break;
		
		if (ele->get_id() != SPRINKLERSCHELLE_VIROTEC && ele->get_id() != BOHR24 && ele->get_id() != BOHR50) 
			return true;
	}
	return false;
}

///Prueft ob Rohrkostruktion andere Elemente ausser Anfang/Ende/Rohr hat
const bool Rohrleitung::isRohrGlatt() const
{
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRedBogEinz())
			return false;

		if (!ele->get_uelementList().IsEmpty())
			return false;
	}
	return true;
}

///Ist ein Bogen in der Konstruktion?
bool Rohrleitung::isBogen() const
{
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *element = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleBogen()) 
			return true;
	} 
	return false;
}


int Rohrleitung::CountBoegen() const
{
	int iWieviele = 0;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *element = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleBogen()) 
			iWieviele++;
	} 
	return iWieviele;
}


///Ist ein Bogen in der Konstruktion?
const bool Rohrleitung::isRedBogEinz()
{
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *element = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRedBogEinz()) 
			return true;
	} 
	return false;
}

///Ist eine Reduzierung in der Konstruktion?
const bool Rohrleitung::isRed() const
{
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *element = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRedEinz()) 
			return true;
	} 
	return false;
}

///Hat das Rohr einen Knick
const Element* Rohrleitung::getKnick() const
{
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;
		
		if (ele->isEleRohr())
		{
			for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
			{
				const Element *uele = dynamic_cast<const Element*>(ele->get_uelementList().GetNext(upos));
				XASSERT(uele);
				if (!uele)
					break;

				if (uele->isKnick())
					return uele;
			}
		}
	}
	return NULL;
}

const bool Rohrleitung::get_ruesten(int index) const
{
	return ruesten[index];
}

void Rohrleitung::set_ruesten(int index, bool value)
{
	ruesten[index] = value;
}

CString Rohrleitung::get_stdfertigungshinweis() const
{
	/**
	Bei Nicht-Standardwinkel wird ein Hinweistext zurückgeliefert
	*/
	CString ret;
	if (hasNonStandardWinkel())
		ret = getStammdaten().getText("IDS_FERTHINWEIS1").c_str(); 
	return ret;
}




/*void Rohrleitung::set_stdfertigungshinweis(const CString& value)
{
	stdfertigungshinweis = value;
}
*/
/*
///Generiert den Standardfertigungstext
void Rohrleitung::setFertigungshinweis()
{
	stdfertigungshinweis = "";
	for (int i=0; i<elementArray.GetSize(); i++)
	{
		Element *ele = elementArray.get_element_at(i);
		if (!ele)
			break;
	
		if (ele->get_winkel() != 0 && ele->get_winkel() != 90 && ele->get_winkel() != 180 && ele->get_winkel() != 270)
		{
			stdfertigungshinweis = getStammdaten().getText("IDS_FERTHINWEIS1);
			break;
		}
	}
}
*/

bool Rohrleitung::hasNonStandardWinkel() const
{
	/**
	Liefert true, wenn irgendein Winkel ausser 0,90,180 oder 270 gefunden wurde
	*/
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		const Element * const ele = elementArray.get_element_at(i);
		if (!ele)
			continue;
	
		if (ele->get_winkel() != 0 
			&& ele->get_winkel() != 90 
			&& ele->get_winkel() != 180 
			&& ele->get_winkel() != 270)
		{
			return true;
		}
	}
	return false;
}

const CArray<int, int>& Rohrleitung::get_radien() const
{
	return radien;
}

///Ermittelt die Kranzeit fuer die Afos 16/17 (Vorrichten und Schweissen)
double Rohrleitung::getKranzeit(bool mitVerlaengerung)
{
	///Ermittelt die Anzahl der Rohre nach Nennweiten fuer Kranzeit
	setAnzahlNennweiten(mitVerlaengerung);

	///Kranzeit berechnen
	return calcKranzeit(anzahlNennweiten);
}

///Ermittelt die Anzahl der Rohre nach Nennweiten fuer Kranzeit
void Rohrleitung::setAnzahlNennweiten(bool mitVerlaengerung)
{
	UNREFERENCED_PARAMETER(mitVerlaengerung);
	
		///erstmal alte loeschen
	anzahlNennweiten.RemoveAll();
	
	///initialisieren fuer DN 25-65, 80-125, 150-200, 250-400
	for (int i=0; i<4; i++)
		anzahlNennweiten.Add(0);

	///neu setzen
	set_anzahlNennweiten(get_dnvl(), get_laenge(), getGesGewicht());
	
	/**
	///Hier wird jedes Rohstueck ausgewertet, aber so erstmal nicht 
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr()) 
		{
			if (ele->get_dn() <  65 && ele->get_laenge() < 1500
			||  ele->get_dn() >= 65 && ele->get_laenge() <  500)
			continue;
			  
			if (ele->get_dn() < 80)
				anzahlNennweiten.SetAt(0, anzahlNennweiten.GetAt(0) + get_anzahl());
			else if (ele->get_dn() < 150)
				anzahlNennweiten.SetAt(1, anzahlNennweiten.GetAt(1) + get_anzahl());
			else if (ele->get_dn() < 250)
				anzahlNennweiten.SetAt(2, anzahlNennweiten.GetAt(2) + get_anzahl());
			else 
				anzahlNennweiten.SetAt(3, anzahlNennweiten.GetAt(3) + get_anzahl());
		}
	}
	*/
}

void Rohrleitung::set_anzahlNennweiten(int dn, int laenge, double gewicht, bool mitVerlaengerung)
{
	if (mitVerlaengerung) // && gewicht < 30)
	{
		if (dn >= 65 && dn <= 100 && laenge < 3000
	  				 || dn == 125 && laenge < 2500
				     || dn == 150 && laenge < 1800
				     || dn == 200 && laenge < 1500
				     || dn == 250 && laenge <  900
				     || dn == 300 && laenge <  700
					 || dn <   65  && laenge < 3000)
			return;
	}
	else
	{
		if (gewicht < 40.0 && (dn < 65 && laenge < 1500 || dn >= 65 && laenge < 500))
			return;
	}

	if (dn < 80)
		anzahlNennweiten.SetAt(0, anzahlNennweiten.GetAt(0) + get_anzahl());
	else if (dn < 150)
		anzahlNennweiten.SetAt(1, anzahlNennweiten.GetAt(1) + get_anzahl());
	else if (dn < 250)
		anzahlNennweiten.SetAt(2, anzahlNennweiten.GetAt(2) + get_anzahl());
	else 
		anzahlNennweiten.SetAt(3, anzahlNennweiten.GetAt(3) + get_anzahl());
}

///Kranzeit berechnen
double Rohrleitung::calcKranzeit(CUIntArray& anznennw)
{
	int anzahlTransporte = 0;
	Krantaetigkeit kt;

	for (int i = Krantaetigkeit::DN25_65; i <= Krantaetigkeit::DN250_400; i++)
	{
		int n_rohre_pro_transport = (int)kt.getWert(i);
		anzahlTransporte += anznennw.GetAt(i) / n_rohre_pro_transport;
		if (anznennw.GetAt(i) % n_rohre_pro_transport)
			anzahlTransporte++;
	}
	return anzahlTransporte * kt.getWert(Krantaetigkeit::ZEIT_PRO_TRANSPORT); 
}

///get anzahlNennweiten
int Rohrleitung::get_anzahlNennweiten(int index)
{
	if (anzahlNennweiten.GetSize() == 0)
		return 0;
	return anzahlNennweiten.GetAt(index);
}


///Liefert den ersten Abgang der Rohrleitung
const Element* Rohrleitung::getFirstAbgang() const
{
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr())
		{
			if (!ele->get_uelementList().IsEmpty())
				return dynamic_cast<const Element*>(ele->get_uelementList().GetHead());
		}
	}
	return NULL;
}

///Liefert den letzten Abgang der Rohrleitung
const Element* Rohrleitung::getLastAbgang() const
{
	const Element* last = NULL;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr())
		{
			if (!ele->get_uelementList().IsEmpty())
				last = dynamic_cast<const Element*>(ele->get_uelementList().GetTail());
		}
	}
	return last;
}


///Liefert die maximal Anzahl der Standardelemente
const int Rohrleitung::getMaxAnzahlStdelemente() const
{
	// Muss in abgeleiteten Klassen ueberschrieben oder darf nicht aufgerufen werden
	return (const int)0;
}

///Liefert den relativen Abstand eines Standardelementes zum vorherigen Standardelement
const int Rohrleitung::getMuffenRelAbst(int index) const
{
	// Muss in abgeleiteten Klassen ueberschrieben oder darf nicht aufgerufen werden
	UNREFERENCED_PARAMETER(index);
	return (const int)0;
}


///Liefert den Restabstand des letzten Standardelementes zum Rohrende
const int Rohrleitung::getMuffenRestabstand() const
{
	// Muss in abgeleiteten Klassen ueberschrieben oder darf nicht aufgerufen werden
	return (const int)0;
}

const CString Rohrleitung::getBemassungstext() const
{
	return "";
}

///Liefert die Anzahl der Rohrleitungen
int Rohrleitung::getAnzDefLaengen()
{
	// Wird in Verteilerleitung ueberschrieben
	// Standardmaessig besteht die Konstruktion aus einer Rohrleitung 	
	return (int)1;
}

CString Rohrleitung::checkRohrlaenge(int value, Element* ele)
{
	CString err, str;
	if (value < 0)
	{
		str = getStammdaten().getText("IDS_ERROR104").c_str(); 
		err.Format(str, ele->get_bezeichnung(), ele->get_abstRel()); 
	}
	return err;
}




///Erzeugen der Afos fuers Anliefern
void Rohrleitung::createAnliefAfos(Arbeitsplan& plan)
{
	plan.addAfo_Anlieferung(get_beschichtung().isVerzinkt());
}




void Rohrleitung::reset_rohrlaengen_und_ids()
{
	memset(rohrLaengen, 0, sizeof(rohrLaengen));
	memset(rohrID, 0, sizeof(rohrID));
}


/**
Zu der Rohrquali ID und dem Rohrdurchmesser den Rohrqualikurztext ausgeben
*/
CString Rohrleitung::getRohrQualikurztext()
{
	return getRohrQualikurztext(get_rohrQuali(), get_dnvl());
}

/**
Zu der Rohrquali ID und dem Rohrdurchmesser den Rohrqualikurztext ausgeben
*/
CString Rohrleitung::getRohrQualikurztext(int rohrQualiId,int dnvl)
{
	CString bezQuali;

	if  ((rohrQualiId == RQ2440_2458GSCH && dnvl < 65) || 
		  rohrQualiId == RQ2440NLOS || 
		  rohrQualiId == RQ2440GSCH)
		bezQuali = "2440";
	else if ((rohrQualiId == RQ2440_2458GSCH && dnvl > 50) || 
		      rohrQualiId == RQ2458GSCH)
		bezQuali = "2458";
	else if  (rohrQualiId == RQ2448NLOS)
		bezQuali = "2448";

	return bezQuali;
}

Rohrleitung::IDS Rohrleitung::elementid2rohrquali(const Element::IDS eleids)
{
	static const struct _elementid2rohrquali
	{
		Element::IDS eleids;
		Rohrleitung::IDS quali;
	} id2q[] =
	{
		// DIN 2440/2458 standard geschweißt normalwandig, eleID = ROHR_Q1
		ROHR_Q1, RQ2440_2458GSCH,
		// DIN 2440 nahtlos, eleID = ROHR_Q3
		ROHR_Q3, RQ2440NLOS,
		// DIN 2448 nahtlos, normalwandig, eleID = ROHR_Q5
		ROHR_Q5, RQ2448NLOS,
		// DIN 2440 verzinkt, eleID = ROHR_Q6
		ROHR_Q6, RQ2440VZIN,
		// DIN 2458 verzinkt, eleID = ROHR_Q7
		ROHR_Q7, RQ2458VZIN,
		// DIN 2458 standard, geschweisst, dickwandig, eleID = ROHR_Q8
		ROHR_Q8, RQ2458DICK,
		// DIN 2458 standard geschweißt normalwandig, eleID = ROHR_Q9
		ROHR_Q9, RQ2458GSCH,
		// DIN 2440 mittelschwer, geschweißt, eleID = ROHR_Q10
		ROHR_Q10, RQ2440GSCH,
		// DIN EN10217-1 APSAD-Rohr, eleID = ROHR_Q11
		ROHR_Q11, RQEN10217,
		//CO2
		ROHR_Q12, RQEN10217_CO2,
		//DIN EN 10217-1 Standard, verzinkt
		ROHR_Q13, RQEN10217VERZ,

		ROHR_Q14, RQEN10255_KALTBAND,

		ROHR_Q15, RQEN10255_KALTBAND_VERZ,

		Element::LEER, LEER,
	};
	
	for (int i = 0; id2q[i].eleids != Element::LEER; i++)
	{
		if (id2q[i].eleids == eleids)
			return id2q[i].quali;
	}
	return LEER;
}


/// Typ: Wasser oder Gas
int Rohrleitung::get_typ() const
{
	int prodlinie = get_leitkarte().get_prj_produktlinie();
	switch (prodlinie)
	{
		case 21:
		case 22:
			return GAS;
		case 0: 
			return LEERTYP;
	}
	return WASSER;
}


int Rohrleitung::getDin()
{
	int din = 0;

	if  ((get_rohrQuali() == Rohrleitung::RQ2440_2458GSCH && get_dnvl() < 65) || get_rohrQuali() == Rohrleitung::RQ2440NLOS || get_rohrQuali() == Rohrleitung::RQ2440GSCH)
		din = 2440;
	else if ((get_rohrQuali() == Rohrleitung::RQ2440_2458GSCH && get_dnvl() > 50) || get_rohrQuali() == Rohrleitung::RQ2458GSCH)
		din = 2458;
	else if  (get_rohrQuali() == Rohrleitung::RQ2448NLOS)
		din = 2448;

	return din;
}

///Prueft ob Verteiler ueber Verteileranlage laeuft
bool Rohrleitung::checkAnlage() const
{
	for (int i=0; i<elementArray.GetSize(); i++)
	{
		const Element* const ele = elementArray.get_element_at(i);
		if (!ele) 
			continue;

		if (ele->get_makeAnlage())
			return true;
	}
	return false;
}


///Erzeugt Elementliste und Stueckliste fuer Sap
void Rohrleitung::makeRohrleitungSap()
{
	///Alles zur Erzeugung der Stueckliste fuer SAP

	// elementArray in Standardelemente und AV-Elemente wandeln
	konvert_elementArraySap();	

	// elementArray sichern und mit elementArraySap fuellen
	EnContainerRohrElemente merk;

	// Elementarray kopieren
	//merk.Copy(elementArray);
	int i;
	for (i=0; i<elementArray.GetSize(); i++)	
	{
		Element* ele = elementArray.get_element_at(i);
		if (ele)
		{	
			Element* e = new Element;
			e->copy(*ele);
			merk.Add(e);
		}
	}
	
	elementArray.deleteAll();
	
	// elementArray mit Sap-Elementen fuellen
	//elementArray.Copy(elementArraySap);
	for (i=0; i<elementArraySap.GetSize(); i++)	
	{
		Element* ele = elementArraySap.get_element_at(i);
		XASSERT(ele);
		if (ele)
		{	
			Element* e = new Element;
			e->copy(*ele);
			elementArray.Add(e);
		}
	}
	
	// Restabstand setzen
	set_restabstand(0);

	// Loeschen aller Elemente aus der Liste
	elementListSap.deleteAllElemente();

	// Laengen für Anfang/Ende/Verb. setzen 
	setAnfaEndeVerbLaenge();

	// Berechnen der Rohrstuecke
	CString err = setRohrlaengen();
	if (err.GetLength())
		return;
	
	// Elementliste fuer Rohrkonstruktion generieren
	if (!makeRohre(elementListSap))
		return; // Schwerer Fehler


	//Rohrstutzen neue Artikelnummer zuweisen			       
	MxCObListIterator iter(elementListSap);
	while (iter.next())
	{
		Element *ele = dynamic_cast<Element*>(elementListSap.getAt(iter));
		XASSERT(ele);
		if (!ele)
			break;

		EnCObListModell &uListe = ele->get_uelementList();

		//In Elementen vom Rohr (hier stecken die Rohrstutzen drin) die Artikelnummer tauschen
		setQualiRohrstutzen(uListe);
	}

	// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen 
	rohreBeschriften(elementListSap);

	// Stueckliste fuer SAP erstellen
	create_stuecklisteSap();

	// Sicherung wieder einspielen	
	elementArray.deleteAll();

	elementArray.copy(merk);
	
	merk.deleteAll();
}

///Elemente von Verteilerelemente in Standardelemente aendern
bool Rohrleitung::konvert_elementArraySap()
{
	Element firstele, merkstd, merkav;
	CString err, str, txt;
	int count = 0;
	int elelaenge = 0; 

	Element *next = NULL;
	Element *prev = NULL;

	// Erstmal alte loeschen
	avelementArray.deleteAll();
	stdelementArray.deleteAll();
	elementArraySap.deleteAll();


	// Verteilerelemente aufteilen, aendern
	for (int i=0; i<get_elementArray().GetSize(); i++)	
	{
		Element *ele = get_elementArray().get_element_at(i);
		if (ele)
		{	
			str = boost::str( boost::format(getStammdaten().getText("IDS_FORMAT_ELE_ERROR")) % ((LPCSTR)get_posNr()) % (i+1)).c_str();

			next = NULL;
			prev = NULL;
			Element *ele = get_elementArray().get_element_at(i);
			if (!ele)
				return false;
			
			if (i > 0)
			{
				prev = get_elementArray().get_element_at(i-1);
				if (!prev)
					break;
			}
			if (i < get_elementArray().GetSize()-1)
			{
				next = get_elementArray().get_element_at(i+1);
				if (!next)
					break;
			}

			// Wenn ueber Anlage, dann Bedingungen pruefen
			// und Werte fuer Standardelemente setzen
			if (ele->get_makeAnlage())
			{
				
				if (merkstd.get_id() == ROHR && merkstd.get_abstAbs() == ele->get_abstAbs())
				{
					if (ele->isEleAbgEnde() && ele->get_id() != NUT && ele->get_id() != GEW && ele->get_id() != GLATT)
					{
						txt = getStammdaten().getText("IDS_ERROR126").c_str();  //Nur Rohrstutzen mit Nut, Glatt oder Gewinde für Anlage möglich!
						err = str + txt;
						break;
					}
				}
				
				if (ele->isEleAbgang() && merkstd.get_abstAbs() == ele->get_abstAbs())
				{
					txt = getStammdaten().getText("IDS_ERROR127").c_str(); //Vorheriges Element für Anlage hat gleiche Position!
					err = str + txt;
					break;
				}

				
				if (ele->get_id() == ROHR && (!next || next && (next->get_id() != NUT && next->get_id() != GLATT && next->get_id() != GEW)))
				{
					txt = getStammdaten().getText("IDS_ERROR126").c_str();  ////Nur Rohrstutzen mit Nut, Glatt oder Gewinde möglich!
					err = str + txt;
					break;
				}
				
	
				//Beim ersten Element fuer Anlage Werte setzen, 
				//die bei allen Standardelemente gleich sind  
				if (count == 0)
				{
					firstele.copy(*ele);
				}

				Element* stdele = new Element(*ele);
				
				//if (ele->get_id() != NUT)
				//	stdele->set_id((ele->get_id() == MUFFE) ? STDMUFFE : STDSTUTZEN);

				stdele->set_urid(ele->get_id());
			
				//Laengenangabe bei Anlage immer mit halben Rohrdurchmesser
				if (ele->get_id() == MUFFE || ele->get_id() == NUTST)
				{
					DNzuAussendurchmesser dntoad;
					int aussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_anschlussDN());
					elelaenge = ele->get_laenge() + aussendurchmesser/2;
				}
				else if (ele->get_id() == ROHR)
				{
					elelaenge = ele->get_laenge();
				}
				else if (ele->get_id() == NUT   ||
						 ele->get_id() == GLATT ||      
						 ele->get_id() == GEW || 
						 ele->get_id() == STDMUFFE || ele->get_id() == STDSTUTZEN)
				{
					;
				}
				else
				{
					// Fehler: generell falscher Typ
					txt = getStammdaten().getText("IDS_ERROR128").c_str(); 
					err = str + txt;
					delete stdele;
					break;	
				}
				stdele->set_laenge(elelaenge);

				stdelementArray.Add(stdele);
				// In Sap-Array mit neuer ID aber altem Abstand
				elementArraySap.Add(new Element(*stdele));

				stdele->set_abstRel(ele->get_abstAbs()-merkstd.get_abstAbs());
				count++;
				merkstd.copy(*ele);
			}
			// Wenn nicht ueber Anlage, dann in Nacharbeitsarray schieben 
			else
			{
				// Wenn Nut nicht als Anlage makiert aber vorheriger Rohrstutzen ueber Anlage, 
				// dann darf Nut nicht ins Nacharbeitsarray
				if (ele->get_id() == NUT) 
				{
					if (prev && prev->get_makeAnlage())
						continue;
				}

				elementArraySap.Add(new Element(*ele));

				// In AV-Array 
				Element* avele = new Element(*ele);
				if (!ele->isEleAbgEnde())
					avele->set_abstRel(ele->get_abstAbs()-merkav.get_abstAbs());

				avelementArray.Add(avele);
				merkav.copy(*ele);
			}
		}
	}
	if (err.GetLength())
	{
		getErrorHandler().appendErrorDescription((LPCSTR)err, MxError::Information);
		
		return false;
	}
	return true;
}

///Generieren der Stueckliste fuer SAP
void Rohrleitung::create_stuecklisteSap()
{
	// Hier werden die Element aus der Elementlist in die Stueckliste gepackt.
	// Standardstutzen von DN80-125 werden auftragsgebunden gefertigt
	
	// Erstmal alte loeschen
	delete_stuecklisteSap();

	// Temporaere Stueckliste generieren
	CObList templist;
	create_templist(templist, elementListSap);

	// Rohrqualitaet bei Rohrstutzen setzen
	setQualiRohrstutzen(templist);

	// Templiste in Stueckliste zusammenfassen
	putTemplistInStueckliste(templist, stuecklisteSap);

	// Temporaere Objekte wieder loeschen
	for (POSITION xpos = templist.GetHeadPosition(); xpos; )
	{
		Element *x = dynamic_cast<Element*>(templist.GetNext(xpos));
		if (x)
			delete x;
	}
	templist.RemoveAll();
}

///Loescht die Objekte und die Pointer aus der Stueckliste
void Rohrleitung::delete_stuecklisteSap()
{
	stuecklisteSap.clear();
}

EnStueckliste& Rohrleitung::get_stuecklisteSap() 
{
	return stuecklisteSap;
}


#ifdef _DEBUG
/*
Ausgabe der verschiedenen Listen. Kann zur Prüfung der Elemente etc. verwendet werden.
*/
void Rohrleitung::Dump(CDumpContext &dc) const
{

	//Werte von elementArray ausgeben
	dc << "\n\nRohrleitung::elementArray: ";
	elementArray.Dump(dc);
	
	//Werte von elementList ausgeben
	dc << "\n\nRohrleitung::elementList: ";
	dc << elementList;

}

#endif


void Rohrleitung::elementeingabe_invertieren()
{
	/**
	Vertauscht die Reihenfolge der Elemente, Rohr wird gedreht
	*/
	int len = get_laenge();

	EnContainerRohrElemente tmp;

	// Anfang und Ende vertauschen
	int rohranfang = get_rohrAnfang();
	set_rohrAnfang(get_rohrEnde());
	set_rohrEnde(rohranfang);

	if (!elementArray.GetCount())
		return;
	
	for (int pos = 0; pos < elementArray.GetCount(); pos++) 
	{
		Element* ele = elementArray.get_element_at(pos);
		if (!ele)
			continue;

		if (ele->get_id() == KRED || ele->get_id() == ERED || ele->get_id() == EINZ)
		{
			set_dnvl(ele->get_dn());
			int dn = ele->get_dn();
			ele->set_dn(ele->get_anschlussDN());
			ele->set_anschlussDN(dn);
			ele->set_abstAbs(len - ele->get_abstAbs() - ele->get_laenge());
		}
		else
		{
			ele->set_abstAbs(len - ele->get_abstAbs());
		}

		// Neuer Winkel:Spiegelung an der Y Achse
		ele->set_winkel(360-ele->get_winkel(true));

		tmp.Add(ele);
	}

	elementArray.RemoveAll(); // Nicht deleteAll!
	// Array umsortieren: Endeelement nach vorne. Reihenfolge für verkettete Elemente beibehalten
	for (int pos = 0; pos < tmp.GetCount(); pos++)
	{
		Element* ele = tmp.get_element_at(pos);
		if (!ele)
			continue;

		Element* elenext = NULL;
		if (pos < tmp.GetCount()-1)
		{
			elenext = tmp.get_element_at(pos+1);
		}
		
		if (elenext && !elenext->get_abstRel())
		{
			elementArray.InsertAt(0, elenext);
			pos++;
		}
		elementArray.InsertAt(0, ele);
	}

	elementArray.calcRelAbst();
	
	tmp.RemoveAll();
}


string Rohrleitung::get_beschriftung(int dn, bool zoll, int rohrcount) const
{
	/**
	Liefert die Beschriftung für die Grafikausgabe
	*/
	string ret;

	// Werden mehrere Rohre ausgegeben, erhält nur Rohrnr 1 die Elementid des Anfangselements
	BauelementGruppen bg;		
	string anfid = rohrcount > 1 ? "" : bg.getKurzbezeichnung(rohrAnfang);//      Element(rohrAnfang).id2str();
	if (!zoll)
		ret = str(format("DN %d %s") % dn % anfid);
	else
		ret = str(format("%s'' %s") % WaxStamm::dn2zoll(dn) %  anfid);
	return ret;
}



int Rohrleitung::getStandardRohrlaenge()
{
	return get_laenge();
}



void Rohrleitung::check_biegung()const
{
	int anzahlKnicke = 2;
	
	int abstPrev = 0;
	int lastKnickAbst = 0;
	int abst = 0;

	Biegung bg;

	long maxWinkel = 0;
	long minWinkel = 0;
	long minAbst = 0;
	long minEleAbst = 0;

	CString err;

	bool hasKnick = false;

	Element startEl, endEl;

	startEl.initStammdaten(dnvl, dnvl, rohrAnfang);
	startEl.initDaten(0, 0, 0, rohrAnfang, 0);
	
	endEl.initStammdaten(getLastRohrDN(), getLastRohrDN(), rohrEnde);
	endEl.initDaten(0, 0, 0, rohrEnde, 0);

	int winkel = 0;

	int countKnicke = 0;
	
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		const Element* const ele = elementArray.get_element_at(i);
		XASSERT(ele);
		if (!ele)
			break;

		

		if (ele->isEleAbgEnde())
			continue;

		//Abstand zum nächsten Element prüfen
		if(hasKnick)
		{
			if(!ele->isKnick())
			{
				abst = ele->get_abstAbs() - abstPrev;
				if (abst < minEleAbst)
				{
					err.Format(getStammdaten().getText("err_biegung4").c_str(), minEleAbst); 
					throw err; //Der Mindestelementabstand l={0}mm wird nicht eingehalten.
				}
				hasKnick = false;
			}
		}

	
		if (!ele->isKnick())
		{	
			abstPrev = ele->get_abstAbs();
			continue;
		}
		countKnicke++;

		if (countKnicke > anzahlKnicke)
		{
			err.Format(getStammdaten().getText("err_biegung5").c_str(), anzahlKnicke);
			throw err; 
		}

		if (countKnicke > 1)
		{
			if (winkel != ele->get_winkel() &&
				winkel + 180 != ele->get_winkel() &&
				winkel - 180 != ele->get_winkel())
			throw getStammdaten().getText("err_biegung6");//Die Biegungen müssen in einer Ebene liegen.
		}


		if (!WaxSys::isAV())
		{
			//Zur Zeit sind nur 0° und 180° zulässig
			if (ele->get_winkel() != 0 && ele->get_winkel() != 180)
			{
				throw getStammdaten().getText("err_biegung8"); // Es sind nur Biegungen in der Ansichtsebene möglich. (0 oder 180 Grad)
			}
		}

		//Wenn Werte noch nicht eingelesen sind, diese jetzt einlesen
		if (maxWinkel == 0)
		{
			if (!bg.getAbstaende(ele->get_anschlussDN(), this->get_rohrQuali() + ROHR_Q1 -1, minWinkel, maxWinkel, minAbst, minEleAbst))
				throw getStammdaten().getText("err_biegung1"); //Eine Biegung ist bei der Rohrqualität und DN nicht zulässig.
		}

		if (ele->get_durchmesser() > maxWinkel)
		{
			err.Format(getStammdaten().getText("err_biegung2").c_str(), maxWinkel);
			throw err; //Der Biegewinkel ist für diese Rohrqualität und DN zu groß. Maximal {0}° zulässig.
		}

		if (ele->get_durchmesser() < minWinkel)
		{
			err.Format(getStammdaten().getText("err_biegung7").c_str(), minWinkel);
			throw err; //Der Biegewinkel ist zu klein. Gebogen wird ab %i°.
		}

		if(countKnicke == 2)
		{
			abst = ele->get_abstAbs() - abstPrev;
			abstPrev = 0;
		}
		else
		{
			if(i==0)
			{
				abst = ele->get_abstAbs() - startEl.get_laenge();
			}
			else
			{
				abst = ele->get_abstAbs() - abstPrev;
			}

		}

		if((abst < minAbst && i == 0) || (abst < minAbst && countKnicke == 2))
		{
			err.Format(getStammdaten().getText("err_biegung3").c_str(), minAbst);
			throw err; //Der Mindestabstand l={0}mm wird zum Rohranfang bzw. -ende nicht eingehalten.
		}

		if (abstPrev > 0)
		{
			abst = ele->get_abstAbs() - abstPrev;
			if (abst < minEleAbst)
			{
				err.Format(getStammdaten().getText("err_biegung4").c_str(), minEleAbst); 
				throw err; //Der Mindestelementabstand l={0}mm wird nicht eingehalten.
			}
		}

		winkel = ele->get_winkel();
		lastKnickAbst = abstPrev = ele->get_abstAbs();
		hasKnick = true;

	}

	if (lastKnickAbst)
	{
		abst = this->get_laenge() - lastKnickAbst - endEl.get_laenge();
		if (abst < minAbst)
		{
			err.Format(getStammdaten().getText("err_biegung3").c_str(), minAbst);
			throw err; //Der Mindestabstand l={0}mm wird zum Rohranfang bzw. -ende nicht eingehalten.
		}
	}

}

bool Rohrleitung::calcZuschnittBeiBiegung()
{
	for (POSITION pos = get_elementList().GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element *>(get_elementList().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (!ele->isEleRohr())
			continue;

		if (ele->get_uelementList().GetSize() <= 0)
			continue;

		double zusLaenge = 0;

		for (POSITION pos = ele->get_uelementList().GetHeadPosition(); pos;)
		{
			Element *uele = dynamic_cast<Element *>(ele->get_uelementList().GetNext(pos));
			XASSERT(uele);
			if (!uele)
				break;

			if (!uele->isKnick())
				continue;

			//zusätzlich Länge berechnen
			
			zusLaenge += calcBiegenZuschlag();
		}

		//Runden
		if (zusLaenge < 0)
			zusLaenge -= 0.5;
		else
			zusLaenge += 0.5;
		int iLaenge = (int)zusLaenge;
		if (iLaenge != 0)
		{
			ele->set_laengeZuschnitt(ele->get_laenge() + iLaenge);
		}

	}
	return true;
}



double Rohrleitung::calcBiegenZuschlag()const
{
	
	double LaengeZuschlag = 0;
	
	// Keine Zuschlaege mehr fuer das Biegen weil der Zuschlag vernachlaessigt werden kann
	/*
	if(iKnicke == 2 && dAbstandErsterBogen !=  dAbsElement)
	{
		double dAbstandknicke = 0;
		dAbstandknicke =  dAbsElement - dAbstandErsterBogen;
		double GesamtGestreckteLaenge = dAbstandknicke / cos(winkel * pi / 180.0);
		LaengeZuschlag = GesamtGestreckteLaenge - dAbstandknicke;
	}
	*/
	return LaengeZuschlag;
}

/**
Zuschnittslänge wird vom ersten Rohrabschnitt bis zum 1. Bogen bzw. Reduzierung berechnet.
*/
int Rohrleitung::getLaengeZuschnitt() const
{
	int zlaenge = 0;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *element = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRedBog())
			break;

		zlaenge += element->getLaengeZuschnitt();
	}

	return zlaenge;
}


void Rohrleitung::checkHoeheBeschichten()
{
	
	const int maxHoehe = 1300;


	//Nur bei gepulverten Rohren prüfen
	if (!get_beschichtung().isGepulvert())
		return;
	

	

	double winkelHoehe = 0;



	//Längstes Rohr ermitteln
	double countWinkel1 = 0;
	double countWinkel2 = 0;
	int laenge1 = 0;
	int laenge2 = 0;
	int laenge3 = 0;
	int countWinkel = 0;
	int laengeprevRohr = 0;
	int lastLaengeRohr = 0;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;	

		if (ele->isEleRohr() && ele->get_uelementList().GetSize() > 0)
		{					
			int lastPos = ele->get_abstAbs();
			int lastKnickAbst = 0;
			
			for (POSITION pos = ele->get_uelementList().GetHeadPosition(); pos;)
			{
				const Element *uele = dynamic_cast<const Element *>(ele->get_uelementList().GetNext(pos));
				XASSERT(uele);
				if (!uele)
					break;

				if (!uele->isKnick())
					continue;

				
				if (countWinkel == 0)
				{
					laenge1 = uele->get_abstAbs() - lastPos - laengeprevRohr;
					lastKnickAbst = uele->get_abstAbs();
				}
				else if (countWinkel == 1)
				{
					laenge2 = uele->get_abstAbs() - lastPos - laengeprevRohr;
					lastKnickAbst = uele->get_abstAbs();
				}
				
				lastPos = uele->get_abstAbs();
				
				if (uele->get_winkel() == 0)
				{
					if (countWinkel == 0)
						countWinkel1 += uele->get_durchmesser();
					countWinkel2 += uele->get_durchmesser();
								
				}
				else if (uele->get_winkel() == 180)
				{
					if (countWinkel == 0)
						countWinkel1 -= uele->get_durchmesser();
					countWinkel2 -= uele->get_durchmesser();							
				}
								
				countWinkel++;
			}	

			//Restabstand			
			if (countWinkel == 0)			
				laenge1 = lastLaengeRohr + ele->get_laenge() - lastPos;			
			else if (countWinkel == 1)			
				laenge2 = lastLaengeRohr + ele->get_laenge() - lastPos;			
			else if (countWinkel == 2)
				laenge3 = lastLaengeRohr + ele->get_laenge() - lastPos;			
			

			laengeprevRohr = lastKnickAbst;
			lastLaengeRohr = ele->get_laenge();

			//nur Winkel im 1. Rohr berücksichtigen
			//if (countWinkel > 0)
			//	break;
		}				
	}


	bool calcHoehe = false;
	//Referenzwinkel berechnen (Vom längsten Rohr ausgehend
	if (countWinkel > 0)
	{		
		if (laenge2 > laenge1 && laenge2 > laenge3)
		{
			winkelHoehe = countWinkel1 * (-1);
			calcHoehe = true;
		}
		else if (laenge3 > laenge1 && laenge3 > laenge2)
		{
			winkelHoehe = countWinkel2 * (-1);
			calcHoehe = true;
		}		
	}



	double hoehe = 0;
	double laenge = 0;		
	double tmphoehe = 0;
	double tmplaenge = 0;
	double countBoegen = 0;
	double hoeheMax = 0;
	double hoeheMin = 0;
	lastLaengeRohr = 0;
	//jetzt die Höhe berechnen	
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;	

		if (ele->isEleRohr() && ele->get_uelementList().GetSize() > 0)
		{			
			double lastPos = ele->get_abstAbs();
			int countKnick = 0;
			int tmplastLaengeRohr = lastLaengeRohr;
			for (POSITION pos = ele->get_uelementList().GetHeadPosition(); pos;)
			{
				const Element *uele = dynamic_cast<const Element *>(ele->get_uelementList().GetNext(pos));
				XASSERT(uele);
				if (!uele)
					break;

				if (!uele->isKnick())
					continue;

				countKnick++;
				if (countKnick == 2)
					tmplastLaengeRohr = 0;

				if (calcHoehe)
				{					
					calhoehe(winkelHoehe, uele->get_abstAbs() - lastPos - tmplastLaengeRohr, tmphoehe, tmplaenge);
					laenge += tmplaenge;
					hoehe += tmphoehe;

					if (hoehe < hoeheMin)
						hoeheMin = hoehe;
					if (hoehe > hoeheMax)
						hoeheMax = hoehe;
				}
				else
				{
					laenge += uele->get_abstAbs() - lastPos - tmplastLaengeRohr;
				}

				lastPos = uele->get_abstAbs();
				
				if (uele->get_winkel() == 0)
				{
					winkelHoehe += uele->get_durchmesser();
					calcHoehe = true;					
				}
				else if (uele->get_winkel() == 180)
				{
					winkelHoehe -= uele->get_durchmesser();
					calcHoehe = true;					
				}
				else
					calcHoehe = false;
			}	

			//Restabstand
			if (calcHoehe)
			{					
				calhoehe(winkelHoehe, lastLaengeRohr + ele->get_laenge() - lastPos, tmphoehe, tmplaenge);
				laenge += tmplaenge;
				hoehe += tmphoehe;

				if (hoehe < hoeheMin)
					hoeheMin = hoehe;
				if (hoehe > hoeheMax)
					hoeheMax = hoehe;
			}
			else
			{
				laenge += ele->get_laenge() - lastPos;
			}

			lastLaengeRohr = ele->get_laenge();
			
		}
		else
		{
			//Höhe des Bauteils berechnen
			if (calcHoehe)
			{					
				calhoehe(winkelHoehe, ele->get_laenge(), tmphoehe, tmplaenge);
				laenge += tmplaenge;
				hoehe += tmphoehe;		

				if (hoehe < hoeheMin)
					hoeheMin = hoehe;
				if (hoehe > hoeheMax)
					hoeheMax = hoehe;
			}
			else
			{
				laenge += ele->get_laenge();
			}
		}

		if (ele->isEleBogen())
		{		
			countBoegen++;

			if (ele->get_winkel() == 0)
			{
				winkelHoehe += 90; //Zur Zeit nur 90° Bögen
				calcHoehe = true;				
			}
			else if (ele->get_winkel() == 180)
			{
				winkelHoehe += -90; //Zur Zeit nur 90° Bögen
				calcHoehe = true;				
			}
			else
				calcHoehe = false;

			//Länge Bogen in der neuen Richtung berücksichtigen
			if (calcHoehe)
			{					
				calhoehe(winkelHoehe, ele->get_laenge(), tmphoehe, tmplaenge);
				laenge += tmplaenge;
				hoehe += tmphoehe;		

				if (hoehe < hoeheMin)
					hoeheMin = hoehe;
				if (hoehe > hoeheMax)
					hoeheMax = hoehe;
			}
		}
	}


	hoehe = fabs(hoeheMin) + fabs(hoeheMax);

	//Höhe und Länge prüfen	
	if (abs((int)hoehe) > maxHoehe &&  (int)laenge > maxHoehe)
	{
		CString err;
		err.Format(getStammdaten().getText("err_groesse_beschichtung").c_str(), maxHoehe); //Das Bauteil ist zu groß für die Beschichtungsanlage. (Max. Höhe = {0})
		AfxMessageBox(err, MB_OK | MB_ICONERROR);
	}



	return;
}


void Rohrleitung::calhoehe(double winkel, double lh, double &h, double &l)
{
	//Höhe
	h = lh * sin(winkel * pi / 180.0);
		
	//Länge
	l = fabs(lh * cos(winkel * pi / 180.0));

	return;
}

/**
Schnittlänge verschieben wenn ein Kick genau auf dem Schnitt liegt
wird nur in der AV Version ausgeführt.
*/
bool Rohrleitung::setSchnittlaenge()
{
	if (!WaxSys::isAV())
		return true;

	int abstand = 400;


	for (int i=0; i<get_elementArray().GetSize(); i++)
	{
		const Element* const ele = get_elementArray().get_element_at(i);
		if (!ele)
			break;
		
		if (!ele->isKnick())
			continue;

		if (get_schnittlaenge() >= ele->get_abstAbs() - abstand &&
			get_schnittlaenge() <= ele->get_abstAbs() + abstand)
			set_schnittlaenge(ele->get_abstAbs() - abstand);		
	}


	return true;
}




