#include "stdafx.h"
#include "resource.h"


#include "Verteiler.h"
#include "Element.h"
#include "ElementList.h"



#include <mxutil/strloader.h>
using namespace MxUtil;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


using namespace MxErrorHandling;

//Standardkonstruktor
Verteiler::Verteiler()
{
	m_defLaengen.RemoveAll();

	rohrDefLaengen	= "";
	stdbezeichnung = getStammdaten().getText("IDS_VERTEILER").c_str(); 
	set_dnvl(100);
	sap = true;
}

//Destruktor
Verteiler::~Verteiler()
{
	
}

//Speichern und Laden der Rohrleitung.
void Verteiler::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	Rohrleitung::serialize(ar);
	if (ar.is_storing()) 
	{
		ar << elementArray.GetSize() << rohrDefLaengen;
		for (int i=0; i<elementArray.GetSize(); i++) 
		{
			Element *ele = elementArray.get_element_at(i);
			XASSERT(ele);
			if (ele)
				ele->serialize(ar);
		}
	} 
	else 
	{
		int anzahlElemente = 0;
		ar >> anzahlElemente >> rohrDefLaengen;
		for (int i=0; i<anzahlElemente; i++) 
		{
			Element *ele = new Element;
			ele->serialize(ar);
			elementArray.Add(ele);
		}
		set_rohrDefLaengen(rohrDefLaengen);
	
		if (version > 29 && version < 31)
		{
			delete_stuecklisteSap();
			int dummy;
			ar >> dummy;
			while (dummy)
			{
				Element *ele = new Element;
				ele->serialize(ar);
				stuecklisteSap.add(ele);
				ar >> dummy;
			}
		}
		if (version > 29 && version < 41)
		{
			Arbeitsplan dummy;
			dummy.serialize(ar);
		}
	}
}

///Kopierfunktion fuer den Verteiler
void Verteiler::copy(Verteiler& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	Rohrleitung::copy(other);

	rohrDefLaengen = other.rohrDefLaengen;
	m_defLaengen.Copy(other.m_defLaengen);
}

///Kopierfunktion fuer den Verteiler
void Verteiler::copy(Rohrleitung& other)
{
	// Kopieren von Rohrleitung nach Verteiler
	Rohrleitung::copy(other);
}

///Rohrquali auf neuen Standard aendern
void Verteiler::switchRohrquali()
{
	// Gemischte Rohrquali 2440/2458 nur bei Verteiler wieder drin,
	// deswegen passiert hier nichts!
}



void Verteiler::handleSprinklerSchelle()
{
	int iSchelle = 0;
	int iBohrung = 0;

	for( int s = 0; s < elementArray.GetCount(); s++)
	{
		Element *pEleMain = elementArray.get_element_at(s);
		
		if(pEleMain->get_id() == SPRINKLERSCHELLE_VIROTEC)
		{
			iSchelle = s;
			for( int g = 0; g < elementArray.GetCount(); g++)
			{
				Element *pEle = elementArray.get_element_at(g);
				if( pEle->get_abstAbs() == pEleMain->get_abstAbs() && pEle->get_id() != SPRINKLERSCHELLE_VIROTEC )
				{
					iBohrung = g;
					Element *pEleSchelletmp = elementArray.get_element_at(iSchelle);
					Element *pEleBohrungtmp = elementArray.get_element_at(iBohrung);
					pEleSchelletmp->set_laenge(20);
					pEleSchelletmp->set_dn1(pEleBohrungtmp->get_durchmesser());

					if( iBohrung < iSchelle)
					{
						pEleSchelletmp = elementArray.get_element_at(iSchelle);
						pEleBohrungtmp = elementArray.get_element_at(iBohrung);

						Element *pEleSchelle = new Element(*pEleSchelletmp);
						Element *pEleBohrung = new Element(*pEleBohrungtmp);

						elementArray.RemoveAt(iSchelle);
						elementArray.RemoveAt(iBohrung);

						elementArray.InsertAt(iSchelle-1, pEleBohrung);
						elementArray.InsertAt(iSchelle-1, pEleSchelle);
						
						handleSprinklerSchelle();
						break;
					}
				}
			}
		}
	}

}


///Rohrkonstruktion erzeugen
void Verteiler::createModell()
{
	get_stdelementArray().deleteAll();
	avelementArray.deleteAll();
	elementArraySap.deleteAll();

	//handleSprinklerSchelle();


	setSchnittlaenge();

	Rohrleitung::createModell();

	/*
	Rohrkonstruktion in Elementliste eingeben
	*/
	if (get_laenge() > 0) 
	{		
		// Restabstand setzen
		set_restabstand(0);
		
		// Loeschen aller Elemente aus der Liste
		elementList.deleteAllElemente();

		// Laengen für Anfang/Ende/Verb. setzen 
		setAnfaEndeVerbLaenge();

		// Berechnen der Rohrstuecke
		CString err = setRohrlaengen();
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

		if (rohrDefLaengen.GetLength() > 0)
			setmaxLaenge();
		else
		{
			if (getMaxXLaenge() > getMaxYLaenge())
				set_maxlaenge(getMaxXLaenge());
			else
				set_maxlaenge(getMaxYLaenge());
		}

	}
	delete_stueckliste();
	create_stueckliste();

	calcZuschnittBeiBiegung();

	// fuer VK3-Berechnung
	createVk3Liste();
	createPulverlaengenListe();
	createPasslaengenListe();

/*
#ifdef _DEBUG
	afxDump << get_elementList();

	afxDump << "\n\nElementArray";
	for (int p = 0; p < get_elementArray().GetCount(); p++)	
	{
		Element* ele = get_elementArray().get_element_at(p);
		XASSERT(ele);
		afxDump << ele;
	}

	afxDump << get_elementListSap();
#endif
*/

	
/*
#ifdef _DEBUG
	afxDump << get_stueckliste();

	afxDump << get_stuecklisteSap();
#endif
*/
}



//Laenge von Anfang/Ende/Verbindung setzen
void Verteiler::setAnfaEndeVerbLaenge()
{
	Rohrleitung::setAnfaEndeVerbLaenge();
}

//Setzen der Rohrlaengen
CString Verteiler::setRohrlaengen()
{
	/*
	Berechnung der Rohrstuecke inkl. Red/Bog. 
	Red/Bog sind Rohrstuecke, da sie in der Elementliste an die 
	Rohre gehängt werden und nicht in deren Unterliste. Da aber die 
	"richtigen" Rohrstuecke immer durch Elemente beidseitig verbunden 
	werden, muesssen die Red/Bog-Stuecke als Rohrverbindung behandelt
	werden. Um die "richtigen" Rohrstueke von Red/Bog unterscheiden 
	zu koennen, wird die ID gespeichert. Ist bei definierten Rohrlaengen
	ein Rohrstueck laenger als 6000 wird dieses geteilt, mit SNAHT verbunden
	und das erste Teilstueck mit ROHR_Q1+100 als Ueberlaenge gekennzeichnet.
	Hierbei koennen maximal 2 Teile entstehen, da eine definierte Rohrlaenge
	auf 12000 begrenzt ist.   
	*/

	memset(rohrLaengen, 0, sizeof(rohrLaengen));
	
	// Rohrlaengen def.?
	if (rohrDefLaengen.GetLength() > 0) 
	{
		if (setrohrDefLaengen())
		{
			return setDefRohrlaengen();
		}
	}

	// Rohrlaengen werden autom. festgelegt -> get_schnittlaenge() = 12000 
	// inkl. Verb. + Rest inkl. Verb.
	return setAutoRohrlaengen();
}

