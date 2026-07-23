#include "stdafx.h"
#include "resource.h"


#include "Strangrohr.h"
#include <mxutil/ddx_extension.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

using namespace MxErrorHandling;




Strangrohr::~Strangrohr()
{
	muffenDNSET80 = false;
}

//Standardkonstruktor
Strangrohr::Strangrohr()
{
	BohrschelleBohrung = 0;
	muffenDNSET80 = false;
	muffenDN = 15;
	muffenID = MUFFE;
	muffenWinkel = 0;
	muffenGekehlt = false;
	muffenRestabstand = 5000;
	for (int i=0; i<MAX_ANZAHL_STDELEMENTE; i++)
		muffenRelAbst[i] = 0;

	muffenRelAbst[0] = 1000;
	set_dnvl(50);
	set_rohrQuali(RQ2458GSCH);
	get_leitkarte().set_sdMaterialnummer(400020);
	stdbezeichnung = getStammdaten().getText("IDS_STRANG").c_str(); 
	durchmesser = 24;
	dnvl = 32;
	sonderMuffenLaenge = 0;
	ElementAnzahlid = 0;
	vk3elementeListe.SetRohrType(EnCObListVk3::eRohrTypeStrangRohr);
	get_arbeit().SetRohrType(this);
}



void Strangrohr::copy(Strangrohr& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	Rohrleitung::copy(other);

	muffenDNSET80			= other.muffenDNSET80;
	muffenDN				= other.muffenDN;
	muffenID				= other.muffenID;
	muffenWinkel			= other.muffenWinkel;
	muffenGekehlt			= other.muffenGekehlt;
	muffenRestabstand		= other.muffenRestabstand;
	durchmesser				= other.durchmesser;
	
	for (int i=0; i<MAX_ANZAHL_STDELEMENTE; i++)
		muffenRelAbst[i]  = other.muffenRelAbst[i];
}

CString Strangrohr::copy(const Rohrleitung& other)
{
	/*
	Konvertierung einer Rohrleitung in ein Strangrohr
	*/
	Rohrleitung::copy(other);

	detect_stdmuffe(elementArray);
	move_avelements(elementArray, avelementArray);

	CString faultMessage = "";
	if(elementArray.GetCount() == 0)
	{   
		
		CString strSDAuftrag, strSDPosition;

		strSDAuftrag.Format("%I64d", get_leitkarte().get_sdAuftragsnr());
		strSDPosition.Format("%u", get_leitkarte().get_sdPositionsnr());
		faultMessage = "Fehler Strangrohr  SD Auftragsnummer=" + strSDAuftrag + " SD Positionsnummer=" + strSDPosition +"\n\r";
	}

	if (muffenWinkel != 0)
	{
		rotate_elements(avelementArray, 360 - muffenWinkel);
		rotate_elements(elementArray, 360 - muffenWinkel);
		muffenWinkel = 0;
	}

	// Relative Abstände müssen neu berechnet werden, wg. Umsortierung
	elementArray.calcRelAbst();

	int dummy = 0;
	avelementArray.sortElements(dummy);
	
	avelementArray.calcRelAbst();

	init_muffenRelAbst(elementArray, avelementArray);

	elementArray.deleteAll();

	set_cadpos(false);

	return faultMessage; 
}




void Strangrohr::detect_stdmuffe(const EnContainerRohrElemente& from)
{
	for (int i = 0; i < from.GetCount(); i++)
	{
		Element* e = from.get_element_at(i);
		if (!e->get_makeAnlage())
			continue;
		
		muffenID = e->get_id();
		muffenGekehlt = muffenID == KRMUFFE;
		muffenWinkel = e->get_winkel();
		durchmesser = e->get_dn();
		
		if (muffenID == SONDERMUFFE)
			set_sonderMuffenLaenge(e->get_laenge());
		else if (muffenID == BOHR24)
			set_durchmesser(e->get_durchmesser());
		muffenDN = e->get_dn();
		break;
	}
}

void Strangrohr::rotate_elements(EnContainerRohrElemente& elementarray, int winkel) const
{
	if (winkel == 0)
		return;
	for (int i = 0; i < elementarray.GetCount(); i++)
	{
		Element* e = elementarray.get_element_at(i);
		e->set_winkel((e->get_winkel() + winkel) % 360);
	}
}

void Strangrohr::move_avelements(EnContainerRohrElemente& from, EnContainerRohrElemente& to)
{
	/**
	Verschiebt alle AV Elemente aus dem elementArray ins
	avelementArray.
	Vor dieser Methode muss detect_stdmuffe aufgerufen werden.
	*/
	//for (int i = from.GetCount() - 1; i >= 0 ; i--)
	for (int i = 0; i<  from.GetCount() ; i++) //Array nicht rückwärts durchlaufen, da sonst die Sortierung in Array to falsch ist
	{
		Element* e = from.get_element_at(i);
		if (!e->get_makeAnlage())
		{
			to.Add(e);
			from.RemoveAt(i);

			i= i - 1;
		}
		else
		{
			// Prüfung, ob Standardmasse eingehalten sind. Falls ein Element trotz Anlagenflag abweichende Daten hat, 
			// wird es ins avelementArray verschoben
			bool move_element = false;
			XASSERT2((LPCSTR)get_posNr(), muffenID == e->get_id() && muffenWinkel == e->get_winkel() && muffenDN == e->get_dn())
			// Falls Element fälschlicherweise Anlagenflag hat, schieben wir es ins AvElementarray und Meldung in Logdatei
			if (muffenID != e->get_id() || 	muffenWinkel != e->get_winkel() || muffenDN != e->get_dn())
				move_element = true;

			if (muffenID == SONDERMUFFE && get_sonderMuffenLaenge() != e->get_laenge())
				move_element = true;

			if (muffenID == BOHR24 && get_durchmesser() != e->get_durchmesser())
				move_element = true;

			if (move_element)
			{
				to.Add(e);
				from.RemoveAt(i);
				i=i - 1;
			}
		}
	}
}

void Strangrohr::init_muffenRelAbst(EnContainerRohrElemente& elementArr, EnContainerRohrElemente& avelementArr)
{
	/**
	Überträgt die relativen Längen der Elemente in elementArr in muffenRelAbst.
	ElementArr muss vorher von AV Elementen befreit und die relativen Längen müssen neu berechnet
	worden sein.
	Falls mehr als die maximale Anzahl von Muffen enthalten ist, werden alle überzähligen Elemente
	ins avelementArr verschoben.
	*/

	if(elementArr.GetCount() == 0)
	{
		muffenRelAbst[0] = 0;
	}
	for (int i = 0; i < elementArr.GetCount(); i++)
	{
		Element* e = elementArr.get_element_at(i);
		if (i < getMaxAnzahlStdelemente())
			muffenRelAbst[i] = e->get_abstRel();
		else
		{
			avelementArr.Add(e);
			elementArr.RemoveAt(i);
			avelementArr.calcRelAbst();
			i = -1; // Wieder von Vorn
		}
	}
}


