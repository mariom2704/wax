#include "stdafx.h"

#include "DialogWaxHtmlFile.h"

#include "Dialogdruckliste.h"

#include "Dialogdrucklistezuschnitt.h"
#include "drucklistenverwaltung.h"
#include "../rohrkonst/rohrleitung.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;

DialogDrucklisteZuschnitt::DialogDrucklisteZuschnitt(DialogHtmlDruck& file, WaxPart* pt, int id) 
: DialogDruckliste(file, pt)
{
	schweissung = both;
	listid = id;
	default_rohr = NULL; // Wird erzeugt, wenn Apax Aufträge Rohr enthalten
}


DialogDrucklisteZuschnitt::~DialogDrucklisteZuschnitt()
{
	delete default_rohr;
}

CObList* DialogDrucklisteZuschnitt::listefromid(int id)
{
	/**
	Liefert für die Druckliste id die passende Maschinenliste
	*/
	CObList* ret = NULL;
	switch (id)
	{
		case DrucklistenVerwaltung::GEWINDEROHRLISTE:
			ret = &maschinenlisten->zuschnittliste_Gewinde_gesamt;
			break;

		case DrucklistenVerwaltung::GEWINDEROHRE_20_STUECK_LAENGE_50_500:
			ret = &maschinenlisten->zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030;
			break;
						
		case DrucklistenVerwaltung::GEWINDEROHRE_AB_10_STUECK:
			ret = &maschinenlisten->zuschnittliste_Gewinde_ab10Stueck_32070;
			break;
		
		case DrucklistenVerwaltung::GEWINDEROHRE_BIS_10_STUECK:
			ret = &maschinenlisten->zuschnittliste_Gewinde_bis10Stueck_31010;
			break;
			
		case DrucklistenVerwaltung::GEWINDEROHRE_LAENGE:
			ret = &maschinenlisten->zuschnittliste_Gewinde_gesamt;
			break;
		
		case DrucklistenVerwaltung::ZUSCHNITTSLISTE_32060:
			ret = &maschinenlisten->zuschnittliste_32060;
			break;
		case DrucklistenVerwaltung::ZUSCHNITTSLISTE_32020:
			ret = &maschinenlisten->zuschnittliste_32020;
			break;
		case DrucklistenVerwaltung::ZUSCHNITTSLISTE_32070:
			ret = &maschinenlisten->zuschnittliste_32070;
			break;
		case DrucklistenVerwaltung::ZUSCHNITTSLISTE_38030_HANDSW:
			schweissung = hand;
			ret = &maschinenlisten->zuschnittliste_38030_handsw;
			break;
		case DrucklistenVerwaltung::ZUSCHNITTSLISTE_38030_ANLAGE:
			schweissung = anlage;
			ret = &maschinenlisten->zuschnittliste_38030_anlage;
			break;
		case DrucklistenVerwaltung::ZUSCHNITTSLISTE_EK:
			schweissung = anlage;
			ret = &maschinenlisten->zuschnittliste_EK;
			break;
	}
	return ret;
}

//Erzeugt für alle Werkaufträge die Maschinenlisten und prüft, ob irgendein 
//WA eine der Maschinenlisten füllt. Dann wird der Menüeintrag in die 
//(Menü)listen eingefügt.
void DialogDrucklisteZuschnitt::add_drucklisten(vector<int>& menuitems) const
{

	// Die Maschinenlisten werden für jeden Werkauftrag erzeugt, dann wird geprüft, ob
	// eine Liste vorhanden ist. Wenn ja wird der Menüpunkt einmal hinzugefügt.
	const CObList* lsts[] = 
	{ 
		&maschinenlisten->zuschnittliste_38030_handsw, 
		&maschinenlisten->zuschnittliste_38030_anlage,
		&maschinenlisten->zuschnittliste_32060,
		&maschinenlisten->zuschnittliste_32020,
		&maschinenlisten->zuschnittliste_32070, 
		&maschinenlisten->zuschnittliste_EK,
		&maschinenlisten->zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030,
		&maschinenlisten->zuschnittliste_Gewinde_ab10Stueck_32070,
		&maschinenlisten->zuschnittliste_Gewinde_bis10Stueck_31010,
		NULL 
	};
	UINT menuentry[] = 
	{ 
		DrucklistenVerwaltung::ZUSCHNITTSLISTE_38030_HANDSW,
		DrucklistenVerwaltung::ZUSCHNITTSLISTE_38030_ANLAGE,
		DrucklistenVerwaltung::ZUSCHNITTSLISTE_32060,
		DrucklistenVerwaltung::ZUSCHNITTSLISTE_32020,
		DrucklistenVerwaltung::ZUSCHNITTSLISTE_32070,
		DrucklistenVerwaltung::ZUSCHNITTSLISTE_EK,
		DrucklistenVerwaltung::GEWINDEROHRE_20_STUECK_LAENGE_50_500,
		DrucklistenVerwaltung::GEWINDEROHRE_AB_10_STUECK,
		DrucklistenVerwaltung::GEWINDEROHRE_BIS_10_STUECK
	};
	
	for (int i = 0; lsts[i]; i++)
	{
		if (!lsts[i]->IsEmpty())
		{
			if (menuentry[i]) 
			{
				menuitems.push_back(menuentry[i]);
				menuentry[i] = 0; // Jeden Menüeintrag nur einmal einfügen
			}
		}
	}

	// Bei Apax wird noch die leere Zuschnittsliste benötigt
	if (!maschinenlisten->rohr)
		menuitems.push_back(DrucklistenVerwaltung::ZUSCHNITT_LEER);

}

