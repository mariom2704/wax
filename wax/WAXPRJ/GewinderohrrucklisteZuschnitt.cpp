#include "stdafx.h"
#include "drucklistezuschnitt.h"
#include "..\rohrkonst\elementlist.h"
#include "GewinderohrrucklisteZuschnitt.h"


using namespace WaxDruck;



GewinderohrrucklisteZuschnitt::GewinderohrrucklisteZuschnitt(HtmlDruck& file, WaxPart* pt, CObList* pWalist, int id, DrucklistenVerwaltung* pDrucklistenVerwaltung ):
 DrucklisteZuschnitt(file, pt, id), m_pWalist(pWalist), m_pDrucklistenVerwaltung(pDrucklistenVerwaltung)
{

	dn2zoll = true;

}

GewinderohrrucklisteZuschnitt::~GewinderohrrucklisteZuschnitt(void)
{
}

bool GewinderohrrucklisteZuschnitt::create()
{
	std::string bemerkung;

	// in den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
	// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
	

	CObject *tmp = m_pWalist->GetHead();
	Werkauftrag *ersterWerk = dynamic_cast<Werkauftrag*>(tmp);
	XASSERT(ersterWerk);
	if (ersterWerk)
		bemerkung = (LPCSTR)ersterWerk->get_bemerkung();

	std::string ueberschrift = getStammdaten().getText("IDS_BEMERKUNG");
	
	bool Local2quali = m_pDrucklistenVerwaltung->check2Rohrquali(*m_pWalist);		
	
	
	for (int i = 0; i < m_pWalist->GetCount(); i++, m_pWalist->AddTail(m_pWalist->GetHead()), m_pWalist->RemoveHead())
	{
		

		if(dynamic_cast<SammelWerkauftrag*>(m_pDrucklistenVerwaltung->cur_werkauftrag()) && m_pWalist->GetCount() > 1)
			continue;

		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(m_pDrucklistenVerwaltung->cur_werkauftrag());

		string stdbemerkung;
			
		html.insert_bemerkung(bemerkung, ueberschrift, stdbemerkung);
		html.insertQuali(rohr->get_rohrQuali(), Local2quali, rohr->get_dnvl(), 0, rohr->get_typ());
		break;
	
	}
	createGewinderohrlaengen(); 

return true;
}


void GewinderohrrucklisteZuschnitt::createGewinderohrlaengen()
{
	CObList* elelist = listefromid(listid);
	
	if (elelist->IsEmpty())
		return;

	Druckliste::sort_liste(*elelist, false);
	// Kopf fuer Gewinderohrliste

	create_Gewinderohrkopf();
	WaxHtmlFile& w =  html.get_file();
	// Tabellenspalten fuer Rohre	
	const int rows[] = { 5, 10, 15, 12, 12, 20};
	CString str, txt;

	w.startTable();
	w.startRow();
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows[0], "CENTER", true);
	if (!dn2zoll)
	{
		str = getStammdaten().getText("IDS_NENNW_DN").c_str();
		txt = "<b>" + str + "</b>";
		w.appendCol(txt,	rows[1], "CENTER", true);
	}
	else
	{
		str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
		txt = "<b>" + str + "</b>";
		w.appendCol(txt, rows[1], "CENTER", true);
	}
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt, rows[2], "CENTER", true);

	str = getStammdaten().getText("IDS_ANFANG").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt, rows[3], "CENTER", true);

	str = getStammdaten().getText("IDS_ENDE").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt, rows[4], "CENTER", true);

	str = getStammdaten().getText("IDS_WEITERBEARBEITUNG").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt, rows[5], "CENTER", true);
	w.endRow();

	
	int counter = 0;
	// Elementliste abarbeiten und Rohrstuecke mit Anfa/Ende direkt ausgeben 
	for (POSITION pos = elelist->GetHeadPosition(); pos != NULL;)	
	{
		Element *element = dynamic_cast<Element*>(elelist->GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRohr()) 
		{			
				
			POSITION p = elelist->FindIndex(counter+1); 
			Element *prev = dynamic_cast<Element*>(elelist->GetAt(p));
			p = elelist->FindIndex(counter+2); 
			Element *next = dynamic_cast<Element*>(elelist->GetAt(p));
			XASSERT(prev && next);
			if (!prev || !next)
				break;

			w.startRow();

			CString txt;

			//Anzahl
			w.appendCol(element->get_anzahl(), rows[0], "CENTER", true);

			//DN bzw. Zoll
			if (!dn2zoll)
				w.appendCol(element->get_dn(), rows[1], "CENTER", true);
			else
				w.appendCol(WaxStamm::dn2zoll(element->get_dn()), rows[1], "CENTER", true);

			//Länge
			w.appendCol(element->get_laenge(), rows[2], "CENTER", true);


			//Anfang u. Ende
			CString a = "", e = "", bezEnde = "-";
			if (prev->get_id() == GRFITTING)
				a = getStammdaten().getText("IDS_GEWIND").c_str();
			else 
				a = prev->get_bezeichnung();

			if (next->get_id() == GRFITTING)
			{
				e = getStammdaten().getText("IDS_GEWIND").c_str();
				bezEnde = "X";
			}
			else
				e = next->get_bezeichnung();

			w.appendCol(a, rows[3], "CENTER", true);
			w.appendCol(e, rows[4], "CENTER", true);
			w.appendCol(bezEnde, rows[5], "CENTER", true);
			w.endRow();
			
		}
		counter++;
	}
	w.endTable();
	
}


