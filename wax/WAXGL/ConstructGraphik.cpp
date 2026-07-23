#include "stdafx.h"


#include "constructgraphik.h"
#include "constructelement.h"
#include "constructrohr.h"
#include "rohrqx.h"
#include "bemassung.h"
#include "ConstructBoden.h"
#include "ConstructKbod.h"
#include "ConstructGewind.h"
#include "ConstructNut.h"
#include "ConstructVflan.h"
#include "ConstructFflan.h"
#include "ConstructCOVflanWiththread.h"
#include "ConstructCOVflanWiththreadWithMuffe.h"
#include "ConstructCOVflanGegenAbgang.h"
#include "Ered.h"
#include "ConstructKred.h"
#include "ConstructBogen.h"
#include "ConstructBohr.h"
#include "ConstructGlatt.h"
#include "ConstructKStueck.h"
#include "ConstructTStueck.h"
#include "ConstructDoni.h"
#include "ConstructMuffe.h"
#include "ConstructWinkel.h"
#include "ConstructScheib.h"
#include "ConstructKnick.h"
#include "ConstructCOBockWinkel.h"
#include "ConstructCOBlflan.h"
#include "ConstructCOVGegenflanHautrohr.h"
#include "textplot.h"
#include "resource.h"


#include "..\rohrkonst\elementlist.h"


#include <mxutil\strloader.h>
#include <mxutil\tracer.h>
using namespace MxUtil;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class DrawingException
{
public:
	DrawingException(const std::string& str) { cause = str; }

	std::string cause;
};


///Konstruktor
ConstructGraphik::ConstructGraphik( Rohrleitung* rohr, enum WaxGrafik::GRAFIKTYP tp, const string& graficfile) : rohrleitung(rohr)
{
	grafiktyp = tp;

	nRohre = 0;
	lEndeLaenge = 0;
	rEndeLaenge = 0; 
	drehfaktor = 0;
	bogenlaenge = 0;
	laenge = 0;
	
	avnachbearbeitung = rohr ? rohr->get_avnachbearbeitung() : false;
	maxlaengeAbgOben = 0;

	X0 = 0; Y0 = 0;
	xLaenge = 0;
	negxLaenge = 0;
	yLaenge = 0;
	negyLaenge = 0;

	restmassBemassen = false;
	rohrcounter = 0;
	

	maxlaenge_0 = 0;
	maxlaenge_90 = 0;
	maxlaenge_180 = 0;
	maxlaenge_270 = 0;
	maxlaengeAbg_0 = 0;
	maxlaengeAbg_90 = 0;
	maxlaengeAbg_180 = 0;
	maxlaengeAbg_270 = 0;

	maxlaengeAbg = 0;

	abstKnick = 0;
		
	init_filename(graficfile);

	scan_rohrteilung();
}

//Destruktor
ConstructGraphik::~ConstructGraphik()
{
	for (std::vector<ConstructElement*>::iterator it = cg_list.begin(); it != cg_list.end(); it++)
		delete *it;
}

//Einstiegsfunktion zur Graphik
void ConstructGraphik::createGraphik()
{
	// Graphikfaktoren berechnen
	setGraphikfaktoren();
	
	try
	{
		// Elementlist scannen
		runElementList();
	}
	catch (DrawingException e)
	{
		string str = e.cause;
	}

	// AV-Elemente bemassen
	if (avnachbearbeitung && rohrleitung->get_avelementArray().GetSize() > 0 && !rohrleitung->isBogen())
		aveleBemassen();
	
	


	// Zum Schluss ggbf. das letzte Rohr bemassen
	// (drehfaktor == -90 || drehfaktor == -270) = Bogen nach vorne oder hinten
	if (xLaenge > 0)
	{
		if (prevelemerk.get_id() == SNAHT || prevelemerk.get_id() == SCHWEISS || prevelemerk.isEleEnde()
			|| elemerk.isEleEnde() && (drehfaktor == -90 || drehfaktor == -270))
		{
			if (drehfaktor == -90 || drehfaktor == -270)
				drehfaktor = 0;
			if (restmassBemassen)
			{
				doRestmassBemassen(false);
				restmassBemassen = false;
			}
			gesamtmasseSetzen();
		}
	}

    //Knicke mit eigener Maßkette bemaßen (avnachbearbeitung auf False setzen, damit Knicke auf jeden Fall bemaßt werden)
	//Wenn Rohr Bogen oder Reduzierung enthält keine Extra Maßkette, wegen der Übersicht
	if (!rohrleitung->isRedBogEinz())
	{
		bool tmp = avnachbearbeitung;
		avnachbearbeitung = false;
		knickeBemassen();
		avnachbearbeitung = tmp;
	}

	checkBemassung();
	
	// Serialisierung der Graphikelemente
	serializeGraphicElements(filename);
}

bool ConstructGraphik::knickeBemassen()
{
	int rohrDn     = rohrleitung->get_dnvl();
	int rohrLaenge = rohrleitung->get_laenge();

	this->laenge = 0;
	this->X0 = 0;
	this->nRohre = 0;
	bool hasKnicke = false;
	double merkeX0 = 0;
	int lastAbsKnick = 0;

	ElementList elelst(rohrleitung->get_elementList());
	// Abarbeiten der Elementliste
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *ele = elelst.get_at(count);

		// Merker fuer aktuelles Element
		elemerk.copy(*ele);

		// vorheriges und naechstes Element fuer diverse Abfragen
		if (elelst.get_prev(count))
			prevelemerk.copy(*elelst.get_prev(count));

		if (elelst.get_next(count))
			nextelemerk.copy(*elelst.get_next(count));


		if (ele->get_id() == SCHWEISS)
		{	
			// neuen Rohranfangspunkt setzen
			if (prevelemerk.isEleRohr() && nextelemerk.get_id() != EINZ) 		
				setNewPoint();

			merkeX0 = this->X0;

			if (!hasKnicke)
				this->X0 = 0;
			else
			{
				this->laenge = ele->get_abstAbs();
			}

		
			nRohre++;
		}
	
		if (!ele->isEleRohr())
			continue;

		CObList& uelementList = ele->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uele);
			if (!uele)
				break;
			
			if (!uele->isKnick())
				continue;	

			int abs = 0;
			if (hasKnicke)
				abs = lastAbsKnick;

			// Merker fuer aktuelles Element
			elemerk.copy(*uele);			
			
			// Merker fuer Abgangselement	
			abgelemerk.copy(*uele);		
			
			//Relativen Abstand neu berechnen.
			elemerk.set_abstRel(elemerk.get_abstAbs() - abs);
			abgelemerk.set_abstRel(abgelemerk.get_abstAbs() - abs);
			
			lastAbsKnick = uele->get_abstAbs();
			hasKnicke = true;
			
			// Abgang zeichnen
			createAbgList(rohrLaenge, rohrDn, true, true); 
		}

		//letzten Knick bemaßen
		if (hasKnicke && nextelemerk.get_id() != SCHWEISS)
		{
			this->X0 = merkeX0;

			doRestmassBemassen(false);

			hasKnicke = false;
		}
	}
	return true;
}


void ConstructGraphik::init_filename(const string& fn)
{
	if (fn.length())
		filename = fn;
	else
		filename = WaxSys::GetTempFilename(get_fileextension());
}


