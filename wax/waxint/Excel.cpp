#include "stdafx.h"


#include "resource.h"
#include "grohrauswahl.h"
#include "..\rohrkonst\elementlist.h"
#include "BefehlZusammenfassungWerkauftraegeRobbi.h"
#include "GpAnlageSaegeNut.h"
#include "GpAnlageVerteiler.h"
#include "ExcelODBC.h"




#include <mxcontrols/msgbox.h>
using namespace MxGui;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Excel::Excel()
{ 
	
}



Excel::Excel(Werkauftrag* wa) : werkauftrag(wa), currentAction(0)
{ 
}



void Excel::setExcelTableOfficeVersion2010(bool bOfficeVersion)
{
	setExcelTableOffice2010(bOfficeVersion);
};



Excel::~Excel()
{
//Destruktor
}

const CWordArray& Excel::get_aktionen()
{
	return aktionen;
}

void Excel::set_currentAction(int value)
{
	currentAction = value;
}

Werkauftrag* Excel::get_werkauftrag()
{
	if (walist.GetCount() > 0)
	{
		Werkauftrag *werk = dynamic_cast<Werkauftrag*>(walist.GetHead());
		XASSERT(werk);
		if (!werk)
			return werkauftrag;
		else
			return werk;
	}
	else
	{
		walist.AddHead(werkauftrag);
		return werkauftrag;
	}
}

void Excel::set_werkauftrag(Werkauftrag* value)
{
	werkauftrag = value;
	Werkauftrag *werk = walist.IsEmpty() ? NULL : dynamic_cast<Werkauftrag*>(walist.GetHead());
	if (werk != value)
		walist.AddHead((CObject*)value);
}

void Excel::initAction()
{
/*
Liste mit Action-IDs (aktionen) und Aktionsnamen initialisieren.
"aktionen" enthaelt die fuer den jeweiligen Werkauftrag gueltigen Action-IDs. 
*/
	aktionen.RemoveAll();
	
	// Unterobjekte durchgehen bis ein Werkauftrag kein Sammelauftrag ist,
	// sonst werden die Menuepunkte fuer Rohrleitung nicht angezeigt.
	if (walist.GetCount() > 0)
	{
		for (POSITION pos = walist.GetHeadPosition(); pos != NULL;)
		{
			werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetNext(pos));
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if (!werkauftrag)
				continue;
			if (!werkauftrag->isEmpty() && !dynamic_cast<SammelWerkauftrag*>(werkauftrag)) 
				break;
		}

		aktionen.Add(AUFTRAGSTABELLE);
		
		if (dynamic_cast<Rohrleitung*>(werkauftrag))
		{
			if (WaxSys::isAV())
			{
				if (dynamic_cast<Verteiler*>(werkauftrag) ||
					dynamic_cast<Stationsverteiler*>(werkauftrag) ||
					dynamic_cast<Pumpenverteiler*>(werkauftrag) ||
					dynamic_cast<Pumpenprobierleitung*>(werkauftrag))
				{
					aktionen.Add(SAEGENUTANLAGE);
					aktionen.Add(VERTEILERANLAGE);
				}
				if (dynamic_cast<Strangrohr*>(werkauftrag))
				{
					aktionen.Add(STRANGROHRANLAGE_1);
				}
			}
		}

	}
	
	if (WaxSys::isAV())
		aktionen.Add(ZUSAMMENFASSUNG_WERKAUFTRAEGE);
}

CString Excel::getName(int id)
{
/*
Liefert den Namen (für Menueeintrag) fuer eine Aktion. 
@todo Mehrsprachlichkeit einbauen fuer alle Aktionen, die in den NL benutzt werden
*/
	if (aktionen.GetSize() == 0)
		initAction();

	CString str, txt;
	switch (id) 
	{
	case AUFTRAGSTABELLE:
		str = getStammdaten().getText("IDS_AUFTRAGSTABELLE").c_str();
		break;
	case SAEGENUTANLAGE:
		str = getStammdaten().getText("IDS_FREIGABE_SAEGENUT").c_str();
		break;
	
	case VERTEILERANLAGE:
		str = getStammdaten().getText("IDS_FREIGABE_VERTEILER").c_str();
		break;
	case STRANGROHRANLAGE_1:
		str = getStammdaten().getText("IDS_FREIGABE_STRANG").c_str();
		break;
	case STRANGROHRANLAGE_2:
		txt = getStammdaten().getText("IDS_FREIGABE_STRANG").c_str();
		str.Format(txt, 2);
		break;
	case STRANGROHRANLAGE_3:
		txt = getStammdaten().getText("IDS_FREIGABE_STRANG").c_str();
		str.Format(txt, 3);
		break;
	case STRANGROHRANLAGE_4:
		txt = getStammdaten().getText("IDS_FREIGABE_STRANG").c_str();
		str.Format(txt, 4);
		break;
	case PREISTEST:
		str = getStammdaten().getText("IDS_PREISTEST").c_str();
		break;
	case ZUSAMMENFASSUNG_WERKAUFTRAEGE:
		str = getStammdaten().getText("IDS_ZUSAMMENFASSUNG_WERKAUFTRAEGE").c_str();
		break;
	default:
		XASSERT(FALSE);
		str = getStammdaten().getText("IDS_UNBEKANNTE_AKTION").c_str();
		break;
	}
	
	return str;
}

/// Sortieren der Werkauftraege
void Excel::sortWerkauftraege(int actionid)
{
	if (actionid == AUFTRAGSTABELLE)
	{
		POSITION pos, nextpos; 
   
		for (int i=0; i < walist.GetCount()-1; i++)	
		{
			pos = walist.FindIndex(i);
			Werkauftrag *first = dynamic_cast<Werkauftrag *>(walist.GetAt(pos));
			nextpos = walist.FindIndex(i+1);
			Werkauftrag *second = dynamic_cast<Werkauftrag *>(walist.GetAt(nextpos));

			XASSERT(first && second);
			if (!first || !second)
				return;
			if (first->get_status().get_wanr() > second->get_status().get_wanr())
			{	
				// Werkauftraege werden vertauscht
				walist.SetAt(nextpos, first);
				walist.SetAt(pos, second);
				i = -1; // Wieder von vorn beginnen
			}
		}
	}
}

void Excel::makeAction(int id)
{
/*
Gewuenschte Aktion ausfuehren
ID gibt die auszufuehrende Aktion an. Default ist id = -1, was zur Folge hat, dass der Wert
von currentAction benutzt wird. Ist currentAction nicht gesetzt, wird die erste in aktionen definierte 
Aktion als Default gesetzt und ausgefuehrt.  
*/
	CString fn;

	zeit.RemoveAll();
	zeitSegment.RemoveAll();
	zeitPulvern.RemoveAll();
	anzRohrstuecke.RemoveAll();
	anzRohrstraenge.RemoveAll();
	anzahl.RemoveAll();
	dnvl.RemoveAll();
	laenge.RemoveAll();
	rohrlaengen.RemoveAll();
	quali.RemoveAll();

	XASSERT(werkauftrag);
	if (!get_werkauftrag())
		return;
	int actionid = id == -1 ? currentAction : id;

	// Keine Auswahl getroffen: In der Sapvorschau wurde eine andere Position ausgewählt. 
	if (currentAction < 0) 
	{ // Automatisch default initialisieren
		initAction();
		XASSERT(aktionen.GetSize());
		if (aktionen.GetSize())
			actionid = currentAction = aktionen.GetAt(1);
	}


	int zul= werkauftrag->get_leitkarte().get_zulassungsnummer();
	werkauftrag->set_zulassungsstelle(zul);

	// In der Auftragstabelle sollen die Werkauftraege nach WAnr sortiert aufgelistet werden
	sortWerkauftraege(actionid);

	// In den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
	// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
	ZeitGesamt = 0;
	for (int i = 0; i < walist.GetCount(); i++)
	{
		werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetHead());
		XASSERT(werkauftrag);
		if (!werkauftrag)
		{
			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
			continue;
		}
		else
		{
			// Der erste Werkauftrag kann ein Sammelauftrag sein. 
			if ((werkauftrag->isEmpty() || dynamic_cast<SammelWerkauftrag*>(werkauftrag)) && (walist.GetCount() > 1))
			{
				CObject *tmp = walist.GetHead();
				walist.RemoveHead();
				walist.AddTail(tmp);
				continue;
			}

			switch (actionid) 
			{
			case AUFTRAGSTABELLE:
				createAuftragsdata();
				break;
			case SAEGENUTANLAGE:
				createSaegenutdata();
				break;
			case VERTEILERANLAGE:
				if (!createVerteilerdata())
					return;
				break;
			case STRANGROHRANLAGE_1:
			case STRANGROHRANLAGE_2:
			case STRANGROHRANLAGE_3:
			case STRANGROHRANLAGE_4:
				createStrangrohrdata();
				break;
			}

			CObject *tmp = walist.GetHead();
			walist.RemoveHead();
			walist.AddTail(tmp);
		}
	}
	
	if (walist.GetCount() == 0)
	{
		switch (actionid) 
		{
		case AUFTRAGSTABELLE:
			createAuftragsdata();
			break;
		case SAEGENUTANLAGE:
			createSaegenutdata();
			break;
		case VERTEILERANLAGE:
			createVerteilerdata();
			break;
		case STRANGROHRANLAGE_1:
		case STRANGROHRANLAGE_2:
		case STRANGROHRANLAGE_3:
		case STRANGROHRANLAGE_4:
			createStrangrohrdata();
			break;
		}
	}

	GpAnlageSaegeNut saegeNut(&walist);
	GpAnlageVerteiler verteiler(&walist);
	
	//GpAnlageVerteiler verteiler(walist);
	switch (actionid) 
	{
	case AUFTRAGSTABELLE:
		createAuftragstabelle("");
		break;
	case SAEGENUTANLAGE:
		//Werte für Sägenutanlage und Verteilerrohrstrasse in Tabelle schreiben
		if (!saegeNut.createSaegeNutVerteilerDb())
			AfxMessageBox(getStammdaten().getText("IDS_SAEGENUT_FEHLER").c_str(), MB_OK | MB_ICONERROR);
		else
			AfxMessageBox(getStammdaten().getText("IDS_SAEGENUT_FREIGEGBEN").c_str(), MB_OK);

		verteiler.createVerteilerXls();
		break;
	
	case VERTEILERANLAGE:
		//neue Xls Datei
		verteiler.createVerteilerXls(true);
		break;
	
	case STRANGROHRANLAGE_1:
		createStrangrohrtabelle();
		break;
	case STRANGROHRANLAGE_2:
		createStrangrohrtabelle();
		break;
	case STRANGROHRANLAGE_3:
		createStrangrohrtabelle();
		break;
	case STRANGROHRANLAGE_4:
		createStrangrohrtabelle();
		break;
	case ZUSAMMENFASSUNG_WERKAUFTRAEGE:
		createZusammenfassungAuftraegeRobbi();
		break;
	}
}


