#include "stdafx.h"


#include <mxodbc\mxodbc.h>
#include "resource.h"

#include "sap.h"
#include "..\waxprj\drucklistenverwaltung.h"
#include "..\waxprj\DrucklisteWap.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int LEN_AUFTRAGSNR  =  12;
const int LEN_PROJEKTNR   =   8;
const int LEN_PSP_ELEMENT =  20;
const int LEN_SD_AUFTRNR  =  10;
const int LEN_SD_POSINR   =   6;
const int LEN_LIEFDAT     =   8;
const int LEN_SD_MATNR    =  18;
const int LEN_SACHBEARB   =  12;
const int LEN_BEZEICHNUNG =  20;
const int LEN_DEBITORENNR =  10;
const int LEN_AUFTRAGSBEZ =  35;
const int LEN_KEY         =   3;
const int LEN_PLZ         =  10;
const int LEN_ORT         =  35;
const int LEN_NL          =  12;
const int LEN_BAU         =  12;
const int LEN_AUSSTELLER  =  12;
const int LEN_DATUM       =   8;
const int LEN_KC_AUFTRNR  =  10;
const int LEN_KC_POSINR   =   6;
const int LEN_IS_NEXT_GENERATION = 1;
const int LEN_IPPE_MATNR		 = 18;
const int LEN_IPPE_KNOTEN_NR	 = 40;


// Standardkonstruktor
Sap::Sap()
{
	createOK = false;
}

// Destruktor
Sap::~Sap()
{
	CObList *listen[] = { &rohrtemplist, &rohrlist, &rohrstutlist, NULL };
	for (int i = 0; listen[i]; i++)
	{
		CObList *list = listen[i];
		for (POSITION pos = list->GetHeadPosition(); pos;)
		{
			delete list->GetNext(pos);
		}
	}
}

// Liste mit Ausgaben-IDs (ausgaben) und Ausgabennamen initialisieren
void Sap::initAusgaben()
{
	//"ausgaben" enthaelt die fuer den jeweiligen Werkauftrag gueltigen Ausgaben-IDs. 
	Ausgaben::initAusgaben();
	ausgaben.RemoveAll();

	if (WaxSys::isAV()) 
		ausgaben.Add(DELETEWAXFROMSAP);
	
	// kein Objekt in walist 
	if (walist.IsEmpty())
		return;

	if (WaxSys::isAV()) 
		ausgaben.Add(SAPTOWAX_ALL);

	// Unterobjekte durchgehen bis ein Werkauftrag kein Sammelauftrag ist,
	// sonst werden die Ausgaben fuer Rohrleitung nicht angezeigt.
	for (POSITION pos = walist.GetHeadPosition(); pos != NULL;)
	{
		werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetNext(pos));
		// der Werkauftrag kann ein Sammelauftrag sein. 
		if (!werkauftrag->isEmpty() && !dynamic_cast<SammelWerkauftrag*>(werkauftrag)) 
			break;
	}

	// erstes Objekt in walist kein Werkauftrag
	if (!get_werkauftrag())
		return;
	// nur Gliederungspunkte und leere Sammelauftraege in walist
	if (werkauftrag->isEmpty() || dynamic_cast<SammelWerkauftrag*>(werkauftrag))
		return;

	if (WaxSys::isAV()) 
		ausgaben.Add(WAXTOSAP);
}

/*
@mfunc Liefert den Namen (für Menueeintrag) fuer eine Aktion. 
@todo Mehrsprachlichkeit einbauen fuer alle Aktionen, die in den NL benutzt werden
*/
CString Sap::getName()
{
	if (ausgaben.GetSize() == 0)
		initAusgaben();
	CString str;
	switch (currentAusgabe) 
	{
	case WAXTOSAP:
		str = getStammdaten().getText("IDS_SAP_MENU1").c_str();
		return str;
	
	case DELETEWAXFROMSAP:
		str = getStammdaten().getText("IDS_SAP_MENU2").c_str();
		return str;

	case SAPTOWAX_FILE:
		str = getStammdaten().getText("IDS_SAP_MENU3").c_str();
		return str;
	
	case SAPTOWAX_ALL:
		str = getStammdaten().getText("IDS_SAP_MENU4").c_str();
		return str;
	}
	
	XASSERT(FALSE);
	str = getStammdaten().getText("IDS_UNBEKANNTE_AKTION").c_str();
	return str;
}