/**
Grafikfaktoren ermitteln
*//*
Änderungen:
04.04.06, Jacobs: Ermittung der Max. Rohrlänge überarbeitet
*/
void ConstructGraphik::setGraphikfaktoren()
{
	// Graphikfaktor holen, bei Bogen in der Konstruktion wird immer 1 gesetzt
	dn2zoll = (WaxSys::para_int(GF_DN2ZOLL) == 0) ? false : true;

	const Verteiler* vert = dynamic_cast<const Verteiler*>(rohrleitung);

	int laenge = 0;
	int dnvl		= rohrleitung->getMinRohrDN();
	int restabstand = rohrleitung->get_restabstand();

	// Bei Verteiler mit Rohrteilung laengstes Einzelrohr ermitteln, sonst normale Gesamtrohrlänge
	if (vert)
		laenge = vert->getMaxRohrlaenge();
	else
		laenge = rohrleitung->get_laenge();	


	// Auf zu geringen Abstand des 1. u. letzten Abgangs zum Anfang/Ende pruefen
	const Element* first = rohrleitung->getFirstAbgang();
	bool abstandZuKlein = false; 
	if (first && (first->get_abstAbs() <  50 && laenge > 3000
			   || first->get_abstAbs() < 100 && laenge > 5000
		 	   || first->get_abstAbs() < 150 && laenge > 7000
			   || first->get_abstAbs() < 200 && laenge > 9000
			   || first->get_abstAbs() < 250 && laenge >11000))
		 abstandZuKlein = true;
	
	if (restabstand > 0 && (restabstand <  50 && laenge > 1000
						 || restabstand < 100 && laenge > 3000
						 || restabstand < 150 && laenge > 5000
						 || restabstand < 200 && laenge > 7000
						 || restabstand < 250 && laenge > 9000))
		 abstandZuKlein = true;


	// Wenn Bogen im Rohr dann alles 1 
	if (rohrleitung->isBogen())
	{
		faktorRohrlaenge = 1;
		faktorRohrbreite = 1;
		faktorElelaenge  = 2;
		faktorElebreite = 2*faktorRohrbreite;
	}
	// oder Rohrteilung bei Verteiler
	else if (rohrteilung != -1)
	{
		faktorRohrlaenge = 2;
		faktorRohrbreite = 1;
		faktorElelaenge  = 2;
		faktorElebreite = 2*faktorRohrbreite;
	}
	// sonst berechnen
	else
	{
		faktorElelaenge = laenge > 3000 ? 2 : 1;

		if (dnvl > 100)
			faktorRohrbreite = 0.5;
		else if (dnvl > 40)
			faktorRohrbreite = 1;
		else if (dnvl > 32)
			faktorRohrbreite = 2;
		else 
			faktorRohrbreite = 3;

		faktorElebreite = 2*faktorRohrbreite;
		if (faktorElebreite < 1)
			faktorElebreite = 1;

		const struct {
			int laenge;
			double faktor;
		} laengefaktor[] = {
			500, 0.125,
			1000, 0.25,
			2000, 0.75,
			3000, 1.5,
			4000, 2.0,
			6000, 3.5,
			7000, 4.0,
			8000, 4.5,
			10000, 6.0,
			11000, 6.5,
			12000, 7.0,
			0, 7.0,
		};
		int i = 0;
		for (; laenge >= laengefaktor[i].laenge && laengefaktor[i].laenge; i++);
		faktorRohrlaenge = laengefaktor[i].faktor;

		// Wenn zu kleiner Abstand, dann Faktor fuer Rohrlaenge halbieren,
		// damit Element nicht uebers Ende hinausgeht oder in der Nut sitzt
		if (abstandZuKlein && faktorRohrlaenge > 1)
			faktorRohrlaenge /= 2.0;
	}	

	
	COIIVerteiler* pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(rohrleitung);

	if( pCOIIVerteiler && faktorRohrlaenge > 1.99)
	{
		faktorRohrlaenge = 2.2;
		if(pCOIIVerteiler->has_GegenflanschWithMuffe())
		{
			faktorRohrlaenge = 1.55;
		}
	}
	ConstructElement::setGraphikfaktoren(faktorRohrlaenge,	faktorRohrbreite, faktorElelaenge, faktorElebreite, dn2zoll);
}

//Bemassung der Restmasse
void ConstructGraphik::doRestmassBemassen(bool erstesElement)
{
	int laenge = 0, laenge1 = 0;	
	if (nRohre >= 0)
	{
		int i;
		for (i=0; i<=nRohre; i++) 
			laenge += rohrleitung->get_rohrLaengen(i);
		for (i=0; i<nRohre; i++) 
			laenge1 += rohrleitung->get_rohrLaengen(i);
	}
	if (avnachbearbeitung && rohrleitung->get_avelementArray().GetSize() > 0 && !rohrleitung->isBogen())
	{
		ConstructRohr *rohr1 = new ConstructRohr;
		ConstructRohr *rohr2 = new ConstructRohr;
		rohr1->rohrleitung = rohrleitung;
		rohr2->rohrleitung = rohrleitung;

		//Solange zurückgehen bis kein Knickelement
		Element *ele = NULL;
		for (int i=rohrleitung->get_avelementArray().GetSize()-1; i>=0; i--)
		{
			ele = dynamic_cast<Element*>(rohrleitung->get_avelementArray().get_element_at(i));

			if (!ele->isKnick())
				break;

			ele = NULL;
		}

		if (rohrleitung->get_avelementArray().GetSize() > 0)
		{
			Element eler;

			if (ele != NULL)
			{
				//letztes Maß für AV Teil
				eler.set_abstAbs(laenge);
				eler.set_abstRel(laenge-ele->get_abstAbs());
				rohr1->initEleDaten(eler);
				
				eler.set_abstAbs(ele->get_abstAbs());
				eler.set_abstRel(ele->get_abstRel());
				rohr1->initEleDatenMerker(eler);
				
				rohr1->set_avele(true);
				rohr1->initGraphikDaten(X0, Y0, drehfaktor, bogenlaenge, rohrleitung->isBogen(), erstesElement, 0, lEndeLaenge, rEndeLaenge, ABG);
				rohr1->set_maxRohrDN(rohrleitung->getMaxRohrDN());
				rohr1->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
									 maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
									 maxlaengeAbg, ele->get_anschlussDN());
				rohr1->createBemassung();

				cg_list.push_back(rohr1);
			}

			//Letztes Maß für Standardteil
			eler.set_abstAbs(laenge);
			eler.set_abstRel(rohrleitung->getMuffenRestabstand());
			rohr2->initEleDaten(eler);
			
			eler.set_abstAbs(laenge-rohrleitung->getMuffenRestabstand());
			eler.set_abstRel(laenge-rohrleitung->getMuffenRestabstand());
			rohr2->initEleDatenMerker(eler);
			
			rohr2->initGraphikDaten(X0, Y0, 0/*drehfaktor*/, bogenlaenge, rohrleitung->isBogen(), erstesElement, 0, lEndeLaenge, rEndeLaenge, ABG);
			rohr2->set_maxRohrDN(rohrleitung->get_dnvl());
			rohr2->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
								 maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
								 maxlaengeAbg, rohrleitung->get_dnvl());
			rohr2->createBemassung();
			cg_list.push_back(rohr2);
		}
	}
	else if (abgelemerk.get_abstAbs() > 0)
	{
		ConstructRohr *rohr = new ConstructRohr;
		rohr->rohrleitung = rohrleitung;
		
		Element eler;
		eler.set_id(abgelemerk.get_id());
		eler.set_abstAbs(laenge);
		eler.set_abstRel(laenge-abgelemerk.get_abstAbs());
		rohr->initEleDaten(eler);
		
		eler.set_abstAbs(abgelemerk.get_abstAbs());
		eler.set_abstRel(abgelemerk.get_abstRel());
		rohr->initEleDatenMerker(eler);
		
		rohr->initGraphikDaten(X0, Y0, drehfaktor, bogenlaenge, rohrleitung->isBogen(), erstesElement, laenge1, lEndeLaenge, rEndeLaenge, ABG);
		rohr->set_maxRohrDN(rohrleitung->getMaxRohrDN());
		rohr->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
							maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
							maxlaengeAbg, abgelemerk.get_anschlussDN());
		if ((!rohrleitung->isRohrGlatt() || rohrleitung->isBogen()))
		{
			if (rohrleitung->isRedBogEinz())
				rohr->set_knickBemassung(false);


			COIIVerteiler* pCOIIVerteilerUsedAsBase =  dynamic_cast<COIIVerteiler*>(rohrleitung);
			if(pCOIIVerteilerUsedAsBase)
			{
				pCOIIVerteilerUsedAsBase->SetFirstGraficAbgang(false);
			}
			rohr->createBemassung();
		}
		cg_list.push_back(rohr);
	}
}


