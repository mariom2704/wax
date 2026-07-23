
#include "stdafx.h"

#include "resource.h"


#include "drucklistenverwaltung.h"


#include "..\waxgl\WaxGrafik.h"
#include "..\rohrkonst\elementlist.h"
#include "EditProfile.h"
#include "drucklistezuschnitt.h"
#include "drucklisteverzinkung.h"
#include "drucklistestationsverteiler.h"
#include "DrucklisteCOIIMx.h"
#include "DrucklisteWap.h"
#include "drucklistestrangrohr.h"
#include "WpPreisanfrage.h"
#include "DrucklistePreisanfrage.h"
#include "GewinderohrrucklisteZuschnitt.h"

#include <mxutil/strloader.h>
using namespace MxUtil;




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;

//lastAusgabe, ID der zu erzeugenden Ausgabe
int DrucklistenVerwaltung::lastAusgabe = 0;

//currentAusgabe, ID der zu erzeugenden Ausgabe
int DrucklistenVerwaltung::currentAusgabe = 0;

DrucklistenVerwaltung::DrucklistenVerwaltung(WaxPart* pt, bool bWidthJScript) : waxpart(pt), m_bWidthJScript(bWidthJScript)
{
	init(pt);
}

void DrucklistenVerwaltung::init(WaxPart* pt)
{
	pDrucklisteRohrwaCollector = new Rohrleitung;
	set_werkauftrag(NULL);
	anzahlRohre = 0;
	anzahlRohrstuecke = 0;
	anzahlElemente = 0;

	dn2zoll = (WaxSys::para_int(GF_DN2ZOLL) == 0) ? false : true;
	int dn[] = { 32, 40, 50, 65, 80, 100, 125 };
	for (int i=0; i<8; i++)
	{
		anzahlStutzen.Add(0);
		anzahlMuffen.Add(0);
		nennweiten.Add(dn[i]);	
	}
	if (pt)
	{
		
		set_werkauftrag(pt->get_werkauftrag());
		set_walist(pt->get_subobjects());
	}
}

//Destruktor
DrucklistenVerwaltung::~DrucklistenVerwaltung()
{
	for (POSITION pos = elelist.GetHeadPosition(); pos;)
	{
		delete elelist.GetNext(pos);
	}
	elelist.RemoveAll();
	delete pDrucklisteRohrwaCollector;
}

///Erzeugt die Liste mit den m�glichen Drucklisten
void DrucklistenVerwaltung::init_menu()
{
	//Liste mit Listenids (listen) und Listennamen (listennamen) initialisieren
	//listen enth�lt die f�r den jeweiligen Werkauftrag g�ltigen Drucklisten-IDs . 
	menuitems.clear();
	menuitems.push_back(DRUCKVORSCHAU_ABBRECHEN);



	WpPreisanfrage *anfrage = dynamic_cast<WpPreisanfrage*>(waxpart);
	if (anfrage)
	{
#ifndef __WAX_FUER_DRITTE__
		menuitems.push_back(PREISANFRAGE);
#endif
		//Wenn Preisanfrage, dann keine weiteren Men�punkte
		return;
	}



	// kein Objekt in walist 
	if (walist.IsEmpty())
		return;
	
	// Wenn Unterauftr�ge in der Liste sind, wird der letzte Auftrag benutzt.
	// Am Kopf k�nnte ein Sammelauftrag stehen, dessen Werkauftrag leer ist
	werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetTail());
	if (!werkauftrag)
		return;

	if (init_material_menu())
		return; /// Falls nur Materialbestellung

	werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetTail());
	if (!werkauftrag)
		return;

	menuitems.push_back(GRAPHIKDRUCK);


#ifndef __WAX_FUER_DRITTE__
	if (werkauftrag->isGewinderohr())
	{
		menuitems.push_back(ROHRLEITUNGSPLAN);
		menuitems.push_back(ELEMENTPLAN);
	}

	menuitems.push_back(SEPARATOR);
	menuitems.push_back(MATERIALLISTE_GESAMT);
	
	if (WaxSys::isAV())
	{
		menuitems.push_back(ARBEITSPLAN);
		menuitems.push_back(SEPARATOR);
			
		menuitems.push_back(VK3_ABRECHNUNG_KURZ);
		menuitems.push_back(VK3_ABRECHNUNG_LANG);

		menuitems.push_back(SEPARATOR);
		menuitems.push_back(WAP_VOLLKOSTEN);
		
		if (WaxSys::isAvAdmin())
			menuitems.push_back(WAP_GRENZKOSTEN);

		if (werkauftrag->get_beschichtung().isVerzinkt())
		{
			menuitems.push_back(SEPARATOR);
			menuitems.push_back(VERZINKUNG_PREISE);
		}
	}
	else
	{
		menuitems.push_back(VK3_ABRECHNUNG_KURZ);
	}
	
	if (werkauftrag->isGewinderohr())
	{
		menuitems.push_back(SEPARATOR);

		menuitems.push_back(GEWINDEROHRLISTE);

		if (WaxSys::isAV())
		{
			menuitems.push_back(GEWINDEROHRE_LAENGE);
			DrucklisteZuschnitt z(html, waxpart, currentAusgabe);
			z.add_drucklisten(menuitems);
		
		}
	}
	else
	{
		if (WaxSys::isAV())
		{
			menuitems.push_back(SEPARATOR);
			if(!dynamic_cast<COIIVerteiler*>(werkauftrag))
			{
				menuitems.push_back(SAEGENUTANLAGELISTE);
			}
			
			DrucklisteZuschnitt z(html, waxpart, currentAusgabe);
			z.add_drucklisten(menuitems);
		}	
	}

	if (werkauftrag->isStrangrohr())
	{
		menuitems.push_back(SEPARATOR);
		menuitems.push_back(STRANGROHRLISTE);
	} 

#endif
	if (werkauftrag->isStationsverteiler())
	{
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
		if (!rohr->get_avnachbearbeitung())
		{
			menuitems.push_back(SEPARATOR);
			menuitems.push_back(STATIONSVERTEILER);
		}
	} 
	else if (dynamic_cast<COIIVerteiler*>(werkauftrag))
	{	
		menuitems.push_back(SEPARATOR);
		menuitems.push_back(COMXVERTEILER_MONTAGE_LISTE);
		
	} 
	else if (werkauftrag->isPumpenprobierleitung())
	{
		menuitems.push_back(SEPARATOR);
		menuitems.push_back(PUMPENPROBIERLEITUNG);
	}
	else if (werkauftrag->isPumpenverteiler())
	{
		menuitems.push_back(SEPARATOR);
		menuitems.push_back(PUMPENVERTEILER);
	} 
	else if (werkauftrag->isEntnahmeleitungDWB())
	{
		menuitems.push_back(SEPARATOR);
		menuitems.push_back(ENTNAHMELEITUNGDWB);
	} 
}

string DrucklistenVerwaltung::get_name(int ausgabe_id, bool menu) const
{
	/**
	Liefert den Namen (f�r Men�eintrag bei menu = true oder Ausgabe) f�r eine spezifische Druckliste. 
	*/

#define ADDMENUITEM(aa) { ##aa, "IDS_"#aa"_MENU", "IDS_"#aa },
	int i = 0;
	static const struct NAMELIST
	{
		int id;
		string menu;
		string name;
	} listen[] =
	{
		ADDMENUITEM(VK3_ABRECHNUNG_KURZ)			
		ADDMENUITEM(VK3_ABRECHNUNG_LANG)			
		ADDMENUITEM(GRAPHIKDRUCK)					
		ADDMENUITEM(WAP_GRENZKOSTEN)			
		ADDMENUITEM(WAP_VOLLKOSTEN)			
		ADDMENUITEM(ARBEITSPLAN)					
		ADDMENUITEM(MATERIALLISTE_GESAMT)			
		ADDMENUITEM(ZUSCHNITTSLISTE_38030_HANDSW)	
		ADDMENUITEM(ZUSCHNITTSLISTE_38030_ANLAGE)	
		ADDMENUITEM(ZUSCHNITTSLISTE_32060)			
		ADDMENUITEM(ZUSCHNITTSLISTE_32020)			
		ADDMENUITEM(ZUSCHNITTSLISTE_32070)			
		ADDMENUITEM(ROHRLEITUNGSPLAN)				
		ADDMENUITEM(ELEMENTPLAN)					
		ADDMENUITEM(SAEGENUTANLAGELISTE)			
		ADDMENUITEM(STATIONSVERTEILER)	
		ADDMENUITEM(COMXVERTEILER_MONTAGE_LISTE)
		ADDMENUITEM(STRANGROHRLISTE)				
		ADDMENUITEM(PUMPENPROBIERLEITUNG)			
		ADDMENUITEM(PUMPENVERTEILER)				
		ADDMENUITEM(ENTNAHMELEITUNGDWB)				
		ADDMENUITEM(GEWINDEROHRLISTE)				
		ADDMENUITEM(GEWINDEROHRE_LAENGE)			
		ADDMENUITEM(GEWINDEROHRE_AB_10_STUECK)	
		ADDMENUITEM(GEWINDEROHRE_BIS_10_STUECK)
		ADDMENUITEM(GEWINDEROHRE_20_STUECK_LAENGE_50_500)				
		ADDMENUITEM(PROFILLISTE_38010)				
		ADDMENUITEM(PROFILLISTE_38040)				
		ADDMENUITEM(ZUSCHNITT_LEER)					
		ADDMENUITEM(DRUCKVORSCHAU_ABBRECHEN)		
		ADDMENUITEM(VERZINKUNG_PREISE)	
		ADDMENUITEM(PREISANFRAGE)
		ADDMENUITEM(ZUSCHNITTSLISTE_EK)		
		{ 0, "", "" },
	};
	for (i = 0; listen[i].id; i++)
	{
		if (listen[i].id == ausgabe_id)
			break;
	}
	if (!listen[i].id)
		return "??????";
	CString str;
	str = getStammdaten().getText(menu ? listen[i].menu : listen[i].name).c_str();
	return (LPCSTR)str;
}

/**
Pr�ft auf g�ltigen Wert von currentAusgabe. Bei G�ltigkeit wird 
currentAusgabe zur�ckgeliefert, sonst die ID der ersten Druckliste.
*/
bool DrucklistenVerwaltung::check_currentAusgabe() const
{
	/**
	currentAusgabe muss auch in ausgaben vorhanden sein, sonst default 
	*/
	for (vector<int>::const_iterator it = menuitems.begin(); it != menuitems.end(); it++)
		if (*it == currentAusgabe)
			return true;
	return false;
}

bool DrucklistenVerwaltung::set_default_druckliste()
{
	/**
	ausgaben Liste muss vorher gesetzt werden durch initmenuitems.
	Liefert false, wenn keine Druckliste ausgew�hlt werden kann.
	*/
	for (vector<int>::iterator it = menuitems.begin(); it != menuitems.end(); it++)
	{
		if (*it != 0 && *it != SEPARATOR && *it != DRUCKVORSCHAU_ABBRECHEN && *it != GRAPHIKDRUCK)
		{
			currentAusgabe = *it;
			return true;
		}
	}
	// Kein vern�nftiger Men�punkt vorhanden, bzw. gar kein Men� (Projekt, Gliederung)
	currentAusgabe = DRUCKVORSCHAU_ABBRECHEN;
	return false;
}