/**
Befehl zum Zusammenfassen der Werkaufträge für die Strangrohrstrasse aufrufen
*/
bool Excel::createZusammenfassungAuftraegeRobbi()
{
	BefehlZusammenfassungWerkauftraegeRobbi befehl;
	return befehl.start();
}

int Excel::createAuftragsdata()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	//XASSERT(rohr);

	// WA-Typ
	Verteiler*				vert		= dynamic_cast<Verteiler*>(get_werkauftrag());
	Strangrohr*				strang		= dynamic_cast<Strangrohr*>(get_werkauftrag());
	Stationsverteiler*		stat		= dynamic_cast<Stationsverteiler*>(get_werkauftrag());
	Gewinderohr*			grohr		= dynamic_cast<Gewinderohr*>(get_werkauftrag());
	Handelslaenge*			hl			= dynamic_cast<Handelslaenge*>(get_werkauftrag());
	Pumpenverteiler*		pumpvert	= dynamic_cast<Pumpenverteiler*>(get_werkauftrag());
	Pumpenprobierleitung*	ppbleit		= dynamic_cast<Pumpenprobierleitung*>(get_werkauftrag());
	EntnahmeleitungDWB*		entleit		= dynamic_cast<EntnahmeleitungDWB*>(get_werkauftrag());
	FormstueckeFittinge*	form		= dynamic_cast<FormstueckeFittinge*>(get_werkauftrag());
	ApaxWerkauftrag*		apax		= dynamic_cast<ApaxWerkauftrag*>(get_werkauftrag());

	// Sonst werden verschiedene Auftrag faelschlicherweise zusammengefasst und nur der 1. mit Nr. 0 ausgegeben  
	if (WaxSys::isAV() && get_werkauftrag()->get_status().get_wanr() == 0)
	{
		CString str; str = getStammdaten().getText("IDS_ERROR1c").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	if (!WaxSys::isAV() && !get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers() &&  (get_werkauftrag()->get_leitkarte().get_sdAuftragsnr() == 0 || get_werkauftrag()->get_leitkarte().get_sdPositionsnr() == 0))
	{
		CString str; str = getStammdaten().getText("IDS_ERROR2b").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	if (!WaxSys::isAV() && get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers() &&  get_werkauftrag()->get_leitkarte().get_ippeMatNr().IsEmpty())
	{
		CString str; str = getStammdaten().getText("IDS_ERROR2b").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	// Leitkarte
	createLeitkartedata();
	
	// Rohrattribute fuer Ausgabe der Einzelpositionen merken
	if (neuwa)
	{
		if (strang)			watyp.Add(STRANG);
		else if (stat)		watyp.Add(STATVERT);
		else if (hl)		watyp.Add(HL);
		else if (vert)		watyp.Add(VERT);
		else if (pumpvert)	watyp.Add(PUMPVERT);
		else if (ppbleit)	watyp.Add(PPBLEIT);
		else if (entleit)   watyp.Add(ENTLEIT);
		else if (grohr)		watyp.Add(GROHR);
		else if (form)		watyp.Add(FORM);
		else if (apax)		watyp.Add(APAX);
		else				watyp.Add(FormstueckeUndFittinge::UNKNOWN);
	
		// Anzahl der Rohrstraenge
		anzRohrstraenge.Add(get_werkauftrag()->get_anzahl());

		// Gesamte Fertigungszeit fürs Segment 
		if (grohr)
		{
			// todo: Stringvergleich ist nicht gut, besser id für Segment in Tab. Kostst. einfügen
			Kostenstellen k;
			zeitSegment.Add(get_werkauftrag()->get_arbeit().getZeitKostenstelle(k.getNr(Kostenstellen::SCHLOSSEREI)));
		}
		else
		{
			//get_werkauftrag()->get_arbeit().setMindermenge();
			double time= get_werkauftrag()->get_arbeit().getZeitArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz());
	
			if(time>0)
			{
				get_werkauftrag()->get_arbeit().setMindermenge();
				time= get_werkauftrag()->get_arbeit().getZeitArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz());
				zeitSegment.Add(time);
			}
			else
			{
				zeitSegment.Add(time);
			}
		}


		// Zeit nur Pulveranlage
		Kostenstellen k;
		zeitPulvern.Add(get_werkauftrag()->get_arbeit().getZeitKostenstelle(k.getNr(Kostenstellen::BESCHICHTUNG)));
		
		
		
		// Gesamtzeit, nicht wichtig
		zeit.Add(get_werkauftrag()->get_arbeit().getZeit());
		
		if (rohr)
		{
			// Anzahl der Rohrstuecke 
			int count = rohr->getAnzahlRohrstuecke();
			anzRohrstuecke.Add(get_werkauftrag()->get_anzahl()*count);
			
			dnvl.Add(rohr->get_dnvl());
			rohrlaengen.Add(rohr->get_laenge()*get_werkauftrag()->get_anzahl());
			
			Element ele;
			ele.set_id(rohr->get_rohrQuali()+ROHR_Q1-1);
			ele.initStammdaten(rohr->get_beschichtung().isVerzinkt());
			quali.Add(ele.get_bezeichnung());

		}
		else
		{
			anzRohrstuecke.Add(0);
			dnvl.Add(0);
			laenge.Add(0);
			rohrlaengen.Add(0);
			quali.Add(" ");
		}

		CString GUID = get_werkauftrag()->get_GUID();
		if(GUID.IsEmpty()== TRUE)
		{
			UwieGUID.Add("-");
		}
		else
		{
			UwieGUID.Add("x");
		}
		// Muffenanzahl bei Strangrohren
		if (strang)
		{
			muffenID.Add(strang->get_muffenID());
			
			

			
			muffenDN.Add(strang->get_muffenDN());
			muffenAnzahl.Add(strang->getAnzahlMuffen()*get_werkauftrag()->get_anzahl());
			Arbeitsfolge* pArbeitsfolgeStrangrohrstrasse = get_werkauftrag()->get_arbeit().getAfo( Arbeitsfolge::eSTRANGROHR_STRASSE );
			if( dynamic_cast<Arbeitsfolge *>(pArbeitsfolgeStrangrohrstrasse) )
			{
				zeitStrangrohrStrasse.Add(pArbeitsfolgeStrangrohrstrasse->get_zeit());
			}
			else
			{
				zeitStrangrohrStrasse.Add(0);
			}
		} 
		else
		{
			muffenID.Add(0);
			muffenDN.Add(0);
			muffenAnzahl.Add(0);
			zeitStrangrohrStrasse.Add(0);
			
		}
	}
	else
	{
		// Anzahl der Rohrstraenge
		anzRohrstraenge.SetAt(anzRohrstraenge.GetSize()-1, anzRohrstraenge.GetAt(anzRohrstraenge.GetSize()-1)+get_werkauftrag()->get_anzahl());
		
		// Gesamte Fertigungszeit fürs Segment 
		if (grohr)
		{
			Kostenstellen k;
			zeitSegment.SetAt(zeitSegment.GetSize()-1, zeitSegment.GetAt(zeitSegment.GetSize()-1)+get_werkauftrag()->get_arbeit().getZeitKostenstelle(k.getNr(Kostenstellen::SCHLOSSEREI)));
		}
		else
			zeitSegment.SetAt(zeitSegment.GetSize()-1, zeitSegment.GetAt(zeitSegment.GetSize()-1)+get_werkauftrag()->get_arbeit().getZeitArbeitsplatz(Arbeitsplan::get_abpSchweissVorrichtplatz()));

		// Zeit nur Pulveranlage
		Kostenstellen k;
		zeitPulvern.SetAt(zeitPulvern.GetSize()-1, zeitPulvern.GetAt(zeitPulvern.GetSize()-1)+get_werkauftrag()->get_arbeit().getZeitKostenstelle(k.getNr(Kostenstellen::BESCHICHTUNG)));
		
		// Gesamte Fertigungszeit
		zeit.SetAt(zeit.GetSize()-1, zeit.GetAt(zeit.GetSize()-1)+get_werkauftrag()->get_arbeit().getZeit());

		if (rohr)
		{
			// Anzahl der Rohrstuecke 
			anzRohrstuecke.SetAt(anzRohrstuecke.GetSize()-1, anzRohrstuecke.GetAt(anzRohrstuecke.GetSize()-1)+get_werkauftrag()->get_anzahl()*rohr->getAnzahlRohrstuecke());
			
			// Laenge der Rohrleitungen
			rohrlaengen.SetAt(rohrlaengen.GetSize()-1, rohrlaengen.GetAt(rohrlaengen.GetSize()-1)+rohr->get_laenge()*get_werkauftrag()->get_anzahl());
		}


		// Muffenanzahl bei Strangrohren
		if (strang)
		{
			muffenAnzahl.SetAt(muffenAnzahl.GetSize()-1, muffenAnzahl.GetAt(muffenAnzahl.GetSize()-1)+strang->getAnzahlMuffen()*get_werkauftrag()->get_anzahl());
		} 
	}

	return (1);
}

