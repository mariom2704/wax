#include "stdafx.h"


#include "DrucklisteCOIIMx.h"

#include <mxutil/strloader.h>
using namespace MxUtil;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;


DrucklisteCOIIMx::DrucklisteCOIIMx(HtmlDruck& file, WaxPart* pt) : Druckliste(file, pt)
{
	HRSRC rclog;
	CString logo;
	rclog = ::FindResource(NULL, (LPSTR)"COIIMXVERTEILER" , (LPSTR)"JPG");
	logo = WaxSys::GetTempDir() + "COIIVert_Abg.jpg";
	loadResource(logo, rclog);
}


DrucklisteCOIIMx::~DrucklisteCOIIMx()
{
}

bool DrucklisteCOIIMx::create()
{
	print_header();

	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const COIIVerteiler* pConstStat = dynamic_cast<const COIIVerteiler*>(*it);

	
		if (!pConstStat)
			continue;

		COIIVerteiler* stat = (COIIVerteiler*)pConstStat; //wegen const cast

		if (!print_list(stat))
			return false;
		if (*it != auftraege.back())
			html.get_file().insertTrennlinie();
	}
	return true;
}

void DrucklisteCOIIMx::print_header()
{
	const Werkauftrag* const wa = auftraege.front();

	if (!wa)
		return;
	html.insert_bemerkung((LPCSTR)wa->get_bemerkung(), getStammdaten().getText("IDS_BEMERKUNG").c_str(), (LPCSTR)wa->get_stdbemerkung());
}

bool DrucklisteCOIIMx::print_list( COIIVerteiler* pCOIIMx)
{
	const CString fontFace = "face='Verdana, Arial, Helvetica, sans-serif'";
	
	CString wert, str, out, txt;

	WaxHtmlFile &w = html.get_file();

	COIIVerteiler* pCOIIMxLocal = pCOIIMx;
	
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
    w.WriteString("<tr>");
	// PosNr
	w.WriteString("<tr>");
	
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%%'><b><font "+fontFace+" size='2'>"+ pCOIIMxLocal->get_posNr() + "</font></b></td>");
	
	// Anfang
	str = getStammdaten().getText("IDS_LINKS").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
	
	CString strBezeichnung = Element(pCOIIMxLocal->get_rohrAnfang(), pCOIIMxLocal->get_dnvl()).get_bezeichnung();
	if(pCOIIMxLocal->get_rohrAnfang() == BLINDFLANSCH_COII)
	{
		strBezeichnung += CString("  ") + Element(FLANSCH_COII, pCOIIMxLocal->get_dnvl()).get_bezeichnung();
	}
	w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+ strBezeichnung +"</font></b></td>");
	w.WriteString("</tr>");

	// Anzahl
	w.WriteString("<tr>");
	wert.Format("%d", pCOIIMxLocal->get_anzahl());
	str = getStammdaten().getText("IDS_ANZAHL").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");

	strBezeichnung = Element(pCOIIMxLocal->get_rohrEnde(), pCOIIMxLocal->get_dnvl()).get_bezeichnung();
	if(pCOIIMxLocal->get_rohrEnde() == BLINDFLANSCH_COII)
	{
		strBezeichnung =  Element(FLANSCH_COII, pCOIIMxLocal->get_dnvl()).get_bezeichnung()+ CString("  ") + strBezeichnung;
	}
	// Ende
	str = getStammdaten().getText("IDS_RECHTS").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+ strBezeichnung +"</font></b></td>");
	w.WriteString("</tr>");
	
	// Druckstufe
	wert.Format("%d", pCOIIMxLocal->get_rohrDruckstufe());
	w.WriteString("<tr>");
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>Endschalter</font></td>", str);
    w.WriteString(out);
	if(!pCOIIMxLocal->GetElementEndschalterTyp())
	{
		w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>-</font></b></td>");
	}
	else
	{
		w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+Element(pCOIIMxLocal->GetElementEndschalterTyp(), 0).get_bezeichnung()+"</font></b></td>");
	}
	// Dnvl
	wert.Format("%d", pCOIIMxLocal->get_dnvl());
	str = getStammdaten().getText("IDS_NENNW_DN").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	w.WriteString("</tr>");

	// Laenge
	wert.Format("%d", pCOIIMxLocal->get_laenge());
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");

    // Laenge Stutzen
	wert.Format("%d", pCOIIMxLocal->get_laenge());
	str = "-";// getStammdaten().getText("IDS_ROHRSTUTZENLAENGE").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>-</font></b></td>");
	w.WriteString("</tr>");


	// Sicherheitsventil
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_SICHERHEITS_VENTIL").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
	
	if(!pCOIIMxLocal->GetAbgangSicherheitsventilVorhanden())
	{
		w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>-</font></b></td>");
	}
	else
	{
		if(pCOIIMxLocal->GetAbgangSicherheitsventilRechts())
		{
			str = getStammdaten().getText("IDS_RECHTS").c_str();
			str = CString( " (") + str + CString( " )");
		}
		else
		{
			str = getStammdaten().getText("IDS_LINKS").c_str();
			str = CString( " (") + str + CString( ")");
		}
		w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+Element(MUFFE_COII).get_bezeichnung()+ str +"</font></b></td>");
	}

	// Entleerung
	str = getStammdaten().getText("IDS_ENTLEERUNG").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
	if(!pCOIIMxLocal->GetAbgangEntleerungVorhanden())
	{
		w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>-</font></b></td>");
	}
	else
	{
		if(pCOIIMxLocal->GetAbgangEntleerungRechts())
		{
			str = getStammdaten().getText("IDS_RECHTS").c_str();
			str = CString( " (") + str+  CString( ")");
		}
		else
		{
			str = getStammdaten().getText("IDS_LINKS").c_str();
			str = CString( " (") + str + CString( ")");
		}
		w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+Element(MUFFE_COII).get_bezeichnung()+ str +"</font></b></td>");
	}
	w.WriteString("</tr>");

	
	populateNennweiten(w, pCOIIMxLocal);
	
	w.WriteString("</tr>");
	w.WriteString("</table>");

	w.WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n"
				"<P ALIGN='CENTER'><IMG SRC='COIIVert_Abg.jpg' WIDTH='80%'  ></TD>\n</tr>");

	return true;
}



