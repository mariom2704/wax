#include "stdafx.h"
#include "DialogWaxHtmlFile.h"

	#include "DialogHtmlDruck.h"
#include "DialogDrucklisteWap.h"
#include "GpVk3Preis.h"

using namespace WaxDruck;

DialogDrucklisteWap::DialogDrucklisteWap(DialogHtmlDruck& file, WaxPart* pt, GpWapPreis::PREISART part) : DialogDruckliste(file, pt)
{
	preisart = part;
}

DialogDrucklisteWap::~DialogDrucklisteWap()
{
}


DialogDrucklisteWap::DialogDrucklisteWap(DialogHtmlDruck& file, CObList* ptWerkauf, GpWapPreis::PREISART part) : DialogDruckliste(file, ptWerkauf)	
{
	preisart = part;
}





/**
Druckliste WAP erzeugen
*/
double DialogDrucklisteWap::createWap()
{
	addBemerkung();

	double gesMat = 0;
	double gesFert = 0;
	double gesFremd = 0;
	double selbstkosten = 0;
	string ueberschrift;
	
	ueberschrift = getStammdaten().getText("IDS_MATERIALKOSTEN");
	addMaterialkosten(gesMat, true, ueberschrift);
	
	ueberschrift = getStammdaten().getText("IDS_FERTIGUNGSKOSTEN");
	addFertigungskosten(gesFert, true, ueberschrift, FALSE);

	addGesKosten(gesMat, gesFert, 0, selbstkosten);

	addFremdleistungen(gesFremd);
	
	GpVk3Preis vk3;
	
	if(waxpart != NULL)
	{
		// Für Druckliste
		addVk3(vk3.getVk3Preis(this->waxpart), selbstkosten);
	}
	else
	{
		// Für den Export
		int countAmountOfPipes = get_AnzahlRohreProPos();
		selbstkosten = selbstkosten/countAmountOfPipes;
	}

	
	return selbstkosten;
}


int DialogDrucklisteWap::get_AnzahlRohreProPos()
{
	int countAmountOfPipes = 0;
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{	
		//Sammelaufträge überspringen
		const SammelWerkauftrag* sammel = dynamic_cast<const SammelWerkauftrag*>(*it);
		if (sammel)
			continue;

		const Werkauftrag* auftrag = dynamic_cast<const Werkauftrag*>(*it);
		if (!auftrag)
			continue;
		
		countAmountOfPipes += auftrag->get_anzahl();

	}

	return countAmountOfPipes;

}




/**
Druckliste Material erzeugen
*/
bool DialogDrucklisteWap::createMaterialliste(bool showPreise)
{
	addBemerkung();
	addRohrQuali();

	double gesMat = 0;
	addMaterialkosten(gesMat, showPreise, "");

	return true;
}



bool DialogDrucklisteWap::createArbeitsplan(bool showPreise, BOOL last)
{
	/*
	addBemerkung();
	addUberschriftWaNrPosNr();
	addRohrQuali();
*/
	double gesFert = 0;
	addFertigungskosten(gesFert, showPreise, "", last);

	return true;
}





void DialogDrucklisteWap::addUberschriftWaNrPosNr()
{
	

	//Rohr suchen
	Auftragsliste::iterator it = auftraege.begin();
		
	Werkauftrag* pWerkauftrag = dynamic_cast< Werkauftrag*>(*it);

	
	DialogWaxHtmlFile &w = html.get_file();

	w.startTable();

	CString strTemp;
	 strTemp +=	CString("<TR><TD WIDTH='30%' VALIGN='MIDDLE' COLSPAN=1>") + 
			CString("<P ALIGN='CENTER'><IMG SRC='Minimax.jpg' WIDTH=155 HEIGHT=23></TD>") +
			CString("<TD WIDTH='50%' VALIGN='MIDDLE' COLSPAN=2>") +
			CString("<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>Arbeitsplan</B></FONT></TD>") +
			CString("<TD WIDTH='20%' VALIGN='MIDDLE' COLSPAN=1>") +
			CString("<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'><DIV id='ele3' ALIGN='CENTER'></DIV></B></FONT></TD></TR>");

	 w.WriteString(strTemp);
	 w.endTable();

	w.startTable();
	w.startRow();
	w.appendCol("Wanr", 15, "left", true);

	CString wan;
	wan.Format("%06ld", pWerkauftrag->get_status().get_wanr());
	w.appendCol(wan,  15, "left", true);

	w.appendCol("PosNr", 15, "left", true);
	wan = pWerkauftrag->get_posNr();
	w.appendCol(wan,  15, "left", true);

	w.endRow();

	w.endTable();
	

	//w.WriteString();
		
/*
	if (bemerkung.length() > 0)
	{
		std::string ueberschrift = getStammdaten().getText("IDS_BEMERKUNG");
		html.insert_bemerkung(bemerkung, ueberschrift, stdbemerkung);
	}
*/
}

