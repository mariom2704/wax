#include "stdafx.h"


#include "drucklistestationsverteiler.h"

#include <mxutil/strloader.h>
using namespace MxUtil;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;


DrucklisteStationsverteiler::DrucklisteStationsverteiler(HtmlDruck& file, WaxPart* pt) : Druckliste(file, pt)
{
	HRSRC rclog;
	CString logo;
	rclog = ::FindResource(NULL, (LPSTR)"STATIONSVERTEILER", (LPSTR)"JPG");
	logo = WaxSys::GetTempDir() + "StatVert_Abg.jpg";
	loadResource(logo, rclog);
}


DrucklisteStationsverteiler::~DrucklisteStationsverteiler()
{
}

bool DrucklisteStationsverteiler::create()
{
	print_header();

	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{		
		const Stationsverteiler* stat = dynamic_cast<const Stationsverteiler*>(*it);
		if (!stat)
			continue;

		if (!print_list(stat))
			return false;
		if (*it != auftraege.back())
			html.get_file().insertTrennlinie();
	}
	return true;
}

void DrucklisteStationsverteiler::print_header()
{
	const Werkauftrag* const wa = auftraege.front();

	if (!wa)
		return;
	html.insert_bemerkung((LPCSTR)wa->get_bemerkung(), getStammdaten().getText("IDS_BEMERKUNG").c_str(), (LPCSTR)wa->get_stdbemerkung());
}