//Bemassung der gesamten Konstruktion in X-Richtung
void ConstructGraphik::gesamtmasseSetzen()
{
	ElementList elelst(rohrleitung->get_elementList());

	Element *firstele = elelst.get_at(0);
	XASSERT(firstele);
	if (!firstele)
		return;
		
	int typ = IMROHR;
	int diffx = xLaenge - negxLaenge;
	
	// zuerst X-Laenge bemassen
	WaxGrafik::RohrQx *rohrQx = new WaxGrafik::RohrQx();	
	rohrQx->rohrleitung = rohrleitung;

	Element ele;
	ele.set_laenge((diffx<0)?(-diffx+xLaenge-lEndeLaenge-rEndeLaenge):xLaenge-firstele->get_laenge()-rEndeLaenge);
	ele.set_winkel(0);
	rohrQx->initEleDaten(ele);
		
	if (Y0 < 0)
		rohrQx->initGraphikDaten((diffx<0)?diffx-firstele->get_laenge():0, Y0, 0/*drehfaktor*/, 0, false, false, (diffx<0)?(-diffx+xLaenge-firstele->get_laenge()-rEndeLaenge):xLaenge-firstele->get_laenge()-rEndeLaenge, (diffx<0)?0:firstele->get_laenge(), rEndeLaenge, typ);
	else
		rohrQx->initGraphikDaten((diffx<0)?diffx-firstele->get_laenge():0,  0, 0/*drehfaktor*/, 0, false, false, (diffx<0)?(-diffx+xLaenge-firstele->get_laenge()-rEndeLaenge):xLaenge-firstele->get_laenge()-rEndeLaenge, (diffx<0)?0:firstele->get_laenge(), rEndeLaenge, typ);

	rohrQx->set_maxRohrDN(rohrleitung->getMaxRohrDN());
	rohrQx->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
						  maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
						  maxlaengeAbg, 0, rohrleitung->getBemassungstext());
	
	rohrQx->createBemassung();
	cg_list.push_back(rohrQx);
}

///Bemassung der der Elemente, wenn Rohrleitung mit Standard- und Nacharbeitselementen 
void ConstructGraphik::aveleBemassen()
{
	int rohrDn = 0;
	int abstRel[10], abstAbs[10];
	memset(abstRel, 0, sizeof(abstRel));
	memset(abstAbs, 0, sizeof(abstAbs));

	int i;
	for (i=0; i<rohrleitung->getMaxAnzahlStdelemente(); i++)
		abstRel[i] = rohrleitung->getMuffenRelAbst(i);

	int typ = ABG;
	X0 = 0;
	Y0 = 0;
	
	// Standardelemente bemassen	
	for (i=0; i<rohrleitung->getMaxAnzahlStdelemente(); i++)
	{
		if (abstRel[i] > 0)
		{
			if (i==0)
				abstAbs[i] = abstRel[i];
			else 	
				abstAbs[i] = abstAbs[i-1] + abstRel[i];
		} 
		else 
			abstAbs[i] = 0;

		if (abstRel[i] > 0)
		{
			ConstructRohr *rohr = new ConstructRohr;
			rohr->rohrleitung = rohrleitung;
			
			Element ele;
			ele.set_abstAbs(abstAbs[i]);
			ele.set_abstRel(abstRel[i]);
			rohr->initEleDaten(ele);
			if (i>0)
			{
				ele.set_abstAbs(abstAbs[i-1]);
				ele.set_abstRel(abstRel[i-1]);
				rohr->initEleDatenMerker(ele);
			}
			else 
			{
				ele.set_abstAbs(0);
				ele.set_abstRel(0);
				rohr->initEleDatenMerker(ele);
			}
			rohr->initGraphikDaten(X0, Y0, 0/*drehfaktor*/, bogenlaenge, rohrleitung->isBogen(), false, 0, lEndeLaenge, rEndeLaenge, typ);
			rohr->set_maxRohrDN(rohrleitung->getMaxRohrDN());
			rohr->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
								maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
								maxlaengeAbg, rohrleitung->get_dnvl());
			rohr->createBemassung();
			cg_list.push_back(rohr);
		}
	}

	
	// AVElemente bemassen
	int abstKnick = 0;
	Element elMerken;
	for (i=0; i<rohrleitung->get_avelementArray().GetSize(); i++)
	{
		Element *e = dynamic_cast<Element*>(rohrleitung->get_avelementArray().get_element_at(i));
		
		if (e->get_abstRel() == 0)
			continue;

		if (e->isKnick())
		{
			abstKnick += e->get_abstRel();
			continue;
		}
		
		Element *em = NULL;
		if (i>0)
			em = dynamic_cast<Element*>(rohrleitung->get_avelementArray().get_element_at(i-1));

		Element eCopy;
		eCopy.copy(*e);
		if (abstKnick > 0)
		{
			eCopy.set_abstRel(eCopy.get_abstRel() + abstKnick);
			abstKnick = 0;
		}


		ConstructRohr *rohr = new ConstructRohr;	
		rohr->rohrleitung = rohrleitung;
		rohr->initEleDaten(eCopy);
		rohr->set_avele(true);
		if (i>0)
		{

			//elMerken.copy(*em);
			//if (abstKnick > 0)
			//	elMerken.set_abstRel(elMerken.get_abstRel() + abstKnick);

			rohr->initEleDatenAbgMerk(*em);
			rohr->initEleDatenMerker(*em);
		}
		else 
		{
			Element ele; //leeres Element
			rohr->initEleDatenMerker(ele);
		}
		rohr->initGraphikDaten(X0, Y0, 0/*drehfaktor*/, bogenlaenge, rohrleitung->isBogen(), false, 0, lEndeLaenge, rEndeLaenge, typ);
		rohr->set_maxRohrDN(rohrleitung->getMaxRohrDN());
		rohr->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
							maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
							maxlaengeAbg, rohrDn);
		rohr->createBemassung();
		cg_list.push_back(rohr);
	}
}

void ConstructGraphik::scan_rohrteilung()
{
	rohrteilung = -1;
	int endelemente = 0;

	ElementList elelst(rohrleitung->get_elementList());
	for (int count = 0; count < elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		const Element* const ele = elelst.get_at(count);
		
		// Anzahl der Endeelemente bestimmen
		if (ele->isEleEnde())
			endelemente++;
		if (endelemente > 2)
		{
			// Bei mehr als 2 Endeelementen wird das Rohr geteilt
			rohrteilung = 0; // Zählerstart
			return;
		}
	}
}

