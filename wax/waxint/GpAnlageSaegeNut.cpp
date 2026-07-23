#include "stdafx.h"


#include "GpAnlageSaegeNut.h"
#include "..\rohrkonst\mxcoblist.h"
#include "resource.h"

GpAnlageSaegeNut::GpAnlageSaegeNut(MxCObList *auftraege)
: GpAnlage(auftraege), dbAnlage(NULL)
{
	minTeilrohrLaenge = Verteiler::A_MIN_ROHRLAENGE; //Mindestrohrlänge
	maxTeilrohrLaenge = Verteiler::A_MAX_ROHRLAENGE; //Maximale Länge des Rohres auf der Sägenutanlage
	minDn = Verteiler::A_MIN_DNVL;
	maxDnSaegeNut = 200;
	maxDnVerteiler = Verteiler::A_MAX_DNVL;

	maxLaengeAblage4 = 0;  //Maximale Länge der Ablage 4  (es soll alles auf Ablage 3 laufen)

	minDnMuffe = 15;
	maxDnMuffe = 65;
	minDnStutzen = 32;
	maxDnStutzen = 65;

	minAbstRohrAnfangEndeSaegeNut = 65; //Mindestabstand zum Rohranfang bzw- -ende von Lochmitte
	isMinAbstandVonMitteLoch = false;       //true=von Mitte Loch, false=von Außenkante Loch


	minAbstAnfangVerteiler = Verteiler::A_MIN_ABSTAND_ANFANG;
	minAbstEndeVerteiler = Verteiler::A_MIN_ABSTAND_ENDE;
}

GpAnlageSaegeNut::~GpAnlageSaegeNut(void)
{
}


/**
Werte für die Schnittstelle zur Sägenutanlage und Verteilerrohrstrasse erzeugen und in die
Tabellen schreiben
*/
bool GpAnlageSaegeNut::createSaegeNutVerteilerDb()
{
	//Daten erzeuegen
	if (!createWerte())
		return false;

	//Daten in die Datenbank schreiben
	if (!writeDatenInDb())
	{
		AfxMessageBox(getStammdaten().getText("IDS_SAEGENUT_DB_FEHLER").c_str(), MB_OK | MB_ICONERROR); //Fehler beim Schreiben in die Datenbank!
		return false;
	}

	return true;
}

Werkauftrag *GpAnlageSaegeNut::getFirstAuftrag()
{
	MxCObList *auftraege = get_auftraege();
	if (!auftraege)
		return NULL;

	//Kopfsatz schreiben
	Werkauftrag *werkauftrag = dynamic_cast<Werkauftrag*>(auftraege->getFirst());
	XASSERT(werkauftrag);
	if (!werkauftrag)
		return NULL;

	return werkauftrag;
}

bool GpAnlageSaegeNut::createWerte()
{
	Werkauftrag *werkauftrag = getFirstAuftrag();
	if (!werkauftrag)
		return false;
	
	//prüfen ob Auftrag schon gefertigt ist. Status pürfen
	if (!checkStatus(werkauftrag->get_status().get_wanr()))
	{
		AfxMessageBox(getStammdaten().getText("IDS_SAEGENUT").c_str(), MB_OK | MB_ICONERROR);
		return NULL;
	}

	//Kopfsatz schreiben
	if (!createKopfsatz(werkauftrag))
		return false;

	//Listen zurücksetzen
	saegeNutRohrListe.clear();
	saegeNutRohrListeLegacy.clear();
	saegeNutLoecherListe.clear();
	verteilerRohrListe.clear();
	verteilerNutAbgangListe.clear();


	MxCObList *auftraege = get_auftraege();

	//Rohrdaten schreiben
	MxCObListIterator iter(*auftraege);
	while (iter.next())
	{
		Werkauftrag *werkauftrag = dynamic_cast<Werkauftrag*>(auftraege->getAt(iter));
		XASSERT(werkauftrag);
		if (!werkauftrag)
			return false;

		if (werkauftrag->get_waid() <= 0)
		{
			AfxMessageBox(getStammdaten().getText("IDS_KEINE_WAID").c_str(), MB_OK | MB_ICONERROR); 
			return false;
		}

		//Rohredaten erstellen
		if (!createRohre(werkauftrag))
			return false;
	}

	//Anlagentyp im Kopfsatz Sägenut+Verteiler setzen, wenn es Rohre gibt die an die Verteilerrohrstrasse übergeben werden
	list<DbAnlage::saegeNutRohre>::iterator iterSnR = saegeNutRohrListe.begin();
	while (iterSnR != saegeNutRohrListe.end())
	{
		if (iterSnR->typ == (int)GpAnlageSaegeNut::TypVerteilerAnlage)
		{
			kopfData.anlagentyp = (int)GpAnlageSaegeNut::AnlageSaegeNutUndVerteiler;
			break;
		}
		iterSnR++;
	}


	//Rohrlisten sortieren
	saegeNutRohrListe.sort(GpAnlageSaegeNut::greater);
	saegeNutRohrListeLegacy.sort(GpAnlageSaegeNut::greater);

	return true;
}


