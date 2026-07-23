#include "stdafx.h"


#include "Dbfreigabe.h"
#include "Fortschrittanzeige.h"
#include "GpFreigabe.h"
#include "ArbeitsplanGewindeSammelauftrag.h"


	#include "sammelauftrag.h"



BEGIN_MESSAGE_MAP(SammelAuftrag, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(SammelAuftrag, WaxPart, 1)

SammelAuftrag::SammelAuftrag()
{
	set_icon(IDI_SAMMELAUFTRAG);
	set_werkauftrag(new SammelWerkauftrag);
}


SammelAuftrag::~SammelAuftrag()
{
}

///Bearbeitungsklassen zurückliefern
void SammelAuftrag::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);

	if (WaxSys::isAV())
	{
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(ArbeitSammelEingabeForm));
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(MaterialSammelEingabeForm));
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(SonderkostenSammelEingabeForm));
	}
}

///Serialisierung
void SammelAuftrag::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

///Liefert eine Referenz auf das Kontextmenü zurück
CMenu* SammelAuftrag::get_menu()
{
	/* 
	Kontextmenü initialisieren
	Ein Sammelauftrag kann immer nur eine Art von Werkauträgen enthalten
	Ist bereits ein Werkauftrag eingefügt worden, können nur noch gleichartige Was über
	das Kontextmenü erzeugt werden.


	Hier kann leider kein dynamic_cast benutzt werden, sonst wäre WaxPrj.dll von waxui.dll
	abhängig. Waxui.dll ist aber schon von WaxPrj.dll abhängig.
	Das wäre dann eine (unerwünschte) Rekursion.
	*/
	CMenu* menu = WaxPart::get_menu();

	struct MENUITEMS
	{
		UINT id;
		const char* classname;
		bool enable;
	} menuItems[] = {
		{ WAXID_NEU_VERTEILER,				"VerteilerData"				   , true },
		{ WAXID_NEU_STATIONSVERTEILER,		"StationsverteilerData"		   , true }, 
		{ WAXID_NEU_CO2VERTEILER,			"COIIVerteilerData"			   , true }, 
		{ WAXID_NEU_MX1230MX200VERTEILER,	"CMx2001230VerteilerData"	   , true }, 
		{ WAXID_NEU_HANDELSLAENGE,			"HandelslaengeData"			   , true }, 
		{ WAXID_NEU_STRANGROHR,				"StrangrohrData"			   , true }, 
		{ WAXID_NEU_GEWINDEROHR,			"GewinderohrData"			   , true }, 
		{ WAXID_NEU_FORMSTUECKEUNDFITTINGE, "FormstueckeUndFittingeDaten"  , true }, 
		{ WAXID_NEUE_ENTNAHMELEITUNG,		"EntnahmeleitungData"		   , true }, 
		{ WAXID_NEUE_PUMPENPROBIERLEITUNG,	"PumpenprobierleitungData"	   , true }, 
		{ WAXID_NEUER_PUMPENVERTEILER,		"PumpenverteilerData"		   , true }, 
		{ WAXID_NEU_APAX,					"ApaxData"					   , true }, 
		{ 0,"", true }
	};
	
	WaxPart *firstitem = getFirstSubobject();
	if (firstitem)
	{ 
		CString classname = firstitem->GetRuntimeClass()->m_lpszClassName;
		for (int i = 0; menuItems[i].id; i++)
			menuItems[i].enable = menuItems[i].classname == classname ? true : false;
	}
	for (int i = 0; menuItems[i].id; i++)
	{
		menu->EnableMenuItem(menuItems[i].id, MF_BYCOMMAND | (menuItems[i].enable ? MF_ENABLED : MF_GRAYED));
	}
	return menu;
}

