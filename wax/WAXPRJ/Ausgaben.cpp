#include "stdafx.h"


#include "drucklistenverwaltung.h"



//lastAusgabe, ID der zu erzeugenden Ausgabe
int Ausgaben::lastAusgabe = 0;
//currentAusgabe, ID der zu erzeugenden Ausgabe
int Ausgaben::currentAusgabe = 0;

//Konstruktor
Ausgaben::Ausgaben()
{
	werkauftrag = NULL;
	resetAttributes();
	walist.RemoveAll();
}

//Destruktor
Ausgaben::~Ausgaben()
{
	deleteListen();
}

// resetAttributes, setze die Klassenattribute auf 0
void Ausgaben::resetAttributes()
{
	maxlaenge = 0;
	gesamtlaenge = 0;
	gesamtgewicht = 0;
	gesamtgewichtEK = 0;
	gesamtgewichtLAGER = 0;
	gesamtgewichtOP = 0;
	gesamtzeit = 0;
	gesamtzeitOhnePulvern = 0;
	strangrohr = false;
}

// deleteListen, loescht die Listen im Objekt des Werkauftrages
void Ausgaben::deleteListen()
{
	POSITION pos;
	for (pos = wa.get_material().get_matlist().GetHeadPosition(); pos;)
		delete wa.get_material().get_matlist().GetNext(pos);
	wa.get_material().get_matlist().RemoveAll();
	
	wa.get_stueckliste().clear();

	for (pos = wa.get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos;)
		delete wa.get_arbeit().get_arbeitsfolgen().GetNext(pos);
	wa.get_arbeit().get_arbeitsfolgen().RemoveAll();

	for (pos = wa.get_preis().get_sokolist().GetHeadPosition(); pos;)
		delete wa.get_preis().get_sokolist().GetNext(pos);
	wa.get_preis().get_sokolist().RemoveAll();

	for (pos = wa.get_elementList().GetHeadPosition(); pos;)
		delete wa.get_elementList().GetNext(pos);
	wa.get_elementList().RemoveAll();

	for (pos = wa.get_vk3elementeListe().GetHeadPosition(); pos;)
		delete wa.get_vk3elementeListe().GetNext(pos);
	wa.get_vk3elementeListe().RemoveAll();

	for (pos = wa.get_pulverlaengenListe().GetHeadPosition(); pos;)
		delete wa.get_pulverlaengenListe().GetNext(pos);
	wa.get_pulverlaengenListe().RemoveAll();

	for (pos = wa.get_passlaengenListe().GetHeadPosition(); pos;)
		delete wa.get_passlaengenListe().GetNext(pos);
	wa.get_passlaengenListe().RemoveAll();
}

//set walist
void Ausgaben::set_walist(CObList& value)
{
	/*
	Achtung: Mehrfacher Aufruf führt zu einer aufsummierten Liste
	*/
	WaxPart *pt;
	for (POSITION pos = value.GetHeadPosition(); pos; )
	{
		pt = dynamic_cast<WaxPart*>(value.GetNext(pos));
		XASSERT(pt); 
		if (!pt || pt->get_deleted()) 
			continue;
		Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
		if (rohr)
		{
			rohr->createModell();
		}
		walist.AddTail((CObject*)pt->get_werkauftrag());
	}
}

//get werkauftrag
Werkauftrag* Ausgaben::get_werkauftrag() 
{
	XASSERT(walist.GetCount());
	if (!walist.GetCount())
		return NULL;

	Werkauftrag *werk = dynamic_cast<Werkauftrag*>(walist.GetHead());
	XASSERT(werk);
	return werk;
}

//set werkauftrag
void Ausgaben::set_werkauftrag(Werkauftrag* value)
{
	XASSERT(value);
	XASSERT(!walist.Find(value));
	walist.AddHead(value);
}

// initAusgaben, Ausgaben initialisieren
void Ausgaben::initAusgaben()
{

}

//get ausgaben
const CArray<int,int>& Ausgaben::get_ausgaben()
{
	return ausgaben;
}

