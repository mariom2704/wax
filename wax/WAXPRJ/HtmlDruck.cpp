
#include "stdafx.h"



#include "resource.h"



#include "GpBarcode.h"
#include "GpVk3Preis.h"
#include "drucklistenverwaltung.h"



	#include "Htmldruck.h"



HtmlDruck::HtmlDruck()
{}


HtmlDruck::HtmlDruck(CString fn)
:file(&fn, "")
{}



HtmlDruck::~HtmlDruck()
{
	file.closeFile();
}

// Fuegt eine Zeile mit den Fertigungszeiten ein 
void HtmlDruck::insertZeiten(double schweisserei, double pulveranlage)
{
    CString txt;
	
	get_file().startTable();
	get_file().startRow();
	txt = getStammdaten().getText("IDS_SCHWEISSEREI").c_str();
	get_file().appendCol(txt,		40, "center", true);
	txt.Format("%.1f h", schweisserei);
	get_file().appendCol("<b>" + txt + "</b>",	10, "center", true);
	txt = getStammdaten().getText("IDS_PULVERANLAGE").c_str();
	get_file().appendCol(txt,		40, "center", true);
	txt.Format("%.1f h", pulveranlage);
	get_file().appendCol("<b>" + txt + "</b>",  10, "center", true);
	get_file().endRow();
  	get_file().endTable();
}

/**
fügt eine Zeile mit 2 Spalten ein
*/
void HtmlDruck::insertZeile2Spalten(const CString &spalte1,int breiteSpalte1,bool center1, const CString &spalte2,int breiteSpalte2,bool center2)
{
	CString align;

	get_file().startTable();
	get_file().startRow();
	
	(center1 ? align = "center" : align = "left");
	get_file().appendCol(spalte1, breiteSpalte1,align);

	(center2 ? align = "center" : align = "left");
	get_file().appendCol("<b>" + spalte2 + "</b>", breiteSpalte2, align);

	get_file().endRow();
  	get_file().endTable();
}


/**
fügt eine Zeile mit 4 Spalten ein
*/
void HtmlDruck::insertZeile4Spalten(const CString &spalte1,int breiteSpalte1,bool center1, 
									const CString &spalte2,int breiteSpalte2,bool center2,
									const CString &spalte3,int breiteSpalte3,bool center3,
									const CString &spalte4,int breiteSpalte4,bool center4)
{
	CString align;

	get_file().startTable();
	get_file().startRow();
	
	(center1 ? align = "center" : align = "left");
	get_file().appendCol(spalte1, breiteSpalte1,align);

	(center2 ? align = "center" : align = "left");
	get_file().appendCol("<b>" + spalte2 + "</b>", breiteSpalte2, align);

	(center3 ? align = "center" : align = "left");
	get_file().appendCol(spalte3, breiteSpalte3, align);

	(center4 ? align = "center" : align = "left");
	get_file().appendCol("<b>" + spalte4 + "</b>", breiteSpalte4, align);

	get_file().endRow();
  	get_file().endTable();
}




//Fügt eine Zeile mit der Beschichtung, dem Sichtbereichsflag, dem Zeugnisflag 
//und der Zulassungsstelle ein
void HtmlDruck::insertOberflaeche(const Beschichtung &beschichtung, BOOL zeugnis, BOOL sichtbereich, int iKolliliste)
{
    /*
	Erzeugen des Kopfes für Druckausgaben
	Der Kopf besteht aus den Daten der Leitkarte und der Ausgabe der Beschichtung. Die Leitkarte wird
	in <mf .insertLeitkarte> erzeugt. 
	*/
	CString sichtbar, zeugn, strKolliliste, str;
	
	sichtbar.Format("%s", sichtbereich ? "X" : "-");
	zeugn.Format("%s", zeugnis ? "X" : "-");
	if (iKolliliste == 0)
		strKolliliste = "-";
	else
		strKolliliste = "X";
	

	CString oberfl = beschichtung.get_bezeichnung();

	int ral = beschichtung.get_ral();

	if (oberfl.Find("RAL", 0) == -1 && ral >= 1000)
	{
		oberfl.Format("%s   RAL %i", beschichtung.get_bezeichnung(), ral);
	}
	
	get_file().startTable();
	get_file().startRow();
	
	str = getStammdaten().getText("IDS_KOLLILISTE_EPORT").c_str();
	get_file().appendCol(str, 150);
	get_file().appendCol("<b>" + strKolliliste + "</b>", 100, "center");

	str = getStammdaten().getText("IDS_IM_SICHTBEREICH").c_str();
	get_file().appendCol(str, 150);
	get_file().appendCol("<b>" + sichtbar + "</b>", 50, "center");

	str = getStammdaten().getText("IDS_ZEUGNIS").c_str();
	get_file().appendCol(str, 100);
	get_file().appendCol("<b>" + zeugn + "</b>", 50, "center");

	str = getStammdaten().getText("IDS_OBERFLAECHE").c_str();
	get_file().appendCol(str, 100);
	get_file().appendCol("<b>" + oberfl + "</b>", 400, "center");
	get_file().endRow();
  	get_file().endTable();
}