void DialogDrucklisteWap::addMaterialkosten(double &gesMat, bool showPreis, const string ueberschrift)
{
	//Gesamtliste bilden
	MaterialListe matliste;
	setGesamtmaterialliste(matliste);


	//Gesamtpreis und Zuschlag berechnen
	double gesGldPreis = 0;
	double gesFestPreis = 0;
	double gesPulverpreis = 0;
	double zuschlag = 0;
	double prozent = 0;
	GpWapPreis wapPreis;
	wapPreis.calcMaterialkosten(preisart, matliste, gesFestPreis, gesGldPreis, gesPulverpreis, prozent, zuschlag);

	DialogWaxHtmlFile &w = html.get_file();
	CString txt, str;

	//Überschrift
	if (!ueberschrift.empty())
	{
		txt.Format("<p><b>%s:</b></p>", ueberschrift.c_str());
		w.WriteString(txt);
	}

	//Tabelle Matkosten
	const int rows[] = {80, 400, 50, 50, 80, 80, 80, 80};
	
	w.startTable();
	w.startRow();
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	w.appendCol(str,	rows[0], "center");
	str = getStammdaten().getText("IDS_BEZEICHNUNG").c_str();
	w.appendCol(str,	rows[1], "center");
	str = getStammdaten().getText("IDS_ART_NR").c_str();
	w.appendCol(str,	rows[2], "center");
	str = getStammdaten().getText("IDS_EK").c_str();
	w.appendCol(str,	rows[3], "center");
	str = getStammdaten().getText("IDS_GEWICHT_K").c_str();
	w.appendCol(str,	rows[4], "CENTER");
	if (showPreis)
	{
		

		str = getStammdaten().getText("IDS_PREIS_GLD").c_str();
		w.appendCol(str,	rows[6], "center");

		if (gesPulverpreis > 0)
		{
			str = getStammdaten().getText("IDS_PULVERPREIS_E").c_str();
			w.appendCol(str,	rows[7], "CENTER");
		}
		
	}
	w.endRow();

	double gesGewicht = 0;
	double gewicht = 0;
	MxCObList& matlist = matliste.get_matlist();
	POSITION pos =  matlist.GetHeadPosition();
	for (pos = matlist.GetHeadPosition(); pos != NULL;) 
	{
		Material *material = dynamic_cast<Material*>(matlist.GetNext(pos));
		XASSERT(material);
		if (!material)
		{
			str = getStammdaten().getText("IDS_FEHLER_MATLIST").c_str();
			w.appendCol(str, rows[0]);	
			return;
		}
		w.startRow();
		if (material->get_mass1() > 0)
		{
			txt.Format("%.3f", (double)material->get_mass1()/1000);
			w.appendCol(txt, rows[0], "RIGHT");		
		}
		else
			w.appendCol(material->get_anzahl(), rows[0]);		
		
		txt = getStammdaten().getSapArtikelbezeichnung(material->get_artikelnr());
		if (txt.IsEmpty())
			txt = material->get_bezeichnung();

		w.appendCol(txt, rows[1]);
	
		w.appendCol(material->get_artikelnr(), rows[2], "CENTER");
		
		w.appendCol((material->get_einkauf())?"x":"-", rows[3], "CENTER");

		gewicht = material->getGewicht();
		gesGewicht +=gewicht;
		txt.Format("%.3f", gewicht);
		w.appendCol(txt, rows[4], "RIGHT");

		if (showPreis)
		{
			

			if (material->get_mass1() > 0)
				w.appendCol(material->get_sapPreis() * (double)material->get_mass1()/1000, rows[5]);
			else 
				w.appendCol(material->get_sapPreis() * (double)material->get_anzahl(), rows[5]);

			if (gesPulverpreis > 0)
			{
				if (material->get_mass1() > 0)
					w.appendCol(material->get_pulverpreis() * (double)material->get_mass1()/1000, rows[6]);
				else 
					w.appendCol(material->get_pulverpreis() * (double)material->get_anzahl(), rows[6]);
			}
		}
		w.endRow();
	}

	if (showPreis)
	{
		//Summe Materialkosten
		w.startRow();
		str = getStammdaten().getText("IDS_GESAMT").c_str();
		txt.Format("<b>%s</b>", str);
		w.appendCol(txt, rows[0]);
		w.appendCol("",	rows[1]);
		w.appendCol("",	rows[2], "CENTER");		
		w.appendCol("",	rows[3], "CENTER");	
		txt.Format("<b>%.3f<b>", gesGewicht);
		w.appendCol(txt, rows[4], "RIGHT");	
		
		txt.Format("<b>%.2f</b>", gesGldPreis);
		w.appendCol(txt, rows[5], "RIGHT");
		if (gesPulverpreis > 0)
		{
			txt.Format("<b>%.2f</b>", gesPulverpreis);
			w.appendCol(txt, rows[6], "RIGHT");
		}
		w.endRow();

		//Zuschlag
		w.startRow();
		str.Format("%s [%.2f%s]",getStammdaten().getText("ids_mgk").c_str(), prozent * 100.0, "%");
		w.appendCol(str,rows[0]);
		w.appendCol("",	rows[1]);
		w.appendCol("",	rows[2], "CENTER");		
		w.appendCol("",	rows[3], "CENTER");	
		w.appendCol("",	rows[4], "CENTER");	
		
		txt.Format("%.2f", zuschlag);
		w.appendCol(txt,rows[5], "RIGHT");
		if (gesPulverpreis > 0)
			w.appendCol("",	rows[6], "CENTER");	
		w.endRow();

		//Gesamtkosten Material
		w.startRow();
		str = getStammdaten().getText("IDS_SUMMEMAT").c_str();
		txt.Format("<b>%s</b>", str);
		w.appendCol(txt, rows[0]);
		w.appendCol("",	rows[1]);
		w.appendCol("",	rows[2], "CENTER");		
		w.appendCol("",	rows[3], "CENTER");	
		w.appendCol("",	rows[4], "CENTER");
		txt.Format("<b>%.2f</b>", gesGldPreis + zuschlag);
		w.appendCol(txt, rows[5], "RIGHT");
		if (gesPulverpreis > 0)
			w.appendCol("",	rows[6], "CENTER");	
		w.endRow();
	}
	else
	{
		w.startRow();
		str = getStammdaten().getText("IDS_GESAMT").c_str();
		txt.Format("<b>%s</b>", str);
		w.appendCol(txt, rows[0]);
		w.appendCol("",	rows[1]);
		w.appendCol("",	rows[2], "CENTER");		
		w.appendCol("",	rows[3], "CENTER");	
		txt.Format("<b>%.3f<b>", gesGewicht);
		w.appendCol(txt, rows[4], "RIGHT");	
		w.endRow();
	}

	w.endTable();

	gesMat = gesGldPreis + zuschlag;

	return;
}