bool DialogDrucklisteZuschnitt::create(Werkauftrag* werkauf)
{
	CObList* liste = listefromid(listid);

	
	const Rohrleitung* rohr = maschinenlisten->rohr;
/*
	// Bei Apax muss die Materialliste nach Rohren durchsucht werden
	if (!rohr)
	{
		delete default_rohr;
		rohr = default_rohr = scan_rohr(twoquali);
	}


	CString ueberschrift; ueberschrift = getStammdaten().getText("IDS_BEMERKUNG").c_str();
	CString bemerkung;
	string stdBemerkung;
	if (rohr)
	{
		bemerkung = rohr->get_bemerkung();
		if (rohr && dynamic_cast<const COIIVerteiler*>(rohr))
		{
			stdBemerkung = rohr->get_stdbemerkung();
		}
	}
	html.insert_bemerkung((LPCSTR)bemerkung, (LPCSTR)ueberschrift, stdBemerkung);
	
	insert_rohr_quali(rohr, listid == DrucklistenVerwaltung::ZUSCHNITTSLISTE_38030_ANLAGE ? maschinenlisten->stutzenAnlage2440 : 0);
	*/
	html.insertListeUnterschriften();

	/*
	Druckt eine Liste mit Rohren, die Nuten am Anfang oder Ende enthalten.
	*/
	// Tabellenspalten	
	const int rows[] = { 35, 35, 75, 180, /*80,*/ 80, 75, 60, 60, 130};

	CString txt, str, str2;

	DialogWaxHtmlFile &w = html.get_file();
	
	int spalte = 0;

	w.startTable();
	w.startRow();

	w.appendCol(w.bold_string_ids("IDS_POSNR_ABK"), rows[spalte++]);
	w.appendCol(w.bold_string_ids("IDS_WANR"), rows[spalte++]);
	w.appendCol(w.bold_string_ids("IDS_ANZ_STK"), rows[spalte++], "CENTER");
	w.appendCol(w.bold_string_ids("IDS_ROHR"),rows[spalte++], "CENTER");
	
	if (rohr && dynamic_cast<const COIIVerteiler*>(rohr))
	{
		w.appendCol(w.bold_string_ids("IDS_ROHR_STAERKE"),rows[spalte++], "CENTER");
	}

	//if (rohr && !dynamic_cast<const COIIVerteiler*>(rohr))
	{
		w.appendCol(w.bold_string_ids(dn2zoll ? "IDS_NENNW_ZOLL" : "IDS_NENNW_DN"), rows[spalte++], "CENTER");
	}

	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	if (rohr && dynamic_cast<const Handelslaenge*>(rohr))
		str = "ca." + str;

	w.appendCol(w.bold_string((LPCSTR)str), rows[spalte++], "CENTER");
	w.appendCol(w.bold_string_ids("IDS_ANFANG"), rows[spalte++], "CENTER");
	w.appendCol(w.bold_string_ids("IDS_ENDE"),rows[spalte++], "CENTER");
	
	//if (rohr && !dynamic_cast<const COIIVerteiler*>(rohr))
	{
		w.appendCol(w.bold_string_ids("IDS_KLINKEN"), rows[spalte++], "CENTER");
	}

	w.endRow();
	w.endTable();


	// Bei Druckliste 'Leere Zuschnittsliste' ist hier Schluss
	if (!liste || liste->IsEmpty())
		return false;

	bool testAnlage = true;

	CObList list;
	for (POSITION pos = liste->GetHeadPosition(); pos != NULL;)	
	{
		Element *element, *elementAnfang, *elementEnde;

		element = dynamic_cast<Element*>(liste->GetNext(pos));

		
		testAnlage = true;
		if (schweissung >= 0)
		{
			if ((int)element->get_makeAnlage() != schweissung)
				testAnlage = false;
		}
		
		if (((element->get_id() == ROHR || element->isEleRohr() || element->get_id() == STDSTUTZEN) && testAnlage) ||
		(element->get_id() == ROHR || element->isEleRohr() && element->get_dn() > 300) || (rohr && dynamic_cast<const COIIVerteiler*>(rohr))
			)
		{
			
			elementAnfang = dynamic_cast<Element*>(liste->GetNext(pos));
			elementEnde = dynamic_cast<Element*>(liste->GetNext(pos));
/*
			// Prüfung: gleiches Rohr schon in Liste?
			
			POSITION pp;
			for (pp = list.GetHeadPosition(); pp;)	
			{
				Element *ele, *eleAnfang, *eleEnde;
				
				ele = dynamic_cast<Element*>(list.GetNext(pp));
				eleAnfang = dynamic_cast<Element*>(list.GetNext(pp));
				eleEnde = dynamic_cast<Element*>(list.GetNext(pp));

				if (element->get_artikelnr() == ele->get_artikelnr() 
				 && element->getLaengeZuschnitt()	 == ele->getLaengeZuschnitt()
				 && elementAnfang->get_id()  == eleAnfang->get_id()
				 && elementEnde->get_id()	 == eleEnde->get_id()
				 && element->get_rohrPosNr() == ele->get_rohrPosNr())
				{
					ele->set_anzahl(ele->get_anzahl() + element->get_anzahl()); // Anzahl hochsetzen
					pp = list.GetHeadPosition(); // pp dient als Flag für nachfolgende Abfrage
					break;
				}
			}
			if (!pp)
			*/
			{
				//if(werkauf->get_waid() == element->get_Waid())
				{
					list.AddTail(element);
					list.AddTail(elementAnfang);
					list.AddTail(elementEnde);
				}
			}
		}

	}

	Druckliste::sort_liste(list);

	w.startTable();
	string bezeichnung; // letzte Bezeichnung
	for (POSITION p = list.GetHeadPosition(); p != NULL;)	
	{
		Element *element, *elementAnfang, *elementEnde;

		element = dynamic_cast<Element*>(list.GetNext(p));
		elementAnfang = dynamic_cast<Element*>(list.GetNext(p));
		elementEnde = dynamic_cast<Element*>(list.GetNext(p));
				
		XASSERT(element && elementAnfang && elementEnde);
		if (!element || !elementAnfang || !elementEnde)
			break;
			
		

		w.startRow();


		spalte = 0;

		//Positionsnr.
		w.appendCol(element->get_rohrPosNr(), rows[spalte++] - 5, "LEFT");
		w.appendCol(element->get_WaNr(),rows[spalte++], "LEFT");

		//Anzahl
		// Pos wurde hier missbraucht, um die Anzahl der Rohrleitungen zu merken 
		w.appendCol(element->get_anzahl()*element->get_pos(),rows[spalte++], "CENTER");

		//Rohrbezeichnung
		if (element->get_id() == ROHR_Q1  || element->get_id() == ROHR && element->get_urid() == GEW)
		{
			CString bez;
			if (element->get_dn() >= 25 && element->get_dn() <= 50)
			{	bez = CString(element->get_bezeichnung()) + " ";
				bez += getStammdaten().getText("IDS_2440STD").c_str();
			}
			else 
				bez = getStammdaten().getText("IDS_2458STD").c_str();
			element->set_bezeichnung(bez);
		}
		
		if (element->get_id() == ROHR_Q11)
		{
			CString bez;
			if (element->get_dn() < 65)
			{
				bez = getStammdaten().getText("IDS_2440STD").c_str();
				
			}
			else 
				bez = getStammdaten().getText("APSAD").c_str();
			
			element->set_bezeichnung(bez);
		}


		if (element->get_id() == ROHR_Q16 || element->get_id() == ROHR_Q18 || element->get_id() == ROHR_Q19)
		{
			CString txt = getStammdaten().getSapArtikelbezeichnung(element->get_artikelnr());
			element->set_bezeichnung(txt);
		}

		if (rohr && dynamic_cast<const COIIVerteiler*>(rohr) && element->get_id() == ROHR_Q8)
		{
			CString txt = getStammdaten().getSapArtikelbezeichnung(element->get_artikelnr());
			element->set_bezeichnung(txt);
		}
		// Nur ausgeben, wenn sich die Bezeichnung geändert hat
		if ((string)(LPCSTR)element->get_bezeichnung() != bezeichnung)
		{
			if( element->get_id() == ROHR_Q10)
			{
				w.appendColBold((string)(LPCSTR)element->get_bezeichnung(), rows[spalte++], "center", false);
				//w.appendCol(element->get_bezeichnung(),rows[spalte++]);
			}
			else
			{
				w.appendCol(element->get_bezeichnung(),rows[spalte++]);
			}
			bezeichnung = (LPCSTR)element->get_bezeichnung();
		}
		else
			w.appendCol("\"",rows[spalte++], "CENTER");

		if (rohr && dynamic_cast<const COIIVerteiler*>(rohr))
		{
			CString txt = getStammdaten().getSapArtikelbezeichnung(element->get_artikelnr());
			int iWo1 = txt.Find(" ");
			int iWo2 = txt.Find("x");
			if(iWo2 == -1)
			{
				iWo2 = txt.Find("X");
			}
			iWo2 = txt.Find(",", iWo2+1);
			CString strText = txt.Mid(iWo1+1, iWo2+1-iWo1);
			w.appendCol(strText,rows[spalte++], "CENTER");
		}

	//	if (rohr && !dynamic_cast<const COIIVerteiler*>(rohr))
		{
			//Dn bzw. Zoll Wert
			CString dnWert;
			if (!dn2zoll)
				dnWert.Format("%d",element->get_dn());
			else
				dnWert = WaxStamm::dn2zoll(element->get_dn());

			w.appendCol(dnWert,rows[spalte++], "CENTER");
		}
		//Rohrlänge
		CString txt;
		txt.Format("%d", element->getLaengeZuschnitt());	
		if (dynamic_cast<const Handelslaenge*>(rohr))
			txt = "~ " + txt;
		w.appendCol(element->getLaengeZuschnitt(),rows[spalte++], "CENTER");

		//Rohranfang
		if (elementAnfang->get_id() == FASE_COII || elementAnfang->get_id() == NUT || elementAnfang->get_id() == GEW || elementAnfang->get_id() == GLATT)
			w.appendCol(elementAnfang->get_bezeichnung(),rows[spalte++], "CENTER");
		else
		{
			str = getStammdaten().getText("IDS_GLATT").c_str();
			w.appendCol(str,rows[spalte++], "CENTER");
		}

		//Rohrende
		if (elementEnde->get_id() == FASE_COII || elementEnde->get_id() == NUT || elementEnde->get_id() == GEW || elementEnde->get_id() == GLATT)
			w.appendCol(elementEnde->get_bezeichnung(),	rows[spalte++], "CENTER");
		else
		{
			str = getStammdaten().getText("IDS_GLATT").c_str();
			w.appendCol(str,rows[spalte++], "CENTER");
		}


		//Klinken
		//if (rohr && !dynamic_cast<const COIIVerteiler*>(rohr))
		{
			int radius = 0;
			if (element->get_id() == ROHR && !element->get_makeAnlage())
			{
				AbzugRadius ar;
				radius = ar.getRadius(element->get_dn(), element->get_anschlussDN());
			}

			txt = "-";
			if (radius != 0)
			{
				str = getStammdaten().getText("IDS_AUF_VL_DN").c_str();
				txt.Format(str, element->get_anschlussDN(), radius);
			}
			w.appendCol(txt,rows[spalte++], "CENTER");
		}
		w.endRow();
	}
	w.endTable();
	return true;
}