///Liefert Zwischensumme der bereits abgearbeiteten Laengen
int Verteiler::getSummeDeflaengen(int index)
{

	int summe = 0;
	for (int i=0; i<=index && i < m_defLaengen.GetSize(); i++)
	{
		summe += m_defLaengen[i];
	}
	return summe;
}

///Definierte Rohrunterteilung
CString Verteiler::setDefRohrlaengen()
{
	CString err;
	if (elementArray.GetSize() == 0)
	{
		// Berechnung der Rohrstuecke nach definierten Rohrlaengen ohne Elemente
		err = calDefRohrlaengenOhneEle();
	}
	else
	{
		// Berechnung der Rohrstuecke nach definierten Rohrlaengen
		err = calDefRohrlaengen();
		if (!err.GetLength())
			// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
			err = checkRohrtrennstelle();
	}
	return err;
}

///Berechnung der Rohrstuecke nach definierten Rohrlaengen ohne Elemente
CString Verteiler::calDefRohrlaengenOhneEle()
{
	int k=0;
	int counter    = 0;
	int nRohre	   = 0;
	CString err;

	for (counter=0; counter<m_defLaengen.GetSize(); counter++)
	{	
		if (m_defLaengen[counter] > get_schnittlaenge())
		{
			set_rohrLaengen(k, get_schnittlaenge()); 
			rohrID[k] = ROHR_Q1+100;
			set_rohrLaengen(++k, m_defLaengen[counter] - get_schnittlaenge()); 
			rohrID[k] = ROHR_Q1;
			k++;
		}
		else 
		{
			set_rohrLaengen(k, m_defLaengen[counter]); 
			rohrID[k] = ROHR_Q1;
			k++;
		}
	}
	nRohre = k;
	// Attribute Rohrleitung setzen
	set_nRohre(nRohre);

	return err;
}

///Berechnung der Rohrstuecke nach definierten Rohrlaengen
CString Verteiler::calDefRohrlaengen()
{
	CString err = "";
	int nRohre	   = 0;
	//int counter    = 0;

	bool ueberlaenge = false;
	bool trefferRed  = false;	
	bool treffer     = false;
	int summe1 = 0, summe2  = 0;
	int j=0, k=0; 
	// Laenge des Rohrstuecks bis vor Red/Bog 
	int trefferLaenge  = 0;
	// Absoluter Abstand bis zu Red/Bog
	int trefferAbstand = 0;

	reset_rohrlaengen_und_ids();

	Element *ele = NULL;
	//while (defLaengen[counter] != 0)
	//	counter++;
		
	// die def. Rohrlaengen durchgehen und ggbfs. aufteilen
	for (int i=0; i<m_defLaengen.GetSize(); i++) 
	{
		// Beim vorherigen Rohrstueck Red. gefunden
		if (trefferRed) 
		{
			// Laenge vom Rohrstueck groesser Schnittlaenge
			if (ueberlaenge) 
			{
				if ((m_defLaengen[i] - trefferLaenge) > get_schnittlaenge())
					summe2 = summe1 + get_schnittlaenge();
				else 
					summe2 = summe1 + m_defLaengen[i] - trefferLaenge; 
				summe2 = getSummeDeflaengen(i);
			} 
			// sonst
			else 
			{
				// Restlaenge vom Rohrstueck groesser Schnittlaenge
				if ((getSummeDeflaengen(i) - summe1) > get_schnittlaenge())
					summe2 = summe1 + get_schnittlaenge();
				// sonst
				else 
					summe2 = getSummeDeflaengen(i);
			}
		} 
		else 
		{
			// vorherige Teillaenge vom Rohrstueck groesser Schnittlaenge
			if (ueberlaenge) 
				summe2 = getSummeDeflaengen(i);
			// Laenge Rohrstueck ist groesser Schnittlaenge
			else if (m_defLaengen[i] > get_schnittlaenge()) 
				summe2 += get_schnittlaenge();
			// sonst
			else 
				summe2 = getSummeDeflaengen(i);
		}
		
		trefferRed = false;	
		trefferLaenge = 0;
		trefferAbstand = 0;
		while (k < elementArray.GetSize()) 
		{
			// Element aus Array
			ele = elementArray.get_element_at(k);
			XASSERT(ele);
			if (!ele || !ele->isEleRedBogEinz())
			{
				k++;
				continue;
			}

			// Element liegt hinter Rohrstueck
			if (ele->get_abstAbs() > summe2) 
				break;
		
			rohrID[j] = ROHR_Q1; 
			// bei Bogen Bemassung bis Elementmitte
			if (ele->isEleBogen()) 
			{
				trefferLaenge = ele->get_abstAbs() - summe1 - ele->get_laenge();	
				trefferAbstand = ele->get_abstAbs();
				err = checkRohrlaenge(ele->get_abstAbs() - summe1 - ele->get_laenge(), ele);
				if (err.GetLength())
					return err;
				set_rohrLaengen(j, ele->get_abstAbs() - summe1 - ele->get_laenge());	
				summe1 += ele->get_abstAbs() - summe1 - ele->get_laenge();
				nRohre++;
			// bei Red Bemassung bis Elementanfang
			} 
			else 
			{
				trefferLaenge = ele->get_abstAbs() - summe1;	
				trefferAbstand = ele->get_abstAbs();
				err = checkRohrlaenge(ele->get_abstAbs() - summe1, ele);
				if (err.GetLength())
					return err;
				set_rohrLaengen(j, ele->get_abstAbs() - summe1);	
				summe1 += ele->get_abstAbs() - summe1;
				nRohre++;
			}
			j++;
			rohrID[j] = ele->get_id(); 
			// bei Bogen muss man bei beiden angrenzenden Stuecken Laenge abziehen
			if (ele->isEleBogen()) 
			{
				trefferLaenge += (2*ele->get_laenge());			
				trefferAbstand = ele->get_abstAbs();
				set_rohrLaengen(j, (2*ele->get_laenge()));			
				summe1 += (2*ele->get_laenge()); 
				if (!ueberlaenge) 
					nRohre++;
			} 
			// bei Red muss man nur beim ersten jetzigen Rohrstueck Laenge abziehen
			else 
			{
				trefferLaenge += ele->get_laenge();			
				trefferAbstand = ele->get_abstAbs();
				set_rohrLaengen(j, ele->get_laenge());			
				summe1 += ele->get_laenge(); 
				if (!ueberlaenge) 
					nRohre++;
			}
			j++;
			if (ueberlaenge) 
				nRohre++; 

			trefferRed = true;
			treffer = true;
			k++;
		} 

		if (!trefferRed) 
		{ 
			rohrID[j] = ROHR_Q1; 
			// beim vorherigen Rohrstueck Red?
			if (treffer) 
			{
				// vorherige Teillaenge vom Rohrstueck groesser Schnittlaenge
				if (ueberlaenge) 
				{
					err = checkRohrlaenge(getSummeDeflaengen(i) - summe1, ele);
					if (err.GetLength())
						return err;
					set_rohrLaengen(j, getSummeDeflaengen(i) - summe1);
					ueberlaenge = false;
					treffer = false;
				} 
				// Laenge Rohrstueck groesser Schnittlaenge
				else if ((getSummeDeflaengen(i) - summe1) > get_schnittlaenge()) 
				{
					rohrID[j] = ROHR_Q1+100; 
					set_rohrLaengen(j, get_schnittlaenge());
					ueberlaenge = true;
					i--;
				} 
				// Laenge Rohrstueck kleiner Schnittlaenge
				else 
				{
					err = checkRohrlaenge(getSummeDeflaengen(i) - summe1, ele);
					if (err.GetLength())
						return err;
					set_rohrLaengen(j, getSummeDeflaengen(i) - summe1); 
					treffer = false;
				}
				nRohre++;
			} 
			else 
			{
				// vorherige Teillaenge vom Rohrstueck groesser Schnittlaenge
				if (ueberlaenge) 
				{
					err = checkRohrlaenge(m_defLaengen[i] - get_schnittlaenge(), ele);
					if (err.GetLength())
						return err;
					set_rohrLaengen(j, m_defLaengen[i] - get_schnittlaenge());
					ueberlaenge = false;
					nRohre++;
				} 
				// Laenge Rohrstueck groesser Schnittlaenge
				else if (m_defLaengen[i] > get_schnittlaenge()) 
				{
					rohrID[j]      = ROHR_Q1+100; 
					set_rohrLaengen(j, get_schnittlaenge());
					ueberlaenge = true;
					nRohre++;
					i--;
				} 
				// Laenge Rohrstueck kleiner Schnittlaenge
				else 
				{
					set_rohrLaengen(j, m_defLaengen[i]);
					nRohre++; 
				}
			}
			summe1 = summe2;  	
			j++;
		} 
		else 
			i--;
	}
	// Attribute Rohrleitung setzen
	set_nRohre(nRohre);

	return err;
}