//Speichern und Laden des Strangrohrs
void Strangrohr::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	Rohrleitung::serialize(ar);
	int anzahl;

	int i;
	if (ar.is_storing()) 
	{
		ar << muffenDN << muffenID << muffenWinkel << muffenRestabstand << avelementArray.GetSize();
		
		for (i=0; i<MAX_ANZAHL_STDELEMENTE; i++)
			ar << muffenRelAbst[i];

		for (i=0; i<avelementArray.GetSize(); i++) 
		{
			Element *ele = avelementArray.get_element_at(i);
			ele->serialize(ar);
		}
		ar << durchmesser;
		ar << sonderMuffenLaenge;
	} 
	else 
	{
		ar >> muffenDN >> muffenID >> muffenWinkel >> muffenRestabstand >> anzahl;
		for (i=0; i<MAX_ANZAHL_STDELEMENTE; i++)
			ar >> muffenRelAbst[i];
		for (i=0; i<anzahl; i++) 
		{
			Element *ele = new Element;
			ele->serialize(ar);
			avelementArray.Add(ele);
		}
		if (version > 28)
			ar >> durchmesser;

		if (version >= 39)
			ar >> sonderMuffenLaenge;

	}
	serialize_bool(trockenanlage, ar);
	serialize_bool(muffenGekehlt, ar);
}


///Rohrkonstruktion erzeugen
void Strangrohr::createModell()
{
	Rohrleitung::createModell();

	// Rohrkonstruktion in Elementliste eingeben
	fill_elementArray();
	
	if (get_laenge() > 0) 
	{
		// Muffenrestabstand setzen
		set_muffenRestabstand(0);
		
		// Restabstand setzen
		set_restabstand(0);

		// Löschen aller Elemente aus der Liste
		elementList.deleteAllElemente();

		// Laengen für Anfang/Ende/Verb. setzen 
		setAnfaEndeVerbLaenge();
		// Berechnen der Rohrstuecke
		CString err = setAutoRohrlaengen();
		if (err.GetLength())
			return;
		
		// Elementliste fuer Rohrkonstruktion generieren
		if (!makeRohre(elementList))
			return; // Schwerer Fehler

			
			POSITION pos = elementList.GetHeadPosition();
			if(pos)
			{
				Element *ele = dynamic_cast<Element*>(elementList.GetNext(pos));
				// Ist am Anfang des Rohres eine Scheibe, dann wird diese an der Fertigungsstraße 
				// erstellt     
				if(ele->get_id() == SCH)
				{
					if(dnvl != 80)
					{
						ele->set_ElementAnzahlid(ElementAnzahlid);
						ele->set_ElementAnzahldn(ElementAnzahldnMuffeOrDBohrung);
						ele->set_makeAnlage(true);
					}
					else
					{
						ele->set_makeAnlage(false); // Bei 80 Rohr wird eine Scheibe nicht auf der Anlage gefahren
					}
				}
			}
		
			pos = elementList.GetTailPosition();
			if(pos)
			{
				// Die einzige Scheibe auf dem Strangrohr wird immer mit der Fertigungsstraße erstellt
				// hier das letzte Element. Dies wird nur gesetzt, wenn der Anfang keine Scheibe hat
				// sonst wird immer die Anfangsscheibe auf der Fertiggungsstraße geschweißt
				Element *ele = dynamic_cast<Element*>(elementList.GetAt(pos));

				if(ele->get_id() == SCH) 
				{
					if(rohrAnfang != SCH)
					{
						if(dnvl != 80)
						{
							ele->set_ElementAnzahlid(ElementAnzahlid);
							ele->set_ElementAnzahldn(ElementAnzahldnMuffeOrDBohrung);
							ele->set_makeAnlage(true);
						}
						else
						{
							ele->set_makeAnlage(false);  // Bei 80 Rohr wird eine Scheibe nicht auf der Anlage gefahren
						}
					}
				}
			}
		
		// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen 
		rohreBeschriften(elementList);

		// Bei Rohrstuecken setzen ob mit od. ohne Rohrstutzen (fuers Pulvern)
		setRohrMitStut();
		calc_laengeProNr();		
	}
	delete_stueckliste();
	create_stueckliste();

	calcZuschnittBeiBiegung();

	// fuer VK3-Berechnung, bei Strangrohr keine Passlaenge! 
	createVk3Liste();
	
	pulverlaengenListe.deleteAllElemente();
	
	createPulverlaengenListe();
	
		
	//#ifdef _DEBUG
	//	afxDump << "\n\n\n\n" << get_elementList();
	//#endif
}






void Strangrohr::create_stueckliste()
{
	Rohrleitung::create_stueckliste();

	if(Werkauftrag::MAGDEBURG == (Werkauftrag::FERTIGUNGSSTAETTE) Get_Fertigungsstaette())
	{
		
		for (POSITION pos = stueckliste.GetHeadPosition(); pos != NULL;)	
		{
			Element *element = dynamic_cast<Element*>(stueckliste.GetNext(pos));
			XASSERT(element);
	
					
			if(element->get_id() == FRMUFFE ||  element->get_id() == KRMUFFE || element->get_id() == NEW_ROBBI_MUFFE) // schellen kommen direkt herein
			{
				element->set_id(MUFFE);
				element->initStammdaten(element->get_dn(), dnvl, element->get_id());
			}
		}
	}
	
}




//Das Elementarray der Rohrleitung fuellen
void Strangrohr::fill_elementArray()
{
	elementArray.deleteAll();
	// Absolute Abstaende der Muffen berechnen
	int muffenAbsAbst[MAX_ANZAHL_STDELEMENTE];
	int i;
	for (i=0; i<MAX_ANZAHL_STDELEMENTE; i++)
	{
		if (muffenRelAbst[i] != 0)
		{
			if (i==0)
				muffenAbsAbst[i] = muffenRelAbst[i];
			else 	
				muffenAbsAbst[i] = muffenAbsAbst[i-1] + muffenRelAbst[i];
		} 
		else 
			muffenAbsAbst[i] = 0;
	}
	
	// MuffenID
	int id = 0;
	if (get_dnvl() == 80 || ((get_dnvl() == 32 || get_dnvl() == 40) && muffenDN == 25))
	{
		muffenGekehlt = true;
	}
	if (muffenID == MUFFE)
	{
		if (muffenGekehlt)
			id = KRMUFFE;
		else 
			id = FRMUFFE;
	} 
	else
		id = muffenID;

	// Array mit Muffen fuellen
	i=0;
	while(muffenRelAbst[i] != 0)
	{
		Element *ele = new Element;
		ele->set_abstAbs(muffenAbsAbst[i]);
		ele->set_abstRel(muffenRelAbst[i]);
		ele->set_anschlussDN(get_dnvl());
		ele->set_dn(muffenDN);
		ele->set_id(id);
		ele->set_winkel(muffenWinkel);
		
		if (muffenID == SONDERMUFFE)
		{
			ele->set_laenge(sonderMuffenLaenge); //Laenge wird in ConstructAdgEle::init mit faktorElelaenge multipliziert
			ele->set_mass1(sonderMuffenLaenge);  //Angedruckter Text
		}
		else
			ele->set_laenge(get_muffenLaenge()); 
		
		ele->set_durchmesser(durchmesser);

		ele->set_makeAnlage(true);

		ElementAnzahlid = ele->get_id();
		ele->set_ElementAnzahlid(ElementAnzahlid);
		ElementAnzahldnMuffeOrDBohrung = ele->get_dn(); // für Muffen
				
		if(ele->get_id() == BOHR24) // Robbi Bohrung
		{
			ElementAnzahldnMuffeOrDBohrung = ele->get_durchmesser();
			ele->set_dn(0);
		}
		ele->set_ElementAnzahldn(ElementAnzahldnMuffeOrDBohrung);

		elementArray.Add(ele);
		i++;
		if (i==MAX_ANZAHL_STDELEMENTE)
		{
			break;
		}
	}

	processAVElemente();
	
	
	// Rel. Abstaende neu berechnen
	elementArray.calcRelAbst();

	Werkauftrag::FERTIGUNGSSTAETTE Fertigungsstaette = (Werkauftrag::FERTIGUNGSSTAETTE) Get_Fertigungsstaette();


	if(Fertigungsstaette == Werkauftrag::MAGDEBURG)
	{
		changeElementArrayMuffenTöSchellen();
	}
}