void DialogDrucklisteWap::addFertigungskosten(double &gesPreisMitZuschlag, bool showPreise, const string ueberschrift, BOOL last)
{
	Beschichtung beschichtung;
	int maxlaenge = 0;
	double gesGewicht = 0;
	int wanr = 0;

	Arbeitsplan ap;
	setGesamtArbeitsplan(ap, beschichtung, maxlaenge, gesGewicht, wanr);

	if (ap.get_arbeitsfolgen().getSize() == 0)
		return;

	//Preise berechnen
	gesPreisMitZuschlag = 0;
	double gesPreis = 0;
	double gesZeit = 0;
	GpWapPreis wapPreis;
	wapPreis.calcFertigungskosten(auftraege, preisart, beschichtung, maxlaenge, gesGewicht, wanr, ap, gesPreis, gesPreisMitZuschlag, gesZeit);

	
	DialogWaxHtmlFile &w = html.get_file();
	CString txt, str;

	
	txt.Format("<p></p>", ueberschrift.c_str());
	w.WriteString(txt);
	

	//Überschrift
	if (!ueberschrift.empty())
	{
		txt.Format("<p><b>%s:</b></p>", ueberschrift.c_str());
		w.WriteString(txt);
	}

	// Tabellenspalten
	const int rows[] = { 30, 60, 30, 30, 300, 60, 60, 60, 60, 60, 60, 60, 60};
	
	w.startTable();
	w.startRow();
	str = getStammdaten().getText("IDS_NR").c_str();
	w.appendCol(str, rows[0], "CENTER");
	str = getStammdaten().getText("IDS_ARBEITSPLATZ").c_str();
	w.appendCol(str, rows[1], "CENTER");
	
	if (WaxSys::isAdministrator())
	{
		str = getStammdaten().getText("IDS_CODE").c_str();
		w.appendCol(str, rows[2], "CENTER");
		str = getStammdaten().getText("IDS_ANZAHL").c_str();
		w.appendCol(str, rows[3], "CENTER");
	}

	str = getStammdaten().getText("IDS_TEXT").c_str();
	w.appendCol(str, rows[4], "CENTER");
	str = getStammdaten().getText("IDS_ZEIT_H").c_str();
	w.appendCol(str, rows[5], "CENTER");
	
	if (WaxSys::isAdministrator())
	{
		str = getStammdaten().getText("IDS_AFOZEIT").c_str();
		w.appendCol(str, rows[6], "CENTER");
		str = getStammdaten().getText("IDS_RUESTZEIT_H").c_str();
		w.appendCol(str, rows[7], "CENTER");
	}
	if (showPreise)
	{
		str = getStammdaten().getText("IDS_KOSTENSTELLE").c_str();
		w.appendCol(str, rows[8], "CENTER");
		str = getStammdaten().getText("IDS_TARIF").c_str();
		w.appendCol(str, rows[9], "CENTER");
		str = getStammdaten().getText("ids_fk").c_str();
		w.appendCol(str, rows[12], "CENTER");
	}

	w.endRow();

	CString strKostenstelle = _T("0");
	for (POSITION pos = ap.get_arbeitsfolgen().GetHeadPosition(); pos; )
	{
		Arbeitsfolge *folge = (Arbeitsfolge*)(ap.get_arbeitsfolgen().GetNext(pos));
		
		w.startRow();
		w.appendCol(folge->get_nr(), rows[0]);
		w.appendCol(folge->get_arbpl(),	rows[1]);

		if (WaxSys::isAdministrator())
		{
			w.appendCol(folge->get_id(), rows[2]);
			w.appendCol(folge->get_anz(), rows[3]);
		}
		w.appendCol(folge->get_txt(),	rows[4]);
		if( folge->get_arbpl().Compare(_T("99000")) == 0)
		{
			strKostenstelle = _T("99000");
			txt = _T("0.0000");
			w.appendCol(txt, rows[5], "RIGHT");
		}
		else
		{
			txt.Format("%.4f", folge->get_zeit()+folge->get_ruestzeit());
			w.appendCol(txt, rows[5], "RIGHT");
		}
		
		if (WaxSys::isAdministrator())
		{
			if( folge->get_arbpl().Compare(_T("99000")) == 0)
			{
				txt = _T("0.0000");
				w.appendCol(txt, rows[6], "RIGHT");
				txt = _T("0.0000");
				w.appendCol(txt, rows[7], "RIGHT");	
			}
			else
			{
				txt.Format("%.4f", folge->get_zeit());
				w.appendCol(txt, rows[6], "RIGHT");
				txt.Format("%.4f", folge->get_ruestzeit());
				w.appendCol(txt, rows[7], "RIGHT");	
			}
		}
		if (showPreise)
		{
			w.appendCol(folge->get_kostst(),rows[8]);
			w.appendCol(folge->get_ppstd(), rows[9]);
			w.appendCol(folge->get_preis() * (1 + folge->get_zuschlag() / 100.0), rows[12]);
			w.endRow();
		}
	}

	str = getStammdaten().getText("IDS_GESAMT").c_str();
	txt = "<b>" + str + "</b>";
	w.startRow();
	w.appendCol(txt, rows[0]);
	w.appendCol("", rows[1], "RIGHT");
	
	if (WaxSys::isAdministrator())
	{
		w.appendCol("", rows[2], "RIGHT");
		w.appendCol("", rows[3], "RIGHT");
	}
	w.appendCol("", rows[4], "RIGHT");
	if( strKostenstelle.Compare(_T("99000")) == 0)
	{
		txt = _T("0.0000");
		w.appendCol(txt, rows[5], "RIGHT");
	}
	else
	{
		txt.Format("<b>%.4f</b>", gesZeit);
		w.appendCol(txt, rows[5], "RIGHT");
	}

	if (WaxSys::isAdministrator())
	{
		w.appendCol("",	rows[6], "RIGHT");
		w.appendCol("",	rows[7], "RIGHT");
	}
	if (showPreise)
	{
		w.appendCol("",	rows[8], "RIGHT");
		w.appendCol("",	rows[9], "RIGHT");
		txt.Format("<b>%.2f</b>", gesPreisMitZuschlag);
		w.appendCol(txt, rows[12], "RIGHT");
		w.endRow();
	}

	w.endTable();

	txt = "<p><br></br></p>";
	w.WriteString(txt);
	if(last == FALSE)
	{
		w.insertSeitenumbruch();
	}

}