/**
Max. Rohrlänge zurückgeben. Wenn Def. Längen definiert sind, wird längstes Teilstück zurückgegeben
sonst wird die Gesamtrohrlänge des hauptrohres zurückgegeben
*/
const int Verteiler::getMaxRohrlaenge()const 
{
	int laenge = 0;

	if (m_defLaengen.GetSize() > 0)
	{
		//längstes Teilrohrstück ermitteln
		for (int i=0; i<m_defLaengen.GetSize(); i++)
		{
			if (laenge < m_defLaengen[i])
				laenge = m_defLaengen[i];
		}
	}
	else
		laenge = get_laenge();

	return laenge;
}



const CString& Verteiler::get_rohrDefLaengen() const
{
	return rohrDefLaengen;
}


void Verteiler::set_rohrDefLaengen(const CString& value)
{
	rohrDefLaengen = value;

	//Array zurücksetzen
	//Array wird durch den Aufruf von checkPlausi wieder gesetzt
	m_defLaengen.RemoveAll();

}	

//Hier wird automatisch der Arbeitsplan generiert
void Verteiler::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	

	bool isAnlage = checkAnlage();

	// Damit die Standardarbeitsfolgen im AP fuer VK-Abrechnung landen 
	// und nicht im AP fuer Sap
	sap = false;
	Rohrleitung::createAutoArbeit();
	sap = true;

/*
#ifdef _DEBUG
afxDump << get_arbeit();
#endif
*/



	Element *ele = new Element;
	Element *prev = NULL;
	Element *pprev = NULL;

	int count = 0;
	// Afos aus Elementliste der Rohrleitung generieren
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			return;

		if (count > 0)
		{
			POSITION posi = elementList.FindIndex(count-1);
			prev = dynamic_cast<Element*>(elementList.GetAt(posi));
			XASSERT(prev);
			if (!prev)
				break;
		}
			
		if (count > 1)
		{
			POSITION posi = elementList.FindIndex(count-2);
			pprev = dynamic_cast<Element*>(elementList.GetAt(posi));
			XASSERT(pprev);
			if (!pprev)
				break;
		}
		// Wenn Bogen etc. am Ende, dann nur eine Schweissnaht
		//if (element->isEleRedBog() && prev && prev->isEleRohr() && prev->get_laenge() == 0)
		//if (element->get_id() == SCHWEISS && pprev && pprev->isEleRohr() && pprev->get_laenge() == 0)
		//{
		//	count++;
		//	continue;
		//}


		// Rohr mit Laenge 0, enstanden durch Red oder Bogen, muessen natuerlich nicht bearbeitet werden 
		if (element->isEleRohr() && element->get_laenge() <= 0)
		{
			count++;
			continue;
		}

		// Schweissnaht fuer Rohrverbindung vor Ort nicht, jetzt doch laut Uslu 
		// Schweissnaht im Werk ist SCHWEISS
		switch (element->get_id())
		{
			//case SNAHT:
			case LEER:
				break;
			case SCH:
			{
				get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER, 1);
				break;
			}
			case KB50:
			case KB40:
			{
				// Plus Scheibe od. Boden
				if (element->get_anschlussDN() < 200)
				{
					ele->set_id(SCH);
					ele->set_dn(element->get_anschlussDN());
					ele->set_anschlussDN(element->get_anschlussDN());
					get_arbeit().addAfos(get_anzahl(), ele, get_beschichtung(),  0, "0", Rohrleitung::WASSER, 1);
				}
				else
				{
					ele->set_id(BOD);
					ele->set_dn(element->get_anschlussDN());
					ele->set_anschlussDN(element->get_anschlussDN());
					get_arbeit().addAfos(get_anzahl(), ele, get_beschichtung(),  0, "0", Rohrleitung::WASSER);
				}
				get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(),  0, "0", Rohrleitung::WASSER);
				break;
			}
			case ERED:
			case KRED:
			{
				//Von Reduzierung nur den größeren Durchmesser nehmen. Für den kleineren Durchmesser
				//wird die Zeit von der Schweißnaht genommen

				int dnRed;
				int dnSchweiss;
				if (element->get_dn() > element->get_anschlussDN())
				{
					dnRed = element->get_dn();
					dnSchweiss = element->get_anschlussDN();
				}
				else
				{
					dnSchweiss = element->get_dn();
					dnRed = element->get_anschlussDN();
				}

				//Zeiten für Reduzierung
				ele->set_id(element->get_id());
				ele->set_dn(dnRed);
				get_arbeit().addAfos(get_anzahl(), ele, get_beschichtung(), 0, "0", Rohrleitung::WASSER);

				//Zeiten für kleineren Duchmesser von Schweißnaht nehmen. (Hier keine Ausrichtung notwendig, darum geringere Zeit von Schweißnaht nehmen)
				ele->set_id(SCHWEISS);
				ele->set_dn(dnSchweiss);
				get_arbeit().addAfos(get_anzahl(), ele, get_beschichtung(), 0, "0", Rohrleitung::WASSER);


				break;
			}
			case SCHWEISS:
				

				if (prev)
				{
					//nur Schweißnaht setzen, wenn es keine Reduzierung ist
					if (!prev->isEleRed())
						get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(),  0, "0", Rohrleitung::WASSER);
				}
				else
					get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
				break;
			default:
				get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
				break;
		}

		CObList& uelementList = element->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			Element tmpEle = *uelement;
			if (isAnlage)
			{
				// Wenn Rohr > DN50 muss bei Nacharbeit der Vorrichter auch die Loecher brennen.
				// Somit muessen bei Afo16 immer die groesseren Zeiten vom Rohrlaengenbereich 0-1499 genommen werden.
				if (get_dnvl() > 50 && (tmpEle.get_id() == ROHR || tmpEle.get_id() == NUTST || tmpEle.get_id() == MUFFE))
					tmpEle.set_rohrLaenge(500);
			}

			if (tmpEle.get_makeAnlage())
			{
				if (tmpEle.get_id() == ROHR || tmpEle.get_id() == NUTST)
					tmpEle.set_id(STDSTUTZEN);

				get_arbeit().addAfos(get_anzahl(), &tmpEle, get_beschichtung(),  0, "0", Rohrleitung::WASSER, 0,1);
			}
			else
				get_arbeit().addAfos(get_anzahl(), &tmpEle, get_beschichtung(),  0, "0", Rohrleitung::WASSER);

			CObList& uuelementList = uelement->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)	
			{
				Element *uuelement = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuelement);
				if (!uuelement)
					break;

				get_arbeit().addAfos(get_anzahl(), uuelement, get_beschichtung(),  0, "0", Rohrleitung::WASSER);
			}
		}
		count++;
	}
	delete ele;

	// Rüstzeiten
	addRuestzeiten(); 


	// Afos sortieren
	get_arbeit().sortAfos(get_arbeit().get_arbeitsfolgen());

	///Es duerfen nur die Positionen beruecksichtigt werden, die auch geschweisst werden
	if (get_arbeit().hasArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz()))
		///Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16/17 gesetzt 
		get_arbeit().setKranzeit(getKranzeit());

	///Es duerfen nur die Positionen beruecksichtigt werden, die auch geschweisst werden
	if (get_arbeit().hasAfo(16))
		///Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16
		get_arbeit().setKranzeit(getKranzeit(true), true);


