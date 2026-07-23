#include "stdafx.h"

#include "resource.h"
#include "Handelslaenge.h"
#include <afxcoll.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//Konstruktor
Handelslaenge::Handelslaenge()
{
	stdbezeichnung = getStammdaten().getText("IDS_HANDELSLAENGE").c_str(); 
	set_stdbemerkung(stdbezeichnung);
	set_laenge(6000);
}

//Destruktor
Handelslaenge::~Handelslaenge()
{
}

//Speichern und Laden der Rohrleitung. Achtung:
void Handelslaenge::serialize(BinaryArchive& ar)
{
	Rohrleitung::serialize(ar);
}

//Hier wird automatisch der Arbeitsplan generiert
void Handelslaenge::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	Rohrleitung::createAutoArbeit();

	// Afos aus Elementliste der Rohrleitung generieren
	int firstEle = 0;
	Element ele;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		firstEle++;
		Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;
		
		// Bei Handelslaenge ca. 6000 wird nicht gesaegt
		// Bei Handelslaenge ca. 3000 wird nur jedes 2 Rohr gesägt
		if (element->isEleRohr())
		{
			if (get_laenge() == 3000)
			{
				int rest = get_anzahl() % 2;					
				int anz = (int)get_anzahl()/2;			
				if (rest > 0) 
					anz += 1;	
				get_arbeit().addAfos(anz, element, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
			}
			else if (get_laenge() != 6000)
			{
				get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
			}
		}
		else
		{
			get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
		}
	}

	// Afos sortieren
	get_arbeit().sortAfos(get_arbeit().get_arbeitsfolgen());
}

//Rohrkonstruktion auf korrekte Werte checken
CString Handelslaenge::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err = "";

	// Laenge checken
	if (get_laenge() != 6000 && get_laenge() != 3000 && get_laenge() != 2000
		  && get_laenge() != 1500 && get_laenge() != 1000)
	{
		err.Format(getStammdaten().getText("IDS_FEHLER_HL_LAENGEN").c_str(), 6000, 3000, 2000, 1500, 1000);
		CString str; str = getStammdaten().getText("IDS_BITTE1").c_str(); 
		return err + " " + str;
	}
	// Duenn nicht grundiert bei HL
	if ((get_rohrQuali() == RQ2458GSCH && get_dnvl() < 65) && get_beschichtung().isGrundiert())
	{
		Element testele;
		testele.set_id(ROHR_Q1 + get_rohrQuali() - 1);
		err.Format(getStammdaten().getText("IDS_ERROR47").c_str(), testele.get_bezeichnung());
		return err;
	}

	return Rohrleitung::checkPlausis();
}

//Rohrkonstruktion erzeugen
void Handelslaenge::createModell()
{
	Rohrleitung::createModell();

	if (get_laenge() > 0) 
	{
		// Löschen aller Elemente aus der Liste
		elementList.deleteAllElemente();

		// Laengen für Anfang/Ende/Verb. setzen 
		setAnfaEndeVerbLaenge();

		// Berechnen der Rohrstuecke
		setRohrlaengen();		
		
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

	// fuer VK3-Berechnung
	createVk3Liste();
}

void Handelslaenge::check_elementArray()
{
	///Kein Aufruf der Funktion in der Basisklasse, da Handelslaenge keine Abgaenge hat
}

void Handelslaenge::addRohrausschuss(CObList& list)
{
	///Kein Aufruf der Funktion in der Basisklasse, da bei Handelslaenge kein Ausschuss berechnet wird
	UNREFERENCED_PARAMETER(list);
}

bool Handelslaenge::createVk3Liste()
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

		// Nuten erstmal nicht mehr berechnen, damit HL guenstiger als Verteiler, Engelmann 21.09.2004
		if (e->get_id() == NUT)
			continue;

		Element* ele = new Element;
		ele->copy(*e);

		if (ele->isEleRohr())
		{
			// grundiert, bei Handels/Fixlaenge und verzinkt auch grundiert nehmen 
			if (get_beschichtung().isGrundiert() || get_beschichtung().isVerzinkt())
			{
				if (get_laenge() == 6000)
					ele->set_id(ele->get_id() + 200);		
				// Fixlaenge
				else 
					ele->set_id(ele->get_id() + 400);		
			}
			// gepulvert
			else 
			{
				if (get_laenge() == 6000)
					ele->set_id(ele->get_id() + 100);		
				// Fixlaenge
				else 
					ele->set_id(ele->get_id() + 300);		
			}
		}

		// Element in Liste packen
		if (ele->isEleVK3Rohr())
			ele->set_laenge(ele->get_laenge()*get_anzahl());
		else
			ele->set_anzahl(ele->get_anzahl()*get_anzahl());
		vk3elementeListe.AddTail(ele);
	}


	// gleiche Elemente zusammenfassen
	vk3elementeListe.kumulieren();

	// zuletzt noch Preise setzen
	vk3elementeListe.setVk3Preise(get_beschichtung().isVerzinkt(), 0, 0, 0);

	return true;
}

const CString Handelslaenge::getBemassungstext() const
{
	CString str; str = getStammdaten().getText("IDS_CA").c_str(); 
	return str;
}

///Rohrquali auf neuen Standard aendern
void Handelslaenge::switchRohrquali()
{
	// Gemischte Rohrquali 2440/2458 nur bei Verteiler u. HL wieder drin,
	// deswegen passiert hier nichts!
}



///Liefert das Gesamtgewicht der Rohrkonstruktion aus der Stueckliste
double Handelslaenge::getGesGewicht(int teile) const
{
	UNREFERENCED_PARAMETER(teile);

	
	double gesGew = 0.0;
	// Alles multiplizieren mit Anzahl der Rohre
	POSITION spos = stueckliste.GetHeadPosition();
	for (spos = stueckliste.GetHeadPosition(); spos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(stueckliste.GetNext(spos));
		
		if (ele)
			gesGew += ele->getGewicht();
	}
	return gesGew;
}