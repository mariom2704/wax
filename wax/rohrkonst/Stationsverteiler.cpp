#include "stdafx.h"
#include "resource.h"


#include "Stationsverteiler.h"
#include "GpConvertOldVersion.h"
#include "Element.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

using namespace MxErrorHandling;

//Standardkonstruktor
Stationsverteiler::Stationsverteiler()
{
	set_dnvl(100);
	memset(elementRelAbst, 0, sizeof(elementRelAbst));
	memset(elementAbsAbst, 0, sizeof(elementAbsAbst));
	memset(elementDN,      0, sizeof(elementDN));
	memset(muffenRelAbst,  0, sizeof(muffenRelAbst));
	elementID         = ROHR; 
	elementLaenge     = 200;
	elementlEnde      = NUT;
	elementrEnde      = NUT;
	extraAbgDN        = 0;
	extraAbgAbsAbst   = 0;
	extraAbgID        = 0;
	extraAbgLaenge    = 0;
	extraAbgObenDN    = 0;
	extraAbgObenAbsAbst= 0;
	extraAbgObenID    = 0;
	extraAbgObenLaenge= 0;
	elementlEndeLaenge= 0;
	elementrEndeLaenge= 0;
	elementDN[0] = 100; 
	set_dnvl(elementDN[0]);
	elementRelAbst[0] = 1000;
	elementAbsAbst[0] = 1000;

	memset(rohrLaengen, 0, sizeof(rohrLaengen));
	stdbezeichnung = getStammdaten().getText("IDS_STATIONSVERT").c_str(); 

	CString str; 
	str = getStammdaten().getText("IDS_STDTEXT_STATIONSVERT1").c_str(); 
	str += getStammdaten().getText("IDS_STDTEXT_STATIONSVERT2").c_str(); 
	set_stdbemerkung(str);
}

//Destruktor
Stationsverteiler::~Stationsverteiler()
{
	
}

//Speichern und Laden ders Stationsverteilers.
void Stationsverteiler::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	// hier nicht ändern
	Rohrleitung::serialize(ar);
	if (ar.is_storing())
	{
		ar	<< extraAbgAbsAbst		<< extraAbgDN			<< extraAbgID
			<< extraAbgLaenge;
		ar	<< extraAbgObenAbsAbst	<< extraAbgObenDN		<< extraAbgObenID
			<< extraAbgObenLaenge;
		int i;
		for (i = 0; i < MAXCOUNT; i++)
		{
			ar << elementRelAbst[i];
			ar << elementDN[i];
		}
		ar	<< elementLaenge		<< elementlEnde			<< elementrEnde; 
		ar	<< avelementArray.GetSize();
	
		for (i=0; i<avelementArray.GetSize(); i++) 
		{
			Element *ele = avelementArray.get_element_at(i);
			ele->serialize(ar);
		}
	} 
	else 
	{
		int anzahl = 0;
		ar	>> extraAbgAbsAbst		>> extraAbgDN			>> extraAbgID 
			>> extraAbgLaenge;
		if (version > 19)
			ar	>> extraAbgObenAbsAbst	>> extraAbgObenDN	>> extraAbgObenID
				>> extraAbgObenLaenge;

		int i;
		for (i = 0; i < MAXCOUNT; i++)
		{
			ar >> elementRelAbst[i];
			ar >> elementDN[i];
		}
		ar	>> elementLaenge		>> elementlEnde			>> elementrEnde; 
		

		if (version <= 37)
		{
			elementlEnde = GpConvertOldVersion::getNewFlanschId(elementlEnde,get_dnvl());
			elementrEnde = GpConvertOldVersion::getNewFlanschId(elementrEnde,get_dnvl());
		}
		
		
		ar	>> anzahl;
		
		for (i=0; i<anzahl; i++) 
		{
			Element *ele = new Element;
			ele->serialize(ar);
			avelementArray.Add(ele);
		}

		// Elementdaten setzen
		setElementdaten(elementDN,    elementRelAbst, elementID,       elementLaenge, elementlEnde, elementrEnde,	
			extraAbgAbsAbst, extraAbgDN, extraAbgID,   extraAbgLaenge,
			extraAbgObenAbsAbst, extraAbgObenDN, extraAbgObenID,   extraAbgObenLaenge);
	}
}