// Gewuenschte Ausgabe ausfuehren
const CStringArray& Sap::makeAusgabe(int id)
{
	/*
	ID gibt die auszufuehrende Ausgabe an. Default ist id = -1, was zur Folge hat, dass der Wert
	von currentAusgabe benutzt wird. Ist currentAusgabe nicht gesetzt, wird die erste in ausgaben definierte 
	Ausgabe als Default gesetzt und ausgefuehrt.  
	*/
	Ausgaben::makeAusgabe(id);

	// keine Objekte in walist
	if (walist.IsEmpty())
		return htmlFiles;

	CString fn;

	// Erstmal Listen loeschen	
	resetAttributes();
	deleteListen();
	
	anzahl.RemoveAll();	
	dnvl.RemoveAll();
	posNr.RemoveAll();
	laenge.RemoveAll();
	hinweis.RemoveAll();
	gewicht.RemoveAll();
	
	if (!get_werkauftrag())
		return htmlFiles;

	setMaxLaengeUndGewicht();

	int ausgabeid = id == -1 ? currentAusgabe : id;

	// Keine Auswahl getroffen: In der Sapvorschau wurde eine andere Position ausgewählt. 
	if (currentAusgabe < 0) 
	{ // Automatisch default initialisieren
		initAusgaben();
		XASSERT(ausgaben.GetSize());
		if (ausgaben.GetSize())
			ausgabeid = currentAusgabe = ausgaben.GetAt(1);
	}
	currentAusgabe = ausgabeid;

	// Leitkarten Info von SAP einlesen gilt eh fuer das gesamte Projekt. 
	// Da braucht man die Unterobjekte nicht durchgehen.
	if (ausgabeid == SAPTOWAX_FILE || ausgabeid == SAPTOWAX_ALL) 
	{
		switch (ausgabeid) 
		{
		case SAPTOWAX_FILE:
			viewSapfile("");
		case SAPTOWAX_ALL:
			readSapfile("");
		}
		// Alle Werkauftraege mit den Daten Setzen
		Werkauftrag* sa = get_werkauftrag();
		CObject *tmp = walist.GetHead();
		walist.RemoveHead();
		walist.AddTail(tmp);
		for (int i = 1; i < walist.GetCount(); i++)
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			get_werkauftrag()->get_leitkarte().copy(sa->get_leitkarte());
			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
		}
		return htmlFiles;
	}
	else if (ausgabeid == WAXTOSAP) 
	{
		// Der 1. Durchlauf ist fuer den Preis
		// In den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
		// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
		int i;
		for (i = 0; i < walist.GetCount(); i++)
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if ((get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(get_werkauftrag())) && (walist.GetCount() > 1))
			{
				CObject *tmp = walist.GetHead();
				walist.RemoveHead();
				walist.AddTail(tmp);
				continue;
			}
			
			// Eine od. mehrere Positionen nicht freigegeben, das SAP Flag kann nicht gesetzt werden.
			if (get_werkauftrag()->get_status().get_wanr() == 0)
			{
				CString err;
				err = getStammdaten().getText("IDS_KEINE_WANR_KEIN_SAP").c_str();
				AfxMessageBox(err, MB_OK | MB_ICONERROR);
				((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
				return htmlFiles;
			}
			
			///VK3-Flag setzen!!!
			wa.get_preis().set_vk3flag(get_werkauftrag()->get_preis().get_vk3flag());
			
			createSapdata();

			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
		}

		// Liefer- und Freigabedatum und Beschichtung, Bonus und vk3-Status kopieren 
		wa.get_beschichtung().copy(get_werkauftrag()->get_beschichtung());
		wa.get_leitkarte().set_wunschliefertermin(get_werkauftrag()->get_leitkarte().get_wunschliefertermin());
		wa.get_status().set_freigabedatum(get_werkauftrag()->get_status().get_freigabedatum());
		wa.get_preis().copy(get_werkauftrag()->get_preis());		

		// Matlist & Stklist in Gesamtlist 
		setGesamtmaterialliste();		

		// Matlist sortieren
		wa.get_material().sortMaterial();

		///VK3-Flag abfragenn!!!
		if (wa.get_preis().get_vk3flag())
		{
			// Bei VK3 wird Mindermenge aufgeschlagen als Soko
			setVK3Gesamtpreis();
			// Afosondersachen
			afoSonderbehandlung();
		}
		else
		{
			// Bei Wap wird Mindermenge die erhöhte Afozeit direkt berechnet 
			// Afosondersachen
			afoSonderbehandlung();
			setGesamtpreise();
		}
		
		// Attribute wieder loeschen
		deleteListen();
		anzahl.RemoveAll();	
		dnvl.RemoveAll();
		posNr.RemoveAll();
		laenge.RemoveAll();
		hinweis.RemoveAll();
		
		// Der 2. Durchlauf ist fuer die Materialbestellung
		// In den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
		// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
		for ( i = 0; i < walist.GetCount(); i++)
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if ((get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(get_werkauftrag())) && (walist.GetCount() > 1))
			{
				CObject *tmp = walist.GetHead();
				walist.RemoveHead();
				walist.AddTail(tmp);
				continue;
			}
			
			createSapdata();
			createRohrdata();
			createRohrstutzendata();

			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
		}

		werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetTail());
		Strangrohr* strang = dynamic_cast<Strangrohr*>(werkauftrag);
		// nur bei Strangrohr
		if (strang)
		{
			// Ausschuss fuer Muffen addieren (nur fuer Materialentnahme, kein VK3) 
			Element ele;
			strang->addMuffenausschuss(wa.get_stueckliste());
			strangrohr = true;
		}
		
		// Rohrdaten ermitteln, hier werden die benötigten 6m-Laengen berechnet,
		// jetzt wird aber erstmal der Wert aus der Stueckliste zu SAP uebertragen
		//if (rohr && !grohr)
			//setRohrdata();

		// Matlist & Stklist in Gesamtlist 
		setGesamtmaterialliste();		

		// Matlist sortieren
		wa.get_material().sortMaterial();

		// Afosondersachen
		afoSonderbehandlung();
			
		// Hinweis ausgeben
		check_NullEuro();
		isMatNull();

		// Ausgabe
		double preis = 0;
		if (createSapfile("", preis))
		{
			createOK = true;
			updateSapTabelle(preis);
		}
	}
	else if (ausgabeid == DELETEWAXFROMSAP)
	{
		deleteWaxFromSap();
	}

	return htmlFiles;
}

int Sap::createSapdata()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());

	// Materialliste
	set_matlist(get_werkauftrag()->get_material().get_matlist(), WaxDruck::DrucklistenVerwaltung::MATERIALLISTE_GESAMT, false, get_werkauftrag()->get_einkauf());

	// bei Rohrleitung
	if (rohr)
	{
		// Stueckliste
		if (rohr->isVerteiler() && rohr->checkAnlage()) 
			set_stklist(rohr->get_stuecklisteSap(), false, get_werkauftrag()->get_einkauf());
		else
			set_stklist(rohr->get_stueckliste(), false, get_werkauftrag()->get_einkauf());

		
		// Rohrattribute fuer Ausgabe der Einzelpositionen merken
		posNr.Add(rohr->get_posNr());
		anzahl.Add(rohr->get_anzahl());
		dnvl.Add(rohr->get_dnvl());
		laenge.Add(rohr->get_laenge());
		gewicht.Add(rohr->getGesGewicht(0));

		// Hinweistext: z.B. Bogen im Rohr  
		CString str;
		
		if (rohr->isBogen())
		{
			str = getStammdaten().getText("IDS_HINWEIS4b").c_str();
			hinweis.Add(str);
		}
		else
		{
			str = getStammdaten().getText("IDS_HINWEIS5b").c_str();
			hinweis.Add(str);
			
		}
				
		// VK3 - Elemente
		set_elelist(rohr->get_vk3elementeListe(),true);	
		set_passlist(rohr->get_passlaengenListe());
		set_pulverlist(rohr->get_pulverlaengenListe(),true);
	}
	
	// Afoliste
	get_wa().get_arbeit().merge(get_werkauftrag()->get_arbeit());
	
	// Sonderkosten
	set_sokolist(get_werkauftrag()->get_preis().get_sokolist());

	return 1;
}