void  DialogDrucklisteWap::setGesamtArbeitsplan(Arbeitsplan &ap, Beschichtung &beschichtung, int &maxLaenge, double &gesGewicht, int &wanr)
{
	gesGewicht = 0;
	maxLaenge = 0;
	bool isSetBesch = false;
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{	
		//Sammelaufträge überspringen
		const SammelWerkauftrag* sammel = dynamic_cast<const SammelWerkauftrag*>(*it);
		if (sammel)
			continue;

		const Werkauftrag* auftrag = dynamic_cast<const Werkauftrag*>(*it);
		if (!auftrag)
			continue;

		if (!isSetBesch)
		{
			beschichtung.copy(auftrag->getBeschichtung());
			isSetBesch = true;
		}

		if (wanr <= 0)
			wanr = auftrag->get_status_const().get_wanr();

		gesGewicht += auftrag->getGesGewicht();

		if (maxLaenge < auftrag->getMaxLaenge())
			maxLaenge = auftrag->getMaxLaenge();

		ap.merge(auftrag->get_arbeit());
	}
}

void DialogDrucklisteWap::setGesamtmaterialliste(MaterialListe &matliste)
{
	bool isSammel = false;

	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const Werkauftrag* auftrag = dynamic_cast<const Werkauftrag*>(*it);
		if (!auftrag)
			continue;

		// Material aus der (Fertigungs)Stückliste ermitteln
		for (POSITION pos = auftrag->get_stueckliste().GetHeadPosition(); pos;)
		{
			const Element *ele = dynamic_cast<const Element*>(auftrag->get_stueckliste().GetNext(pos));
			XASSERT(ele);
			if (!ele)
				break;

			POSITION upos;
			for (upos = matliste.get_matlist().GetHeadPosition(); upos;)
			{
				Material *mat = dynamic_cast<Material*>(matliste.get_matlist().GetNext(upos));
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
				matliste.get_matlist().AddTail(m);
			}
		}
		

		//Wenn Autragsliste einen Sammelauftrag enthält, das Material von diesem nehmen, danach aber nicht mehr von den Positionen
		//da dies sonst doppelt ist. Wax kopiert das Material von den Positionen immer in den Sammelauftrag. 
		if (!isSammel)
		{
			//Zusätzliches Material ermitteln
			for (POSITION pos = auftrag->get_material().get_matlist().GetHeadPosition(); pos;)
			{
				const Material *ele = dynamic_cast<const Material*>(auftrag->get_material().get_matlist().GetNext(pos));
				XASSERT(ele);
				if (!ele)
					break;

				POSITION upos = NULL;
				if (ele->get_id() != NOARTNR && ele->get_artikelnr() != 0)
				{					
					for (upos = matliste.get_matlist().GetHeadPosition(); upos;)
					{
						Material *mat = dynamic_cast<Material*>(matliste.get_matlist().GetNext(upos));
						XASSERT(mat);
						if (!mat)
							break;

						if (mat->get_artikelnr() == ele->get_artikelnr()) 
						{
							// Bei Rohr & Profil die Laenge als mm in Mass1 speichern 
							if (ele->get_mass1() > 0)
								mat->set_mass1(mat->get_mass1() + (ele->get_mass1() * ele->get_anzahl()));
							else 
								mat->set_anzahl(mat->get_anzahl() + ele->get_anzahl());
							upos--;
							break;
						}
					}
				}

				// in Gesamtliste packen
				if (upos == NULL) 
				{
					Material *m = new Material;
					m->copy(*ele);
					// Bei Rohr & Profil die Laenge als Meter in Mass1 speichern 
					if (ele->get_mass1() > 0)
						m->set_mass1(ele->get_mass1() * ele->get_anzahl());
					else
						m->set_mass1(0);
					matliste.get_matlist().AddTail(m);
				}
			}
		}

		//Wenn Auftrag Sammelauftrag ist, dies merken damit aus den Positionen kein weiteres Material eingelesen wird
		//da Wax das Material immer aus den Positionen in den Sammelauftrag kopiert !!!!
		if (dynamic_cast<const SammelWerkauftrag*>(auftrag))
			isSammel = true;
	}
}