void DrucklisteCOIIMx::populateNennweiten(WaxHtmlFile& w, COIIVerteiler* pCOIIMxLocal )
{
	const CString fontFace = "face='Verdana, Arial, Helvetica, sans-serif'";
	CString wert, str, out, txt;
	
	// Tabelle Nennweiten
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
	w.WriteString("<tr>");
	//str = getStammdaten().getText("IDS_DN_ABGAENGE_UML").c_str();
	out.Format("<td width='16%%'><font "+fontFace+" size='2'>DN</font><font "+fontFace+" size='2'></font></td>");//, str);
    w.WriteString("<br><br>");
	w.WriteString(out);
	
	
	int i = 0;

    for (i = 0; i < pCOIIMxLocal->get_AnzahlAnschluesse(); i++)
	{
		wert = "-";
		if (pCOIIMxLocal->get_elementDN(i)>0)
			wert.Format("%d", pCOIIMxLocal->get_elementDN(i));
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>"+wert+"</b></font></div>");
		w.WriteString("</td>");
    }
	w.WriteString("</tr>");
	
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABSTAENDE_UML").c_str();
	out.Format("<td width='16%%'><font "+fontFace+" size='2'>%s</font><font "+fontFace+" size='2'></font></td>", "Typ Abgang");
    w.WriteString(out);

    for (i = 0; i < pCOIIMxLocal->get_AnzahlAnschluesse(); i++)
	{
		wert = "-";
		if (pCOIIMxLocal->get_elementRelAbst(i)>0)
			wert.Format("%d", pCOIIMxLocal->GetElementTypAbgangEnde(i));
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>"+Element(pCOIIMxLocal->GetElementTypAbgangEnde(i), pCOIIMxLocal->get_dnvl()).get_bezeichnung() +"</b></font></div>");
		w.WriteString("</td>");
    }
	w.WriteString("</tr>");	

	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABSTAENDE_UML").c_str();
	out.Format("<td width='16%%'><font "+fontFace+" size='2'>%s</font><font "+fontFace+" size='2'></font></td>", "Typ Gegenflansch");
    w.WriteString(out);

    for (i = 0; i < pCOIIMxLocal->get_AnzahlAnschluesse(); i++)
	{
		wert = "-";
		int iwert = 0;
		
		w.WriteString("<td width='14%'>");
		switch(pCOIIMxLocal->GetElementTypGegenFLansch(i))
		{
			case GEGENFLANSCH_COII_ABGANG:
			iwert = FLANSCH_COII;
			w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>FL</b></font></div>");
			break;

			case GEGENFLANSCH_GEWINDE_COII:
			iwert = FLANSCH_COII;
			w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>FL+ STUTZEN+GEW</b></font></div>");
			break;

			case GEGENFLANSCH_GEWINDE_MUFFE_COII:
			iwert = FLANSCH_COII;
			w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>FL+STUTZEN+ GEW+MUFFE</b></font></div>");
			break;


			default:
			w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>-</b></font></div>");
			break;

		}
		w.WriteString("</td>");
    }
	w.WriteString("</tr>");	

	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABSTAENDE_UML").c_str();
	out.Format("<td width='16%%'><font "+fontFace+" size='2'>%s</font><font "+fontFace+" size='2'></font></td>", "Länge Gegenflansch");
    w.WriteString(out);

    for (i = 0; i < pCOIIMxLocal->get_AnzahlAnschluesse(); i++)
	{
		wert = "-";
		if (pCOIIMxLocal->get_elementRelAbst(i)>0)
			wert.Format("%d", pCOIIMxLocal->GetElementLaengeGegenFLansche(i));
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>"+wert+"</b></font></div>");
		w.WriteString("</td>");
    }
	w.WriteString("</tr>");	
}