//Erzeugt die gew�nschte Ausgabedatei
const CString DrucklistenVerwaltung::makeAusgabe(int id)
{
	UNREFERENCED_PARAMETER(id);
	reset();
	init_menu();

	// ausgewaehlte Ausgabe checken und evtl. Standarddruckliste setzen
	if (!check_currentAusgabe())
		set_default_druckliste();
	
	// keine Ausgabe ausgewaehlt - dann Defaultausgabe
	if (!currentAusgabe)
	{
		createDefaultDruckliste();
		return htmlFiles;
	}
	
	set_dn2zoll(currentAusgabe);
	
	setMaxLaengeUndGewicht();

	// Header generieren
	if(m_bWidthJScript)
	{
		JScriptVisualisationAndPrinting.Initialize();	
		JScriptVisualisationAndPrinting.write_header(cur_werkauftrag(), get_name(currentAusgabe, false).c_str());//insertLeitkarte(cur_werkauftrag()->get_leitkarte(), cur_werkauftrag()->get_status(), bez, ziel);
	}
	else
	{
		write_header();
	}

	// Wenn Auftrag �ber Verteileranlage, dann in Saegelisten vermerken (Leitkarte - Ziel)
	if (currentAusgabe == SAEGENUTANLAGELISTE || currentAusgabe == ZUSCHNITTSLISTE_32020)
	{
		for (int i = 0; i < walist.GetCount(); i++, walist.AddTail(walist.GetHead()), walist.RemoveHead())
		{
			if ((cur_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(cur_werkauftrag())) && (walist.GetCount() > 1))
				continue;
	
			const Rohrleitung* rohr = dynamic_cast<const Rohrleitung*>(cur_werkauftrag());
			if (rohr && rohr->isVerteiler() && rohr->checkAnlage())
			{
				ziel = getStammdaten().getText("IDS_VERTEILERROHRSTRASSE").c_str();
				break;
			}
		}
	}

	CString bez = get_name(currentAusgabe, false).c_str();
	if (currentAusgabe == PREISANFRAGE)
	{
		CString tmp;
		tmp.Format("%s %i", bez, cur_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer());
		bez = tmp;
	}

	

	if(m_bWidthJScript)
	{		
		JScriptVisualisationAndPrinting.insertLeitkarte(cur_werkauftrag()->get_leitkarte(), cur_werkauftrag()->get_status(), bez, ziel);
	}
	else
	{
			html.insertLeitkarte(cur_werkauftrag()->get_leitkarte(), cur_werkauftrag()->get_status(), bez, ziel);
	}
	
	// Beim Graphikdruck Gesamtgewicht und max. Laenge vorneweg fuer Versand
	if (currentAusgabe == GRAPHIKDRUCK	   || currentAusgabe == STRANGROHRLISTE  || 
		currentAusgabe == GEWINDEROHRLISTE || currentAusgabe == STATIONSVERTEILER || currentAusgabe == COMXVERTEILER_MONTAGE_LISTE ||
		currentAusgabe == PUMPENPROBIERLEITUNG || currentAusgabe == PUMPENVERTEILER) 
	{
		if(m_bWidthJScript)
		{
			JScriptVisualisationAndPrinting.insertLaengeGewichtBarcode(maxlaenge, gesamtgewicht, cur_werkauftrag()->get_leitkarte().get_kcAuftragsnr(), cur_werkauftrag()->get_leitkarte().get_kcPositionsnr(), "IDS_GESAMTGEWICHT_KG");
		}
		else
		{
			html.insertLaengeGewichtBarcode(maxlaenge, gesamtgewicht, cur_werkauftrag()->get_leitkarte().get_kcAuftragsnr(), cur_werkauftrag()->get_leitkarte().get_kcPositionsnr(), "IDS_GESAMTGEWICHT_KG");
		}
	}

	if(m_bWidthJScript)
	{
		JScriptVisualisationAndPrinting.insertOberflaeche(cur_werkauftrag()->get_beschichtung(), cur_werkauftrag()->get_zeugnis(), 
						   cur_werkauftrag()->get_sichtbereich(), cur_werkauftrag()->get_KollilisteExport());

	}
	else
	{
		html.insertOberflaeche(cur_werkauftrag()->get_beschichtung(), cur_werkauftrag()->get_zeugnis(), 
							   cur_werkauftrag()->get_sichtbereich(), cur_werkauftrag()->get_KollilisteExport());
	}

	// Fertigungszeiten einfuegen -> @todo ab in die DrucklisteGrafik Klasse
	if ((currentAusgabe == GRAPHIKDRUCK || currentAusgabe == STRANGROHRLISTE) && WaxSys::isAV())
	{
		bool insert = true;
		for (int i = 0; i < walist.GetCount(); i++, walist.AddTail(walist.GetHead()), walist.RemoveHead())
		{
			if ((cur_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(cur_werkauftrag())) && (walist.GetCount() > 1))
				continue;
			if (cur_werkauftrag()->isGewinderohr() || dynamic_cast<FormstueckeFittinge*>(cur_werkauftrag()))
			{
				insert = false;
				break;
			}
			pDrucklisteRohrwaCollector->get_arbeit().merge(cur_werkauftrag()->get_arbeit());
		}
		if (insert)
		{
			Kostenstellen k;
			if(m_bWidthJScript)
			{
				JScriptVisualisationAndPrinting.insertZeiten(pDrucklisteRohrwaCollector->get_arbeit().getZeitArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz()), pDrucklisteRohrwaCollector->get_arbeit().getZeitKostenstelle(k.getNr(Kostenstellen::BESCHICHTUNG)));
			}
			else
			{
				html.insertZeiten(pDrucklisteRohrwaCollector->get_arbeit().getZeitArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz()), pDrucklisteRohrwaCollector->get_arbeit().getZeitKostenstelle(k.getNr(Kostenstellen::BESCHICHTUNG)));
			}
		}
	}
	
	create_drucklist(waxpart);
	return htmlFiles;
}

/**
Erzeugt die gew�hlte Druckausgabe
*/
void DrucklistenVerwaltung::create_drucklist(WaxPart* curwaxpart)
{
	switch (currentAusgabe)
	{
		case ZUSCHNITT_LEER:
			curwaxpart = NULL; 
		case ZUSCHNITTSLISTE_32060:
		case ZUSCHNITTSLISTE_32020:
		case ZUSCHNITTSLISTE_32070:
		case ZUSCHNITTSLISTE_EK:
		case ZUSCHNITTSLISTE_38030_HANDSW:
		case ZUSCHNITTSLISTE_38030_ANLAGE:
			{
				DrucklisteZuschnitt z(html, curwaxpart, currentAusgabe);
				z.create();
			}
			break;

		case VERZINKUNG_PREISE:
			{
				DrucklisteVerzinkung verzinkung(html, curwaxpart);
				verzinkung.create();
			}
			break;

		case STATIONSVERTEILER:
			{
				DrucklisteStationsverteiler stat(html, curwaxpart);
				stat.create();
			}
			break;

		case COMXVERTEILER_MONTAGE_LISTE:
			{
				DrucklisteCOIIMx COIIMx(html, curwaxpart);
				COIIMx.create();
			}
			break;

		case STRANGROHRLISTE:
			{
				DrucklisteStrangrohr str(html, curwaxpart);
				str.create();
			}
			break;
		case WAP_VOLLKOSTEN:
			{
				DrucklisteWap wap(html, curwaxpart, GpWapPreis::Vollkosten);
				wap.createWap();
			}
			break;
		case WAP_GRENZKOSTEN:
			{
				DrucklisteWap wap(html, curwaxpart, GpWapPreis::Grenzkosten);
				wap.createWap();
			}
			break;
		case MATERIALLISTE_GESAMT:
			{
				DrucklisteWap matges(html, curwaxpart, GpWapPreis::Vollkosten);
				matges.createMaterialliste(false);
			}
			break;
		case ARBEITSPLAN:
			{
				DrucklisteWap wap(html, curwaxpart, GpWapPreis::Vollkosten);
				wap.createArbeitsplan(false);
			}
			break;
		case PREISANFRAGE:
			{
				DrucklistePreisanfrage pa(html, curwaxpart);
				pa.create();
			}
			break;
		case GEWINDEROHRE_AB_10_STUECK:
		case GEWINDEROHRE_BIS_10_STUECK:
		case GEWINDEROHRE_20_STUECK_LAENGE_50_500:
		case GEWINDEROHRE_LAENGE:
			{
				GewinderohrrucklisteZuschnitt GewinderohrZuschnitt(html, curwaxpart, &walist, currentAusgabe, this);
				GewinderohrZuschnitt.create();
				
			}
			break;
		case GEWINDEROHRLISTE:
			{
				GewinderohrrucklisteZuschnitt GesGewinderohrListe(html, curwaxpart, &walist, currentAusgabe, this);
				GesGewinderohrListe.createGewinderohrliste();
			}
			break;

		default: 
			// @todo: Alle Drucklisten in eigene Klassen auslagern bis diese beiden Methoden �berfl�ssig sind 
			initListen();
			writeList();
			break;
	}
}


void DrucklistenVerwaltung::initListen()
{
	for (int i = 0; i < walist.GetCount(); i++, walist.AddTail(walist.GetHead()), walist.RemoveHead())
	{
		if ((cur_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(cur_werkauftrag())) && (walist.GetCount() > 1))
			continue;

	
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
		switch (currentAusgabe)
		{
			case VK3_ABRECHNUNG_KURZ:
			case VK3_ABRECHNUNG_LANG:
				if (rohr)
				{
					createVK3Abrechnungdata();
					break;
				}
				break;
			case STRANGROHRLISTE:
				{
					set_matlist(cur_werkauftrag()->get_material().get_matlist(), currentAusgabe);
					Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
					if (!rohr) 
						break;
					
					if (rohr->isVerteiler() && rohr->checkAnlage())
						set_stklist(rohr->get_stuecklisteSap());	
					else
						set_stklist(rohr->get_stueckliste());	
				}
				break;
			
			case GEWINDEROHRLISTE:
				createGewinderohrdata();
				break;
		}
	}
}

//Generierung der ausgewaehlten Liste
void DrucklistenVerwaltung::writeList()
{
	std::string bemerkung;

	// in den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
	// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
	int counter = 0;

	CObject *tmp = walist.GetHead();
	Werkauftrag *ersterWerk = dynamic_cast<Werkauftrag*>(tmp);
	XASSERT(ersterWerk);
	if (ersterWerk)
		bemerkung = (LPCSTR)ersterWerk->get_bemerkung();

	std::string ueberschrift = getStammdaten().getText("IDS_BEMERKUNG");
	bool twoquali = check2Rohrquali(walist);		
	bool first = true;

	CString  Scheibe;
	for (POSITION pos = walist.GetHeadPosition(); pos != NULL; )
	{
		const Strangrohr* strang = dynamic_cast<const Strangrohr*>(walist.GetNext(pos));

		if(strang )
		{
			if(first == true)
			{
				first = false;
			}
			else
			{
				if(Scheibe.IsEmpty())
				{
					Scheibe = strang->get_stdbemerkung();
				}
			}
		}
	}
		
	first = true;
	for (int i = 0; i < walist.GetCount(); i++, walist.AddTail(walist.GetHead()), walist.RemoveHead())
	{
		// Der erste Werkauftrag kann ein Sammelauftrag sein. 
		if (cur_werkauftrag()->isEmpty() && currentAusgabe != GRAPHIKDRUCK && !dynamic_cast<ApaxWerkauftrag*>(cur_werkauftrag())) 
			continue;

		if(dynamic_cast<SammelWerkauftrag*>(cur_werkauftrag()) && walist.GetCount() > 1)
			continue;

		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());

		
		if (first)
		{
			string stdbemerkung;
			if (currentAusgabe == GRAPHIKDRUCK	|| currentAusgabe == ENTNAHMELEITUNGDWB
		          || currentAusgabe == PUMPENPROBIERLEITUNG)
			{
				stdbemerkung = (LPCSTR)cur_werkauftrag()->get_stdbemerkung() + Scheibe;
			}
			if(m_bWidthJScript)
			{
				JScriptVisualisationAndPrinting.insert_bemerkung(bemerkung, ueberschrift, stdbemerkung);
			}
			else
			{
				html.insert_bemerkung(bemerkung, ueberschrift, stdbemerkung);
			}

			if (rohr)
			{
				
				if (rohr->get_typ() == Rohrleitung::LEERTYP)
				{
					if (AfxMessageBox("Soll der Anlagentyp auf Wasser gesetzt werden?", MB_YESNO | MB_ICONQUESTION) == IDYES)
						rohr->get_leitkarte().set_prj_produktlinie(11);
				}
				if(m_bWidthJScript)
				{
					JScriptVisualisationAndPrinting.insertQuali(rohr->get_rohrQuali(), twoquali, rohr->get_dnvl(), 0, rohr->get_typ());
				}
				else
				{
					html.insertQuali(rohr->get_rohrQuali(), twoquali, rohr->get_dnvl(), 0, rohr->get_typ());
				}

			}

			first = false;
		}
				
		bool trenn = false;
		switch (currentAusgabe)
		{
			case GRAPHIKDRUCK:
			{			
				createGraphikdruck(); 
			}
			break;
			case ELEMENTPLAN:
				createElementplan(); 
				trenn = true;
				break;
			case ROHRLEITUNGSPLAN:
				createRohrleitungsplan(); 
				trenn = true;
				break;
			case SAEGENUTANLAGELISTE:
				createSaegenutanlageliste(); 
				break;
			case PUMPENPROBIERLEITUNG:
				createPumpenprobierleitung();
				trenn = true;
				break;
			case PUMPENVERTEILER:
				createPumpenverteiler();
				break;
			case ENTNAHMELEITUNGDWB:
				createEntnahmeleitungDWB();
				trenn = true;
				break;
			case PROFILLISTE_38010:
			case PROFILLISTE_38040:
				createProfilliste(currentAusgabe);
				break;
		}
		
		// nicht beim ersten und letzten Werkauftrag einf�gen
		if ((i && i < walist.GetCount()-1) && trenn)
		{
			html.insertTrennlinie();
		}
		
		counter++; 
	}		
	
	// Ausgabe von Einzellisten generiert aus tempor�ren Daten die in initListen initialisiert wurden
	switch (currentAusgabe)
	{
		case VK3_ABRECHNUNG_KURZ:
		case VK3_ABRECHNUNG_LANG:
			// Liefer- und Freigabedatum und Beschichtung, Bonus und vk3-Status kopieren 
			pDrucklisteRohrwaCollector->get_beschichtung().copy(cur_werkauftrag()->get_beschichtung());
			pDrucklisteRohrwaCollector->get_leitkarte().set_wunschliefertermin(cur_werkauftrag()->get_leitkarte().get_wunschliefertermin());
			pDrucklisteRohrwaCollector->get_status().set_freigabedatum(cur_werkauftrag()->get_status().get_freigabedatum());
			
			// Listen sortieren
			pDrucklisteRohrwaCollector->sortVk3Listen(pDrucklisteRohrwaCollector->get_vk3elementeListe());
			pDrucklisteRohrwaCollector->sortVk3Listen(pDrucklisteRohrwaCollector->get_passlaengenListe());
			pDrucklisteRohrwaCollector->sortVk3Listen(pDrucklisteRohrwaCollector->get_pulverlaengenListe());

			setVK3Gesamtpreis();
			createVK3Abrechnung(); 
			break;
	}
}