///Kopf fuer Gewinderohrliste
void GewinderohrrucklisteZuschnitt::create_Gewinderohrkopf()
{
	WaxHtmlFile &w = html.get_file();
	CString txt, str;

	int anlagentyp		  = 0; 
	int dichtmittel		  = 0;
	int anzahlRohrstuecke = 0;
	int anzahlElemente	  = 0;

	CObListzuschnittliste_Gewinde* elelist = dynamic_cast<CObListzuschnittliste_Gewinde*>(listefromid(listid));
	if(elelist)
	{
		anzahlRohrstuecke = elelist->GetAnzahlRohrstuecke();
		anzahlElemente    = elelist->GetAnzahlFittinge();
	}

	for (int i = 0; i < m_pWalist->GetCount(); i++)
	{
		// Der erste Werkauftrag kann ein Sammelauftrag sein. 
		if (m_pDrucklistenVerwaltung->cur_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(m_pDrucklistenVerwaltung->cur_werkauftrag()))
		{
			CObject *tmp = m_pWalist->GetHead();
			m_pWalist->RemoveHead();
			m_pWalist->AddTail(tmp);
			continue;
		}
		// Daten nur beim 1. Gewinderohr abfragen, da diese im gesamten Sammelauftrag gleich sind
		if (i==0 || i==1)
		{
			Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(m_pDrucklistenVerwaltung->cur_werkauftrag());
			
			anlagentyp = grohr->get_typ();
			dichtmittel = grohr->get_dichtmittel();
		}
		CObject *tmp = m_pWalist->GetHead();
		m_pWalist->RemoveHead();
		m_pWalist->AddTail(tmp);
	}

	// Table 1
	const int rows1[] = { 250, 150, 100, 100, 100, 80 };

	w.startTable();
	w.startRow();
	w.startCol(700);
	
	w.startTable(0,0);
	w.startRow();
	str = getStammdaten().getText("IDS_ANLAGENTYP").c_str();
	w.appendCol(str,	rows1[0]);
	str = getStammdaten().getText("IDS_DICHTMITTEL").c_str();
	w.appendCol(str,	rows1[1]);
	str = getStammdaten().getText("IDS_ANZ_ROHRSTUECKE").c_str();
	w.appendCol(str,	rows1[3]);
	w.endRow();
	
	w.startRow();
	if (anlagentyp == Gewinderohr::WASSER)			txt = getStammdaten().getText("IDS_WASSER").c_str();
	else if (anlagentyp == Gewinderohr::SCHAUM)			txt = getStammdaten().getText("IDS_SCHAUMANLAGE").c_str();
	else if (anlagentyp == Gewinderohr::GAS)		txt = getStammdaten().getText("IDS_GAS").c_str();

	w.appendCol(txt, rows1[0]);

	if (dichtmittel == Gewinderohr::LOCTITE)		txt = getStammdaten().getText("IDS_LOCTITE").c_str();
	else if (dichtmittel == Gewinderohr::HANF)		txt = getStammdaten().getText("IDS_HANF").c_str();
	else if (dichtmittel == Gewinderohr::TEFLON)	txt = getStammdaten().getText("IDS_TEFLON").c_str();
	else if (dichtmittel == Gewinderohr::SYNTHESOL)	txt = getStammdaten().getText("IDS_SYNTHESOL").c_str();
	else if (dichtmittel == Gewinderohr::NEOFERMIT)	txt = getStammdaten().getText("IDS_NEOFERMIT").c_str();
	else if (dichtmittel == Gewinderohr::PERMABOND)	txt = getStammdaten().getText("IDS_PERMABOND").c_str();
	else txt = getStammdaten().getText("IDS_KEINE").c_str();

	w.appendCol(txt, rows1[1]);
	w.appendCol(anzahlRohrstuecke,	rows1[3], "CENTER");
	w.endRow(false);
	w.endTable(false);
		
	w.endCol();
	w.startCol(80);
	
	w.startTable(0,0);
	w.startRow();
	str = getStammdaten().getText("IDS_ANZ_FITTINGE").c_str();
	w.appendCol(str, rows1[4]);
	w.endRow();
	
	w.startRow();
	w.appendCol(anzahlElemente, rows1[4], "CENTER");
	w.endRow(false);
	w.endTable(false);

	w.endCol();
	w.endRow();
	w.endTable();
	return;
}


