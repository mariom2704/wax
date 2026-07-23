
#include "stdafx.h"
#include "resource.h"

#ifdef __WAX_FUER_DRITTE__		
#else
	#include "GpBarcode.h"		
#endif


		#include "JScriptDruck.h"




CJScriptPrinting::CJScriptPrinting():m_Seperator("#####"),m_SeperatorAnzahl("+++++"),m_JScriptDoubleblank(":&#9;"), m_SeperatorJpg("|||||"),m_HochformatMarker("*****")
{}



CJScriptPrinting::~CJScriptPrinting()
{}



void CJScriptPrinting::Initialize()
{
	m_strGraficLinks.Empty();

	for(UINT i=0; i<55; i++)
	{
		m_strHeader.push_back(m_Seperator);
	}	
}


void CJScriptPrinting::write_header(Werkauftrag* pWerkauftrag, string strListname)
{
	
	CString listname = strListname.c_str();
	CString wa, str; 

	if (WaxSys::isAV())
	{
		str = getStammdaten().getText("IDS_VON_WA").c_str();
		wa.Format(str, pWerkauftrag->get_status().get_wanr(), 
					   pWerkauftrag->get_status().get_mbtermin().Format(VAR_DATEVALUEONLY));
	}
	else
	{
		if(pWerkauftrag->get_leitkarte().get_IsChoosenIppeNumbers())
		{
			str = getStammdaten().getText("IDS_VON_IPPEMuK").c_str();
			wa.Format(str, pWerkauftrag->get_leitkarte().get_ippeMatNr(),
					   pWerkauftrag->get_leitkarte().get_ippeKnotenNr());
		}
		else
		{
			str = getStammdaten().getText("IDS_VON_SD").c_str();
			wa.Format(str, pWerkauftrag->get_leitkarte().get_sdAuftragsnr(),
					   pWerkauftrag->get_leitkarte().get_sdPositionsnr());
		}
	}

	listname += wa;
	m_strHeader[1] = listname; 
	
}




std::string CJScriptPrinting::get_tempdirWaxHTML()
{
	/**
	Liefert das temporäre Verzeichnis, mit app kann z.B. der Applikationsname übergeben werden, 
	dann wird das Verzeichnis <TEMP>/app zurückgegeben.
	*/
	std::string app = WAX_BACKSLASH_HTML;
	std::string temp = getenv("TEMP");
	if (!temp.length()) 
		temp = getenv("TMP");
#ifdef _WINDOWS	
	temp += "\\" + app ;
#else
	temp += "/" + app + "/" + "HTML";
#endif
	try
	{	
		bool exist = filesystem::exists(temp);
		if(!exist)
		{
			filesystem::create_directory(temp);
		}


		CFileFind finder;
		CString strAppPath = GetApplicationPath().c_str();
		BOOL bCopyFiles = FALSE;
		if(finder.FindFile(CString(temp.c_str()) + "\\" + WAX_LOADER_FOR_IMAGES_DWFSVG_JS, 0) == TRUE)
		{
			CFileStatus statusTimeTempFileLastModified;
			if (CFile::GetStatus (CString(temp.c_str()) + "\\" + WAX_LOADER_FOR_IMAGES_DWFSVG_JS, statusTimeTempFileLastModified))
			{
			  CTime tTimeTempFileLastModified = statusTimeTempFileLastModified.m_mtime;
			 			  
			  CFileStatus statusTimeServerFileLastModified;
			  
			  CFile::GetStatus (strAppPath + "\\" + WAX_LOADER_FOR_IMAGES_DWFSVG_JS, statusTimeServerFileLastModified);
			  CTime tTimeServerFileLastModified = statusTimeServerFileLastModified.m_mtime;

			  if(tTimeServerFileLastModified != tTimeTempFileLastModified)
			  {
				bCopyFiles = TRUE;
			  }
			}
		}
		else
		{
			bCopyFiles = TRUE;
		}

		if(bCopyFiles)
		{
			CString  strTemp (temp.c_str());
			CString strFileName = WAX_LOADER_FOR_IMAGES_DWFSVG_CSS;
			CopyFile(strAppPath + "\\" + strFileName , CString(temp.c_str()) + "\\" + strFileName, FALSE);
			
			strFileName = WAX_LOADER_FOR_IMAGES_DWFSVG_JS;
			CopyFile(strAppPath + "\\" + strFileName , CString(temp.c_str()) + "\\" + strFileName, FALSE);
			
			strFileName = WAX_VIEWER_FOR_HTML_DWF_AND_SVG_xHTML;
			CopyFile(strAppPath + "\\" + strFileName , CString(temp.c_str()) + "\\" + strFileName, FALSE);
			strFileName = WAX_VIEWER_FOR_HTML_DWF_AND_SVG_HTML;
			CopyFile(strAppPath + "\\" + strFileName , CString(temp.c_str()) + "\\" + strFileName, FALSE);
			
			strFileName = WAX_BLANK_JPG;
			CopyFile(strAppPath + "\\" + strFileName , CString(temp.c_str()) + "\\" + strFileName, FALSE);
		}
	}
	catch(...)
	{
		return "c:\\temp\\";
	}
	return temp;
}