/**
An diese Ausgabe wird der Rest der Druckliste angeh�ngt.
*/
void DrucklistenVerwaltung::write_header()
{
	listname = get_name(currentAusgabe, false).c_str();
	CString wa, str; 

	if (WaxSys::isAV())
	{
		str = getStammdaten().getText("IDS_VON_WA").c_str();
		wa.Format(str, cur_werkauftrag()->get_status().get_wanr(), 
					   cur_werkauftrag()->get_status().get_mbtermin().Format(VAR_DATEVALUEONLY));
	}
	else
	{
		if(cur_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers())
		{
				str = getStammdaten().getText("IDS_VON_SD").c_str();
			wa.Format(str, cur_werkauftrag()->get_leitkarte().get_ippeMatNr(),
					   cur_werkauftrag()->get_leitkarte(). get_ippeKnotenNr());
		}
		else
		{
			str = getStammdaten().getText("IDS_VON_SD").c_str();
			wa.Format(str, cur_werkauftrag()->get_leitkarte().get_sdAuftragsnr(),
					   cur_werkauftrag()->get_leitkarte().get_sdPositionsnr());
		}
	}

	listname = get_name(currentAusgabe, false).c_str();
	listname += wa; 

	html.get_file().set_title(listname);
	htmlFiles = html.get_file().GetFilePath();
}

//Liefert eine Defaultdatei zur�ck, die angezeigt wird, wenn die gew�nschte 
//Druckliste nicht verf�gbar ist, bzw. wenn keine Druckliste verf�gbar ist.
void DrucklistenVerwaltung::createDefaultDruckliste( )
{
	/*
	Nur bei einem einzelnen Werkauftrag in der walist wird eine Meldung erzeugt. Bei einem Sammelauftrag
	darf die Meldung nicht erzeugt werden.
	*/
	if (walist.GetCount() == 1)
	{ 
		CString tit;
		tit = getStammdaten().getText("IDS_KEINE_DRUCKLISTE_VERFUEGBAR").c_str();
		html.get_file().setTitle(tit);
	}
}

//Fuegt die Graphik in eine Druckliste ein
bool DrucklistenVerwaltung::insertGrafic()
{
	/*
	Grafikdatei erzeugen und in Html-Datei einf�gen
	*/
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
	//ApaxWerkauftrag* apax = dynamic_cast<ApaxWerkauftrag*>(cur_werkauftrag());
	if (rohr && 
		(rohr->get_anhang()->get_filename().IsEmpty() || !rohr->get_anhang()->get_showAnhang()))
	{
		if (rohr->get_elementList().isEmpty())
			rohr->createModell();

		string grafic_file = WaxGrafik::Creator::create(rohr);

		if(m_bWidthJScript)
		{
			JScriptVisualisationAndPrinting.insertDwfOrSvgLink( grafic_file);
		}
		else
		{
			html.get_file().insertDWFLink(grafic_file);
		}
	}
	else 
	{
		if (!cur_werkauftrag()->get_anhang()->get_data())
			return false;

		//Endung ermitteln
		CString end = cur_werkauftrag()->get_anhang()->get_filename().Right(3);

		
		string fn = WaxSys::GetTempFilename(end.GetBuffer());
		WaxSys::add_tmpfile(fn.c_str());

		fstream tmpfile;
		tmpfile.open(fn.c_str(), ios_base::out | ios_base::trunc | ios_base::binary);
		if (tmpfile.is_open())
			tmpfile.write((const char*)cur_werkauftrag()->get_anhang()->get_data(), cur_werkauftrag()->get_anhang()->get_len());

		if(m_bWidthJScript)
		{			
			JScriptVisualisationAndPrinting.insertDwfOrSvgLink( fn);
		}
		else
		{
			html.get_file().insertDWF(fn);
		}
		
	}
	

	if (WaxSys::isAV() && !cur_werkauftrag()->get_posNr().IsEmpty())
	{
		//Barcode f�r Positionsnummer erzeugen
		if(m_bWidthJScript)
		{
			JScriptVisualisationAndPrinting.createBarcode(cur_werkauftrag(), 100, 20, cur_werkauftrag()->get_posNr().GetString(), "LEFT");
		}
		else
		{
			html.get_file().WriteString(html.get_file().createBarcode(100, 20, cur_werkauftrag()->get_posNr().GetString(), "LEFT"));
		}
	}
	else
    {
		JScriptVisualisationAndPrinting.createBlankJPG(cur_werkauftrag());
	}

	if (WaxSys::para_int(TRENNLINIE))
		html.get_file().insertTrennlinie();
	
	return true;
}



//Erfast die Daten fuer die VK3 - Abrechnung
void DrucklistenVerwaltung::createVK3Abrechnungdata()
{

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;
	// Zur Erzeugung der Sokos
	cur_werkauftrag()->get_preis().updatePreis();

	if(rohr->isStrangrohr())
	{
		EnCObListVk3& vk3remoteelementeListe = rohr->get_vk3elementeListe();
		EnCObListVk3& vk3thiselementeListe = pDrucklisteRohrwaCollector->get_vk3elementeListe();
		vk3thiselementeListe.SetRohrType(vk3remoteelementeListe.GetRohrType());
	}

	// Noetig fuer Mindermenge
	pDrucklisteRohrwaCollector->get_arbeit().merge(cur_werkauftrag()->get_arbeit());

	set_elelist(rohr->get_vk3elementeListe(),true);
	set_passlist(rohr->get_passlaengenListe());
	set_pulverlist(rohr->get_pulverlaengenListe(),true);

	// Sonderkosten
	set_sokolist(cur_werkauftrag()->get_preis().get_sokolist());

	// Die Preise der Einzelpositionen
	if (!WaxSys::isAV())
	{
		CString preis, str;
		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		preis.Format("<b>%s %s:</b> %.2f�\n", str, cur_werkauftrag()->get_posNr(), cur_werkauftrag()->get_preis().get_gesamtpreisVK3());
		preise.Add(preis);
	}
}




//Druckliste fuer die VK - Abrechnung
void DrucklistenVerwaltung::createAbrechnung()
{
	if (!WaxSys::isAV())
		html.insertEinzelpreise(preise);  
}

void DrucklistenVerwaltung::createVK3Abrechnung()
{
	if (!WaxSys::isAV())
		html.insertEinzelpreise(preise);  


	if (currentAusgabe == VK3_ABRECHNUNG_LANG)
	{
		html.insertVK3Preise(pDrucklisteRohrwaCollector->get_vk3elementeListe(), 
			                 pDrucklisteRohrwaCollector->get_passlaengenListe(),
							 pDrucklisteRohrwaCollector->get_pulverlaengenListe(),
							 pDrucklisteRohrwaCollector->get_preis().get_vk3berechnung().get_preis());
	}
	html.insertSokoPreise(pDrucklisteRohrwaCollector->get_preis().get_sokolist(), 
						  pDrucklisteRohrwaCollector->get_preis().get_sonderkosten());

	html.insertVK3GesPreise(pDrucklisteRohrwaCollector->get_preis().get_vk3berechnung().get_preis(),
						    pDrucklisteRohrwaCollector->get_preis().get_sonderkosten(), 
						    pDrucklisteRohrwaCollector->get_preis().get_gesamtpreisVK3());

	check_NullEuro();
}

//Erzeugt den Graphikdruck
void DrucklistenVerwaltung::createGraphikdruck()
{
	/*
	Graphikdruck der Rohrkonstruktion
	Hier ist Mehrfachdruck mit verschiedenen Bezeichnungen erforderlich.
	Graphik ist fuer alle Rohrkonstruktionen aufrufbar. 
	*/
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
	ApaxWerkauftrag* apax = dynamic_cast<ApaxWerkauftrag*>(cur_werkauftrag());
	if (!rohr && !apax)
	{
		createDefaultDruckliste();
		return;
	}
	insertGrafic();	
}

#define INSERT_ELEMENT2(element, marker) \
w.startRow(); \
w.appendCol((CString)marker + ((element->get_id()==GRFITTING)?element->get_kurztext():element->get_bezeichnung()), rows[0], "LEFT", true); \
if (!dn2zoll) { w.appendCol(element->get_anschlussDN(), rows[1], "CENTER", true); } \
else w.appendCol(WaxStamm::dn2zoll(element->get_anschlussDN()), rows[1], "CENTER", true); \
if (!dn2zoll) { w.appendCol(element->get_dn(), rows[2], "CENTER", true); } \
else w.appendCol(WaxStamm::dn2zoll(element->get_dn()), rows[2], "CENTER", true); \
w.appendCol(element->get_laenge(), rows[3], "CENTER", true); \
w.appendCol(element->get_winkel(), rows[4], "CENTER", true); \
w.appendCol(element->get_abstRel(), rows[5], "CENTER", true); \
w.appendCol(element->get_abstAbs(), rows[6], "CENTER", true);