void DialogDrucklisteWap::addGesKosten(double gesMat, double gesFert, double gesFremd, double &selbstkosten)
{
	DialogWaxHtmlFile &w = html.get_file();

	double zuschlag = 0;
	double zuschlagProzent = 0;
	double herstKosten = 0;
	selbstkosten = 0;

	//Kosten berechnen
	GpWapPreis wapPreis;
	wapPreis.calcSelbstkosten(preisart, gesMat, gesFert, gesFremd, zuschlag, zuschlagProzent, herstKosten, selbstkosten);


	//Überschrift
	CString str; str = getStammdaten().getText("IDS_GESAMTKOSTEN").c_str();
	CString txt; txt.Format("<p><b>%s:</b></p>", str);
	w.WriteString(txt);

	//Tabelle 
	const int rows[] = {500, 50};
	
	w.startTable();
	w.startRow();
	str = getStammdaten().getText("IDS_BEZEICHNUNG").c_str();
	w.appendCol(str,	rows[0], "center");
	str = getStammdaten().getText("IDS_PREIS_E").c_str();
	w.appendCol(str,	rows[1], "center");
	w.endRow();


	w.startRow();
	str = getStammdaten().getText("IDS_SUMMEHERSTKOSTEN").c_str();
	txt.Format("<b>%s</b>", str);
	w.appendCol(txt, rows[0]);
	
	txt.Format("<b>%.2f</b>", herstKosten);
	w.appendCol(txt, rows[1], "RIGHT");
	w.endRow();

	w.startRow();
	str.Format("%s [%.2f%s]", getStammdaten().getText("IDS_VVZUSCHLAG").c_str(), zuschlagProzent * 100.0, "%");
	w.appendCol(str, rows[0]);
	
	txt.Format("%.2f", zuschlag);
	w.appendCol(txt, rows[1], "RIGHT");
	w.endRow();

	w.startRow();
	str = getStammdaten().getText("IDS_SELBSTKOSTEN").c_str();
	txt.Format("<b>%s</b>", str);
	w.appendCol(txt, rows[0]);
	
	txt.Format("<b>%.2f</b>", selbstkosten);
	w.appendCol(txt, rows[1], "RIGHT");
	w.endRow();

	w.endTable();

}