bool DrucklisteStationsverteiler::print_list(const Stationsverteiler* stat)
{
	const CString fontFace = "face='Verdana, Arial, Helvetica, sans-serif'";
	int i = 0;
	CString wert, str, out, txt;

	WaxHtmlFile &w = html.get_file();

	const Stationsverteiler* stationsverteiler = stat;
	
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
    w.WriteString("<tr>");
	// PosNr
	w.WriteString("<tr>");
	
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%%'><b><font "+fontFace+" size='2'>"+stationsverteiler->get_posNr()+"</font></b></td>");
	
	// Anfang
	str = getStammdaten().getText("IDS_ANFANG").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
	w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+Element(stat->correctElemID(stat->get_elementlEnde(), stat->get_dnvl())).get_bezeichnung()+"</font></b></td>");
	w.WriteString("</tr>");

	// Anzahl
	w.WriteString("<tr>");
	wert.Format("%d", stationsverteiler->get_anzahl());
	str = getStammdaten().getText("IDS_ANZAHL").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");

	// Ende
	str = getStammdaten().getText("IDS_ENDE").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+Element(stat->correctElemID(stat->get_elementrEnde(), stat->get_dnvl())).get_bezeichnung()+"</font></b></td>");
	w.WriteString("</tr>");
	
	// Druckstufe
	wert.Format("%d", stationsverteiler->get_rohrDruckstufe());
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_DRUCKSTUFE").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>PN "+wert+"</font></b></td>");

	// Dnvl
	wert.Format("%d", stationsverteiler->get_dnvl());
	str = getStammdaten().getText("IDS_NENNW_DN").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	w.WriteString("</tr>");

	// Laenge
	wert.Format("%d", stationsverteiler->get_laenge());
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");

    // Laenge Stutzen
	wert.Format("%d", stationsverteiler->get_elementLaenge());
	str = getStammdaten().getText("IDS_ROHRSTUTZENLAENGE").c_str();
	out.Format("<td width='15%%'><font "+fontFace+" size='2'>%s</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='35%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	w.WriteString("</tr>");

	// Abgang unten ID
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABGANG_UNTEN").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s ID</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+Element(stat->get_extraAbgID()).get_bezeichnung()+"</font></b></td>");
	
	// Abgang oben ID
	str = getStammdaten().getText("IDS_ABGANG_OBEN").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s ID</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+Element(stat->get_extraAbgObenID()).get_bezeichnung()+"</font></b></td>");
	w.WriteString("</tr>");

    // Abgang unten Abstand
	if (stationsverteiler->get_extraAbgID() == Element::LEER)
		wert = "-";
	else 
		wert.Format("%d",stationsverteiler->get_extraAbgAbsAbst());
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABGANG_UNTEN").c_str();
	txt = getStammdaten().getText("IDS_ABSTAND").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s %s</font></td>", str, txt);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	
	// Abgang oben Abstand
	if (stationsverteiler->get_extraAbgObenID() == Element::LEER)
		wert = "-";
	else 
		wert.Format("%d",stationsverteiler->get_extraAbgObenAbsAbst());
	str = getStammdaten().getText("IDS_ABGANG_OBEN").c_str();
	txt = getStammdaten().getText("IDS_ABSTAND").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s %s</font></td>", str, txt);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	w.WriteString("</tr>");

    // Abgang unten DN
	if (stationsverteiler->get_extraAbgID() == Element::LEER)
		wert = "-";
	else 
		wert.Format("%d",stationsverteiler->get_extraAbgDN());
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABGANG_UNTEN").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s DN</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	
	// Abgang oben DN
	if (stationsverteiler->get_extraAbgObenID() == Element::LEER)
		wert = "-";
	else 
		wert.Format("%d",stationsverteiler->get_extraAbgObenDN());
	str = getStammdaten().getText("IDS_ABGANG_OBEN").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s DN</font></td>", str);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	w.WriteString("</tr>");

    // Abgang unten Laenge
	if (stationsverteiler->get_extraAbgID() == Element::LEER || stationsverteiler->get_extraAbgID() == MUFFE)
		wert = "-";
	else 
		wert.Format("%d",stationsverteiler->get_extraAbgLaenge());
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABGANG_UNTEN").c_str();
	txt = getStammdaten().getText("IDS_LAENGE_UML").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s %s</font></td>", str, txt);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	
	// Abgang oben Laenge
	if (stationsverteiler->get_extraAbgObenID() == Element::LEER || stationsverteiler->get_extraAbgObenID() == MUFFE)
		wert = "-";
	else 
		wert.Format("%d",stationsverteiler->get_extraAbgObenLaenge());
	str = getStammdaten().getText("IDS_ABGANG_OBEN").c_str();
	txt = getStammdaten().getText("IDS_LAENGE_UML").c_str();
	out.Format("<td width='18%%'><font "+fontFace+" size='2'>%s %s</font></td>", str, txt);
    w.WriteString(out);
    w.WriteString("<td width='32%'><b><font "+fontFace+" size='2'>"+wert+"</font></b></td>");
	w.WriteString("</tr>");
	w.WriteString("</table>");

	// Tabelle Nennweiten
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
	w.WriteString("<tr>");
	//str = getStammdaten().getText("IDS_DN_ABGAENGE_UML").c_str();
	out.Format("<td rowspan='2' width='16%%'><font "+fontFace+" size='2'>DN</font><font "+fontFace+" size='2'></font></td>");//, str);
    w.WriteString(out);
    for (i = 1; i <= 6; i++) 
	{
		wert.Format("%d", i);
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'>DN"+wert+"</font></div>");
		w.WriteString("</td>");
	}
    w.WriteString("</tr>");
	w.WriteString("<tr>");
    for (i = 0; i < 6; i++)
	{
		wert = "-";
		if (stationsverteiler->get_elementDN(i)>0)
			wert.Format("%d", stationsverteiler->get_elementDN(i));
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>"+wert+"</b></font></div>");
		w.WriteString("</td>");
    }
	w.WriteString("</tr>");
	w.WriteString("</table>");

	// Tabelle Abstaende
	w.WriteString("<table width='100%' border='1' cellpadding='2' cellspacing='0'>");
	w.WriteString("<tr>");
	str = getStammdaten().getText("IDS_ABSTAENDE_UML").c_str();
	out.Format("<td rowspan='2' width='16%%'><font "+fontFace+" size='2'>%s</font><font "+fontFace+" size='2'></font></td>", str);
    w.WriteString(out);
    for (i = 1; i <= 6; i++) 
	{
		wert.Format("%d", i);
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'>L"+wert+"</font></div>");
		w.WriteString("</td>");
	}
    w.WriteString("</tr>");
	w.WriteString("<tr>");
    for (i = 0; i < 6; i++)
	{
		wert = "-";
		if (stationsverteiler->get_elementRelAbst(i)>0)
			wert.Format("%d", stationsverteiler->get_elementRelAbst(i));
		w.WriteString("<td width='14%'>");
		w.WriteString("<div align='center'><font "+fontFace+" size='2'><b>"+wert+"</b></font></div>");
		w.WriteString("</td>");
    }
	w.WriteString("</tr>");
	w.WriteString("</table>");
	
	w.WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n"
				"<P ALIGN='CENTER'><IMG SRC='StatVert_Abg.jpg' WIDTH=600 HEIGHT=200 ></TD>\n</tr>");

	return true;
}