void Strangrohr::processAVElemente()
{
	Rohrleitung::processAVElemente();	
	// AVElemente je nach absoluten Abstand ins Array schieben
	
	EnContainerRohrElemente &elementArray = get_elementArray();
	
	for(int i=0; i < elementArray.GetSize(); i++)
	{
		Element *elAv = elementArray.get_element_at(i);
		XASSERT(elAv);
		if (!elAv)
			break;

		if(elAv->get_id() != SPRINKLERSCHELLE_VIROTEC)
		{
			continue;
		}

		
		// Element mit kleinerem absoluten Abstand suchen
		int j;
		for (j=0; j<elementArray.GetSize(); j++) 	
		{
			Element *elem = elementArray.get_element_at(j);
			XASSERT(elem);
			if (!elem)
				break;

			if (abs(elAv->get_abstAbs() - elem->get_abstAbs()) <2 && elem->get_id() == BOHR24)
			{
				if(BohrschelleBohrung == 0)
				{
					BohrschelleBohrung = elem->get_durchmesser();
				}
				elementArray.RemoveAt(j);
				delete elem;
				break;
			}
		}
		
		if(elAv->get_laenge() == 0)
		{
			elAv->set_laenge(BohrschelleBohrung);
		}
		
	}
}



void Strangrohr::changeElementArrayMuffenTöSchellen()
{
/*	
	BOOL bRobbimuffeORSprinklerschelle_VirotecFound = FALSE; 
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		Element*  ele = elementArray.get_element_at(i);
		XASSERT(ele);
		if (!ele)
			break;
		
		if(ele->get_id() == FRMUFFE || ele->get_id() == SPRINKLERSCHELLE_VIROTEC)
		{
			bRobbimuffeORSprinklerschelle_VirotecFound = TRUE;
			break;
		}	
	}
	

	if( 25 < dnvl && dnvl < 65  )
	{
		for (int i=0; i < elementArray.GetSize(); i++)
		{
			Element*  ele = elementArray.get_element_at(i);
			XASSERT(ele);
			if (!ele)
				break;

			if(ele->get_id() == FRMUFFE ||  ele->get_id() == KRMUFFE) // schellen kommen direkt herein
			{
				ele->set_id(MUFFE);
				ele->set_makeAnlage(false);
				//ele->set_laenge(24);
				ele->SetDoIgnoreRohrOrVk3Rohr(true);
			}
		}
	}
	*/
}


int Strangrohr::get_muffenLaenge()
{
	const CObList& grp = getStammdaten().get_bauelemente();
	for (POSITION gpos = grp.GetHeadPosition(); gpos;)
	{
		Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
		if ( (g->get_elemid() == muffenID) 
		  && (g->get_dn() == muffenDN) 
		  && (g->get_dnvl() == get_dnvl() || g->get_dnvl() == 0) ) 
			return (g->get_mass1());
	}
	return (0);
}

//Hier wird automatisch der Arbeitsplan generiert
void Strangrohr::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	
	// Afos aus Elementliste der Rohrleitung generieren
	if(Get_Fertigungsstaette() ==  Werkauftrag::MAGDEBURG)
	{
		createAutoArbeitMagdeburg(beschichtung);
		return;
	}

	Rohrleitung::createAutoArbeit();
	Element *ele = new Element;
	Element *prev = NULL; 
	//bool scheibe = false;
	bool zusatzafos = false;
	int count = 0;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;
		
		if (count > 0)
		{
			POSITION ppos = elementList.FindIndex(count-1);
			prev = dynamic_cast<Element*>(elementList.GetAt(ppos));
		}
		
		// Alles hinter diesen Elementen muss manuell bearbeitet werden
		if (element->isEleRedBogEinz())
			zusatzafos = true;
		
		// Schweissnaht im Werk ist SCHWEISS
		// Bei Strangrohr Saegen, Schweissen u. Nuten bei Muffen mit drin
		if (!zusatzafos && (element->get_id() == NUT   || element->isEleRohr() || element->get_id() == LEER || element->get_id() == GEW
		                 || element->get_id() == SNAHT || element->get_id() == SCHWEISS && !prev->isEleRedBogEinz()))
		{
			; //In this case do nothing
		}
		// Maschine kann nur eine Scheibe, zweite macht der Schweisser
		else if (element->get_id() == SCH)
		{
			if (element->get_makeAnlage())
				get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER, 0, 1);
			else 
				get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER, 1);
			//scheibe = true;
		}
		else
		{
			get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(),  0, "0", Rohrleitung::WASSER);
		}

		CObList& uelementList = element->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			// Wenn Rohr DN65 und muss bei Nacharbeit der Vorrichter auch die Loecher brennen.
			// Somit muessen bei Afo16 immer die groesseren Zeiten vom Rohrlaengenbereich 0-1499 genommen werden.
			if (get_dnvl() == 65 && (uelement->get_id() == ROHR || uelement->get_id() == NUTST || uelement->get_id() == MUFFE))
				uelement->set_rohrLaenge(500);
			
			get_arbeit().addAfos(get_anzahl(), uelement, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
			
			CObList& uuelementList = uelement->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)	
			{
				Element *uuelement = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuelement);
				if (!uuelement)
					break;

				get_arbeit().addAfos(get_anzahl(), uuelement, get_beschichtung(), 0, "0", Rohrleitung::WASSER);
			}
		}
		count++;
	}
	delete ele;

	// Rüstzeiten
	addRuestzeiten(); 

	// Afos sortieren
	get_arbeit().sortAfos(get_arbeit().get_arbeitsfolgen());

	// Bei Strangrohr Afo 5 nicht zu SAP rüber, damit kein Lohnschein gedruckt wird
	get_arbeit().setSap(5, false);

	///Es duerfen nur die Positionen beruecksichtigt werden, die auch geschweisst werden
	if (get_arbeit().hasArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz()))
		///Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16/17 gesetzt 
		get_arbeit().setKranzeit(getKranzeit());

	///Es duerfen nur die Positionen beruecksichtigt werden, die auch geschweisst werden
	if (get_arbeit().hasAfo(16))
		///Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16
		get_arbeit().setKranzeit(getKranzeit(true), true);
}


void Strangrohr::createAutoArbeitMagdeburg(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);

	

	// Afos aus Elementliste der Rohrleitung generieren
	FERTIGUNGSSTAETTE Fertigungsstaette = Werkauftrag::MAGDEBURG;
	CString strArbeitsplatz = "99000";
	
	Arbeitsplan& plan = get_arbeit();
	
	plan.clean();
	
	// Diese Afo (Uebergabe an Produktion) immer zuerst
	plan.addAfo_Produktion();

	// Erzeugen der Afos fuer die Beschichtung 