//Druckliste fuer die Elementliste, die Elemente der Rohrkonstruktion 
void DrucklistenVerwaltung::createElementplan()
{
// Die Elementdaten werden an mehreren Stellen in die Liste eingef�gt. Statt den Code zu kopieren,
// gibt es auch die M�glichkeit ein Makro zu definieren. 
// Unterschiedlich beim Einf�gen ist nur der Marker.

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
	if (!rohr)
	{
		createDefaultDruckliste();
		return;
	}

	WaxHtmlFile &w = html.get_file();
	CString str, txt;
	// Tabellenspalten
	const int rows[] = { 15, 5, 10, 10, 20, 20, 20}; 
	w.startTable(0,0);
	w.startRow();
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	txt = "<b> " + str + " </b>";
	w.appendCol(txt,				rows[0], "LEFT", true);
	str = getStammdaten().getText("IDS_ANZAHL").c_str();
	txt = "<b> " + str + " </b>";
	w.appendCol(txt,				rows[1], "CENTER", true);
	if (!dn2zoll)
	{
		str = getStammdaten().getText("IDS_NENNW_DN").c_str();
		txt = "<b> " + str + " </b>";
		w.appendCol(txt,		rows[2], "CENTER", true);
	}
	else
	{
		str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
		txt = "<b> " + str + " </b>";
		w.appendCol(txt,	rows[2], "CENTER", true);
	}
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	txt = "<b> " + str + " </b>";
	w.appendCol(txt,			rows[3], "LEFT", true);
	str = getStammdaten().getText("IDS_ANFANG").c_str();
	txt = "<b> " + str + " </b>";
	w.appendCol(txt,				rows[4], "CENTER", true);
	str = getStammdaten().getText("IDS_ENDE").c_str();
	txt = "<b> " + str + " </b>";
	w.appendCol(txt,				rows[5], "CENTER", true);
	str = getStammdaten().getText("IDS_VERB").c_str();
	txt = "<b> " + str + " </b>";
	w.appendCol(txt,				rows[6], "CENTER", true);
	w.endRow();

	w.startRow();
	w.appendCol(rohr->get_posNr(),				rows[0], "LEFT", true);
	w.appendCol(rohr->get_anzahl(),				rows[0], "CENTER", true);
	if (!dn2zoll) 
		w.appendCol(rohr->get_dnvl(),			rows[1], "CENTER", true); 
	else 
		w.appendCol(WaxStamm::dn2zoll(rohr->get_dnvl()), rows[1], "CENTER", true); 
	w.appendCol(rohr->get_laenge(),				rows[2], "LEFT", true);
	Element ele;
	ele.set_id(rohr->get_rohrAnfang());
	w.appendCol(ele.get_bezeichnung(),			rows[3], "CENTER", true);
	ele.set_id(rohr->get_rohrEnde());
	ele.set_bezeichnung("");
	w.appendCol(ele.get_bezeichnung(),			rows[4], "CENTER", true);
	ele.set_id(rohr->get_rohrVerb());
	ele.set_bezeichnung("");
	w.appendCol(ele.get_bezeichnung(),			rows[5], "CENTER", true);
	w.endRow();
	w.endTable();
	
	if (rohr->get_elementArray().GetSize())
	{
		// Tabellenspalten
		const int rows[] = { 22, 15, 15, 9, 9, 15, 15};

		w.startTable(0,0);
		w.startRow();
		str = getStammdaten().getText("IDS_ELEMENTTYP").c_str();
		w.appendCol(str,	rows[0], "LEFT", true);
		if (!dn2zoll)
		{
			str = getStammdaten().getText("IDS_AN_NENNW_DN").c_str();
			w.appendCol(str,rows[1], "CENTER", true);
		}
		else
		{
			str = getStammdaten().getText("IDS_AN_NENNW_ZOLL").c_str();
			w.appendCol(str,rows[1], "CENTER", true);
		}
		if (!dn2zoll)
		{
			str = getStammdaten().getText("IDS_NENNW_DN").c_str();
			w.appendCol(str,rows[2], "CENTER", true);
		}
		else
		{
			str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
			w.appendCol(str,rows[2], "CENTER", true);
		}
		str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
		w.appendCol(str,	rows[3], "CENTER", true);
		str = getStammdaten().getText("IDS_WINKEL_G").c_str();
		w.appendCol(str,	rows[4], "CENTER", true);
		str = getStammdaten().getText("IDS_ABST_REL_MM").c_str();
		w.appendCol(str,	rows[5], "CENTER", true);
		str = getStammdaten().getText("IDS_ABST_ABS_MM").c_str();
		w.appendCol(str,	rows[6], "CENTER", true);
		w.endRow();

		for (int i=0; i<rohr->get_elementArray().GetSize(); i++)
		{
			Element *ele = rohr->get_elementArray().get_element_at(i);
			XASSERT(ele); 
			if (!ele) 
				break;

			Element e(*ele);
			INSERT_ELEMENT2((&e), "");

		}
		w.endTable();
	}
	// Ggbf. Fertigungstext
	if (rohr->get_fertigungshinweis().GetLength())	
	{
		html.insert_bemerkung((LPCSTR)rohr->get_fertigungshinweis(), getStammdaten().getText("IDS_FERTIGUNGSHINWEIS").c_str());
	}
}

//Druckliste f�r den Rohrleitungsplan, die Elemente der Rohrkonstruktion 
//werden hier direkt ausgegeben
void DrucklistenVerwaltung::createRohrleitungsplan()
{
	/**
	Aufgrund des Wunsches eines einzelnen Herrn an der Maschine wird bei Muffen grunds�tzlich
	die DN in Zoll ausgegeben. Und zwar im Format 112, wie bei der alten Wax Liste
	*/
// Die Elementdaten werden an mehreren Stellen in die Liste eingef�gt. Statt den Code zu kopieren,
// gibt es auch die M�glichkeit ein Makro zu definieren. 
// Unterschiedlich beim Einf�gen ist nur der Marker.
#define INSERT_ELEMENT(element, marker) \
{ bool zoll = element->get_id() == MUFFE ? true : dn2zoll; \
w.startRow(); \
w.appendCol((CString)marker + ((element->get_id()==GRFITTING)?element->get_kurztext():element->get_bezeichnung()), rows[0]); \
if (!zoll) { w.appendCol(element->get_anschlussDN(), rows[2]); } \
else w.appendCol(WaxStamm::dn2zoll(element->get_anschlussDN(), 1), rows[2], "RIGHT"); \
if (!zoll) { w.appendCol(element->get_dn(), rows[2]); } \
else w.appendCol(WaxStamm::dn2zoll(element->get_dn(), 1), rows[2], "RIGHT"); \
w.appendCol(element->get_laenge(), rows[3]); \
w.appendCol(element->get_winkel(), rows[4]); \
w.appendCol(element->get_abstRel(), rows[5]); \
w.appendCol(element->get_abstAbs(), rows[6]); }

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
	if (!rohr)
	{
		createDefaultDruckliste();
		return;
	}

	WaxHtmlFile &w = html.get_file();
	
	// Tabellenspalten
	const int rows[] = { 300, 90, 30, 60, 50, 90, 90};

	int eleID = 0;
	CString str;
	// Elementliste abarbeiten und jedes Element direkt ausgeben
	CObList& elementList = rohr->get_elementList();
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
		XASSERT(element); 
		if (!element) 
			break;

		if (pos)
		{ // Wenn das Folgeelement ein Rohr ist, wird f�r dieses Rohr eine neue Tabelle angefangen
			// Das erste Element muss immer Nut, Glatt usw. sein.
			// Ausser bei LEER; hinter LEER geht das Rohr weiter
			if (element->get_id() != Element::LEER)
			{
				POSITION p = pos;
				Element *next = dynamic_cast<Element*>(elementList.GetNext(p));
				XASSERT(next); 
				if (!next) 
					break;
				
				if (next->isEleRohr()) 
				{
					w.endTable(); // Erzeugt einen kleinen Abstand zwischen den Tabellen. 

					w.startTable();
					w.startRow();
					str = getStammdaten().getText("IDS_BESCHRIFTUNG_L").c_str();
					w.appendCol(str,   300);
					w.appendCol("<b>" + next->get_beschriftungLinks()  + "</b>", 300);
					str = getStammdaten().getText("IDS_BESCHRIFTUNG_R").c_str();
					w.appendCol(str,  300);
					w.appendCol("<b>" + next->get_beschriftungRechts() + "</b>", 300);
					w.endRow();
  				
					w.startTable();
					w.startRow();
					str = getStammdaten().getText("IDS_ELEMENTTYP").c_str();
					w.appendCol(str,	rows[0], "CENTER");
					if (!dn2zoll)
					{
						str = getStammdaten().getText("IDS_AN_NENNW_DN").c_str();
						w.appendCol(str,rows[1], "CENTER");
					}
					else
					{
						str = getStammdaten().getText("IDS_AN_NENNW_ZOLL").c_str();
						w.appendCol(str,rows[1], "CENTER");
					}
					if (!dn2zoll)
					{
						str = getStammdaten().getText("IDS_NENNW_DN").c_str();
						w.appendCol(str,rows[2], "CENTER");
					}
					else
					{
						str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
						w.appendCol(str,rows[2], "CENTER");
					}
					str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
					w.appendCol(str,	rows[3], "CENTER");
					str = getStammdaten().getText("IDS_WINKEL_G").c_str();
					w.appendCol(str,	rows[4], "CENTER");
					str = getStammdaten().getText("IDS_ABST_REL_MM").c_str();
					w.appendCol(str,	rows[5], "CENTER");
					str = getStammdaten().getText("IDS_ABST_REL_MM").c_str();
					w.appendCol(str,	rows[6], "CENTER");
					w.endRow();
				}
			}
		}		

		if (element->get_id() != Element::LEER)
		{
			eleID  = element->get_id();
			CString txt;
			Element e(*element);
			if (!element->isEleRohr() && eleID != ELEMENT_BOGEN && eleID != BOG2S && eleID != BOGVERZ && eleID != EINZ    
									  && eleID != ERED	&& eleID != KRED)
			{
				//e.set_abstAbs(0); // Warum? Henning fragen...
				//e.set_abstRel(0); // Weil Werte der absolute Abstand des Elements bezogen auf das Rohrstueck ist und nicht
								  // bezogen auf die Rohrkonstruktion, wie bei den anderen Elementen. 
								  // Deswegen ist vielleicht besser 0 anzugeben.
								  // Zum Verst�ndnis diesen Part kommentieren und Rohr mit Red. eingeben!   	
			}
			INSERT_ELEMENT((&e), "");
		}

		CObList& uelementList = element->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			INSERT_ELEMENT(uelement, " -> ");
			
			// Unterelemente auf Elementen, z.B. Nut auf Rohrstutzen
			CObList& uuelementList = uelement->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos;)	
			{
				Element *uuelement = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuelement); 
				if (!uuelement) 
					break;
			
				INSERT_ELEMENT(uuelement, " + ");
			}
		}
	}
	w.endTable();
}





