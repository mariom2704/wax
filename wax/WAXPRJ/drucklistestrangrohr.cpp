#include "stdafx.h"


#include "drucklistestrangrohr.h"

#include <mxutil/strloader.h>
using namespace MxUtil;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;


DrucklisteStrangrohr::DrucklisteStrangrohr(HtmlDruck& file, WaxPart* pt) : Druckliste(file, pt)
{
	gesamtlaenge = 0;
	anzahlRohre = 0;
	anzahlElemente = 0;
	rohrQuali = 0; 
	dnvl	  = 0;
	elementID = 0;
	elementDN = 0;
	sondermuffenlaenge = 0;
	durchmesser = 0;
	trockenanlage = false;
	gekehlt = false;
}

bool DrucklisteStrangrohr::create()
{
	bool bFertigungshinweis = false;
	
	
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		Strangrohr* strang = dynamic_cast<Strangrohr*>(*it);
		if (!strang)
			continue;

		// Daten sind für alle Unteraufträge gleich
		if (!rohrQuali)
		{
			rohrQuali = strang->get_rohrQuali();
			dnvl	  = strang->get_dnvl();
			elementID = strang->get_muffenID();
			elementDN = strang->get_muffenDN();
			trockenanlage = strang->get_trockenanlage();
			durchmesser = strang->get_durchmesser();
			gekehlt = strang->get_muffenGekehlt();
			sondermuffenlaenge = strang->get_sonderMuffenLaenge();
			bemerkung = (LPCSTR)strang->get_bemerkung();
		}

		
		if(strang->get_fertigungshinweis().GetLength() > 1)
		{
			bFertigungshinweis = true;
		}

		
		
		gesamtlaenge += strang->get_laenge() * strang->get_anzahl();
		anzahlRohre += strang->get_anzahl();
		int i = 0, eleanz = 0;
		while (strang->get_muffenRelAbst(i) != 0)
		{
			eleanz++;
			i++;
			if (i==10) break;
		}
		eleanz *= strang->get_anzahl();
		anzahlElemente += eleanz; 
	}

	string tmpbemerkung;
	
	if(bFertigungshinweis)
	{
		tmpbemerkung += _T("<br>Achtung, Positionstexte sind vorhanden.<br>");
	}

	bemerkung += tmpbemerkung;

	create_header();

	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const Strangrohr* str = dynamic_cast<const Strangrohr*>(*it);
		if (!str)
			continue;
		if (!create_strangrohrliste(str))
			return false;
	}
	return true;
}

