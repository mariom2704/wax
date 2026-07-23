#include "stdafx.h"


#include "ConstructGewinderohr.h"
#include "constructelement.h"
#include "bemassung.h"
#include "rohrqx.h"

#include "..\rohrkonst\elementlist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructGewinderohr::ConstructGewinderohr(Rohrleitung* rohr, WaxGrafik::GRAFIKTYP typ, const std::string& fn)
: ConstructGraphik(rohr, typ, fn)
{
	abstand = 0;
}

void ConstructGewinderohr::createGraphik()
{
	// Graphikfaktoren berechnen
	setGraphikfaktoren();
	
	// Elementlist scannen
	runElementList();

	// Serialisierung der Graphikelemente
	serializeGraphicElements(filename);
}

void ConstructGewinderohr::setGraphikfaktoren()
{
	faktorRohrlaenge = 1;
	faktorRohrbreite = 1;
	faktorElelaenge  = 1;
	faktorElebreite  = 1;
	dn2zoll = (WaxSys::para_int(DN2ZOLL) == 0) ? false : true;

	ConstructElement::setGraphikfaktoren(faktorRohrlaenge,	faktorRohrbreite, faktorElelaenge, faktorElebreite, dn2zoll);
}

void ConstructGewinderohr::runElementList()
{
	bool erstesElement = true;
	int rohrDn     = rohrleitung->get_dnvl();
	int rohrLaenge = rohrleitung->get_laenge();
	int rohrAnzahl = rohrleitung->get_anzahl();
	CString rohrPos= rohrleitung->get_posNr();

	// Rohr mit Einziehung
	int einz = 0;

	ElementList elelst(rohrleitung->get_elementList());
	// Abarbeiten der Elementliste
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *ele = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(ele);
		if (!ele)
			break;

		// Merker fuer aktuelles Element 
		elemerk.copy(*ele);

		// vorheriges und naechstes Element fuer diverse Abfragen
		Element* prevele = NULL;
		Element* nextele = NULL;
		if (count > 0) 
			prevele = elelst.get_prev(count);
		if (count < elelst.get_count()-1) 
			nextele = elelst.get_next(count);
		if (prevele)
			prevelemerk.copy(*prevele);
		if (nextele)
			nextelemerk.copy(*nextele);

		// Wenn Element Anfang/Ende
		if (elemerk.isEleEnde()	&& (count < elelst.get_count()-1) && nextele->isEleRohr())
			erstesElement = true;
		else if (elemerk.isEleRohr()) 
		{
			// Init Bemassung
			initBemassung(&elemerk);
			restmassBemassen = true;
			rohrcounter++; 
	
			// Setzen der Laenge fuer linkes Rohrende 
			if (prevele && prevele->get_id() != GRFITTING)
				lEndeLaenge = prevele->get_laenge();

			// Setzen der Laenge fuer rechtes Rohrende 
			if (nextele && nextele->isEleEnde()) 
				rEndeLaenge = nextele->get_laenge();
		}

		createRohrList(rohrLaenge, rohrDn, rohrPos, rohrAnzahl, einz, erstesElement);   
		erstesElement = false;

		// Nur Fittinge koennen Fittinge aufnehmen
		if (ele->get_id() == GRFITTING)
		{
			CObList& uelementList = ele->get_uelementList();
			for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
			{
				Element *uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
				XASSERT(uele);
				if (!uele)
					break;

				elemerk.copy(*uele);					
				abgelemerk.copy(*uele);					
				if (elemerk.get_id() == GRFITTING)
					createRohrList(rohrLaenge, rohrDn, rohrPos, rohrAnzahl, einz, erstesElement);   
				else
					createAbgList(rohrLaenge, rohrDn, false , false);   

				CObList& uuelementList = uele->get_uelementList();
				for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)	
				{
					Element *uuele = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
					XASSERT(uuele);
					if (!uuele)
						break;

					elemerk.copy(*uuele);
					if (elemerk.get_id() == GRFITTING)
						createRohrList(rohrLaenge, rohrDn, rohrPos, rohrAnzahl, einz, erstesElement);   
					else
						createAbgList(rohrLaenge, rohrDn, false, false);   
				}
			}
		}
	}
}