/*
#ifdef _DEBUG
afxDump << get_arbeit();
#endif


#ifdef _DEBUG
	afxDump << get_elementList();
#endif



#ifdef _DEBUG
	afxDump << get_elementListSap();
#endif
*/

}



//Rohrkonstruktion auf korrekte Werte checken
CString Verteiler::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err;

	// extra wegen Rohrlaengenfeld
	if (!check_rohrDefLaengen(err, get_rohrQuali(), get_dnvl()))
	{
		return err;
	}
	
	// Geht hier nicht -> kommt andauernd
	// Hinweis: Glatte Laengen ohne Schweissarbeit seperat bestellen
	//if (elementArray.GetSize() == 0)
	//	return "Glatte Längen ohne Schweissarbeit bitte als seperate SD-Posnr. bestellen!";

	// Rohrdaten und Standardelemente checken
	if (checkAnlage())
	{
		createModell();
		err = checkStdVl().c_str();
	}
	if (err.GetLength())
		return err;


	//wenn Oberfläche verzinkt, dann darf nicht über die Anlage gefahren werden
	if (get_beschichtung().get_typ() == Beschichtung::VERZINKT)
	{
		//prüfen ob Elemente Anlagenflag haben
		EnContainerRohrElemente& elemente = get_elementArray();
		for (int i=0; i<elemente.GetSize(); i++)
		{
			Element* pElement = elemente.get_element_at(i);
			XASSERT(pElement);
			if (!pElement)
				continue;

			if (pElement->get_makeAnlage())
			{
				err = getStammdaten().getText("IDS_VERZINKT_ANLAGE").c_str(); 
				return err;
			}
		}
	}

	err = Rohrleitung::checkPlausis();




	return err;
}

/*
Änderungen:
04.04.06, Jacobs: Bei Def Rohrlängen sind nur noch Zahlen getrennt durch Leerzeichen zulässig
*/
/**
Das Rohrlaengenfeld checken und in m_defLaengen Array setzen
*/
bool Verteiler::check_rohrDefLaengen(CString &error, int rohrQuali, int rohrDN)
{
	
	CString str;
	if (rohrDefLaengen.GetLength() == 0) 
	{
		if (get_laenge() > MAX_GESAMTLAENGE) 
		{
			str = getStammdaten().getText("IDS_FERTHINWEIS3").c_str(); 
			error.Format(str, MAX_GESAMTLAENGE);
			return false;
		}
	}
	else
	{
		int i;
		for (i = 0; i < rohrDefLaengen.GetLength(); i++)
		{
			unsigned char testc = rohrDefLaengen.GetAt(i);
			
			if (!isdigit(testc) /*&& (testc != '.')*/ && (testc != ' '))
			{
				error = getStammdaten().getText("IDS_ERROR121").c_str();  //Es dürfen nur Zahlen getrennt durch Leerzeichen eingegeben werden!
				return false;
			}
		}

		if (!setrohrDefLaengen())
		{
			error = getStammdaten().getText("IDS_ERROR122").c_str();  //Fehler beim Auswerten des Rohrlängenfeldes. Bitte Eingabe überprüfen.
			return false;
		}
		
		// Laenge checken
		int geslaenge = 0;
		for (i=0; i<m_defLaengen.GetSize(); i++) 
		{
			geslaenge += m_defLaengen[i];
			if (m_defLaengen[i] > MAX_GESAMTLAENGE)
			{
				str = getStammdaten().getText("IDS_ERROR123").c_str(); 
				error.Format(str, MAX_GESAMTLAENGE);
				return false;
			}
			if (rohrQuali == RQ2458GSCH && m_defLaengen[i] > MAX_ROHRLAENGE_DUENN && rohrDN < 50) 
			{ 
				Element testele;
				testele.set_id(ROHR_Q1 + rohrQuali - 1);
				str = getStammdaten().getText("IDS_ERROR124").c_str(); 
				error.Format(str, testele.get_bezeichnung(), MAX_ROHRLAENGE_DUENN);
				return false;
			} 
		}
		if (geslaenge > get_laenge())
		{
			error = getStammdaten().getText("IDS_ERROR125").c_str(); 
			return false;
		}
	}
	return true; 
}

// Liefert als default avelementArray, bei Verteiler allerdings elementArray
const EnContainerRohrElemente& Verteiler::getelemente() const
{
	return elementArray;
}