void CJScriptPrinting::insertZeiten(double schweisserei, double pulveranlage)
{
    CString txt;
	m_strShowRows += m_Seperator + "2";
	m_strHeader[46] = m_Seperator + getStammdaten().getText("IDS_SCHWEISSEREI").c_str();
	//mds#432- keine zeiten mehr
	//txt.Format("%.1f h", 0);
	txt="";
	m_strHeader[47]	= m_Seperator + txt;
	m_strHeader[48] = m_Seperator + getStammdaten().getText("IDS_PULVERANLAGE").c_str();
	//txt.Format("%.1f h", 0);
	txt="";
	m_strHeader[49] = m_Seperator + txt;
}





void CJScriptPrinting::insertOberflaeche(const Beschichtung &beschichtung, BOOL zeugnis, BOOL sichtbereich, int iKolliliste)
{
   
	m_strShowRows += m_Seperator + "1" ;
	CString sichtbar, zeugn, strKolliliste, str;
	
	sichtbar.Format("%s", sichtbereich ? "X" : "-");
	zeugn.Format("%s", zeugnis ? "X" : "-");
	strKolliliste.Format("%s", iKolliliste ? "X" : "-");
	
	CString oberfl = beschichtung.get_bezeichnung();

	int ral = beschichtung.get_ral();

	if (oberfl.Find("RAL", 0) == -1 && ral >= 1000)
	{
		oberfl.Format("%s   RAL %i", beschichtung.get_bezeichnung(), ral);
	}
	
	m_strHeader[38] = m_Seperator + getStammdaten().getText("IDS_KOLLILISTE_EPORT").c_str() + m_JScriptDoubleblank;
	m_strHeader[39] = m_Seperator + strKolliliste;
	
	m_strHeader[40] = m_Seperator + getStammdaten().getText("IDS_IM_SICHTBEREICH").c_str() + m_JScriptDoubleblank;
	m_strHeader[41] = m_Seperator + sichtbar;
	
	m_strHeader[42] = m_Seperator + getStammdaten().getText("IDS_ZEUGNIS").c_str() + m_JScriptDoubleblank;
	m_strHeader[43] = m_Seperator + zeugn;

	m_strHeader[44] = m_Seperator + getStammdaten().getText("IDS_OBERFLAECHE").c_str() + m_JScriptDoubleblank;
	m_strHeader[45] = m_Seperator + oberfl;
}



//Fuegt die Rohrqualitaet in eine Druckliste ein
void CJScriptPrinting::insertQuali(int quali, bool twoquali, int dn, int stutzenAnlage2440, int anlagentyp)
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
		
	
	str = getStammdaten().getText("IDS_DIN").c_str();
	m_strHeader[52]	= m_Seperator + str;
	m_strHeader[53]	= m_Seperator + qual;

	CString tmp;
	if (anlagentyp == Gewinderohr::WASSER)		tmp = getStammdaten().getText("IDS_WASSER").c_str();
	else if (anlagentyp == Gewinderohr::SCHAUM)	tmp = getStammdaten().getText("IDS_SCHAUMANLAGE").c_str();
	else if (anlagentyp == Gewinderohr::GAS)	tmp = getStammdaten().getText("IDS_GAS").c_str();
	
	m_strHeader[54] = m_Seperator + tmp;
	m_strShowRows += m_Seperator + "4";
}