//	if (beschichtung)
//		createBeschAfos(plan);

	// Erzeugen der Afos fuers Anliefern
	createAnliefAfos(plan);

	// Zeugnis
	if (get_zeugnis())
		plan.addAfo(new Arbeitsfolge(37));
	plan.updateSpecialAF(NULL, this->get_beschichtung(), this->getGesGewicht(), this->getMaxLaenge(), this->get_status().get_wanr());




	Element *ele = new Element;
	Element *prev = NULL; 
	//bool scheibe = false;
	bool zusatzafos = false;
	int count = 0;
	
	EnCObListModell elementListLocal;
	elementListLocal.copy(elementList);

	double dPreis =0;
	const VK3Preise& vk3 = getStammdaten().get_vk3preise();
	for (POSITION pos = elementListLocal.GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elementListLocal.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;
	
		if (count > 0)
		{
			POSITION ppos = elementListLocal.FindIndex(count-1);
			prev = dynamic_cast<Element*>(elementListLocal.GetAt(ppos));
		}
		
		// Alles hinter diesen Elementen muss manuell bearbeitet werden
		if (element->isEleRedBogEinz())
			zusatzafos = true;
		
		// Schweissnaht im Werk ist SCHWEISS
		// Bei Strangrohr Saegen, Schweissen u. Nuten bei Muffen mit drin
		
		if (element->get_id() == SCH /*&&  prev != null !prev->isEleRedBogEinz()*/)
		{

			Arbeitsfolge* pNewAfo = new Arbeitsfolge(16);
			pNewAfo->set_arbpl("99000");
			
			pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
			pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
			strArbeitsplatz = pNewAfo->get_arbpl();
			
			pNewAfo->set_dn(get_dnvl());
			pNewAfo->set_anz( get_anzahl());
			if(element->get_dn() == 40)
			{
				pNewAfo->set_zeit(0.0596 * pNewAfo->get_anz());
			}
			if(element->get_dn() == 50)
			{
				pNewAfo->set_zeit(0.0648 * pNewAfo->get_anz());
			}
			get_arbeit().addAfo(pNewAfo);

			const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, element->get_id(), element->get_dn(), this->get_dnvl(), 0, 0, 0, 0, 0, 0);
			if(v != NULL)
			{
				dPreis = dPreis + v->get_preis()* get_anzahl();
			}
			continue;
		}


		if (element->get_id() == SCHWEISS && !prev->isEleRedBogEinz())
		{
			get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), Fertigungsstaette, strArbeitsplatz, Rohrleitung::WASSER); //In this case do nothing

			const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, element->get_id(), element->get_dn(), this->get_dnvl(), 0, 0, 0, 0, 0, 0);
			if(v != NULL)
			{
				dPreis = dPreis + v->get_preis()* get_anzahl();
			}
			continue;
		}

		if (!zusatzafos && ( element->isEleRohr() || element->get_id() == LEER 
		                 || element->get_id() == SCHWEISS && !prev->isEleRedBogEinz()))
		{
			;
		}	
		else
		{ 
			if(element->get_id() == GEW )
			{
				Fertigungsstaette = Werkauftrag::MAGDEBURG;
				const int AFO_GEWINDESCHNEIDEN = 4010;
			
				Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_GEWINDESCHNEIDEN);
				pNewAfo->set_arbpl("99000");
			
				pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
				pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
				strArbeitsplatz = pNewAfo->get_arbpl();
			
				pNewAfo->set_dn(get_dnvl());
				pNewAfo->set_anz( get_anzahl());
				pNewAfo->set_zeit(	get_arbeit().getAfoZeit(pNewAfo->get_id(), get_dnvl(),
									get_beschichtung(), Fertigungsstaette, strArbeitsplatz, Rohrleitung::WASSER)* pNewAfo->get_anz());
				get_arbeit().addAfo(pNewAfo);
				continue;

			}

			if(element->get_id() == NUT)
			{
				
				Fertigungsstaette = Werkauftrag::MAGDEBURG;
				const int AFO_VNUT_EINROLLEN   = 4015;

				
				const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, AFO_VNUT_EINROLLEN, element->get_dn(), this->get_dnvl(), 0, 0, 0, 0, 0, 0);
				if(v != NULL)
				{
					dPreis = dPreis + v->get_preis()* get_anzahl();
				}

			
				Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_VNUT_EINROLLEN);
				pNewAfo->set_arbpl("99000");
			
				pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
				pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
				strArbeitsplatz = pNewAfo->get_arbpl();
			
				pNewAfo->set_dn(get_dnvl());
				pNewAfo->set_anz( get_anzahl());
				pNewAfo->set_zeit(	get_arbeit().getAfoZeit(pNewAfo->get_id(), get_dnvl(),
									get_beschichtung(), Fertigungsstaette, strArbeitsplatz, Rohrleitung::WASSER) * pNewAfo->get_anz());
				get_arbeit().addAfo(pNewAfo);
				continue;
			}
			get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(),  Fertigungsstaette, strArbeitsplatz, Rohrleitung::WASSER);
		}

		CObList& uelementList = element->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			
			if(uelement->get_id() == FRMUFFE ||  uelement->get_id() == KRMUFFE) // schellen kommen direkt herein
			{
				uelement->set_id(MUFFE);
				uelement->set_makeAnlage(false);
				uelement->SetDoIgnoreRohrOrVk3Rohr(true);
				
			}
				
			const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, uelement->get_id(), uelement->get_dn(), this->get_dnvl(), 0, 0, 0, 0, 0, 0);
			if(v != NULL)
			{
				dPreis = dPreis + v->get_preis()* get_anzahl();
			}

			// Wenn Rohr DN65 und muss bei Nacharbeit der Vorrichter auch die Loecher brennen.
			// Somit muessen bei Afo16 immer die groesseren Zeiten vom Rohrlaengenbereich 0-1499 genommen werden.
			if (get_dnvl() == 65 && (uelement->get_id() == ROHR || uelement->get_id() == NUTST || uelement->get_id() == MUFFE))
				uelement->set_rohrLaenge(500);
			
			get_arbeit().addAfos(get_anzahl(), uelement, get_beschichtung(), Fertigungsstaette, strArbeitsplatz,  Rohrleitung::WASSER);

			CObList& uuelementList = uelement->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)	
			{
				Element *uuelement = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuelement);
				if (!uuelement)
					break;

				get_arbeit().addAfos(get_anzahl(), uuelement, get_beschichtung(), Fertigungsstaette, strArbeitsplatz, Rohrleitung::WASSER);
			}
		}
		count++;
	}
	delete ele;


	const int AFO_ROHR_SAEGEN = 4000;
	Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
	pNewAfo->set_arbpl("99000");
	pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
	pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
	strArbeitsplatz = pNewAfo->get_arbpl();
			
	pNewAfo->set_dn(get_dnvl());
	if(get_laenge() <= 6000)
	{
		pNewAfo->set_anz( get_anzahl());
		const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, AFO_ROHR_SAEGEN, get_dnvl(), get_dnvl(), 0, 0, 0, 0, 0, 0);
		if(v != NULL)
		{
			dPreis = dPreis + v->get_preis() * get_anzahl();
		}
	}
	else
	{
		if(get_laenge() > 6000)
		{
			pNewAfo->set_anz( get_anzahl() * 2); // 2* mal saegen
			const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, AFO_ROHR_SAEGEN, get_dnvl(), get_dnvl(), 0, 0, 0, 0, 0, 0);
			if(v != NULL)
			{
				dPreis = dPreis + v->get_preis()* get_anzahl() * 2;
			}
		}
	}

	pNewAfo->set_zeit(	get_arbeit().getAfoZeit(pNewAfo->get_id(), get_dnvl(),
				get_beschichtung(), Fertigungsstaette, strArbeitsplatz, Rohrleitung::WASSER) * pNewAfo->get_anz());
	get_arbeit().addAfo(pNewAfo);


	Arbeitsfolge *afx = new Arbeitsfolge (68); 
	for (POSITION pos = get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos; )
	{
			Arbeitsfolge *af = (Arbeitsfolge*) get_arbeit().get_arbeitsfolgen().GetNext(pos);
			
			int tg = af->get_id();
			
			if(tg != 99  && tg != 41) //Übergabe Produktion Bündeln
			{
				afx->set_zeit(afx->get_zeit() + af->get_zeit());
				afx->set_anz(afx->get_anz() + af->get_anz());
			}	
	}
		

	get_arbeit().clean();
	const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, 751, get_dnvl(), get_dnvl(), 0, 0, 0, 0, 0, 0);// Pulvern nur Vk3 keine Zeit bei afos da extern
	if(v != NULL)
	{
		dPreis = dPreis + v->get_preis() * get_laenge() / 1000 * get_anzahl() ;
	}
	afx->set_preis(dPreis);
	afx->set_arbpl("99000");
	get_arbeit().get_arbeitsfolgen().AddTail(afx);

	afx = new Arbeitsfolge (42);
	afx->set_arbpl("32999");
	get_arbeit().get_arbeitsfolgen().AddTail(afx);

	afx = new Arbeitsfolge (99);
	get_arbeit().get_arbeitsfolgen().AddTail(afx);

	get_arbeit().sortAfos();
}