///Die Elementliste wird abgearbeitet
void ConstructGraphik::runElementList()
{
	bool erstesElement = true;
	int rohrDn     = rohrleitung->get_dnvl();
	int rohrLaenge = rohrleitung->get_laenge();
	
	int rohrAnzahl = rohrleitung->get_anzahl();
	CString rohrPos= rohrleitung->get_posNr();

	// Rohr mit Einziehung?
	int einz = 0;

	abstKnick = 0;


/*	
#ifdef _DEBUG
	afxDump << rohrleitung->get_elementList();
#endif
*/



	ElementList elelst(rohrleitung->get_elementList());
	// Abarbeiten der Elementliste
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *ele = elelst.get_at(count);
		
		// Merker fuer aktuelles Element 
		elemerk.copy(*ele);

		if (elemerk.isEleRohr())
			rohrcounter++;

		// vorheriges und naechstes Element fuer diverse Abfragen
		if (elelst.get_prev(count))
			prevelemerk.copy(*elelst.get_prev(count));

		if (elelst.get_next(count))
			nextelemerk.copy(*elelst.get_next(count));

		// Wegen Bemassung werden diese Rohrstuecke auch als Abgangselement gemerkt 
		if (ele->isEleRedBogEinz())
			abgelemerk.copy(*ele);		

		// Wenn Element Anfang/Ende
		if ((elemerk.get_id() == SNAHT || elemerk.get_id() == SCHWEISS || elemerk.isEleEnde()) 
			&& (count < elelst.get_count()-1) && elelst.get_next(count)->isEleRohr())
		{
			erstesElement = true;
		}
		else if (elemerk.isEleRohr()) 
		{
			// Init Bemassung
			initBemassung(&elemerk);
			
			// Bei Rohrteilung sollten Rohre ohne Abgaenge nicht doppelt bemasst werden
			if (!elemerk.get_uelementList().IsEmpty() || rohrleitung->isBogen())
				restmassBemassen = true;
			
			if (elelst.get_prev(count) && elelst.get_next(count))
			{
				// Soll Rohr mit oder ohne Einziehung gezeichnet werden
				einz = 0;
				if (elelst.get_prev(count)->get_id() == EINZ && elelst.get_next(count)->get_id() != EINZ)
					einz = 1;
				else if (elelst.get_prev(count)->get_id() != EINZ && elelst.get_next(count)->get_id() == EINZ)
					einz = 2;
				else if (elelst.get_prev(count)->get_id() == EINZ && elelst.get_next(count)->get_id() == EINZ)
					einz = 3;
			
				// Setzen der Laenge fuer linkes Rohrende 
				lEndeLaenge = elelst.get_prev(count)->get_laenge();

				// Setzen der Laenge fuer rechtes Rohrende 
				rEndeLaenge = elelst.get_next(count)->get_laenge();
			}
			else
				throw DrawingException("ERROR!: elelst.get_prev(count) && elelst.get_next(count) in ConstruktGraphik.cpp");
		}
	
		// Rohrstueck zeichnen
		COIIVerteiler* pCOIIVerteilerUsedAsBase =  dynamic_cast<COIIVerteiler*>(rohrleitung);
		if(pCOIIVerteilerUsedAsBase)
		{
			pCOIIVerteilerUsedAsBase->SetFirstGraficAbgang(true);
		}
		createRohrList(rohrLaenge, rohrDn, rohrPos, rohrAnzahl, einz, erstesElement);  
		handle_rohr(ele);
		erstesElement = false;
	}
}



void ConstructGraphik::handle_rohr(Element* ele)
{
	int rohrDn     = rohrleitung->get_dnvl();
	int rohrLaenge = rohrleitung->get_laenge();
	bool hasBogen = rohrleitung->isRedBogEinz();
	
// Nur Rohre haben Abgaenge
	if (!ele->isEleRohr())
		return;

	//int abstKnick = 0;
	
	CObList& uelementList = ele->get_uelementList();
	for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
	{
		Element *uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
		XASSERT(uele);
		if (!uele)
			break;

		
		// Merker fuer aktuelles Element
		elemerk.copy(*uele);

		if (hasBogen)
			abgelemerk.copy(*uele);		
		else
		{
			// Merker fuer Abgangselement	
			if (!elemerk.isKnick())
				abgelemerk.copy(*uele);		

			if (elemerk.isKnick())
				abstKnick += elemerk.get_abstRel();
			else if (abstKnick > 0)
			{
				elemerk.set_abstRel(elemerk.get_abstRel() + abstKnick);
				abstKnick = 0;
			}
		}

		// Abgang zeichnen
		createAbgList(rohrLaenge, rohrDn, false, false);   

		// Nur Rohrstutzen hat Abschlusselement
		// Standardstutzen fuer Anlage kann urspruenglich ein Rohrstutzen
		// oder ein Nutstutzen gewesen sein
		if (uele->get_id() == ROHR 
		 || uele->get_id() == STDSTUTZEN && uele->get_urid() == ROHR)
		{
			CObList& uuelementList = uele->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)	
			{
				Element *uuele = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuele);
				if (!uuele)
					break;

				// Merker fuer aktuelles Element
				elemerk.copy(*uuele);
				
				// Abschlusselement auf Abgang zeichnen
				createAbgList(rohrLaenge, rohrDn, false, false);   
			}
		}
		COIIVerteiler* pCOIIVerteilerUsedAsBase =  dynamic_cast<COIIVerteiler*>(rohrleitung);
		if(pCOIIVerteilerUsedAsBase)
		{
			pCOIIVerteilerUsedAsBase->SetPreElementId(uele->get_id());
		}
	}
}