///Werkauftrag freigeben
/*
void SammelAuftrag::onFreigeben()
{
}
*/
bool SammelAuftrag::freigeben(bool sicherheitsabfrage, Fortschrittanzeige *fortschritt, DWORD dbhandle)
{

	if (isPreisanfrage())
	{
		AfxMessageBox(getStammdaten().getText("ERR_NOT_REIGABE_PA").c_str(), MB_ICONERROR | MB_OK);
		return true;
	}

	if (!checkFreigabe())
		return true;
	
	if (sicherheitsabfrage)
	{
		CString str; str = getStammdaten().getText("IDS_SAMMEL_FREI").c_str();
		if (AfxMessageBox(str, MB_ICONQUESTION | MB_YESNO) == IDNO)
			return true;
	}	
	// Wegen Afo Ruesten
	if (!WaxSys::isAV())
		onCreateAutoarbeit();


	bool result = true;
	TRY
	{
		if (!checkAVNachbearbeitung())
			return true;

		// CheckPlausi fuer alle Positionen
		POSITION pos;
		for (pos = get_subobjects().GetHeadPosition(); pos; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
			XASSERT(next);
			if (!next || next->get_deleted())
				continue;
			
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(next->get_werkauftrag());
			if (rohr)
			{
				if (rohr->get_elementList().IsEmpty())
					rohr->createModell();
			}

			CString err = next->get_werkauftrag()->checkPlausis();
			if (err.GetLength() > 0)
			{
				AfxMessageBox(err, MB_ICONERROR | MB_OK);
				return true;
			}
			// Pruefung Rohrstutzen auf Abschlusselement
			if (rohr)
			{
				CString err = rohr->check_rohrstutzen();			
				if (err.GetLength() > 0)
				{
					AfxMessageBox(err, MB_ICONERROR | MB_OK);
					return true;
				}
			}
		}

		map<string,string> posmap;
		// CheckPlausi fuer alle Positionen
		for (pos = get_subobjects().GetHeadPosition(); pos; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
			XASSERT(next);
			if (!next || next->get_deleted())
				continue;
			map<string, string>::iterator it;
			string tofind(next->get_werkauftrag()->get_posNr());
			it = posmap.find(tofind);
			if (it == posmap.end())
				posmap.insert(make_pair(next->get_werkauftrag()->get_posNr(), next->get_werkauftrag()->get_bezeichnung()));
			else
			{
				CString str; str = getStammdaten().getText("IDS_FREI_NICHT").c_str();
				AfxMessageBox(str, MB_ICONINFORMATION); 
				return true;
			}	
		}

		// In NL muss VK3-Flag von Unterposition uebernommen werden. 
		if (!WaxSys::isAV() && get_subobjects().GetSize()>0) 
		{
			WaxPart* first = getFirstSubobject();
			if (first)
			{
				get_werkauftrag()->get_leitkarte().set_vk3(first->get_werkauftrag()->get_leitkarte().get_vk3());
			}
		}

		// alle Leitkartendaten auf Positionen vererben
		vererbe_leitkarte();
		
		// Liefert das Unterobjekt mit der niedrigsten waid bzw. wenn alle waids Null sind, den ersten 
		// Unterauftrag
		WaxPart *testobj = get_testobject();

		
		// Prüfung, ob alle Eingabewerte ok sind
		// Plausis brauchen nur für einen Auftrag geprüft werden, da die Leitkartendaten
		// für alle Unterpositionen gleich sind
		DbFreigabe frei(testobj, dbhandle);
		if (!frei.check_freigabe_plausis())
			return true;

		// Der Sammelauftrag gibt alle Unteraufträge frei, nicht aber sich selbst.
		for (pos = get_subobjects().GetHeadPosition(); pos; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
			XASSERT(next);
			if (!next || next->get_deleted())
				continue;
			CString err = next->get_werkauftrag()->checkPlausis();
			if (err.GetLength() > 0)
			{
				AfxMessageBox(err, MB_ICONERROR | MB_OK);
				return false;
			}

			if (!WaxSys::isAV() && !next->get_werkauftrag()->checkSDMatNr())
			{
				CString str; str = getStammdaten().getText("IDS_SDMAT_UNGUELTIG").c_str();
				AfxMessageBox(str, MB_ICONERROR | MB_OK);
				
				return false;
			}

			//Fortschritt hochsetzen
			nextStepFortschritt(fortschritt, next->get_werkauftrag());

			// Normaler Auftrag
			DbFreigabe wafrei(next, dbhandle);
			if (!wafrei.freigeben())
			{ 
				CString str; str = getStammdaten().getText("IDS_FREIGABE_NICHT2").c_str();
				AfxMessageBox(str);

				MxString st = "Freigabefehler: " + str;
				LOG(st.getString());

				result = false;
				break;
			}

			if (next->get_werkauftrag()->get_waid())
			{

				//diese Abfrage kann wegfallen!!  prüfen


				// Bei jeder erfolgreichen Freigabe werden die Statusvariablen upgedated
				Server serv(Server::load, next->get_werkauftrag(), dbhandle);	
				if (!serv.updateStatus())
					result = false;

				//Zum Testen, weil die WAID in der NL nicht immer in der Datei gespeichert wird
				if (!WaxSys::isAV())
				{
					MXDate cur = MXDate::GetCurrentTime();
					MxString date = cur.Format("%d.%m.%Y %H:%M:%S");

					MxString st = MxString("Freigabe: WAID= {0} Datum:{1}").arg(next->get_werkauftrag()->get_waid()).arg(date);
					LOG(st.getString());
					
				}
			}
			else
			{
				MxString st = "Freigabefehler: Keine WAID";
				LOG(st.getString());
			}
		
			needUpdate = true;
		}
		WaxPart* sub = this->getLastSubobject();
		get_leitkarte().clone(sub->get_leitkarte());
		get_werkauftrag()->get_status().copy(sub->get_werkauftrag()->get_status());
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		result = false;
		
		MxString st = "Freigabefehler: " + e->ReportError();
		LOG(st.getString());
	}
	END_CATCH_ALL

	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);

	return result;
}