/**
Datensätze zur Wanr aus den Tabellen löschen
*/
bool GpAnlageSaegeNut::cleanTables(int wanr)
{
	//Liste mit Waids ermitteln
	list<int> waids;
	dbAnlage.readWaidsFromSaegeNutRohre(wanr, waids);

	//Datensätze zur Wanr löschen
	dbAnlage.deleteDatenWanr(dbAnlage.get_tabAuftraege(), wanr);
	dbAnlage.deleteDatenWanr(dbAnlage.get_tabSaegenutRohre(), wanr);


	//Abhängige Datensätze zur Wanr löschen
	list<int>::iterator iter = waids.begin();
	while (iter != waids.end())
	{
		cleanTablesWaid(*iter);
		iter++;
	}

	return true;
}

/*
Datensätze zur Waid löschen
*/
bool GpAnlageSaegeNut::cleanTablesWaid(int waid)
{
	dbAnlage.deleteDatenWaid(dbAnlage.get_tabSaegenutRohreDaten(), waid);
	dbAnlage.deleteDatenWaid(dbAnlage.get_tabSaegeNutLoecher(), waid);
	dbAnlage.deleteDatenWaid(dbAnlage.get_tabVerteilerRohre(), waid);
	dbAnlage.deleteDatenWaid(dbAnlage.get_tabVerteilerNutAbgang(), waid);

	return true;
}