//Elementdaten setzen
void Stationsverteiler::setElementdaten(int elementDN[6], int elementRelAbst[6], int elementID, 
										int elementLaenge,		 int elementlEnde,	 int elementrEnde, 
										int extraAbgAbsAbst,	 int extraAbgDN,	 int extraAbgID,     int extraAbgLaenge, 
										int extraAbgObenAbsAbst, int extraAbgObenDN, int extraAbgObenID, int extraAbgObenLaenge)
{
	int i;
	for (i = 0; i < MAXCOUNT; i++) 
	{
		if (elementRelAbst[i] == 0) 
			break;
		if (i == 0) 
			elementAbsAbst[i] = elementRelAbst[i];
		else        
			elementAbsAbst[i] = elementAbsAbst[i-1] + elementRelAbst[i];
	}
	
	for (i = 0; i < MAXCOUNT; i++) 
	{
		this->elementDN[i]		= elementDN[i];
		this->elementRelAbst[i]	= elementRelAbst[i];
	}
	this->elementID			= elementID;
	this->elementLaenge		= elementLaenge;
	this->elementlEnde		= elementlEnde;
	this->elementrEnde		= elementrEnde;
	this->extraAbgAbsAbst	= extraAbgAbsAbst;
	this->extraAbgDN		= extraAbgDN;
	this->extraAbgID		= extraAbgID;
	this->extraAbgLaenge	= extraAbgLaenge;
	this->extraAbgObenAbsAbst= extraAbgObenAbsAbst;
	this->extraAbgObenDN	= extraAbgObenDN;
	this->extraAbgObenID	= extraAbgObenID;
	this->extraAbgObenLaenge= extraAbgObenLaenge;

	rohrAnfang = elementlEnde;
	rohrEnde = elementrEnde;
}