void CJScriptPrinting::insertLaengeGewichtBarcode(int maxlaenge, double gewicht, CString kc, int kcPosnr, string ids )
{
	/*
	Erzeugen des abschliessenden Tables mit der groessten Laenge und des Gesamtgewichtes
	*/

	CString laenge, 
		    sgewicht, 
			strLaenge,
			strGewicht;


	laenge.Format("%d", maxlaenge);
	

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

	m_strHeader[33]	= m_Seperator + strGewicht + m_JScriptDoubleblank;
	m_strHeader[34]	= m_Seperator + sgewicht;
	
	
	m_strHeader[35]	= m_Seperator + getStammdaten().getText("IDS_MAX_LAENGE").c_str() + m_JScriptDoubleblank;
	m_strHeader[36]	= m_Seperator + laenge;

	if(	WaxSys::isAV() && !kc.IsEmpty())
	{
		MxString kcstr = MxString("{0}-{1}").arg(kc.GetString()).arg(kcPosnr);
		CString strFile;
		get_file().createBarcode(210, 20, kcstr.getString(), "", &strFile);
		strFile.Replace("\\","/");
		m_strHeader[37] = m_Seperator + strFile;
	}
	else
	{
		CString strFile;
		strFile = CString(GetApplicationPath().c_str()) + _T("\\") + WAX_BLANK_JPG;
		strFile.Replace("\\","/");
		m_strHeader[37] = m_Seperator + strFile;
	}
}



WaxHtmlFile& CJScriptPrinting::get_file()  
{
	return file;
}



void CJScriptPrinting::insert_bemerkung(const std::string& bem, const std::string& ueberschrift, const std::string& stdbemerkung)
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

	m_strShowRows += m_Seperator + "3";
	m_strHeader[50] = m_Seperator + ueberschrift.c_str();
	txt = "<b>" + boost::algorithm::replace_all_copy(txt, "\r\n", " <br/>") + "</b>";
	m_strHeader[51]	= m_Seperator + txt.c_str();
}