void DrucklisteStrangrohr::create_header()
{
	html.insert_bemerkung(bemerkung, getStammdaten().getText("IDS_BEMERKUNG").c_str());

	//evt. Sondermuffentext einfügen
	if (elementID == SONDERMUFFE)
		html.insertSondermuffentext(sondermuffenlaenge);

	WaxHtmlFile &w = html.get_file();
	CString txt, str;

	
	
	// Table Fertigungsablaufplan
	if (WaxSys::isAV())
	{
		const int rows[] = { 50, 100, 50, 20, 50, 20, 50, 50, 50, 100 };
		w.startTable();

		for (int i=0; i<3; i++)
		{
            w.startRow();
			str = getStammdaten().getText("IDS_NAME").c_str();
			txt = str + ":"; 
			w.appendCol(txt,	rows[0]);
			w.appendCol(".",	rows[1]);
			str = getStammdaten().getText("IDS_MASCHINE").c_str();
			txt = str + ":"; 
			w.appendCol(txt,	rows[2]);
			w.appendCol(".",	rows[3]);
			str = getStammdaten().getText("IDS_SCHICHT").c_str();
			txt = str + ":"; 
			w.appendCol(txt,	rows[4]);
			w.appendCol(".",	rows[5]);
			str = getStammdaten().getText("IDS_DATUM").c_str();
			txt = str + ":"; 
			w.appendCol(txt,	rows[6]);
			w.appendCol(".",	rows[7]);
			str = getStammdaten().getText("IDS_UNTERSCHRIFT").c_str();
			txt = str + ":"; 
			w.appendCol(txt,	rows[8]);
			w.appendCol(".",	rows[9]);
			w.endRow();
		}
		w.endTable();
	}
	
	// Table 1
	const int rows1[] = { 250, 65, 50, 90, 60, 60, 60, 100 };

	w.startTable();
	w.startRow();
	w.startCol(0);
	
	w.startTable(0,0);
	w.startRow();
	str = getStammdaten().getText("IDS_QUALITAET").c_str();
	w.appendCol(str,		rows1[0]);
	if (!dn2zoll)
	{
		str = getStammdaten().getText("IDS_NENNW_DN").c_str();
		w.appendCol(str,	rows1[1], "RIGHT");
	}
	else
	{
		str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
		w.appendCol(str,	rows1[1], "RIGHT");
	}
		str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	w.appendCol(str,		rows1[2], "RIGHT");
		str = getStammdaten().getText("IDS_GESAMTLAENGE_M").c_str();
	w.appendCol(str,		rows1[3], "RIGHT");
	w.endRow();
	
	w.startRow();

	if (rohrQuali == Rohrleitung::RQ2458GSCH_DICK)
	{
		Element ele;
		ele.set_id(rohrQuali+ROHR_Q1-1);
		ele.initStammdaten(false); // Braucht keinen Preis
		txt = ele.get_bezeichnung();
	}
	if (rohrQuali == Rohrleitung::RQ2440_2458GSCH)
		if (dnvl <= 50)
			txt = getStammdaten().getText("IDS_2440STD").c_str();
		else
			txt = getStammdaten().getText("IDS_2458STD").c_str();
	else
		txt.LoadString(WAXIDS_FIRST+30+rohrQuali);
	if (rohrQuali == Rohrleitung::RQ2440_2458GSCH)
		w.appendCol(txt, rows1[0]);
	else
		w.appendCol("<b>" + txt + "</b>", rows1[0]);
	if (!dn2zoll)
		w.appendCol(dnvl, rows1[1]);
	else
		w.appendCol(WaxStamm::dn2zoll(dnvl), rows1[1], "RIGHT");
	w.appendCol(anzahlRohre, rows1[2]);
	w.appendCol((double)gesamtlaenge/1000, rows1[3]);
	w.endRow(false);
	w.endTable(false);
		
	w.endCol();
	w.startCol(0);
	
	w.startTable(0,0);
	w.startRow();
	str = getStammdaten().getText("IDS_TYP").c_str();
	w.appendCol(str,		rows1[4]);
	if (elementID == BOHR24)
	{
		str = getStammdaten().getText("IDS_DURCHMESSER_MM").c_str();
		w.appendCol(str,	rows1[5], "RIGHT");
	}
	else if (!dn2zoll)
	{
		str = getStammdaten().getText("IDS_NENNW_DN").c_str();
		w.appendCol(str,	rows1[5], "RIGHT");
	}
	else
	{
		str = getStammdaten().getText("IDS_NENNW_ZOLL").c_str();
		w.appendCol(str,	rows1[5], "RIGHT");
	}
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	w.appendCol(str,		rows1[6], "RIGHT");
	str = getStammdaten().getText("IDS_TROCKENANLAGE").c_str();
	w.appendCol(str,		rows1[7]);
	w.endRow();
	
	w.startRow();
	if (elementID == BOHR24)
	{
		str = getStammdaten().getText("IDS_BOHRUNG").c_str();
		w.appendCol(str,	rows1[4]);
		w.appendCol(durchmesser, rows1[5]);
	}
	else 
	{
		if (elementID == MUFFE || elementID == FRMUFFE || elementID == KRMUFFE)
			txt = getStammdaten().getText("IDS_MUFFE").c_str();
		else if (elementID == SONDERMUFFE)
			txt = getStammdaten().getText("IDS_SONDERMUFFE").c_str();
		else txt = (CString)"Elementid nachpflegen in " + (CString)__FILE__;

		if (gekehlt) 
			txt = "<b>K-" + txt + "</b>";
		w.appendCol(txt,	rows1[4]);
		if (!dn2zoll)
			w.appendCol(elementDN, rows1[5]);
		else
			w.appendCol(WaxStamm::dn2zoll(elementDN), rows1[5], "RIGHT");
	}
	w.appendCol(anzahlElemente, rows1[6]);
	if (trockenanlage)
		txt.Format("X");
	else
		txt.Format("-");
	w.appendCol(txt, rows1[7], "CENTER");
	w.endRow(false);
	w.endTable(false);
	w.endCol();
	w.endRow();
	w.endTable();

	// Table 3 
	const int rows3[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};

	w.startTable(0,0);
	w.startRow();
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows3[0]);
	
	str = getStammdaten().getText("IDS_KAPPF").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows3[1]);
	
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows3[2]);
	
	str = getStammdaten().getText("IDS_ANFANG").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows3[3]);
	
	str = getStammdaten().getText("IDS_ENDE").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,			rows3[4]);
	
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows3[5]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "1/6 [mm]</b>";
	w.appendCol(txt,	rows3[6]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "2/7 [mm]</b>";
	w.appendCol(txt,	rows3[7]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "3/8 [mm]</b>";
	w.appendCol(txt,	rows3[8]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "4/9 [mm]</b>";
	w.appendCol(txt,	rows3[9]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "5/10 [mm]</b>";
	w.appendCol(txt,	rows3[10]);
	
	str = getStammdaten().getText("IDS_REST_MM").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows3[11]);
	
	str = getStammdaten().getText("IDS_AV").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,			rows3[12]);
	
	w.endRow();

}

//Druckliste fuer die alte Strangrohrstrasse
bool DrucklisteStrangrohr::create_strangrohrliste(const Strangrohr* const strang)
{
	/*
	Liste fuer Strangrohre.
	*/
	CString txt;
	const int rows3[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	WaxHtmlFile &w = html.get_file();
	w.startRow();
	w.appendCol(strang->get_posNr(), rows3[0]);
	w.appendCol(strang->getKappenfarbe(strang->get_rohrKFarbe()), rows3[1]);
	w.appendCol(strang->get_anzahl(), rows3[2]);


	CString startele;
	CString endele;

	bool isDiscStart=false;
	bool isDiscEnd=false;
	bool isDoubleDisc=false;
	int DiscSize=0;
	int rohrDn=strang->get_dnvl();

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


	startele=strang->get_abschluss_kurzbezeichnung(true).c_str();
	endele=strang->get_abschluss_kurzbezeichnung(false).c_str();

	if(startele=="Scheibe")
	{
		isDiscStart=true;
	}

	if(endele=="Scheibe")
	{
		isDiscEnd=true;
	}

	if(endele=="Scheibe" && startele=="Scheibe" )
	{
		isDoubleDisc=true;
		isDiscEnd=false;
		isDiscStart=false;
	}

	// Anfangelement
	w.appendCol(strang->get_abschluss_kurzbezeichnung(true).c_str(), rows3[3]);
	// Endelement
	if(((Strangrohr*)strang)->isRedBogEinz())
	{
		CString strGlatt(_T("Glatt"));
		w.appendCol(strGlatt, rows3[4]);
	}
	else
	{
		w.appendCol(strang->get_abschluss_kurzbezeichnung(false).c_str(), rows3[4]);
	}

	int bogenmass = 0; 
	for (int j=0; j<strang->get_avelementArray().GetSize(); j++)
	{
		// Hinter Bogen duerfen keine Standardelemente mehr sein
		Element *ele = strang->get_avelementArray().get_element_at(j);
		if (ele->isEleBogen())
		{
			bogenmass = ele->get_abstAbs()-ele->get_laenge();
			break;
		}	
	}
	//w.appendCol((bogenmass > 0) ? bogenmass : strang->getLaengeZuschnitt(), rows3[5]);

	if(isDiscEnd || isDiscStart || isDoubleDisc)
	{
		if(!isDoubleDisc)
		{
			w.appendCol(strang->getLaengeZuschnitt()-DiscSize, rows3[5]);
		}
		else
		{
			w.appendCol(strang->getLaengeZuschnitt()- (DiscSize*2), rows3[5]);
		}
	}
	else
	{
		w.appendCol(strang->getLaengeZuschnitt(), rows3[5]);
	}



	/*
	w.appendCol(strang->get_muffenRelAbst(0), rows3[6]);
	w.appendCol(strang->get_muffenRelAbst(1), rows3[7]);
	w.appendCol(strang->get_muffenRelAbst(2), rows3[8]);
	w.appendCol(strang->get_muffenRelAbst(3), rows3[9]);
	w.appendCol(strang->get_muffenRelAbst(4), rows3[10]);
	*/

	if(isDiscStart)
		w.appendCol(strang->getRelLaengeStdMuffe(0) -DiscSize , rows3[6]);
	else
	{
		w.appendCol(strang->getRelLaengeStdMuffe(0), rows3[6]);
	}

	w.appendCol(strang->getRelLaengeStdMuffe(1), rows3[7]);
	w.appendCol(strang->getRelLaengeStdMuffe(2), rows3[8]);
	w.appendCol(strang->getRelLaengeStdMuffe(3), rows3[9]);
	w.appendCol(strang->getRelLaengeStdMuffe(4), rows3[10]);
	
	
	
	if (strang->get_muffenRelAbst(5) != 0)
	{
		w.appendCol(" ", rows3[11]);
		if (strang->get_avnachbearbeitung())	
		{
			txt = "X";
		}
		else
		{
			txt = "-";
		}
		w.appendCol(txt, rows3[12], "CENTER");
	
		w.endRow();
		w.startRow();
		txt = "-";
		w.appendCol(txt, rows3[0]);
		w.appendCol(txt, rows3[1]);
		w.appendCol(txt, rows3[2]);
		w.appendCol(txt, rows3[3]);
		w.appendCol(txt, rows3[4]);
		w.appendCol(txt, rows3[5]);

		w.appendCol(strang->getRelLaengeStdMuffe(5),	rows3[6]);
		w.appendCol(strang->getRelLaengeStdMuffe(6),	rows3[7]);
		w.appendCol(strang->getRelLaengeStdMuffe(7),	rows3[8]);
		w.appendCol(strang->getRelLaengeStdMuffe(8),	rows3[9]);
		w.appendCol(strang->getRelLaengeStdMuffe(9),	rows3[10]);

		if(isDoubleDisc)
			w.appendCol(strang->getRelLaengeStdMuffe(99) - (DiscSize),rows3[11]);	//Restabstand
		else
		{
			w.appendCol(strang->getRelLaengeStdMuffe(99),rows3[11]);	//Restabstand
		}
		w.endRow();
	}
	else
	{
		//w.appendCol(strang->get_muffenRestabstand(), rows3[11]);
		if(isDoubleDisc  || isDiscEnd)
			w.appendCol(strang->getRelLaengeStdMuffe(99) - (DiscSize),rows3[11]);	//Restabstand
		else
		{
			w.appendCol(strang->getRelLaengeStdMuffe(99),rows3[11]);	//Restabstand
		}

		if (strang->get_avnachbearbeitung())	
			txt = "X";
		else					
			txt = "-";
		w.appendCol(txt, rows3[12], "CENTER");
		w.endRow();
	}
	return true;
}