///Grafikdatei erzeugen
void SammelAuftrag::createGraphic()
{
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(next);
		if (next && !next->get_deleted())
		{
			next->createGraphic();
		}
	}
}


///Titel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
CString SammelAuftrag::getBezeichnung() const
{
	CString ret = get_werkauftrag_const()->get_bezeichnung();
	if (ret.GetLength())
		return ret;
	
	const WaxPart *firstitem = getFirstSubobject();
	CString bez = firstitem ? firstitem->get_werkauftrag_const()->get_stdbezeichnung() : "";
	if (WaxSys::isAV())
	{		
		if (get_werkauftrag_const()->get_status_const().get_wanr() > 0 && get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_nummer() > 0)
			ret.Format("%06lu, PA %i-%i, %s", get_werkauftrag_const()->get_status_const().get_wanr(), get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_nummer(), get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_position(), bez);
		else if (get_werkauftrag_const()->get_status_const().get_wanr() <= 0 && get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_nummer() > 0)
			ret.Format("PA %i-%i, %s", get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_nummer(), get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_position(), bez);
		else
			ret.Format("%06lu %s", get_werkauftrag_const()->get_status_const().get_wanr(), bez);
	}
	else
	{
		const Leitkarte& leit = get_werkauftrag_const()->get_leitkarte();

		if (get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_nummer() > 0)
			ret.Format("%I64d  %lu, PA %i-%i, %s", leit.get_sdAuftragsnr(), leit.get_sdPositionsnr(), leit.get_preisanfrage().get_nummer(), leit.get_preisanfrage().get_position(), bez);
		else
		{
			if(leit.get_IsChoosenIppeNumbers())
			{
				ret.Format("%s  %s %s", leit.get_ippeMatNr(), leit.get_ippeKnotenNr(), bez);
			}
			else
			{
				ret.Format("%I64d  %lu %s", leit.get_sdAuftragsnr(), leit.get_sdPositionsnr(), bez);
			}
		}
	}

	// Anzeigen wieviele Unterobjekte storniert wurden
	int storniert_count = 0;
	for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos; )
	{
		const WaxPart* const pt = dynamic_cast<const WaxPart* const>(get_subobjects_const().GetNext(pos));
		if (pt && pt->get_werkauftrag_const()->get_status_const().get_storniert())
			storniert_count++;
	}
	if (storniert_count)
	{
		CString tmp;
		tmp = getStammdaten().getText("IDS_STORNIERT").c_str();
		CString bez;
		bez.Format("%s (%i/%i): %s", tmp, storniert_count, get_subobject_count(), ret);
		return bez;
	}
	return ret;
}

///Ist der Menuepunkt Delete zu disablen
bool SammelAuftrag::iconFreigegeben()
{
	/*
	Ausserdem wird die Sichtbarkeit	auch als Indikator fürs Löschen benutzt. 
	Das ist eigentlich nicht Ziel dieser Funktion.
	todo: Aufdröseln in 2 Funktionen 
	*/

	// Es müssen alle Unterwerkaufträge überprüft werden. Ist auch nur einer dabei,
	// der bereits freigegeben wurde, wird false zurückgeliefert. 
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			if (pt->get_werkauftrag()->get_status().get_freigabedatum().m_dt == 0)
			{
			 	return true;
			}
		}
	}
	return false;
}