void DialogDrucklisteWap::addFremdleistungen(double &gesFremd)
{
	gesFremd = 0;

	//Zinkkosten
	ZinkKosten zk;
	getZinkkosten(zk);

	if (zk.get_gesamt() <= 0)
		return;

	gesFremd = zk.get_gesamt();


	DialogWaxHtmlFile &w = html.get_file();

	//Überschrift
	CString str; str = getStammdaten().getText("IDS_FREMDLEISTUNGEN").c_str();
	CString txt; txt.Format("<p><b>%s:</b></p>", str);
	w.WriteString(txt);

	//Tabelle Matkosten
	const int rows[] = {50, 500, 50};
	
	w.startTable();
	w.startRow();
	str = getStammdaten().getText("IDS_NR").c_str();
	w.appendCol(str,	rows[0], "center");
	str = getStammdaten().getText("IDS_BEZEICHNUNG").c_str();
	w.appendCol(str,	rows[1], "center");
	str = getStammdaten().getText("IDS_PREIS_E").c_str();
	w.appendCol(str,	rows[2], "center");
	w.endRow();

	int count = 1;
	w.startRow();
	w.appendCol(count, rows[0], "CENTER");
	w.appendCol(getStammdaten().getText("IDS_ZINKKOSTEN").c_str(), rows[1]);
	txt.Format("%.2f", zk.get_gesamt());
	w.appendCol(txt, rows[0], "CENTER");
	w.endRow();


	w.endTable();
}



