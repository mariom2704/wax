#include "stdafx.h"


#include "zinkkostendlg.h"


Preise::Preise(Werkauftrag *wa)
{
	set_werkauftrag(wa);	
	resetAttributes();
	walist.RemoveAll();
}

// Destruktor
Preise::~Preise()
{
}

void Preise::resetAttributes()
{
	sonderkosten				= 0;
	geldwert					= 0;
	preisVK3					= 0;
	gesamtpreisVK3				= 0; 
	
	Ausgaben::resetAttributes();
}

//get werkauftrag
Werkauftrag* Preise::get_werkauftrag() 
{
	XASSERT(walist.GetCount());
	if (!walist.GetCount())
		return NULL;

	WaxPart *wp = dynamic_cast<WaxPart*>(walist.GetHead());
	Werkauftrag *werk = wp->get_werkauftrag();
	XASSERT(werk);
    return werk;
}

// Liste mit Ausgaben-IDs (ausgaben) und Ausgabennamen initialisieren
void Preise::initAusgaben()
{
	//"ausgaben" enthaelt die fuer den jeweiligen Werkauftrag gueltigen Ausgaben-IDs. 
	Ausgaben::initAusgaben();
	ausgaben.RemoveAll();

	// kein Objekt in walist 
	if (walist.IsEmpty())
		return;

	if (!WaxSys::isAV())
	{
		ausgaben.Add(PREIS);
		return;
	}
	bool vk = false, wap = false;

	for (int i = 0; i < walist.GetCount(); i++, walist.AddTail(walist.GetHead()), walist.RemoveHead())
	{
		// Objekt in walist kein Werkauftrag
		WaxPart *wp = dynamic_cast<WaxPart*>(walist.GetHead());
		if (!wp->get_werkauftrag())
			return;
		
		// der Werkauftrag kann ein Sammelauftrag sein. 
		if (wp->get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wp->get_werkauftrag())) 
		{
			if (wp->get_subobjects().IsEmpty())
				return;
			werkauftrag = dynamic_cast<Werkauftrag*>(wp->getFirstSubobject()->get_werkauftrag());
		}
		else
			werkauftrag = dynamic_cast<Werkauftrag*>(wp->get_werkauftrag());

		if (dynamic_cast<Rohrleitung*>(werkauftrag) || dynamic_cast<ApaxWerkauftrag*>(werkauftrag))
			vk = true;
		//if (dynamic_cast<Gewinderohr*>(werkauftrag) || dynamic_cast<FormstueckeFittinge*>(werkauftrag) 
		//|| dynamic_cast<ApaxWerkauftrag*>(werkauftrag))
		if (dynamic_cast<FormstueckeFittinge*>(werkauftrag))
			wap = true;
	}

	if (vk)
	{
		ausgaben.Add(VK3_PREISINFO);
		// nur als AV
		if (WaxSys::isAV() || WaxSys::para_int(DEBUGNL)) 
		{
			ausgaben.Add(VK3_ELEMENTLISTE);
			ausgaben.Add(VK3_KOMPLETTLISTE);
		}
	}
	if (WaxSys::isAV())
		ausgaben.Add(ZINKKOSTEN);
}

// Liefert den Namen (fuer Menueeintrag) fuer eine Ausgabe.
CString Preise::getName(int ausgabe_id) const
{
	CString str;
	switch (ausgabe_id) 
	{
		case PREIS:
			str = getStammdaten().getText("IDS_MENU_PREIS").c_str();
			return str;
		case VK3_PREISINFO:
			str = getStammdaten().getText("IDS_MENU_VK3_PREIS").c_str();
			return str;
		case VK3_KOMPLETTLISTE:
			str = getStammdaten().getText("IDS_MENU_VK3_GESAMTPREIS").c_str();
			return str;
		case VK3_ELEMENTLISTE:
			str = getStammdaten().getText("IDS_MENU_VK3_ELEPREIS").c_str();
			return str;
		case ZINKKOSTEN:
			return getStammdaten().getText("IDS_VERZINKUNGSKOSTEN").c_str();
	}
	
	XASSERT(FALSE);
	str = getStammdaten().getText("IDS_UNBEKANNTE_AUSGABE").c_str();
	return str;
}