Rohrleitung* DialogDrucklisteZuschnitt::scan_rohr(bool& mehrereRohrquali)
{
	/**
	Durchsucht die Materialliste nach Rohr. Ist Rohr vorhanden, wird 
	eine Rohrleitung mit der entsprechenden Rohrqualität zurückgeliefert.
	Sind verschiedene Rohrqualitäten vorhanden, wird mehrereRohrquali auf
	true gesetzt.
	*/
	Rohrleitung* ret = NULL;
	map<Rohrleitung::IDS, int> rohrqualitaeten;
	for (Auftragsliste::const_iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{
		const Werkauftrag* w = *it;
		const MxCObList& lst = w->get_material().get_matlist(); 
		
		for (POSITION p = lst.GetHeadPosition(); p;)
		{
			const Material* m = dynamic_cast<const Material*>(lst.GetNext(p));
			if (m)
			{
				Rohrleitung::IDS ids = Rohrleitung::elementid2rohrquali((Element::IDS)m->get_id());
				if (ids != Rohrleitung::LEER)
				{
					rohrqualitaeten.insert(make_pair<Rohrleitung::IDS, int>(ids, m->get_id()));
				}
			}
		}
	}
	if (rohrqualitaeten.size() > 0)
	{
		ret = new Rohrleitung;
		ret->initDaten(1, 32, 1000, "1", rohrqualitaeten.begin()->first, 0);
		ret->get_leitkarte().copy((*auftraege.begin())->get_leitkarte());
		mehrereRohrquali = rohrqualitaeten.size() > 1;
	}
	return ret;
}