//Druckliste fuer die Saegenutanlage
void DrucklistenVerwaltung::createSaegenutanlageliste( )
{
#define APPENDCOL_BOLD(_txt,_width,_dir)  { w.appendCol("<b>" + (_txt) + "</b>", _width, _dir); };
#define APPENDCOL_BOLD_INT(_i,_width,_dir)  { CString tmp; tmp.Format("%d", _i); w.appendCol("<b>" + tmp + "</b>", _width, _dir); };
#define APPENDCOL_NORM(_txt,_width) { w.appendCol((_txt), _width); };
#define APPENDCOL_NORM_CENTER(_txt,_width) { w.appendCol((_txt), _width, "CENTER"); };

	// Achtung: Aenderungen muessen auch in Excel::createSaegenutdatei() angepasst werden
	// Elementdaten
	int eleLaenge  = 0;
	int eleDn      = 0;
	int eleID      = 0;
	int eleAnzahl  = 0;
	int eleAnfa    = 0;
	int eleEnde    = 0;
	CString elePosNr  = "";
	int eleAnfaLaenge = 0;
	int eleEndeLaenge = 0;
	int lfdnr  = 0;
	int laenge = 0;
	bool first = false;
	bool trennlinie = false;
	CString str;

	Verteiler* vert = dynamic_cast<Verteiler*>(cur_werkauftrag());
	Stationsverteiler* statvert = dynamic_cast<Stationsverteiler*>(cur_werkauftrag());
	Strangrohr* strangrohr =  dynamic_cast<Strangrohr*>(cur_werkauftrag());

	if (!vert && !statvert && !strangrohr)
	{
		createDefaultDruckliste();
		return;
	}

	if (strangrohr)
	{	
	  if(strangrohr->get_dnvl() != 65 && strangrohr->get_muffenDN() != 25)
	  {		
		createDefaultDruckliste();
		return;
	  }
	}

	
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(cur_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;

	eleAnzahl = rohr->get_anzahl();
	elePosNr  = rohr->get_posNr();

	WaxHtmlFile &w = html.get_file();

	// Tabellenspalten fuer Rohre	
	const int rows[] = { 40, 20, 250, 20, 100, 50, 50};
	// Tabellenspalten fuer Abgaenge
	const int rows1[] = { 250, 20, 200, 80};
	
	// Elementliste abarbeiten und Rohrstuecke mit Anfa/Ende direkt ausgeben 
	ElementList elelst(rohr->get_elementList());
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *element = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(element);
		if (!element)
			break;
	
		eleID     = element->get_id();
		eleDn     = element->get_dn();
		eleLaenge = element->getLaengeZuschnitt();
		
		if (element->isEleRohr())
		{
			// vorheriges und naechstes Element fuer diverse Abfragen
			Element* prev = elelst.get_prev(count);
			Element* next = elelst.get_next(count);
			XASSERT(prev && next);
			if (!prev || !next)
				break;

			Element *pprev = NULL;
			if (count > 2)
			{
				pprev = dynamic_cast<Element*>(elelst.get_prevprev(count));
				XASSERT(pprev);
				if (!pprev)
					break;
			}

			if (prev->isEleEnde() || prev->get_id() == GLATT && pprev && pprev->get_id() == SNAHT)
				first = true;

			if (first)
			{
				lfdnr++;
				first = false;
			}
					
			if (element->getLaengeZuschnitt() > 1499 && element->get_dn() > 50 && element->get_dn() < 250) 
			{
				trennlinie = true;
				// Wenn links keine NUT und rechts NUT dann Rohr drehen
				bool drehen = false; 
				if (prev->get_id() != NUT && next->get_id() == NUT)
					drehen = true;
			
				w.startTable(0, 0);
				w.startRow();
				str = getStammdaten().getText("IDS_BESCHRIFTUNG_L").c_str();
				APPENDCOL_NORM(str, 300);
				APPENDCOL_BOLD(element->get_beschriftungLinks(), 300, "LEFT");
				str = getStammdaten().getText("IDS_BESCHRIFTUNG_R").c_str();
				APPENDCOL_NORM(str,  300);
				APPENDCOL_BOLD(element->get_beschriftungRechts(), 300, "LEFT");
				w.endRow();

				w.startRow();
				str = getStammdaten().getText("IDS_POSNR_LFDNR").c_str();
				APPENDCOL_NORM(str, rows[0]);
				str = getStammdaten().getText("IDS_ANZ_STK").c_str();
				APPENDCOL_NORM(str,  rows[1]);
				str = getStammdaten().getText("IDS_ELEMENTTYP").c_str();
				APPENDCOL_NORM(str,   rows[2]);
				if (!dn2zoll)
				{
					str = getStammdaten().getText("IDS_NENNW_DN").c_str();
					APPENDCOL_NORM(str,	rows[3]);
				}
				else
				{
					str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
					APPENDCOL_NORM(str, rows[3]);
				}
				str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
				APPENDCOL_NORM(str, rows[4]);
				str = getStammdaten().getText("IDS_ANFANG").c_str();
				APPENDCOL_NORM_CENTER(str, rows[5]);
				str = getStammdaten().getText("IDS_ENDE").c_str();
				APPENDCOL_NORM_CENTER(str, rows[6]);
				w.endRow();

				if (drehen)
				{
					eleEnde = prev->get_id(); 
					eleEndeLaenge = prev->get_laenge();
				}
				else
				{
					eleAnfa = prev->get_id(); 
					eleAnfaLaenge = prev->get_laenge();
				}

				if (drehen)
				{
					eleAnfa = next->get_id(); 
					eleAnfaLaenge = next->get_laenge();
				}
				else
				{
					eleEnde = next->get_id(); 
					eleEndeLaenge = next->get_laenge();
				}
				w.startRow();

				CString txt, str;
				str.Format(".%d", lfdnr);
				txt.Format("%s", elePosNr);
				txt += str;
				APPENDCOL_BOLD(txt, rows[0], "LEFT");
				APPENDCOL_BOLD_INT(eleAnzahl, rows[1], "LEFT");
				if (element->get_id() == ROHR_Q1 || element->get_id() == ROHR)
				{
					CString bez; 
					if (element->get_dn() < 65)
					{
						bez = getStammdaten().getText("IDS_2440STD").c_str();
						element->set_bezeichnung(bez);
					}
					else
					{
						bez = getStammdaten().getText("IDS_2458STD").c_str();
						element->set_bezeichnung(bez);
					}
				}
				APPENDCOL_BOLD(element->get_bezeichnung(), rows[2], "LEFT");
				if (!dn2zoll)
				{
					APPENDCOL_BOLD_INT(eleDn, rows[3], "LEFT");
				}
				else
				{
					APPENDCOL_BOLD(WaxStamm::dn2zoll(eleDn), rows[3], "RIGHT");
				}
				APPENDCOL_BOLD_INT(eleLaenge, rows[4], "LEFT");
				CString a, e;
				if (prev->get_id() == NUT)
					a = getStammdaten().getText("IDS_NUT").c_str();
				else
					a = getStammdaten().getText("IDS_GLATT").c_str();
				if (next->get_id() == NUT)
					e = getStammdaten().getText("IDS_NUT").c_str();
				else
					e = getStammdaten().getText("IDS_GLATT").c_str();
				APPENDCOL_BOLD(drehen ? e : a, rows[5], "CENTER");
				APPENDCOL_BOLD(drehen ? a : e, rows[6], "CENTER");
				w.endRow();

				CObList& uelementList = element->get_uelementList();
				if (uelementList.IsEmpty()) 
				{
					w.endTable();
					// bisheriger L�nge berechnen
					laenge += (element->isEleBogen()) ? (2*element->get_laenge()) : element->get_laenge();
					continue;
				}

				// Bei Verteiler, die ueber Verteileranlage laufen, sollen jetzt keine 
				// Loecher gebrannt werden
				// Sollen zumindest fuer die Abgaenge, die nicht auf der Verteilerstrasse
				// geschweisst werden, die Loecher gebrannt werden, einfach diese 
				// if-Anweisung entfernen und uele auf makeAnlage abfragen.
				if (!element->hasRohrStdelemente()  ||(strangrohr && strangrohr->get_dnvl() == 65 && strangrohr->get_muffenDN() == 25))
				{
					// �berschrift f�r die Unterelementliste
					w.startRow();
					str = getStammdaten().getText("IDS_ELEMENTTYP").c_str();
					APPENDCOL_NORM(str, rows1[0]);
					str = getStammdaten().getText("IDS_NENNWEITE_DN_ZOLL").c_str();
					APPENDCOL_NORM(str, rows1[1]);
					str = getStammdaten().getText("IDS_ABSTAND_MM").c_str();
					APPENDCOL_NORM(str, rows1[2]);
					str = getStammdaten().getText("IDS_WINKEL_G").c_str();
					APPENDCOL_NORM(str, rows1[3]);
					w.endRow();

					int k=0; Element *up = NULL;
					if (drehen)
					{
						for (POSITION upos = uelementList.GetTailPosition(); upos != NULL;)	
						{
							if (k>0)
							{
								POSITION p = uelementList.FindIndex(k-1);
								up = dynamic_cast<Element*>(uelementList.GetAt(p));
							}
 
							Element *uelement = dynamic_cast<Element*>(uelementList.GetPrev(upos));
							XASSERT(uelement);
							if (!uelement)
								break;

							// fuer diese Elemente werden Loecher gebrannt
							if ((!uelement->get_makeAnlage() && (uelement->get_id() == ROHR  
														     || uelement->get_id() == NUTST 
														     || uelement->get_id() == GWRST
														     || uelement->get_id() == MUFFE
															 || uelement->get_id() == BOHR50)) 
															 ||
															 ((uelement->get_id() == ROHR  
														     || uelement->get_id() == NUTST 
														     || uelement->get_id() == GWRST
														     || uelement->get_id() == MUFFE
															 || uelement->get_id() == BOHR50) && ( strangrohr && strangrohr->get_dnvl() == 65 && strangrohr->get_muffenDN() == 25)))
							{
								// Wenn zwei gleich Rohrstutzen zusammen, dann darf nur ein Loch gebrannt werden,
								// sonst bleibt vom Rohr nichts mehr �brig. Das zweite wird von Hand gemacht.
								if (up && up->get_id() == ROHR && up->get_dn() == up->get_anschlussDN()
									   && uelement->get_id() == ROHR && uelement->get_dn() == uelement->get_anschlussDN())
								{
									DNzuAussendurchmesser dntoad;
									int aussendurchmesser = dntoad.getAussendurchmesserToInt(uelement->get_dn());
									if (uelement->get_abstRel() < aussendurchmesser + 50) 
										continue;
								}
								Saegenutanlageliste_insert_uelement(w, uelement, element->get_laenge() - uelement->get_abstAbs() + laenge, 360 - uelement->get_winkel());
							}
							k++;
						}
					}
					else
					{
						for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
						{
							if (k>0)
							{
								POSITION p = uelementList.FindIndex(k-1);
								up = dynamic_cast<Element*>(uelementList.GetAt(p));
							}

							Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
							XASSERT(uelement);
							if (!uelement)
								break;

							// fuer diese Elemente werden Loecher gebrannt
							if ((!uelement->get_makeAnlage() && (uelement->get_id() == ROHR
											 			     || uelement->get_id() == NUTST 
														     || uelement->get_id() == GWRST
														     || uelement->get_id() == MUFFE
															 || uelement->get_id() == BOHR50))
															 ||
															 ((uelement->get_id() == ROHR  
														     || uelement->get_id() == NUTST 
														     || uelement->get_id() == GWRST
														     || uelement->get_id() == MUFFE
															 || uelement->get_id() == BOHR50) && ( strangrohr && strangrohr->get_dnvl() == 65 && strangrohr->get_muffenDN() == 25)))
							{
								// Wenn zwei gleich Rohrstutzen zusammen, dann darf nur ein Loch gebrannt werden,
								// sonst bleibt vom Rohr nichts mehr �brig. Das zweite wird von Hand gemacht.
								if (up &&   up->get_id() == ROHR &&   up->get_dn() ==   up->get_anschlussDN()
										&& uelement->get_id() == ROHR && uelement->get_dn() == uelement->get_anschlussDN())
								{
									DNzuAussendurchmesser dntoad;
									int aussendurchmesser = dntoad.getAussendurchmesserToInt(uelement->get_dn());
									if (uelement->get_abstRel() < aussendurchmesser + 50) 
										continue;
								}
								Saegenutanlageliste_insert_uelement(w, uelement, uelement->get_abstAbs() - laenge, uelement->get_winkel());
							}
							k++;
						}
					}
				}
				w.endTable();
			}
		}
		// bisheriger L�nge berechnen
		laenge += (element->isEleBogen()) ? (2*element->get_laenge()) : element->get_laenge();
	}
	if (trennlinie)
		w.insertTrennlinie();
}

void DrucklistenVerwaltung::Saegenutanlageliste_insert_uelement(WaxHtmlFile &w, const Element *uelement, const int eleAbst, const int eleWinkel)
{
	// von createSaege... kopiert
	const int rows1[] = { 250, 20, 200, 80};

	int eleDn = uelement->get_dn();
	
	w.startRow();
	CString txt, str = " -> ";
	str += uelement->getBezeichnung();
	APPENDCOL_BOLD(str, rows1[0], "LEFT");
	if (uelement->get_id() == BOHR50)
	{
		txt.Format("%.2fmm", uelement->get_durchmesser());
		APPENDCOL_BOLD(txt, rows1[1], "LEFT");
	}
	else if (uelement->get_id() == MUFFE)
	{
		APPENDCOL_BOLD(WaxStamm::dn2zoll(eleDn, 1), rows1[1], "LEFT");
	}
	else
	{
		APPENDCOL_BOLD_INT(eleDn, rows1[1], "LEFT");
	}
	APPENDCOL_BOLD_INT(eleAbst, rows1[2], "LEFT");
	APPENDCOL_BOLD_INT(eleWinkel, rows1[3], "LEFT");
	w.endRow();
}