// Gewuenschte Ausgabe ausfuehren
const CStringArray& Preise::makeAusgabe(int id)
{
	/*
	ID gibt die auszufuehrende Ausgabe an. Default ist id = -1, was zur Folge hat, dass der Wert
	von currentAusgabe benutzt wird. Ist currentAusgabe nicht gesetzt, wird die erste in ausgaben definierte 
	Ausgabe als Default gesetzt und ausgefuehrt.  
	*/

	currentAusgabe = id;
	Ausgaben::makeAusgabe(currentAusgabe);

	// keine Objekte in walist
	if (walist.IsEmpty())
		return htmlFiles;
	
	CString fn;
	CString text, str; 
	Preisausgabe pout(get_werkauftrag());
	bool gf = WaxSys::isAV(); //if (WaxSys::isGF()) //Geschaeftsfuehrung (Niemann)

	// Erstmal Listen loeschen	
	deleteListen();
	resetAttributes();
	
	setMaxLaengeUndGewicht(true, true);

	// In den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
	// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
	for (int i = 0; i < walist.GetCount(); i++)
	{
		WaxPart *wp = dynamic_cast<WaxPart*>(walist.GetHead());
		XASSERT(wp->get_werkauftrag());
		if (!wp->get_werkauftrag())
		{
			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
			continue;
		}
		else
		{
			if (wp->get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wp->get_werkauftrag())) 
			{
				if (!wp->get_subobjects().IsEmpty())
					werkauftrag = dynamic_cast<Werkauftrag*>(wp->getFirstSubobject()->get_werkauftrag());
			}
			else
				werkauftrag = dynamic_cast<Werkauftrag*>(wp->get_werkauftrag());

			bool weiter = false;
			switch (currentAusgabe) 
			{
			case PREIS:
				weiter = true;
				break;
			case VK3_PREISINFO:
			case VK3_KOMPLETTLISTE:
			case VK3_ELEMENTLISTE:
				if (dynamic_cast<Rohrleitung*>(werkauftrag) || dynamic_cast<ApaxWerkauftrag*>(werkauftrag))
					weiter = true;
			}
				
			if (!weiter)
			{
				CObject *tmp = walist.GetHead();
				walist.RemoveHead();
				walist.AddTail(tmp);
				continue;
			}

			wp->get_werkauftrag()->get_preis().updatePreis();
			switch (currentAusgabe) 
			{
			
			case PREIS:
			case VK3_PREISINFO:
			case VK3_KOMPLETTLISTE:
			case VK3_ELEMENTLISTE:
				createVK3Abrechnungdata();
				break;
			}

			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
		}
	}
	
	// Eigentliche Ausgabe
	switch (currentAusgabe) 
	{
	case PREIS:
	case VK3_PREISINFO:
		check_NullEuro();
		str = getStammdaten().getText("IDS_PREIS_BETRAEGT").c_str();
		text.Format(str, gesamtpreisVK3);
		AfxMessageBox(text);
		break;
	case VK3_KOMPLETTLISTE:
		check_NullEuro();
		pout.openDialog(VK3_KOMPLETTLISTE, gf, 
						preisVK3, 
						sonderkosten, 
						gesamtpreisVK3);
		break;
	case VK3_ELEMENTLISTE:
		check_NullEuro();
		// alle Preise 
		setVK3Gesamtpreis();

		// Listen sortieren
		wa.sortVk3Listen(wa.get_vk3elementeListe());
		wa.sortVk3Listen(wa.get_passlaengenListe());
		wa.sortVk3Listen(wa.get_pulverlaengenListe());

		pout.openDialog(VK3_ELEMENTLISTE, gf, 
						wa.get_vk3elementeListe(),
						wa.get_passlaengenListe(),
						wa.get_pulverlaengenListe(),
						wa.get_preis().get_vk3berechnung().get_preis());
		break;
	case ZINKKOSTEN:
		ZinkKostenDlg d(walist);
		d.DoModal();
		break;
	}
	
	return htmlFiles;
}