/*
Änderung:
04.04.06, Jacobs: komplett überarbeitet
*/
/**
Definiert das Integerarray mit den Def laengen. Diese werden aus dem String Def.Laengen ausgewertet

Liefert false zurück, wenn im String falsche Eingaben gemacht wurden. (Syntaxprüfung in check_rohrDefLaengen())
*/
bool Verteiler::setrohrDefLaengen()
{
	int summe	 = 0,
		iWert	 = 0,
		posStart = 0,
		posEnd   = 0;
	
	CString s1;	


	//Array zurücksetzen
	m_defLaengen.RemoveAll();

	//wenn keine Def. Längen definiert sind Funktion verlassen
	if (rohrDefLaengen.IsEmpty())
		return true;

	//evt. Leerzeichen vorne und hinten löschen
	rohrDefLaengen.TrimLeft();
	rohrDefLaengen.TrimRight();

	//Integer Werte aus String in das integer Array lesen
	while ((posEnd = rohrDefLaengen.Find(" ",posStart)) > 0)
	{
		s1 = rohrDefLaengen.Mid(posStart,posEnd-posStart);
		
		//Wert anhängen
		m_defLaengen.Add(atoi(s1));

		posStart = posEnd + 1;

		//posStart darf nicht größer als Stringlänge sein. (Sollte nie vorkommen)
		if (posStart >= rohrDefLaengen.GetLength())
			return false;
	}

	//Letzten Wert anhängen
	s1 =  rohrDefLaengen.Mid(posStart);
	m_defLaengen.Add(atoi(s1));


	//Summe der Rohrlängen
	for (int i=0; i<m_defLaengen.GetSize(); i++)
	{
		summe += m_defLaengen[i];
	}


	int rohrLaenge = get_laenge(); 

	if (summe > rohrLaenge)
	{
		//Fehler: Def Längen größer als Rohrlänge
		return false;
	}


	//Reststück anhängen
	iWert = rohrLaenge - summe;

	if (iWert > 0)
		m_defLaengen.Add(iWert);


	return true;
}

/*
Änderungen:
04.04.06, Jacobs: komplett überarbeitet
*//**
Liest das defLaengen Feld und setzt die maximale Rohrlänge.
*/
void Verteiler::setmaxLaenge()
{
	// Maxlaenge setzen, Def. Laengen werden berücksichtigt
	set_maxlaenge(getMaxRohrlaenge());
}


/*
Änderungen:
04.04.06, Jacobs: überarbeitet
*//**
Errechnet die Pulverzeit für den Werkauftrag
*/
double Verteiler::getPulverzeit() 
{
	
	if (m_defLaengen.GetSize() > 0)
	{
		// Todo: Abgabglaenge fuer jedes Rohrstueck bestimmen  
		double zeit = 0.0;
		int maxdn = getMaxRohrDN();
		int maxabglaenge = getMaxAbgLaenge();	
		
		for (int i=0; i<m_defLaengen.GetSize(); i++)
		{
			PulvernZeiten::data *pz = NULL;
			for (POSITION pzpos = getStammdaten().get_pulvernzeiten().GetHeadPosition(); pzpos;)
			{
				pz = (PulvernZeiten::data *)getStammdaten().get_pulvernzeiten().GetNext(pzpos);

				if (   (pz->get_dn()            == maxdn)				 
					&& (pz->get_laenge_von()    <= m_defLaengen[i])			
					&& (pz->get_laenge_bis()    >= m_defLaengen[i] || pz->get_laenge_bis()    == 0)
					&& (pz->get_abglaenge_von() <= maxabglaenge)			
					&& (pz->get_abglaenge_bis() >= maxabglaenge  || pz->get_abglaenge_bis() == 0))
				{
					zeit += (pz->get_zeit() * (double)get_anzahl());
					break;
				}
			}
		
		}
		XASSERT(zeit);
		if (!zeit)
			return 0;
		return zeit;
	}
	return Rohrleitung::getPulverzeit();
}


#if 0
///Elemente von Standardelemente in Verteilerelemente aendern
bool Verteiler::konvert_elementArray()
{
	CString err;

	// Standardelemente aendern
	for (int i=0; i<elementArray.GetSize(); i++)	
	{
		Element *ele = dynamic_cast<Element*>(elementArray.GetAt(i));
		XASSERT(ele);
		if (ele)
		{	
			if (ele->get_id() == STDMUFFE)
			{
				DNzuAussendurchmesser dntoad;
				int aussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_anschlussDN());
				ele->set_laenge(ele->get_laenge() - aussendurchmesser/2);
				ele->set_id(MUFFE);
				ele->set_makeAnlage(true);
			}
			else if (ele->get_id() == STDSTUTZEN)
			{
				DNzuAussendurchmesser dntoad;
				int aussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_anschlussDN());
				
				//if (ele->get_laenge() - aussendurchmesser/2 == 50 && ele->get_dn() >= 32 && ele->get_dn() <= 50)
				if (ele->get_urid() == NUTST)
				{
					ele->set_laenge(ele->get_laenge() - aussendurchmesser/2);
					ele->set_id(NUTST);

					// Nut loeschen
					Element* next = dynamic_cast<Element*>(elementArray.GetAt(i+1));
					delete next;
					elementArray.RemoveAt(i+1);
				}
				else
					ele->set_id(ROHR);
				ele->set_makeAnlage(true);
			}
		}
	}
	if (err.GetLength())
	{
		AfxMessageBox(err, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

#endif


void Verteiler::rohreBeschriften(CObList& elementList)
{
	if (!rohrDefLaengen.GetLength())
		Rohrleitung::rohreBeschriften(elementList);
	else
	{
		// wanr
		CString str = "", txt = "";
		txt.Format("%06ld", get_status().get_wanr());
			for (int i=2; i<6; i++) 
				str += txt.GetAt(i);

		bool first = false;
		int count = 0, schweissnr = 0, lfdnr = 0;
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
				Element *prev = dynamic_cast<Element*>(elementList.GetAt(p));
				XASSERT(prev);
				if (!prev)
					break;

				Element *pprev = NULL;
				if (count > 2)
				{
					p = elementList.FindIndex(count-2); 
					pprev = dynamic_cast<Element*>(elementList.GetAt(p));
					XASSERT(pprev);
					if (!pprev)
						break;
				}

				p = elementList.FindIndex(count+1); 
				Element *next = dynamic_cast<Element*>(elementList.GetAt(p));
				XASSERT(next);
				if (!next)
					break;

				if (prev->isEleEnde() || prev->get_id() == GLATT && pprev && pprev->get_id() == SNAHT)
					first = true;

				if (first)
				{
					schweissnr = 0;
					lfdnr++;
					first = false;
				}
				
				if (schweissnr == 0)
					links.Format("%s-%s.%d", str, get_posNr(), lfdnr);
				else
					links.Format("%s.%d", get_posNr(), lfdnr);

				element->set_beschriftungLinks(links);
				
				if (next->isEleRedBogEinz() || next->get_id() == SCHWEISS || prev->get_id() == LEER)
				{
					if (schweissnr == 0)
						links.Format("%s-%s.%d.%d", str, get_posNr(), lfdnr, ++schweissnr);
					else
						links.Format("%s.%d.%d", get_posNr(), lfdnr, ++schweissnr);
					element->set_beschriftungLinks(links);
				}
			}
			count++;
		}
	}
}