//Erzeugung der Rohrkonstruktion
void Stationsverteiler::createModell()
{
	Rohrleitung::createModell();

	// Das Array mit Elementen fuellen
	fill_elementArray();
	
	if (get_laenge() > 0) 
	{
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
		//setRohrlaengen();

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
	createPulverlaengenListe();
	createPasslaengenListe();



}

//Das Array mit Elementen fuellen
void Stationsverteiler::fill_elementArray()
{
	elementArray.deleteAll();
	int i = 0;
	int rel_abst[MAXCOUNT]; // relative Abstände, Pinökel wird eingerechnet

	// Flansche auf Rohrstutzen
	Element flansche[MAXCOUNT];
	

	// Absolute und relative Abstände errechnen und tatsächliche Flanschids und Längen setzen
	for (i = 0; i < MAXCOUNT; i++) 
	{
		elementAbsAbst[i] = i ? elementAbsAbst[i-1] + elementRelAbst[i] : elementRelAbst[i];
		rel_abst[i] = elementRelAbst[i];

		// Aus FFL wird VFLAN oder VFL16
		if (elementDN[i] != 0) 
		{
			try
			{
				int flid = correctElemID(FFL, elementDN[i]);
				flansche[i].initStammdaten(elementDN[i], elementDN[i], flid);
				flansche[i].initDaten(elementAbsAbst[i], 0, 0, flid, 0);
			}
			catch (Element::Error e) 
			{
				getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Information);
				
			}
		}
	}
	

	int ins_i = -1; 
	int ins_j = -1; 
	// index berechnen, bei dem der Pinökel eingefügt werden muss
	if (extraAbgAbsAbst && extraAbgID)
	{
		for (i = 0; i < MAXCOUNT; i++) 
		{
			if (elementAbsAbst[i] == elementAbsAbst[i-1])
				break;
			if (elementAbsAbst[i] > extraAbgAbsAbst)
			{
				rel_abst[i] = elementAbsAbst[i] - extraAbgAbsAbst; // relativer Abstand neu für Element direkt hinter Pinökel
				break;
			}
		}
		ins_i = i;
	}
	// index berechnen, bei dem der Pinökel oben eingefügt werden muss
	if (extraAbgObenAbsAbst && extraAbgObenID)
	{
		for (i = 0; i < MAXCOUNT; i++) 
		{
			if (elementAbsAbst[i] == elementAbsAbst[i-1])
				break;
			if (elementAbsAbst[i] > extraAbgObenAbsAbst)
			{
				rel_abst[i] = elementAbsAbst[i] - extraAbgObenAbsAbst; // relativer Abstand neu für Element direkt hinter Pinökel
				break;
			}
		}
		ins_j = i;
	}
	if (ins_i == ins_j && i < 6)
	{
		if (extraAbgAbsAbst > extraAbgObenAbsAbst)
			rel_abst[i] = elementAbsAbst[i] - extraAbgAbsAbst;
		else
			rel_abst[i] = elementAbsAbst[i] - extraAbgObenAbsAbst;
	}
	
	Element *ele1 = NULL, *ele2 = NULL, *ele3 = NULL, *ele4 = NULL;
	// Elemente in Elementarray einfügen, bei ins_i/ins_j werden die Pinökel eingefügt
	for (i = 0; i < MAXCOUNT; i++) 
	{
		if (ins_i == i) 
		{
			int rel = 0;
			if (i>0)
				rel = extraAbgAbsAbst - elementAbsAbst[i-1];
			else
				rel = extraAbgAbsAbst;
			if (ins_i == ins_j)
			{
				if (extraAbgAbsAbst > extraAbgObenAbsAbst)
					rel = extraAbgAbsAbst - extraAbgObenAbsAbst;
				else 
					rel = 0;
			}

			ele1 = new Element(extraAbgID, extraAbgLaenge, extraAbgDN, get_dnvl(), rel, extraAbgAbsAbst, 180);
			// erstmal Muffe
			if (ins_i != ins_j) 
				elementArray.Add(ele1);
			
			if (extraAbgID == NUTST || extraAbgID == GWRST) 
			{
				// ID auf Rohrstutzen ändern
				ele1->set_id(ROHR);
				// erstmal Nut

				//Änderung 03.04.06,es wird dnvl von Stutzen übergeben, anstatt des Hauptrohres 
				ele2 = new Element(extraAbgID == GWRST ? GEW : NUT, 0, extraAbgDN, extraAbgDN, 0, extraAbgAbsAbst, 180);				
				//ele2 = new Element(extraAbgID == GWRST ? GEW : NUT, 0, extraAbgDN, get_dnvl(), 0, extraAbgAbsAbst, 180);
			
				if (ins_i != ins_j) 
					elementArray.Add(ele2);
			} 			
		}
		
		if (ins_j == i) 
		{
			int rel = 0;
			if (i>0)
				rel = extraAbgObenAbsAbst - elementAbsAbst[i-1];
			else
				rel = extraAbgObenAbsAbst;
			if (ins_i == ins_j)
			{
				if (extraAbgObenAbsAbst > extraAbgAbsAbst)
					rel = extraAbgObenAbsAbst - extraAbgAbsAbst;
			}

			ele3 = new Element(extraAbgObenID, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), rel,	extraAbgObenAbsAbst, 0);
			// erstmal Muffe
			if (ins_i != ins_j) 
				elementArray.Add(ele3);
			
			if (extraAbgObenID == NUTST || extraAbgObenID == GWRST) 
			{
				// ID auf Rohrstutzen ändern
				ele3->set_id(ROHR);
				// erstmal Nut

				//Änderung 03.04.06,es wird dnvl von Stutzen übergeben, anstatt des Hauptrohres 
				ele4 = new Element(extraAbgObenID == GWRST ? GEW : NUT, 0, extraAbgObenDN, extraAbgObenDN, 0, extraAbgObenAbsAbst, 0);
				//ele4 = new Element(extraAbgObenID == GWRST ? GEW : NUT, 0, extraAbgObenDN, get_dnvl(), 0, extraAbgObenAbsAbst, 0);

				if (ins_i != ins_j) 
					elementArray.Add(ele4);
			} 			
		}
		if (ins_i == ins_j && ins_j == i) 
		{
			if (extraAbgAbsAbst < extraAbgObenAbsAbst)
			{
				if (ele1 != NULL) 
					elementArray.Add(ele1);
				if (ele2 != NULL) 
					elementArray.Add(ele2);
				if (ele3 != NULL) 
					elementArray.Add(ele3);
				if (ele4 != NULL) 
					elementArray.Add(ele4);
			}
			else
			{
				if (ele3 != NULL) 
					elementArray.Add(ele3);
				if (ele4 != NULL) 
					elementArray.Add(ele4);
				if (ele1 != NULL) 
					elementArray.Add(ele1);
				if (ele2 != NULL) 
					elementArray.Add(ele2);
			}
		}

		// nicht ausgefüllte Felder werden übergangen
		if (!elementRelAbst[i] || !elementDN[i])
			continue;

		// Rohrstutzen
		elementArray.Add(new Element(elementID, elementLaenge, elementDN[i], get_dnvl(), 
							rel_abst[i], elementAbsAbst[i]));

		// Flansch
		elementArray.Add(new Element(flansche[i]));
	}
	
	set_muffenRelAbst();
	processAVElemente();	
	
	// Rel. Abstaende neu berechnen
	elementArray.calcRelAbst();
}