/*
Pakt Auftragsdaten in eine Exceldatei. 
*/
CString Excel::createAuftragstabelle(CString fname)
{
	static CString fn, path;
	if (fname.IsEmpty()) 
	{
        path = WaxSys::para_str(EXCEL);
		// wenn am Ende kein Backslasch, dann einen dazu 
		if (path.Right(1) != "\\")
			path += "\\";
		fn = path;
	} 
	else 
	{
		fn = fname;
	}

#ifndef _DEBUG
	// In Bearbeitung
	if (WaxSys::isAV() && get_werkauftrag()->get_status().get_sachbearbeiterSegment().IsEmpty()) 
	{
		CString str; str = getStammdaten().getText("IDS_HINWEIS1b").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return "";
	}
#endif

	////////// load library //////////	
	if (!isLoaded())
		return fn;

	CString datum = MXDate::GetCurrentTime().Format("%y%m%d_%H%M%S");
	fn += "WAX Aufträge ";
	fn += datum;
	fn += ".xls";

	CString n = path;
	n += "WAX Aufträge ";
	n += datum;
	char* name = n.GetBuffer(1);
	
	CFile file;
	if (file.Open(fn, CFile::modeRead | CFile::shareDenyNone)) 
	{
		file.Close();
		::DeleteFile(fn);
	}

	char* table = "";
	bool createTable = false;
	bool tableVert = false, tableStrang = false, tableStatvert = false, tableGrohr = false;	
	bool tableStdvert = false, tableHl = false, tablePumpvert = false, tablePpbleit = false;
	bool tableEntleit = false, tableForm = false, tableApax = false;
	
	// Auftraege abarbeiten
	for (int i=0; i<wan.GetSize(); i++)
	{
		createTable = false;
		// Welcher Table? 
		// Muss Table erstellt werden?
		if (watyp.GetAt(i) == VERT)	
		{
			table = "Verteiler";
			if (!tableVert)
				createTable = true;
		}
		else if (watyp.GetAt(i) == HL)	
		{
			table = "Handelslängen";
			if (!tableHl)
				createTable = true;
		}
		else if (watyp.GetAt(i) == STDVERT)	
		{
			table = "Standarderteiler";
			if (!tableStdvert)
				createTable = true;
		}
		else if (watyp.GetAt(i) == STRANG)	
		{
			table = "Strangrohr";
			if (!tableStrang)
				createTable = true;
		}
		else if (watyp.GetAt(i) == STATVERT)	
		{
			table = "Stationsverteler";
			if (!tableStatvert)
				createTable = true;
		}
		else if (watyp.GetAt(i) == GROHR)	
		{
			table = "Gewinderohr";
			if (!tableGrohr)
				createTable = true;
		}
		else if (watyp.GetAt(i) == PPBLEIT)	
		{
			table = "Pumpenprobierleitung";
			if (!tablePpbleit)
				createTable = true;
		}
		else if (watyp.GetAt(i) == PUMPVERT)	
		{
			table = "Pumpenverteiler";
			if (!tablePumpvert)
				createTable = true;
		}
		else if (watyp.GetAt(i) == ENTLEIT)	
		{
			table = "EntnahmeleitungDWB";
			if (!tableEntleit)
				createTable = true;
		}
		else if (watyp.GetAt(i) == FORM)	
		{
			table = "FormstückeFittinge";
			if (!tableForm)
				createTable = true;
		}
		else if (watyp.GetAt(i) == APAX)	
		{
			table = "ApaxWerkauftrag";
			if (!tableApax)
				createTable = true;
		}


		if (createTable)
		{
			// Table erstellen
			vector<char*> columnName; 
			vector<int> columnType;  

			if (!WaxSys::isAV())
			{
				columnName.push_back("WA_Nr");
				columnName.push_back("SD_Auftrnr");
				columnName.push_back("SD_Posnr");
				columnName.push_back("Freigabedat");
				columnName.push_back("Auftrbez");
				columnName.push_back("PLZ");
				columnName.push_back("Ort");
				columnName.push_back("Lieferdat");
				columnName.push_back("NLSachbearb");
				columnName.push_back("DNRohr");
				columnName.push_back("AnzRohrstränge");
				columnName.push_back("Beschich");
				columnName.push_back("Gesamtlänge");
				columnName.push_back("DIN");
				columnName.push_back("Zulassung");
				columnName.push_back("Kappenfarbe");
				columnName.push_back("Scheibe");

				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
			}
			else
			{
				columnName.push_back("A");
				columnName.push_back("WA_Nr");
				columnName.push_back("SD_Auftrnr");
				columnName.push_back("SD_Posnr");
				columnName.push_back("KC_Auftrnr");
				columnName.push_back("KC_Posnr");
				columnName.push_back("Freigabedat");
				columnName.push_back("Auftrbez");
				columnName.push_back("PLZ");
				columnName.push_back("Ort");
				columnName.push_back("Lieferdat");
				columnName.push_back("NLSachbearb");
				columnName.push_back("DNRohr");
				columnName.push_back("DNMuffe");
				columnName.push_back("AnzMuffe");
				columnName.push_back("Beschich");
				columnName.push_back("Typ");
				columnName.push_back("Wittenberge");
				columnName.push_back("ZeitSegment");
				columnName.push_back("DIN");
				columnName.push_back("S");
				columnName.push_back("Gesamtlänge");
				columnName.push_back("U");
				columnName.push_back("AnzRohrstränge");
				columnName.push_back("ZeitPulvern");
				
				if(watyp.GetAt(i) == STRANG)
				{
					columnName.push_back("ZeitStrangrohrstrasse");
				}
				
				columnName.push_back("Zulassung");
				columnName.push_back("Kappenfarbe");
				columnName.push_back("Scheibe");

				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELDOUBLE);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELINT);
				columnType.push_back(EXCELDOUBLE);
				
				if(watyp.GetAt(i) == STRANG)
				{
					columnType.push_back(EXCELDOUBLE);
				}
				
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
				columnType.push_back(EXCELTEXT);
			}

			char *columns[200];
			int type[200];

			for (unsigned int j=0; j<columnType.size(); j++)
			{
				columns[j] = columnName.at(j);
				type[j] = columnType.at(j);
			}
			int quantity = columnType.size();	
	
			ExcelTableSetVar(name, table);
			ExcelTableSetColumnNames(columns, quantity);
			ExcelTableSetColumnType(quantity, type);
			ExcelTableCreateTable();
			ExcelTableCloseTable(name, table);

			// Merken, dass Table schon existiert
			if (watyp.GetAt(i) == VERT)	
				tableVert = true;
			else if (watyp.GetAt(i) == STRANG)	
				tableStrang = true;
			else if (watyp.GetAt(i) == STATVERT)	
				tableStatvert = true;
			else if (watyp.GetAt(i) == GROHR)	
				tableGrohr = true;
			else if (watyp.GetAt(i) == HL)	
				tableHl = true;
			else if (watyp.GetAt(i) == STDVERT)	
				tableStdvert = true;
			else if (watyp.GetAt(i) == PUMPVERT)	
				tablePumpvert = true;
			else if (watyp.GetAt(i) == PPBLEIT)	
				tablePpbleit = true;
			else if (watyp.GetAt(i) == ENTLEIT)	
				tableEntleit = true;
			else if (watyp.GetAt(i) == FORM)	
				tableForm = true;
			else if (watyp.GetAt(i) == APAX)	
				tableApax = true;
		}

		ExcelTableOpenTable(name, table);

		char *values[200];

		// Table mit Daten fuellen			
		CString txt1, txt2, txt3, txt4, txt5, txt6;
		CString txt7, txt8, txt9, txt10, txt11, txt12, txt13, txt14, txt15;
		CString idMuffe, dnMuffe, anzMuffe, stkcAuf, stkcPos, strGUID;


		//Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
		//MxString kFarbe= rohr->getKappenfarbe(rohr->get_rohrKFarbe());
	


		if (watyp.GetAt(i) == STRANG)	
		{
			idMuffe.Format("%s", (muffenID.GetAt(i) == BOHR24) ? "Bohr" : "Muffen");
			dnMuffe.Format("%d", muffenDN.GetAt(i));
			anzMuffe.Format("%d", muffenAnzahl.GetAt(i)); 
		}

		strGUID = UwieGUID.GetAt(i);
		CString strAnzRohrstraenge, strDnvl, strLaenge, strAnzRohrstuecke;
		CString strZeit, strZeitSegment, strZeitPulvern, strZeitStrangrohrStrasse;
		strAnzRohrstraenge.Format("%d", anzRohrstraenge.GetAt(i));
		strDnvl.Format("%d", dnvl.GetAt(i));
		strLaenge.Format("%.1f", rohrlaengen.GetAt(i)/1000.0);
		strAnzRohrstuecke.Format("%d", anzRohrstuecke.GetAt(i));
		strZeit.Format("%.4f", zeit.GetAt(i));
		if (WaxSys::isAV())
		{
			strZeitSegment.Format("%.4f", zeitSegment.GetAt(i));
			strZeitPulvern.Format("%.4f", zeitPulvern.GetAt(i));
			if(watyp.GetAt(i) == STRANG)
			{
				strZeitStrangrohrStrasse.Format("%.4f", zeitStrangrohrStrasse.GetAt(i));
			}
		}

		int k=0;

		if (!WaxSys::isAV())
		{
			txt1 = wan.GetAt(i);
			values[k++] = txt1.GetBuffer(1);
			txt2 = sdAuf.GetAt(i);
			values[k++] = txt2.GetBuffer(1);
			txt3 = sdPos.GetAt(i);
			values[k++] = txt3.GetBuffer(1);
			txt4 = freidat.GetAt(i);
			values[k++] = txt4.GetBuffer(1);
			txt5 = auftrbez.GetAt(i);
			values[k++] = txt5.GetBuffer(1);
			txt6 = plz.GetAt(i);
			values[k++] = txt6.GetBuffer(1);
			txt7 = ort.GetAt(i);
			values[k++] = txt7.GetBuffer(1);
			txt8 = wliefdat.GetAt(i);
			values[k++] = txt8.GetBuffer(1);
			txt9 = nlsachb.GetAt(i);
			values[k++] = txt9.GetBuffer(1);
			values[k++] = strDnvl.GetBuffer(1);
			values[k++] = strAnzRohrstraenge.GetBuffer(1);
			txt10 = beschichtung.GetAt(i);
			values[k++] = txt10.GetBuffer(1);
			values[k++] = strLaenge.GetBuffer(1);
			txt11 = quali.GetAt(i);
			values[k++] = txt11.GetBuffer(1);
			txt12 = m_Kolliliste.GetAt(i);
			values[k++] = txt12.GetBuffer(1);

			txt14 = kappenfarbe.GetAt(i);
			values[k++] = txt14.GetBuffer(1);

			txt15 = scheibe.GetAt(i);
			values[k++] = txt15.GetBuffer(1);
		}
		else
		{
			values[k++] = " ";
			txt1 = wan.GetAt(i);
			values[k++] = txt1.GetBuffer(1);
			txt2 = sdAuf.GetAt(i);
			values[k++] = txt2.GetBuffer(1);
			txt3 = sdPos.GetAt(i);
			values[k++] = txt3.GetBuffer(1);

			stkcAuf = kcAuf.GetAt(i);
			values[k++] = stkcAuf.GetBuffer(1);
			stkcPos = kcPos.GetAt(i);
			values[k++] = stkcPos.GetBuffer(1);

			txt4 = freidat.GetAt(i);
			values[k++] = txt4.GetBuffer(1);
			txt5 = auftrbez.GetAt(i);
			values[k++] = txt5.GetBuffer(1);
			txt6 = plz.GetAt(i);
			values[k++] = txt6.GetBuffer(1);
			txt7 = ort.GetAt(i);
			values[k++] = txt7.GetBuffer(1);
			txt8 = wliefdat.GetAt(i);
			values[k++] = txt8.GetBuffer(1);
			txt9 = nlsachb.GetAt(i);
			values[k++] = txt9.GetBuffer(1);
			values[k++] = strDnvl.GetBuffer(1);
			values[k++] = dnMuffe.GetBuffer(1);
			values[k++] = anzMuffe.GetBuffer(1);
			if(strGUID.Compare("-") == 0)
			{
				txt10 = beschichtung.GetAt(i);
			}
			else
			{
				txt10 = "";
			}

			values[k++] = txt10.GetBuffer(1);
			values[k++] = idMuffe.GetBuffer(1);
			values[k++] = strGUID.GetBuffer(1);
			values[k++] = strZeitSegment.GetBuffer(1);
			txt11 = quali.GetAt(i);
			values[k++] = txt11.GetBuffer(1);
			values[k++] = " ";
			values[k++] = strLaenge.GetBuffer(1);
			values[k++] = " ";
			values[k++] = strAnzRohrstraenge.GetBuffer(1);
			values[k++] = strZeitPulvern.GetBuffer(1);
			if(watyp.GetAt(i) == STRANG)
			{
				values[k++] = strZeitStrangrohrStrasse.GetBuffer(1);
			}
			txt12 = m_Kolliliste.GetAt(i);
			values[k++] = txt12.GetBuffer(1);

			txt14 = kappenfarbe.GetAt(i);
			values[k++] = txt14.GetBuffer(1);

			txt15 = scheibe.GetAt(i);
			values[k++] = txt15.GetBuffer(1);


		}
		ExcelTableInsertInto(values);
		ExcelTableCloseTable(name, table);
	}

	CString str; str = getStammdaten().getText("IDS_ERROR3b").c_str();
	
	openFile(fn, str);
	

	return (fn);
}