/*
Änderung:
04.04.06, Jacobs: wird nicht mehr gebraucht
*/
/**
gibt die Teillänge zurück, die in Def.Laengen gesetzt sind. 
*/
/*
const int Verteiler::get_defLaengen(int index) const
{
	if (index >= m_defLaengen.GetSize())
		return 0;

	return m_defLaengen[index];
}
*/

///Ermittelt die Anzahl der Rohre nach Nennweiten fuer Kranzeit
void Verteiler::setAnzahlNennweiten(bool mitVerlaengerung)
{
	///Wenn keine Rohrteilung dann allgemein, ansonsten sollen nur die angegebenen Rohrlaengen
	///mit der jeweiligen Anfangsnennweite gezaehlt werden. 
	if (!rohrDefLaengen.GetLength() && !mitVerlaengerung)
	{
		Rohrleitung::setAnzahlNennweiten();
		return;
	}
	
	///erstmal alte loeschen
	anzahlNennweiten.RemoveAll();
		
	///initialisieren fuer DN 25-65, 80-125, 150-200, 250-400
	for (int i=0; i<4; i++)
		anzahlNennweiten.Add(0);
/*
	///sind verschiedene Rohrnennweiten vorhanden
	bool red = false;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRedEinz()) 
		{
			red = true;
			break;
		}
	} 
	
	///neu setzen
	if (red)
	{
*/		// Hier wird zunaechst jedes Rohstueck abgefragt
		// Bewertet werden soll nur das 1. Rohrstuek einer angegebenen Rohrlaenge
		int rohrcounter = 0; 
		bool schweissen = false;
		double gewicht = 0.0;
		int laenge = 0, dn = 0;
		ElementList elelst(elementList);
		for (int count=0; count<elelst.get_count(); count++)	
		{
			Element *next = NULL, *prev = NULL, *pprev = NULL;
			// Element aus der Liste 
			Element *ele = dynamic_cast<Element*>(elelst.get_at(count));
			XASSERT(ele);
			if (!ele)
				break;

			if (count > 0) 
			{
				prev = dynamic_cast<Element*>(elelst.get_prev(count));
				XASSERT(prev);
				if (!prev)
					break;
			}
			if (count > 1) 
			{
				pprev = dynamic_cast<Element*>(elelst.get_prevprev(count));
				XASSERT(pprev);
				if (!pprev)
					break;
			}
			if (count < elelst.get_count()-1)
			{
				next = dynamic_cast<Element*>(elelst.get_next(count));
				XASSERT(next);
				if (!next)
					break;
			}

			// Glatt nur bei Anfang/Ende, hinter Schweiss, Bog & Red. kommt Leer
			// Wenn Element Anfang/Ende
			if (ele->get_id() == SNAHT || ele->isEleEnde()) 
			{
				// Wenn nicht letztes Element
				if (next && next->isEleRohr())
				{
					if (schweissen)
						set_anzahlNennweiten(dn, laenge, gewicht);		
					rohrcounter = 0;
					schweissen = false;
					gewicht = 0.0;
				}
			} 

			// Gesamtgewicht einer angegebenen Rohrlaenge
			gewicht += ele->getGewicht() * get_anzahl();

			if (ele->get_id() != NUT && ele->get_id() != GEW && ele->get_id() != GLATT && ele->get_id() != LEER && !ele->isEleRohr())
				schweissen = true;

			if (ele->isEleRohr()) 
			{
				rohrcounter++;

				if (rohrcounter > 1 && mitVerlaengerung)
				{
					// Lieber gleich mit Gewicht, das ist fuer alle gleich ?
					if (pprev && pprev->isEleRedBog())
						set_anzahlNennweiten(ele->get_dn(), pprev->get_laenge()+ele->get_laenge(), ele->getGewicht()*get_anzahl(), mitVerlaengerung);		
					else
						set_anzahlNennweiten(ele->get_dn(), ele->get_laenge(), ele->getGewicht()*get_anzahl(), mitVerlaengerung);		
				}
								
				if (!ele->get_uelementList().IsEmpty())
				{
					schweissen = true;
					for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
					{
						Element* uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(upos));					
						gewicht += uele->getGewicht() * get_anzahl();
					}
				}
				
				// Rohrstuecke nach Red. od. Ueberlaenge werden nicht ausgewertet 	
				if (prev && prev->get_id() == SCHWEISS || pprev && pprev->get_id() == SCHWEISS)
					continue;
				
				dn = ele->get_dn();
				laenge = ele->get_laenge();
			}
		}
		if (schweissen)
			set_anzahlNennweiten(dn, laenge, gewicht);		
/*	}
	else
	{
		///Hier werden nur die angegebenen Rohrlaengen ausgewertet  
		i=0;
		do
		{
			set_anzahlNennweiten(get_dnvl(), defLaengen[i], getGesGewicht());		
			i++;
		}
		while (defLaengen[i] != 0);
	}*/
}

/*
Änderungen:
04.04.06, Jacobs: Ermittlung der abslaengen überarbeitet

*/
void Verteiler::check_elementArray()
{
	///Todo: Bereich um die Rohrtrennstelle muss genauer gepueft werden.
	///Dazu ist der Elementdurchmesser in mm zuberuecksichtigen!
	///Rohrlaengen def.?, dann auf Elementposition pruefen
	CString err, str, txt;
	CArray<int, int> abslaengen;

	if (rohrDefLaengen.GetLength() > 0) 
	{
		if (setrohrDefLaengen())
		{
			err = setDefRohrlaengen();
			if (err.GetLength() > 0)
				throw err;
		}

		int abs=0;
		int i;
		for (i=0; i<m_defLaengen.GetSize(); i++)
		{
			abs += m_defLaengen[i];
			abslaengen.Add(abs);
		}

		for (i=0; i < elementArray.GetSize(); i++)
		{
			str.Format(getStammdaten().getText("IDS_FORMAT_ELE_ERROR").c_str(), get_posNr(), i+1);
			Element *ele = elementArray.get_element_at(i);
			
			for (int j=0; j<abslaengen.GetSize(); j++)
			{
				if (ele->get_abstAbs() == abslaengen.GetAt(j))
				{
					txt = getStammdaten().getText("IDS_ERROR130").c_str();  //Absoluter Abstand liegt bei Rohrtrennstelle!
					err = str + txt;
					if (err.GetLength() > 0)
						throw err;
				}
			}
		}
	}
	else
	{
		err = setAutoRohrlaengen();
		if (err.GetLength() > 0)
			throw err;
	}
	
	Rohrleitung::check_elementArray();
}

/*
Änderung:
04.04.06, Jacobs: komplett überarbeitet
*//**
Liefert die Anzahl der Rohrleitungen
*/
int Verteiler::getAnzDefLaengen()
{
	// Im Rohrlaengenfeld werden die Laengen und somit die Anzahl
	// der Rohrleitungen festgelegt
	setrohrDefLaengen();
	int i = m_defLaengen.GetSize();

	return (i==0)?1:i;
}