//Initialisiert die Bemassungshöhe und Tiefe
void ConstructGraphik::initBemassung(Element* element)
{
	// todo: Muss noch genauer unterschieden werden 
	int laenge = 0;

	int abglaenge_0 = 0;
	int endlaenge_0 = 0;
	int abglaenge_90 = 0;
	int endlaenge_90 = 0;
	int abglaenge_180 = 0;
	int endlaenge_180 = 0;
	int abglaenge_270 = 0;
	int endlaenge_270 = 0;

	 maxlaengeAbgOben = 0;
	int maxlaengeAbgUnten = 0;
	int maxlaengeAbgLinks = 0;
	int maxlaengeAbgRechts = 0;

	for (POSITION upos = element->get_uelementList().GetHeadPosition(); upos != NULL;)	
	{
		Element *uelement = dynamic_cast<Element*>(element->get_uelementList().GetNext(upos));
		XASSERT(uelement);
		if (!uelement)
			break;
			
		if (uelement->get_id() == BOHR50)
			laenge = 50;
		else
			laenge = uelement->get_laenge();

		if (uelement->get_winkel() < 90)
		{
			if (abglaenge_0 < laenge)
				abglaenge_0 = laenge; 
		}
		else if (uelement->get_winkel() < 180)
		{
			if (abglaenge_90 < laenge)
				abglaenge_90 = laenge; 
		}
		else if (uelement->get_winkel() < 270)
		{
			if (abglaenge_180 < laenge)
				abglaenge_180 = laenge; 
		}
		else if (uelement->get_winkel() < 360)
		{
			if (abglaenge_270 < laenge)
				abglaenge_270 = laenge; 
		}

		for (POSITION uupos = uelement->get_uelementList().GetHeadPosition(); uupos != NULL;)	
		{
			Element *uuelement = dynamic_cast<Element*>(uelement->get_uelementList().GetNext(uupos));
			XASSERT(uuelement);
			if (!uuelement)
				break;
			
			if (uuelement->get_winkel() < 90)
			{
				if (endlaenge_0 < uuelement->get_laenge())
					endlaenge_0 = uuelement->get_laenge(); 
			}
			else if (uuelement->get_winkel() < 180)
			{
				if (endlaenge_90 < uuelement->get_laenge())
					endlaenge_90 = uuelement->get_laenge(); 
			}
			else if (uuelement->get_winkel() < 270)
			{
				if (endlaenge_180 < uuelement->get_laenge())
					endlaenge_180 = uuelement->get_laenge(); 
			}
			else if (uuelement->get_winkel() < 360)
			{
				if (endlaenge_270 < uuelement->get_laenge())
					endlaenge_270 = uuelement->get_laenge(); 
			}
		}
	}

	if (drehfaktor == 0)
	{
		maxlaenge_0 += element->get_laenge();
	}
	else if (drehfaktor == 90)
	{
		maxlaenge_90 += element->get_laenge();
	}
	else if (drehfaktor == 180)
	{
		maxlaenge_180 += element->get_laenge();
	}
	else if (drehfaktor == 270)
	{
		maxlaenge_270 += element->get_laenge();
	}

	if (maxlaengeAbg_0 < abglaenge_0 + endlaenge_0)
		maxlaengeAbg_0 = abglaenge_0 + endlaenge_0;
	if (maxlaengeAbg_90 < abglaenge_90 + endlaenge_90)
		maxlaengeAbg_90 = abglaenge_90 + endlaenge_90;
	if (maxlaengeAbg_180 < abglaenge_180 + endlaenge_180)
		maxlaengeAbg_180 = abglaenge_180 + endlaenge_180;
	if (maxlaengeAbg_270 < abglaenge_270 + endlaenge_270)
		maxlaengeAbg_270 = abglaenge_270 + endlaenge_270;

	if (drehfaktor == 0)
	{
		maxlaengeAbgOben = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgUnten = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	else if (drehfaktor == 90)
	{
		maxlaengeAbgLinks = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgRechts = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	else if (drehfaktor == 180)
	{
		maxlaengeAbgUnten = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgOben = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	else if (drehfaktor == 270)
	{
		maxlaengeAbgLinks = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgRechts = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	
	maxlaengeAbg = (maxlaengeAbg_0>maxlaengeAbg)?maxlaengeAbg_0:maxlaengeAbg;
	maxlaengeAbg = (maxlaengeAbg_90>maxlaengeAbg)?maxlaengeAbg_90:maxlaengeAbg;
	maxlaengeAbg = (maxlaengeAbg_180>maxlaengeAbg)?maxlaengeAbg_180:maxlaengeAbg;
	maxlaengeAbg = (maxlaengeAbg_270>maxlaengeAbg)?maxlaengeAbg_270:maxlaengeAbg;
}

//Die Konstruktionselemente der Rohrstuecke werden erzeugt
void ConstructGraphik::createRohrList(int rohrLaenge, int rohrDn, CString rohrPos, int rohrAnzahl, int einz, const bool erstesElement)
{
	laenge = 0;
	int minuslaenge = 0;
	int anfendid = 0;
	int typ = IMROHR;

	// Schweissnaht nach folgenden Elementen
	if (elemerk.get_id() == SCHWEISS && prevelemerk.isEleRedBogEinz())
	{
		nRohre++;
		if (nRohre>0) 
			for (int i=0; i<nRohre; i++) 
				laenge += rohrleitung->get_rohrLaengen(i);
		
		prevelemerk.set_id(elemerk.get_id()); 
		
		// Gesamtlaenge in X+Y-Richtung setzen
		setNewXYLaenge();
		return;
	}

	// hier beginnt eine neue Rohrkonstruktion 
	if (erstesElement) 
	{
		if (xLaenge != 0)
		{
			if (prevelemerk.get_id() == SNAHT || prevelemerk.isEleEnde())
		     {
				if (restmassBemassen)
				{
					doRestmassBemassen(true);
					restmassBemassen = false;
				}
				gesamtmasseSetzen();
			}
			xLaenge = 0;
			yLaenge = 0;
			negxLaenge = 0;
			negyLaenge = 0;
		}
		restmassBemassen = false;

		maxlaengeAbg_0 = 0;
		maxlaengeAbg_90 = 0;
		maxlaengeAbg_180 = 0;
		maxlaengeAbg_270 = 0;

		if (Y0 > 0) 
			Y0 = 0;
		
		if (prevelemerk.get_id() != Element::LEER) 
		{ 
			Y0 -= minuslaenge + maxlaengeAbgOben + rohrDn*faktorRohrbreite + 11*Bemassung::EBENENABSTAND;
			
			X0 = 0;
			drehfaktor = 0;
			bogenlaenge = 0;
			nRohre++;
			minuslaenge = 0;
		}
	} 
	else 
	{
		if (prevelemerk.isEleRedBogEinz())
			nRohre++;
	}

	// Gesamtlaenge in X+Y-Richtung setzen
	setNewXYLaenge();
	
	// Das Rohrstueck Einziehung wird nicht gezeichnet, im Rohr mit drin 
	if (elemerk.get_id() == EINZ && prevelemerk.get_id() == SCHWEISS)
		nRohre--;
	
	// Gesamtlaenge der bisher abgebildeten Rohre
	if (nRohre>0) 
		for (int i=0; i<nRohre; i++) 
			laenge += rohrleitung->get_rohrLaengen(i);

	// Was soll bei welchem Element abgebildet werden
	bool beschriftung = true;
	bool bemassung = false;
	bool create_element = true;

	if (elemerk.isEleEnde())
	{
		elemerk.set_winkel(drehfaktor);
		if (nRohre == 0 && erstesElement)
			beschriftung = false;
	} 
	else if (elemerk.isEleRedBog())
	{
		if (elemerk.get_abstRel() > 0)
			bemassung = true;
	}
	else if (elemerk.get_id() == EINZ) 
	{
		if (elemerk.get_abstRel() > 0)
			bemassung = true;
		create_element = false;
	} 
	else if (elemerk.isEleRohr()) 
	{
		elemerk.set_winkel(drehfaktor); 
		bemassung = false;

		if (prevelemerk.get_id() != SCHWEISS && prevelemerk.get_id() != EINZ && prevelemerk.get_id() != Element::LEER)
			beschriftung = true;
		else if (rohrleitung->isRed())
			beschriftung = true;
		else
			beschriftung = false;
	} 
	
	

	if (elemerk.get_id() != GLATT && elemerk.get_id() != Element::LEER)
	{
		// Elemente konstruieren, bemassen, beschriften
		ConstructElement *ce = constructElementFromID(elemerk);
		if (ce)
		{
			ce->rohrleitung = rohrleitung;
			ce->initEleDatenAbgMerk(merkerAbgele);
			ce->initEleDatenMerker(merkerRohrele);
			ce->initEleDaten(elemerk);
			//ce->set_anfendid(anfendid);		
			ce->set_maxRohrDN(rohrleitung->getMaxRohrDN());
			ce->initGraphikDaten(X0, Y0, drehfaktor, bogenlaenge, rohrleitung->isBogen(), erstesElement, laenge, lEndeLaenge, rEndeLaenge, typ);
			ce->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
							  maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
							  maxlaengeAbg, rohrDn);
			WaxGrafik::RohrQx* rx = dynamic_cast<WaxGrafik::RohrQx*>(ce);
			if (rx)
			{
				if (rohrcounter == 1)
				{
					rx->set_show_dn(false);
					rx->set_show_text(true); 
				}
				else if (rohrteilung >= 0 && X0 == 0.0)
				{
					rx->set_show_dn(false);
					rx->set_show_text(true); 
					rohrteilung++;
				}
				else 
				{
					rx->set_show_text(false); 
					rx->set_show_dn(true);
				}
			
				rx->init_rohrqx((WaxGrafik::RohrQx::Einziehung)einz, rohrteilung >= 0 ? rohrteilung+1 : 0);
			}
			
			if (create_element)
				ce->createElement();
			
			if (beschriftung)
				ce->createBeschriftung();
			
			if (bemassung)
			{
				if (elemerk.isEleRedEinz())
					ce->initGraphikDaten(X0, /*y*/Y0, drehfaktor, bogenlaenge, rohrleitung->isBogen(), erstesElement, laenge, lEndeLaenge, rEndeLaenge, typ);
				
				if (!(avnachbearbeitung && rohrleitung->get_avelementArray().GetSize() > 0 && !rohrleitung->isBogen()) && !elemerk.isEleAbgEnde() && elemerk.get_abstRel() > 0)
					ce->createBemassung();
			}
			
			if (ce->has_coords_changed())
				ce->update_coords(X0, Y0);

			if (rx)
				cg_list.insert(cg_list.begin(), ce);
			else
				cg_list.push_back(ce);
		}
	}
	// Je nach Element weitere Konstruktionsvariablen setzen 
	if (elemerk.isEleEnde())
	{
		// ID von Anfang/Ende merken
		anfendid = elemerk.get_id();
	}	
	else if (elemerk.isEleRedEinz()) 
	{
		// neuen Rohranfangspunkt setzen
		setNewPoint();

		// Element ist sowohl Rohrende als auch Rohrstück
		nRohre++;
		
		// Element merken
		merkerRohrele.copy(elemerk);

		// Reset Bogenlaenge 
		bogenlaenge = 0;
	} 
	else if (elemerk.isEleBogen()) 
	{
		// neuen Rohranfangspunkt setzen
		setNewPoint();

		// Element ist sowohl Rohrende alsauch Rohrstueck
		nRohre++;
		
		// Element merken
		merkerRohrele.copy(elemerk);

		// Drehfaktor bestimmen 
		drehfaktor = rohrleitung->setNewDrehfaktor(drehfaktor, elemerk.get_winkel());

		// Gesamtlaenge in X+Y-Richtung setzen
		setNewXYLaenge();

		// Bogenlaenge setzen
		bogenlaenge = elemerk.get_laenge();

		// Wenn Bogen nach vorne oder hinten Rest nicht mehr zeichnen
		if (elemerk.get_winkel() != 0 && elemerk.get_winkel() != 180)
			throw DrawingException("Not drawable");
	} 
	else if (elemerk.get_id() == SCHWEISS)
	{
		// neuen Rohranfangspunkt setzen
		if (prevelemerk.isEleRohr() && nextelemerk.get_id() != EINZ) 		
			setNewPoint();
		
		nRohre++;
	}
	else if (elemerk.isEleRohr()) 
	{
		// Rohrlaenge in neg. Y-Richtung, fuer Anfangspunkt der naechsten Konstruktion
		if (drehfaktor == 270)
			minuslaenge += elemerk.get_laenge();

		// Reset Bogenlaenge 
		bogenlaenge = 0;	
	}
	
	// ID vorheriges Element merken
	prevelemerk.set_id(elemerk.get_id()); 
}

//Die Konstruktionselemente der Abgaenge werden erzeugt
void ConstructGraphik::createAbgList(int rohrLaenge, int rohrDn, bool knickeBemassen, bool nurBemassung)
{
	int typ = ABG;

	ConstructElement *ce = constructElementFromID(elemerk);
	if (!ce)
		return;
	ce->rohrleitung = rohrleitung;
	ce->set_maxRohrDN(rohrleitung->getMaxRohrDN());
	ce->initGraphikDaten(X0, Y0, drehfaktor, bogenlaenge, rohrleitung->isBogen(), false, laenge, lEndeLaenge, rEndeLaenge, typ);

	ce->initEleDatenAbgMerk(merkerAbgele);
	ce->initEleDatenMerker(merkerRohrele);
	ce->initEleDaten(elemerk);
		
	ce->initBemassung(maxlaenge_0, maxlaenge_90, maxlaenge_180, maxlaenge_270,
					  maxlaengeAbg_0, maxlaengeAbg_90, maxlaengeAbg_180, maxlaengeAbg_270,
					  maxlaengeAbg, rohrDn);

	if (!nurBemassung)
		ce->createElement();
	if ((!(avnachbearbeitung && rohrleitung->get_avelementArray().GetSize() > 0 && !rohrleitung->isBogen()) && !elemerk.isEleAbgEnde() && elemerk.get_abstRel() > 0) 
		|| rohrleitung->get_cadpos())
	{
		if ((knickeBemassen || (!knickeBemassen && !elemerk.isKnick())) || rohrleitung->isRedBogEinz())
		{
			if (rohrleitung->isRedBogEinz())
				ce->set_knickBemassung(false);

			ce->createBemassung();
		}
	}
	
	if (!nurBemassung)
		ce->createBeschriftung();
	
	cg_list.push_back(ce);

	// letztes Element merken
	if (elemerk.isEleAbgang())
	{
		merkerRohrele.copy(elemerk);
		merkerAbgele.copy(elemerk);
	}
}


//Erstellt ein neues ConstructElement Objekt in Abhängigkeit der Element ID.
ConstructElement* ConstructGraphik::constructElementFromID(Element & ele) const
{
	switch (ele.get_id())
	{
		case SCH:   
			return new ConstructScheib;	
		case BOD: 
		case BODEN_COII:
			return new ConstructBoden;
		case KB40:
		case KB50:   
			return new ConstructKbod;
		case GEW:
		case GEWINDE_KAPPE_COII:
		case GEWINDE_KAPPE_TYP_D:
			return new ConstructGewind;
		case NUT:      
			return new ConstructNut;	
		case VFL10:
		case VFL10NEU:
		case VFL16NEU:
		case VFL16:
		case AFL: 
		case FLANSCH_COII:
			return new ConstructVflan;
		case BLINDFLANSCH_COII:
			return new ConstructBlflan;
		case GEGENFLANSCH_COII_SCHARZ_HAUPTROHR:
			return new ConstructCOGegenflanHauptrohrVflan;
		case FFL:
		case SFL:
		case FFL16:
		case FFL10:
			return new ConstructFflan;	
		case ERED:	   
			return new WaxGrafik::ERed;
		case KRED:
		case EINZ:     
			return new ConstructKred;
		case Element::BOGEN:
		case BOGVERZ:
		case BOG2S:    
			return new ConstructBogen(ele.get_uelementList());
		case ROHR_Q0:
		case ROHR_Q1:
		case ROHR_Q3:
		case ROHR_Q5:  
		case ROHR_Q6:  
		case ROHR_Q7:  
		case ROHR_Q8:  
		case ROHR_Q9:  
		case ROHR_Q10:  
		case ROHR_Q11:  
		case ROHR_Q12:  
		case ROHR_Q13: 
		case ROHR_Q14: 
		case ROHR_Q15: 
		case ROHR_Q16:
		case ROHR_Q18:
		case ROHR_Q19:
		case ROHR_Q20:
			return new WaxGrafik::RohrQx;
		case STDMUFFE:
		case STDSTUTZEN:
		case MUFFE:
		case MUFFE_COII:
		case KRMUFFE:
		case FRMUFFE:
		case NEW_ROBBI_MUFFE:
		case ROHR:
		case NUTST:
		case GWRST: 
		case SONDERMUFFE:
		case POTENIALAUSGLEICH:
		case ROHRNIPPEL:
		case ROHRNIPPEL_COII:
		case SPRINKLERSCHELLE_VIROTEC:
			return new ConstructRohr;
		case WINKEL_LINKS_COII:
		case WINKEL_RECHTS_COII:
			return new ConstructCOBockWinkel;
		case GEGENFLANSCH_COII_ABGANG:
		case GEGENFLANSCH_COII_SCHWARZ_ABGANG:
			return new ConstructCOGegenflanVflan;
		case GEGENFLANSCH_GEWINDE_COII:		
			return new ConstructCOVflanWiththread;
		case GEGENFLANSCH_GEWINDE_MUFFE_COII:
			return new ConstructCOVflanWiththreadWithMuffe;
		case BOHR24:
		case BOHR50:   
			return new ConstructBohr;	
		case GRFITTING:
			return constructFittingFormID();
		case GLATT:
			return new ConstructGlatt;	
		case Element::LEER:
		case SCHWEISS: 
		case SNAHT:
			return NULL;
		case KNICK:
			return new ConstructKnick;
		default:
			XASSERT(FALSE);
	}
	return NULL;
}

///Erstellt ein neues ConstructElement Objekt in Abhängigkeit der Gruppen ID bei GRFITTING
ConstructElement* ConstructGraphik::constructFittingFormID() const
{
	if (elemerk.get_id() == GRFITTING)
	{
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
			return new ConstructKStueck;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::T_STUECK)
			return new ConstructTStueck;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::DOPPELNIPPEL)
			return new ConstructDoni;

		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::KAPPE)
			return new ConstructMuffe;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::STOPFEN)
			return new ConstructMuffe;

		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::F_MUFFE)
			return new ConstructMuffe;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::RED_MUFFE)
			return new ConstructMuffe;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL)
			return new ConstructMuffe;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::VERSCHRAUBUNG)
			return new ConstructMuffe;

		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL)
			return new ConstructWinkel;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_A1_45)
			return new ConstructWinkel;
		
		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_30)
			return new ConstructWinkel;

		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKEL_A4)
			return new ConstructWinkel;

		if (elemerk.get_gruppenid() == FormstueckeUndFittinge::WINKELVERSCHRAUBUNG)
			return new ConstructMuffe;
	}
	return NULL;
}