void HtmlDruck::insertKopf(const CString &ueberschrift1, const CString &ueberschrift2,
							const CString &wanr, const CString &sachbearbeiterOd,const CString &besteller,
							const CString & bearbeitungsDat, const CString & freigabeDat, const CString & lieferDat,
							const CString &nl, const CString &auftragsBez, const CString &bauabschnitt, const CString &plz, const CString &ort,
							const CString &sdAuftrag, const CString &sdPos, const CString &sdMaterial, const CString &psp,
							bool umbruch,const CString &nichtGesetzt, const CString &barcode)
{
	CString stWert,
		    out,
			head,
			str;


	//Kopfstring zusammenbasteln
	head = 

	// Beginn Tabelle  
	"<P ALIGN='CENTER'><CENTER><TABLE BORDER=1 CELLSPACING=0 CELLPADDING=3 WIDTH=100%>\n"
		// 1. Zeile
		"<TR>"
			// 1. Spalte
			"<TD WIDTH='30%' VALIGN='MIDDLE' COLSPAN=1>\n"
				"<P ALIGN='CENTER'><IMG SRC='Minimax.jpg' WIDTH=155 HEIGHT=23></TD>\n"
			
			// 2. Spalte
			"<TD WIDTH='50%' VALIGN='MIDDLE' COLSPAN=2>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>" 
				+ ueberschrift1
				+ "</B></FONT></TD>\n"
			
			// 3. Spalte
			"<TD WIDTH='20%' VALIGN='MIDDLE' COLSPAN=1>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>" 
				+ ueberschrift2							// z.B. VERSAND
				+ "</B></FONT></TD></TR>\n";
		head +=	
		
		// 2. Zeile 
		"<TR>"
			// 1. Spalte
			"<TD WIDTH='30%' VALIGN='TOP' COLSPAN=1>\n";

	str = getStammdaten().getText("IDS_WANR").c_str();
	head += "<FONT FACE='Arial' SIZE=2>" + str + "</FONT><B><FONT FACE='Arial'>" + wanr;

	//SachbearbeiterOd
	(sachbearbeiterOd.IsEmpty() ? stWert = nichtGesetzt : stWert = sachbearbeiterOd);
	str = getStammdaten().getText("IDS_SACHBEARBEITER").c_str();
	out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
    head += out + stWert;
			
	//Besteller
	(besteller.IsEmpty() ? stWert = nichtGesetzt : stWert = besteller);
	str = getStammdaten().getText("IDS_BESTELLER").c_str();
	out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
	head += out + stWert + "</B></FONT></TD>\n";

	// 2. Spalte
	head +=	"<TD WIDTH='30%' VALIGN='TOP' COLSPAN=1>\n";

	//Bearbeitungsdatum
	(bearbeitungsDat.IsEmpty() ? stWert = nichtGesetzt : stWert = bearbeitungsDat);
	str = getStammdaten().getText("IDS_BEARBEITUNGSDATUM").c_str();
	out.Format("<FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
	head += out	+ stWert;

	//Freigabedatum
	(freigabeDat.IsEmpty() ? stWert = nichtGesetzt : stWert = freigabeDat);
	str = getStammdaten().getText("IDS_FREIGABE_DATUM").c_str();
	out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
	head += out + stWert + "</B></FONT></TD>\n";

	// 3. Spalte
	head +=	"<TD WIDTH='40%' VALIGN='TOP' COLSPAN=2>\n";

	//Bau
	(auftragsBez.IsEmpty() ? stWert = nichtGesetzt : stWert = auftragsBez);
	str = getStammdaten().getText("IDS_BAU").c_str();
	out.Format("<FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
	head += out + stWert;
			
	if (!auftragsBez.IsEmpty() && !bauabschnitt.IsEmpty())
		head += " - " + bauabschnitt;
		
	//Plz
	(plz.IsEmpty() ? stWert = nichtGesetzt : stWert = plz);
	str = getStammdaten().getText("IDS_PLZ").c_str();
	out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str); 
	head += out + stWert;
			

	//Ort
	(ort.IsEmpty() ? stWert = nichtGesetzt : stWert = ort);
	str = getStammdaten().getText("IDS_ORT").c_str();
	out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str); 
	head += out + stWert;
			
	head += "</B></FONT></TD>\n</TR>\n";

	// 3. Zeile 1.Spalte
	head += "<TR> <TD WIDTH='30%' VALIGN='TOP' COLSPAN=1>\n";

	//Lieferdatum
	(lieferDat.IsEmpty() ? stWert = nichtGesetzt : stWert = lieferDat);
	str = getStammdaten().getText("IDS_LIEFERUNG_AM").c_str();
	out.Format("<FONT FACE='Arial' SIZE=2><P ALIGN='CENTER'>%s<br></FONT><B><FONT FACE='Arial'>", str);
	head += out + stWert + + "<br>\n" + barcode + "</B></FONT></TD>\n";
		
	// 2. Spalte
	head += "<TD WIDTH='10%' VALIGN='TOP' COLSPAN=1>\n";

	//Niederlassung
	(nl.IsEmpty() ? stWert = nichtGesetzt : stWert = nl);
	str = getStammdaten().getText("IDS_NL_KURZ").c_str();
	out.Format("<FONT FACE='Arial' SIZE=2><P ALIGN='CENTER'>%s<br></FONT><B><FONT FACE='Arial'>", str);
	head += out + stWert + "</B></FONT></TD>\n";

	//3. Spalte
	head +=	"<TD WIDTH='15%' VALIGN='TOP' COLSPAN=1>\n <FONT FACE='Arial' SIZE=2>";

	str = getStammdaten().getText("IDS_SD_AUFTRAG").c_str();
	out.Format("%s:&#9;<br>\n", str);
	head += out;

	str = getStammdaten().getText("IDS_SD_MATNR").c_str();
	out.Format("%s:&#9;<br>\n", str);
	head += out;

	str = getStammdaten().getText("IDS_PSP_ELE").c_str();
	out.Format("%s:&#9;<br>\n", str);
	head += out;

	str = getStammdaten().getText("IDS_KC_AUFTRAG").c_str();
	out.Format("%s:&#9;", str);
	head += out	 + "</FONT></TD>\n";

	// 4. Spalte
	head +=	"<TD WIDTH='55%' VALIGN='TOP' COLSPAN=1>\n <B><FONT FACE='Arial' SIZE=2>";

	//Sd Auftrag
	(sdAuftrag.IsEmpty() ? stWert = nichtGesetzt : stWert = sdAuftrag);
	head +=	stWert  + "<br>\n"; 

	//Sd Pos
	(sdPos.IsEmpty() ? stWert = nichtGesetzt : stWert = sdPos);
	head +=	stWert + "<br>\n" ;

	//Sd Material
	(sdMaterial.IsEmpty() ? stWert = nichtGesetzt : stWert = sdMaterial);
	head +=	stWert  + "<br>\n" ;

	//Psp
	(psp.IsEmpty() ? stWert = nichtGesetzt : stWert = psp);
	head += stWert;

	//Ende Table
	head += "</B></FONT></TD></TR></TABLE>\n";
	
	if (umbruch)
		get_file().insertSeitenumbruch();

	get_file().WriteString(head);
}