void Preise::createVK3Abrechnungdata()
{
	// erstmal alle Listen leeren
	if (currentAusgabe != VK3_ELEMENTLISTE)
		deleteListen();

	// Der Werkauftrag kann ein Sammelauftrag sein. 
	WaxPart *wp = dynamic_cast<WaxPart*>(walist.GetHead());
	if (wp->get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wp->get_werkauftrag()))
	{
		for (POSITION pos = wp->get_subobjects().GetHeadPosition(); pos;)
		{
			WaxPart *pt = dynamic_cast<WaxPart*>(wp->get_subobjects().GetNext(pos));
			if (pt->get_deleted())
				continue;
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
			/*
			Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(pt->get_werkauftrag());
			if (grohr)
			{
				CString str; str = getStammdaten().getText("IDS_VKABRECHNUNG_NUR_ROHR").c_str();
				AfxMessageBox(str, MB_ICONERROR |MB_OK);
				break;
			}
			*/

			if (rohr)
			{
				rohr->createModell();

				//Noetig fuer Mindermenge
				get_wa().get_arbeit().merge(rohr->get_arbeit());

				// VK3 - Elemente
				set_elelist(rohr->get_vk3elementeListe(),true);	
				set_passlist(rohr->get_passlaengenListe());
				set_pulverlist(rohr->get_pulverlaengenListe(),true);
				
				// Sonderkosten
				set_sokolist(rohr->get_preis().get_sokolist());
			}
			else
			{
				ApaxWerkauftrag* apax = dynamic_cast<ApaxWerkauftrag*>(pt->get_werkauftrag());
				if (apax)
				{
					set_elelist(apax->get_vk3elementeListe(),false);	
				}

			}
		}
	}
	else
	{
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(wp->get_werkauftrag());
		
		if (rohr)
		{
			// Wozu, nur um zu pruefen ob gueltige Afos drin sind?
			for (POSITION pos = wp->get_werkauftrag()->get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos;)
			{
				Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge*>(wp->get_werkauftrag()->get_arbeit().get_arbeitsfolgen().GetNext(pos));
				XASSERT(afo)
				if (!afo)
					break;
			}

			// VK3 - Elemente
			rohr->createModell();
			
			//Noetig fuer Mindermenge
			get_wa().get_arbeit().merge(rohr->get_arbeit());

			set_elelist(rohr->get_vk3elementeListe(),false);	
			set_passlist(rohr->get_passlaengenListe());
			set_pulverlist(rohr->get_pulverlaengenListe(),false);
			
			// Sonderkosten
			set_sokolist(rohr->get_preis().get_sokolist());
		}
		else
		{
			ApaxWerkauftrag* apax = dynamic_cast<ApaxWerkauftrag*>(wp->get_werkauftrag());
			if (apax)
			{
				set_elelist(apax->get_vk3elementeListe(),false);	
			}
		}
	}

	if (currentAusgabe != VK3_ELEMENTLISTE)
	{
		// Liefer- und Freigabedatum, Bonus und vk3-Status kopieren 
		wa.get_beschichtung().copy(wp->get_werkauftrag()->get_beschichtung());
		wa.get_leitkarte().set_wunschliefertermin(wp->get_leitkarte().get_wunschliefertermin());
		wa.get_status().set_freigabedatum(wp->get_werkauftrag()->get_status().get_freigabedatum());
			
		
		// Preise berechnen
		setVK3Gesamtpreis();

		// Preise holen
		preisVK3		+=	wa.get_preis().get_vk3berechnung().get_preis(); 
		sonderkosten	+=	wa.get_preis().get_sonderkosten(); 
		gesamtpreisVK3	+=	wa.get_preis().get_gesamtpreisVK3();
	}
}

// Erfasst die Daten fuer die Sokos
void Preise::createKomplettliste()
{
	// Afosondersachen
	afoSonderbehandlung();

	// Gesamtpreis berechnen
	setGesamtpreise();
}

bool Preise::isEleNull()
{
	// Element in Eleliste mit Preis = 0.00
	for (POSITION pos = wa.get_elementList().GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(wa.get_elementList().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->get_festPreis() < 0.009) 
			return true;
	}
	return false;
}

//Kopiert die Liste mit (Unter)Aufträgen
void Preise::set_walist(CObList& value)
{
	WaxPart *pt;
	for (POSITION pos = value.GetHeadPosition(); pos; )
	{
		pt = dynamic_cast<WaxPart*>(value.GetNext(pos));
		XASSERT(pt); 
		if (!pt || pt->get_deleted()) 
			continue;
		Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
		if (rohr)
			rohr->createModell();
		
		// Ist Objekt eine Gliederung, dann in extra Liste speichern 
		if (pt->IsKindOf(RUNTIME_CLASS(PrjGliederung)))
			gliederunglist.AddTail(pt); 
		// sonst in walist
		else
			walist.AddTail(pt);
	}
}