int Excel::createSaegenutdata()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(this->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return (0);

	// Rohrattribute fuer Ausgabe der Einzelpositionen merken
	posNr.Add(rohr->get_posNr());
	anzahl.Add(rohr->get_anzahl());
	dnvl.Add(rohr->get_dnvl());
	laenge.Add(rohr->get_laenge());
	
	return (1);
}


void Excel::createLeitkartedata()
{
	XASSERT(get_werkauftrag());
	if (!get_werkauftrag())
		return;

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());

	// Leitkarte
	CString strwan, strpspel, strsdAuf, strsdPos, strsdMat, strkcAuf, strkcPos;
	CString strbauabschnitt, strauftrbez, strplz, strort; 
	CString strnl, strnlsachb, strodsachb;
	CString strerstelldat, strfreidat, strbearbdat, strliefdat, strwliefdat;
	CString strKolliliste;


	if (!WaxSys::isAV())
	{
		strsdAuf.Format("%I64d",	get_werkauftrag()->get_leitkarte().get_sdAuftragsnr()); 
		strsdPos.Format("%ld",	get_werkauftrag()->get_leitkarte().get_sdPositionsnr()); 
		neuwa = true;
		for (int i = 0; i < sdAuf.GetSize(); i++)
		{
			if (sdAuf.GetAt(i) == strsdAuf && sdPos.GetAt(i) == strsdPos)
			{
				neuwa = false;
				break;
			}
		}
	}
	else
	{
		strwan.Format("%06ld", get_werkauftrag() ? get_werkauftrag()->get_status().get_wanr() : 0);
		neuwa = true;
		for (int i = 0; i < wan.GetSize(); i++)
		{
			if (wan.GetAt(i) == strwan)
			{
				neuwa = false;
				break;
			}
		}
	}
		
	if (neuwa)
	{
		strwan.Format("%06ld",		get_werkauftrag() ? get_werkauftrag()->get_status().get_wanr() : 0);
		strpspel.Format("%s",		get_werkauftrag()->get_leitkarte().get_prj_pspElement());
		if( get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers() )
		{
			strsdAuf.Format("%s", get_werkauftrag()->get_leitkarte().get_ippeMatNr()); 
		}
		else
		{
			strsdAuf.Format("%I64d",		get_werkauftrag()->get_leitkarte().get_sdAuftragsnr()); 
		}
		strsdPos.Format("%ld",		get_werkauftrag()->get_leitkarte().get_sdPositionsnr()); 

		strkcAuf = get_werkauftrag()->get_leitkarte().get_kcAuftragsnr();
		strkcPos.Format("%ld",		get_werkauftrag()->get_leitkarte().get_kcPositionsnr()); 

		strsdMat.Format("%ld",		get_werkauftrag()->get_leitkarte().get_sdMaterialnummer()); 
		strbauabschnitt.Format("%s",get_werkauftrag()->get_leitkarte().get_prj_bauAbschnitt()); 
		strauftrbez.Format("%s",	get_werkauftrag()->get_leitkarte().get_prj_auftragsbez()); 
		strplz.Format("%s",			get_werkauftrag()->get_leitkarte().get_prj_bauPlz()); 
		strort.Format("%s",			get_werkauftrag()->get_leitkarte().get_prj_bauOrt()); 
		strnl.Format("%d",			get_werkauftrag()->get_leitkarte().get_prj_nl());
		strnlsachb.Format("%s",		get_werkauftrag()->get_leitkarte().get_sachbearbeiterNL()); 
		strodsachb.Format("%s",		get_werkauftrag()->get_status().get_sachbearbeiterSegment()); 

		strerstelldat.Format("%s",	get_werkauftrag()->get_erstellungsdatum().Format("%d.%m.%y")); 
		strfreidat.Format("%s",		get_werkauftrag()->get_status().get_freigabedatum().Format("%d.%m.%y")); 
		strbearbdat.Format("%s",	get_werkauftrag()->get_status().get_bearbeitungsdatum().Format("%d.%m.%y")); 
		strliefdat.Format("%s",		get_werkauftrag()->get_status().get_mbtermin().Format("%d.%m.%y")); 
		strwliefdat.Format("%s",	get_werkauftrag()->get_leitkarte().get_wunschliefertermin().Format("%d.%m.%y")); 

		CString guidline="ohne";
		if(get_werkauftrag()->get_leitkarte().get_zulassungsnummer() == 2)
		{
			guidline="FM";
		}
		if(get_werkauftrag()->get_leitkarte().get_zulassungsnummer() == 1)
		{
			guidline="VdS";
		}
		strKolliliste.Format("%s",   guidline);

		//mds#550
		CString farbe;
		if (rohr != NULL)
		{
			farbe = rohr->getKappenfarbe(rohr->get_rohrKFarbe());
		}
		else
		{
			farbe = "";
		}
		kappenfarbe.Add(farbe);

		if(rohr->get_rohrAnfang() == SCH || rohr->get_rohrEnde() == SCH)
		{
			scheibe.Add("JA");
		}
		else
		{
		    scheibe.Add("NEIN");
		}


		wan.Add(strwan); 
		pspel.Add(strpspel); 
		sdAuf.Add(strsdAuf); 
		sdPos.Add(strsdPos); 

		kcAuf.Add(strkcAuf); 
		kcPos.Add(strkcPos); 
		
		sdMat.Add(strsdMat); 
		bauabschnitt.Add(strbauabschnitt); 
		auftrbez.Add(strauftrbez); 
		plz.Add(strplz); 
		ort.Add(strort); 
		nl.Add(strnl); 
		nlsachb.Add(strnlsachb); 
		odsachb.Add(strodsachb);
		erstelldat.Add(strerstelldat);
		freidat.Add(strfreidat);
		bearbdat.Add(strbearbdat);
		liefdat.Add(strliefdat);
		wliefdat.Add(strwliefdat);
		m_Kolliliste.Add(strKolliliste);

		// und Beschichtung gleich mit
		CString strbeschichtung = get_werkauftrag()->get_beschichtung().get_bezeichnung();
		if (strbeschichtung.Compare("gepulvert") == 0)
		{
			CString strral;
			if (get_werkauftrag()->get_beschichtung().get_typ() == Beschichtung::ROT_GEPULV)
				strral = " 3000";
			else if (get_werkauftrag()->get_beschichtung().get_typ() == Beschichtung::WEISS_GEPULV)
				strral = " 9010";
			else
				strral.Format(" %d", get_werkauftrag()->get_beschichtung().get_ral()); 
			strbeschichtung.Format("pulv %s", strral);
		}
		beschichtung.Add(strbeschichtung);
	}
}

void Excel::createRohrdata(int maxAnzStdele)
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	XASSERT(rohr);
	if (!rohr) 
		return;

	// Rohrattribute fuer Ausgabe der Einzelpositionen merken
	rohrQuali.Add(rohr->get_rohrQuali());
	anzahl.Add(rohr->get_anzahl());
	dnvl.Add(rohr->get_dnvl());

	int bogenmass = 0; 
	for (int j=0; j<rohr->get_avelementArray().GetSize(); j++)
	{
		// Hinter Bogen duerfen keine Standardelemente mehr sein
		Element *ele = rohr->get_avelementArray().get_element_at(j);
		if (ele->isEleBogen())
		{
			bogenmass = ele->get_abstAbs()-ele->get_laenge();
			break;
		}	
	}
	
	/*
	if (rohr->isStrangrohr())
        laenge.Add((bogenmass > 0) ? bogenmass : rohr->getLaengeZuschnitt());
	else
		laenge.Add(rohr->getLaengeZuschnitt());
	*/

	laenge.Add(rohr->getLaengeZuschnitt());

	posNr.Add(rohr->get_posNr());
	rohrAnfang.Add(rohr->get_rohrAnfang());
	
	bool doubleDisc=false;
	if (rohr->isStrangrohr() && rohr->isBogen())
		rohrEnde.Add(GLATT);
	else if (rohr->get_rohrAnfang() == SCH && rohr->get_rohrEnde() == SCH)
	{
		rohrEnde.Add(GLATT);
		doubleDisc=true;
	}

	else
		rohrEnde.Add(rohr->get_rohrEnde());
	
	isdoubledisc.Add(doubleDisc);
	rohrKFarbe.Add(rohr->get_rohrKFarbe());
	avnachbearbeitung.Add(rohr->get_avnachbearbeitung());

	// Typspezifische Daten
	if (rohr->isVerteiler() && rohr->checkAnlage())
		createRohrdataStdvert();
	else if (rohr->isStrangrohr())
		createRohrdataStrang(maxAnzStdele);
}