void Strangrohr::set_muffenRelAbst(int index, int value)
{
	muffenRelAbst[index] = value;
}

const int Strangrohr::get_muffenRelAbst(int index) const
{
	return muffenRelAbst[index];
}

int Strangrohr::get_muffenDN() 
{
	if(muffenDNSET80)
	{
		muffenDNSET80 = false;
	}
	return muffenDN;
}

void Strangrohr::set_muffenDN(int value)
{
	if(muffenDNSET80)
	{
		return;
	}
	muffenDN = value;
}


const int Strangrohr::get_muffenID() const
{
	return muffenID;
}

void Strangrohr::set_muffenID(int value)
{
	muffenID = value;
}

//get avelementArray
const int Strangrohr::get_muffenWinkel() const
{
	return muffenWinkel;
}

void Strangrohr::set_muffenWinkel(int value)
{
	muffenWinkel = value;
}

const bool Strangrohr::get_muffenGekehlt() const
{
	return muffenGekehlt;
}

void Strangrohr::set_muffenGekehlt(bool value)
{
	muffenGekehlt = value;
}

const int Strangrohr::get_muffenRestabstand() const
{
	return muffenRestabstand;
}

void Strangrohr::set_muffenRestabstand(int value)
{
	muffenRestabstand = value;

	if (value == 0)
	{
		int laenge = 0;
		if(isRedBogEinz())
		{
			for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
			{
				const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
				XASSERT(ele);
				if (!ele)
					break;

				if (ele->isEleRedBogEinz()) 
					break;
				laenge += ele->get_laenge();
			} 
		}
		else
			laenge = get_laenge();

		// Nebenbei den Restabstand setzen
		int muffenAbsAbst=0;

		for (int i=0; i<MAX_ANZAHL_STDELEMENTE ; i++)
			muffenAbsAbst += muffenRelAbst[i];
		
		muffenRestabstand = laenge - muffenAbsAbst; 
	}
}

void Strangrohr::set_durchmesser(double value)
{
	durchmesser = value;
}

const double Strangrohr::get_durchmesser() const
{
	return durchmesser;
}

///Rohrkonstruktion auf korrekte Werte checken
CString Strangrohr::checkPlausis()
{
	
	Werkauftrag::checkPlausis();
	CString err, str;

	// Erst mit neuen Werten fuellen, sonst wird mit alten geprueft 
	fill_elementArray();
	
	// Max. Laenge checken
	if (get_laenge() > MAX_GESAMTLAENGE) 
	{ 
		str = getStammdaten().getText("IDS_ERROR105").c_str(); 
		err.Format(str, MAX_GESAMTLAENGE);
		str = getStammdaten().getText("IDS_BITTE1").c_str(); 
		err += " " + str;
		return err;
	} 
	// Min. Laenge checken
	if (get_laenge() < MIN_ROHRLAENGE)
	{
		str = getStammdaten().getText("IDS_ERROR111").c_str(); 
		err.Format(str, MIN_ROHRLAENGE);
		str = getStammdaten().getText("IDS_BITTE1").c_str(); 
		err += " " + str;
		return err;
	}
	
	// DN checken, Strang hat weniger 
	if (get_dnvl() < MIN_DNVL || get_dnvl() > MAX_DNVL) 
	{ 
		str = getStammdaten().getText("IDS_ERROR91").c_str(); 
		err.Format(str, get_dnvl());
		return err;
	}

	// Stranglaenge mit groesstem absoluten Elementabstand abgleichen
	if (get_elementArray().GetSize() > 0)
	{
		Element *ele = get_elementArray().get_element_at(get_elementArray().GetSize()-1);

		if (ele->get_abstAbs() > (get_laenge()-50))
		{
			str = getStammdaten().getText("IDS_ERROR110").c_str(); 
			err.Format(str, 50);
			return err;
		}
	}


	//Länge bei Sondermuffen prüfen
	if (get_muffenID() == SONDERMUFFE)
	{
		if (get_sonderMuffenLaenge() < MIN_SONDERMUFFEN_LAENGE || get_sonderMuffenLaenge() > MAX_SONDERMUFFEN_LAENGE)
		{
			err = getStammdaten().getText("IDS_SONDERMUFFEN_LAENGE").c_str(); 
			return err;
		}
	}



	// Standardelemente checken
	err = checkStdElemente();
	// AV nur Meldung, Scheel 23.07.2004
	checkPlausiFertigungMagdeburg(err);
	if (err.GetLength())
	{
		return err;
	}

	
	if(  get_dnvl() == 80 && !(get_laenge() > 1400 && get_laenge() < 9000))
	{
	  err =  getStammdaten().getText("IDS_ERROR_ESFR_RORHR1").c_str(); 
	  return "";
	}
	err = Rohrleitung::checkPlausis();

	return err;
}