//get currentAusgabe
const int Ausgaben::get_currentAusgabe() const
{
	return currentAusgabe;
}

//set currentAusgabe
void Ausgaben::set_currentAusgabe(int value)
{
	currentAusgabe = value;
}

// get lastAusgabe
int Ausgaben::get_lastAusgabe() const
{
	return lastAusgabe;
}

//set lastAusgabe
void Ausgaben::set_lastAusgabe(int value)
{
	lastAusgabe = value;
}

// makeAusgabe, Gewuenschte Ausgabe ausfuehren
const CStringArray& Ausgaben::makeAusgabe(int id)
{
	UNREFERENCED_PARAMETER(id);
	return htmlFiles;
}


//Durchläuft die walist und errechnet die Gesamtgewichte und die maximale Länge
void Ausgaben::setMaxLaengeUndGewicht(bool mat, bool stueck)
{
	bool isSammel = false;
	for (POSITION pos = walist.GetHeadPosition(); pos; )
	{
		werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetNext(pos));
		XASSERT(werkauftrag);
		if (!werkauftrag)
			continue;
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
		SammelWerkauftrag* sammel = dynamic_cast<SammelWerkauftrag*>(werkauftrag);
	
		if (mat && !isSammel)
		{
			gesamtgewicht		+= werkauftrag->get_material().get_gewicht(0, true);
			gesamtgewichtEK		+= werkauftrag->get_material().get_gewicht(1, true);
			gesamtgewichtLAGER	+= werkauftrag->get_material().get_gewicht(2, true);
			gesamtgewichtOP		+= werkauftrag->get_material().get_gewicht(3, true);

			if (sammel)
				isSammel = true;
		}
		if (stueck && rohr)
		{
			gesamtgewicht		+= rohr->getGesGewicht();
			gesamtgewichtEK		+= rohr->getGesGewicht(1);
			gesamtgewichtLAGER	+= rohr->getGesGewicht(2);
			gesamtgewichtOP		+= rohr->getGesGewicht(3);
		}
		
		if (werkauftrag->getMaxLaenge() > maxlaenge)
			maxlaenge = werkauftrag->getMaxLaenge();
	}
}


// set_stklist, fuellt die Stueckliste des Werkauftragobjekts mit der 
//uebergebenen Stueckkliste des aktuellen Werkauftrages
void Ausgaben::set_stklist(const CObList& value, bool nullpreis, bool einkauf)
{
	int test = 0;
	// Stueckliste vom Werkauftrag scannen
	for (POSITION pos = value.GetHeadPosition(); pos;)
	{
		const Element *ele = dynamic_cast<const Element*>(value.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->get_artikelnr() < 100000)
			test++;


		// EK Teile zu SAP? Erstmal nur bei Gewinderohr nicht (siehe Konstruktor) 
		if (einkauf || (!einkauf && !ele->get_einkauf()))
		{
			
			// Nur Material ohne Preis?
			if ((ele->get_festPreis() < 0.009 &&  nullpreis) || !nullpreis)
			{
				POSITION upos;
				// Element schon in Gesamtliste, dann Anzahl addieren (bei Rohr die Laenge)
				for (upos = wa.get_stueckliste().GetHeadPosition(); upos;)
				{
					Element *element = dynamic_cast<Element*>(wa.get_stueckliste().GetNext(upos));
					XASSERT(element);
					if (!element)
						break;

					if (element->get_artikelnr() == ele->get_artikelnr()) 
					{
						if (element->get_id() == ROHR || element->isEleRohr() || (element->get_id() == STDSTUTZEN && element->get_urid() == ROHR))
						{
							element->set_laenge(element->get_laenge() + ele->get_laenge());
							element->set_anzahl(element->get_anzahl() + ele->get_anzahl());
						}
						else 
							element->set_anzahl(element->get_anzahl() + ele->get_anzahl());
						upos--;
						break;
					}
				}
				// in Gesamtliste packen
				if (upos == NULL) 
					wa.get_stueckliste().add_copy(*ele);
			}
			
		}
	}
}