void DialogDrucklisteWap::getZinkkosten(ZinkKosten &zk)
{
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const Rohrleitung* rohr = dynamic_cast<const Rohrleitung*>(*it);
		if (!rohr)
			continue;

		if (!rohr->getBeschichtung().isVerzinkt())
			continue;

		const Verzinkung::data* d = getStammdaten().get_verzinkung().query(rohr->getBeschichtung().get_verzinkerei());
		if (!d)
			continue;

		ZinkKosten z(d, *rohr);
		zk.add(z);
	}
}


void DialogDrucklisteWap::addRohrQuali()
{
	//Rohr suchen
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const Rohrleitung* rohr = dynamic_cast<const Rohrleitung*>(*it);
		if (rohr)
		{
			html.insertQuali(rohr->get_rohrQuali(), twoquali, rohr->get_dnvl(), 0, rohr->get_typ());
			break;
		}
	}
}


void DialogDrucklisteWap::addBemerkung()
{
	std::string bemerkung;
	std::string stdbemerkung;

	//Rohr suchen
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const Werkauftrag* rohr = dynamic_cast<const Werkauftrag*>(*it);
		if (rohr)
		{
			
			bemerkung = (LPCSTR)rohr->get_bemerkung();

			break;
		}
	}

	if (bemerkung.length() > 0)
	{
		std::string ueberschrift = getStammdaten().getText("IDS_BEMERKUNG");
		html.insert_bemerkung(bemerkung, ueberschrift, stdbemerkung);
	}
}


void DialogDrucklisteWap::addVk3(double vk3Ges, double selbstkosten)
{

	DialogWaxHtmlFile &w = html.get_file();


	//Überschrift
	CString str; str = getStammdaten().getText("IDS_VK3").c_str();
	CString txt; txt.Format("<p><b>%s:</b></p>", str);
	w.WriteString(txt);

	//Tabelle 
	const int rows[] = {500, 50};
	
	w.startTable();
	w.startRow();
	str = getStammdaten().getText("IDS_BEZEICHNUNG").c_str();
	w.appendCol(str,	rows[0], "center");
	str = getStammdaten().getText("IDS_PREIS_E").c_str();
	w.appendCol(str,	rows[1], "center");
	w.endRow();


	w.startRow();
	str = getStammdaten().getText("IDS_VK3_GES").c_str();
	txt.Format("<b>%s</b>", str);
	w.appendCol(txt, rows[0]);
	
	txt.Format("<b>%.2f</b>", vk3Ges);
	w.appendCol(txt, rows[1], "RIGHT");
	w.endRow();

	w.startRow();
	str = getStammdaten().getText("IDS_DELTA").c_str();
	txt.Format("<b>%s</b>", str);
	w.appendCol(txt, rows[0]);
	
	txt.Format("<b>%.2f</b>", vk3Ges - selbstkosten);
	w.appendCol(txt, rows[1], "RIGHT");
	w.endRow();

	w.endTable();
}