/**
Datensätze für die Rohrtabelle für die Sägenutanlage und Verteilerrohrstrasse erzeugen und in die Db schreiben

Der Werkauftrag muss in der Elementliste die Rohre schon in den richtigen Längen aufgeteilt haben (L<6m). 
Abzüge für Schweißnähte müssen auch berücksichtigt sein.

*/
bool GpAnlageSaegeNut::createRohre(Werkauftrag *werkauftrag)
{

	DbAnlage::saegeNutRohre rohrDaten;
	DbAnlage::verteilerRohre vertRohrDaten;
	

   //Rohrtyp
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	if (!rohr)
		return false;

	Handelslaenge *handelsl = dynamic_cast<Handelslaenge*>(werkauftrag);
	if (handelsl)
		rohrDaten.typ = (int)GpAnlageSaegeNut::TypHandelslaenge;
	else
		rohrDaten.typ = (int)GpAnlageSaegeNut::TypVerteilerhandschweissung;

	EnCObListModell &elemente = rohr->get_elementList();


	//Rohrdaten für Verteilerrohranlage schreiben
	bool hasAnlagenTeile = false;
	if (elemente.hasAnlagenTeile() && rohr->get_dnvl() <= maxDnVerteiler)
	{
		hasAnlagenTeile = true;
		if (!createVerteilerRohre(rohr, vertRohrDaten))
			return false;
	}

	//Allgemeine Daten Sägenut
	rohrDaten.wanr = werkauftrag->get_status().get_wanr();
	rohrDaten.status = 0; //Dummywert, wird von der Sägenutanlage verwendet
	rohrDaten.sortierung = 0; //erstmal 0, wird später festgelegt
	rohrDaten.waid = werkauftrag->get_waid();
	rohrDaten.posNr = werkauftrag->get_posNr();
	rohrDaten.anzahl = werkauftrag->get_anzahl();


	//Elementliste durchlaufen
	bool isPrevTeilrohrVertAnlage = false; //Merker ob das vorherige Teilrohr Verteiler Anlage ist.
	int teilRohrCounter = 1;
	int absoluteLaenge = 0;	
	Element *prevEl = NULL;
	Element *nextEl = NULL;
	MxCObListIterator iter(elemente);
	while (iter.next())
	{	
		Element *element = dynamic_cast<Element*>(elemente.getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		if (!element->isEleRohr()) 
		{
			absoluteLaenge +=   (element->isEleBogen()) ? (2*element->getLaengeZuschnitt()) : element->getLaengeZuschnitt();
			prevEl = element;
			continue;
		}

		//Rohre filtern wenn, L<1500, Dn<65, DN>250
		if (element->getLaengeZuschnitt() < minTeilrohrLaenge ||
			element->get_dn() < minDn ||
			element->get_dn() > maxDnSaegeNut)
		{
			absoluteLaenge += element->getLaengeZuschnitt();
			prevEl = element;
			continue;
		}

		//nächstes Element ermitteln
		nextEl =  dynamic_cast<Element*>(elemente.getNextObject(iter));


		//Prüfen ob Länge eingehalten wird
		if (element->getLaengeZuschnitt() > maxTeilrohrLaenge)
		{
			//Fehler im Modell
			AfxMessageBox(getStammdaten().getText("IDS_MODELL_FEHLER").c_str(), MB_OK | MB_ICONERROR); //Interner Fehler: Fehler im Rohrmodell
			return false;
		}

		//Prüfen ob verheriges Rohr für die Verteilerrohranlage bestimmt war
		if (isPrevTeilrohrVertAnlage)
		{
			//Wenn ja, dann Rohr nicht über Anlage fertigen, da beim Brennen sich das Rohr verzieht und das nachträglich Ausrichten
			//größeren Aufwand bedeuten würde.
			rohrDaten.typ = TypVerteilerhandschweissung;
		}

		//Abgänge holen
		EnCObListModell &unterElemente = element->get_uelementList();
		
		//Wenn Rohr Anlagenteile hat dieses Rohrstück aber nicht, wird es auch nicht über die Anlage gefertigt. (Wegen dem Ausrichten)
		if (hasAnlagenTeile && !unterElemente.hasAnlagenTeile())
		{
			rohrDaten.typ = TypVerteilerhandschweissung;
		}



		//Werte zuweisen
		rohrDaten.teilRohrCounter = teilRohrCounter;	//Teilrohrcounter 
		rohrDaten.laenge = element->getLaengeZuschnitt();		//(Teil)rohrlänge 
		rohrDaten.dn = element->get_dn();				//Dn Wert

		//Beschriftung (letzen 4Stellen Wanr + PosNr + Teilrohrcounter
		if (teilRohrCounter == 1)
			rohrDaten.beschriftung = MxString("{0}-{1}").arg(MxString("{0}").arg(rohrDaten.wanr).right(4))
														.arg(rohrDaten.posNr);
		else
			rohrDaten.beschriftung = MxString("{0}").arg(rohrDaten.posNr);
		


		//prüfen ob es evt. Verteiler Anlage ist
		if (rohrDaten.typ != (int)GpAnlageSaegeNut::TypHandelslaenge && unterElemente.hasAnlagenTeile() && rohrDaten.dn <= maxDnVerteiler)
			rohrDaten.typ = (int)GpAnlageSaegeNut::TypVerteilerAnlage;
		else
			rohrDaten.typ = (int)GpAnlageSaegeNut::TypVerteilerhandschweissung;

		//Ablageplatz ermitteln
		if (rohrDaten.typ == (int)GpAnlageSaegeNut::TypHandelslaenge)
			rohrDaten.ablageplatz = GpAnlageSaegeNut::AblageHandelslaenge;
		else if (rohrDaten.typ == (int)GpAnlageSaegeNut::TypVerteilerAnlage)
			rohrDaten.ablageplatz = GpAnlageSaegeNut::AblageVerteilerAnlage;
		else if (rohrDaten.laenge <= maxLaengeAblage4)
			rohrDaten.ablageplatz = GpAnlageSaegeNut::AblageVerteilerHand4;
		else
			rohrDaten.ablageplatz = GpAnlageSaegeNut::AblageVerteilerHand3;


		//NutAnfang
		if (prevEl && prevEl->get_id() == NUT)
			rohrDaten.nutAnfang = true;
		else
			rohrDaten.nutAnfang = false;

		//NutEnde
		if (nextEl && nextEl->get_id() == NUT)
			rohrDaten.nutEnde = true; //wenn nächstes Element Nut ist und kein weiteres Teilrohrstück folgt
		else
			rohrDaten.nutEnde = false;
	

		//prüfen ob Rohr für die Verteilerrohrstrasse gedreht werden muss
		bool drehen = false;
		if (rohrDaten.typ == (int)GpAnlageSaegeNut::TypVerteilerAnlage)
		{
			int result = element->checkDrehen(0, minAbstAnfangVerteiler, minAbstEndeVerteiler);
			if (result == 1)
				drehen = true;
			else if (result == 2)
			{
				CString str;
				str = getStammdaten().getText("IDS_MIN_ABST_VERT").c_str(); //Die Mindestabstände für die Verteilerrohrstraße werden nicht eingehalten
				MxString meldung = MxString(str).arg(MxString(werkauftrag->get_posNr()));
				AfxMessageBox(meldung.getString(), MB_OK | MB_ICONERROR);
				return false;
			}
		}
		else /*if (teilRohrCounter > 1) */
		{
			//Das Zweite Teilrohr drehen, wenn am Ende eine Nut ist.
			if (rohrDaten.nutEnde == true && rohrDaten.nutAnfang == false)
				drehen = true;
		}

		if (drehen)
			changeBoolValue(rohrDaten.nutAnfang, rohrDaten.nutEnde);

		//Datensätze mit Löcher werden immer erstellt, auch wenn Typ Verteilerrohrstrasse
		//Löcher erst erzeugen, damit die Anzahl der Löcher feststeht
		rohrDaten.anzLoecher = 0;
		if(unterElemente.getSize() > 0)
		{
			//Werte für Löcher in Db schreiben
			if (!createLoecher(absoluteLaenge, drehen, rohrDaten, vertRohrDaten, unterElemente, rohrDaten.anzLoecher))
				return false;
		}

		//Rohrdaten in der Liste Speichern
		saegeNutRohrListe.push_back(rohrDaten);
		if(teilRohrCounter == 1)
		{
			saegeNutRohrListeLegacy.push_back(rohrDaten);
		}
		//Wenn Teilrohr 1 ein Rohr für die Verteilerrohrstrasse war und die weiteren Rohr keine Abgänge für die Verteilerrohranlage enthalten, 
		//werden keine weiteren Rohrstücke übergeben, darum hier Merker setzen, damit weiter oben die Schleife verlassen werden kann
		if (rohrDaten.typ == (int)GpAnlageSaegeNut::TypVerteilerAnlage)
			isPrevTeilrohrVertAnlage = true;

		teilRohrCounter++;	
		absoluteLaenge += element->getLaengeZuschnitt();
		prevEl = element;
	}

	return true;
}

/**
prüfen ob r1 > r2
Sortierung nach Längenberechen in Schritten 1500, danach nach Dn und Länge
*/
bool GpAnlageSaegeNut::greater(DbAnlage::saegeNutRohre r1, DbAnlage::saegeNutRohre r2)
{
	int laengenBereich = 1500;
	
	//Bereich festlegen
	int b1 = 0;
	int b2 = 0;
	int bereich = 0;
	while (bereich < r1.laenge || bereich < r2.laenge)
	{
		if (r1.laenge > bereich)
			b1++;

		if (r2.laenge > bereich)
			b2++;
		
		bereich += laengenBereich;
	}

	//Größenprüfung
	if (b1 > b2)
		return true;

	if (b1 == b2 && 
		r1.dn > r2.dn)
		return true;

	if (b1 == b2 &&
		r1.dn == r2.dn &&
		r1.laenge > r2.laenge)
		return true;
	
	return false;
}


/**
Rohrlisten und Kopfsatz in die Datenbank schreiben. Sortierung dabei setzen
*/
bool GpAnlageSaegeNut::writeDatenInDb()
{
	//Transaktion
	dbAnlage.beginTransaction();

	//alle Datensätze zur Wanr aus den Tabellen löschen
	cleanTables(kopfData.wanr);


	//Kopfsatz schreiben
	if (!dbAnlage.updateKopfsatz(DbBase::save, kopfData))
	{
		dbAnlage.rollbackTransaction();
		return false;
	}

	//Rohre Sägenutanlage
	list<DbAnlage::saegeNutRohre>::iterator iterSnR = saegeNutRohrListe.begin();
	int counter = 0;
	int rohrCounter = 1;
	while (iterSnR != saegeNutRohrListe.end())
	{
		//Sortierung setzen
		iterSnR->sortierung = counter;

		if (!dbAnlage.updateFertSaegeNutRohre(DbBase::save, *iterSnR, rohrCounter))
		{
			dbAnlage.rollbackTransaction();
			return false;
		}

		counter++;
		iterSnR++;
	}

	//Löcher Sägenutanlage
	list<DbAnlage::saegeNutLoecher>::iterator iterSnL = saegeNutLoecherListe.begin();
	while (iterSnL != saegeNutLoecherListe.end())
	{
		if (!dbAnlage.updateFertSaegeNutLoecher(DbBase::save, *iterSnL))
		{
			dbAnlage.rollbackTransaction();
			return false;
		}
		iterSnL++;
	}

	//Rohre VerteilerrohrStrasse
	list<DbAnlage::verteilerRohre>::iterator iterVr = verteilerRohrListe.begin();
	while (iterVr != verteilerRohrListe.end())
	{
		if (!dbAnlage.updateFertVerteilerRohre(DbBase::save, *iterVr))
		{
			dbAnlage.rollbackTransaction();
			return false;
		}
		iterVr++;
	}

	//Rohrstutzen VerteilerrohrStrasse
	list<DbAnlage::verteilerNutAbgang>::iterator iterVn = verteilerNutAbgangListe.begin();
	while (iterVn != verteilerNutAbgangListe.end())
	{
		if (!dbAnlage.updateFertVerteilerNutAbgang(DbBase::save, *iterVn))
		{
			dbAnlage.rollbackTransaction();
			return false;
		}
		iterVn++;
	}


	dbAnlage.commitTransaction();

	return true;
}


/**
Datensätze für die Löcher erzeugen
absAbstandRohr: Absolutabstand des Rohrstückes zum absoluten Nullpunkt. Notwendig da sich die Absolutabstände 
der Elemente auf diesen Nullpunkt beziehen.
*/
bool GpAnlageSaegeNut::createLoecher(int absAbstandRohr, bool drehen, 
									DbAnlage::saegeNutRohre rohrDaten, DbAnlage::verteilerRohre vertRohrDaten, 
									EnCObListModell &unterelemente,
									int &lochCounter)
{

	int toleranzGeo = 1;  //1 mm Toleranz

	MxString strNut = "NUT";
    MxString strGlatt = "GLATT";

	MxString muffe = "MUFFE";
	MxString stutzen = "NUTST";

	//Toleranz berücksichtigen
	int minAbstRohrAnfangEnde = minAbstRohrAnfangEndeSaegeNut - toleranzGeo;

	DNzuAussendurchmesser dntoad;
	DbAnlage::saegeNutLoecher  lochDaten;
	DbAnlage::verteilerNutAbgang vertNutAbgang;

	list<DbAnlage::saegeNutLoecher>	tmpSaegeNutLoecherListe;

	int anzAnlagenteile = unterelemente.getAnzahlAnlagenteile();

	if (anzAnlagenteile > 0)
	{
		//Allgemeine Daten Verteilerrohrstr
		vertNutAbgang.waid = vertRohrDaten.waid;
		vertNutAbgang.posNr = vertRohrDaten.posNr;
		vertNutAbgang.uPosNr = 1; //zur Zeit ohne Verwendung
		vertNutAbgang.anzRohre = vertRohrDaten.anzahl;
		vertNutAbgang.dn = rohrDaten.dn;
		vertNutAbgang.din = vertRohrDaten.din;
		vertNutAbgang.laenge = rohrDaten.laenge;
		vertNutAbgang.teilRohrCounter = rohrDaten.teilRohrCounter;

		if (rohrDaten.nutAnfang)
			vertNutAbgang.anfang = strNut;
		else
			vertNutAbgang.anfang = strGlatt;

		if (rohrDaten.nutEnde)
			vertNutAbgang.ende = strNut;
		else
			vertNutAbgang.ende = strGlatt;

		vertNutAbgang.anzSoll  = vertRohrDaten.anzahl;
		vertNutAbgang.anzRohre = vertRohrDaten.anzahl;

		vertNutAbgang.anzElemente = anzAnlagenteile;
	}


	//Allgemeine Daten Sägenut
	lochDaten.waid = rohrDaten.waid;
	lochDaten.teilRohrCounter = rohrDaten.teilRohrCounter;

	//Elementliste durchlaufen
	MxCObListIterator iter(unterelemente);
	int counterLochSaegeNut = 1;
	int counterLochVerteiler = 0;
	if (drehen)
		counterLochVerteiler = anzAnlagenteile + 1;

	int winkel;
	while (iter.next())
	{	
		Element *element = dynamic_cast<Element*>(unterelemente.getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		//prüfen ob Loch auf Anlage geht
		if (((element->get_id() == MUFFE || element->get_id() == SONDERMUFFE || element->get_id() == STDMUFFE) && (element->get_dn() < minDnMuffe || element->get_dn() > maxDnMuffe)) ||
			((element->get_id() == NUTST || element->get_id() == STDSTUTZEN || element->get_id() == GWRST) && (element->get_dn() < minDnStutzen || element->get_dn() > maxDnStutzen)) ||
			(element->get_id() == ROHR &&  (element->get_dn() < minDnStutzen || element->get_dn() > rohrDaten.dn)))
			continue;

		//Abstand auf Rohranfang umrechnen
		int abst = element->get_abstAbs() - absAbstandRohr;	
		winkel = element->get_winkel();

		if (drehen)
		{
			abst = rohrDaten.laenge - abst;
			if (winkel > 0)
				winkel = 360 - winkel;
		}
		XASSERT(!(abst <= 0));
		if (abst <= 0)
			return false;

		//Radius des Lochs ermitteln
		double radius = 0;
		if (!isMinAbstandVonMitteLoch)
		{
			if (element->get_id() == BOHR50)
				radius = element->get_durchmesser() / 2.0;
			else
				radius = dntoad.getAussendurchmesser(element->get_dn()) / 2.0;
		}

		//Abstände zum Anfang und Ende berechnen
		double abstAnf = abst - radius;
		double abstEnde = rohrDaten.laenge - abst - radius;

		//prüfen ob Mindestabstände eingehalten werden
		bool isSaegenutLoch = true;
		if (abstAnf < minAbstRohrAnfangEnde || abstEnde < minAbstRohrAnfangEnde)
			isSaegenutLoch = false;

		//jetzt noch prüfen ob es über die Verteilerrohrstrasse geht. Hier beziehen sich die Abstände auch die Achse des Abgangs
		abstAnf = abst;
		abstEnde = rohrDaten.laenge - abst;
		bool isVerteilerLoch = true;
		if (abstAnf < minAbstAnfangVerteiler || abstEnde < minAbstEndeVerteiler || element->get_id() == BOHR50)
			isVerteilerLoch = false;

		if (!isSaegenutLoch && !isVerteilerLoch)
			continue;

		//Lochtyp
		if (element->get_id() == MUFFE || element->get_id() == SONDERMUFFE || element->get_id() == STDMUFFE)
			lochDaten.lochTyp = (int)GpAnlageSaegeNut::Muffe;
		else if (element->get_id() == NUTST || element->get_id() == STDSTUTZEN || element->get_id() == GWRST)
			lochDaten.lochTyp = (int)GpAnlageSaegeNut::Nutstutzen;
		else if (element->get_id() == ROHR)
			lochDaten.lochTyp = (int)GpAnlageSaegeNut::Rohr;
		else if (element->get_id() == BOHR50)
			lochDaten.lochTyp = (int)GpAnlageSaegeNut::Bohrung;
		else
			continue;

		lochDaten.abstand = abst;
		lochDaten.lochCounter = counterLochSaegeNut;
		lochDaten.lochDn = element->get_dn();
		lochDaten.winkel = winkel;

		if (element->get_id() == BOHR50)
			lochDaten.durchmesser = element->get_durchmesser(); 
		else
			lochDaten.durchmesser = 0; 
		
		//Daten für Verteilerrohrstrasse
		if (element->get_makeAnlage() && element->get_dn() <= maxDnVerteiler && isVerteilerLoch)
		{
			if (element->get_id() == MUFFE || element->get_id() == STDMUFFE || element->get_id() == SONDERMUFFE)
				vertNutAbgang.elementId = muffe;
			else if (element->get_id() == NUTST || element->get_id() == ROHR || element->get_id() == STDSTUTZEN || element->get_id() == GWRST)
				vertNutAbgang.elementId = stutzen;
			else
			{
				XASSERT(false);
				return false;
			}

			if (drehen)
				counterLochVerteiler--;
			else
				counterLochVerteiler++;
			
			vertNutAbgang.counter = counterLochVerteiler;
			vertNutAbgang.beschrLi = rohrDaten.beschriftung;
			vertNutAbgang.elementDn = element->get_dn();
			vertNutAbgang.winkel = element->get_winkel();
			vertNutAbgang.absAbst = abst;
			vertNutAbgang.elementLaenge = element->getLaengeZuschnitt() + dntoad.getRadiusToInt(rohrDaten.dn);

			//Abgang in Liste schreiben
			verteilerNutAbgangListe.push_back(vertNutAbgang);
		}

		if (isSaegenutLoch)
		{
			//Löcher in die Liste schreiben
			tmpSaegeNutLoecherListe.push_back(lochDaten);
			
			//Anzahl Löcher merken
			lochCounter = counterLochSaegeNut;

			counterLochSaegeNut++;
		}
	}

	if (drehen)
	{
		counterLochVerteiler += anzAnlagenteile - 1; //Für Sicherheitsabfrage wieder hochsetzen

		//Nummerrierung bei Sägenutanlage invertieren, da jetzt erst die Gesamtanzahl feststeht
		list<DbAnlage::saegeNutLoecher>::iterator iterSnL = tmpSaegeNutLoecherListe.begin();
		while (iterSnL != tmpSaegeNutLoecherListe.end())
		{
			iterSnL->lochCounter = counterLochSaegeNut - iterSnL->lochCounter; 

			iterSnL++;
		}
	}

	//Liste in hauptliste schreiben
	list<DbAnlage::saegeNutLoecher>::iterator iterSnL = tmpSaegeNutLoecherListe.begin();
	while (iterSnL != tmpSaegeNutLoecherListe.end())
	{
		saegeNutLoecherListe.push_back(*iterSnL);
		iterSnL++;
	}

	//Sicherheitsüberprüfung, sollte nie unterschiedlich sein, wenn doch sind evt. Plausis beim Verteiler für Anlage falsch eingestellt!
	if (counterLochVerteiler != anzAnlagenteile)
	{
		CString str;
		if (anzAnlagenteile - counterLochVerteiler > 1)
			str = getStammdaten().getText("IDS_ANLAGENTEILE_UNGLEICH2").c_str(); //Bei Position {0} können {1} Löcher, die als Anlage markiert sind, aufgrund der Mindestabstände nicht auf der Sägenutanlage gefertigt werden! 
		else
			str = getStammdaten().getText("IDS_ANLAGENTEILE_UNGLEICH1").c_str(); //Bei Position {0} kann {1} Loch, dass als Anlage markiert ist, aufgrund der Mindestabstände nicht auf der Sägenutanlage gefertigt werden! 
		MxString meldung = MxString(str).arg(vertNutAbgang.posNr).arg(anzAnlagenteile - counterLochVerteiler);
		AfxMessageBox(meldung.getString(), MB_OK | MB_ICONERROR);
		
		XASSERT(!(counterLochVerteiler != anzAnlagenteile));
		return false;
	}

	return true;
}



bool GpAnlageSaegeNut::createKopfsatz(Werkauftrag *werkauftrag)
{
	//Kopfsatz schreiben
	kopfData.wanr = werkauftrag->get_status().get_wanr();
	kopfData.sdAuftrag = werkauftrag->get_leitkarte().get_sdAuftragsnr();
	kopfData.sdPos = werkauftrag->get_leitkarte().get_sdPositionsnr();
	kopfData.sdMaterialNr = werkauftrag->get_leitkarte().get_sdMaterialnummer();
	kopfData.status = (int)GpAnlage::FreigabeSegmentRohr;
	kopfData.ort = werkauftrag->get_leitkarte().get_prj_bauOrt();
	kopfData.anlagentyp = (int)GpAnlageSaegeNut::AnlageSaegeNut;
	kopfData.kcAuftrag = werkauftrag->get_leitkarte().get_kcAuftragsnr();
	kopfData.kcPos = werkauftrag->get_leitkarte().get_kcPositionsnr();
	kopfData.fertDate = werkauftrag->get_status().get_mbtermin();

	return true;
}

/**
Status püfen

FreigabeSegmentRohr = 0,

InBearbeitungSaegeNut = 10,
UnterbrochenSaegeNut = 12,
FertigSaegeNut = 16,
StorniertSaegeNut = 18,

Status >= 20 betrifft andere Maschinen
*/
bool GpAnlageSaegeNut::checkStatus(int wanr)
{
	DbAnlage::kopfsatz kopfData;
	kopfData.wanr = wanr;

	if (!dbAnlage.updateKopfsatz(DbBase::load,kopfData))
		return true;  //Datensatz nicht vorhanden. Alles Ok

	//Status prüfen
	if (kopfData.status >= (int)GpAnlage::InBearbeitungSaegeNut && kopfData.status <= (int)GpAnlage::StorniertSaegeNut)
		return false;

	return true;
}


bool GpAnlageSaegeNut::createVerteilerRohre(Rohrleitung *rohr, DbAnlage::verteilerRohre &vertRohrDaten)
{
	//Allgemeine Daten Verteilerrohrstrasse
	vertRohrDaten.waid = rohr->get_waid();
	vertRohrDaten.wanr = rohr->get_status().get_wanr();
	vertRohrDaten.posNr = rohr->get_posNr();
	vertRohrDaten.anzahl = rohr->get_anzahl();
	vertRohrDaten.laenge = rohr->get_laenge();  //Gesamtrohrlänge
	vertRohrDaten.dn = rohr->get_dnvl();
	vertRohrDaten.din = rohr->getDin();
	vertRohrDaten.kFarbe = rohr->getKappenfarbe(rohr->get_rohrKFarbe());
	vertRohrDaten.anzUpos = 1; //wird zur Zeit nicht verwendet
	
	//Verteilerrohrdaten in Liste schreiben
	verteilerRohrListe.push_back(vertRohrDaten);
	
	return true;
}


void GpAnlageSaegeNut::changeBoolValue(bool &wert1, bool &wert2)
{
	bool tmp = wert1;
	wert1 = wert2;
	wert2 = tmp;
}



bool GpAnlageSaegeNut::readAuftraege(GpAnlage::Status status, list<DbAnlage::kopfsatz> &data, MXDate date)
{	
	return dbAnlage.readAuftraege((int)status, data, date);
}