///Prüft, ob das Objekt gezeigt werden muss. Das ist der Fall, wenn nicht freigegeben wurde.
bool SammelAuftrag::initVisible()
{
	/*
	Ausserdem wird die Sichtbarkeit	auch als Indikator fürs Löschen benutzt. 
	Das ist eigentlich nicht Ziel dieser Funktion.
	todo: Aufdröseln in 2 Funktionen 
	*/
	// Es müssen alle Unterwerkaufträge überprüft werden. Ist auch nur einer dabei,
	// der bereits freigegeben wurde, wird false zurückgeliefert. 
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			if (pt->get_werkauftrag()->get_status().get_freigabedatum().m_dt != 0)
			 	return false;
		}
	}
	return true;
}



///Durchlaeuft die Sammelliste und errechnet das Gesamtgewicht und die maximale Laenge
void SammelAuftrag::setMaxLaengeUndGewicht()
{
	gesamtgewicht = 0.0;
	maxlaenge = 0;
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			Werkauftrag* werkauftrag = dynamic_cast<Werkauftrag*>(pt->get_werkauftrag());
			XASSERT(werkauftrag);
			if (!werkauftrag)
				continue;
		
			gesamtgewicht += werkauftrag->get_material().get_gewicht(0, true);
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
			
			if (rohr)
				gesamtgewicht += rohr->getGesGewicht();
			
			if (werkauftrag->getMaxLaenge() > maxlaenge)
				maxlaenge = werkauftrag->getMaxLaenge();
		}
	}
}

///Startet Fkt. fuers erzeugen der Afos, Sonderkosten und zum Pruefen auf Mindermenge
void SammelAuftrag::onCreateAutoarbeit()
{
	// Afos neu generieren
    createAutoArbeit();
	
	// Sokos neu generieren
	createAutoSoko(false);

	// Auf Mindermenge pruefen und ggbfs. Flag setzen 
	if (checkMindermenge())
		set_mindermenge();
}

///Setzt Flag fuer Mindermenge
void SammelAuftrag::set_mindermenge()
{
	WaxPart *pt = NULL;
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos;)
	{
		pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			if (pt->get_werkauftrag()->get_arbeit().hasAfo(16))
				break;
		}
	}
	XASSERT(pt);
	if (pt)
		pt->get_werkauftrag()->set_mindermenge(true);
}

///Kranzeit fuer den gesamten Auftrag ermitteln und setzen
void SammelAuftrag::setKranzeit()
{
	// Anzahl der Rohrnennweiten fuer die Ermittlung der Kranzeit
	CUIntArray anzahlNennweiten;
	int anzahl = 0;
	for (int i=0; i<4; i++)
		anzahlNennweiten.Add(0);

	// 1. Drurchlauf ohne Verlängerungsstücke (afo17), 2. mit (afo16)
	for (int j=0; j<2; j++)
	{
		anzahl = 0;

		for (int i=0; i<4; i++)
			anzahlNennweiten.SetAt(i, 0);
		POSITION pos;
		for (pos = get_subobjects().GetHeadPosition(); pos; )
		{
			WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
			if (pt && !pt->get_deleted())
			{
				Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
				if (rohr)
				{
					if (j==0)
					{
						// Es duerfen nur die Positionen beruecksichtigt werden, die auch geschweisst werden
						if (rohr->get_arbeit().hasArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz()))
						{
							rohr->setAnzahlNennweiten();
							for (int i=0; i<4; i++)
							{
								anzahlNennweiten.SetAt(i, anzahlNennweiten.GetAt(i)+rohr->get_anzahlNennweiten(i));
								anzahl += rohr->get_anzahlNennweiten(i);
							}
						}
					}
					else
					{
						// Es duerfen nur die Positionen beruecksichtigt werden, die auch geschweisst werden
						if (rohr->get_arbeit().hasAfo(16))
						{
							rohr->setAnzahlNennweiten(true);
							for (int i=0; i<4; i++)
							{
								anzahlNennweiten.SetAt(i, anzahlNennweiten.GetAt(i)+rohr->get_anzahlNennweiten(i));
								anzahl += rohr->get_anzahlNennweiten(i);
							}
						}
					}
					
				}
			}
		}

		// Wenn Anzahl 0, dann Rest nicht erforderlich
		if (anzahl>0)
		{
			// Kranzeit ermitteln
			Rohrleitung rohr;
			double kranzeit = rohr.calcKranzeit(anzahlNennweiten);
			
			// Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16/17 gesetzt.
			int rohrcount = 0;
			bool gesetzt = false;
			for (pos = get_subobjects().GetHeadPosition(); pos; )
			{
				int count = 0;
				WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
				if (pt && !pt->get_deleted())
				{
					Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
					if (rohr)
					{
						for (POSITION pos = rohr->get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos;)
						{
							Arbeitsfolge *afo  = (Arbeitsfolge*)rohr->get_arbeit().get_arbeitsfolgen().GetNext(pos);
							if (j==0)
							{
								if (afo && (afo->get_id() == 17 || afo->get_id() == 54))
								{
									// Nur bei erster Position, die anderen auf 0 setzen 
									if (gesetzt)
										afo->set_ruestzeit(0);
									else
										afo->set_ruestzeit(kranzeit);
									count++;
								}
							}
							else
							{
								if (afo && afo->get_id() == 16)
								{
									// Nur bei erster Position, die anderen auf 0 setzen 
									if (gesetzt)
										afo->set_ruestzeit(0);
									else
										afo->set_ruestzeit(kranzeit);
									count++;
								}

							}
							// Restliche Afos muessen nicht mehr abgefragt werden
							if (count == 1 || afo->get_id() == 54)
							{
								gesetzt = true;
								break;
							}
						}
					}
				}
				rohrcount++;
			}
		}
	}
}