void Excel::createRohrdataStdvert()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	XASSERT(rohr);
	if (!rohr) 
		return;

	int drehen = 0, lfdnr = 0, laenge = 0, eleanz = 0, uposnr = 0;
	bool first = false;
	
	// elementListSap, weil hier die umgewandelten Standardelemente drin sind
	ElementList elelst(rohr->get_elementListSap());
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *ele = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(ele);
		if (!ele)
			break;
		
		if (ele->isEleRohr())
		{
			Element* prev = dynamic_cast<Element*>(elelst.get_prev(count));
			Element* next = dynamic_cast<Element*>(elelst.get_next(count));
			
			// Wenn Element Anfang/Ende
			if (prev->get_id() == SNAHT || prev->isEleEnde()) 
				first = true;

			if (first)
			{
				uposnr++;
				lfdnr = 0;
				first = false;
			}
					
			lfdnr++;
			// Nur Rohre groesser Mindestlaenge
			if (ele->get_laenge() >= Verteiler::A_MIN_ROHRLAENGE)
			{
				// Nur Rohre mit Elementen fuer Anlage
				bool anlage = false;
				for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
				{
					Element* uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(upos));
					XASSERT(uele);
					if (!uele) // Fehler, raus
						return;
					
					if (uele->get_makeAnlage())
					{
						anlage = true;
						break;
					}
				}
				if (!anlage)
				{
					laenge += ele->get_laenge();
					continue;
				}

				// Wenn Abstand zum Anfang und Ende zu gering, dann auch nicht
				drehen = ele->checkDrehen(laenge, Verteiler::A_MIN_ABSTAND_ANFANG, Verteiler::A_MIN_ABSTAND_ENDE);
				if (drehen == 2)
				{
					laenge += ele->get_laenge();
					continue;
				}

				// Rohrstuecke in Liste
				Rohrstuecke* rohrst = new Rohrstuecke;
				if (drehen == 1)
				{
					int anfaid = next->isEleEnde() ? next->get_id() : GLATT;
					int endeid = prev->isEleEnde() ? prev->get_id() : GLATT;
					rohrst->initDaten(rohr->get_posNr(), lfdnr, rohr->get_anzahl(), anfaid, endeid,	
						ele->get_laenge(), ele->get_dn(), ele->get_beschriftungLinks()); 
					rohrst->set_uposnr(uposnr);
					
					eleanz = 0;
					for (POSITION upos = ele->get_uelementList().GetTailPosition(); upos != NULL;)	
					{
						Element* uele = dynamic_cast<Element*>(ele->get_uelementList().GetPrev(upos));
						XASSERT(uele);
						if (!uele)
							break;

						if (uele->get_makeAnlage())
						{
							eleanz++;
							rohrst->eleabstand.Add(ele->get_laenge() - uele->get_abstAbs() + laenge);
							rohrst->eledn.Add(uele->get_dn());
							rohrst->elelaenge.Add(uele->get_laenge());
							rohrst->eleid.Add(uele->get_id());
						}
					}
				}
				else if (drehen == 0)
				{
					int anfaid = prev->isEleEnde() ? prev->get_id() : GLATT;
					int endeid = next->isEleEnde() ? next->get_id() : GLATT;
					rohrst->initDaten(rohr->get_posNr(), lfdnr, rohr->get_anzahl(), anfaid, endeid,	
						ele->get_laenge(), ele->get_dn(), ele->get_beschriftungLinks()); 
					rohrst->set_uposnr(uposnr);
					
					eleanz = 0;
					for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
					{
						Element* uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(upos));
						XASSERT(uele);
						if (!uele)
							break;

						if (uele->get_makeAnlage())
						{
							eleanz++;
							rohrst->eleabstand.Add(uele->get_abstAbs() - laenge);
							rohrst->eledn.Add(uele->get_dn());
							rohrst->elelaenge.Add(uele->get_laenge());
							rohrst->eleid.Add(uele->get_id());
						}
					}
				}
				if (eleanz > 0)
				{
					rohrst->set_eleanz(eleanz);
					rohrlist.AddTail(rohrst);
				}
				else
					delete rohrst;
			}
		}
		laenge += ele->isEleBogen()?2*ele->get_laenge():ele->get_laenge();
	}
}

void Excel::createRohrdataStrang(int maxAnzStdele)
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	XASSERT(rohr);
	if (!rohr) 
		return;

	Strangrohr* strang = dynamic_cast<Strangrohr*>(get_werkauftrag());
	strang->createAutoArbeit();

	Arbeitsplan& ArPlan = strang->get_arbeit();
	double afoStrangzeit =0;
	for (POSITION pos = ArPlan.get_arbeitsfolgen().GetHeadPosition(); pos != NULL;)
	{
		Arbeitsfolge* afo = (Arbeitsfolge*)ArPlan.get_arbeitsfolgen().GetNext(pos);
		if ( afo->get_id() == 51)
		{
			afoStrangzeit = afo->get_zeit();
		}
	}

	Element *ele = NULL;
	
	// Beschriftung
	for (POSITION pos = rohr->get_elementList().GetHeadPosition(); pos != NULL;)	
	{
		ele = dynamic_cast<Element*>(rohr->get_elementList().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleRohr())
			break;
	}
	if (ele)
		beschrL.Add(ele->get_beschriftungLinks());
	else
		beschrL.Add(".");
	beschrR.Add(".");

	// Standardelemente
	muffenID.Add(strang->get_muffenID());
	muffenDN.Add(strang->get_muffenDN());

	muffenAbstand1.Add(strang->getRelLaengeStdMuffe(0));
	muffenAbstand2.Add(strang->getRelLaengeStdMuffe(1));
	muffenAbstand3.Add(strang->getRelLaengeStdMuffe(2));
	muffenAbstand4.Add(strang->getRelLaengeStdMuffe(3));
	muffenAbstand5.Add(strang->getRelLaengeStdMuffe(4));
	muffenAbstand6.Add(strang->getRelLaengeStdMuffe(5));
	muffenAbstand7.Add(strang->getRelLaengeStdMuffe(6));
	muffenAbstand8.Add(strang->getRelLaengeStdMuffe(7));
	muffenAbstand9.Add(strang->getRelLaengeStdMuffe(8));
	muffenAbstand10.Add(strang->getRelLaengeStdMuffe(9));

	restmass.Add(strang->getRelLaengeStdMuffe(99));

	int mufanz = 0; 
	for (int i=0; i<maxAnzStdele; i++)
	{
		if (strang->get_muffenRelAbst(i) > 0)
			mufanz++;
		else
			break;
	}
	muffenAnzahl.Add(mufanz);
	zeit.Add(afoStrangzeit);
	ZeitGesamt += afoStrangzeit;
}

bool Excel::createVerteilerdata()
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	if (!rohr->isVerteiler() || !rohr->checkAnlage()) 
		return true;
	
	// Leitkarte merken
	createLeitkartedata();

	// Rohrattribute fuer Ausgabe der Einzelpositionen merken
	createRohrdata();

	if (!checkLaengeStdElemente(rohr))
	{
		AfxMessageBox(getStammdaten().getText("IDS_STUTZEN_LAENGE").c_str(), MB_ICONERROR | MB_OK);
		return false;
	}
	return true;
}

bool Excel::setTableLeitkarte(char fname[], bool createTable)
{
	if (createTable)
	{
		// Table erstellen
		char* table = "Leitkarte";
		char* columns[21] = { 
			"WA_Nr",			"PSP_Element",      "SD_Aufnr",		     "SD_Posnr",    "SD_Matnr", 
			"Bauabschnitt", 	"Auftrbez",         "PLZ",		         "Ort",		
			"NL",				"NLSachbearbeiter",	"ODSachbearbeiter",     
			"Erstellungsdatum",	"Freigabedatum",    "Bearbeitungsdatum", "Lieferdatum", "MB_Datum",
			"Geprüft",			"Abgearbeitet",		"Geändert",    
			NULL 
		};
		int type [] = { 
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT,
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT,
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT,
			EXCELTEXT, EXCELTEXT, EXCELTEXT
		};
		int quantity = 20;	

		ExcelTableSetVar(fname, table);
		ExcelTableSetColumnNames(columns, quantity);
		ExcelTableSetColumnType(quantity, type);
		ExcelTableCreateTable();
		ExcelTableCloseTable(fname, table);

		// Table mit Daten fuellen			
		ExcelTableOpenTable(fname, table);
		char *values[21];
		CString txt1, txt2, txt3, txt4, txt5, txt6, txt7, txt8, txt9, 
				txt10, txt11, txt12, txt13, txt14, txt15, txt16, txt17;
		txt1 = wan.GetAt(0);
		txt2 = pspel.GetAt(0);
		txt3 = sdAuf.GetAt(0);
		txt4 = sdPos.GetAt(0);
		txt5 = sdMat.GetAt(0);
		txt6 = bauabschnitt.GetAt(0);
		txt7 = auftrbez.GetAt(0);
		txt8 = plz.GetAt(0);
		txt9 = ort.GetAt(0);
		txt10 = nl.GetAt(0);
		txt11 = nlsachb.GetAt(0);
		txt12 = odsachb.GetAt(0);
		txt13 = erstelldat.GetAt(0);
		txt14 = freidat.GetAt(0);
		txt15 = bearbdat.GetAt(0);
		txt16 = liefdat.GetAt(0);
		txt17 = wliefdat.GetAt(0);

		values[ 0] = txt1.GetBuffer(1);
		values[ 1] = txt2.GetBuffer(1);
		values[ 2] = txt3.GetBuffer(1);
		values[ 3] = txt4.GetBuffer(1);
		values[ 4] = txt5.GetBuffer(1);
		values[ 5] = txt6.GetBuffer(1);
		values[ 6] = txt7.GetBuffer(1);
		values[ 7] = txt8.GetBuffer(1);
		values[ 8] = txt9.GetBuffer(1);
		values[ 9] = txt10.GetBuffer(1);
		values[10] = txt11.GetBuffer(1);
		values[11] = txt12.GetBuffer(1);
		values[12] = txt13.GetBuffer(1);
		values[13] = txt14.GetBuffer(1);
		values[14] = txt15.GetBuffer(1);
		values[15] = txt16.GetBuffer(1);
		values[16] = txt17.GetBuffer(1);
		values[17] = ".";
		values[18] = ".";
		values[19] = ".";

		ExcelTableInsertInto(values);
		ExcelTableCloseTable(fname, table);
	}
	return true;
}