//Erfasst die Daten fuer die Rohre in der Stueckliste
void Sap::createRohrdata()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	if (!rohr)
		return;

	// Elementliste abarbeiten
	CObList &elementList = rohr->get_elementList();
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element* element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			continue;
		if (element->isEleRohr()) 
		{
			POSITION upos;
			// Rohrstueck schon in Gesamtliste, dann Anzahl addieren
			for (upos = rohrlist.GetHeadPosition(); upos;)
			{
				Rohrstuecke *rohrst = dynamic_cast<Rohrstuecke*>(rohrlist.GetNext(upos));
				XASSERT(rohrst);
				if (!rohrst)
					continue;
				if (rohrst->get_artikelnr() == element->get_artikelnr()
				 && rohrst->get_laenge()    == element->get_laenge())
				{
					rohrst->set_anz(rohrst->get_anz() + element->get_anzahl()*rohr->get_anzahl());
					upos--;
					break;
				}
			}
			// in Gesamtliste packen
			if (upos == NULL) 
			{
				Rohrstuecke *r = new Rohrstuecke;
				r->set_artikelnr(element->get_artikelnr());
				r->set_laenge(element->get_laenge());
				r->set_anz(element->get_anzahl() * rohr->get_anzahl()); 
				r->set_restlaenge(6000 - r->get_laenge()); 
				rohrlist.AddTail(r);
			}
		}
	}
}

//Erfasst die Daten fuer die Rohrstutzen in der Stueckliste
void Sap::createRohrstutzendata()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	if (!rohr)
		return;

	// erst gleiche Rohrstutzen zusammenfassen
	CObList &elementList = rohr->get_elementList();
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element* ele = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr()) 
		{
			CObList &uelementList = ele->get_uelementList();
			for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
			{
				Element* uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
				XASSERT(uele);
				if (!uele)
					break;

				if (uele->get_id() == ROHR) 
				{
					// Rohrstutzen schon in Gesamtliste, dann Laenge addieren
					POSITION p;
					for (p = rohrstutlist.GetHeadPosition(); p;)
					{
						Element *e = dynamic_cast<Element*>(rohrstutlist.GetNext(p));
						XASSERT(e);
						if (!e)
							break;

						if (e->get_artikelnr() == uele->get_artikelnr())
						{
							uele->set_laenge(uele->get_laenge()+e->get_laenge());
							p--;
							break;
						}
					}
					// in Gesamtliste packen
					if (p == NULL) 
					{
						Element *e = new Element;
						e->copy(*uele);
						rohrstutlist.AddTail(e);
					}
				}
			}
		}
	}				
}

//Erfasst die Daten fuer die Rohre in der Stueckliste
void Sap::setRohrdata()
{
	// zu Rohrstuecken passenden Rest suchen
	setRohrtemplist();

	// gleiche Rohrstuecke zusammenfassen
	setRohrendlist();

	// Rohre in Stueckliste neu setzen
	setRohreNeu();

	// Laengen fuer Rohrstutzen ermitteln
	setRohrstutzenlaenge();

	// Rohrstutzen in Stueckliste neu setzen
	setRohrstutzenNeu();
}