//Druckliste fuer die Gewinderohre
void GewinderohrrucklisteZuschnitt::createGewinderohrliste()
{
	std::string bemerkung;

	// in den folgenden Aufrufen wird immer der erste Werkauftrag in walist behandelt. In der Schleife
	// wird jeweils der erste Werkauftrag ans Ende verschoben, und so die Liste einmal durchgescrollt.
	

	CObject *tmp = m_pWalist->GetHead();
	Werkauftrag *ersterWerk = dynamic_cast<Werkauftrag*>(tmp);
	XASSERT(ersterWerk);
	if (ersterWerk)
		bemerkung = (LPCSTR)ersterWerk->get_bemerkung();

	std::string ueberschrift = getStammdaten().getText("IDS_BEMERKUNG");
	
	bool Local2quali = m_pDrucklistenVerwaltung->check2Rohrquali(*m_pWalist);		
	
	
	for (int i = 0; i < m_pWalist->GetCount(); i++, m_pWalist->AddTail(m_pWalist->GetHead()), m_pWalist->RemoveHead())
	{
		

		if(dynamic_cast<SammelWerkauftrag*>(m_pDrucklistenVerwaltung->cur_werkauftrag()) && m_pWalist->GetCount() > 1)
			continue;

		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(m_pDrucklistenVerwaltung->cur_werkauftrag());

		string stdbemerkung;
			
		html.insert_bemerkung(bemerkung, ueberschrift, stdbemerkung);
		html.insertQuali(rohr->get_rohrQuali(), Local2quali, rohr->get_dnvl(), 0, rohr->get_typ());
		break;
	}
	// Kopf fuer Gewinderohrliste
	create_Gewinderohrkopf();
	
	WaxHtmlFile &w = html.get_file();
	CString txt, str;
	bool winkelangabe = (WaxSys::para_int(WINKELAUSRICHTUNG) == 0) ? true : false;
	bool zmassabzug = (WaxSys::para_int(ZMASSLISTE) == 1) ? true : false;

	// Table 3 
	const int rows3[] = { 7, 8, 8, 9, 10, 35, 5, 5, 13};

	w.startTable(0,0);
	w.startRow();
	
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,			rows3[0], "LEFT",	true);
	
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows3[1], "RIGHT",	true);
	if (!dn2zoll)
	{
		str = getStammdaten().getText("IDS_NENNW_DN").c_str();
		txt = "<b>" + str + "</b>";
		w.appendCol(txt,	rows3[2], "RIGHT",	true);
	}
	else
	{
		str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
		txt = "<b>" + str + "</b>";
		w.appendCol(txt,	rows3[2], "RIGHT",	true);
	}
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows3[3], "RIGHT",	true);
	
	str = getStammdaten().getText("IDS_LINKS").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,				rows3[4], "CENTER",	true);
	
	str = getStammdaten().getText("IDS_FITTING_RECHTS").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows3[5], "LEFT",	true);
	
	if (winkelangabe)
	{
		str = getStammdaten().getText("IDS_WINKEL_G").c_str();
		txt = "<b>" + str + "</b>";
		w.appendCol(txt,	rows3[6], "RIGHT",	true);
	}
	else
	{
		str = getStammdaten().getText("IDS_AUSRICHT").c_str();
		txt = "<b>" + str + "</b>";
		w.appendCol(txt,		rows3[6], "CENTER",	true);
	}
	str = getStammdaten().getText("IDS_ZUS_FITTING").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows3[7], "CENTER", true);
	
	str = getStammdaten().getText("IDS_KAPPF").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,			rows3[8], "CENTER",	true);
	
	w.endRow();
	w.endTable();

	w.insertTrennlinie();

	for (int i = 0; i < m_pWalist->GetCount(); i++)
	{
		// Der erste Werkauftrag kann ein Sammelauftrag sein. 
		if (m_pDrucklistenVerwaltung->cur_werkauftrag()->isEmpty() || dynamic_cast<SammelWerkauftrag*>(m_pDrucklistenVerwaltung->cur_werkauftrag()))
		{
			CObject *tmp = m_pWalist->GetHead();
			m_pWalist->RemoveHead();
			m_pWalist->AddTail(tmp);
			continue;
		}
		Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(m_pDrucklistenVerwaltung->cur_werkauftrag());
		XASSERT(grohr);
		if (!grohr)
			return;

		grohr->createModell();
		if (grohr->get_elementList().IsEmpty())
			continue;

		w.startTable(0,0);

		int rohranzahl = 0, fittanzahl = 0;
		
		ElementList elelst(grohr->get_elementList());
		// Abarbeiten der Elementliste
		for (int count=0; count<elelst.get_count(); count++)	
		{
			// Element aus der Liste 
			Element *ele = dynamic_cast<Element*>(elelst.get_at(count));
			XASSERT(ele);
			if (!ele)
				return;

			if (ele->isEleRohr())
				rohranzahl++;
			else if (ele->get_id() == GRFITTING)
				fittanzahl++; 
		}

		bool first = true, rohr = false, fitt = false, last = false;
		// Elementliste abarbeiten 
		for (int count=0; count<elelst.get_count(); count++)	
		{
			// Element aus der Liste 
			Element *ele = dynamic_cast<Element*>(elelst.get_at(count));
			XASSERT(ele);
			if (!ele)
				return;

			Element* anfang = dynamic_cast<Element*>(elelst.get_at(0));

			rohr = false, fitt = false;
			if (count == elelst.get_count()-1)
				last = true;

			Element* prev = NULL;
			Element* next = NULL;
			if (count > 0)
				prev = dynamic_cast<Element*>(elelst.get_prev(count));
			if (count < elelst.get_count()-1)
				next = dynamic_cast<Element*>(elelst.get_next(count));

			int laenge = 0;

			if (ele->get_id() == GRFITTING)
			{
				fitt = true;
				if (prev && prev->isEleRohr())
				{
					if (zmassabzug)
                        laenge = prev->get_laenge();
					else
						laenge = ele->get_abstRel();
				}
			}
			else if (ele->isEleRohr()) 
			{
				if (next && next->get_id() != GRFITTING)
				{
					laenge = ele->get_laenge();
					rohr = true;
				}
			}

			if (fitt || rohr)
			{
				w.startRow();
				w.appendCol(grohr->get_posNr(),  rows3[0], "LEFT", true);
				w.appendCol(grohr->get_anzahl(), rows3[1], "CENTER", true);
				CString txt;
				if (first) 
				{
					if (fitt && laenge == 0)
					{
						w.appendCol(" ", rows3[2], "RIGHT", true);
						w.appendCol(" ", rows3[3], "RIGHT", true);
					} 
					else
					{
						if (!dn2zoll)
							txt.Format("%d *", ele->get_anschlussDN());
						else
							txt.Format("%s *", WaxStamm::dn2zoll(ele->get_anschlussDN()));
						w.appendCol(txt,	rows3[2], "RIGHT", true);
						w.appendCol(laenge, rows3[3], "RIGHT", true);
					}
					
					if (fitt && ele->get_firstele())
						txt = "";
					else if (fitt)
						txt = anfang->get_bezeichnung();
					else
						txt = anfang->get_bezeichnung();
						w.appendCol(txt, rows3[4], "CENTER", true);
			
					if (fitt)
						txt = ele->get_kurztext();
					else 
						txt = next->get_bezeichnung();
					w.appendCol(txt, rows3[5], "LEFT", true);
				}
				else 
				{
					if (!dn2zoll)
						txt.Format("%d *", ele->get_anschlussDN());
					else
						txt.Format("%s *", WaxStamm::dn2zoll(ele->get_anschlussDN()));
					w.appendCol(txt,	rows3[2], "RIGHT", true);
					if (fitt && laenge == 0)
						w.appendCol("-", rows3[3], "RIGHT", true);
					else 
						w.appendCol(laenge, rows3[3], "RIGHT", true);
					
					txt = "";
					w.appendCol(txt,	rows3[4], "CENTER", true);
					if (fitt)
						txt = ele->get_kurztext();
					else 
						txt = next->get_bezeichnung();
					w.appendCol(txt,	rows3[5], "LEFT", true);
				}
				if (fitt)
				{
					if (winkelangabe)
						w.appendCol(ele->get_winkel(), rows3[6], "RIGHT", true);
					else
					{
						if (ele->get_winkel() == 0)
						{
							str = getStammdaten().getText("IDS_OBEN").c_str();
							w.appendCol(str,		rows3[6], "CENTER", true);
						}
						else if (ele->get_winkel() == 90)
						{
							str = getStammdaten().getText("IDS_HINTEN").c_str();
							w.appendCol(str,	rows3[6], "CENTER", true);
						}
						else if (ele->get_winkel() == 180)
						{
							str = getStammdaten().getText("IDS_UNTEN").c_str();
							w.appendCol(str,	rows3[6], "CENTER", true);
						}
						else if (ele->get_winkel() == 270)
						{
							str = getStammdaten().getText("IDS_VORNE").c_str();
							w.appendCol(str,	rows3[6], "CENTER", true);
						}
					}
				}
				else
					w.appendCol("", rows3[6], "RIGHT", true);
				txt = "-";
				w.appendCol(txt, rows3[7], "CENTER", true);
				
				txt = (grohr->get_rohrKFarbe()==0)?"-":grohr->getKappenfarbe(grohr->get_rohrKFarbe());
				
				if (rohranzahl < 2 && fittanzahl < 2)
				{
					first = false;
					w.appendCol(txt, rows3[8], "CENTER", true);
				}
				else if (first)
				{
					first = false;
					w.appendCol(txt+" &nbsp;&nbsp;<--,", rows3[8], "CENTER", true);
				}
				else
				{
					if ((last && ele->get_uelementList().IsEmpty()) || !fitt)
						w.appendCol(txt+" &nbsp;&nbsp;_ _|", rows3[8], "CENTER", true);
					else
						w.appendCol(txt+" &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|", rows3[8], "CENTER", true);
				}

				w.endRow();
			}

			if (fitt)
			{
				for (POSITION upos = ele->get_uelementList().GetHeadPosition(); upos != NULL;)	
				{
					Element* uele = dynamic_cast<Element*>(ele->get_uelementList().GetNext(upos));
	
					w.startRow();
					w.appendCol(grohr->get_posNr(),		rows3[0], "LEFT",	true);
					w.appendCol(grohr->get_anzahl(),	rows3[1], "CENTER",	true);
					w.appendCol("",						rows3[2], "RIGHT",	true);
					w.appendCol("",						rows3[3], "RIGHT",	true);
					w.appendCol("",						rows3[4], "CENTER", true);
					w.appendCol(uele->get_kurztext(),	rows3[5], "LEFT",	true);
					if (winkelangabe)
						w.appendCol(uele->get_winkel(),	rows3[6], "RIGHT",	true);
					else
					{
						if (uele->get_winkel() == 0)
						{
							str = getStammdaten().getText("IDS_OBEN").c_str();
							w.appendCol(str,			rows3[6], "CENTER", true);
						}
						else if (uele->get_winkel() == 90)
						{
							str = getStammdaten().getText("IDS_HINTEN").c_str();
							w.appendCol(str,		rows3[6], "CENTER", true);
						}
						else if (uele->get_winkel() == 180)
						{
							str = getStammdaten().getText("IDS_UNTEN").c_str();
							w.appendCol(str,		rows3[6], "CENTER", true);
						}
						else if (uele->get_winkel() == 270)
						{
							str = getStammdaten().getText("IDS_VORNE").c_str();
							w.appendCol(str,		rows3[6], "CENTER", true);
						}
					}
					txt.Format("%s", (uele->get_verkettung()==0)?"-":"X");
					w.appendCol(txt,					rows3[7], "CENTER", true);
					txt = (grohr->get_rohrKFarbe()==0)?"-":grohr->getKappenfarbe(grohr->get_rohrKFarbe());
					if (rohranzahl < 2)
						w.appendCol(txt,				rows3[8], "CENTER", true);
					else if (last && upos == NULL)
						w.appendCol(txt+" &nbsp;&nbsp;_ _|", rows3[8], "CENTER", true);
					else
						w.appendCol(txt+" &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|", rows3[8], "CENTER", true);
					w.endRow();
				}
			}
		}
		CObject *tmp = m_pWalist->GetHead();
		m_pWalist->RemoveHead();
		m_pWalist->AddTail(tmp);

		w.endTable();
		
		// Ggbf. Fertigungstext
		if (grohr->get_fertigungshinweis().GetLength())	
		{
			html.insert_bemerkung((LPCSTR)grohr->get_fertigungshinweis(), getStammdaten().getText("IDS_FERTIGUNGSHINWEIS").c_str());
		}
		
		if (i < m_pWalist->GetCount()-1)
			w.insertTrennlinie();
	}
	w.endTable();
}