///Entfernen doppelter Ruestzeiten
void SammelAuftrag::remove_doppelteRuestzeiten()
{
	/**
	Doppelte Ruestzeiten muessen wieder entfernt werden, da beim Zusammenfassen gleicher
	Afos nicht nur die Zeit und Anzahl addiert werden, sondern auch die Ruestzeit.
	Bezieht sich also nur auf die Afo Ruesten fuers Klinken. Dazu wird fuer den gesamten
	Sammelauftrag ermittelt wieoft umgeruestet werden muss.
	Es muss bei jedem neu einzustellenden Radius umgeruestet werden,
	nicht nur bei jeder neuen Nennweite.
    */

	// Todo: aufteilen in mehrere Fkt.

	CStringArray plaetze;
	CArray<int,int> radien;
	bool ruesten[4];
	int i;
	for (i=0; i<4; i++)
		ruesten[i] = false;

	POSITION pos;
	for (pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			///Erst die Zeit bei Afo 8 (Ruesten) in den Einzelpositionen auf 0 setzen 
			CObList &afos = pt->get_werkauftrag()->get_arbeit().get_arbeitsfolgen();
			for (POSITION afpos = afos.GetHeadPosition(); afpos;)
			{
				Arbeitsfolge *af = dynamic_cast<Arbeitsfolge *>(afos.GetNext(afpos));
				XASSERT(af);
				if (!af)
					continue;

				// Afo Ruesten fuers Klinken Zeit loeschen
				if (af->get_id() == 8)
					af->set_zeit(0.0);
				
				// Afo 16/17/54 haben diesen Arbeitsplan,
				// bei diesen Afos ist die Ruestzeit die Kranzeit
				if (af->get_arbpl() == Arbeitsplan::get_abpSchweissVorrichtplatz())
					continue;

				int i;
				// doppelte Ruestzeiten loeschen 
				for (i = 0; i < plaetze.GetSize(); i++)
					if (plaetze.GetAt(i) == af->get_arbpl())
						break;
				
				if (i != plaetze.GetSize())
				{
					af->set_ruestzeit(0.0);
				}
				else
					plaetze.Add(af->get_arbpl());
			}

			///dann Nennweiten, die geruestet werden muessen zusammenfassen
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
			if (rohr)
			{
				bool add = true;
				for (int j=0; j<rohr->get_radien().GetSize(); j++)
				{
					add = true;
					for (int i=0; i<radien.GetSize(); i++)
					{
						if (radien.GetAt(i) == rohr->get_radien().GetAt(j))
						{
							add = false;
							break;
						}
					}
					if (add)
						radien.Add(rohr->get_radien().GetAt(j));
				}
				
				for (int i=0; i<4; i++)
				{
					if (rohr->get_ruesten(i))
						ruesten[i] = true;
				}
			}
		}
	}

	// Wieviel mal muss (um)geruestet werden fuers Klinken
	int count = 0;
	for (i=0; i<4; i++)
	{
		if (ruesten[i]) 
			count++;
	}

	// Bei erster Unterposition die Afo 8 (Ruesten) mit Gesamtzeit neu setzen
	for ( pos = get_subobjects().GetHeadPosition(); pos;)
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			Verteiler* vert = dynamic_cast<Verteiler*>(getFirstSubobject()->get_werkauftrag());
			Stationsverteiler* statvert = dynamic_cast<Stationsverteiler*>(getFirstSubobject()->get_werkauftrag());
			Strangrohr* strang = dynamic_cast<Strangrohr*>(getFirstSubobject()->get_werkauftrag());

			if (radien.GetSize() > 0 && (vert || statvert || strang))
				getFirstSubobject()->get_werkauftrag()->get_arbeit().addAfo_Ruesten(radien.GetSize());
			break;
		}
	}
}