bool Strangrohr::checkPlausiFertigungMagdeburg(CString& strError)
{
	
	if(Get_Fertigungsstaette()== Werkauftrag::MAGDEBURG)
	{

		if(getBeschichtung().get_typ() != Beschichtung::ROT_GEPULV)
		{
			strError += "Nur Pulverfarbe \"Rot\" kann EIs fertigen";
		}

		if(get_laenge() >= 9000)
		{
			strError += "Nur Rohre bis 9m kann EIs fertigen";
		}

		for (int i=0; i < elementArray.GetSize(); i++)
		{
			const Element* const ele = elementArray.get_element_at(i);
			XASSERT(ele);
			if (!ele)
				break;
			
			CString strLocalError(getStammdaten().getText("IDS_KEINE_SCHELLE_VORHANDEN").c_str());
			if( get_dnvl() == 32  || get_dnvl() == 40  ||  get_dnvl() == 50 )
			{
				if((ele->get_dn() <  10 || ele->get_dn() > 25) && (ele->get_id() == FRMUFFE ||  ele->get_id() == KRMUFFE))
				{					
					strLocalError.Format(strLocalError, ele->get_dn(), get_dnvl() );
					strError += strLocalError;
				}

				strLocalError = getStammdaten().getText("IDS_KEIN_NUTZSTUTZEN_VORHANDEN").c_str();
				
				if((ele->get_dn() < 32 ||  ele->get_dn() > get_dnvl()) &&  ele->get_id() == NUTST)
				{
					strLocalError.Format(strLocalError, ele->get_dn(), get_dnvl() );
					strError += strLocalError;
				}
				
			}
			else
			{

				CString strLocalErrorII(getStammdaten().getText("IDS_KEIN_ROHR_VORHANDEN").c_str());
				strLocalErrorII.Format(strLocalErrorII, ele->get_dn(), get_dnvl() );
				strError += strLocalErrorII;
			}

		}
	

		if( strError.GetLength() > 0)
		{	
			HWND hWnd = ::FindWindow(_T("#32770"), _T("Wax+"));

			if (hWnd)
			{
				::PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			CString strPos;
			strPos =  CString("Positions Nr. ") +  get_posNr();
			strError = "Sie haben Fertigung EIS gewählt" + CString("\n") + strError  + CString("\n") + strPos;
			if(WaxSys::isAV())
			{		

				AfxMessageBox(strError , MB_OK | MB_ICONERROR);
				strError.Empty();
				return true;
			}
			else
			{
				return false; 
			}
		}
/*
		if(get_beschichtung().isVerzinkt() && WaxSys::isAV() && Werkauftrag::MAGDEBURG == Get_Fertigungsstaette())
		{
			AfxMessageBox(_T("Die Beschichtung muß jetzt von 'verzinkt' auf 'ohne' gesetzt, damit die Verzinkungskosten entfallen!!") , MB_OK | MB_ICONINFORMATION);	
		}
		*/
	}
	return true;
}



void Strangrohr::changeMuffenToDependsOnPipe80()
{
	if(get_dnvl() == 80)
	{
		if(muffenID == MUFFE  || muffenID == KRMUFFE || muffenID == FRMUFFE)
		{
			if( muffenDN < 20)
			{
				muffenDNSET80 = true;
				muffenDN = 20;
			}
		}
	}
}

///Prueft die Standardelemente auf korrekte Werte
CString Strangrohr::checkStdElemente()
{
	CString err, str;
	
	// Typ checken
	if (muffenID == LEER)  
	{
		err = getStammdaten().getText("IDS_ERROR112").c_str();  //Bitte Typ auswählen!

#ifdef _DEBUG
		return "";
#else
		return err;
#endif
	}

	// MuffenDN checken 
	if (muffenID == BOHR24)
	{
		if (muffenDN != 0)
		{
			err = getStammdaten().getText("IDS_ERROR17").c_str(); 	//Bei Bohrung muss DN 0 sein!
			return err;
		}
		if (durchmesser != 24 && durchmesser != 31 && durchmesser != 35) //Änderung 24.04.06: + durchmesser 35
		{
			err = getStammdaten().getText("IDS_ERROR113").c_str();  //Bei Bohrung nur Durchmesser 24 und 31mm zulässig. Andere Durchmesser als Nacharbeit erfassen!
			str = getStammdaten().getText("IDS_BITTE2").c_str(); 
			return err + " " + str;
		}
	}
	if (muffenID == MUFFE && muffenDN != 10 && muffenDN != 15 && muffenDN != 20 && muffenDN != 25)
	{
		err = getStammdaten().getText("IDS_ERROR114").c_str();  //Bei Muffe muss DN 10/15/20/25 sein!
		str = getStammdaten().getText("IDS_BITTE2").c_str(); 
		return err + " " + str;
	}

	// Strang auf min. 1 Abgang checken
	
	
	
	// Relativen Abstand zum Anfang prüfen
	int minAbst;
	if (muffenID == SONDERMUFFE && sonderMuffenLaenge > 60)
		minAbst = MIN_ABSTAND_SONDERMUFFE_L60;
	else
		minAbst = MIN_ABSTAND_ANFANG;

	if (muffenRelAbst[0] < minAbst) 
	{
		str = getStammdaten().getText("IDS_ERROR116").c_str(); 			//Abstand vom 1.Element zum Rohranfang muss min. %dmm sein!
		err.Format(str, minAbst);
		return err;
	}

	// Relativen Abstand zwischen den Elementen checken
	int i=0, muffenAbsAbst = 0;  
	while (muffenRelAbst[i] != 0 && i < MAX_ANZAHL_STDELEMENTE)
	{
		if (muffenRelAbst[i] < MIN_RELATIVER_ABSTAND && i > 0)
		{
			str = getStammdaten().getText("IDS_ERROR117").c_str(); 
			err.Format(str, MIN_RELATIVER_ABSTAND);
			return err;
		}
				
		// Absoluter Abstand der Muffen mit Nachbearbeitungspositionen abgleichen.
		muffenAbsAbst += muffenRelAbst[i];
		for (int j=0; j<get_avelementArray().GetSize(); j++)
		{
			// Hinter Bogen duerfen keine Standardelemente mehr sein
			Element* ele = get_avelementArray().get_element_at(j);
			if (ele->isEleBogen())
			{
				if (muffenAbsAbst >= ele->get_abstAbs())
				{
					err = getStammdaten().getText("IDS_ERROR118").c_str(); 	//Hinter Bogen dürfen keine Standardelemente mehr sein!
					str = getStammdaten().getText("IDS_BITTE1").c_str(); 
					return err + "\n" + str;
				}
			}
			
			// Bei gleichem Abstand muss der Winkel unterschiedlich sein.
			if (muffenAbsAbst == ele->get_abstAbs())
			{
				if (muffenWinkel == ele->get_winkel())
				{
					if(ele->get_id() != SPRINKLERSCHELLE_VIROTEC )
					{
						err = getStammdaten().getText("IDS_ERROR119").c_str();  //Absoluter Abstand und Winkel entspricht einer anderen Elementposition!\nBitte Winkel oder Abstand ändern!
					}
					return err;
				}
			}
		}
		i++;
	}
	


	if (muffenID == SONDERMUFFE && sonderMuffenLaenge > 60)
		minAbst = MIN_ABSTAND_SONDERMUFFE_L60;
	else
		minAbst = MIN_ABSTAND_ENDE;

	// Groessten absoluten Abstand mit Rohrlaenge pruefen
	if (muffenAbsAbst > (get_laenge() - minAbst))
	{
		str = getStammdaten().getText("IDS_ERROR110").c_str();  //Absoluter Abstand muss kleiner Rohrlänge - %dmm sein!
		err.Format(str, minAbst);
		return err;
	}

	return err;
}

///Liefert der Anzahl der Standardelemente auf dem Strang
int Strangrohr::getAnzahlMuffen()
{
	int mufanz = 0; 
	for (int i=0; i<MAX_ANZAHL_STDELEMENTE; i++)
	{
		if (muffenRelAbst[i] > 0)
			mufanz++;
		else
			break;
	}
	return mufanz;
}

///Strangrohrmuffen bekommen einen Aufschlag   
void Strangrohr::addMuffenausschuss(CObList& list)
{
	// Ausschuss fuer Strangrohrmuffen betraegt 3%, nur zu SAP nicht VK3
	Preisfaktoren pf;
	double faktor = 1+pf.getPreisfaktor(Preisfaktoren::STRANGROHRMUFFEN)/100;
	
	for (POSITION pos = list.GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(list.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->get_id() == FRMUFFE || ele->get_id() == KRMUFFE)
			ele->set_anzahl((int)((double)ele->get_anzahl() * faktor));
	}
}

///Fuegt dem Rohr einen Aufschlag hinzu fuer Verschnitt 
void Strangrohr::addRohrausschuss(CObList& list)
{
	// Ausschuss fuer Strangrohre betraegt 3%, zu SAP und VK3
	Preisfaktoren pf;
	double faktor = 1+pf.getPreisfaktor(Preisfaktoren::STRANGROHRVERSCHNITT)/100;

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

///Liefert das Gesamtgewicht der Rohrkonstruktion aus der Stueckliste
double Strangrohr::getGesGewicht(int teile) const
{
	/*
	teile: 0=alle, 1=Einkauf, 2=Lager, 3=ohne Preis 
	Gesamtgewicht einer Rohrleitung ermitteln. 
	Zur Berechnung des gesamten Verteilers muss noch mit
	der Anzahl multipliziert werden.
	*/

	// Ausschuss fuer Strangrohre betraegt 3%, zu SAP und VK3
	// Fuer Gewicht muss man diesen wieder abziehen 
	Preisfaktoren pf;
	double faktor = pf.getPreisfaktor(Preisfaktoren::STRANGROHRVERSCHNITT);
	faktor = 100/(100+faktor); 

	double gesGew = 0.0;
	// Alles multiplizieren mit Anzahl der Rohre
	POSITION spos = stueckliste.GetHeadPosition();
	for (spos = stueckliste.GetHeadPosition(); spos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(stueckliste.GetNext(spos));
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

///Setzt diverse Rohrdaten
void Strangrohr::initDaten(int anzahl, int dnvl, int laenge, CString posNr, int rohrQuali, int rohrVerb)
{
	Rohrleitung::initDaten(anzahl, dnvl, laenge, posNr, rohrQuali, rohrVerb);

	set_muffenRestabstand(0);
}

///Prueft Anfang/Ende auf Scheibe und verzinkt
void Strangrohr::check_AnfangEnde_ScheibeOk() const
{
	CString err;
	if (rohrAnfang == SCH || rohrEnde == SCH) 
	{
		if (getBeschichtung().isVerzinkt())
		{
			err = getStammdaten().getText("IDS_ERROR120").c_str(); 
			throw err;
		}
	}
	Rohrleitung::check_AnfangEnde_ScheibeOk();
}

///Erzeugt die VK3-Elemente aus der Elementliste
bool Strangrohr::createVk3Liste()
{
	bool zusatzele = false;

	//Liste löschen
	vk3elementeListe.deleteAllElemente();

	
	int count = -1;
	// Liste mit Elementen durchgehen
	bool bScheibeFound = false;
	for (POSITION pos = elementList.GetHeadPosition(); pos;)
	{
		count++;
		Element *e = dynamic_cast<Element *>(elementList.GetNext(pos));
		if(e->get_id() == 51) // Hauptrohr
		{
			
			if( (e->get_dn() == 65 || e->get_dn() == 80) && (ElementAnzahldnMuffeOrDBohrung == 20 || ElementAnzahldnMuffeOrDBohrung == 25) )
			{
				e->set_ElementAnzahlid(ElementAnzahlid);
				e->set_ElementAnzahldn(ElementAnzahldnMuffeOrDBohrung);
			}
			
		}
		XASSERT(e);
		if (!e)
			break;

		// Alles hinter diesen Elementen muss berechnet werden, 
		// jetzt auch NUT, die sonst bei Strangrohr nichts kostet.  
		if (e->isEleRedBogEinz())
			zusatzele = true;

		if (!zusatzele && (e->get_id() == GLATT || e->get_id() == LEER || 
			               e->get_id() == NUT   || e->get_id() == SCHWEISS))
			continue;

		Element* ele = new Element;
		ele->copy(*e);
		
		if(Werkauftrag::MAGDEBURG == (Werkauftrag::FERTIGUNGSSTAETTE) Get_Fertigungsstaette())
		{	
/*
			if(e->get_id() == SCH)
			{
				bScheibeFound = true;
				delete ele;
				continue;
			}

			if(get_beschichtung().isGepulvert())
			{
				switch(ele->get_artikelnr())
				{
					case 841248:
					case 841249:
					case 841247:
					case 843032:
					{
						ele->initStammdaten(dnvl, dnvl, 51);
						ele->set_mass1(ele->get_laenge());
						ele->SetDoIgnoreRohrOrVk3Rohr(true);
						break;
					}

					
					case 104086:
					case 104128:
					case 104165:
					{
						ele->initStammdaten(dnvl, dnvl, 752);
						ele->set_mass1(ele->get_laenge());
						ele->SetDoIgnoreRohrOrVk3Rohr(true);
						break;
					}
					
				}
			}
			else
			{ 
				// verzinkt
				switch(ele->get_artikelnr())
				{
					case 841248:
					case 841249:
					case 841247:
					{
						ele->initStammdaten(dnvl, dnvl, 651);
						ele->set_mass1(ele->get_laenge());
						ele->SetDoIgnoreRohrOrVk3Rohr(true);
						break;
					}
				}
			}	
			*/
		}

		if (ele->isEleRohr())
		{
			// grundiert, 
			if (get_beschichtung().isGrundiert()) 
				ele->set_id(ele->get_id() + 600);		
		}
		
		// Element in Liste packen
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


	//Auf Halbe Meter runden
	//vk3elementeListe.rundenHalbeMeter(); //Ab Version3.2 kein Runden mehr

	//Menge hochsetzen
	vk3elementeListe.setMenge(get_anzahl());
	
	if(Werkauftrag::MAGDEBURG == (Werkauftrag::FERTIGUNGSSTAETTE) Get_Fertigungsstaette())
	{

		if(bScheibeFound)
		{
			//set_stdbemerkung("");
			set_stdbemerkung("Scheibe = Nut + Endkappe wird baustellenseitig bereitgestellt<br/>");
		}
	}

	for (POSITION pos = vk3elementeListe.GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(vk3elementeListe.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if(ele->get_id() == SPRINKLERSCHELLE_VIROTEC)
		{
			Element *e = new Element;
			e->copy(*ele);
			e->set_id(BOHR50);
			vk3elementeListe.AddTail(e);
		}
		
	}
			
	
	// zuletzt noch Preise setzen
	m_iPreiswertigkeit = vk3elementeListe.setVk3Preise(get_beschichtung().isVerzinkt(), get_laenge(), getAnzahlMuffen(), get_muffenLaenge());

	

//#ifdef _DEBUG
//afxDump << get_vk3elementeListe();
//#endif

	return true;
}

///Strangrohre dürfen nur mit oder ohne Nacharbeit innerhalb eines Sammelauftrags vorliegen. 
bool Strangrohr::avNacharbeitsPruefung() const
{
	return true;
}

///Liefert die maximale Anzahl der Standardelemente
const int Strangrohr::getMaxAnzahlStdelemente() const
{
	return MAX_ANZAHL_STDELEMENTE;
}

///Liefert den rel. Abstand der Standardelemente
const int Strangrohr::getMuffenRelAbst(int index) const
{
	return get_muffenRelAbst(index);
}

///Liefert den Restabstand zum Rohrende vom letzten Standardelement
const int Strangrohr::getMuffenRestabstand() const
{
	return get_muffenRestabstand();
}

///Liefert das Array mit Nachbearbeitungselementen
EnContainerRohrElemente& Strangrohr::getElementArray()
{
	return get_avelementArray();
}

///Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
CString Strangrohr::getTabname()
{
	return "avelement";
}

///verschiebt die relativen Abstaende nach links, wenn zwischendrin einer 0 ist 
void Strangrohr::sortMuffen() 
{
	for (int i=0; i<MAX_ANZAHL_STDELEMENTE-1; i++)
	{
		if (muffenRelAbst[i] == 0)
		{
			muffenRelAbst[i] = muffenRelAbst[i+1];
			muffenRelAbst[i+1] = 0;
		}
	}
}

///bererchnet die nachfolgenden relativen Abstaende neu, wenn zwischendrin einer 0 gesetzt wird
void Strangrohr::setMuffenRelAbst(int index, int value)
{
	
		if (value == 0)
		{
			for (int i=index+1; i<MAX_ANZAHL_STDELEMENTE; i++)
			{
				if (muffenRelAbst[i] > 0)
					muffenRelAbst[i] = muffenRelAbst[i] + muffenRelAbst[index];
			}
		}
		set_muffenRelAbst(index, value);
	
}

///Erzeugen der Afos fuer die Beschichtung
void Strangrohr::createBeschAfos(Arbeitsplan& plan)
{
	// Beschichtung
	if (!get_beschichtung().isVerzinkt() || (get_rohrQuali() != RQ2440VZIN && get_rohrQuali() != RQEN10217VERZ))
		plan.addAfo_Beschichtung(get_anzahl(), get_beschichtung(), getPulverzeit(), getGrundierungszeit(), getPutzzeit());
}

string Strangrohr::get_abschluss_kurzbezeichnung(bool anfang) const
{
	/**
	Liefert für Gewinde nur GEW statt des Textes aus den Stammdaten. Bei Ende und Nacharbeit mit Bogen
	wird immer Glatt zurückgegeben, da der Bogen angeschweisst werden muss
	*/
	int id = anfang ? get_rohrAnfang() : get_rohrEnde();
	
	if (!anfang && isBogen())
		id = GLATT;
 
	if (id == GEW)
	{
		CString txt;
		txt = getStammdaten().getText("IDS_GEW").c_str(); 
		return (LPCSTR)txt;
	}
	BauelementGruppen bg;
	return (LPCSTR)bg.getBezeichnung(id);
}

int Strangrohr::get_sonderMuffenLaenge() const
{
	return sonderMuffenLaenge;
}

void Strangrohr::set_sonderMuffenLaenge(int laenge)
{
	sonderMuffenLaenge = laenge;
}


///Muffenlänge in Abhängigkeit des Typs berechnen
int Strangrohr::getMuffenLaenge()
{
	int laenge = 0;

	Element ele;
	try 
	{ 
		if (get_muffenID() == SONDERMUFFE)
			laenge = get_sonderMuffenLaenge();
		else
		{
			if (get_muffenID() == BOHR24)
				ele.initStammdaten(get_muffenDN(),get_dnvl(),get_muffenID());
			else if (get_muffenGekehlt())
				ele.initStammdaten(get_muffenDN(),get_dnvl(),KRMUFFE);
			else 
				ele.initStammdaten(get_muffenDN(),get_dnvl(),FRMUFFE);
			ele.initDaten(0, 0, 0, 0, 0);

			laenge = ele.get_laenge();
		}
	}
	catch(Element::Error e) 
	{
		getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Information);
		
	}

	return laenge;
}


void Strangrohr::check_rohrQuali(int dnvl) const
{
	int quali = get_rohrQuali();

	// Supp.Nr. 15323: Kein Fehler bei AV Version und verzinktem/gepulvertem Strangrohr > 9000 mm
	if (WaxSys::isAV())
	{
		if ((getBeschichtung().isVerzinkt() || getBeschichtung().isGepulvert()) && get_laenge() > 9000 && quali == RQ2458GSCH)
			return;
	}
	
	Rohrleitung::check_rohrQuali(dnvl);
}

int Strangrohr::getStandardRohrlaenge()
{
	int bogenmass = 0; 
	for (int j=0; j<get_avelementArray().GetSize(); j++)
	{
		// Hinter Bogen duerfen keine Standardelemente mehr sein
		Element *ele = get_avelementArray().get_element_at(j);
		if (!ele)
			continue;
		if (ele->isEleBogen())
		{
			bogenmass = ele->get_abstAbs() - ele->get_laenge();
			break;
		}	
		else if (ele->isEleRedEinz())
		{
			bogenmass = ele->get_abstAbs();
		}
	}
	
    int laenge = bogenmass > 0 ? bogenmass : get_laenge();

	return laenge;
}






int Strangrohr::getRelLaengeStdMuffe(int nr)const
{
	
	if (nr < MAX_ANZAHL_STDELEMENTE && get_muffenRelAbst(nr) == 0)
		return 0;

	int lmuffePrev = 0;
	int lmuffe = 0;
	for (int i=0; i <= nr && i < MAX_ANZAHL_STDELEMENTE; i++)
	{
		if (i-1 >= 0)
			lmuffePrev += muffenRelAbst[i-1];

		lmuffe += muffenRelAbst[i];
	}

	if (nr >= MAX_ANZAHL_STDELEMENTE)
		lmuffe = this->get_laenge();


	//Prüfen ob vor der StdMuffe ein Knick liegt
	double zusLaenge = 0;
	bool isSet = false;
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;


		if (!ele->isEleRohr())
			continue;

		if (ele->get_uelementList().GetSize() <= 0)
			continue;

		bool isbreak = false;
		
		for (POSITION pos = ele->get_uelementList().GetHeadPosition(); pos;)
		{
			const Element *uele = dynamic_cast<const Element *>(ele->get_uelementList().GetNext(pos));
			XASSERT(uele);
			if (!uele)
				break;

			if (!uele->isKnick())
				continue;

			if (uele->get_abstAbs() > lmuffe)
			{
				isbreak = true;
				break;
			}

			//Abstand nur berechnen wenn Knick zwischen den Muffen liegt
			if (uele->get_abstAbs() > lmuffePrev)
			{
				//zusätzlich Länge berechnen
				
				zusLaenge += calcBiegenZuschlag();
				isSet = true;
			}
		}		
		if (isbreak)
			break;
	}

	if (isSet)
	{
		//Runden
		if (zusLaenge < 0)
			zusLaenge -= 0.5;
		else
			zusLaenge += 0.5;
		int iLaenge = (int)zusLaenge;
		if (iLaenge != 0)
		{
			if (nr >= MAX_ANZAHL_STDELEMENTE)
				return get_muffenRestabstand() + iLaenge;
			else
				return get_muffenRelAbst(nr) + iLaenge;
		}
	}

	if (nr >= MAX_ANZAHL_STDELEMENTE)
		return get_muffenRestabstand();
	else
		return get_muffenRelAbst(nr);
}