void ConstructGewinderohr::createRohrList(int rohrLaenge, int rohrDn, CString rohrPos, int rohrAnzahl, int einz, const bool erstesElement)
{
	static int x = 0;
	laenge = 0;
	int typ = IMROHR;

	// hier beginnt eine neue Rohrkonstruktion 
	if (erstesElement) 
	{
		if (xLaenge != 0)
		{
			if (prevelemerk.get_id() == GLATT || prevelemerk.isEleEnde())
		    {
				if (restmassBemassen)
				{
					doRestmassBemassen(erstesElement);
					restmassBemassen = false;
				}
			}
			xLaenge = 0;
			yLaenge = 0;
			negxLaenge = 0;
			negyLaenge = 0;
		}
		
		if (prevelemerk.get_id() != Element::LEER) 
		{ 
			Y0 -= maxlaenge_270 + 12*Bemassung::EBENENABSTAND;
			X0 = 0;
			drehfaktor = 0;
			nRohre++;
		}
	} 

	// Gesamtlaenge in X+Y-Richtung setzen
	setNewXYLaenge();
	
	// Gesamtlaenge der bisher abgebildeten Rohre
	if (nRohre>0) 
		for (int i=0; i<nRohre; i++) 
			laenge += rohrleitung->get_rohrLaengen(i);

	// Was soll bei welchem Element abgebildet werden
	bool beschriftung = true;
	bool bemassung = false;
	bool create_element = true;

	if (elemerk.get_id() == GEW || elemerk.get_id() == NUT || elemerk.get_id() == GLATT)
	{
		elemerk.set_winkel(drehfaktor);
		
		if (!erstesElement && elemerk.get_abstRel() > 0)
			bemassung = true;
	} 
	if (elemerk.get_id() == GRFITTING)
	{
		if (elemerk.get_nurFitting() == 0)  
			bemassung = true;
		if (elemerk.get_abstRel() > 0)
			bemassung = true;
		if (elemerk.get_verkettung() != 0)
		{
			create_element = false;
			beschriftung = false;
			bemassung = false;
		}
	}
	else if (elemerk.isEleRohr()) 
	{
		elemerk.set_winkel(drehfaktor); 
		bemassung = false;

		if (prevelemerk.get_id() != Element::LEER)
			beschriftung = true;
		else if (rohrleitung->isRed())
			beschriftung = true;
		else
			beschriftung = false;
	} 

	// Elemente konstruieren, bemassen, beschriften
	ConstructElement *ce = constructElementFromID(elemerk);
	if (ce)
	{
		WaxGrafik::RohrQx* rx = dynamic_cast<WaxGrafik::RohrQx*>(ce);
		if (rx)
		{
			// Der Abstand vom letzten Element mit Rohr setzen
			X0 = abstand;
			if (rohrcounter == 1)
			{
				rx->set_show_text(true); 
				rx->set_show_dn(true);
			}
			else 
				rx->set_show_text(false); 
			rx->init_rohrqx((WaxGrafik::RohrQx::Einziehung)einz);
		}
		ce->rohrleitung = rohrleitung;
		ce->initEleDatenAbgMerk(merkerAbgele);
		ce->initEleDatenMerker(merkerRohrele);
		ce->initEleDaten(elemerk);
		
		ce->set_maxRohrDN(rohrleitung->getMaxRohrDN());
		ce->initGraphikDaten(X0, Y0, drehfaktor, bogenlaenge, rohrleitung->isBogen(), erstesElement, laenge, lEndeLaenge, rEndeLaenge, typ);
		ce->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
						  maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
						  maxlaengeAbg, rohrDn);
		
		if (create_element)
			ce->createElement();
		
		if (beschriftung)
			ce->createBeschriftung();
		
		if (bemassung)
			ce->createBemassung();
		
		cg_list.push_back(ce);
	}
	if (elemerk.isEleEnde())
		merkerRohrele.copy(elemerk);
		
	else if (elemerk.get_id() == GRFITTING)
	{
		// neuen Rohranfangspunkt setzen
		setNewPoint();

		// Drehfaktor bestimmen 
		if ((elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL		 && elemerk.get_verkettung() == 0)
		 || (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_30    && elemerk.get_verkettung() == 0)
		 || (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_A1_45 && elemerk.get_verkettung() == 0)
		 || (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_A4 	 && elemerk.get_verkettung() == 0))
			drehfaktor = rohrleitung->setNewDrehfaktor(drehfaktor, elemerk.get_winkel());

		 if (elemerk.get_verkettung() == 0)
			 merkerAbgele.copy(elemerk);
	}
	
	// ID vorheriges Element merken
	prevelemerk.set_id(elemerk.get_id()); 
}