//Laenge von Anfang/Ende/Verbindung setzen
void Stationsverteiler::setAnfaEndeVerbLaenge()
{
	Rohrleitung::setAnfaEndeVerbLaenge();

	//Todo: Variablen umbenennen 
	elementlEndeLaenge = rohrAnfaLaenge;
	elementrEndeLaenge = rohrEndeLaenge;
}

//get avelementArray
const int Stationsverteiler::get_elementDN(int index) const
{
	return elementDN[index];
}

void Stationsverteiler::set_elementDN(int index, int value)
{
	elementDN[index] = value;
}

const int Stationsverteiler::get_elementRelAbst(int index) const
{
	return elementRelAbst[index];
}

void Stationsverteiler::set_elementRelAbst(int index, int value)
{
	elementRelAbst[index] = value;
}

const int Stationsverteiler::get_elementID() const
{
	return elementID;
}

void Stationsverteiler::set_elementID(int value)
{
	elementID = value;
}

//get elementLaenge
const int Stationsverteiler::get_elementLaenge() const
{
	return elementLaenge;
}

void Stationsverteiler::set_elementLaenge(int value)
{
	elementLaenge = value;
}

const int Stationsverteiler::get_elementlEnde() const
{
	return elementlEnde;
}

void Stationsverteiler::set_elementlEnde(int value)
{
	elementlEnde = value;
	rohrAnfang = elementlEnde;
}

const int Stationsverteiler::get_elementrEnde() const
{
	return elementrEnde;
}

void Stationsverteiler::set_elementrEnde(int value)
{
	elementrEnde = value;
	rohrEnde = elementrEnde;
}

const int Stationsverteiler::get_extraAbgAbsAbst() const
{
	return extraAbgAbsAbst;
}

void Stationsverteiler::set_extraAbgAbsAbst(int value)
{
	extraAbgAbsAbst = value;
}

const int Stationsverteiler::get_extraAbgDN() const
{
	return extraAbgDN;
}

void Stationsverteiler::set_extraAbgDN(int value)
{
	extraAbgDN = value;
}

const int Stationsverteiler::get_extraAbgID() const
{
	return extraAbgID;
}

void Stationsverteiler::set_extraAbgID(int value)
{
	extraAbgID = value;
}

const int Stationsverteiler::get_extraAbgLaenge() const
{
	return extraAbgLaenge;
}

void Stationsverteiler::set_extraAbgLaenge(int value)
{
	extraAbgLaenge = value;
}

//Hier wird automatisch der Arbeitsplan generiert
void Stationsverteiler::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	Rohrleitung::createAutoArbeit();

	// Afos aus Elementliste der Rohrleitung generieren
	Element *ele = new Element;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *element =  dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;
		
		if (element->get_id() == SCH)
		{
			get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(), 0, "0", Rohrleitung::WASSER, 1);
			continue;
		}
		get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(),  0, "0", Rohrleitung::WASSER);

		CObList& uelementList = element->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

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
}

//Rohrkonstruktion auf korrekte Werte checken
CString Stationsverteiler::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err, str, txt;

	// Max. Laenge checken
	if (get_laenge() > MAX_GESAMTLAENGE) 
	{ 
		set_laenge(MAX_GESAMTLAENGE);
		str = getStammdaten().getText("IDS_ERROR105").c_str(); 
        err.Format(str, MAX_GESAMTLAENGE);
		str = getStammdaten().getText("IDS_BITTE1").c_str(); 
		err += " " + str;
		return err;
	} 

	err = checkStdElemente();
	if (err.GetLength() > 0)
		return err;
	
	return Rohrleitung::checkPlausis();
}