//Erfast die Daten fuer die Gewinderohrliste
void DrucklistenVerwaltung::createGewinderohrdata()
{
	Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(cur_werkauftrag());
	XASSERT(grohr);
	if (!grohr)
		return;

	gesamtlaenge += grohr->get_laenge() * grohr->get_anzahl();
	anzahlRohre += grohr->get_anzahl();

	ElementList elelst(grohr->get_elementList());
	// Abarbeiten der Elementliste
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *element = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(element);
		if (!element)
			break;

		if (element->get_id() == GRFITTING)
			anzahlElemente += grohr->get_anzahl();
		else if (element->isEleRohr()) 
		{
			// vorheriges und naechstes Element fuer diverse Abfragen
			Element* prevelement = NULL;
			Element* nextelement = NULL;
			prevelement = elelst.get_prev(count);
			nextelement = elelst.get_next(count);
			XASSERT(prevelement && nextelement);
			if (!prevelement || !nextelement)
				break;

			anzahlRohrstuecke += grohr->get_anzahl();
			
			int counter = 0;
			POSITION upos;
			// Element schon in Gesamtliste, dann Anzahl addieren
			for (upos = elelist.GetHeadPosition(); upos;)
			{
				Element *ele = dynamic_cast<Element*>(elelist.GetNext(upos));
				XASSERT(ele);
				if (!ele)
					break;

				if (ele->isEleRohr())
				{
					POSITION pp = elelist.FindIndex(counter+1);
					Element* prevele = dynamic_cast<Element*>(elelist.GetAt(pp));
					POSITION np = elelist.FindIndex(counter+2);
					Element* nextele = dynamic_cast<Element*>(elelist.GetAt(np));
					XASSERT(prevele && nextele);
					if (!prevele || !nextele)
						break;

					if (ele->get_artikelnr() == element->get_artikelnr()
					//if (ele->get_id() == element->get_id() 
					// && ele->get_dn() == element->get_dn()
					 && ele->get_laenge() == element->get_laenge()
					 && (prevele->get_id() == prevelement->get_id() 
						|| prevele->get_id() == GEW    && prevelement->get_id() == GRFITTING 
						|| prevele->get_id() == GRFITTING && prevelement->get_id() == GEW) 
					 && (nextele->get_id() == nextelement->get_id() 
						|| nextele->get_id() == GEW    && nextelement->get_id() == GRFITTING 
						|| nextele->get_id() == GRFITTING && nextelement->get_id() == GEW)) 
					{
						ele->set_anzahl(ele->get_anzahl() + element->get_anzahl()*grohr->get_anzahl());
						upos--;
						break;
					}
				}
				counter++;
			}
			// in Gesamtliste packen
			if (upos == NULL) 
			{
				Element *e = new Element;
				e->copy(*element);
				e->set_anzahl(e->get_anzahl()*grohr->get_anzahl()); 
				elelist.AddTail(e);
				
				Element *pe = new Element;
				pe->copy(*prevelement);
				elelist.AddTail(pe);

				Element *ne = new Element;
				ne->copy(*nextelement);
				elelist.AddTail(ne);
			}
		}
	}
}





//Druckliste f�r die Pumpenprobierleitung. Die Grafik wird als Bitmap 
//eingebunden, wie Sondermz. Grafik ist VDS abgenommen. F�r Infos: Hr. 
//Richter
void DrucklistenVerwaltung::createPumpenprobierleitung( )
{
	Pumpenprobierleitung *pumpro = dynamic_cast<Pumpenprobierleitung*>(cur_werkauftrag());
	XASSERT(pumpro);
	if (!pumpro)
		return;

	CString logo = WaxSys::GetTempDir() + "pumpro.bmp";
	//HRSRC rclog = ::FindResource(NULL, (LPSTR)"IDB_PUMPENPROBIERLEITUNG", (LPSTR)"JPG");
	HMODULE module = GetModuleHandle("waxprj.dll");
	HRSRC rclog = ::FindResource(module, MAKEINTRESOURCE(IDB_PUMPENPROBIERLEITUNG), RT_BITMAP);
	Druckliste::loadResource(logo, rclog, module);

	WaxHtmlFile &w = html.get_file();

	const int rows[] = { 50, 50, 50, 50, 100, 100, 100, 50, 50, 50};
	CString str;

	// Daten
	w.startTable();
	w.startRow();

	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	w.appendCol(str,			rows[0], "CENTER");
	
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	w.appendCol(str,			rows[1], "CENTER");
	
	w.appendCol("DN 1",			rows[2], "CENTER");
	
	w.appendCol("DN 2",			rows[3], "CENTER");
	
	w.appendCol("L1 [mm]",		rows[4], "CENTER");
	
	w.appendCol("L2 [mm]",		rows[5], "CENTER");
	
	w.appendCol("L3 [mm]",		rows[6], "CENTER");
	
	w.appendCol("L4 [mm]",		rows[7], "CENTER");
	
	w.appendCol("L5 [mm]",		rows[8], "CENTER");
	
	w.appendCol("d [mm]",		rows[9], "CENTER");
	
	w.endRow();
	w.startRow();
	w.appendCol(pumpro->get_posNr(),	rows[0]);
	w.appendCol(pumpro->get_anzahl(),	rows[1]);
	w.appendCol(pumpro->get_dn1(),		rows[2]);
	w.appendCol(pumpro->get_dn2(),		rows[3]);
	w.appendCol(pumpro->get_laenge1(),	rows[4]);
	w.appendCol(pumpro->get_laenge2(),	rows[5]);
	w.appendCol(pumpro->get_laenge3(),	rows[6]);
	w.appendCol(pumpro->get_minval(Pumpenprobierleitung::L4), rows[7], "RIGHT");
	w.appendCol(pumpro->get_minval(Pumpenprobierleitung::L5), rows[8], "RIGHT");
	w.appendCol(pumpro->get_minval(Pumpenprobierleitung::D),  rows[9], "RIGHT");
	w.endRow();
	w.endTable();
	
	w.startTable();
	w.startRow();
	BauelementGruppen bg;
	w.appendCol("A:", 50, "CENTER");
	w.appendCol(bg.getBezeichnung(pumpro->get_anschlussA()), 450, "LEFT");
	w.appendCol("B:", 50, "CENTER");
	w.appendCol(bg.getBezeichnung(pumpro->get_anschlussB()), 100, "RIGHT");
	w.endRow();
	w.endTable();
	
	// Graphik
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
	w.WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n"
				"<P ALIGN='CENTER'><IMG SRC='pumpro.bmp' WIDTH=600 HEIGHT=200 ></TD>\n</tr>");
}

//Druckliste f�r den Pumpenverteiler. Die Grafik wird als Bitmap 
//eingebunden, wie Sondermz. Grafik ist VDS abgenommen. F�r Infos: Hr. 
//Richter
void DrucklistenVerwaltung::createPumpenverteiler( )
{
	Pumpenverteiler *ppvert = dynamic_cast<Pumpenverteiler*>(cur_werkauftrag());
	XASSERT(ppvert);
	if (!ppvert)
		return;

	const int size = 200;
	CString logo = WaxSys::GetTempDir() + "pumpenverteiler.jpg";
	HRSRC rclog = ::FindResource(NULL, (LPSTR)"PUMPENVERTEILER", (LPSTR)"JPG");
	Druckliste::loadResource(logo, rclog);

	WaxHtmlFile &w = html.get_file();
	CString str, txt;

	// Daten
	w.startTable();
	w.startRow();
	
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	w.appendCol(str,	size, "LEFT");
	
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	w.appendCol(str,	size, "LEFT");
	
	str = getStammdaten().getText("IDS_GEWICHT_K").c_str();
	w.appendCol(str,	size, "LEFT");
	
	w.endRow();
	w.startRow();
	w.appendCol(ppvert->get_posNr(),  size, "LEFT");
	w.appendCol(ppvert->get_anzahl(), size, "LEFT");
	w.appendCol(ppvert->getGesGewicht(), size, "LEFT");
	w.endRow();
	w.endTable();
	
	txt = 	"<TD WIDTH='50%' VALIGN='MIDDLE' COLSPAN=2>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>"; 
	str = getStammdaten().getText("IDS_PUMPENDRUCKSTUTZEN").c_str();
	txt +=      "DN1 ";
	txt +=		str;
	txt +=      ":";
	txt +=		"</B></FONT></TD>\n";
	txt += 	"<TD WIDTH='50%' VALIGN='MIDDLE' COLSPAN=4>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>"; 
	str = getStammdaten().getText("IDS_PROBIERANSCHLUSS").c_str();
	txt +=		"DN2 ";
	txt +=		str;
	txt +=      ":";
	txt +=		"</B></FONT></TD>\n";
	
	w.startTable();
	w.startRow();
	w.WriteString(txt);
	w.endRow();
	w.startRow();
	
	w.appendCol("DN",			size, "CENTER");
	
	str = getStammdaten().getText("IDS_WINKEL_G").c_str();
	w.appendCol(str,			size, "CENTER");
	
	str = getStammdaten().getText("IDS_TYP").c_str();
	w.appendCol(str,			size, "CENTER");
	
	w.appendCol("DN",			size, "CENTER");
	
	w.appendCol("L1 [mm]",		size, "CENTER");
	
	w.appendCol("L2 [mm]",		size, "CENTER");
	
	w.endRow();
	w.startRow();
	w.appendCol(ppvert->get_dnPumpDruckSt(),	size);
	w.appendCol(ppvert->get_winkelRohnippel(),	size);
	Element ele;
	ele.set_id(ppvert->get_probAnschl());
	w.appendCol(ele.get_bezeichnung(),			size, "CENTER");
	w.appendCol(ppvert->get_dnProbAnschl(),		size);
	w.appendCol(ppvert->get_laengeProbAnschl(), size);
	w.appendCol(ppvert->get_abstandProbAnschl(),size);
	w.endRow();
	w.endTable();

	txt = 	"<TD WIDTH='size%' VALIGN='MIDDLE' COLSPAN=3>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>"; 
	str = getStammdaten().getText("IDS_SPRINKLERANLAGENASNSCHLUSS").c_str();
	txt +=      "DN3 ";
	txt +=      str;
	txt +=      ":";
	
	
	txt +=		"</B></FONT></TD>\n";
	txt += 	"<TD WIDTH='size%' VALIGN='MIDDLE' COLSPAN=2>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>"; 
	str = getStammdaten().getText("IDS_ANSCHLUSS_FUER_AUFFUELLBEHAELTER").c_str();
	txt +=		str;
	txt +=      ":";
	txt +=		"</B></FONT></TD>\n";
	
	w.startTable();
	w.startRow();
	w.WriteString(txt);
	w.endRow();
	w.startRow();

	str = getStammdaten().getText("IDS_TYP").c_str();
	w.appendCol(str,			size, "CENTER");

	w.appendCol("DN",			size, "CENTER");

	w.appendCol("L3 [mm]",		size, "CENTER");

	w.appendCol("L4 [mm]",		size, "CENTER");

	str = getStammdaten().getText("IDS_WINKEL_G").c_str();
	w.appendCol(str,			size, "CENTER");

	w.endRow();
	w.startRow();
	Element ele1;
	ele1.set_id(ppvert->get_sprAnlAnschl());
	w.appendCol(ele1.get_bezeichnung(),			size, "CENTER");
	w.appendCol(ppvert->get_dnSprAnlAnschl(),	size);
	w.appendCol(ppvert->get_gesamthoehe(),		size);

	if (ppvert->get_schwMuffe())
	{
		w.appendCol(ppvert->get_abstandSchwMuffe(), size);
		w.appendCol(ppvert->get_winkelSchwMuffe(),	size);
	}
	
	w.endRow();
	w.endTable();

	// Graphik Originalgr�sse ist 252x201. Hier um 25% aufgezoomt
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
	w.WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n"
				"<P ALIGN='CENTER'><IMG SRC='pumpenverteiler.jpg' WIDTH=375 HEIGHT=251 ></TD>\n</tr>");
}