// Setzt Auftraege in Liste
void Preise::set_walist()
{
	CObList templist;
	WaxPart *pt;

	POSITION pos;
	// Liste mit Gliederungen durchgehen
	for (pos = gliederunglist.GetHeadPosition(); pos; )
	{
		// Unterobjekte in Subliste speichern
		pt = dynamic_cast<WaxPart*>(gliederunglist.GetNext(pos));
		XASSERT(pt); 
		if (!pt) 
			continue;
		set_wasublist(pt->get_subobjects());		
		
		// Subliste durchgehen
		for (POSITION pos = wasublist.GetHeadPosition(); pos; )
		{
			pt = dynamic_cast<WaxPart*>(wasublist.GetNext(pos));
			XASSERT(pt); 
			if (!pt || pt->get_deleted()) 
				continue;
			
			Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
			if (rohr)
				rohr->createModell();

			// Ist Objekt wieder eine Gliederung, dann in extra Liste speichern 
			if (pt->IsKindOf(RUNTIME_CLASS(PrjGliederung)))
				templist.AddTail(pt); 
			// Sonst in walist
			else
				walist.AddTail(pt);
		}
		wasublist.RemoveAll();
	}
	// abgearbeitete Gliederungen aus Liste löschen
	gliederunglist.RemoveAll();

	// Liste wieder mit weiteren Gliederungen füllen
	for (pos = templist.GetHeadPosition(); pos; )
	{
		pt = dynamic_cast<WaxPart*>(templist.GetNext(pos));
		gliederunglist.AddTail(pt); 
	}
	templist.RemoveAll();
}

// Setzt Unterobjekte in Liste
void Preise::set_wasublist(CObList& value)
{
	WaxPart *pt;
	for (POSITION pos = value.GetHeadPosition(); pos; )
	{
		pt = dynamic_cast<WaxPart*>(value.GetNext(pos));
		XASSERT(pt); 
		if (!pt || pt->get_deleted()) 
			continue;
		wasublist.AddTail(pt);
	}
}

// Liefert Liste mit Gliederungen
const CObList& Preise::get_gliederunglist() const
{
	return gliederunglist;
}


void Preise::setMaxLaengeUndGewicht(bool mat, bool stueck)
{
	for (POSITION pos = walist.GetHeadPosition(); pos; )
	{
		
		WaxPart* wp = dynamic_cast<WaxPart*>(walist.GetNext(pos));
		if (wp->get_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wp->get_werkauftrag()))
		{
			for (POSITION pos = wp->get_subobjects().GetHeadPosition(); pos;)
			{
				WaxPart *pt = dynamic_cast<WaxPart*>(wp->get_subobjects().GetNext(pos));
				werkauftrag = dynamic_cast<Werkauftrag*>(pt->get_werkauftrag());
				Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());

				if (mat)
				{
					gesamtgewicht		+= werkauftrag->get_material().get_gewicht();
					gesamtgewichtEK		+= werkauftrag->get_material().get_gewicht(1);
					gesamtgewichtLAGER	+= werkauftrag->get_material().get_gewicht(2);
					gesamtgewichtOP		+= werkauftrag->get_material().get_gewicht(3);
				}
				if (stueck && rohr)
				{
					gesamtgewicht		+= rohr->getGesGewicht();
					gesamtgewichtEK		+= rohr->getGesGewicht(1);
					gesamtgewichtLAGER	+= rohr->getGesGewicht(2);
					gesamtgewichtOP		+= rohr->getGesGewicht(3);
				}
				
				if (werkauftrag->get_maxlaenge() > maxlaenge)
					maxlaenge = werkauftrag->get_maxlaenge();
			}
		}
		else
		{
			werkauftrag = dynamic_cast<Werkauftrag*>(wp->get_werkauftrag());
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(wp->get_werkauftrag());
			if (mat)
			{
				gesamtgewicht		+= werkauftrag->get_material().get_gewicht();
				gesamtgewichtEK		+= werkauftrag->get_material().get_gewicht(1);
				gesamtgewichtLAGER	+= werkauftrag->get_material().get_gewicht(2);
				gesamtgewichtOP		+= werkauftrag->get_material().get_gewicht(3);
			}
			if (stueck && rohr)
			{
				gesamtgewicht		+= rohr->getGesGewicht();
				gesamtgewichtEK		+= rohr->getGesGewicht(1);
				gesamtgewichtLAGER	+= rohr->getGesGewicht(2);
				gesamtgewichtOP		+= rohr->getGesGewicht(3);
			}
			
			if (werkauftrag->get_maxlaenge() > maxlaenge)
				maxlaenge = werkauftrag->get_maxlaenge();
		}
	}
}