//Fasst alle Grafikobjekte in einer Liste zusammen.
vector<GlObj*> ConstructGraphik::getObjectList() 
{
	vector<GlObj*> ret;
	for ( std::vector<ConstructElement*>::iterator it = cg_list.begin(); it != cg_list.end(); it++)
	{
		ConstructElement* ele = *it;

		for (vector<GlObj*>::const_iterator it = ele->get_grafiklist().begin(); it != ele->get_grafiklist().end(); it++)
			ret.push_back(*it);
	}
	return ret;
}

//Fasst alle Bemassungsobjekte in einer Liste zusammen. Doppelte Bemassungen 
//werden ignoriert.
vector<Bemassung*> ConstructGraphik::getBemassungList()
{
	vector<Bemassung*> lst;
	for (std::vector<ConstructElement*>::iterator it = cg_list.begin(); it != cg_list.end(); it++)
	{
		ConstructElement *ele = *it;
		for (vector<GlObj*>::const_iterator it = ele->get_grafiklist().begin(); it != ele->get_grafiklist().end(); it++)
		{
			Bemassung* bem = dynamic_cast<Bemassung*>(*it);
			if (!bem) 
				continue;
			// Test auf Gleichheit
			bool gleichheit = false;
			
			for (vector<Bemassung*>::iterator itx = lst.begin(); itx != lst.end(); itx++)
			{
				Bemassung *xbem = *itx;
				if (xbem->sameCoordinates(*bem))
				{
					gleichheit = true;
					break;
				}
			}
			
			if (!gleichheit)
				lst.push_back(bem);
		}
	}	
	return lst;
}