// setGesamtmaterialliste, fuellt die Materialiste des Werkauftragobjekts mit der 
//Stueckliste des Werkauftragobjekts auf
void Ausgaben::setGesamtmaterialliste()
{
	// Stueckliste vom Werkauftrag scannen
	for (POSITION pos = wa.get_stueckliste().GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(wa.get_stueckliste().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		POSITION upos;
		// Material schon in Gesamtliste, dann Anzahl addieren
		for (upos = wa.get_material().get_matlist().GetHeadPosition(); upos;)
		{
			Material *mat = dynamic_cast<Material*>(wa.get_material().get_matlist().GetNext(upos));
			XASSERT(mat);
			if (!mat)
				break;

			if (mat->get_artikelnr() == ele->get_artikelnr()) 
			{
				// Bei Rohr & Profil die Laenge als mm in Mass1 speichern 
				if (ele->get_id() == ROHR || ele->isEleRohr() || (ele->get_id() == STDSTUTZEN && ele->get_urid() == ROHR))
					mat->set_mass1(mat->get_mass1() + ele->get_laenge());
				else 
					mat->set_anzahl(mat->get_anzahl() + ele->get_anzahl());
				upos--;
				break;
			}
		}
		// in Gesamtliste packen
		if (upos == NULL) 
		{
			Material *m = new Material;
			m->copy(*ele);
			// Bei Rohr & Profil die Laenge als Meter in Mass1 speichern 
			if (ele->get_id() == ROHR || ele->isEleRohr() || (ele->get_id() == STDSTUTZEN && ele->get_urid() == ROHR))
				m->set_mass1(ele->get_laenge());
			else
				m->set_mass1(0);
			wa.get_material().get_matlist().AddTail(m);
		}
	}
}

// set_matlist, fuellt die Materialliste des Werkauftragobjekts mit der 
//uebergebenen Materialliste des aktuellen Werkauftrages
void Ausgaben::set_matlist(const CObList& value, int list, bool nullpreis, bool einkauf)
{
	// Materialliste vom Werkauftrag scannen
	for (POSITION pos = value.GetHeadPosition(); pos;)
	{
		const Material *mat = dynamic_cast<const Material*>(value.GetNext(pos));
		XASSERT(mat);
		if (!mat)
			break;

		// EK Teile zu SAP? Erstmal nur bei Gewinderohr nicht (siehe Konstruktor) 
		if (einkauf || (!einkauf && !mat->get_einkauf()))
		{
			// Alles oder nur EK- bzw. Lagerteile
			if ((list == WaxDruck::DrucklistenVerwaltung::MATERIALLISTE_GESAMT)
				|| (list == WaxDruck::DrucklistenVerwaltung::STRANGROHRLISTE))
			{				
				POSITION upos = NULL;
				if ((mat->get_sapPreis() < 0.009 &&  nullpreis) || !nullpreis)
				{					
					if (mat->get_id() != NOARTNR && mat->get_artikelnr() != 0)
					{
						// Material schon in Gesamtliste, dann Anzahl addieren
						for (upos = wa.get_material().get_matlist().GetHeadPosition(); upos;)
						{
							Material *material = dynamic_cast<Material*>(wa.get_material().get_matlist().GetNext(upos));
							XASSERT(material);
							if (!material)
								break;

							if (material->get_artikelnr() == mat->get_artikelnr()) 
							{
								// Bei Rohr & Profil die Laenge * Anzahl als mm in mass1 speichern 
								if (material->get_mass1() > 0)
									material->set_mass1(material->get_mass1() + (mat->get_anzahl()*mat->get_mass1()));
								else
									material->set_anzahl(material->get_anzahl() + mat->get_anzahl());
								upos--;
								break;
							}
						}
					}

					// in Gesamtliste packen
					if (upos == NULL) 
					{
						Material *m = new Material;
						m->copy(*mat);
						// Bei Rohr & Profil die Laenge * Anzahl als mm in mass1 speichern 
						if (m->get_mass1() > 0)
							m->set_mass1(m->get_anzahl()*m->get_mass1());
						wa.get_material().get_matlist().AddTail(m);
					}
				}
			}
		}
	}
}

// set_sokolist, fuellt die Sokoliste des Werkauftragobjekts mit der uebergebenen 
//Sokoliste des aktuellen Werkauftrages
void Ausgaben::set_sokolist(const CObList& value)
{
	// Liste mit Sokos durchgehen
	for (POSITION pos = value.GetHeadPosition(); pos;)
	{
		const Preis::Sonderkosten *soko = dynamic_cast<const Preis::Sonderkosten*>(value.GetNext(pos));
		XASSERT(soko);
		if (!soko)
			break;

		POSITION upos;
		// Soko schon in Gesamtliste, dann Preis addieren
		for (upos = wa.get_preis().get_sokolist().GetHeadPosition(); upos;)
		{
			Preis::Sonderkosten *so = dynamic_cast<Preis::Sonderkosten*>(wa.get_preis().get_sokolist().GetNext(upos));
			XASSERT(so);
			if (!so)
				break;

			// Wenn gleicher Text
			if (so->get_text() == soko->get_text()) 
			{
				// Aber nicht bei Mindermenge
				CString str; str = getStammdaten().getText("IDS_AUFSCHLAG_MINDER").c_str();
				if (so->get_text() != str) 
					so->set_preis(so->get_preis() + soko->get_preis());
				upos--;
				break;
			}
		}

		// in Gesamtliste packen
		if (upos == NULL) 
		{
			Preis::Sonderkosten *s = new Preis::Sonderkosten;
			s->copy(*soko);
			wa.get_preis().get_sokolist().AddTail(s);
		}
	}
}

void Ausgaben::set_passlist(const CObList& value)
{
	// Templiste mit Elementen durchgehen und gleiche addieren
	for (POSITION pos = value.GetHeadPosition(); pos;)
	{
		const Element *ele = dynamic_cast<const Element *>(value.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		POSITION upos;
		// Element schon in Gesamtliste, dann Anzahl addieren
		for (upos = wa.get_passlaengenListe().GetHeadPosition(); upos;)
		{
			Element *elem = dynamic_cast<Element*>(wa.get_passlaengenListe().GetNext(upos));
			XASSERT(elem);
			if (!elem)
				break;

			if (elem->get_id() == ele->get_id() && elem->get_anschlussDN() == ele->get_anschlussDN()
												&& elem->get_dn()		   == ele->get_dn()) 
			{
				elem->set_anzahl(elem->get_anzahl() + ele->get_anzahl());
				upos--;
				break;
			}
		}
		// in Gesamtliste packen
		if (upos == NULL) 
		{
			Element *e = new Element;
			e->copy(*ele);
			wa.get_passlaengenListe().AddTail(e);
		}
	}
}

//Liste kopieren und wenn sort = true, dann kumulieren
void Ausgaben::set_pulverlist(EnCObListVk3& value, bool sort)
{
	EnCObListVk3 tmpListe;

	EnCObListVk3& liste = wa.get_pulverlaengenListe();

	//Kopie der Liste 
	tmpListe.copy(value);

	//Liste anhängen
	liste.appendListe(tmpListe);

	if (sort)
		liste.kumulieren();

}


//Liste kopieren und wenn sort = true, dann kumulieren
void Ausgaben::set_elelist(EnCObListVk3& value, bool sort)
{
	EnCObListVk3 tmpListe;

	EnCObListVk3& liste = wa.get_vk3elementeListe();

	//Kopie der Liste 
	tmpListe.copy(value);

	//Liste anhängen
	liste.appendListe(tmpListe);

	if (sort)
		liste.kumulieren();

}


///afoSonderbehandlung 
void Ausgaben::afoSonderbehandlung()
{
	///Afos fuer Schweisser zusammenfassen 
	wa.get_arbeit().mergeSchweissafos();
	
	///Mindermenge setzen fuer Arbeitsplan
	wa.get_arbeit().setMindermenge();
	
	///Gesamtzeit ermitteln
	gesamtzeit = wa.get_arbeit().getZeit();

	///Gesamtzeit ohne Pulvern ermitteln 
	gesamtzeitOhnePulvern = wa.get_arbeit().getZeit(false);
}

// setGesamtpreise, berechnet alle Preise des Werkauftragobjekts
void Ausgaben::setGesamtpreise()
{
	// Changed-Flag auf false, damit nicht makeRohrleitung etc. aufgerufen wird 
	// und dadurch eine leere Stueckliste, Afoliste und Sokoliste erzeugt wird
//	wa.set_changed(false);

	// Muss wegen Beschichtungsabfrage fuer Addition des Material- und Pulverpreises bei F&F
	wa.set_beschichtung(get_werkauftrag()->get_beschichtung());

	// alle Preise berechnen
	wa.get_preis().calc_all();
}


//get htmlFiles
const CStringArray& Ausgaben::get_htmlFiles() const
{
	return htmlFiles;
}

const CString& Ausgaben::get_ziel() const
{
	return ziel;
}

void Ausgaben::set_ziel(const CString& value)
{
	ziel = value;
}

const int Ausgaben::get_zielwert() const
{
	return zielwert;
}

void Ausgaben::set_zielwert(const int value)
{
	zielwert = value;
}

Rohrleitung& Ausgaben::get_wa() 
{
	return wa;
}

void Ausgaben::setVK3Gesamtpreis()
{
	// alle Preise berechnen
	wa.get_preis().calc_gesamtpreisVK3();
}

///Liste auf Preis 0,00€ pruefen
void Ausgaben::check_NullEuro()
{
	CString nulleuro;
	for (POSITION pos = wa.get_vk3elementeListe().GetHeadPosition(); pos != NULL;) 
	{
		Element *ele = dynamic_cast<Element*>(wa.get_vk3elementeListe().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			return;
	
		if (ele->get_festPreis() < 0.01)
		{
			nulleuro += ele->get_kurztext() + "\n";
		}
	}

	if (nulleuro.GetLength())
	{
		CString msg, str; str = getStammdaten().getText("IDS_VKPREIS_NULL").c_str();
		msg.Format(str, nulleuro);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
}

void Ausgaben::isMatNull()
{
	CString nulleuro;
	// Element in Stueckliste mit Preis = 0.00
	POSITION pos;
	for (pos = wa.get_stueckliste().GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(wa.get_stueckliste().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->get_sapPreis() < 0.01) 
			nulleuro += ele->get_kurztext() + "\n";
	}
	// Material in Matliste mit Preis = 0.00
	for (pos = wa.get_material().get_matlist().GetHeadPosition(); pos;)
	{
		Material *mat = dynamic_cast<Material*>(wa.get_material().get_matlist().GetNext(pos));
		XASSERT(mat);
		if (!mat)
			break;

		if (mat->get_sapPreis() < 0.009) 
			nulleuro += mat->get_kurztext() + "\n";
	}
	if (nulleuro.GetLength())
	{
		CString msg;
		msg.Format(getStammdaten().getText("IDS_MAT_NULL").c_str(), nulleuro);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
	}
}



double Ausgaben::getApaxVK3Gesamtpreis()
{
	double preis = 0;
	for (POSITION pos = walist.GetHeadPosition(); pos; )
	{
		ApaxWerkauftrag *apax = dynamic_cast<ApaxWerkauftrag*>(walist.GetNext(pos));
		if (!apax)
			continue;
		
	
		preis += apax->get_preis().get_gesamtpreisVK3();
		
	}

	return preis;
}


double Ausgaben::getFormVK3Gesamtpreis()
{
	double preis = 0;
	for (POSITION pos = walist.GetHeadPosition(); pos; )
	{
		FormstueckeFittinge* formstueck = dynamic_cast<FormstueckeFittinge*>(walist.GetNext(pos));
		if (!formstueck)
			continue;

		formstueck->get_preis().calc_sonderkosten();
		preis += formstueck->get_preis().get_sonderkosten();
	}

	return preis;
}