bool Excel::setTableVerteilerdaten(int index, char fname[], bool createTable)
{
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	XASSERT(rohr);
	if (!rohr) 
		return (0);

	int i = index;

	// Table erstellen
	char* table = "Verteilerdaten";
	char* columnsV[11] = { 
		"PosNr", "Anzahl", "Länge", "DN", "DIN", "Kfarbe", "AnzUpos", 
		"Geprüft", "Abgearbeitet", "Geändert",  NULL    
	};
	int typeV [] = { 
		EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
		EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT
	};
	int quantityV = 10;	
	
	if (createTable)
	{
		ExcelTableSetVar(fname, table);
		ExcelTableSetColumnNames(columnsV, quantityV);
		ExcelTableSetColumnType(quantityV, typeV);
		ExcelTableCreateTable();
		ExcelTableCloseTable(fname, table);
	}
	else
	{
		ExcelTableSetColumnNames(columnsV, quantityV);
		ExcelTableSetColumnType(quantityV, typeV);
	}

	ExcelTableOpenTable(fname, table);
 
	// Table mit Daten fuellen			
	CString txt1, txt2, txt3, txt4, txt5, txt6; 
	txt1 = posNr.GetAt(i);
	txt2.Format("%d", anzahl.GetAt(i));
	txt3.Format("%d", laenge.GetAt(i));
	txt4.Format("%d", dnvl.GetAt(i));
	if      ((rohrQuali.GetAt(i) == Rohrleitung::RQ2440_2458GSCH && dnvl.GetAt(i) < 65) || rohrQuali.GetAt(i) == Rohrleitung::RQ2440NLOS || rohrQuali.GetAt(i) == Rohrleitung::RQ2440GSCH)
		txt5 = "2440";
	else if ((rohrQuali.GetAt(i) == Rohrleitung::RQ2440_2458GSCH && dnvl.GetAt(i) > 50) || rohrQuali.GetAt(i) == Rohrleitung::RQ2458GSCH)
		txt5 = "2458";
	else if  (rohrQuali.GetAt(i) == Rohrleitung::RQ2448NLOS)
		txt5 = "2448";
	txt6 = rohr->getKappenfarbe(rohrKFarbe.GetAt(i));

	char *valuesV[11];
	valuesV[ 0] = txt1.GetBuffer(1);
	valuesV[ 1] = txt2.GetBuffer(1);
	valuesV[ 2] = txt3.GetBuffer(1);
	valuesV[ 3] = txt4.GetBuffer(1);
	valuesV[ 4] = txt5.GetBuffer(1);
	valuesV[ 5] = txt6.GetBuffer(1);
	valuesV[ 6] = "1";
	valuesV[ 7] = ".";
	valuesV[ 8] = ".";
	valuesV[ 9] = ".";

	ExcelTableInsertInto(valuesV);
	ExcelTableCloseTable(fname, table);

	return true;
}

bool Excel::setTableRohrdaten(int index, char fname[], bool createTable)
{
	// Table erstellen
	char* table = "Rohrdaten";
	char* columnsR[23] = { 
		"PosNr",    "UposNr",       "LfdNr",     "AnzRohre",  "AnzElemente", 
		"BeschrLi", "BeschrRe",     "DN",        "DIN",       "Länge", 
		"Anfang",   "Ende",         "ElementID", "ElementDN", "AbsAbst",     
		"Winkel",   "AnzSoll",      "AnzIst",    "Geprüft",   "Abgearbeitet", 
		"Geändert", "ElementLänge",	NULL    
	};
	int typeR [] = { 
		EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
		EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
		EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
		EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
		EXCELTEXT, EXCELTEXT, EXCELTEXT
	};
	int quantityR = 22;	
	
	if (createTable)
	{
		ExcelTableSetVar(fname, table);
		ExcelTableSetColumnNames(columnsR, quantityR);
		ExcelTableSetColumnType(quantityR, typeR);
		ExcelTableCreateTable();
		ExcelTableCloseTable(fname, table);
	}
	else
	{
		ExcelTableSetColumnNames(columnsR, quantityR);
		ExcelTableSetColumnType(quantityR, typeR);
	}

	ExcelTableOpenTable(fname, table);

	POSITION pos = rohrlist.FindIndex(index);
	Rohrstuecke* rohrst = dynamic_cast<Rohrstuecke*>(rohrlist.GetAt(pos));

	// Table mit Daten fuellen			
	for (int j=0; j < Verteiler::A_MAX_ANZAHL_STDELEMENTE_PROROHR; j++)
	{
		char *valuesR[23];
		CString txt0, txt1, txt2, txt3, txt4, txt5, txt6, txt7, txt8, txt9, txt10, txt01; 
		CString txt11, txt12, txt13, txt14, txt15, txt16, txt17, txt18, txt19, txt20, txt21; 
		// Rohrdaten
		if (j==0)
		{
			txt0.Format("%d", rohrst->get_lfdnr());
			txt01.Format("%d", rohrst->get_uposnr());
			txt1 = rohrst->get_posnr();
			txt2.Format("%d", rohrst->get_anz());
			txt3.Format("%d", rohrst->get_eleanz());
			txt4 = rohrst->get_beschriftung();		// Beschriftung links
			txt5 = ".";								// Beschriftung rechts
			txt6.Format("%d", rohrst->get_dn());

			if      ((rohrQuali.GetAt(0) == Rohrleitung::RQ2440_2458GSCH && rohrst->get_dn() < 65) || rohrQuali.GetAt(0) == Rohrleitung::RQ2440NLOS || rohrQuali.GetAt(0) == Rohrleitung::RQ2440GSCH)
				txt7 = "2440";
			else if ((rohrQuali.GetAt(0) == Rohrleitung::RQ2440_2458GSCH && rohrst->get_dn() > 50) || rohrQuali.GetAt(0) == Rohrleitung::RQ2458GSCH)
				txt7 = "2458";
			else if  (rohrQuali.GetAt(0) == Rohrleitung::RQ2448NLOS)
				txt7 = "2448";
		
			txt8.Format("%d", rohrst->get_laenge());
			
			if (rohrst->get_anfang() == NUT)
				txt9 = "NUT";
			else 
				txt9 = "GLATT";
			if (rohrst->get_ende() == NUT)
				txt10 = "NUT";
			else 
				txt10 = "GLATT";
		
			valuesR[ 0] = txt1.GetBuffer(1);
			valuesR[ 1] = txt01.GetBuffer(1);
			valuesR[ 2] = txt0.GetBuffer(1);
			valuesR[ 3] = txt2.GetBuffer(1);
			valuesR[ 4] = txt3.GetBuffer(1);
			valuesR[ 5] = txt4.GetBuffer(1);
			valuesR[ 6] = txt5.GetBuffer(1);
			valuesR[ 7] = txt6.GetBuffer(1);
			valuesR[ 8] = txt7.GetBuffer(1);
			valuesR[ 9] = txt8.GetBuffer(1);
			valuesR[10] = txt9.GetBuffer(1);
			valuesR[11] = txt10.GetBuffer(1);
			valuesR[16] = txt2.GetBuffer(1);
		}
		else
		{
			valuesR[ 0] = ".";
			valuesR[ 1] = ".";
			valuesR[ 2] = ".";
			valuesR[ 3] = ".";
			valuesR[ 4] = ".";
			valuesR[ 5] = ".";
			valuesR[ 6] = ".";
			valuesR[ 7] = ".";
			valuesR[ 8] = ".";
			valuesR[ 9] = ".";
			valuesR[10] = ".";
			valuesR[11] = ".";
			valuesR[16] = ".";
		}
		// Elemente
		if (j < rohrst->get_eleanz())
		{
			if (rohrst->get_eleid(j) == STDMUFFE)
				txt11 = "MUFFE";
			else if (rohrst->get_eleid(j) == STDSTUTZEN || (rohrst->get_eleid(j) == ROHR_Q9 && rohrst->get_eledn(j) == 65))
				txt11 = "NUTST";
			txt12.Format("%d", rohrst->get_eledn(j));
			txt13.Format("%d", rohrst->get_eleabstand(j));
			txt14.Format("%d", rohrst->get_elelaenge(j));
		
			valuesR[12] = txt11.GetBuffer(1);
			valuesR[13] = txt12.GetBuffer(1);
			valuesR[14] = txt13.GetBuffer(1);
			valuesR[15] = "0";
			valuesR[21] = txt14.GetBuffer(1);
		}
		else
		{
			valuesR[12] = ".";
			valuesR[13] = ".";
			valuesR[14] = ".";
			valuesR[15] = ".";
			valuesR[21] = ".";
		}
		valuesR[17] = ".";
		valuesR[18] = ".";
		valuesR[19] = ".";
		valuesR[20] = ".";
		
		ExcelTableInsertInto(valuesR);
	}
	ExcelTableCloseTable(fname, table);
	
	return true;
}


int Excel::createStrangrohrdata()
{
	Strangrohr* strang = dynamic_cast<Strangrohr*>(get_werkauftrag());
	XASSERT(strang);
	if (!strang) 
		return (0);

	//Leitkarte merken
	createLeitkartedata();

	// Rohrattribute fuer Ausgabe der Einzelpositionen merken
	createRohrdata(Strangrohr::MAX_ANZAHL_STDELEMENTE);
	
	return (1);
}