//Prüft die Bemassungsobjekte auf ausreichende Abstände.
void ConstructGraphik::checkBemassung()
{
	vector<Bemassung*> lst = getBemassungList();
	
	for (vector<Bemassung*>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		Bemassung *bem = *it;
		for (vector<Bemassung*>::iterator itx = lst.begin(); itx != lst.end(); itx++)
		{
			Bemassung *xbem = *itx;
			if (bem->testCoordinates(*xbem))
			{
				it = lst.begin();
				break;
			}
		}
	}
}

//Die Graphikelemente werden serialisiert
void ConstructGraphik::serializeGraphicElements(const string& filename)
{
	/*
	Für jedes Grafikelement wird get_maxxy() aufgerufen. Dadurch kann am Ende die Min/Max
	Position im DWF File gesetzt werden. Diese Informationen werden im DWF File in der View
	GESAMTANSICHT gespeichert. 
	*/
	double gxmax = -99999999999,gymax = -99999999999;
	double gxmin = 99999999999,gymin = 99999999999;

	double xmax = -99999999999,ymax = -99999999999;
	double xmin = 99999999999,ymin = 99999999999;

	GlFile* grafikfile =  NULL;

	grafikfile =  new SVGFile(filename.c_str());


	// Alle Elemente zusammenfassen
	vector<GlObj*> gesamt = getObjectList();

	// Maximal und Minimalwerte ermitteln
	for (vector<GlObj*>::iterator it = gesamt.begin(); it != gesamt.end(); it++)
	{
		GlObj *globj = *it;
		globj->getmax_xy(xmax, ymax);
		globj->getmin_xy(xmin, ymin);
	}
	if (gxmin > xmin) gxmin = xmin;
	if (gymin > ymin) gymin = ymin;
	if (gxmax < xmax) gxmax = xmax;
	if (gymax < ymax) gymax = ymax;

	// Zoomfaktor fuer Fontgroesse festlegen je nach Masse der Zeichnung
	double x = xmax - xmin;
	double y = ymax - ymin;
	int zoom = 0;
	if (x <= 1000) zoom = 5;
	else if (x <= 1500) zoom = 6;
	else if (x <= 2500) zoom = 7;
	else if (x <= 3500) zoom = 8;
	else if (x <= 4500) zoom = 9;
	else if (x <= 5500) zoom = 10;
	else if (x <= 6500) zoom = 11;
	else if (x <= 8500) zoom = 12;
	else if (x <= 10500) zoom = 13;
	else if (x <= 12500) zoom = 14;
	else zoom = 15;


	// Alle Grafikobjekte serialisieren
	for (vector<GlObj*>::iterator it = gesamt.begin(); it != gesamt.end(); it++)
	{
		GlObj *globj = *it;
		
		// Zoomfaktor setzen
		TextPlot *text = dynamic_cast<TextPlot*>(globj);
		Bemassung *bema = dynamic_cast<Bemassung*>(globj);
		if (text)
		{
			if((rohrleitung->isBogen() && (y  > 5000 || x > 5000) ) )
			{			
				text->set_fontGroesse(text->get_fontGroesse() + 4);
			}
			
			text->set_zoom(zoom);
		}
		if (bema)
		{
			if((rohrleitung->isBogen() && (y  > 5000 || x > 5000)))
			{			
				bema->Set_BemassungFontGroesse(8);
			}
			bema->set_zoom(zoom);
		}	
		globj->serialize(grafikfile);
	}

	/// Fertigungshinweis jetzt in Grafik, damit durch Seitenumbruch nichts verloren gehen kann
	int ym = serialize_fertigungshinweis(grafikfile, (int)xmin, (int)ymin);

	grafikfile->set_max_xy(Point((int)xmax, (int)ymax));
	grafikfile->set_min_xy(Point((int)xmin, ym));
	delete grafikfile;

}

int ConstructGraphik::serialize_fertigungshinweis(GlFile* dwfFile, int x, int y)
{
	/**
	Fügt an der Stelle x,y den Fertigungshinweis und den Standardfertigungshinweis ein,
	falls vorhanden
	*/

	

	if (!rohrleitung->get_fertigungshinweis().GetLength() && 
		!rohrleitung->get_stdfertigungshinweis().GetLength() &&
		rohrleitung->get_rohrKFarbe() == 0)
		return y;
	
	std::vector<std::string> lst = rohrleitung->getFertigungshinweis();

	if (rohrleitung->get_stdfertigungshinweis().GetLength())
		lst.push_back((LPCSTR)rohrleitung->get_stdfertigungshinweis());

	if (rohrleitung->get_rohrKFarbe() > 0)
	{
		if (lst.size() > 0)
		{
			std::string dummy;
			lst.push_back(dummy); //Leerzeile
			lst.push_back(dummy);
			lst.push_back(dummy);
		}

		CString farbe = rohrleitung->getKappenfarbe(rohrleitung->get_rohrKFarbe());
		std::string bez = getStammdaten().getTextWithColon("IDS_KAPPENFARBE") + " ";
		bez.append((LPCSTR)farbe.MakeUpper());

		lst.push_back(bez);
	}

	for (std::vector<std::string>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		TextPlot ferthinweis(x, y, it->c_str(), GlObj::RED);
		ferthinweis.set_fontGroesse(7);
		ferthinweis.serialize(dwfFile);
		y -= ferthinweis.get_fontpixel();
	}
	return y;
}