//Fuegt die Leitkarte in eine Druckliste ein
void HtmlDruck::insertLeitkarte(Leitkarte& leitkarte, Status& status, const CString& name, const CString & ziel, bool umbruch)
{
		/*
	Erzeugt den oberen Teil des Kopfes der Druckausgabe
	Die Daten der Leitkarte werden als Standardkopf mit Logo ausgegeben
	*/
	CString wanrstr, ippeMatNrstr, sdauftragsnrstr, sdmaterialnrstr, sdpositionsnrstr, prjnl, str, out, kcauftragsnrstr, kcpositionsnrstr;
	CString freigabedatum;
	// Datumstrings
	if (status.get_freigabedatum().m_dt > 0.0)
		freigabedatum = status.get_freigabedatum().Format(_T("%d.%m.%Y %H:%M"));


	MXDate beatmp(status.get_bearbeitungsdatum());
	CString bearbeitungsdat;
	TRY
	{
		bearbeitungsdat = beatmp.Format(VAR_DATEVALUEONLY);
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL

	wanrstr.Format("____%06d", status.get_wanr());
	if(leitkarte.get_IsChoosenIppeNumbers())
	{
		ippeMatNrstr = leitkarte.get_ippeMatNr();
	}
	else
	{
		sdauftragsnrstr.Format("%I64d", leitkarte.get_sdAuftragsnr());
	}
	sdmaterialnrstr.Format("%d", leitkarte.get_sdMaterialnummer());
	sdpositionsnrstr.Format("%d", leitkarte.get_sdPositionsnr());
	prjnl = getStammdaten().getNLBezeichnung(leitkarte.get_prj_nl());
	
	__int64 tmp = _atoi64(leitkarte.get_kcAuftragsnr());
	if (tmp > 0)
	{
		kcauftragsnrstr = leitkarte.get_kcAuftragsnr();
		kcpositionsnrstr.Format("%d", leitkarte.get_kcPositionsnr());
	}

	CString panr;
	
	if (leitkarte.get_preisanfrage().get_nummer() > 0)
	{
		panr.Format("PA %i", leitkarte.get_preisanfrage().get_nummer());
		if (leitkarte.get_preisanfrage().get_position() > 0)
		{
			str.Format(" - %i", leitkarte.get_preisanfrage().get_position());
			panr += str;
		}
	}

	
	CString head = 

	// Beginn Tabelle  
	"<P ALIGN='CENTER'><CENTER><TABLE BORDER=1 CELLSPACING=0 CELLPADDING=3 WIDTH=100%>\n"
		// 1. Zeile
		"<TR>"
			// 1. Spalte
			"<TD WIDTH='30%' VALIGN='MIDDLE' COLSPAN=1>\n"
				"<P ALIGN='CENTER'><IMG SRC='Minimax.jpg' WIDTH=155 HEIGHT=23></TD>\n"
			
			// 2. Spalte
			"<TD WIDTH='50%' VALIGN='MIDDLE' COLSPAN=2>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'>" 
				+ name
				+ "</B></FONT></TD>\n"
			
			// 3. Spalte
			"<TD WIDTH='20%' VALIGN='MIDDLE' COLSPAN=1>\n<B><FONT FACE='Arial' SIZE=4><P ALIGN='CENTER'><DIV id='ele3' ALIGN='CENTER'>" 
				+ ziel							// z.B. VERSAND
				+ "</DIV></B></FONT></TD></TR>\n";
		head +=	
		
		// 2. Zeile 
		"<TR>"
			// 1. Spalte
			"<TD WIDTH='30%' VALIGN='TOP' COLSPAN=1>\n";

		str = getStammdaten().getText("IDS_WANR").c_str();
		head += "<FONT FACE='Arial' SIZE=2>" + str + "</FONT><B><FONT FACE='Arial'>" + wanrstr;

		str = getStammdaten().getText("IDS_SACHBEARBEITER").c_str();
		out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
        head += out 
				+ status.get_sachbearbeiterSegment();

		str = getStammdaten().getText("IDS_BESTELLER").c_str();
		out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
		head += out
				+ leitkarte.get_sachbearbeiterNL() 
				+ "</B></FONT></TD>\n";
		head +=	
			// 2. Spalte
			"<TD WIDTH='30%' VALIGN='TOP' COLSPAN=1>\n";

		str = getStammdaten().getText("IDS_BEARBEITUNGSDATUM").c_str();
		out.Format("<FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
		head += out
				+ bearbeitungsdat;

		str = getStammdaten().getText("IDS_FREIGABE_DATUM").c_str();
		out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
		head += out
				+ freigabedatum
				+ "</B></FONT></TD>\n";
		head +=	
			// 3. Spalte
			"<TD WIDTH='40%' VALIGN='TOP' COLSPAN=2>\n";

		str = getStammdaten().getText("IDS_BAU").c_str();
		out.Format("<FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str);
		head += out
				+ leitkarte.get_prj_auftragsbez() 
				+ " - "
				+ leitkarte.get_prj_bauAbschnitt(); 
			
		str = getStammdaten().getText("IDS_PLZ").c_str();
		out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str); 
		head += out
				+ leitkarte.get_prj_bauPlz();

		str = getStammdaten().getText("IDS_ORT").c_str();
		out.Format("<br>\n</B></FONT><FONT FACE='Arial' SIZE=2>%s:&#9;</FONT><B><FONT FACE='Arial'>", str); 
		head += out
				+ leitkarte.get_prj_bauOrt()
				+ "</B></FONT></TD>\n</TR>\n";
		head += 
		// 3. Zeile
		"<TR>"
			// 1. Spalte
			"<TD WIDTH='30%' VALIGN='TOP' COLSPAN=1>\n";

		str = getStammdaten().getText("IDS_LIEFERUNG_AM").c_str();
		out.Format("<FONT FACE='Arial' SIZE=2><P ALIGN='CENTER'>%s<br></FONT><B><FONT FACE='Arial'>", str);
		str.Format("<br><br>%s", panr);
		if (WaxSys::isAV())
		{
			head += out
					+ leitkarte.get_wunschliefertermin().Format(VAR_DATEVALUEONLY) 
					+ str
					+ "</B></FONT></TD>\n"
				// 2. Spalte
				"<TD WIDTH='10%' VALIGN='TOP' COLSPAN=1>\n";
		}
		else
		{
			head += out
					+  getStammdaten().getText("IDS_LIEFER_UEBER_SAP").c_str()
					+ str
					+ "</B></FONT></TD>\n"
				// 2. Spalte
				"<TD WIDTH='10%' VALIGN='TOP' COLSPAN=1>\n";
		}

		str = getStammdaten().getText("IDS_NL_KURZ").c_str();
		out.Format("<FONT FACE='Arial' SIZE=2><P ALIGN='CENTER'>%s<br></FONT><B><FONT FACE='Arial'>", str);
		head += out
				+ prjnl 
				+ "</B></FONT></TD>\n";
		head +=
			// 3. Spalte
			"<TD WIDTH='15%' VALIGN='TOP' COLSPAN=1>\n"
				"<FONT FACE='Arial' SIZE=2>";

		str = getStammdaten().getText("IDS_SD_AUFTRAG").c_str();
		out.Format("%s:&#9;<br>\n", str);
		head += out;

		str = getStammdaten().getText("IDS_SD_MATNR").c_str();
		out.Format("%s:&#9;<br>\n", str);
		head += out;

		str = getStammdaten().getText("IDS_PSP_ELE").c_str();
		out.Format("%s:&#9;<br>\n", str);
		head += out;

		str = getStammdaten().getText("IDS_KC_AUFTRAG").c_str();
		out.Format("%s:&#9;", str);
		head += out
			 + "</FONT></TD>\n"
			// 4. Spalte
			"<TD WIDTH='55%' VALIGN='TOP' COLSPAN=1>\n"
				"<B><FONT FACE='Arial' SIZE=2>";
		if(leitkarte.get_IsChoosenIppeNumbers())
		{
			
			head +=	ippeMatNrstr
				+ " - " + sdpositionsnrstr + "<br>\n"  
				+ sdmaterialnrstr  + "<br>\n";
				
		}
		else
		{
			
			head += sdauftragsnrstr  + " - " + sdpositionsnrstr + "<br>\n"  
				+ sdmaterialnrstr  + "<br>\n";
			
		}
		if (leitkarte.get_prj_COauftrag().GetLength() > 1)
		{
			str = getStammdaten().getText("IDS_CO").c_str();
			out.Format("%s: ", str);
			head += out + leitkarte.get_prj_COauftrag() + "<br>\n";
		}
		else
			head += leitkarte.getPSPElement() + "<br>\n";

		if (!kcauftragsnrstr.IsEmpty())
			head += kcauftragsnrstr  + " - " + kcpositionsnrstr;

		head += "</B></FONT></TD></TR></TABLE>\n";
	
	if (umbruch)
		get_file().insertSeitenumbruch();
	get_file().WriteString(head);
}

//Fuegt die Bemerkung aus der Leitkarte in eine Druckliste
void HtmlDruck::insert_bemerkung(const std::string& bem, const std::string& ueberschrift, const std::string& stdbemerkung)
{
	/**
	Erzeugen des Tables mit der Bemerkung
	Bemerkungsfeld wird nur eingefügt, wenn die Länge > 2 ist, da default in die Datenbank ein '-'
	geschrieben wird.
	*/
	string txt = bem;

	if (bem.length() && stdbemerkung.length())	
		txt += "\r\n" + stdbemerkung;
	else if (stdbemerkung.length())	
		txt = stdbemerkung;
	
	if (txt.length() < 2)
		return;

	file.startTable(0, 0);
	
	// Erste Zeile: Überschrift 'Bemerkung'
	file.startRow();
	file.appendCol(ueberschrift, 6000);
	file.endRow();

	// Zweite Zeile mit Bemerkungstext
	file.startRow();
	// Zeilenumbrüche nach html-Zeilenumbrüche wandeln
	file.appendCol("<b>" + boost::algorithm::replace_all_copy(txt, "\r\n", " <br/>") + "</b>", 6000);
	file.endRow();

  	file.endTable();
}




void HtmlDruck::insertListeUnterschriften()
{
	if (WaxSys::isAV())
	{
		const int rows[] = { 150, 30, 150, 30, 150, 30 };
		
		file.startTable();

			file.startRow();
			CString out;
			out = _T("<td align=center style=\"border:0px\" width=50px><font face=\"Arial\"SIZE=2><b>Bezeichnung:</TD> \r\n");
			file.WriteString(out);
			out = _T("<td align=center style=\"border:0px\" width=30%><font face=\"Arial\"SIZE=2><b>Unterschrift:</TD> \r\n");
			file.WriteString(out);
			
			out = _T("<td align=center style=\"border:0px\" width=50px><font face=\"Arial\"SIZE=2><b>Bezeichnung:</TD> \r\n");
			file.WriteString(out);
			out = _T("<td align=center style=\"border:0px\" width=30%><font face=\"Arial\"SIZE=2><b>Unterschrift:</TD> \r\n");
			file.WriteString(out);

			out = _T("<td align=center style=\"border:0px\" width=50px><font face=\"Arial\"SIZE=2><b>Bezeichnung:</TD> \r\n");
			file.WriteString(out);
			out = _T("<td align=center style=\"border:0px\" width=30%><font face=\"Arial\"SIZE=2><b>Unterschrift:</TD> \r\n");
			file.WriteString(out);
			file.endRow();

            file.startRow();
			CString str = _T("Sägen");
			CString txt = str + ":"; 
			file.appendCol(txt,	rows[0]);
			file.appendCol(".",	rows[1], _T("'CENTER'"), true);
			str = _T("Biegen");
			txt = str + ":"; 
			file.appendCol(txt,	rows[2]);
			file.appendCol(".",	rows[3], _T("'CENTER'"), true);
			str = _T("Kanten");
			txt = str + ":"; 
			file.appendCol(txt, rows[4]);
			file.appendCol(".",	rows[5], _T("'CENTER'"), true);
			file.endRow();
			
            file.startRow();
			str = _T("Nuten");
			txt = str + ":"; 
			file.appendCol(txt,	0);
			file.appendCol(".",	0);
			str = _T("Klinken");
			txt = str + ":"; 
			file.appendCol(txt,	0);
			file.appendCol(".",	0);
			str =  _T("Lochen");
			txt = str + ":"; 
			file.appendCol(txt,	0);
			file.appendCol(".",	0);
			file.endRow();

			
            file.startRow();
			str = _T("Gewindesch.");;
			txt = str + ":"; 
			file.appendCol(txt,	0);
			file.appendCol(".",	0);
			str = _T(".");
			txt = str; 
			file.appendCol(txt,	0);
			file.appendCol(".",	0);
			str = _T("Schweissen");;
			txt = str + "."; 
			file.appendCol(txt,	0);
			file.appendCol(".",	0);
			file.endRow();
		
		file.endTable();
	}
}



//Text bei Sondermuffen andrucken
void HtmlDruck::insertSondermuffentext(int laenge)
{
	if (laenge <= 0)
		return;


	file.startTable();
	

	CString text1,text2,str;
	text1 = getStammdaten().getText("IDS_SONDERMUFFEN_TEXT1_AUSDRUCK").c_str();
	text2 = getStammdaten().getText("IDS_SONDERMUFFEN_TEXT2_AUSDRUCK").c_str();

	str.Format(text2,laenge);

	file.startRow();
	file.appendCol(text1,125);
	file.appendCol("<b>" + str + "</b>",400);
	file.endRow();

  	file.endTable();
}




//Fuegt die Kappenfarbe in eine Druckliste
void HtmlDruck::insertKFarbe(int kFarbe)
{
	if (!kFarbe)
		return;
	/*
	Erzeugen des Tables mit der Kappenfarbe
	*/
	Rohrleitung rohr;
	CString str, txt;

	file.startTable(0, 0);
	file.startRow();
	str = getStammdaten().getText("IDS_KAPPENFARBE").c_str();
	txt.Format("%s:", str);
	file.appendCol(txt, 50);
	CString kf(rohr.getKappenfarbe(kFarbe));
	kf.MakeUpper();
	file.appendCol("<b><font size = 3 color=\"black\">" + kf + "</font></b>", 1000);

	file.endRow();
  	file.endTable();
}

///Fuegt die Einzelpreise der Positionen ein 
void HtmlDruck::insertEinzelpreise(CStringArray& preise)
{
	CString str; str = getStammdaten().getText("IDS_SUMME_DER_EINZELPREISE").c_str();
	file.appendString(str); 
	
	for (int i=0; i<preise.GetSize(); i++)
	{
		file.appendString(preise.GetAt(i));
	}
}


//Fuegt die Rohrqualitaet in eine Druckliste ein
void HtmlDruck::insertQuali(int quali, bool twoquali, int dn, int stutzenAnlage2440, int anlagentyp )
{
	/*
	Erzeugen des Tables mit der Rohrqualitaet
	*/
	CString qual, str;

	if (stutzenAnlage2440 > 0)
	{
		if (stutzenAnlage2440 == 1)
			qual = getStammdaten().getText("IDS_RQ2440GSCH").c_str();
		else
			qual = getStammdaten().getText("IDS_QUALI_VARIABEL").c_str();
	}
	else if (quali == Rohrleitung::RQ2440VZIN && twoquali)
		qual = getStammdaten().getText("IDS_2440STDVZIN").c_str();
	else if (quali == Rohrleitung::RQEN10217VERZ && twoquali)
		qual = getStammdaten().getText("IDS_RQEN10217VERZ").c_str();
	else if (quali == Rohrleitung::RQ2440_2458GSCH && !twoquali)
	{
		if (dn <= 50)
			qual = getStammdaten().getText("IDS_2440STD").c_str();
		else
			qual = getStammdaten().getText("IDS_2458STD").c_str();
	}
	else
	{
		/**
		QA: Es soll nur die Bezeichnung der Rohrqualität eingefügt werden! Hier bitte
		etwas straigther programmieren.
		*/
		Element ele;
		ele.set_id(quali+ROHR_Q1-1);
		ele.initStammdaten(false); // Braucht keinen Preis
		qual = ele.get_bezeichnung();
	}
		
	get_file().startTable();
	get_file().startRow();
	str = getStammdaten().getText("IDS_DIN").c_str();
	get_file().appendCol(str, 125);

	if (quali != Rohrleitung::RQ2440_2458GSCH)
		qual = get_file().bold_string((LPCSTR)qual).c_str();

	get_file().appendCol(qual, 300);
	
	CString tmp;
	if (anlagentyp == Gewinderohr::WASSER)			tmp = getStammdaten().getText("IDS_WASSER").c_str();
	else
	{
		if (anlagentyp == Gewinderohr::SCHAUM)			tmp = getStammdaten().getText("IDS_SCHAUM").c_str();
		else if (anlagentyp == Gewinderohr::GAS)		tmp = getStammdaten().getText("IDS_GAS").c_str();
	}
	get_file().appendCol(tmp, 100, "CENTER");
	get_file().endRow();
  	get_file().endTable();
}




//Fuegt die Max. Laenge und das Gesamtgewicht in eine Druckliste ein
void HtmlDruck::insertLaengeGewichtBarcode(int maxlaenge, double gewicht, CString kc, int kcPosnr, string ids )
{
	/*
	Erzeugen des abschliessenden Tables mit der groessten Laenge und des Gesamtgewichtes
	*/

	CString laenge, 
		    sgewicht, 
			strLaenge,
			strGewicht;


	laenge.Format("%d", maxlaenge);
	
	get_file().startTable();
	get_file().startRow();


	if (gewicht > 0)
	{
		sgewicht.Format("%.0f",gewicht);
		strGewicht = getStammdaten().getText(ids).c_str();
	}
	else
	{
		strGewicht = " ";
		sgewicht = " ";
	}

	get_file().appendCol(strGewicht, 300);
	get_file().appendCol("<b>" + sgewicht + "</b>", 200, "center");

	strLaenge = getStammdaten().getText("IDS_MAX_LAENGE").c_str();
	get_file().appendCol(strLaenge, 300);
	get_file().appendCol("<b>" + laenge + "</b>", 200, "center");


	
	if (WaxSys::isAV() && !kc.IsEmpty())
	{
		MxString kcstr = MxString("{0}-{1}").arg(kc.GetString()).arg(kcPosnr);
		get_file().appendCol(get_file().createBarcode(210, 20, kcstr.getString(), "CENTER"), 400, "CENTER");
	}
	

	get_file().endRow();
  	get_file().endTable();

}


//Fuegt eine Trennlinie in eine Druckliste ein
void HtmlDruck::insertTrennlinie()
{
	file.WriteString("<hr width=\"100%%\" size=\"2\" align=\"center\">");
}



//Fuegt die Sokos in eine Druckliste ein
void HtmlDruck::insertSokoPreise(CObList &sokolist, double gesamtpreis)
{
	// Sokoliste
	CString str; str = getStammdaten().getText("IDS_SONDERKOSTEN").c_str();
	CString txt; txt.Format("<p><b>%s:</b></p>", str);
	get_file().WriteString(txt);

	const int rows[] = { 10, 75, 15};

	get_file().startTable();
	get_file().startRow();
	str = getStammdaten().getText("IDS_NR").c_str();
	get_file().appendCol(str,	rows[0], "center", true);
	str = getStammdaten().getText("IDS_TEXT").c_str();
	get_file().appendCol(str,	rows[1], "center", true);
	str = getStammdaten().getText("IDS_PREIS_E").c_str();
	get_file().appendCol(str,	rows[2], "center", true);
	get_file().endRow();
	
	for (POSITION pos = sokolist.GetHeadPosition(); pos; )
	{
		Preis::Sonderkosten *soko = (Preis::Sonderkosten*)(sokolist.GetNext(pos));
	
		get_file().startRow();
		get_file().appendCol(soko->get_nr(),	rows[0], "left", true);
		get_file().appendCol(soko->get_text(),	rows[1], "left", true);
		get_file().appendCol(soko->get_preis(),	rows[2], "right", true);
		get_file().endRow();
	}

	get_file().startRow();

	str = getStammdaten().getText("IDS_GESAMT").c_str();
	txt.Format("<b>%s</b>", str);
	get_file().appendCol(txt,	rows[0], "left", true);
	get_file().appendCol("",	rows[1], "left", true);
	txt.Format("<b>%.2f</b>", gesamtpreis);
	get_file().appendCol(txt,	rows[2], "right", true);
	get_file().endRow();
	get_file().endTable();
}



//Zugriff auf die Basisfunktionen der Html-Ausgabe
WaxHtmlFile& HtmlDruck::get_file()  
{
	return file;
}

void HtmlDruck::insertVK3GesPreise(double elementepreis, double sonderkosten, double gesamtpreisVK3)
{
	// Gesamtpreise
	CString str; str = getStammdaten().getText("IDS_GESAMTPREISE").c_str();
	CString txt; txt.Format("<p><b>%s:</b></p>", str);
	get_file().WriteString(txt);

	const int rows[] = {85, 15};
	
	get_file().startTable();
	get_file().startRow();
	str = getStammdaten().getText("IDS_KOSTEN").c_str();
	txt.Format("%s:", str);
	get_file().appendCol(txt,			rows[0], "center", true);
	str = getStammdaten().getText("IDS_SUMMEN_EUR").c_str();
	txt.Format("%s:", str);
	get_file().appendCol(txt,			rows[1], "center", true);
	get_file().endRow();

	get_file().startRow();
	str = getStammdaten().getText("IDS_VK3_ELEMENT").c_str();
	txt.Format("%s:", str);
	get_file().appendCol(txt,			rows[0], "left", true);
	get_file().appendCol(elementepreis,	rows[1], "right", true);
	get_file().endRow();

	get_file().startRow();
	str = getStammdaten().getText("IDS_SONDERKOSTEN").c_str();
	txt.Format("%s:", str);
	get_file().appendCol(txt,			rows[0], "left", true);
	get_file().appendCol(sonderkosten,	rows[1], "right", true);
	get_file().endRow();

	get_file().startRow();
	str = getStammdaten().getText("IDS_GESAMTPREIS").c_str();
	txt.Format("<b>%s:</b>", str);
	get_file().appendCol(txt,			rows[0], "left", true);
	txt.Format("<b>%.2f</b>", gesamtpreisVK3);
	get_file().appendCol(txt,			rows[1], "right", true);
	get_file().endRow();

	get_file().endTable();
}

void HtmlDruck::insertVK3Preise(EnCObListVk3& vk3Liste, EnCObListVk3& passlist, EnCObListVk3& pulverlist, double elementepreis)
{
	// VK3 Elementliste
	EnCObListVk3 vk3ListeLocal,			//Speicher dieser Liste nicht wieder freigeben!
				 pulverListe,		//Speicher dieser Liste nicht freigeben
				 tempVk3ListeLocal;		//Speicher freigeben
				
	POSITION pos;
	vk3ListeLocal.SetRohrType(vk3Liste.GetRohrType()); 
	for (pos = vk3Liste.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(vk3Liste.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;
		vk3ListeLocal.AddTail(ele);
	}
	for (pos = passlist.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(passlist.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;
		vk3ListeLocal.AddTail(ele);
	}

	
	//Liste kopieren, damit sie kumuliert werden kann
	tempVk3ListeLocal.copy(vk3ListeLocal);



	//Pulverliste kopieren, damit sie kumuliert werden kann
	pulverListe.copy(pulverlist);

	pulverListe.kumulieren();
	//pulverListe.rundenVollMeter();//Ab Version3.2 kein Runden mehr

	//Pulverliste an tempListe anhängen
	for (pos = pulverListe.GetHeadPosition(); pos;)
	{
		Element* ele = dynamic_cast<Element*>(pulverListe.GetNext(pos));
		XASSERT(ele); 
		if (!ele) 
			break;
		tempVk3ListeLocal.AddTail(ele);
	}


	//Liste kumulieren
	tempVk3ListeLocal.kumulieren();




	CString str; str = getStammdaten().getText("IDS_VK3_ELEMENTE").c_str();
	CString txt; txt.Format("<p><b>%s:</b></p>", str);;
	get_file().WriteString(txt);

	const int rows[] = {50, 50, 500, 50, 50, 50};
	
	get_file().startTable();
	get_file().startRow();

	str = getStammdaten().getText("IDS_VKDREI_PREIS_COUNTER").c_str();
	get_file().appendCol(str,		rows[0], "center");
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	get_file().appendCol(str,		rows[1], "center");
	str = getStammdaten().getText("IDS_BEZEICHNUNG").c_str();
	get_file().appendCol(str,		rows[2], "center");
	str = getStammdaten().getText("IDS_NENNW_DN").c_str();
	get_file().appendCol(str,		rows[3], "center");
	str = getStammdaten().getText("IDS_ROHRNENNW_DN").c_str();
	get_file().appendCol(str,		rows[4], "center");
	str = getStammdaten().getText("IDS_PREIS_E").c_str();
	get_file().appendCol(str,		rows[5], "center");
	get_file().endRow();

	for (pos = tempVk3ListeLocal.GetHeadPosition(); pos != NULL;) 
	{
		Element *ele = dynamic_cast<Element*>(tempVk3ListeLocal.GetNext(pos));

		XASSERT(ele);
		if (!ele)
		{
			str = getStammdaten().getText("IDS_FEHLER_VK3LIST").c_str();
			get_file().appendCol(str, rows[0]);	
			return;
		}

		get_file().startRow();
	
		get_file().appendCol(ele->get_idCounterVk3Preis(),					rows[0], "center");

		bool rohr = false;
		
		if (ele->get_id() >= 130 && ele->get_id() < 200)
		{
			rohr = true;
		}
		else if (ele->get_id() > 200 && ele->get_id() < 300)
		{
			rohr = true;
		}
		else if (ele->get_id() > 300 && ele->get_id() < 400)
		{
			rohr = true;
		}
		else if (ele->get_id() > 400 && ele->get_id() < 500)
		{
			rohr = true;
		}
		else if (ele->get_id() > 500 && ele->get_id() < 600)
		{
			//Passlaenge keine Laenge sondern Anzahl 
		}
		else if (ele->get_id() > 600 && ele->get_id() < 700)
		{
			rohr = true;
		}
		else if (ele->get_id() == 700)
		{
			rohr = true;
		}
		else if (ele->get_id() == 701)
		{
			rohr = true;
		}
		else if (ele->get_id() >= 702 && ele->get_id() < 800)
		{
			rohr = true;
		}
		if (ele->isEleVK3Rohr() || rohr)
		{
			txt.Format("%.3f", (double)ele->get_laenge()/1000);
			get_file().appendCol(txt,						rows[1], "right");		
		}
		
		else
		{
			get_file().appendCol(ele->get_anzahl(),	rows[1]);		
		}

		get_file().appendCol(ele->get_kurztext(),	rows[2]);
	
		get_file().appendCol(ele->get_dn(),				rows[3], "center");
		get_file().appendCol(ele->get_anschlussDN(),	rows[4], "center");
		
		// WAP	
		
		if (ele->isEleVK3Rohr() || rohr)	
			get_file().appendCol(ele->get_festPreis() * (double)ele->get_laenge()/1000, rows[5]);
		else
			get_file().appendCol(ele->get_festPreis() * (double)ele->get_anzahl(), rows[5]);
		

		get_file().endRow();
	}

	get_file().startRow();
	str = getStammdaten().getText("IDS_GESAMT").c_str();
	txt.Format("<b>%s</b>", str);
	get_file().appendCol(txt,		rows[0]);
	get_file().appendCol("",		rows[1]);
	get_file().appendCol("",		rows[2]);
	get_file().appendCol("",		rows[3], "center");		
	get_file().appendCol("",		rows[4], "center");		
	txt.Format("<b>%.2f</b>", elementepreis);
	get_file().appendCol(txt,		rows[5], "right");
	get_file().endRow();
	get_file().endTable();

	//Speicher freigeben
	tempVk3ListeLocal.deleteAllElemente();
}