///Pruefung der Afo 16 auf Mindermenge
bool SammelAuftrag::checkMindermenge()
{
	// Afoliste zusammenpacken
	Ausgaben aus;
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos;)
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
			aus.get_wa().get_arbeit().merge(pt->get_werkauftrag()->get_arbeit());
	}
	
	// Mindermenge checken
	if (aus.get_wa().checkMindermenge())
		return true;	
	return false;
}


void SammelAuftrag::createAutoArbeit()
{

	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
		{
			pt->get_werkauftrag()->set_UseSammelauftrag(true);
			pt->createAutoArbeit();
			pt->get_werkauftrag()->set_UseSammelauftrag(false);
		}
	}

	WaxPart* sub = getFirstSubobject();
	if (sub)
	{ 
		if(sub->get_werkauftrag()->Get_Fertigungsstaette() == Werkauftrag::OLDESLOE)
		{

			POSITION pos = get_subobjects().GetHeadPosition();
			WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
			XASSERT(next);
			GewinderohrData* pWaxPartGewindeRohr = dynamic_cast< GewinderohrData*>(next);
			
			if(pWaxPartGewindeRohr)
			{
				ArbeitsplanGewindeSammelauftrag APGewindeSammelauftrag(this);
				APGewindeSammelauftrag.createExtraAutoArbeitSaegen();
			}
		}
	}

	// Afo Ruesten fuer den gesamten Auftrag ermitteln
	
	COIIVerteiler* pCOIIVerteiler = NULL;
	if(sub)
	{
		pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(sub->get_werkauftrag());
	}
	if(pCOIIVerteiler == NULL)
	{
		remove_doppelteRuestzeiten();
	}

	// Kranzeit fuer den gesamten Auftrag ermitteln und setzen
	setKranzeit();



}









void SammelAuftrag::createAutoSoko(bool deleteFixedSoko)
{
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (pt && !pt->get_deleted())
			pt->get_werkauftrag()->get_preis().calcAutoSoko(deleteFixedSoko);
	}
}

void SammelAuftrag::initPopUpMenu()
{
CMenu* menu = getPopUpMenu();

	
	if(WaxSys::isAV())
	{
		menu_items items[] =
		{
			ADD_MENU_ITEM(WAXID_NEU_GEWINDEROHR)
			ADD_MENU_ITEM(WAXID_NEU_HANDELSLAENGE)
			ADD_MENU_ITEM(WAXID_NEU_STATIONSVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_STRANGROHR)
			ADD_MENU_ITEM(WAXID_NEU_VERTEILER)
			{ID_SEPARATOR, ""},
			ADD_MENU_ITEM(WAXID_NEU_FORMSTUECKEUNDFITTINGE)
			ADD_MENU_ITEM(WAXID_NEUE_ENTNAHMELEITUNG)
			ADD_MENU_ITEM(WAXID_NEUE_PUMPENPROBIERLEITUNG)
			ADD_MENU_ITEM(WAXID_NEUER_PUMPENVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_APAX)
			{ID_SEPARATOR, ""},
			#pragma message( "-----------------if def Verteiler wieder herausnehmen!!-----")
			
			ADD_MENU_ITEM(WAXID_NEU_CO2VERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_MX1230MX200VERTEILER)
			
			{ID_SEPARATOR, ""},
			{ -1, "" },
		};
		appendMenuItems(menu, items);
	}
	else
	{

		menu_items items[] =
		{
			ADD_MENU_ITEM(WAXID_NEU_GEWINDEROHR)
			//ADD_MENU_ITEM(WAXID_NEU_HANDELSLAENGE)
			ADD_MENU_ITEM(WAXID_NEU_STATIONSVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_STRANGROHR)
			ADD_MENU_ITEM(WAXID_NEU_VERTEILER)
			{ID_SEPARATOR, ""},
			ADD_MENU_ITEM(WAXID_NEU_FORMSTUECKEUNDFITTINGE)
			ADD_MENU_ITEM(WAXID_NEUE_ENTNAHMELEITUNG)
			ADD_MENU_ITEM(WAXID_NEUE_PUMPENPROBIERLEITUNG)
			ADD_MENU_ITEM(WAXID_NEUER_PUMPENVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_APAX)
			{ID_SEPARATOR, ""},
			{ -1, "" },
		};
		appendMenuItems(menu, items);	
	}

	
	appendStandardMenuItems(menu);

}