///Standard checken
string Verteiler::checkStdVl()
{
	string err = "",
		    gesErr = "";

	// Erstmal auf false gesetzt, damit die Eingabe bei Fehlern nicht unmöglich wird
	const bool returnError = false;

	// Rohre checken
	err = checkRohre();
	if (err.length() > 0)
	{
		if (returnError)
			gesErr += err;
		else
			getErrorHandler().appendErrorDescription(err, MxError::Warning, __FILE__, __LINE__);
			
	}

	// Standardelemente extra checken
	err = checkStdElemente();
	if (err.length() > 0)
	{
		if (returnError)
			gesErr += err;
		else
			getErrorHandler().appendErrorDescription(err, MxError::Warning, __FILE__, __LINE__);
			
	}

	// Abstaende der Standardelemente checken, nur meckern aber nicht ablehnen
	err = checkAbstandStdElemente();
	if (err.length())
	{
		if (returnError)
			gesErr += err;
		else
			getErrorHandler().appendErrorDescription(err, MxError::Warning, __FILE__, __LINE__);
			
	}

	if (returnError)
		return gesErr;
	else
		return "";
}

///Rohre checken
CString Verteiler::checkRohre()
{
	CString err, str;
	if (get_laenge()-get_rohrAnfaLaenge()-get_rohrEndeLaenge() < A_MIN_ROHRLAENGE)
	{
		str = getStammdaten().getText("IDS_ERROR131").c_str();   //Rohrlänge %d mm < %d mm nicht zulässig!
		err.Format(str, get_laenge()-get_rohrAnfaLaenge()-get_rohrEndeLaenge(), A_MIN_ROHRLAENGE);
		return err;
	}
	
	// Einzelrohre pruefen
	int rohrcounter = 0;
	for (POSITION pos = elementList.GetHeadPosition(); pos;)	
	{
		Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr() && ele->hasRohrStdelemente())
		{
			rohrcounter++;
			// Laenge ?
			if (ele->get_laenge() < A_MIN_ROHRLAENGE)
			{
				str = getStammdaten().getText("IDS_ERROR132").c_str();   //%d. Rohrstück: Länge %d mm < %d mm mit Standardelementen nicht zulässig!
				err.Format(str, rohrcounter, ele->get_laenge(), A_MIN_ROHRLAENGE);
				str = getStammdaten().getText("IDS_BITTE2").c_str(); 
				return err + " " + str;
			}
			// Nennweite ?
			if (ele->get_dn() < A_MIN_DNVL || ele->get_dn() > A_MAX_DNVL)
			{
				str = getStammdaten().getText("IDS_ERROR133").c_str();   //%d. Rohrstück: Nennweite DN %d ungültig, muss zwischen %d und %d sein!
				err.Format(str, rohrcounter, ele->get_dn(), A_MIN_DNVL, A_MAX_DNVL);
				return err;
			}
		}
	}
	return err;
}

/**
Elemente prüfen, die über die Verteilerrohrstraße laufen. (Das Feld Anlage auf Reiter Elemente muß für
das Element dazu gesetzt sein)
*/
CString Verteiler::checkStdElemente()
{
	CString err, str;
	CString bitte; bitte = getStammdaten().getText("IDS_BITTE2").c_str(); 

	int dn = get_dnvl();
	
	int anzahl = 0;

	//Elemente abarbeiten
	for (int i=0; i<elementArray.GetSize(); i++)
	{
		Element* ele = elementArray.get_element_at(i);
		XASSERT(ele);
		if (!ele)
		{
			err = getStammdaten().getText("IDS_ERROR134").c_str();  //Fehler bei Element!
			break;
		}

		if (ele->isEleAbgEnde())
			continue;

		if (ele->isEleRedEinz())
			dn = ele->get_dn();


		//Wenn Element nicht über Anlage läuft zum nächsten Element springen
		if (!ele->get_makeAnlage())
			continue;

		anzahl++;
		
		// Typ checken
		if (ele->get_id() == LEER)  
		{
			err = getStammdaten().getText("IDS_ERROR112").c_str();  //Bitte Typ auswählen!
			return err;
		}

		// Max Laenge Stutzen
		if (ele->get_laenge() > A_MAX_STUTZEN_LAENGE)
		{
			str = getStammdaten().getText("IDS_ERROR135").c_str(); 			//Max. Länge des Rohrstutzens von Rohrmitte ist %d mm.
			err.Format(str, A_MAX_STUTZEN_LAENGE);
			return err + " " + bitte;
		}

		// Min Laenge Stutzen
		if (ele->get_id() == ROHR)
		{
			DNzuAussendurchmesser dntoad;
			int anschlussAussendurchmesser = dntoad.getAussendurchmesserToInt(dn);
			if (ele->get_laenge() < anschlussAussendurchmesser/2+A_MIN_STUTZEN_LAENGE)
			{
				str = getStammdaten().getText("IDS_ERROR136").c_str(); 			//Min. Länge der Rohrstutzen von Rohroberkante ist %d mm.
				err.Format(str, A_MIN_STUTZEN_LAENGE);
				return err + " " + bitte;
			}
		}

		// MuffenDN checken 
		if (ele->get_id() == STDMUFFE || ele->get_id() == MUFFE)
		{
			if (ele->get_dn() < A_MIN_MUFFE_DN || ele->get_dn() > A_MAX_MUFFE_DN)
			{
				str = getStammdaten().getText("IDS_ERROR137").c_str(); 		//DN < %d & > %d bei Muffen ungültig.
				err.Format(str, A_MIN_MUFFE_DN, A_MAX_MUFFE_DN);
				return err + " " + bitte;
			}
		}
		else if (ele->get_id() == STDSTUTZEN || ele->get_id() == ROHR || ele->get_id() == NUTST)
		{
			if (ele->get_dn() < A_MIN_STUTZEN_DN || ele->get_dn() > A_MAX_STUTZEN_DN)
			{
				str = getStammdaten().getText("IDS_ERROR138").c_str(); 		//DN < %d & > %d bei Rohrstutzen ungültig.
				err.Format(str, A_MIN_STUTZEN_DN, A_MAX_STUTZEN_DN);
				return err + " " + bitte;
			}
			if (ele->get_dn() >= dn)
			{
				err = getStammdaten().getText("IDS_ERROR139").c_str();   //Die Nennweite der Stutzen muss kleiner der Nennweite des Rohres sein!
				return err;
			}
		}	
	}

	if (anzahl > MAX_ANZAHL_ABGAENGE)
	{
		str = getStammdaten().getText("IDS_STRING2232").c_str();   //Maximal %d Abgänge über die Anlage
		err.Format(str, MAX_ANZAHL_ABGAENGE);
		return err;
	}


	return err;
}