CString Stationsverteiler::checkStdElemente()
{
	CString err, str;

	if (elementID == LEER) 
	{ 
		err = getStammdaten().getText("IDS_ERROR4a").c_str(); 
		return err;
	}

	int i=0;
	while (elementDN[i] != 0 && i < MAXCOUNT) 
	{   
		if (elementDN[i] !=  10 && elementDN[i] !=  15 && elementDN[i] !=  20
		 && elementDN[i] !=  25 && elementDN[i] !=  32 && elementDN[i] !=  40 
		 && elementDN[i] !=  50 && elementDN[i] !=  65 && elementDN[i] !=  80
		 && elementDN[i] != 100 && elementDN[i] != 125 && elementDN[i] != 150 
		 && elementDN[i] != 200 && elementDN[i] != 250 && elementDN[i] != 300
		 && elementDN[i] !=   0)
		{ 
			elementDN[i] =  get_dnvl();
			err = getStammdaten().getText("IDS_ERROR25").c_str(); 
			return err;
		}
		
		if (elementDN[i] > get_dnvl()) 
		{
			elementDN[i] =  get_dnvl();
			err = getStammdaten().getText("IDS_ERROR2a").c_str(); 
			return err;
		}
		i++;
	}
	if (elementLaenge > MAX_LAENGE_STDELEMENTE)
	{ 
		elementLaenge = MAX_LAENGE_STDELEMENTE;
		str = getStammdaten().getText("IDS_ERROR106").c_str(); 
		err.Format(str, MAX_LAENGE_STDELEMENTE);
		return err;
	}

	if (extraAbgAbsAbst > get_laenge()) 
	{ 
		extraAbgAbsAbst	= get_laenge()-100;
		err = getStammdaten().getText("IDS_ERROR107").c_str(); 
		return err;
	}
	if (extraAbgObenAbsAbst > get_laenge()) 
	{ 
		extraAbgObenAbsAbst	= get_laenge()-100;
		err = getStammdaten().getText("IDS_ERROR108").c_str(); 
		return err;
	}

	return err;
}

const int Stationsverteiler::get_muffenRelAbst(int index) const
{
	return muffenRelAbst[index];
}

//Berechnet aus dem Elementarray die relativen Abstände der Muffen.
void Stationsverteiler::set_muffenRelAbst()
{
	// Abstaende der Standardelemente inkl. Extraabgang (Muffe)
	int j = 0;
	memset(muffenRelAbst, 0, sizeof(muffenRelAbst));
	for(int i = 0; i < elementArray.GetSize(); i++)
	{
		Element *ele = elementArray.get_element_at(i);
		if (ele->get_abstRel() != 0) 
		{
			XASSERT(j < MAXCOUNT+2);
			if (j > MAXCOUNT+1)
				return;
			muffenRelAbst[j++] = ele->get_abstRel();
		}
	}
}

const int Stationsverteiler::getMuffenRestabstand() const
{
	int muffenAbsAbst = 0;

	for (int i=0; i<MAX_ANZAHL_STDELEMENTE ; i++)
		muffenAbsAbst += muffenRelAbst[i];
	
	return (get_laenge() - muffenAbsAbst); 
}

const int Stationsverteiler::get_extraAbgObenAbsAbst() const
{
	return extraAbgObenAbsAbst;
}

void Stationsverteiler::set_extraAbgObenAbsAbst(int value)
{
	extraAbgObenAbsAbst = value;
}

const int Stationsverteiler::get_extraAbgObenDN() const
{
	return extraAbgObenDN;
}

void Stationsverteiler::set_extraAbgObenDN(int value)
{
	extraAbgObenDN = value;
}

const int Stationsverteiler::get_extraAbgObenID() const
{
	return extraAbgObenID;
}

void Stationsverteiler::set_extraAbgObenID(int value)
{
	extraAbgObenID = value;
}

const int Stationsverteiler::get_extraAbgObenLaenge() const
{
	return extraAbgObenLaenge;
}

void Stationsverteiler::set_extraAbgObenLaenge(int value)
{
	extraAbgObenLaenge = value;
}

const int Stationsverteiler::getMaxAnzahlStdelemente() const
{
	return MAX_ANZAHL_STDELEMENTE;
}

const int Stationsverteiler::getMuffenRelAbst(int index) const
{
	return get_muffenRelAbst(index);
}

EnContainerRohrElemente& Stationsverteiler::getElementArray()
{
	return get_avelementArray();
}

///Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
CString Stationsverteiler::getTabname()
{
	return "avelement";
}


//Je nach Bedingung wird die richtige ID geliefert
int Stationsverteiler::correctElemID(int id, int dn) const
{
	
	int retTyp = Rohrleitung::correctElemID(id,dn);


	if (id == FFL && rohrDruckstufe == 16)
		retTyp = VFL16;

	return retTyp;
}