/*
Schreibt die Daten eines Strangrohrauftrages für die Strangrohrstrasse in eine Exceldatei. 
*/
void Excel::createStrangrohrtabelle()
{
#ifndef _DEBUG
	if (!get_werkauftrag()->get_status().get_bearbeitungsdatum()) 
	{
		CString str; str = getStammdaten().getText("IDS_HINWEIS1b").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return;
	}
#endif

	Strangrohr* strang = dynamic_cast<Strangrohr*>(get_werkauftrag());
	XASSERT(strang);
	if (!strang) 
		return;

	static CString fn, path;
	path.Format("%s", (WaxSys::para_str(DIR_STRANG_FREE)));
	fn = path;

	////////// load library //////////	
	if (isLoaded()) 
	{
		CString str;

		int wanr = get_werkauftrag()->get_status().get_wanr();
		str.Format("%d", wanr);

		//evt. Nullen vorne ahnhängen
		if (str.GetLength() < 6)
		{
			CString stNull = "000000";
			str = stNull.Left(6 - str.GetLength()) + str;
		}


		fn += str;
		fn += ".xls";

		//if (OpenFile(fn, reOpenBuff, style))
		//	DeleteFile(fn);   
		CFile file;
		if (file.Open(fn, CFile::modeRead | CFile::shareDenyNone)) 
		{
			file.Close();
			::DeleteFile(fn);
		}

		char* table = "Tabelle1";
	
		char fname[255]; 
		strcpy(fname, path.GetBuffer(13));
		strcat(fname, str);

		char *values[28];
		char *columns[28] = {	
			"Positionsnummer",
			"AVNachbearbeitung",
			"AnzahlRohreSollwert",
			"AnzahlRohreIstwert",
			"Rohranfang",
			"Rohrende",
			"GesamteStranglänge",
			"AnzahlMuffen",
			"Muffenabstand1",
			"Muffenabstand2",
			"Muffenabstand3",
			"Muffenabstand4",
			"Muffenabstand5",
			"Muffenabstand6",
			"Muffenabstand7",
			"Muffenabstand8",
			"Muffenabstand9",
			"Muffenabstand10",
			"Restmass",
			"BedienerGeändert",
			"Nummer",
			"Kappenfarbe",
			"ZeitenStatistik1",
			"ZeitenStatistik2",
			"ZeitenStatistik3",
			"ZeitenStatistik4",
			"WaxAfoZeit",
			NULL
		};
		int type [] = { 
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
			EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, EXCELTEXT, 
			EXCELTEXT, EXCELTEXT, EXCELTEXT
		};
		int quantity = 27, nr = 0;
		CString txt = "";

		ExcelTableSetVar(fname, table);
		ExcelTableSetColumnNames(columns, quantity);
		ExcelTableSetColumnType(quantity, type);
		ExcelTableCreateTable();
		ExcelTableOpenTable(fname, table);

		CString txt1, txt2, txt3, txt4, txt5, txt6, txt7, txt8, txt9, txt10;
		CString txt11, txt12, txt13, txt14, txt15, txt16, txt17, txt18, txt19, txt20, txt21;
		// 1. Zeile
		int i;
		for (i=0; i<quantity; i++)
		{
			values[i] = ".";
		}
		nr++;
		txt1 = "WA-Nummer";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = wan.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Offset";
		values[ 3] = txt3.GetBuffer(1);
		txt4 = "17";
		values[ 4] = txt4.GetBuffer(1);
		txt5 = "Rohr DN";
		values[ 6] = txt5.GetBuffer(1);
		txt6.Format("%d", strang->get_dnvl());
		values[ 7] = txt6.GetBuffer(1);
		txt7 = "Auftrag geprüft";
		values[ 9] = txt7.GetBuffer(1);
		txt8.Format("%d", nr); 
		values[20] = txt8.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 2. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Bau";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = auftrbez.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Positionen";
		values[ 3] = txt3.GetBuffer(1);
		txt4.Format("%d", anzahl.GetSize());
		values[ 4] = txt4.GetBuffer(1);
		txt5 = "Wandstärke[DIN]";
		values[ 6] = txt5.GetBuffer(1);
		if ((strang->get_rohrQuali() == Rohrleitung::RQ2440_2458GSCH && strang->get_dnvl() < 65) 
			|| strang->get_rohrQuali() == Rohrleitung::RQ2440NLOS
			|| strang->get_rohrQuali() == Rohrleitung::RQ2440GSCH)
			txt6 = "2440";
		else if ((strang->get_rohrQuali() == Rohrleitung::RQ2440_2458GSCH && strang->get_dnvl() > 50) 
			|| strang->get_rohrQuali() == Rohrleitung::RQ2458GSCH)
			txt6 = "2458";
		else if (strang->get_rohrQuali() == Rohrleitung::RQ2448NLOS)
			txt6 = "2448";
		values[ 7] = txt6.GetBuffer(1);
		txt7 = "Auftrag abgearbeitet";
		values[ 9] = txt7.GetBuffer(1);
		txt8.Format("%d", nr); 
		values[20] = txt8.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 3. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Plz";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = plz.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Muffen DN";
		values[ 6] = txt3.GetBuffer(1);
		if (strang->get_muffenID() == BOHR24)
		{
			txt4.Format("%.2f",strang->get_durchmesser());
			txt4.Replace(".",",");
		}
		else
			txt4.Format("%d", muffenDN.GetAt(0));
		values[ 7] = txt4.GetBuffer(1);
		txt5 = "Auftrag geändert";
		values[ 9] = txt5.GetBuffer(1);
		txt6.Format("%d", nr); 
		values[20] = txt6.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 4. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Ort";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = ort.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Muffenhoehe";
		values[ 6] = txt3.GetBuffer(1);

		Element *ele = new Element;
	try 
	{ 
		if (strang->get_muffenID() == BOHR24)
			ele->initStammdaten(strang->get_muffenDN(), strang->get_dnvl(), strang->get_muffenID());
		else if (strang->get_muffenGekehlt())
			ele->initStammdaten(strang->get_muffenDN(), strang->get_dnvl(), KRMUFFE);
		else 
			ele->initStammdaten(strang->get_muffenDN(), strang->get_dnvl(), FRMUFFE);

		if (strang->get_muffenID() == NEW_ROBBI_MUFFE)
			ele->initStammdaten(strang->get_muffenDN(), strang->get_dnvl(), strang->get_muffenID());

		ele->initDaten(0, 0, 0, 0, 0);
	}
	catch(Element::Error e) 
	{
		delete ele;
		MsgBox::show_error(e.get_errorText());
		return;
	}
		txt4.Format("%d", ele->get_laenge());
		values[ 7] = txt4.GetBuffer(1);
		txt5.Format("%d", nr); 
		values[20] = txt5.GetBuffer(1);
		ExcelTableInsertInto(values);
		delete ele;

		// 5. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "IB";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = nl.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Muffenkehlung";
		values[ 6] = txt3.GetBuffer(1);
		txt4.Format("%s", strang->get_muffenGekehlt() ? "X" : ".");
		values[ 7] = txt4.GetBuffer(1);
		txt5.Format("%d", nr); 
		values[20] = txt5.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 6. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Sachbearbeiter NL";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = nlsachb.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Bohr mm";
		values[6] = txt3.GetBuffer(1);
		if (strang->get_muffenID() == BOHR24)
		{
			txt4.Format("%.2f",strang->get_durchmesser());
			txt4.Replace(".",",");
			values[7] = txt4.GetBuffer(1);
		}
		
		txt5.Format("%d", nr); 
		values[20] = txt5.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 7. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Sachbearbeiter OD";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = odsachb.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3 = "Sollzeit";
		values[6] = txt3.GetBuffer(1);
		txt4.Format("%f", ZeitGesamt); 
		int iWo = txt4.Find(".");
		if(iWo > -1)
		{
			txt4 = txt4.Mid(0, iWo+3);
		}
		values[7] = txt4.GetBuffer(1);
		txt5.Format("%d", nr); 
		values[20] = txt5.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 8. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Erstellungsdatum";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = erstelldat.GetAt(0); 
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 9. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Bearbeitungsdatum";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = bearbdat.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 10. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Wunschliefertermin";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = wliefdat.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 11. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Freigabedatum";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = freidat.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 12. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "Bemerkung";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = get_werkauftrag()->get_bemerkung();
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);
			
		// 13. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "SD Auftragsnr";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = sdAuf.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 14. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "SD Materialnr";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = sdMat.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 15. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "SD Positionsnr";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = sdPos.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		// 16. Zeile
		for (i=0; i<quantity; i++)
		{
			txt = ".";
			values[i] = txt.GetBuffer(1);
		}
		nr++;
		txt1 = "PSP-Element";
		values[ 0] = txt1.GetBuffer(1);
		txt2 = pspel.GetAt(0);
		values[ 1] = txt2.GetBuffer(1);
		txt3.Format("%d", nr); 
		values[20] = txt3.GetBuffer(1);
		ExcelTableInsertInto(values);

		int DiscSize=0;
		int rohrDn=strang->get_dnvl();

		bool doubleDisc=false;

		rohrEnde.Add(GLATT);

		if(rohrDn==65)
		{
			DiscSize=13;
		}
		if(rohrDn==32)
		{
			DiscSize=7;
		}
		if(rohrDn==40)
		{
			DiscSize=8;
		}
		if(rohrDn==50)
		{
			DiscSize=10;
		}


		
		// 17. Zeile -> 
		for (i=0; i<anzahl.GetSize(); i++)
		{
			bool discLeft=false;
			bool discRight=false;

			for (int j=0; j<quantity; j++)
			{
				txt = ".";
				values[j] = txt.GetBuffer(1);
			}
			nr++;

			txt1 = posNr.GetAt(i);
			values[ 0] = txt1.GetBuffer(1);
			
			txt2.Format("%s", avnachbearbeitung.GetAt(i) ? "x" : ".");
			values[ 1] = txt2.GetBuffer(1);
			
			txt3.Format("%d", anzahl.GetAt(i));
			values[ 2] = txt3.GetBuffer(1);

			if (rohrAnfang.GetAt(i) == NUT)
				txt4 = "N";	
			else if (rohrAnfang.GetAt(i) == GEW)
				txt4 = "G";
			else if (rohrAnfang.GetAt(i) == GLATT)
				txt4 = "-";
			else if (rohrAnfang.GetAt(i) == SCH)
			{
				txt4 = "(";
				discLeft=true;
			}

			values[ 4] = txt4.GetBuffer(1);
			
			if (rohrEnde.GetAt(i) == NUT)
				txt5 = "N";	
			else if (rohrEnde.GetAt(i) == GEW)
				txt5 = "G";
			else if (rohrEnde.GetAt(i) == GLATT)
				txt5 = "-";
			else if (rohrEnde.GetAt(i) == SCH)
			{
				txt5 = ")";
				discRight=true;
			}

			values[ 5] = txt5.GetBuffer(1);
			
			if(discLeft==true || discRight==true)
			{
				doubleDisc=isdoubledisc.GetAt(i);
				if(doubleDisc)
				{
					int doublediscSize=DiscSize*2;
					txt6.Format("%d", laenge.GetAt(i)- doublediscSize);
					values[ 6] = txt6.GetBuffer(1);
				}
				else
				{
					txt6.Format("%d", laenge.GetAt(i)- DiscSize);
					values[ 6] = txt6.GetBuffer(1);
				}
			}
			else
			{
				txt6.Format("%d", laenge.GetAt(i));
				values[ 6] = txt6.GetBuffer(1);
			}

			txt7.Format("%d", muffenAnzahl.GetAt(i));
			values[ 7] = txt7.GetBuffer(1);
				
			if (muffenAbstand1.GetAt(i) > 0)
			{
				if(discLeft==true)
				{
					txt8.Format("%d", muffenAbstand1.GetAt(i)- DiscSize);
					values[ 8] = txt8.GetBuffer(1);
				}
				else
				{
					txt8.Format("%d", muffenAbstand1.GetAt(i));
					values[ 8] = txt8.GetBuffer(1);
				}
			}
			if (muffenAbstand2.GetAt(i) > 0)
			{
				txt9.Format("%d", muffenAbstand2.GetAt(i));
				values[ 9] = txt9.GetBuffer(1);
			}
			if (muffenAbstand3.GetAt(i) > 0)
			{
				txt10.Format("%d", muffenAbstand3.GetAt(i));
				values[ 10] = txt10.GetBuffer(1);
			}
			if (muffenAbstand4.GetAt(i) > 0)
			{
				txt11.Format("%d", muffenAbstand4.GetAt(i));
				values[ 11] = txt11.GetBuffer(1);
			}
			if (muffenAbstand5.GetAt(i) > 0)
			{
				txt12.Format("%d", muffenAbstand5.GetAt(i));
				values[ 12] = txt12.GetBuffer(1);
			}
			if (muffenAbstand6.GetAt(i) > 0)
			{
				txt13.Format("%d", muffenAbstand6.GetAt(i));
				values[ 13] = txt13.GetBuffer(1);
			}
			if (muffenAbstand7.GetAt(i) > 0)
			{
				txt14.Format("%d", muffenAbstand7.GetAt(i));
				values[ 14] = txt14.GetBuffer(1);
			}
			if (muffenAbstand8.GetAt(i) > 0)
			{
				txt15.Format("%d", muffenAbstand8.GetAt(i));
				values[ 15] = txt15.GetBuffer(1);
			}
			if (muffenAbstand9.GetAt(i) > 0)
			{
				txt16.Format("%d", muffenAbstand9.GetAt(i));
				values[ 16] = txt16.GetBuffer(1);
			}
			if (muffenAbstand10.GetAt(i) > 0)
			{
				txt17.Format("%d", muffenAbstand10.GetAt(i));
				values[ 17] = txt17.GetBuffer(1);
			}
			
			//restmaß
			if(doubleDisc==true || discRight)
			{
				txt18.Format("%d", restmass.GetAt(i)- DiscSize);
				values[18] = txt18.GetBuffer(1);
			}
			else
			{
				txt18.Format("%d", restmass.GetAt(i));
				values[18] = txt18.GetBuffer(1);
			}
			


			txt19.Format("%d", nr); 
			values[20] = txt19.GetBuffer(1);

			txt20.Format("%s", strang->getKappenfarbe(rohrKFarbe.GetAt(i))); 
			values[21] = txt20.GetBuffer(1);

			double dZeitProRohr = zeit.GetAt(i)/anzahl.GetAt(i);
			txt21.Format("%f", dZeitProRohr ); 
			int iWo = txt21.Find(".");
			if(iWo > -1)
			{
				txt21 = txt21.Mid(0, iWo+4);
			}

			values[26] = txt21.GetBuffer(1);

			ExcelTableInsertInto(values);
		}
		ExcelTableCloseTable(fname, table);
	}
}