void CJScriptPrinting::insertLeitkarte(Leitkarte& leitkarte, Status& status, const CString& name, const CString & ziel, bool umbruch)
{
	
	CString freigabedatum, wanrstr(m_Seperator), sdauftragsnrstr(m_Seperator), ippeMatNrstr(m_Seperator), sdmaterialnrstr(m_Seperator),
		sdpositionsnrstr(m_Seperator), ippeKnotenNrstr(m_Seperator),  prjnl(m_Seperator), str(m_Seperator),
		out(m_Seperator), kcauftragsnrstr, kcpositionsnrstr;

	// Datumstrings
	if (status.get_freigabedatum().m_dt > 0.0)
	{
		freigabedatum = status.get_freigabedatum().Format(_T("%d.%m.%Y %H:%M"));
	}

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

	if(leitkarte.get_IsChoosenIppeNumbers())
	{
		ippeKnotenNrstr = leitkarte.get_ippeKnotenNr();
	}
	else
	{
		sdpositionsnrstr.Format("%d", leitkarte.get_sdPositionsnr());
	}

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
	};
	
	CString strTemp = WaxSys::GetTempDir();
	strTemp.Replace("\\", "/");
	m_strHeader[2]  = m_Seperator + strTemp + _T("Minimax.jpg");
	m_strHeader[3]  = m_Seperator + name;
	m_strHeader[4]  = m_Seperator + ziel;
	m_strHeader[5]  = m_Seperator + getStammdaten().getText("IDS_WANR").c_str() + m_JScriptDoubleblank;
	m_strHeader[6]  = m_Seperator + wanrstr;
	m_strHeader[7]  = m_Seperator + getStammdaten().getText("IDS_SACHBEARBEITER").c_str() + m_JScriptDoubleblank;
	m_strHeader[8]  = m_Seperator + status.get_sachbearbeiterSegment();
	m_strHeader[9]  = m_Seperator + getStammdaten().getText("IDS_BESTELLER").c_str() + m_JScriptDoubleblank;
	m_strHeader[10]  = m_Seperator + leitkarte.get_sachbearbeiterNL();
	
	m_strHeader[11] = m_Seperator + getStammdaten().getText("IDS_BEARBEITUNGSDATUM").c_str() + m_JScriptDoubleblank;
	m_strHeader[12] = m_Seperator + bearbeitungsdat;
	m_strHeader[13] = m_Seperator + getStammdaten().getText("IDS_FREIGABE_DATUM").c_str() + m_JScriptDoubleblank;
	m_strHeader[14] = m_Seperator + freigabedatum;
	
	m_strHeader[15] = m_Seperator + getStammdaten().getText("IDS_BAU").c_str() + m_JScriptDoubleblank;
	m_strHeader[16] = m_Seperator + leitkarte.get_prj_auftragsbez() + " - " + leitkarte.get_prj_bauAbschnitt(); 
	m_strHeader[17] = m_Seperator + getStammdaten().getText("IDS_PLZ").c_str() + m_JScriptDoubleblank;
	m_strHeader[18] = m_Seperator + leitkarte.get_prj_bauPlz();

	m_strHeader[19] = m_Seperator + getStammdaten().getText("IDS_ORT").c_str() + m_JScriptDoubleblank;
	m_strHeader[20] = m_Seperator + leitkarte.get_prj_bauOrt();
	
	CString strLieferung = getStammdaten().getText("IDS_LIEFERUNG_AM").c_str();
	strLieferung.Replace("/", "\\");
	m_strHeader[21] = m_Seperator + strLieferung + m_JScriptDoubleblank;
	panr = "<br/>" + panr;
	if (WaxSys::isAV())
	{
		m_strHeader[22] = m_Seperator + leitkarte.get_wunschliefertermin().Format(VAR_DATEVALUEONLY) + panr;
	}
	else
	{	
		m_strHeader[22] = m_Seperator + getStammdaten().getText("IDS_LIEFER_UEBER_SAP").c_str() + panr;
	}
	m_strHeader[23] = m_Seperator + getStammdaten().getText("IDS_NL_KURZ").c_str() + m_JScriptDoubleblank;
	m_strHeader[24] = m_Seperator + prjnl;

	if(leitkarte.get_IsChoosenIppeNumbers())
	{
		m_strHeader[25] = m_Seperator + getStammdaten().getText("IDS_IPPE_MATNR").c_str() + m_JScriptDoubleblank;
		m_strHeader[26] = m_Seperator + ippeMatNrstr;// + " - " + sdpositionsnrstr;
	}
	else
	{
		m_strHeader[25] = m_Seperator + getStammdaten().getText("IDS_SD_AUFTRAG").c_str() + m_JScriptDoubleblank;
		m_strHeader[26] = m_Seperator + sdauftragsnrstr + " - " + sdpositionsnrstr;
	}
	m_strHeader[27] = m_Seperator + getStammdaten().getText("IDS_SD_MATNR").c_str() + m_JScriptDoubleblank;
	m_strHeader[28] = m_Seperator + sdmaterialnrstr; 
	
	m_strHeader[29]	= m_Seperator + getStammdaten().getText("IDS_PSP_ELE").c_str() + m_JScriptDoubleblank;
	if(leitkarte.get_prj_COauftrag().GetLength() > 1)
	{
		m_strHeader[30] = m_Seperator + getStammdaten().getText("IDS_CO").c_str() + leitkarte.get_prj_COauftrag();
	}
	else
	{
		m_strHeader[30] = m_Seperator + leitkarte.getPSPElement();
	}

	m_strHeader[31]	= m_Seperator + getStammdaten().getText("IDS_KC_AUFTRAG").c_str() + m_JScriptDoubleblank;
	if (!kcauftragsnrstr.IsEmpty())
	{
		m_strHeader[32]	= m_Seperator + kcauftragsnrstr + " - " + kcpositionsnrstr;
	}
}



void CJScriptPrinting::insertDwfOrSvgLink( const string& fname)
{
	
	CString strFile(fname.c_str());
	strFile.Replace("\\","/");

	size_t found;
	found = fname.find(".svg");
	if (found != string::npos)
	{
		int iwidth = 0, iheight = 0;
		get_file().get_size_svg(iwidth, iheight, fname);
		if( iwidth < iheight - 100)
		{
			strFile +=  m_HochformatMarker + _T("1");// Hochformat
		}
		else
		{
			strFile += m_HochformatMarker + _T("0"); // Breitformat
		}
	}
	if(m_strGraficLinks.IsEmpty())
	{
		m_strGraficLinks = strFile;
	}
	else
	{
		m_strGraficLinks += m_Seperator + strFile;
	}
}