//Druckliste f�r die EntnahmeleitungDWB. Die Grafik wird als Bitmap 
//eingebunden, wie Sondermz. Grafik ist VDS abgenommen. F�r Infos: Hr. 
//Richter
void DrucklistenVerwaltung::createEntnahmeleitungDWB( )
{
	EntnahmeleitungDWB *entleit = dynamic_cast<EntnahmeleitungDWB*>(cur_werkauftrag());
	XASSERT(entleit);
	if (!entleit)
		return;


	HRSRC rclog;
	CString logo;
	CString sprache = WaxSys::para_str(LANGUAGE);
	if (entleit->get_entnahmeleitung_typ() == 0)
	{
		rclog = ::FindResource(NULL, (LPSTR)"ENTNAHMELEITUNG_A", (LPSTR)"JPG");
		logo = WaxSys::GetTempDir() + "entnahmeleitung_a.jpg";
	}
	else
	{
		rclog = ::FindResource(NULL, (LPSTR)"ENTNAHMELEITUNG_B", (LPSTR)"JPG");
		logo = WaxSys::GetTempDir() + "entnahmeleitung_b.jpg";
	}

	Druckliste::loadResource(logo, rclog);

	const int rows[] = { 40, 50, 200, 80, 40, 40, 40};
	CString str;

	// Daten
	html.get_file().startTable();
	html.get_file().startRow();

	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	html.get_file().appendCol(str,			rows[0], "CENTER");

	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	html.get_file().appendCol(str,			rows[1], "CENTER");

	str = getStammdaten().getText("IDS_ENTLEERUNGb").c_str();
	html.get_file().appendCol(str,			rows[2], "CENTER");

	str = getStammdaten().getText("IDS_AUF_SEITE").c_str();
	html.get_file().appendCol(str,			rows[3], "CENTER");

	html.get_file().appendCol("DN",         rows[4], "CENTER");

	html.get_file().appendCol("L1 [mm]",    rows[5], "CENTER");

	html.get_file().appendCol("L2 [mm]",    rows[6], "CENTER");

	html.get_file().endRow();
	html.get_file().startRow();
	html.get_file().appendCol(entleit->get_posNr(),		rows[0]);
	html.get_file().appendCol(entleit->get_anzahl(),	rows[1]);
	Element ele;
	ele.set_id(entleit->get_entleerung());
	html.get_file().appendCol(ele.get_bezeichnung(),	rows[2]);
	if (entleit->get_entleerungRechts())
		str = getStammdaten().getText("IDS_RECHTS").c_str();
	else
		str = getStammdaten().getText("IDS_LINKS").c_str();
	html.get_file().appendCol(str,						rows[3]);
	html.get_file().appendCol(entleit->get_dnvl(),		rows[4]);
	html.get_file().appendCol(entleit->get_laenge1(),	rows[5]);
	html.get_file().appendCol(entleit->get_laenge2(),	rows[6]);
	html.get_file().endRow();
	html.get_file().endTable();

	// Graphik
	html.get_file().WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
	
	if (entleit->get_entnahmeleitung_typ() == 0)
		html.get_file().WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n"
					"<P ALIGN='CENTER'><IMG SRC='entnahmeleitung_a.jpg' WIDTH=375 HEIGHT=251 ></TD>\n</tr>");
	else	
		html.get_file().WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n"
					"<P ALIGN='CENTER'><IMG SRC='entnahmeleitung_b.jpg' WIDTH=375 HEIGHT=251 ></TD>\n</tr>");
}



void DrucklistenVerwaltung::createProfilliste(int liste)
{
	#define SEITENANSICHT -1
	
	// Tabellenspalten
	const int titel[] = { 30, 30, 270, 30, 300, 30 }; 
	const int rows2[] = {    330, 30, 300, 30 }; // Zeile mit Winkel und L�nge

	// Liste mit Bitmapdateien, die die Ansichten zeigen. Dies sind dieselben Bitmaps, wie sie im 
	// Dialog EditProfile benutzt werden. 
	struct ansichtlist {
		int profiltyp;
		int ansicht;
		const char *ansicht_bmp;
		int ansicht_id;
	} alist[] =
	{
		{ Element::FSTAHL, 0, "f1.bmp", IDC_FLACH1 },
		{ Element::FSTAHL, 1, "f2.bmp", IDC_FLACH2 },
		{ Element::FSTAHL, 5, "f6.bmp", IDC_FLACH6 },
		{ Element::FSTAHL, 6, "f7.bmp", IDC_FLACH7 },

		{ Element::LSTAHL, 0, "l1.bmp", IDC_L_1 },
		{ Element::LSTAHL, 1, "l2.bmp", IDC_L_2 },
		{ Element::LSTAHL, 2, "l3.bmp", IDC_L_3 },
		{ Element::LSTAHL, 3, "l4.bmp", IDC_L_4 },
		{ Element::LSTAHL, 4, "l5.bmp", IDC_L_5 },
		{ Element::LSTAHL, 5, "l6.bmp", IDC_L_6 },
		{ Element::LSTAHL, 6, "l7.bmp", IDC_L_7 },
		{ Element::LSTAHL, 7, "l8.bmp", IDC_L_8 },
		{ Element::LSTAHL, 8, "l9.bmp", IDC_L_9 },

		{ Element::USTAHL, 0, "u1.bmp", IDC_U_1 },
		{ Element::USTAHL, 1, "u2.bmp", IDC_U_2 },
		{ Element::USTAHL, 2, "u3.bmp", IDC_U_3 },
		{ Element::USTAHL, 3, "u4.bmp", IDC_U_4 },
		{ Element::USTAHL, 4, "u5.bmp", IDC_U_5 },
		{ Element::USTAHL, 5, "u6.bmp", IDC_U_6 },
		{ Element::USTAHL, 6, "u7.bmp", IDC_U_7 },
		{ Element::USTAHL, 7, "u8.bmp", IDC_U_8 },
		{ Element::USTAHL, 8, "u9.bmp", IDC_U_9 },
		{ SEITENANSICHT, 0, "l_a.bmp", IDC_L_A },
		{ SEITENANSICHT, 1, "l_b.bmp", IDC_L_B },
		{ SEITENANSICHT, 2, "l_c.bmp", IDC_L_C },
		{ 0, 0, " ", 0 },
	};
	int i;
	for (i = 0; alist[i].ansicht_id; i++)
	{
		CString logo = WaxSys::GetTempDir() + (CString)alist[i].ansicht_bmp;
		HMODULE module = GetModuleHandle("waxprj.dll");
		HRSRC rclog = ::FindResource(module, MAKEINTRESOURCE(alist[i].ansicht_id), RT_BITMAP);
		Druckliste::loadResource(logo, rclog, module);
	}

	WaxHtmlFile &w = html.get_file();
	html.insertListeUnterschriften();
	CObList proflist;

	// Bei 38040 nur UStahl, bei der anderen alle ausser UStahl
	int profile = liste == PROFILLISTE_38040 ? Element::USTAHL : Element::FSTAHL | Element::LSTAHL;
	cur_werkauftrag()->get_material().get_profilliste(proflist, profile);

	CString str;
    
	w.startTable();

	w.startRow();
	
	str = getStammdaten().getText("IDS_NR").c_str();
	w.appendCol(str,	titel[i++], "CENTER");
	
	str = getStammdaten().getText("IDS_ANZAHL").c_str();
	w.appendCol(str,	titel[i++], "CENTER");
	
	str = getStammdaten().getText("IDS_PROFILTYP").c_str();
	w.appendCol(str,	titel[i++], "CENTER");
	
	str = getStammdaten().getText("IDS_WINKEL").c_str();
	str += " L";
	w.appendCol(str,	titel[i++], "CENTER");
	
	str = getStammdaten().getText("IDS_ANSICHT_SEITENANSICHT").c_str();
	w.appendCol(str,	titel[i++], "CENTER");
	
	str = getStammdaten().getText("IDS_WINKEL").c_str();
	str += " R";
	w.appendCol(str,	titel[i++], "CENTER");
	w.endRow();

	for (POSITION pos = proflist.GetHeadPosition(); pos; )
	{
		Material *mat = dynamic_cast<Material*>(proflist.GetNext(pos));
		XASSERT(mat);
		if (!mat)
			break;

		// Aus mass2 werden die relevanten Profildaten ermittelt
		int winkelLinks, winkelRechts, ansicht, seitenansicht;
		mat->getMass2(mat->get_mass2(), winkelLinks, winkelRechts, ansicht, seitenansicht);

		w.startRow();
		i = 0;
		// Die Positionsnummer wird generiert, da das Material (bis jetzt) noch keine Positionsnummer besitzt
		w.appendCol(mat->get_posnr(), titel[i++], "CENTER");
		w.appendCol(mat->get_anzahl(), titel[i++], "CENTER");

		// Der Listentext wird aus den Abmessungen generiert
		w.appendCol(mat->get_bezeichnung(), titel[i++], "CENTER");
		
		CString grad;
		grad.Format("%i'", winkelLinks);
		w.appendCol(grad, titel[i++], "CENTER");
		
		CString bmp[2]; // Maximal 2 Bitmaps (bei L Profil)
		for (int x = 0; alist[x].ansicht_id; x++)
		{
			if (alist[x].profiltyp == mat->get_profilkennung() && alist[x].ansicht == ansicht)
				bmp[0] = alist[x].ansicht_bmp;
			else if (mat->get_profilkennung() == Element::LSTAHL && alist[x].profiltyp == SEITENANSICHT && alist[x].ansicht == seitenansicht)
				bmp[1] = alist[x].ansicht_bmp;
		}
		// Bitmaps einf�gen
		CString bmp_entry;
		bmp_entry.Format("<TD WIDTH=300 VALIGN='bottom' >\n"
					"<P ALIGN='CENTER'><IMG SRC='%s' WIDTH=150 HEIGHT=70 >", bmp[0]);
		if (bmp[1].GetLength())
			bmp_entry += (CString)"<IMG SRC='" + bmp[1] + (CString)"' WIDTH=50 HEIGHT=50 >";
		bmp_entry += (CString)"</p></TD> \r\n";
		w.WriteString(bmp_entry);

		grad.Format("%i'", winkelRechts);
		w.appendCol(grad, titel[++i], "LEFT");
		
		w.endRow();

		// Zeile mit Winkel und L�ngenangaben
		w.startRow();
		i = 0;
		CString fmt;
		fmt.Format("<TD WIDTH=%i VALIGN='RIGHT' COLSPAN=3>\n <FONT FACE='Arial' SIZE=2> </FONT></TD>\n", rows2[i++]);
		w.WriteString(fmt);
		w.appendCol(" ", rows2[i++], "CENTER");
		w.appendCol(mat->get_mass1(), rows2[i++], "CENTER");
		w.appendCol(" ", rows2[i++], "CENTER");
		w.endRow();
	}		
	w.endTable();
}


bool DrucklistenVerwaltung::check2Rohrquali(const CObList& walist) const
{
	/**
	Pr�fung auf verschiedene Rohrqualit�ten in der Auftragsliste.
	Wenn ein einzelnes Rohr bereits zwei Rohrqualis hat, brauchen wird gar nicht weiter
	zu pr�fen. Ansonsten wird f�r die DN �berpr�ft, da �ber DN65 eine andere Rohrqualit�t gilt,
	die allerdings dieselbe ID hat.	
	*/
	bool dn50 = false, dn65 = false;
	int rohrquali = Rohrleitung::LEER;
	for (POSITION pos = walist.GetHeadPosition(); pos != NULL; )
	{
		const Rohrleitung *r = dynamic_cast<const Rohrleitung*>(walist.GetNext(pos));
		if (!r)
			continue;
		
		if (r->hasRohr2Quali())
			return true;

				// Die Rohrqualit�ten der Werkauftr�ge k�nnen sich unterscheiden, ohne dass die Werkauftr�ge 
		// selber verschiedene Rohrqualit�ten haben
		if (rohrquali == Rohrleitung::LEER)
			rohrquali = r->get_rohrQuali(); 
		if (rohrquali != r->get_rohrQuali())
			return true;

		// Ausnahme ist Standardrohr
		// Bei Standardrohr wird bei DN65 automatisch eine andere Rohrqualit�t angezogen
		if (r->get_rohrQuali() == Rohrleitung::RQ2440_2458GSCH)
		{
			if (r->get_dnvl() <= 50)
				dn50 = true;
			else
				dn65 = true;
			
			if (dn50 && dn65)
				return true;
		}
	}
	return false;
}


void DrucklistenVerwaltung::set_werkauftrag(Werkauftrag* w)
{
	if (w)
		walist.AddHead(w);
	werkauftrag = w;
}