//Kopiert die Liste mit (Unter)Aufträgen
void Excel::set_walist(CObList& value)
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
		{
			rohr->createModell();
		}
		// Ist Objekt ein Sammelauftrag, dann in extra Liste speichern 
		Werkauftrag *wa = dynamic_cast<Werkauftrag*>(pt->get_werkauftrag());
		if (wa->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wa))
			sammelauftraege.AddTail(pt); 
		// sonst in walist
		else
			walist.AddTail((CObject*)pt->get_werkauftrag());
	}
}



void Excel::set_walist(MxCObList* walistLocal)
{	
		for (POSITION pos = walistLocal->GetHeadPosition(); pos; )
		{
			Werkauftrag *pt = dynamic_cast<Werkauftrag*>(walistLocal->GetNext(pos));
			walist.AddTail(pt);
		}
}



void Excel::set_walist()
{
	CObList templist;
	WaxPart *pt;

	POSITION pos;
	// Liste mit Sammelauftraegen durchgehen
	for (pos = sammelauftraege.GetHeadPosition(); pos; )
	{
		// Unterobjekte in Subliste speichern
		pt = dynamic_cast<WaxPart*>(sammelauftraege.GetNext(pos));
		XASSERT(pt); if (!pt) continue;
		set_wasublist(pt->get_subobjects());		
		
		// Subliste durchgehen
		for (POSITION pos = wasublist.GetHeadPosition(); pos; )
		{
			pt = dynamic_cast<WaxPart*>(wasublist.GetNext(pos));
			XASSERT(pt); if (!pt) continue;
			
			Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
			if (rohr)
			{
				//Wozu der Aufruf? Fkt. ist eh leer!
				rohr->createModell();
			}

			// Ist Objekt wieder ein Sammelauftrag, dann in extra Liste speichern 
			Werkauftrag *wa = dynamic_cast<Werkauftrag*>(pt->get_werkauftrag());
			if (wa->isEmpty() || dynamic_cast<SammelWerkauftrag*>(wa))
				templist.AddTail(pt); 
			// Sonst in walist
			else
				walist.AddTail((CObject*)pt->get_werkauftrag());
		}
		wasublist.RemoveAll();
	}
	// abgearbeitete Sammelauftraege aus Liste löschen
	sammelauftraege.RemoveAll();

	// Liste wieder mit weiteren (Unter-)Sammelauftraegen füllen
	for (pos = templist.GetHeadPosition(); pos; )
	{
		pt = dynamic_cast<WaxPart*>(templist.GetNext(pos));
		sammelauftraege.AddTail(pt); 
	}
	templist.RemoveAll();
}

void Excel::set_wasublist(CObList& value)
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

const CObList& Excel::get_sammelauftraege() const
{
	return sammelauftraege;
}

double Excel::get_Stammdaten(int id, int dn, int index)
{
	double innendurchmesser = 0; 
	double wandstaerke = 0;
	const CObList& grp = getStammdaten().get_aussendurchmesserwandstaerke();
	for (POSITION gpos = grp.GetHeadPosition(); gpos;)
	{
		AussendurchmesserWandstaerke::data *g = (AussendurchmesserWandstaerke::data *)grp.GetNext(gpos);
		if (g->get_dn() == dn && g->get_elemid() == id) 
		{
			innendurchmesser = g->get_innendurchmesser();
			wandstaerke = g->get_wandstaerke();
		}
	}
	if (index == 0)			return innendurchmesser;
	else if (index == 1)	return wandstaerke;
	
	return 0;
}

CString Excel::insertKopfsatz(Werkauftrag *wa)
{
	// Leitkarte
	CString neueZeile;
	neueZeile.Format("\r\n");
	CString txt = "", str = "", wan = "";
	wan.Format("%06ld", wa->get_status().get_wanr());
	CString erstellungsdatum, lieferdatum, wunschlieferdatum, bearbeitungsdatum;
	erstellungsdatum.Format("%s", wa->get_erstellungsdatum().Format("%d.%m.%y"));
	wunschlieferdatum.Format("%s", wa->get_leitkarte().get_wunschliefertermin().Format("%d.%m.%y"));
	lieferdatum.Format("%s", wa->get_status().get_mbtermin().Format("%d.%m.%y"));
	bearbeitungsdatum.Format("%s", wa->get_status().get_bearbeitungsdatum().Format("%d.%m.%y"));

	// Kopfsatz
	txt += wan;
	txt += neueZeile;
	txt += lieferdatum;
	txt += neueZeile;
	txt += wunschlieferdatum;
	txt += neueZeile;

	txt += wa->get_leitkarte().get_prj_auftragsbez();
	
	txt += neueZeile;
	txt += wa->get_leitkarte().get_prj_bauPlz();
	txt += wa->get_leitkarte().get_prj_bauOrt();
	txt += neueZeile;
	txt += neueZeile; //waxalt kostst
	txt += wa->get_status().get_sachbearbeiterSegment();
	txt += neueZeile;
	txt += bearbeitungsdatum;
	txt += neueZeile;
	//str.Format("%d", wa->get_leitkarte().get_prj_nl()); 
	//txt += str;
	txt += getStammdaten().getNLBezeichnung(wa->get_leitkarte().get_prj_nl());
	txt += neueZeile;
	txt += wa->get_leitkarte().get_sachbearbeiterNL();
	txt += neueZeile;
	txt += erstellungsdatum;
	txt += neueZeile;
	txt += neueZeile; //waxalt mznr
	txt += get_first_line_of(wa->get_bemerkung());
	txt += neueZeile;

	return txt;
}

bool Excel::writeFile(CString fname, CString text)
{
	CFile file;
	if (file.Open(fname, CFile::modeRead | CFile::shareDenyNone))
	{
		file.Close();
		if (!DeleteFile(fname))
		{
			ERRORMSG1(fname);
		}
	}

	TRY
	{
		CFile f(fname, CFile::modeCreate | CFile::modeReadWrite);
		f.Write(text, text.GetLength());
		return true;
	}
	CATCH (CFileException, e)
	{
		ERRORMSG1(fname);
	
		#ifdef _DEBUG
		e->ReportError();
	    #endif

		return false;
	}
	END_CATCH
}

void Excel::openFile(CString fname, CString text)
{
	if ((DWORD)ShellExecute(NULL, "open", fname, NULL, NULL, SW_SHOWNORMAL) < 31)
	{
		fname += _T("b"); // Office 2010 Format
		if ((DWORD)ShellExecute(NULL, "open", fname, NULL, NULL, SW_SHOWNORMAL) < 31)
		{
			AfxMessageBox(text, MB_OK | MB_ICONINFORMATION);
		}
	}
}

/// Rohre nach Nennweite & Laenge sortieren
void Excel::sortListe(CObList& liste, bool aufsteigend)
{
	POSITION pos, nextpos; 
   
	for (int i=0; i<liste.GetCount()-1; i++)	
	{
		pos = liste.FindIndex(i);
		Rohrstuecke *first = dynamic_cast<Rohrstuecke *>(liste.GetAt(pos));
		nextpos = liste.FindIndex(i+1);
		Rohrstuecke *second = dynamic_cast<Rohrstuecke *>(liste.GetAt(nextpos));

		XASSERT(first && second);
		if (!first || !second)
			return;
		if (( aufsteigend && (first->get_dn() > second->get_dn() || first->get_dn() == second->get_dn() && first->get_laenge() > second->get_laenge()))
		 || (!aufsteigend && (first->get_dn() < second->get_dn() || first->get_dn() == second->get_dn() && first->get_laenge() < second->get_laenge())))
		{	
			// Elemente werden vertauscht
			liste.SetAt(nextpos, first);
			liste.SetAt(pos, second);
			i = -1; // Wieder von vorn beginnen
		}
	}
}


const CString Excel::get_first_line_of(const CString&  str) const
{
	/**
	Gibt die erste Zeile oder max. MAX_LINE_LEN Zeichen eines String zurück. Wird für die 
	Ascii Zuschnittslisten benötigt, da die Maschine bei längeren Texten die erste Position 
	'vergisst'. Gilt nur für 32020.
	*/
	const int MAX_LINE_LEN = 80;

	int index = str.FindOneOf("\r\n");
	return str.Left(index < 0 || index > MAX_LINE_LEN ? MAX_LINE_LEN : index);
}

///Prueft, ob Rohrstutzen fuer Anlage unterschiedliche Laenge, aber gleiche Nennweitenkombination haben
bool Excel::checkLaengeStdElemente(Rohrleitung* vert)
{
	int i=0; 
	while (i<vert->get_elementArray().GetSize())
	{
		Element* ele = vert->get_elementArray().get_element_at(i);
		if (!ele) continue;
		
		if (ele->get_makeAnlage() && (ele->get_id() == ROHR || ele->get_id() == NUTST))
		{
			if (ckeckElevector(ele))
				elevector.push_back(ele);
			else
				return false;
		}
		i++;
	}
	return true;
}

bool Excel::ckeckElevector(Element* ele)
{
	for (unsigned int i=0; i<elevector.size(); i++)
	{
		Element *e = elevector.at(i);
		if (e->get_anschlussDN() == ele->get_anschlussDN() && e->get_dn() == ele->get_dn() && e->get_laenge() != ele->get_laenge())
				return false;
	}	
	return true;
}