void ConstructGraphik::setNewXYLaenge()
{
	if (drehfaktor == 0)
		xLaenge += elemerk.get_laenge();
	else if (drehfaktor == 180)
		negxLaenge += elemerk.get_laenge();
	else if (drehfaktor == 90)
		yLaenge += elemerk.get_laenge();
	else if (drehfaktor == 270)
		negyLaenge += elemerk.get_laenge();
}

///Neuen Rohranfangspunkt setzen
void ConstructGraphik::setNewPoint()
{
	if (elemerk.get_id() == SCHWEISS)
		setPointBehindSchweiss();
	else if (elemerk.isEleBogen())
		setPointBehindBogen();
	else if (elemerk.get_id() == ERED)
		setPointBehindEred();
	else if (elemerk.get_id() == KRED || elemerk.get_id() == EINZ)
		setPointBehindKred();
}

///Neuen Rohranfangspunkt hinter Schweissnaht setzen
void ConstructGraphik::setPointBehindSchweiss()
{
	double value = elemerk.get_abstAbs() - laenge + elemerk.get_laenge(); 

	if (drehfaktor == 180 || drehfaktor == 270)
		value *= (-1);

	value /= faktorRohrlaenge;

	if (drehfaktor == 0 || drehfaktor == 180)
		X0 += value;
	else if (drehfaktor == 90 || drehfaktor == 270)
		Y0 += value; 
}

///Neuen Rohranfangspunkt hinter Bogen setzen
void ConstructGraphik::setPointBehindBogen()
{
	double offsetx = 0.0, offsety = 0.0;

	if ((elemerk.get_winkel() == 0 && drehfaktor == 0)
	 || (elemerk.get_winkel() == 180 && drehfaktor == 270))
	{
		offsetx = (elemerk.get_id() == BOG2S)?0.5 : 1.0;
		offsety = (elemerk.get_id() == BOG2S)?1.5 : 2.0;
	}
	else if ((elemerk.get_winkel() == 180 && drehfaktor == 0)
		  || (elemerk.get_winkel() == 0 && drehfaktor == 90))
	{
		offsetx = (elemerk.get_id() == BOG2S)?0.5 : 1.0;
		offsety = (elemerk.get_id() == BOG2S)?0.5 : 1.0;
	}
	else if ((elemerk.get_winkel() == 180 && drehfaktor == 90)
		  || (elemerk.get_winkel() == 0 && drehfaktor == 180))
	{
		offsetx = (elemerk.get_id() == BOG2S)?1.5 : 2.0;
		offsety = (elemerk.get_id() == BOG2S)?0.5 : 1.0;
	}
	else if ((elemerk.get_winkel() == 180 && drehfaktor == 180)
	      || (elemerk.get_winkel() == 0 && drehfaktor == 270))
	{
		offsetx = (elemerk.get_id() == BOG2S)?1.5 : 2.0;
		offsety = (elemerk.get_id() == BOG2S)?1.5 : 2.0;
	}

	double value = elemerk.get_abstAbs() - laenge - elemerk.get_laenge(); 
	if (drehfaktor == 0)
		value -= lEndeLaenge;
	else if (drehfaktor == 180 || drehfaktor == 270)
		value *= -1; 

	if (drehfaktor == 0 )
		value += offsetx*elemerk.get_anschlussDN();
	else if (drehfaktor == 180)
		value -= offsetx*elemerk.get_anschlussDN();

	if (drehfaktor == 90)
		value += offsety*elemerk.get_anschlussDN();
	else if (drehfaktor == 270)
		value -= offsety*elemerk.get_anschlussDN();

	double value2 = 0.0;
	if (drehfaktor == 0 || drehfaktor == 180)
		value2 = offsety*elemerk.get_anschlussDN();
	else if (drehfaktor == 90 || drehfaktor == 270)
		value2 = offsetx*elemerk.get_anschlussDN();

	// nach oben/hinten/vorn	
	if ((drehfaktor ==   0 && elemerk.get_winkel() == 180)
     || (drehfaktor == 180 && elemerk.get_winkel() ==   0)
     || (drehfaktor ==  90 && elemerk.get_winkel() ==   0)
     || (drehfaktor == 270 && elemerk.get_winkel() == 180))
		value2 *= (-1);

	if (drehfaktor == 0 || drehfaktor == 180)
	{
		X0 += value;
		Y0 += value2;
	}
	else if (drehfaktor == 90 || drehfaktor == 270)
	{
		Y0 += value;
		X0 += value2;
	}
}

///Neuen Rohranfangspunkt hinter KRED setzen
void ConstructGraphik::setPointBehindKred()
{
	double value = elemerk.get_abstAbs() - laenge + elemerk.get_laenge();

	if (drehfaktor == 0)
		value += -lEndeLaenge;
	else if (drehfaktor == 180 || drehfaktor == 270)
		value *= (-1);

	value /= faktorRohrlaenge;

	double andn = elemerk.get_anschlussDN()*faktorRohrbreite;
	double dn = elemerk.get_dn()*faktorRohrbreite;
	double value2 = 0.0;

	// Rohr nach rechts
	if (drehfaktor == 0) 
	{
		TRACE("X0 = %i", X0);

		if (dn < andn) 
			value2 = (andn - dn)/2;
		else 
			value2 = -(dn - andn)/2; 
		TRACE("-> X0 = %i\n", X0);
	} 
	// Rohr nach links
	else if (drehfaktor == 180) 
	{
		if (dn < andn) 
			value2 = (andn - dn)/2; 
		else 
			value2 = - (dn - andn)/2; 
	} 
	// Rohr nach oben
	else if (drehfaktor == 90) 
	{
		if (dn < andn) 
			value2 = (andn - dn)/2;	
		else 
			value2 = - (dn - andn)/2;	
	} 
	// Rohr nach unten
	else if (drehfaktor == 270) 
	{
		if (dn < andn) 
			value2 = (andn - dn)/2;	
		else 
			value2 = - (dn - andn)/2;	
	}

	if (drehfaktor == 0 || drehfaktor == 180)
	{
		X0 += value;
		Y0 += value2;
	}
	else if (drehfaktor == 90 || drehfaktor == 270)
	{
		Y0 += value;	
		X0 += value2;	
	}
}

///Neuen Rohranfangspunkt hinter ERED setzen
void ConstructGraphik::setPointBehindEred()
{
	double value = elemerk.get_abstAbs() - laenge + elemerk.get_laenge();

	if (drehfaktor == 0)
		value += -lEndeLaenge;
	else if (drehfaktor == 180 || drehfaktor == 270)
		value *= (-1);

	value /= faktorRohrlaenge;

	if (drehfaktor == 0 || drehfaktor == 180)
		X0 += value;
	else if (drehfaktor == 90 || drehfaktor == 270)
		Y0 += value;	
}

//set avnachbearbeitung
const bool ConstructGraphik::get_avnachbearbeitung() const
{
	return avnachbearbeitung;
}

//set avnachbearbeitung
void ConstructGraphik::set_avnachbearbeitung(bool value)
{
	avnachbearbeitung = value;
}