//set walist
void DrucklistenVerwaltung::set_walist(CObList& value)
{
	/*
	Achtung: Mehrfacher Aufruf f�hrt zu einer aufsummierten Liste
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



// set_stklist, fuellt die Stueckliste des Werkauftragobjekts mit der 
//uebergebenen Stueckkliste des aktuellen Werkauftrages
void DrucklistenVerwaltung::set_stklist(const CObList& value, bool nullpreis, bool einkauf)
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
				for (upos = pDrucklisteRohrwaCollector->get_stueckliste().GetHeadPosition(); upos;)
				{
					Element *element = dynamic_cast<Element*>(pDrucklisteRohrwaCollector->get_stueckliste().GetNext(upos));
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
					pDrucklisteRohrwaCollector->get_stueckliste().add_copy(*ele);
			}
			
		}
	}
}

// setGesamtmaterialliste, fuellt die Materialiste des Werkauftragobjekts mit der 
//Stueckliste des Werkauftragobjekts auf
void DrucklistenVerwaltung::setGesamtmaterialliste()
{
	// Stueckliste vom Werkauftrag scannen
	for (POSITION pos = pDrucklisteRohrwaCollector->get_stueckliste().GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(pDrucklisteRohrwaCollector->get_stueckliste().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		POSITION upos;
		// Material schon in Gesamtliste, dann Anzahl addieren
		for (upos = pDrucklisteRohrwaCollector->get_material().get_matlist().GetHeadPosition(); upos;)
		{
			Material *mat = dynamic_cast<Material*>(pDrucklisteRohrwaCollector->get_material().get_matlist().GetNext(upos));
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
			if (ele->get_id() == KABELKANAL_COII || ele->get_id() == ROHR || ele->isEleRohr() || (ele->get_id() == STDSTUTZEN && ele->get_urid() == ROHR))
				m->set_mass1(ele->get_laenge());
			else
				m->set_mass1(0);
			pDrucklisteRohrwaCollector->get_material().get_matlist().AddTail(m);
		}
	}
}

// set_matlist, fuellt die Materialliste des Werkauftragobjekts mit der 
//uebergebenen Materialliste des aktuellen Werkauftrages
void DrucklistenVerwaltung::set_matlist(const CObList& value, int list, bool nullpreis, bool einkauf)
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
			if ((list == DrucklistenVerwaltung::MATERIALLISTE_GESAMT)
				|| (list == DrucklistenVerwaltung::STRANGROHRLISTE))
			{
				// Nur Material ohne Preis?
				if ((mat->get_festPreis() < 0.009 &&  nullpreis) || !nullpreis)
				{
					POSITION upos;
					// Material schon in Gesamtliste, dann Anzahl addieren
					for (upos = pDrucklisteRohrwaCollector->get_material().get_matlist().GetHeadPosition(); upos;)
					{
						Material *material = dynamic_cast<Material*>(pDrucklisteRohrwaCollector->get_material().get_matlist().GetNext(upos));
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
					// in Gesamtliste packen
					if (upos == NULL) 
					{
						Material *m = new Material;
						m->copy(*mat);
						// Bei Rohr & Profil die Laenge * Anzahl als mm in mass1 speichern 
						if (m->get_mass1() > 0)
							m->set_mass1(m->get_anzahl()*m->get_mass1());
						pDrucklisteRohrwaCollector->get_material().get_matlist().AddTail(m);
					}
				}
			}
		}
	}
}

// set_sokolist, fuellt die Sokoliste des Werkauftragobjekts mit der uebergebenen 
//Sokoliste des aktuellen Werkauftrages
void DrucklistenVerwaltung::set_sokolist(const CObList& value)
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
		for (upos = pDrucklisteRohrwaCollector->get_preis().get_sokolist().GetHeadPosition(); upos;)
		{
			Preis::Sonderkosten *so = dynamic_cast<Preis::Sonderkosten*>(pDrucklisteRohrwaCollector->get_preis().get_sokolist().GetNext(upos));
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
			pDrucklisteRohrwaCollector->get_preis().get_sokolist().AddTail(s);
		}
	}
}

void DrucklistenVerwaltung::set_passlist(const CObList& value)
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
		for (upos = pDrucklisteRohrwaCollector->get_passlaengenListe().GetHeadPosition(); upos;)
		{
			Element *elem = dynamic_cast<Element*>(pDrucklisteRohrwaCollector->get_passlaengenListe().GetNext(upos));
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
			pDrucklisteRohrwaCollector->get_passlaengenListe().AddTail(e);
		}
	}
}

//Liste kopieren und wenn sort = true, dann kumulieren
void DrucklistenVerwaltung::set_pulverlist(EnCObListVk3& value, bool sort)
{
	EnCObListVk3 tmpListe;

	EnCObListVk3& liste = pDrucklisteRohrwaCollector->get_pulverlaengenListe();

	//Kopie der Liste 
	tmpListe.copy(value);

	//Liste anh�ngen
	liste.appendListe(tmpListe);

	if (sort)
		liste.kumulieren();

}


//Liste kopieren und wenn sort = true, dann kumulieren
void DrucklistenVerwaltung::set_elelist(EnCObListVk3& value, bool sort)
{
	EnCObListVk3 tmpListe;

	EnCObListVk3& liste = pDrucklisteRohrwaCollector->get_vk3elementeListe();

	//Kopie der Liste 
	tmpListe.copy(value);

	liste.appendListe(tmpListe);

	if (sort)
		liste.kumulieren();

}
///Liste auf Preis 0,00� pruefen
void DrucklistenVerwaltung::check_NullEuro()
{
	CString nulleuro;
	for (POSITION pos = pDrucklisteRohrwaCollector->get_vk3elementeListe().GetHeadPosition(); pos != NULL;) 
	{
		Element *ele = dynamic_cast<Element*>(pDrucklisteRohrwaCollector->get_vk3elementeListe().GetNext(pos));
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

// reset setzt die Klassenattribute auf 0
void DrucklistenVerwaltung::reset()
{
	anzahlRohre = 0;
	anzahlElemente = 0;
	maxlaenge = 0;
	gesamtlaenge = 0;
	gesamtgewicht = 0;
	gesamtzeit = 0;

	delete pDrucklisteRohrwaCollector;
	pDrucklisteRohrwaCollector = new Rohrleitung;

	for (POSITION pos = elelist.GetHeadPosition(); pos;)
		delete elelist.GetNext(pos);
	elelist.RemoveAll();

}



// deleteListen, loescht die Listen im Objekt des Werkauftrages


// setGesamtpreise, berechnet alle Preise des Werkauftragobjekts
void DrucklistenVerwaltung::setGesamtpreise()
{
	// Changed-Flag auf false, damit nicht makeRohrleitung etc. aufgerufen wird 
	// und dadurch eine leere Stueckliste, Afoliste und Sokoliste erzeugt wird
//	rohrwa->set_changed(false);

	// Muss wegen Beschichtungsabfrage fuer Addition des Material- und Pulverpreises bei F&F
	pDrucklisteRohrwaCollector->set_beschichtung(cur_werkauftrag()->get_beschichtung());

	// alle Preise berechnen
	pDrucklisteRohrwaCollector->get_preis().calc_all();
}

//Durchl�uft die walist und errechnet die Gesamtgewichte und die maximale L�nge
void DrucklistenVerwaltung::setMaxLaengeUndGewicht()
{
	for (POSITION pos = walist.GetHeadPosition(); pos; )
	{
		werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetNext(pos));
		XASSERT(werkauftrag);
		if (!werkauftrag)
			continue;
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	
		gesamtgewicht		+= werkauftrag->get_material().get_gewicht(0, true);
		
		if (rohr)
		{
			gesamtgewicht		+= rohr->getGesGewicht();
		}
		
		if (werkauftrag->get_maxlaenge() > maxlaenge)
			maxlaenge = werkauftrag->get_maxlaenge();
	}
}


void DrucklistenVerwaltung::setVK3Gesamtpreis()
{
	
	// alle Preise berechnen
	pDrucklisteRohrwaCollector->get_preis().calc_gesamtpreisVK3();
}



//get currentAusgabe
const int DrucklistenVerwaltung::get_currentAusgabe() const
{
	return currentAusgabe;
}



//set currentAusgabe
void DrucklistenVerwaltung::set_currentAusgabe(int value)
{
	currentAusgabe = value;
}

// get lastAusgabe
int DrucklistenVerwaltung::get_lastAusgabe() const
{
	return lastAusgabe;
}

//set lastAusgabe
void DrucklistenVerwaltung::set_lastAusgabe(int value)
{
	lastAusgabe = value;
}

const vector<int>& DrucklistenVerwaltung::get_menuitems() const
{
	return menuitems;
}

void DrucklistenVerwaltung::set_ziel(const CString& z)
{
	ziel = z;
}


void DrucklistenVerwaltung::set_dn2zoll(int menuitem)
{
	switch (menuitem)
	{
		case GEWINDEROHRLISTE:
		case GEWINDEROHRE_LAENGE:
		case GEWINDEROHRE_AB_10_STUECK:	
		case GEWINDEROHRE_BIS_10_STUECK:	
		case GEWINDEROHRE_20_STUECK_LAENGE_50_500:
			dn2zoll = (WaxSys::para_int(DN2ZOLL) == 0) ? false : true;
			break;
		default:
			dn2zoll = (WaxSys::para_int(GF_DN2ZOLL) == 0) ? false : true;
	}
}

//get werkauftrag
Werkauftrag* DrucklistenVerwaltung::cur_werkauftrag() 
{
	XASSERT(walist.GetCount());
	if (!walist.GetCount())
		return NULL;

	Werkauftrag *werk = dynamic_cast<Werkauftrag*>(walist.GetHead());
	XASSERT(werk);
	return werk;
}


vector<Werkauftrag*> DrucklistenVerwaltung::get_walist()
{
	vector<Werkauftrag*> wret;

	for (POSITION pos = walist.GetHeadPosition(); pos != NULL; )
	{
		wret.push_back(dynamic_cast<Werkauftrag*>(walist.GetNext(pos)));
	}

	return wret;
}

bool DrucklistenVerwaltung::init_material_menu() 
{
	vector<Werkauftrag*> walst = get_walist();
	for (vector<Werkauftrag*>::iterator it = walst.begin(); it != walst.end(); it++)
	{
		Werkauftrag* werkauftrag = *it;
		if (!werkauftrag)
			return false;

		if ((werkauftrag->isEmpty() || dynamic_cast<SammelWerkauftrag*>(werkauftrag)) && (walst.size() > 1))
			continue;
		
		
		// Materialbestellungen Listen werden nur eingef�gt, wenn auch wirklich Material und 
		// Arbeit da ist. Dies ist z.B. bei der Projektleitkarte oder der Gliederung nicht der Fall.
		if (!dynamic_cast<Rohrleitung*>(werkauftrag))
		{
			menuitems.push_back(GRAPHIKDRUCK);
			menuitems.push_back(SEPARATOR);
			
			if (!werkauftrag->get_material().isEmpty())
			{
				menuitems.push_back(MATERIALLISTE_GESAMT);
			}

			if (WaxSys::isAV())
			{
				if (!werkauftrag->get_arbeit().isEmpty())
				{
					menuitems.push_back(ARBEITSPLAN);
					
				}
				menuitems.push_back(SEPARATOR);
				menuitems.push_back(WAP_VOLLKOSTEN);

				if (WaxSys::isAvAdmin())
					menuitems.push_back(WAP_GRENZKOSTEN);
				
				menuitems.push_back(SEPARATOR);
				
				DrucklisteZuschnitt z(html, waxpart, currentAusgabe);
				z.add_drucklisten(menuitems);	
			}
			if (!werkauftrag->get_material().isEmpty())
			{
				CObList proflist;
				if (werkauftrag->get_material().get_profilliste(proflist, Element::USTAHL | Element::FSTAHL | Element::LSTAHL))
				{
					menuitems.push_back(SEPARATOR);
					menuitems.push_back(PROFILLISTE_38010);
					menuitems.push_back(PROFILLISTE_38040);
				}
			}
			return true;
		}
	}
	return false;
}