bool SammelAuftrag::SammelCheckPlausiFertigungMagdeburg()
{

	POSITION pos = get_subobjects().GetHeadPosition();
	WaxPart *p = dynamic_cast<WaxPart*>(get_subobjects().GetAt(pos));
	while (p->get_deleted())
	{
		if (pos == NULL)
			break;
		p = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
	}
	
	CString strError;
	for(POSITION upos = get_subobjects().GetHeadPosition(); upos;)
	{
		WaxPart* wp = dynamic_cast<WaxPart*>(get_subobjects().GetNext(upos));
		//Strangrohr* wp = dynamic_cast<Strangrohr*>(get_subobjects().GetNext(upos));
		if (wp && !wp->get_deleted())
		{
			Strangrohr* p = (Strangrohr*)wp->get_werkauftrag();
			p->checkPlausiFertigungMagdeburg(strError);
			if(strError.GetLength())
			{
				break;
			}
		}
	}

	if(!strError.IsEmpty())
	{
		AfxMessageBox(strError);
		return false;
	}

	return true;
}

bool SammelAuftrag::doSync(bool checkFreigabe)
{
	if (checkFreigabe && is_freigegeben())
		return false;

	POSITION pos = get_subobjects().GetHeadPosition();
	WaxPart *p = dynamic_cast<WaxPart*>(get_subobjects().GetAt(pos));
	while (p->get_deleted())
	{
		if (pos == NULL)
			break;
		p = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
	}
	
	for(POSITION upos = get_subobjects().GetHeadPosition(); upos;)
	{
		WaxPart* wp = dynamic_cast<WaxPart*>(get_subobjects().GetNext(upos));
		if (wp && !wp->get_deleted())
			wp->synchronize(p);
	}
	return true;
}

bool SammelAuftrag::subobj_allowed() const
{
	/**
	Nur Gliederungen und Sammelaufträge können Unterobjekte enthalten
	*/
	return true;
}


///Prüfen, ob alle Unteraufträge mit oder ohne AV Nachbearbeitung sind
bool SammelAuftrag::checkAVNachbearbeitung()
{
	WaxPart *sub = getFirstSubobject();
	if (!sub)
		return false; // Leerer Sammelauftrag

	if (!sub->get_werkauftrag()->avNacharbeitsPruefung())
		return true;

	Rohrleitung *testrohr = dynamic_cast<Rohrleitung*>(sub->get_werkauftrag());
	if (!testrohr)
		return true;
	
	// der erste Auftrag liefert den Referenzwert
	const bool testcond =  testrohr->get_avnachbearbeitung();
	
	for (POSITION upos = get_subobjects().GetHeadPosition(); upos != NULL; )
	{
		WaxPart* n = dynamic_cast<WaxPart*>(get_subobjects().GetNext(upos));
		XASSERT(n);
		if (!n || n->get_deleted())
			continue;
		testrohr = dynamic_cast<Rohrleitung*>(n->get_werkauftrag());
		if (!testrohr  || (testrohr->get_avnachbearbeitung() != testcond))
		{
			CString msg, str; str = getStammdaten().getText("IDS_SAMMEL_NACH").c_str();
			msg.Format(str, testrohr->get_bezeichnung());
			AfxMessageBox(msg, MB_OK | MB_ICONERROR);
			return false;
		}
	}
	return true;
}

 
bool SammelAuftrag::sollUpdateStatus()
{
	/**
	Bei Sammelauftrag und Einzelauftrag wird Statusaktualisierung abgefragt, 
	wenn der Auftrag freigegeben ist aber noch keine WAnr hat und nicht heute freigegeben wurde.
	Und das ganze nur einmal (status_update_test = true und wenn nicht AV Version
	*/

	// Hier ueberschriben, damit bei Unterpos. nicht auch noch mal gefragt wird
	if (WaxPart::sollUpdateStatus())
	{
		for (POSITION upos = get_subobjects().GetHeadPosition(); upos != NULL; )
		{
			WaxPart* wp = dynamic_cast<WaxPart*>(get_subobjects().GetNext(upos));
			XASSERT(wp);
			if (!wp || wp->get_deleted())
				continue;
			wp->set_status_update_test(false);
		}
		return true;
	}
	return false;
}