/**
Folgende Prüfungen für Abgänge, die über die Schweißrohrstraße laufen. 
(Das Feld Anlage auf Reiter Elemente muß für das Element dazu gesetzt sein)

- Prüfen ob alle Abgänge die auf der Schweißstraße gefertigt werden sollen, den gleichen Winkel haben
- Mindestabstaende zu Rohranfang und -ende zu den Elementen prüfen. 

*//*
Änderungen:
05.04.2006, Jacobs: Bedingungen min. Abstand Rohranfang-Element und Rohrende_Element überarbeitet
*/
string Verteiler::checkAbstandStdElemente()
{
	string err, txt;

	int lfdnr = 0,  eleanz = 0, winkel = -1;

	
	//Elementliste des Bauteils durchlaufen
	for (POSITION pos = get_elementList().GetHeadPosition(); pos != NULL;)	
	{
		Element* ele = dynamic_cast<Element*>(get_elementList().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;
		
		//Wenn Element kein Rohr oder das Rohr keine Unterelemente hat zum nächsten Element springen
		if (!ele->isEleRohr() || !ele->hasRohrStdelemente())
			continue;
			
		lfdnr++;

		eleanz = 0;
		winkel = -1;
		

		// Winkel pruefen (alle Abgänge auf dem Rohr, die über die Schweißstraßestraße laufen, müssen
		// den gleichen Winkel haben
		for (POSITION upos = ele->get_uelementList().GetTailPosition(); upos != NULL;)	
		{
			Element* uele = dynamic_cast<Element*>(ele->get_uelementList().GetPrev(upos));
			XASSERT(uele);
			if (!uele)
				break;

			//nur Unterelemente prüfen, die über die Schweißstraße laufen
			if (uele->get_makeAnlage())
			{
				eleanz++;
				txt = str(format(getStammdaten().getText("IDS_ROHR_ELE")) % lfdnr % eleanz);

				// Winkel
				if (winkel == -1)
					winkel = uele->get_winkel();

				if (winkel != uele->get_winkel())
				{
					//"Winkel der Standardelemente auf einem Rohr muss gleich sein!
					return txt + getStammdaten().getText("IDS_ERROR140");
				}
			}
		}
	}


	/*
	Prüfen ob Längen für Rohranfang und Rohrende eingehalten werden. (für Schweißstraßen). 
	- Die Mindesabstände beziehen sich auf die Achse des Abgangs. 
	*/
	err = check_abstandRohranfangEnde(A_MIN_ABSTAND_ANFANG,A_MIN_ABSTAND_ENDE,
		                              0,0,
									  A_MIN_ABSTAND_ENDE_DN125,125,
									  true, true, false);
	if (!err.empty())
		return err;

	err = check_abstandZwischenStutzen(A_MIN_ZWISCHEN_STUTZEN,true,false).c_str();

	return err;
}



///getMaxAnzahlStdelemente
const int Verteiler::getMaxAnzahlStdelemente() const 
{
	// Bei Verteiler unbegrenzt
	return get_stdelementArray().GetSize();
}

///getMuffenRelAbst
const int Verteiler::getMuffenRelAbst(int index) const
{
	const Element* const ele = get_stdelementArray().get_element_at(index);
	if (!ele)
		return 0;
	return ele->get_abstRel();
}

///getMuffenRestabstand
const int Verteiler::getMuffenRestabstand() const
{
	const Element* const ele = get_stdelementArray().get_element_at(get_stdelementArray().GetSize()-1);
	if (!ele)
		return 0;
	return get_laenge() - ele->get_abstAbs();
}


///Erzeugen der Afos fuer die Beschichtung
void Verteiler::createBeschAfos(Arbeitsplan& plan)
{
	// Beschichtung
	if (!get_beschichtung().isVerzinkt() || (get_rohrQuali() != RQ2440VZIN  && get_rohrQuali() != RQEN10217VERZ))
		plan.addAfo_Beschichtung(get_anzahl(), get_beschichtung(), getPulverzeit(), getGrundierungszeit(), getPutzzeit());
}

///Rohrqualitaet checken
void Verteiler::check_rohrQuali(int dnvl) const
{
	CString err, str;
	Element ele;
	ele.set_id(ROHR_Q1 + get_rohrQuali() - 1); 

	int quali = get_rohrQuali();
	// Auf Laenge pruefen, wenn Rohr nicht unterteilt ist 
	if (quali == RQ2458GSCH 
		&& (get_laenge() > 9000) && (dnvl < 50 || getLastRohrDN() < 50))
	{
		switch (quali)
		{
		case RQ2458GSCH:
			if (getBeschichtung().isGrundiert() && (dnvl < 65 || getLastRohrDN() < 65))
			{
				str = getStammdaten().getText("IDS_ERROR56").c_str();   //und DN < 65 nicht grundiert erhältlich!
				throw (CString)(ele.get_bezeichnung() + str);
			}
			else if (!m_bUeberlaenge && (get_laenge() > 9000 && (dnvl < 50 || getLastRohrDN() < 50))) 
			{
				if (rohrDefLaengen.GetLength() == 0) 
				{
					str = getStammdaten().getText("IDS_ERROR57").c_str();  //und DN 32/40 und Länge > 9000 ungültig!
					throw (CString)(ele.get_bezeichnung() + str);
				}
			}
			else if (dnvl < 32 || getLastRohrDN() < 32) 
			{
				str = getStammdaten().getText("IDS_ERROR58").c_str();  // < DN 32 ungültig!
				throw (CString)(ele.get_bezeichnung() + str);
			}
			break;
		
		} 
	}
	else
		Rohrleitung::check_rohrQuali(dnvl);
}

///Rohrqualitaet bei Rohrstutzen setzen
void Verteiler::setQualiRohrstutzen(CObList& templist)
{
	///Sollen standardmaessig in der selben Quali wie das Rohr ausgefuehrt werden
	for (POSITION pos = templist.GetHeadPosition(); pos;)	
	{
		Element *ele = dynamic_cast<Element*>(templist.GetNext(pos));
		XASSERT(ele);
		if (!ele) 
			return;


		//Rohrquali wird nur geändert, wenn der Standardstutzen vorher ein Rohrstutzen war. 
		if (ele->get_id() == ROHR || (ele->get_id() == STDSTUTZEN && ele->get_urid() == ROHR))
		{
			if( ele->get_urid() == GEW )
			{
				if ( 25 <= ele->get_dn() && ele->get_dn() <= 50 )
				{
					//Artikelnummer für Qualli 2440 ermitteln
					Element tmpEle;
					tmpEle.initStammdaten(ele->get_dn(),ele->get_dn(), ROHR_Q10);
					ele->set_artikelnr(tmpEle.get_artikelnr());	
					ele->set_kurztext(tmpEle.get_kurztext());
				}

				if(ele->get_dn() > 50)
				{
					Element tmpEle;
					tmpEle.initStammdaten(ele->get_dn(),ele->get_dn(), ROHR_Q8);
					ele->set_artikelnr(tmpEle.get_artikelnr());	
					ele->set_kurztext(tmpEle.get_kurztext());
				}
			}
			//Wenn Rohrstutzen Dn32-50 über Anlage läuft, dann Quali 2440
			else if (ele->get_makeAnlage() && ele->get_dn() >= 32 && ele->get_dn() <= 50)
			{
				//Artikelnummer für Qualli 2440 ermitteln
				Element tmpEle;
				tmpEle.initStammdaten(ele->get_dn(),ele->get_dn(),ROHR_Q10);
				ele->set_artikelnr(tmpEle.get_artikelnr());	
				ele->set_kurztext(tmpEle.get_kurztext());
			}
			else //sonst Quali von Hauptrohr übernehmen
			{
				ele->set_id(get_rohrQuali()+ROHR_Q1-1);
				ele->initStammdaten(get_beschichtung().isVerzinkt());
			}
		}
	}
}