void ConstructGewinderohr::setNewPoint()
{
	if ((elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL       && elemerk.get_verkettung() == 0)
	 || (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_30    && elemerk.get_verkettung() == 0)
	 || (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_A1_45 && elemerk.get_verkettung() == 0)
	 || (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_A4    && elemerk.get_verkettung() == 0))
		setPointBehindWinkel();
	else 
		setPointBehindFitting();
}

void ConstructGewinderohr::setPointBehindWinkel()
{
	int andn = elemerk.get_anschlussDN();
	int dn = elemerk.get_dn();

	// unten und nach rechts
	if (elemerk.get_winkel() == 0 && drehfaktor == 270) 
	{
		Y0 = Y0 - elemerk.get_abstRel() - andn; 
		X0 = X0 + andn;
	} 
	// unten und nach links	
	else if (elemerk.get_winkel() == 180 && drehfaktor == 270) 
	{
		Y0 = Y0 - elemerk.get_abstRel() - andn; 
		X0 = X0 - andn;
	}
	// oben und nach links	
	else if (elemerk.get_winkel() == 0 && drehfaktor == 90) 
	{
		Y0 = Y0 + elemerk.get_abstRel() - dn*2/5; 
		X0 = X0 - andn;
	} 
	// oben und nach rechts	
	else if (elemerk.get_winkel() == 180 && drehfaktor == 90) 
	{
		Y0 = Y0 + elemerk.get_abstRel() - dn*2/5; 
		X0 = X0 + andn;
	} 
	// unten und nach rechts	
	else if (elemerk.get_winkel() == 0 && drehfaktor == 180) 
	{
		Y0 = Y0 + elemerk.get_abstRel() - dn*2/5; 
		X0 = X0 + andn;
	} 
	// unten und nach links	
	else if (elemerk.get_winkel() == 180 && drehfaktor == 180) 
	{
		Y0 = Y0 + elemerk.get_abstRel() - dn*2/5; 
		X0 = X0 + andn;
	}
	// nach oben	
	else if (elemerk.get_winkel() == 0 && drehfaktor == 0) 
	{
		Y0 = Y0 + andn; 
		X0 = X0 + elemerk.get_abstRel() - dn*2/5;	
	}
	// nach unten
	else if (elemerk.get_winkel() == 180 && drehfaktor == 0)
	{
		Y0 = Y0; 
		X0 = X0 + elemerk.get_abstRel() - dn*2/5;	
	}
	// nach hinten
	else if (elemerk.get_winkel() ==  90 && drehfaktor == 0) 
	{
		Y0 = Y0; 
		X0 = X0 + elemerk.get_abstRel() - dn*2/5;	
	} 
	// nach vorn
	else if (elemerk.get_winkel() == 270 && drehfaktor == 0) 
	{
		Y0 = Y0; 
		X0 = X0 + elemerk.get_abstRel() - dn*2/5;	
	}
	if (elemerk.get_abstRel() > 0)
		abstand = X0;
}

void ConstructGewinderohr::setPointBehindFitting()
{
	int abst = 0;
		
//	if (elemerk.get_abstRel() == 0)
//		abst = elemerk.get_abstAbs();
//	else
		abst = elemerk.get_abstRel();
		
	if (drehfaktor == 0)
		X0 = X0 + abst/faktorRohrlaenge;
	else if (drehfaktor == 180)
		X0 = X0 - abst/faktorRohrlaenge;
	else if (drehfaktor == 90)
		Y0 = Y0 + abst/faktorRohrlaenge;
	else if (drehfaktor == 270)
		Y0 = Y0 - abst/faktorRohrlaenge;

	// Der Abstand vom letzten Element mit Rohr wird gemerkt,
	// damit das naechste Rohr dort seinen Startpunkt bekommt,
	// sodass die Bemassung trotz zwischenliegenden Fittingen ohne Rohr
	// lueckenlos von Mitte Fitting bis Mitte Fitting geht.
	if (elemerk.get_abstRel() > 0)
		abstand = X0;
}