WaxPart *SammelAuftrag::get_testobject()
{
	/**
	Liefert das Unterobjekt mit der niedrigsten waid bzw. wenn alle waids Null sind, den ersten 
	Unterauftrag
	*/
	map<int, WaxPart*> waidlst;
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(next);
		if (!next || next->get_deleted())
			continue;

		if (next->get_werkauftrag()->get_waid())
			waidlst.insert(make_pair(next->get_werkauftrag()->get_waid(), next));
	}
	if (waidlst.size())
		return waidlst.begin()->second;
	return dynamic_cast<WaxPart*>(get_subobjects().GetHead());
}

///Ist das Icon für nicht freigegeben zu modifizieren
bool SammelAuftrag::is_freigegeben() const
{
	/**
	Zeigt an, dass der Werkauftrag einen Status hat, auf den hingewiesen werden muss.
	Dies ist bis jetzt nur: Werkauftrag angelegt aber nicht freigegeben. 
	Überlagertes Icon ist IDI_ICON_NICHT_FREIGEGEBEN (Ausrufezeichen in linker oberer Ecke)
	Es werden auch alle Unterobjekte überprüft, dieses Flag vererbt sich in der Hierarchie nach 
	oben, d.h. Auch eine Gliederung erhält das nicht freigegeben Symbol, wenn auch nur ein Unterobjekt
	nicht freigegeben wurde.
	false := nicht freigegeben -> !-Zeichen setzen
	true := freigegeben       -> kein !-Zeichen
	*/	

	const Werkauftrag *werk = get_werkauftrag_const();
	
	if (!werk || get_deleted() || !get_subobjects_const().GetSize())
		return false;
	
	for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos; )
	{
		const WaxPart *pt = dynamic_cast<const WaxPart*>(get_subobjects_const().GetNext(pos));
		if (!pt)
			continue;
		if (!pt->is_freigegeben())
			return false;
	}
	return true;
}



Status::STATUS SammelAuftrag::get_status() const
{
	const Werkauftrag* werk = get_werkauftrag_const();
	if (!werk || get_deleted())
		return Status::NichtGesetzt;
	
	for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos; )
	{
		const WaxPart *pt = dynamic_cast<const WaxPart*>(get_subobjects_const().GetNext(pos));
		if (!pt)
			continue;
		//Status von erster Position holen
		return pt->get_status();
	}
	return Status::NichtGesetzt;
}


bool SammelAuftrag::setAfosAndMoveToFirstPos()
{
	WaxPart *sub = NULL;
	for (POSITION pos = get_subobjects().GetTailPosition(); pos != NULL; )
	{
		sub = dynamic_cast<WaxPart*>(get_subobjects().GetPrev(pos));
		XASSERT(sub);
		if (!sub || sub->get_deleted())
			continue;
		sub->get_werkauftrag()->get_arbeit().clean();
	}
	if(getFirstSubobject())
	{
		getFirstSubobject()->get_werkauftrag()->get_arbeit().copy(get_werkauftrag()->get_arbeit());
		getFirstSubobject()->get_werkauftrag()->get_arbeit().sortAfos();
	}
	

	return true;
}

bool SammelAuftrag::copyAfosToSammelauftrag(bool bWithClean)
{
	if(bWithClean)
	{
		this->get_werkauftrag()->get_arbeit().clean();
	}
	for (POSITION pos = this->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart *sub = dynamic_cast<WaxPart*>(this->get_subobjects().GetNext(pos));
		XASSERT(sub);
		if (!sub)
			return false;
		if (sub->get_deleted())
			continue;

		this->get_werkauftrag()->get_arbeit().merge(sub->get_werkauftrag()->get_arbeit());
		//sub->get_werkauftrag()->get_arbeit().clean();
	}
	this->get_werkauftrag()->get_arbeit().sortAfos();

	return true;
}