void CJScriptPrinting::createBlankJPG(Werkauftrag* pWerk)
{
		CString strFile;
		strFile = CString( GetApplicationPath().c_str()) + _T("\\") + WAX_BLANK_JPG;
		strFile.Replace("\\","/");
		m_strGraficLinks += m_SeperatorJpg + strFile;
		int number = pWerk->get_anzahl();
		CString temp;
		temp.Format("%d", number);
		m_strGraficLinks += m_SeperatorAnzahl + temp;
}



string CJScriptPrinting::createBarcode(Werkauftrag* pWerk, int breite, int hoehe, string txt, string align)
{
	string st;

	#ifdef __WAX_FUER_DRITTE__
		UNREFERENCED_PARAMETER(breite);
		UNREFERENCED_PARAMETER(hoehe);
		UNREFERENCED_PARAMETER(txt);
		UNREFERENCED_PARAMETER(align);
	#else
		string fn = WaxSys::GetTempFilename("jpg");
		
		m_strGraficLinks += m_SeperatorJpg + fn.c_str();

		WaxSys::add_tmpfile(fn.c_str());

		//Die Auflösung der Jpg Grafik muss größer als die Größe in der HTML Datei sein!
		GpBarcode barcode;
		barcode.getGrafik(txt, false, breite * 2, hoehe * 2, GpBarcode::winkel0, fn); 

		st = MxString("{0}{1}{2}{3}{4}{5}{6}{7}{8}").arg("<p align='").arg(align.c_str()).arg("'><img src='")
	 												.arg(fn.c_str())
													.arg("' width=\"").arg(breite)
													.arg("\" height=\"").arg(hoehe)
													.arg("\">").getString();
		
		int number = pWerk->get_anzahl();
		CString temp;
		temp.Format("%d", number);

		m_strGraficLinks += m_SeperatorAnzahl + temp;

		if(pWerk->get_leitkarte().get_zulassungsnummer()==2)
		{
			int zulNr=pWerk->get_leitkarte().get_zulassungsnummer();
			pWerk->set_zulassungsstelle(zulNr);
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pWerk);

			int rohrQuali=rohr->get_rohrQuali();
			DNzuAussendurchmesser dntoad;
			double aussendurchmesser = dntoad.getAussendurchmesser(rohr->get_dnvl());

			fn = WaxSys::GetTempFilename("jpg");
			m_strGraficLinks += m_SeperatorJpg + fn.c_str();

			WaxSys::add_tmpfile(fn.c_str());

			string bcText=pWerk->get_barcodeStringForGuidLineStart(rohrQuali,aussendurchmesser, true);


			//Die Auflösung der Jpg Grafik muss größer als die Größe in der HTML Datei sein!
			GpBarcode barcode;
			barcode.getGrafik("Test", false, breite * 2, hoehe * 2, GpBarcode::winkel0, fn); 

			st = MxString("{0}{1}{2}{3}{4}{5}{6}{7}{8}").arg("<p align='").arg(align.c_str()).arg("'><img src='")
	 													.arg(fn.c_str())
														.arg("' width=\"").arg(breite)
														.arg("\" height=\"").arg(hoehe)
														.arg("\">").getString();


			fn = WaxSys::GetTempFilename("jpg");
			m_strGraficLinks += m_SeperatorJpg + fn.c_str();

			WaxSys::add_tmpfile(fn.c_str());
			
			int wanr= pWerk->get_status().get_wanr();
			bcText=pWerk->get_barcodeStringForGuidLineEnd(rohrQuali,aussendurchmesser,wanr);


			//Die Auflösung der Jpg Grafik muss größer als die Größe in der HTML Datei sein!
			barcode.getGrafik("Test", false, breite * 2, hoehe * 2, GpBarcode::winkel0, fn); 

			st = MxString("{0}{1}{2}{3}{4}{5}{6}{7}{8}").arg("<p align='").arg(align.c_str()).arg("'><img src='")
	 													.arg(fn.c_str())
														.arg("' width=\"").arg(breite)
														.arg("\" height=\"").arg(hoehe)
														.arg("\">").getString();


		}

		#endif
	
	return st;
}