//zu Rohrstuecken passenden Rest suchen
void Sap::setRohrtemplist()
{
	int restmerk = 7000;
	int index = 0, uindex = 0, merkindex = 0;
	bool gefunden = false;

	// rohrlist tracen 
	TRACE("\nRohrliste: \n");
	POSITION pos;
	for (pos = rohrlist.GetHeadPosition(); pos;)
	{
		Rohrstuecke *rohr = dynamic_cast<Rohrstuecke*>(rohrlist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		TRACE("Nr: %d, Anz: %d, L: %d\n", rohr->get_artikelnr(), rohr->get_anz(), rohr->get_laenge());
	}

	// Liste mit Rohrstuecken durchgehen 
	for (pos = rohrlist.GetHeadPosition(); pos;)
	{
		Rohrstuecke *rohr = dynamic_cast<Rohrstuecke*>(rohrlist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		if (rohr->get_laenge() < 6000)
		{
			gefunden = false;
			uindex = 0;
			restmerk = 7000;

			// Liste nach passende Resten durchsuchen
			for (POSITION upos = rohrlist.GetHeadPosition(); upos;)
			{
				Rohrstuecke *r = dynamic_cast<Rohrstuecke*>(rohrlist.GetNext(upos));
				XASSERT(r);
				if (!r)
					continue;
				if (rohr->get_artikelnr() == r->get_artikelnr()
		  		 && rohr->get_laenge()    <= r->get_restlaenge())
				{
					gefunden = true;
					if (r->get_restlaenge() < restmerk)	
					{
						restmerk = r->get_restlaenge();
						merkindex = uindex;
					}
				}
				uindex++;
			}

			if (gefunden)
			{
				POSITION pos1 = rohrlist.FindIndex(index);
				POSITION pos2 = rohrlist.FindIndex(merkindex);
				Rohrstuecke *r = dynamic_cast<Rohrstuecke*>(rohrlist.GetAt(pos2));
				XASSERT(r);
				if (!r)
					continue;
				// Anzahl dekrementieren
				if (pos1 == pos2)
					r->set_anz(r->get_anz()-1);
				else
				{
					r->set_anz(r->get_anz()-1);
					rohr->set_anz(rohr->get_anz()-1); 
				}

				// neue volle Laenge in templist
				Rohrstuecke *newrohr = new Rohrstuecke;
				newrohr->set_anz(1);
				newrohr->set_laenge(6000);
				newrohr->set_artikelnr(rohr->get_artikelnr());
				rohrtemplist.AddTail(newrohr);

				// Wenn Anzahl auf 0, dann Rohrstueck loeschen
				if (rohr->get_anz() == 0)
				{
					rohrlist.RemoveAt(pos1);
					delete rohr;
					pos = rohrlist.GetHeadPosition();
					index = -1;
				}
				if (pos1 != pos2 && r->get_anz() == 0)
				{
					rohrlist.RemoveAt(pos2);
					delete r;
					pos = rohrlist.GetHeadPosition();
					index = -1;
				}
			}
			else
			{
				// Rohr auf volle Laenge hochsetzen und in templist schieben
				rohr->set_laenge(6000);
				rohrtemplist.AddTail(rohr);
				
				POSITION pos1 = rohrlist.FindIndex(index);
				rohrlist.RemoveAt(pos1);
				pos = rohrlist.GetHeadPosition();
				index = -1;
			}
		}
		else
		{
			rohrtemplist.AddTail(rohr);
			
			POSITION pos1 = rohrlist.FindIndex(index);
			rohrlist.RemoveAt(pos1);
			pos = rohrlist.GetHeadPosition();
			index = -1;
		}
		index++;
	}

	// die restlichen Rohrstuecke in templist schieben 
	for (pos = rohrlist.GetHeadPosition(); pos;)
	{
		Rohrstuecke *rohr = dynamic_cast<Rohrstuecke*>(rohrlist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		rohr->set_laenge(6000);
		rohrtemplist.AddTail(rohr);
	}
	rohrlist.RemoveAll();

	// templist tracen 
	TRACE("\nRohrtempliste: \n");
	for (pos = rohrtemplist.GetHeadPosition(); pos;)
	{
		Rohrstuecke *rohr = dynamic_cast<Rohrstuecke*>(rohrtemplist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		TRACE("Nr: %d, Anz: %d, L: %d\n", rohr->get_artikelnr(), rohr->get_anz(), rohr->get_laenge());
	}
}

//gleiche Rohrstuecke zusammenfassen
void Sap::setRohrendlist()
{
	POSITION pos;
	// gleiche Rohrstuecke aus templist in endlist zusammenfassen
	for (pos = rohrtemplist.GetHeadPosition(); pos != NULL;)	
	{
		Rohrstuecke* rohr = dynamic_cast<Rohrstuecke*>(rohrtemplist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		// Rohrstueck schon in Gesamtliste, dann Anzahl addieren
		POSITION upos;
		for (upos = rohrendlist.GetHeadPosition(); upos;)
		{
			Rohrstuecke *rohrst = dynamic_cast<Rohrstuecke*>(rohrendlist.GetNext(upos));
			XASSERT(rohrst);
			if (!rohrst)
				break;

			if (rohrst->get_artikelnr() == rohr->get_artikelnr())
			{
				rohrst->set_anz(rohrst->get_anz() + rohr->get_anz());
				upos--;
				break;
			}
		}
		// in Gesamtliste packen
		if (upos == NULL) 
		{
			rohrendlist.AddTail(rohr);
		}
	}
	rohrtemplist.RemoveAll();

	// endlist tracen 
	TRACE("\nRohrendliste: \n");
	for (pos = rohrendlist.GetHeadPosition(); pos;)
	{
		Rohrstuecke *rohr = dynamic_cast<Rohrstuecke*>(rohrendlist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		TRACE("Nr: %d, Anz: %d, L: %d\n", rohr->get_artikelnr(), rohr->get_anz(), rohr->get_laenge());
	}
}

//Rohre in Stueckliste neu setzen
void Sap::setRohreNeu()
{
	POSITION pos;
	// Rohrendliste durchgehen  
	for (pos = rohrendlist.GetHeadPosition(); pos;)
	{
		Rohrstuecke *rohr = dynamic_cast<Rohrstuecke*>(rohrendlist.GetNext(pos));
		XASSERT(rohr);
		if (!rohr)
			break;

		// Stueckliste vom Werkauftrag scannen
		for (POSITION upos = wa.get_stueckliste().GetHeadPosition(); upos;)
		{
			Element *ele = dynamic_cast<Element*>(wa.get_stueckliste().GetNext(upos));
			XASSERT(ele);
			if (!ele)
				break;

			// neue Laenge setzen
			if (ele->get_artikelnr() == rohr->get_artikelnr())
			{
				ele->set_laenge(rohr->get_laenge()*rohr->get_anz());
				//ele->set_anzahl(rohr->get_anz());
			}
		}
	}

	// Rohrendliste loeschen
	for (pos = rohrendlist.GetHeadPosition(); pos;)	
	{
		delete rohrendlist.GetNext(pos);
	} 
	rohrendlist.RemoveAll();
}

//Laengen fuer Rohrstutzen ermitteln
void Sap::setRohrstutzenlaenge()
{
	// die 1m-Laengen ermitteln
	for (POSITION pos = rohrstutlist.GetHeadPosition(); pos != NULL;)	
	{
		Element* ele = dynamic_cast<Element*>(rohrstutlist.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		int rest = ele->get_laenge() % 1000;					
		int nRohre = (int)(ele->get_laenge()/1000);			
		if (rest > 0) 
			nRohre += 1;	

		ele->set_laenge(6000*nRohre);
	}
}

//Rohrstutzen in Stueckliste neu setzen
void Sap::setRohrstutzenNeu()
{
	POSITION pos;
	// Rohrstutzenliste durchgehen  
	for (pos = rohrstutlist.GetHeadPosition(); pos;)
	{
		Element *e = dynamic_cast<Element*>(rohrstutlist.GetNext(pos));
		XASSERT(e);
		if (!e)
			break;

		// Stueckliste vom Werkauftrag scannen
		for (POSITION upos = wa.get_stueckliste().GetHeadPosition(); upos;)
		{
			Element *ele = dynamic_cast<Element*>(wa.get_stueckliste().GetNext(upos));
			XASSERT(ele);
			if (!ele)
				break;

			// neue Laenge setzen
			if (ele->get_artikelnr() == e->get_artikelnr())
				ele->set_laenge(ele->get_laenge() + e->get_laenge());
		}
	}

	// Rohrstutzenliste loeschen
	for (pos = rohrstutlist.GetHeadPosition(); pos;)	
	{
		delete rohrstutlist.GetNext(pos);
	} 
	rohrstutlist.RemoveAll();
}


double Sap::getPreis()
{
	double preis = 0;

	ApaxWerkauftrag *apax     = dynamic_cast<ApaxWerkauftrag*>(get_werkauftrag());
	FormstueckeFittinge* form = dynamic_cast<FormstueckeFittinge*>(get_werkauftrag());

	if (form)
	{
		if(get_werkauftrag()->get_leitkarte().get_vk3())
		{
			preis = getFormVK3Gesamtpreis();
		}
		if(preis == 0)
		{
			preis = 0.1;
		}
	}
	else
	{
		if (apax)
			preis = getApaxVK3Gesamtpreis();
		else
			preis = get_werkauftrag()->get_leitkarte().get_vk3() ? (get_werkauftrag()->get_preis().get_vk3flag() ? wa.get_preis().get_gesamtpreisVK3() : 0.1) : 0.1;
	}

	return preis; 
}


/**
Schreibt die Daten eines Auftrages fuer SAP in ein Textfile. 
*//*
Änderungen:
21.06.06, Jacobs: Die Materialeinheit aus SAP wird berücksichtigt. Werte werden in den Einheiten von Sap übergeben 
                  SupportNr.: 11570
*/
bool Sap::createSapfile(CString fname, double &preis)
{
	
#ifndef _DEBUG
	// In Bearbeitung
	if (get_werkauftrag()->get_status().get_sachbearbeiterSegment().IsEmpty()) 
	{
		CString str; str = getStammdaten().getText("IDS_HINWEIS1b").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return false;
	}
#endif

	int counter = 1, i = 0;
	CString str = "";
	CString neueZeile;
	neueZeile.Format("\r\n");
	
	CString lfdNr;
	lfdNr.Format("%07ld", counter);

	// Werkauftrag
	CString wan;

	preis = 0;
	bool isPreisanfrage = false;

	
	SammelWerkauftrag* sa = dynamic_cast<SammelWerkauftrag*>(get_werkauftrag());
	if (sa)
	{
		if (sa->get_leitkarte().get_preisanfrage().get_nummer() > 0)
		{
			isPreisanfrage = true;
			preis = sa->get_leitkarte().get_preisanfrage().get_preis();
		}

		CObject *tmp = walist.GetHead();
		walist.RemoveHead();
		walist.AddTail(tmp);
	}

	if (!isPreisanfrage)
	{
		
		Werkauftrag* werkauftrag =  get_werkauftrag();
		
		
		if(werkauftrag->get_GUID().IsEmpty() == FALSE)
		{
			HtmlDruck file; 
			
			WaxDruck::DrucklisteWap DrucklisteWap(file, &walist, GpWapPreis::PREISART::Vollkosten); 

			double gesMat = 0;
			double gesFert = 0;
			string ueberschrift;
			DrucklisteWap.addMaterialkosten(gesMat, true, ueberschrift);
			DrucklisteWap.addFertigungskosten(gesFert, true, ueberschrift);
			preis = gesFert + gesMat;
			if(werkauftrag->get_anzahl() > 0)
			{
				preis = preis/werkauftrag->get_anzahl();
			}
		}
		else
		{
			preis = getPreis();
		}
	}


	Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(get_werkauftrag());
	if (grohr)
		wan.Format("G%06ld", get_werkauftrag() ? get_werkauftrag()->get_status().get_wanr() : 0);
	else 
	{
		Werkauftrag* werkauftrag = dynamic_cast<Werkauftrag*>(get_werkauftrag());
		if(werkauftrag->get_GUID().IsEmpty() == TRUE)
		{
			wan.Format("M%06ld", get_werkauftrag() ? get_werkauftrag()->get_status().get_wanr() : 0);
		}
		else
		{
			wan.Format("B%06ld", get_werkauftrag() ? get_werkauftrag()->get_status().get_wanr() : 0);
		}
	}
	// Rohrleitung
	CString strpreis, strgewicht, strlaenge, strZulassung;
	
	//mds#500 Zulassungsnummer
	int zulNr= get_werkauftrag()->get_leitkarte().get_zulassungsnummer();


	strpreis.Format  (" %10.2f EUR", preis);
	strgewicht.Format(" %8.2f KG ",  (gesamtgewicht<0 || gesamtgewicht>10000.0)?0:gesamtgewicht);
	strlaenge.Format (" %5d MM ",	 (maxlaenge>12000)?0:maxlaenge);
	strZulassung.Format("    %1d", zulNr);    

	// Leitkarte
	CString erstellungsdatum, lieferdatum;
	erstellungsdatum.Format("%s", get_werkauftrag()->get_erstellungsdatum().Format("%d%m%y"));
	lieferdatum.Format("%s", get_werkauftrag()->get_leitkarte().get_wunschliefertermin().Format("%d%m%y"));
		
	CString sachbearbeiterSegment = "  ";
	str = get_werkauftrag()->get_status().get_sachbearbeiterSegment();
	if (str.GetLength() > 1) 
	{
		sachbearbeiterSegment.SetAt(0, str.GetAt(0)); 
		sachbearbeiterSegment.SetAt(1, str.GetAt(1)); 
	}
	
	CString sdAuf, sdPos, sdMat, strNextGeneration, strIppeMatNr, strIppeKnotenNr;
	sdAuf.Format("%010I64u", get_werkauftrag()->get_leitkarte().get_sdAuftragsnr()); 
	sdPos.Format("%04ld",  get_werkauftrag()->get_leitkarte().get_sdPositionsnr()); 
	sdMat.Format("%07ld           ", get_werkauftrag()->get_leitkarte().get_sdMaterialnummer());

	if(get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers())
	{

		strNextGeneration	=  "x";
		strIppeMatNr		=  get_werkauftrag()->get_leitkarte().get_ippeMatNr();	
		int iLength = strIppeMatNr.GetLength();
		iLength = LEN_IPPE_MATNR - iLength; 
		strIppeMatNr += CString(' ', iLength);
		strIppeKnotenNr		=  get_werkauftrag()->get_leitkarte().get_ippeKnotenNr();
		iLength = strIppeKnotenNr.GetLength();
		iLength = LEN_IPPE_KNOTEN_NR - iLength; 
		strIppeKnotenNr += CString(' ', iLength);
	}
	else
	{
		strNextGeneration	= " ";
		strIppeMatNr	  =  CString(' ', LEN_IPPE_MATNR);	
		strIppeKnotenNr	  =  CString(' ', LEN_IPPE_KNOTEN_NR);
	}




	////////// Text //////////
	CString txt   = "#";

	// Kopfzeile
	txt += wan;
	txt += "                  ";					// Filler 18,"          9999991 "
	if(werkauftrag->get_GUID().IsEmpty() == TRUE)
	{
		txt += "      1.000";							// Auftragsmenge, immer 1 -> ueberfluessig, 11-stellig
	}
	else
	{
		int amountOfPipes =werkauftrag->get_anzahl();
		char tmp[50];
		sprintf(tmp, "% 7d", amountOfPipes);
		txt += CString(tmp) + ".000";							// Wittenberge Auftragsmenge 11-stellig
	}

	txt += "                                                                                  "; // Filler 82, erstellungsdatum+lieferdatum+Kostentraeger-Nr.+sachbearbeiterSegment+"KEYNOSM"+lfdNr+bezeichnung
	txt += sdAuf;
	txt += sdPos;
	txt += sdMat;									// laut SAP 18-stellig, ist aber 7-stellig
	txt += strpreis;
	txt += strgewicht;
	txt += strlaenge;
	//txt += strNextGeneration;
	//txt += strIppeMatNr;
	//txt += strIppeKnotenNr;
	txt +=strZulassung;
	txt += neueZeile;

	CString	artikel, nr; 		

	// Materialiste ausgeben
	if (!wa.get_material().get_matlist().IsEmpty()) 
	{
		int count = 0;
		for (POSITION p = wa.get_material().get_matlist().GetHeadPosition(); p != NULL;)	
		{
			Material *material = dynamic_cast<Material*>(wa.get_material().get_matlist().GetNext(p));
			XASSERT(material);
			if (!material)
				return false;

			if (material->get_artikelnr() < 100000 || (material->get_id() != NOARTNR && material->get_artikelnr() == 0))
				continue;

			Element ele;
			ele.set_id(material->get_id());
			if (material->get_id() == ROHR || ele.isEleRohr())
			{
				if (material->get_mass1() <= 0)
					continue;
			}
			else if (material->get_anzahl() <= 0)
				continue;

			count++;
			nr.Format("%04ld", count*10);			// in 10er Schritten zählen


			//Materialeinheit aus SAP beachten
			CString einheit;
			
			if (material->get_id() == NOARTNR && material->get_artikelnr() == 0)
			{
				einheit = "ST";
				material->set_mass1(0);
			}
			else
			{
				einheit = getStammdaten().getMaterialeinheit(material->get_artikelnr());
				einheit = einheit.Trim();
			}

			double	faktor = 1;
			CString formatStr;

			if (!einheit.CompareNoCase("mm"))
			{
				faktor = 1000;
				formatStr = "%06d%10.0lf";
			}
			else
			{
				faktor = 1;
				formatStr = "%06d%10.3lf";
			}


			if (material->get_mass1() > 0)
			{
				double laenge;

				//Ab Version3.2 kein Runden mehr
				/*
				int rest = material->get_mass1() % 1000;	

				//auf ganze Meter runden				
				if (rest > 0)
					laenge = (double)(material->get_mass1()-rest)/1000+1;
				else
					laenge = (double)material->get_mass1()/1000;
				*/

				//in Meter umrechnen
				laenge =  (double)material->get_mass1()/1000;
				laenge *= faktor;

				artikel.Format(formatStr, material->get_artikelnr(), laenge); 
			}
			else
				artikel.Format(formatStr, material->get_artikelnr(), (double)material->get_anzahl()); 

			txt += "+";
			txt += "       ";						// Filler 7, lfdNr
			txt += wan;
			txt += "                     ";			// Filler 21, "          9999991 1  "
			txt += "         ";						// Filler 9
			txt += artikel;
			txt += "      ";						// Filler 6, WA-Datum
			txt += nr + einheit;
			txt += neueZeile;
		}
	}
	
	txt += format_arbeitsfolgen(wa.get_arbeit().get_arbeitsfolgen(), wan);

	// Verteiler Pos.
	for (i=0; i<posNr.GetSize(); i++)
	{
		CString vertPos;
		vertPos.Format(" Positionsnr. %8s Anzahl %4d DN %3d Länge %6d Gewicht %5.2f %40s", 
					   posNr.GetAt(i), anzahl.GetAt(i), dnvl.GetAt(i), laenge.GetAt(i), gewicht.GetAt(i), hinweis.GetAt(i));
		txt += "§";
		txt += wan;
		txt += vertPos;
		txt += neueZeile;
	}

	// File schreiben
	CString fn;
	if (fname.IsEmpty()) 
	{
		CString path(WaxSys::para_str(DIR_WAXTOSAP));
		fn = path;
	} 
	else 
	{
		fn = fname;
	}
	if (!writeFile(fn, txt, wan))
		return false;

	// File oeffnen
	str = getStammdaten().getText("IDS_ERROR4b").c_str();
	txt.Format(str, fn);

	return true;
}

bool Sap::getText(const CString &text, int offset, int &pos, CString &result, bool trim)
{
	result = text.Mid(pos, offset);

	if (trim)
	{
		result.TrimLeft();
		result.TrimRight();
	}

	pos += offset;

	return true;
}

void Sap::setLeitkarte(CString wa)
{
	CString text;
	int counter = LEN_AUFTRAGSNR + LEN_PROJEKTNR;

	CString test;

	//Psp Element, Nl, Produktlinie, PspPos
	getText(wa, LEN_PSP_ELEMENT, counter, text, false);

	//Wenn PspPos nicht gesetzt ist, werden Leerzeichnen übergeben. Diese in Nullen umwandeln
	//
	CString psp, pspPos;
	psp = text.Left(1) + text.Mid(2, 6);
	pspPos = text.Mid(16);
	pspPos.Replace(' ','0');
	psp += pspPos;
	
	get_werkauftrag()->get_leitkarte().set_prj_pspElement(psp); 
	get_werkauftrag()->get_leitkarte().set_prj_nl(atoi(text.Mid(9, 3)));
	if(get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers())
	{
		int iBrandschutztechnik = atoi(text.Mid(13, 2));
		get_werkauftrag()->get_leitkarte().set_Brandschutztechnik(iBrandschutztechnik);
	}
	else
	{
		get_werkauftrag()->get_leitkarte().set_prj_produktlinie(atoi(text.Mid(13, 2)));
	}
	
	// SD-Auftragsnr.
	getText(wa, LEN_SD_AUFTRNR, counter, text);
	get_werkauftrag()->get_leitkarte().set_sdAuftragsnr(_atoi64(text)); 
	
	// SD-Positionsnr.
	getText(wa, LEN_SD_POSINR, counter, text);
	get_werkauftrag()->get_leitkarte().set_sdPositionsnr(atoi(text)); 
	
	// Lieferdatum
	getText(wa, LEN_LIEFDAT, counter, text);
	MXDate liefdatum;
	liefdatum.SetDate(atoi(text.Left(4)), atoi(text.Mid(4, 2)), atoi(text.Mid(6, 2)));
	get_werkauftrag()->get_leitkarte().set_wunschliefertermin(liefdatum); 
	
	// SD-Materialnr.
	getText(wa, LEN_SD_MATNR, counter, text);
	get_werkauftrag()->get_leitkarte().set_sdMaterialnummer(atoi(text)); 
	
	// Sachbearbeiter NL -> Aussteller, s.u.
	counter += LEN_SACHBEARB;

	// Bezeichnung NL
	counter += LEN_BEZEICHNUNG;

	// Debitorennr.
	counter += LEN_DEBITORENNR;
	
	// Auftragsbezeichnung
	getText(wa, LEN_AUFTRAGSBEZ, counter, text);
	get_werkauftrag()->get_leitkarte().set_prj_auftragsbez(text); 
	
	// Laenderschluessel
	counter += LEN_KEY;
	
	// PLZ
	getText(wa, LEN_PLZ, counter, text);
	get_werkauftrag()->get_leitkarte().set_prj_bauPlz(text); 
	
	// Ort
	getText(wa, LEN_ORT, counter, text);
	get_werkauftrag()->get_leitkarte().set_prj_bauOrt(text); 
	
	// NL
	counter += LEN_NL;

	// Baustelle -> Auftragsbezeichnung, s.o.
	counter += LEN_BAU;
	
	// Aussteller
	getText(wa, LEN_AUSSTELLER, counter, text);
	get_werkauftrag()->get_leitkarte().set_sachbearbeiterNL(text); 

	//Datum
	counter += LEN_DATUM;
	
	//KC Auftrag
	getText(wa, LEN_KC_AUFTRNR, counter, text);
	get_werkauftrag()->get_leitkarte().set_kcAuftragsnr(text); 
	
	//KC Posnr
	getText(wa, LEN_KC_POSINR, counter, text);
	get_werkauftrag()->get_leitkarte().set_kcPositionsnr(atoi(text)); 


	getText(wa, LEN_IS_NEXT_GENERATION, counter, text);
	if(text.Find("X") > -1 || text.Find("x") > -1)
	{		
		get_werkauftrag()->get_leitkarte().set_IsChoosenIppeNumbers(true);
		get_werkauftrag()->get_leitkarte().set_sdAuftragsnr(0); 
		get_werkauftrag()->get_leitkarte().set_sdPositionsnr(0); 
	}
	else
	{
		get_werkauftrag()->get_leitkarte().set_IsChoosenIppeNumbers(false);
	}
	
	getText(wa, LEN_IPPE_MATNR, counter, text);
	getText(wa, LEN_IPPE_KNOTEN_NR, counter, text);
	get_werkauftrag()->get_leitkarte().set_ippeKnotenNr(text); 	

	if(get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers() == false)
	{
		get_werkauftrag()->get_leitkarte().set_ippeMatNr(CString()); 
		get_werkauftrag()->get_leitkarte().set_ippeKnotenNr(CString()); 	
	}
}


/*
@mfunc Liest die von SAP generierte Textdatei mit mit Leitkarteninfos von Aufträgen ein. 
*/
CString Sap::readSapfile(CString fname)
{
	SapfileOpen sapo;
	
	int index = sapo.openDialog(get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers(), get_werkauftrag()->get_leitkarte().get_ippeMatNr(), 
								get_werkauftrag()->get_leitkarte().get_sdAuftragsnr(), 
								get_werkauftrag()->get_leitkarte().get_sdPositionsnr(),
 								currentAusgabe);

	CString auftrag;

	if (sapo.get_sapauftraege().GetSize() > 0 && index >= 0) 
	{
		auftrag = sapo.get_sapauftraege().GetAt(index);
		setLeitkarte(auftrag);
	}
	return (fname);
}

CString Sap::viewSapfile(CString fname)
{
	static CString fn;
	if (fname.IsEmpty()) 
	{
		CString path(WaxSys::para_str(DIR_SAPTOWAX));
		fn = path;
	} 
	else 
	{
		fn = fname;
	}

	CString txt, str; str = getStammdaten().getText("IDS_ERROR4b").c_str();
	txt.Format(str, fn);
	
	if ((DWORD)ShellExecute(NULL, "open", fn, NULL, NULL, SW_SHOWNORMAL) < 31)
		AfxMessageBox(txt, MB_OK | MB_ICONINFORMATION);
	return (fn);
}

void Sap::deleteWaxFromSap()
{
	// File
	CString fn;
	CString path(WaxSys::para_str(DIR_WAXTOSAP));
	fn = path;

	// Lesen der Wanr aus SAP-Datei
	if (!readAuftraege(fn))
	{
		CString str; str = getStammdaten().getText("IDS_ERROR6b").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONERROR);
		return;
	}

	// View Wanr
	//viewAuftraege(&wanummer);
	SapfileOpen sapo;
	sapo.openDialog(&wanummer, currentAusgabe);

	// Loeschen ausgewaehlter Wanr
	for (int i=0; i<wanummer.GetSize(); i++)
	{
		if (deleteAddAuftrag(fn, "", wanummer.GetAt(i)) != ADDTOFILE)
		{
			CString err, str; str = getStammdaten().getText("IDS_ERROR7b").c_str();
			err.Format(str, wanummer.GetAt(i));
			AfxMessageBox(err, MB_OK | MB_ICONERROR);
		}
	}
}

bool Sap::readAuftraege(CString fn)
{
	TRY
	{
		CStdioFile	file;
		if (file.Open(fn, CStdioFile::modeReadWrite | CStdioFile::shareDenyNone))
		{
			// File nach WAnr durchsuchen
			CStringArray lines;
			CString line;
			while (file.ReadString(line)) 
			{
				lines.Add(line);
			}
			// Wenn nicht leer, StringArray mit Filezeilen scannen und Werkauftrag suchen
			if (lines.GetSize() > 0)
			{
				for (int i=0; i<lines.GetSize(); i++)
				{
					CString temp = lines.GetAt(i).Left(2);
					if (temp == "#B" || temp == "#M" || temp == "#G")
						wanummer.Add(lines.GetAt(i).Left(8));
				}
				return true;
			}
		}
	}
	CATCH( CFileException, e )
	{
		ERRORMSG1(fn);
	
		#ifdef _DEBUG
		e->ReportError();
	    #endif
	}
	END_CATCH
	return false;		
}

int Sap::deleteAddAuftrag(CString fn, CString txt, CString wan)
{
	TRY
	{
		CStdioFile	file;
		if (file.Open(fn, CStdioFile::modeReadWrite | CStdioFile::shareDenyNone))
		{
			// File nach WAnr durchsuchen
			CStringArray lines;
			CString line;
			while (file.ReadString(line)) 
			{
				lines.Add(line);
			}
			// Wenn nicht leer, StringArray mit Filezeilen scannen und Werkauftrag suchen
			if (lines.GetSize() > 0)
			{
				CString waname;
				if (currentAusgabe == DELETEWAXFROMSAP)
					waname = wan;
				else
					waname.Format("#%s", wan);

				bool gefunden = false;
				int countervon = 0, counterbis = 0;
				int i;
				for (i=0; i<lines.GetSize(); i++)
				{
					if (!gefunden)
					{
						CString temp = lines.GetAt(i).Left(8);
						if (temp == waname)
						{
							gefunden = true;
							countervon = i;
						}
					}
					else
					{
						CString temp = lines.GetAt(i).Left(2);
						if (temp == "#B" || temp == "#M" || temp == "#G")
						{
							counterbis = i;
							break;
						}
					}
				}
				// Wenn gefunden dann Werkauftrag loeschen
				if (gefunden)
				{
					if (counterbis == 0)
						counterbis = i; 
					file.Close();
					// Erstmal das ganze File loeschen
					if (!DeleteFile(fn))
					{
						ERRORMSG1(fn);
						return FILEERROR;
					}
					else
					{
						// Alle Zeilen ausser Werkaufrag wieder einfuegen
						CFile newfile(fn, CFile::modeCreate | CFile::modeReadWrite);
						for (int i=0; i<lines.GetSize(); i++)
						{
							if (i < countervon || i >= counterbis)
							{
								newfile.Write(lines.GetAt(i), lines.GetAt(i).GetLength());
								newfile.Write("\r\n", 2);
							}
						}
						// Ans Ende springen und Text anfuegen 
						if (txt.GetLength())
							newfile.Write(txt, txt.GetLength());
						newfile.Close();
					}
				}
				else
				{
					file.Close();
					// Ans Ende springen und Text anfuegen 
					if (txt.GetLength())
						addAuftrag(fn, txt);
				}
			}
			else
			{
				file.Close();
				// Ans Ende springen und Text anfuegen 
				if (txt.GetLength())
					addAuftrag(fn, txt);
			}
			return ADDTOFILE;
		}
	}
	CATCH( CFileException, e )
	{
		ERRORMSG1(fn);
	
		#ifdef _DEBUG
		e->ReportError();
	    #endif
		return FILEERROR;
	}
	END_CATCH
	return NEWFILE;
}

void Sap::addAuftrag(CString fn, CString txt)
{
	TRY
	{
		CFile newfile;
		if (newfile.Open(fn, CFile::modeReadWrite | CFile::shareDenyNone))
		{
			newfile.SeekToEnd();
			newfile.Write(txt, txt.GetLength());
		}
		newfile.Close();
	}
	CATCH( CFileException, e )
	{
		ERRORMSG1(fn);
	
		#ifdef _DEBUG
		e->ReportError();
	    #endif
	}
	END_CATCH
}

bool Sap::writeFile(CString fn, CString txt, CString wan)
{
	switch (deleteAddAuftrag(fn, txt, wan))
	{
	case NEWFILE:
		TRY
		{
			// Neues File
			CFile newfile(fn, CFile::modeCreate | CFile::modeReadWrite);
			newfile.Write(txt, txt.GetLength());
			newfile.Close();
		}
		CATCH( CFileException, e )
		{
			ERRORMSG1(fn);
	
			#ifdef _DEBUG
			e->ReportError();
			#endif
			return false;
		}
		END_CATCH
		return true;
	case ADDTOFILE:
		return true;
	default:
		return false;
	}
}

///Schreibt die Daten eines Auftrages in die Saptabelle 
void Sap::updateSapTabelle(double price)
{
	CString p;
	p.Format("%.2f", price);
	price = atof(p);

	Server serverdb(Server::save, NULL, NULL);
	serverdb.set_werkauftrag(get_werkauftrag());
	serverdb.updateSap(get_werkauftrag()->get_status().get_wanr(), price);

	// Hier wird zusaetzlich noch das sap-Flag in der Tabelle 'status' gesetzt
	// Dies wird aber nie wieder geloescht, auch nicht, wenn Menuepunkt zum
	// Loeschen eines Auftrages aus SAP-File aufgerufen wird.
	SammelWerkauftrag *sammel = NULL;
	for (int i = 0; i < walist.GetCount(); i++)
	{
		// Der erste Werkauftrag kann ein Sammelauftrag sein. 
		if ((get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(get_werkauftrag())) && (walist.GetCount() > 1))
		{
			sammel = dynamic_cast<SammelWerkauftrag*>(get_werkauftrag());
			
			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
			continue;
		}

		//Status auf freigeben durch Av setzen
		get_werkauftrag()->get_status().set_status(Status::FreigegebenAv);

		MXTable set(WaxSys::getServerdb(), "status");
		CString sql, ret;

		sql.Format("UPDATE status SET status.sap=1, status.status=%i WHERE waid=%i", (int)get_werkauftrag()->get_status().get_status(), get_werkauftrag()->get_waid());
		ret = set.executeSQL(sql);

		CObject *tmp = walist.GetHead();
		walist.RemoveHead();
		walist.AddTail(tmp);
	}

	//Wenn Aufträge zu einem Sammelauftrag gehörten, bei diesem auch den Status setzen
	if (sammel)
		sammel->get_status().set_status(Status::FreigegebenAv);
}

///Formatiert die Zeilen mit den Arbeitsfolgen in der WaxToSap Datei
const CString Sap::format_arbeitsfolgen(CObList& afos, const CString& wanummer)
{
	// todo: Inialisierung der Arbeitsfolgen entfernen und Methode const machen
	if (afos.IsEmpty()) 
		return "";

	CString afonr, arbpl, tx, ta, text, txt;

	int count = 0;
	for (POSITION p = afos.GetHeadPosition(); p != NULL;)	
	{
		count++;

		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge*>(afos.GetNext(p));
		XASSERT(afo);
		if (!afo)
			return "";

		// Soll Afo zu SAP rüber
		if (!afo->get_sap() || afo->get_anz() <= 0)
			continue;

		wa.get_arbeit().updateSpecialAF(afo, get_werkauftrag()->get_beschichtung(), gesamtgewicht, maxlaenge, 0);

		afonr.Format("%04ld",afo->get_nr());

		//arbpl.Format("%-5d",  afo->get_arbpl());
		arbpl = afo->get_arbpl();
		//Arbeitsplatz 5stellig 	
		int laengeArbpl = 5;
		if (arbpl.GetLength() < laengeArbpl)
		{
			CString leer = "     ";
			arbpl += leer.Left(laengeArbpl - arbpl.GetLength());
		}

		
		tx.Format("%7.2f",    afo->get_zeit()+afo->get_ruestzeit());

		double preis = 0.0;
		if (atoi(arbpl) >= 99000)
			preis = afo->get_preis();

		ta.Format("%7.2f",   preis);
		
		

		
		
		CString data;
		// Leerzeichen sind hier wg. der Lesbarkeit durch Unterstriche ersetzt
		data.Format("___%s______________________________________%s%s%s%s_______", 				
			wanummer, 
			afonr, 
			arbpl, 
			tx, 
			ta); 
		data.Replace('_', ' ');
		txt += "*" + data + "\r\n"; // Erste Zeile

		//Text der Afo wird nicht mehr nach 40 Zeichen in eine neue Zeile geschrieben
		CString t;
		t.Format("&%s001%s\r\n", data, afo->get_txt());
		txt += t;

		
	}
	return txt;
}
